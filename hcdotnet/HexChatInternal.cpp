/*
.NET Plugin Interface for HexChat
Copyright (C) 2015 Ryan Schmidt

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#include "stdafx.h"
#include <mscoree.h>
#include <metahost.h>
#include <Shlwapi.h>
#include "hcdotnet.h"

#pragma comment(lib, "mscoree.lib")
#pragma comment(lib, "Shlwapi.lib")

// NOTE: This uses deprecated APIs but the replacements are part of Windows 8
// As we support Vista and above for now, we must use the deprecated variants

#define STORED_EXCEPTIONS 100
#define ASSEMBLY_PATH_KEY "HCDOTNET_ASSEMBLY_PATH"
#define TYPE_NAME_KEY "HCDOTNET_TYPENAME"

using namespace System;
using namespace System::Collections::Generic;
using namespace System::Reflection;
using namespace System::IO;
using namespace System::Text;
using namespace System::Xml;
using namespace System::Runtime::Serialization;
using namespace msclr::interop;

namespace HexChatDotNet {
	static HexChatInternal::HexChatInternal() {
		_loadedPlugins = gcnew List<Tuple<AppDomain^, HexChatPlugin^>^>();
		_pluginMap = gcnew Dictionary<String^, Tuple<String^, IntPtr>^>();
		_exceptions = gcnew Dictionary<String^, ExceptionData^>();
		_exceptionKeys = gcnew LinkedList<String^>();
		_saveExceptionDetails = false;

		AppDomain::CurrentDomain->AssemblyResolve += gcnew ResolveEventHandler(&HexChatDotNet::HexChatInternal::LoadDependencies);

		// Locate our default AppDomain using COM
		marshal_context ctx;
		const wchar_t* version = ctx.marshal_as<const wchar_t*>(Assembly::GetExecutingAssembly()->ImageRuntimeVersion);
		
		ICLRMetaHost* metaHost = nullptr;
		ICLRRuntimeInfo* info = nullptr;
		ICLRRuntimeHost* host = nullptr;
		int loaded = 0;

		if (S_OK != CLRCreateInstance(CLSID_CLRMetaHost, IID_ICLRMetaHost, reinterpret_cast<void**>(&metaHost))) {
			goto cleanup;
		}

		if (S_OK != metaHost->GetRuntime(version, IID_ICLRRuntimeInfo, reinterpret_cast<void**>(&info))) {
			goto cleanup;
		}

		if (S_OK != info->IsLoaded(GetCurrentProcess(), &loaded)) {
			goto cleanup;
		}

		if (!loaded) {
			goto cleanup;
		}

		if (S_OK != info->GetInterface(CLSID_CLRRuntimeHost, IID_ICLRRuntimeHost, reinterpret_cast<void**>(&host))) {
			goto cleanup;
		}

		// we have a host!
		_host = host;
		_saveExceptionDetails = true;

	cleanup:
		if (metaHost != nullptr) {
			metaHost->Release();
		}

		if (info != nullptr) {
			info->Release();
		}
	}

	String^ HexChatInternal::GetInfo(const char* id) {
		return gcnew String(hexchat_get_info(ph, id));
	}

	void HexChatInternal::ReportException(Exception^ e) {
		if (_saveExceptionDetails) {
			ExceptionData^ data = gcnew ExceptionData(e);
			DataContractSerializer^ serializer = gcnew DataContractSerializer(ExceptionData::typeid);
			StringWriter^ writer = gcnew StringWriter();
			XmlTextWriter^ xml = gcnew XmlTextWriter(writer);
			serializer->WriteObject(xml, data);

			marshal_context ctx;
			const wchar_t* location = ctx.marshal_as<const wchar_t*>(Assembly::GetExecutingAssembly()->Location);
			const wchar_t* arg = ctx.marshal_as<const wchar_t*>(writer->ToString());
			int retval = 0;
			HRESULT res = _host->ExecuteInDefaultAppDomain(location, L"HexChatDotNet.HexChatInternal", L"ReportExceptionInternal", arg, reinterpret_cast<DWORD*>(&retval));

			if (res != S_OK) {
				_saveExceptionDetails = false;
				_host->Release();
				_host = nullptr;
				ReportException(e);
			}
		} else {
			StringBuilder sb;

			sb.AppendFormat("Uncaught {0} in plugin: {1}", e->GetType()->Name, e->Message != nullptr ? e->Message : String::Empty);
			sb.AppendLine();
			sb.Append(e->StackTrace);

			while (e->InnerException != nullptr) {
				e = e->InnerException;
				sb.AppendLine();
				sb.AppendFormat("Inner Exception {0}: {1}", e->GetType()->Name, e->Message != nullptr ? e->Message : String::Empty);
				sb.AppendLine();
				sb.Append(e->StackTrace);
			}

			HexChat::Print(sb.ToString());
		}
	}

	int HexChatInternal::ReportExceptionInternal(String^ serialized) {
		DataContractSerializer^ serializer = gcnew DataContractSerializer(ExceptionData::typeid);
		ExceptionData^ data = safe_cast<ExceptionData^>(serializer->ReadObject(gcnew XmlTextReader(gcnew StringReader(serialized)), true));
		String^ errid = RegisterException(data);

		HexChat::Print("Uncaught {0} in plugin: {1}", data->Type, data->Message);
		HexChat::Print("Execute \"/dotnet errinfo {0}\" for more detailed information.", errid);

		return 0;
	}

	String^ HexChatInternal::RegisterException(ExceptionData^ e) {
		// TODO: allow number of stored exceptions to be user-configurable
		while (_exceptions->Count >= STORED_EXCEPTIONS) {
			// prune the N oldest exceptions until we have STORED_EXCEPTIONS - 1 stored
			_exceptions->Remove(_exceptionKeys->First->Value);
			_exceptionKeys->RemoveFirst();
		}

		// Create a new exception key by getting a random filename
		// (this means in the future we can also save exception info to disk if that is desired)
		String^ key;

		do {
			key = Path::GetRandomFileName()->Substring(0, 8);
		} while (_exceptions->ContainsKey(key));

		_exceptionKeys->AddLast(key);
		_exceptions[key] = e;

		return key;
	}

	String^ HexChatInternal::RegisterException(Exception^ e) {
		return RegisterException(gcnew ExceptionData(e));
	}

	ExceptionData^ HexChatInternal::GetException(String^ key) {
		ExceptionData^ e;
		if (!_exceptions->TryGetValue(key, e)) {
			return nullptr;
		}

		return e;
	}

	static bool isPluginClass(Type^ type) {
		if (type->BaseType == nullptr) {
			return false;
		} else if (type->BaseType->AssemblyQualifiedName == HexChatPlugin::typeid->AssemblyQualifiedName) {
			return true;
		} else {
			return isPluginClass(type->BaseType);
		}
	}

	void HexChatInternal::FindTypeInAssembly() {
		AppDomain::CurrentDomain->SetData(TYPE_NAME_KEY, nullptr);
		String^ assemblyPath = safe_cast<String^>(AppDomain::CurrentDomain->GetData(ASSEMBLY_PATH_KEY));

		try {
			Assembly^ reflected = Assembly::ReflectionOnlyLoadFrom(assemblyPath);
			bool found = false;

			for each (Type^ type in reflected->GetExportedTypes()) {
				if (!type->IsAbstract && isPluginClass(type)) {
					if (found) {
						HexChat::Print("Error loading plugin {0}: image can only contain one non-abstract class that inherits from HexChatPlugin.", assemblyPath);
						found = false;
						break;
					}

					found = true;
					AppDomain::CurrentDomain->SetData(TYPE_NAME_KEY, type->FullName);
				}
			}

			// don't report any errors if we find a dll that doesn't export any types as this means it is likely a C plugin instead (or a dependency dll)
			if (!found) {
				AppDomain::CurrentDomain->SetData(TYPE_NAME_KEY, nullptr);
			}
		} catch (FileNotFoundException^) {
			HexChat::Print("Error loading plugin {0}: File does not exist or is otherwise inaccessible.", assemblyPath);
		} catch (Exception^ e) {
			ReportException(e);
		}
	}

	// Loads dependencies from the same folder as requesting assembly EXCEPT if we are trying to load hcdotnet.dll
	// in which case we point to ourself instead to prevent two different hcdotnet versions from being loaded
	Assembly^ HexChatInternal::LoadDependencies(Object^ sender, ResolveEventArgs^ args) {
		String^ requestedName = AssemblyName(args->Name).Name;

		if (requestedName == "hcdotnet") {
			return Assembly::GetExecutingAssembly();
		}

		String^ folderPath = Path::GetDirectoryName(args->RequestingAssembly->Location);
		String^ assemblyPath = Path::Combine(folderPath, requestedName + ".dll");

		if (!File::Exists(assemblyPath)) {
			return nullptr;
		}

		return Assembly::LoadFrom(assemblyPath);
	}

	Assembly^ HexChatInternal::ReflectionOnlyLoadDependencies(Object^ sender, ResolveEventArgs^ args) {
		String^ requestedName = AssemblyName(args->Name).Name;

		if (requestedName == "hcdotnet") {
			return Assembly::ReflectionOnlyLoadFrom(Assembly::GetExecutingAssembly()->Location);
		}

		String^ folderPath = Path::GetDirectoryName(args->RequestingAssembly->Location);
		String^ assemblyPath = Path::Combine(folderPath, requestedName + ".dll");

		if (!File::Exists(assemblyPath)) {
			return nullptr;
		}

		return Assembly::ReflectionOnlyLoadFrom(assemblyPath);
	}

	Eat HexChatInternal::Load(String^ path) {
		// Short-circuit if we aren't trying to load a dll
		if (!path->EndsWith(".dll")) {
			return Eat::None;
		} else if (!Path::IsPathRooted(path)) {
			path = Path::Combine(HexChat::ConfigDir, "addons", "dotnet", path);
		}

		HexChatPlugin^ plugin = nullptr;
		AppDomain^ addonDomain = nullptr;
		AppDomain^ reflectionDomain = nullptr;
		String^ typeName = nullptr;
		AppDomainSetup^ setup = gcnew AppDomainSetup();
		
		try {
			setup->ApplicationBase = Path::GetDirectoryName(Assembly::GetExecutingAssembly()->Location);
			reflectionDomain = AppDomain::CreateDomain("ReflectionDomain", nullptr, setup);

			reflectionDomain->ReflectionOnlyAssemblyResolve += gcnew ResolveEventHandler(&HexChatDotNet::HexChatInternal::ReflectionOnlyLoadDependencies);
			reflectionDomain->SetData(ASSEMBLY_PATH_KEY, path);
			reflectionDomain->DoCallBack(gcnew CrossAppDomainDelegate(FindTypeInAssembly));
			typeName = safe_cast<String^>(reflectionDomain->GetData(TYPE_NAME_KEY));
			AppDomain::Unload(reflectionDomain);
		} catch (Exception^ e) {
			String^ errid = RegisterException(e);
			HexChat::Print("Exception occurred when scanning plugins: {0}", e->Message);
			HexChat::Print("Execute \"/dotnet errinfo {0}\" for more detailed information.", errid);
		}

		if (typeName != nullptr) {
			try {
				addonDomain = AppDomain::CreateDomain(path, nullptr, setup);
				addonDomain->AssemblyResolve += gcnew ResolveEventHandler(&HexChatDotNet::HexChatInternal::LoadDependencies);

				plugin = safe_cast<HexChatPlugin^>(addonDomain->CreateInstanceFromAndUnwrap(path, typeName));
				plugin->Init();
				IntPtr _ph = HexChat::PluginGuiAdd(path, plugin->Name, plugin->Desc, plugin->Version);
				_loadedPlugins->Add(gcnew Tuple<AppDomain^, HexChatPlugin^>(addonDomain, plugin));
				_pluginMap[path] = gcnew Tuple<String^, IntPtr>(plugin->Name, _ph);
			} catch (Exception^ e) {
				String^ errid = RegisterException(e);
				HexChat::Print("Exception occured when loading plugin: {0}", e->Message);
				HexChat::Print("Execute \"/dotnet errinfo {0}\" for more detailed information.", errid);

				if (plugin != nullptr) {
					delete plugin;
				}

				AppDomain::Unload(addonDomain);
			}

			return Eat::All;
		}

		return Eat::None;
	}

	Eat HexChatInternal::Reload(String^ path) {
		try {
			bool ourPlugin = false;

			if (path->EndsWith(".dll") && !Path::IsPathRooted(path)) {
				path = Path::Combine(HexChat::ConfigDir, "addons", "dotnet", path);
			}

			if (!path->EndsWith(".dll")) {
				// given a plugin name instead of path, find the path given the name
				for each (auto kvp in _pluginMap) {
					if (kvp.Value->Item1 == path) {
						path = kvp.Key;
						ourPlugin = true;
						break;
					}
				}
			} else if (_pluginMap->ContainsKey(path)) {
				ourPlugin = true;
			}

			if (!ourPlugin) {
				return Eat::None;
			}

			Unload(path);
			Load(path);

			return Eat::All;
		} catch (Exception^ e) {
			String^ errid = RegisterException(e);

			HexChat::Print("{0} has occurred during /reload: {1}", e->GetType()->Name, e->Message);
			HexChat::Print("Execute \"/dotnet errinfo {0}\" for more detailed information.", errid);
		}

		return Eat::None;
	}

	Eat HexChatInternal::Unload(String^ path) {
		int index = -1;
		AppDomain^ pluginDomain = nullptr;
		HexChatPlugin^ plugin = nullptr;
		IntPtr pluginHandle = IntPtr::Zero;

		if (path->EndsWith(".dll") && !Path::IsPathRooted(path)) {
			path = Path::Combine(HexChat::ConfigDir, "addons", "dotnet", path);
		}

		try {
			if (!path->EndsWith(".dll")) {
				// given a plugin name instead of path, find the path given the name
				for (int i = 0; i < _loadedPlugins->Count; ++i) {
					if (_loadedPlugins[i]->Item2->Name == path) {
						pluginDomain = _loadedPlugins[i]->Item1;
						plugin = _loadedPlugins[i]->Item2;
						index = i;

						// update path to be the real path so we can remove from pluginMap
						for each (auto kvp in _pluginMap) {
							if (kvp.Value->Item1 == path) {
								path = kvp.Key;
								pluginHandle = kvp.Value->Item2;
								break;
							}
						}
						break;
					}
				}
			} else {
				Tuple<String^, IntPtr>^ value;
				if (_pluginMap->TryGetValue(path, value)) {
					for (int i = 0; i < _loadedPlugins->Count; ++i) {
						if (_loadedPlugins[i]->Item2->Name == value->Item1) {
							pluginDomain = _loadedPlugins[i]->Item1;
							plugin = _loadedPlugins[i]->Item2;
							pluginHandle = value->Item2;
							index = i;
							break;
						}
					}
				}
			}

			if (index == -1 || pluginHandle == IntPtr::Zero) {
				return Eat::None;
			}

			delete plugin;
			HexChat::PluginGuiRemove(pluginHandle);
			AppDomain::Unload(pluginDomain);
			_loadedPlugins->RemoveAt(index);
			_pluginMap->Remove(path);

			return Eat::All;
		} catch (Exception^ e) {
			String^ errid = RegisterException(e);

			HexChat::Print("{0} has occurred during /unload: {1}", e->GetType()->Name, e->Message);
			HexChat::Print("Execute \"/dotnet errinfo {0}\" for more detailed information.", errid);
		}

		return Eat::None;
	}

	void HexChatInternal::LoadAll() {
		String^ scandir = Path::Combine(HexChat::ConfigDir, "addons", "dotnet");
		String^ ourfile = Assembly::GetExecutingAssembly()->Location;

		for each(String^ dllfile in Directory::GetFiles(scandir, "*.dll", SearchOption::TopDirectoryOnly)) {
			if (dllfile == ourfile) {
				continue; // don't try to load ourselves
			}

			Load(dllfile);
		}
	}

	void HexChatInternal::UnloadAll() {
		List<String^>^ paths = gcnew List<String^>();

		for each (auto kvp in _pluginMap) {
			paths->Add(kvp.Key);
		}

		for each (String^ path in paths) {
			Unload(path);
		}
	}

#ifdef _DEBUG
	void HexChatInternal::PrintAssemblies() {
		for each (Assembly^ a in AppDomain::CurrentDomain->GetAssemblies()) {
			HexChat::Print(a->FullName);
		}
	}

	void HexChatInternal::DebugCommand(String^ param) {
		try {
			if (param == "loadedAssemblies") {
				HexChat::Print("Current Domain: {0}", AppDomain::CurrentDomain->FriendlyName);
				PrintAssemblies();

				for each (Tuple<AppDomain^, HexChatPlugin^>^ p in _loadedPlugins) {
					HexChat::Print("Plugin: {0}", p->Item2->Name);
					HexChat::Print("AppDomain: {0}", p->Item1->FriendlyName);

					p->Item1->DoCallBack(gcnew CrossAppDomainDelegate(PrintAssemblies));
				}
			} else if (param == "dirs") {
				// GetModuleFileName doesn't support a mode where it tells us the maximum buffer size required,
				// so instead we harcode the true maximum path length for the unicode variant of the Windows API (32767)
				wchar_t* path = new wchar_t[32767];

				if (GetModuleFileNameW(NULL, path, 32767)) {
					// ensure this is null-terminated path and then strip out the filename
					path[32766] = 0;
					PathRemoveFileSpecW(path);
					
					int bufsize = WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS, path, -1, NULL, 0, NULL, NULL);

					if (bufsize == 0) {
						return;
					}

					char* buf = new char[bufsize];
					
					if (WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS, path, -1, buf, bufsize, NULL, NULL)) {
						hexchat_print(ph, buf);
					}

					delete[] buf;
				}

				delete[] path;
			}
		} catch (Exception^ e) {
			StringBuilder sb;

			sb.AppendFormat("{0}: {1}", e->GetType()->Name, e->Message != nullptr ? e->Message : String::Empty);
			sb.AppendLine();
			sb.Append(e->StackTrace);

			while (e->InnerException != nullptr) {
				e = e->InnerException;
				sb.AppendLine();
				sb.AppendFormat("Inner Exception {0}: {1}", e->GetType()->Name, e->Message != nullptr ? e->Message : String::Empty);
				sb.AppendLine();
				sb.Append(e->StackTrace);
			}

			HexChat::Print(sb.ToString());
		}
	}
#endif

	ExceptionData::ExceptionData(Exception^ e) {
		Type = e->GetType()->Name;
		Message = e->Message != nullptr ? e->Message : String::Empty;
		StackTrace = e->StackTrace;

		if (e->InnerException != nullptr) {
			InnerData = gcnew ExceptionData(e->InnerException);
		} else {
			InnerData = nullptr;
		}
	}
}
