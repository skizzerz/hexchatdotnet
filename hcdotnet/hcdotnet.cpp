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
using namespace HexChatDotNet;

hexchat_plugin* ph = nullptr;
static hexchat_hook* hooks[4] = { nullptr };
static int reinit = 0;

static int load_command(char** word, char** word_eol, void* user_data) {
	return static_cast<int>(HexChatInternal::Load(gcnew String(word_eol[1])));
}

static int reload_command(char** word, char** word_eol, void* user_data) {
	return static_cast<int>(HexChatInternal::Reload(gcnew String(word_eol[1])));
}

static int unload_command(char** word, char** word_eol, void* user_data) {
	return static_cast<int>(HexChatInternal::Unload(gcnew String(word_eol[1])));
}

static int dotnet_command(char** word, char** word_eol, void* user_data) {
	// TODO: this
	// word[1] is load, reload, unload, or errinfo
	return HEXCHAT_EAT_ALL;
}

extern "C" __declspec(dllexport) int hexchat_plugin_init(hexchat_plugin* plugin_handle, char** plugin_name, char** plugin_desc, char** plugin_version, char* arg) {
	static bool initialized = false;

	if (initialized) {
		hexchat_print(ph, ".NET interface already loaded.");
		// deinit() is still called even if init() is failed, make sure we don't deinit here
		++reinit;
		return 0;
	}

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

	return 1;
}
