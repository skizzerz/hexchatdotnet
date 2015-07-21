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
#pragma once

struct ICLRRuntimeHost;

namespace HexChatDotNet {
	ref class HexChatPlugin;
	ref class ExceptionData;
	enum class Eat;

	ref class HexChatInternal abstract sealed {
		static System::Collections::Generic::List<System::Tuple<System::AppDomain^, HexChatPlugin^>^>^ _loadedPlugins;
		static System::Collections::Generic::Dictionary<System::String^, System::Tuple<System::String^, System::IntPtr>^>^ _pluginMap;
		static System::Collections::Generic::Dictionary<System::String^, ExceptionData^>^ _exceptions;
		static System::Collections::Generic::LinkedList<System::String^>^ _exceptionKeys;
		static bool _saveExceptionDetails;
		static ICLRRuntimeHost* _host;

		static void FindTypeInAssembly();

		static HexChatInternal();

	internal:
		static int ReportExceptionInternal(System::String^ serialized);
		static System::String^ RegisterException(ExceptionData^ e);
		static System::String^ RegisterException(System::Exception^ e);
		static ExceptionData^ GetException(System::String^ key);

		static System::String^ GetInfo(const char* id);
		
		static System::Reflection::Assembly^ LoadDependencies(System::Object^ sender, System::ResolveEventArgs^ args);
		static System::Reflection::Assembly^ ReflectionOnlyLoadDependencies(System::Object^ sender, System::ResolveEventArgs^ args);

		static Eat Load(System::String^ path);
		static Eat Reload(System::String^ path);
		static Eat Unload(System::String^ path);

		static void LoadAll();
		static void UnloadAll();

#ifdef _DEBUG
		static void DebugCommand(System::String^ param);
		static void PrintAssemblies();
#endif

	public:
		static void ReportException(System::Exception^ e);
	};

	[System::Runtime::Serialization::DataContract]
	ref class ExceptionData {
	public:
		[System::Runtime::Serialization::DataMember]
		System::String^ Type;
		[System::Runtime::Serialization::DataMember]
		System::String^ Message;
		[System::Runtime::Serialization::DataMember]
		System::String^ StackTrace;
		[System::Runtime::Serialization::DataMember]
		ExceptionData^ InnerData;

		ExceptionData() : Type(nullptr), Message(nullptr), StackTrace(nullptr), InnerData(nullptr) { }
		ExceptionData(System::Exception^ e);
	};
}
