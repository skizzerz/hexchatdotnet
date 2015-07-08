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

using namespace System;
using namespace System::Reflection;
using namespace System::Collections::Generic;

namespace HexChatDotNet {
	ref class HexChatPlugin;
	enum class Eat;

	ref class HexChatInternal abstract sealed {
		static List<Tuple<AppDomain^, HexChatPlugin^>^>^ _loadedPlugins;
		static Dictionary<String^, Tuple<String^, IntPtr>^>^ _pluginMap;
		static Dictionary<String^, Exception^>^ _exceptions;
		static LinkedList<String^>^ _exceptionKeys;

		static void FindTypeInAssembly();

	internal:
		static HexChatInternal();

		static String^ RegisterException(Exception^ e);
		static Exception^ GetException(String^ key);

		static String^ GetInfo(const char* id);
		
		static Assembly^ LoadDependencies(Object^ sender, ResolveEventArgs^ args);
		static Assembly^ ReflectionOnlyLoadDependencies(Object^ sender, ResolveEventArgs^ args);

		static Eat Load(String^ path);
		static Eat Reload(String^ path);
		static Eat Unload(String^ path);

		static void LoadAll();
		static void UnloadAll();
	};
}
