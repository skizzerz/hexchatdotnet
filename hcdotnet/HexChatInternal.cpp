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
#include "hcdotnet.h"

#define STORED_EXCEPTIONS 100

using namespace System;
using namespace System::Collections::Generic;
using namespace System::Reflection;
using namespace System::IO;

namespace HexChatDotNet {
	static HexChatInternal::HexChatInternal() {
		_loadedPlugins = gcnew List<Tuple<AppDomain^, HexChatPlugin^>^>();
		_exceptions = gcnew Dictionary<String^, Exception^>();
		_exceptionKeys = gcnew LinkedList<String^>();
	}

	String^ HexChatInternal::GetInfo(const char* id) {
		return gcnew String(hexchat_get_info(ph, id));
	}

	String^ HexChatInternal::RegisterException(Exception^ e) {
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

	Exception^ HexChatInternal::GetException(String^ key) {
		Exception^ e;
		_exceptions->TryGetValue(key, e);

		return e;
	}

	static bool isPluginClass(Type^ type) {
		if (type->BaseType == nullptr) {
			return false;
		} else if (type->BaseType == HexChatPlugin::typeid) {
			return true;
		} else {
			return isPluginClass(type->BaseType);
		}
	}

	void HexChatInternal::FindTypeInAssembly() {
		TypeName = nullptr;

		try {
			Assembly^ reflected = Assembly::ReflectionOnlyLoadFrom(AssemblyPath);
			bool found = false;

			for each (Type^ type in reflected->GetExportedTypes()) {
				if (!type->IsAbstract && isPluginClass(type)) {
					if (found) {
						HexChat::Print("Error loading plugin {0}: image can only contain one non-abstract class that inherits from HexChatPlugin.", AssemblyPath);
						found = false;
						break;
					}

					found = true;
					TypeName = type->FullName;
				}
			}

			// don't report any errors if we find a dll that doesn't export any types as this means it is likely a C plugin instead (or a dependency dll)
			if (!found) {
				TypeName = nullptr;
			}
		} catch (Exception^ e) {
			String^ errid = RegisterException(e);
			HexChat::Print("Exception occurred when scanning plugins: {0}", e->Message);
			HexChat::Print("Execute \"/dotnet errinfo {0}\" for more detailed information.", errid);
		}
	}

	Eat HexChatInternal::Load(String^ path) {
		// Short-circuit if we aren't trying to load a dll
		if (!path->EndsWith(".dll")) {
			return Eat::None;
		}

		HexChatPlugin^ plugin = nullptr;
		AppDomain^ addonDomain = AppDomain::CreateDomain(path);
		AppDomain^ reflectionDomain = AppDomain::CreateDomain("ReflectionDomain");
		AssemblyPath = path;
		reflectionDomain->DoCallBack(gcnew CrossAppDomainDelegate(FindTypeInAssembly));
		AppDomain::Unload(reflectionDomain);

		if (TypeName != nullptr) {
			try {
				plugin = safe_cast<HexChatPlugin^>(addonDomain->CreateInstanceFromAndUnwrap(path, TypeName));
				plugin->Init();
				plugin->_ph = HexChat::PluginGuiAdd(path, plugin->Name, plugin->Desc, plugin->Version);
				_loadedPlugins->Add(gcnew Tuple<AppDomain^, HexChatPlugin^>(addonDomain, plugin));
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
		bool ourPlugin = false;

		if (!path->EndsWith(".dll")) {
			// given a plugin name instead of path, find the path given the name
			for each (auto t in _loadedPlugins) {
				if (t->Item2->Name == path) {
					path = t->Item2->_path;
					ourPlugin = true;
					break;
				}
			}
		} else {
			for each (auto t in _loadedPlugins) {
				if (t->Item2->_path == path) {
					ourPlugin = true;
					break;
				}
			}
		}

		if (!ourPlugin) {
			return Eat::None;
		}

		Unload(path);
		Load(path);

		return Eat::All;
	}

	Eat HexChatInternal::Unload(String^ path) {
		int index = -1;
		AppDomain^ pluginDomain = nullptr;
		HexChatPlugin^ plugin = nullptr;

		if (!path->EndsWith(".dll")) {
			// given a plugin name instead of path, find the path given the name
			for (int i = 0; i < _loadedPlugins->Count; ++i) {
				if (_loadedPlugins[i]->Item2->Name == path) {
					pluginDomain = _loadedPlugins[i]->Item1;
					plugin = _loadedPlugins[i]->Item2;
					index = i;
					break;
				}
			}
		} else {
			for (int i = 0; i < _loadedPlugins->Count; ++i) {
				if (_loadedPlugins[i]->Item2->_path == path) {
					pluginDomain = _loadedPlugins[i]->Item1;
					plugin = _loadedPlugins[i]->Item2;
					index = i;
					break;
				}
			}
		}

		if (index == -1) {
			return Eat::None;
		}

		delete plugin;
		AppDomain::Unload(pluginDomain);
		_loadedPlugins->RemoveAt(index);

		return Eat::All;
	}

	void HexChatInternal::LoadAll() {
		String^ scandir = HexChat::ConfigDir + "/addons";
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

		for each (auto t in _loadedPlugins) {
			paths->Add(t->Item2->_path);
		}

		for each (String^ path in paths) {
			Unload(path);
		}
	}
}
