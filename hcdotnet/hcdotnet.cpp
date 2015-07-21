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

using namespace System;
using namespace System::Text;
using namespace System::Collections::Generic;
using namespace msclr::interop;
using namespace HexChatDotNet;

hexchat_plugin* ph = nullptr;
static hexchat_hook* hooks[4] = { nullptr };
static int reinit = 0;

static int load_command(char** word, char** word_eol, void* user_data) {
	return static_cast<int>(HexChatInternal::Load(gcnew String(word_eol[2])));
}

static int reload_command(char** word, char** word_eol, void* user_data) {
	return static_cast<int>(HexChatInternal::Reload(gcnew String(word_eol[2])));
}

static int unload_command(char** word, char** word_eol, void* user_data) {
	return static_cast<int>(HexChatInternal::Unload(gcnew String(word_eol[2])));
}

static int dotnet_command(char** word, char** word_eol, void* user_data) {
	if (!word[2][0] || !word[3][0]) {
		hexchat_print(ph, "Usage: /DOTNET LOAD|UNLOAD|RELOAD|ERRINFO <param> - see /HELP DOTNET for more details.");
		return HEXCHAT_EAT_ALL;
	}

	int retval = HEXCHAT_EAT_ALL;

	if (!stricmp(word[2], "load")) {
		retval = load_command(word + sizeof(char*), word_eol + sizeof(char*), user_data);
	} else if (!stricmp(word[2], "unload")) {
		retval = unload_command(word + sizeof(char*), word_eol + sizeof(char*), user_data);
	} else if (!stricmp(word[2], "reload")) {
		retval = reload_command(word + sizeof(char*), word_eol + sizeof(char*), user_data);
	} else if (!stricmp(word[2], "errinfo")) {
		StringBuilder sb;
		marshal_context ctx;
		ExceptionData^ e = HexChatInternal::GetException(gcnew String(word[3]));

		if (e == nullptr) {
			hexchat_printf(ph, "Error ID %s not found.", word[3]);
		} else {
			sb.AppendFormat("Uncaught {0} in plugin: {1}", e->Type, e->Message);
			sb.AppendLine();
			sb.Append(e->StackTrace);

			while (e->InnerData != nullptr) {
				e = e->InnerData;
				sb.AppendLine();
				sb.AppendFormat("Inner Exception {0}: {1}", e->Type, e->Message);
				sb.AppendLine();
				sb.Append(e->StackTrace);
			}

			hexchat_print(ph, ctx.marshal_as<const char*>(sb.ToString()));
		}
#ifdef _DEBUG
	} else if (!stricmp(word[2], "debug")) {
		HexChatInternal::DebugCommand(gcnew String(word_eol[3]));
		return HEXCHAT_EAT_ALL;
#endif
	} else {
		hexchat_print(ph, "Usage: /DOTNET LOAD|UNLOAD|RELOAD|ERRINFO <param> - see /HELP DOTNET for more details.");
	}

	if (retval == HEXCHAT_EAT_NONE) {
		hexchat_printf(ph, "Cannot %s %s - not a valid file or plugin name", word[2], word_eol[3]);
	}

	return HEXCHAT_EAT_ALL;
}

static bool initialized = false;

extern "C" __declspec(dllexport) int hexchat_plugin_init(hexchat_plugin* plugin_handle, char** plugin_name, char** plugin_desc, char** plugin_version, char* arg) {
	if (initialized) {
		hexchat_print(ph, ".NET interface already loaded.");
		// deinit() is still called even if init() is failed, make sure we don't deinit here
		++reinit;
		return 0;
	}

	ph = plugin_handle;
	*plugin_name = ".NET";
	*plugin_desc = ".NET plugin interface";
	*plugin_version = API_VERSION;

	hooks[0] = hexchat_hook_command(ph, "LOAD", HEXCHAT_PRI_NORM, load_command, nullptr, nullptr);
	hooks[1] = hexchat_hook_command(ph, "RELOAD", HEXCHAT_PRI_NORM, reload_command, nullptr, nullptr);
	hooks[2] = hexchat_hook_command(ph, "UNLOAD", HEXCHAT_PRI_NORM, unload_command, nullptr, nullptr);
	hooks[3] = hexchat_hook_command(ph, "DOTNET", HEXCHAT_PRI_NORM, dotnet_command, "Usage: /DOTNET <command> [params]\nLOAD <filename.dll>\nRELOAD <plugin name|filename.dll>\nUNLOAD <plugin name|filename.dll>\nERRINFO <error id> - Displays detailed error information about a .NET plugin error", nullptr);
	hexchat_print(ph, ".NET interface v" API_VERSION " loaded.");

	HexChatInternal::LoadAll();

	initialized = true;
	return 1;
}

extern "C" __declspec(dllexport) int hexchat_plugin_deinit() {
	if (reinit) {
		--reinit;
		return 1;
	}

	HexChatInternal::UnloadAll();

	for (int i = 0; i < 5; ++i) {
		hexchat_unhook(ph, hooks[i]);
	}

	hexchat_print(ph, ".NET interface unloaded.");
	initialized = false;

	return 1;
}
