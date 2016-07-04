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

#include "Stdafx.h"
#include "hcdotnet.h"

using namespace System;
using namespace System::Collections::Generic;
using namespace System::IO;
using namespace System::Net::Sockets;

namespace HexChatDotNet {
	HexChatPlugin::HexChatPlugin() {
		_hooks = gcnew List<IHexChatHook^>();
		PluginPrefs = gcnew PrefDictionary(this);
	}

	HexChatPlugin::~HexChatPlugin() {
		while (_hooks->Count > 0) {
			_hooks[0]->Unhook();
			_hooks->RemoveAt(0);
		}

		this->!HexChatPlugin();
	}

	HexChatPlugin::!HexChatPlugin() {
		// no-op for now
	}

	// Don't allow our plugin to time out on Remoting and be deallocated prematurely
	Object^ HexChatPlugin::InitializeLifetimeService() {
		return nullptr;
	}

	CommandHook^ HexChatPlugin::CreateCommandHook(String^ name, CommandCallback^ callback, String^ help, Priority priority) {
		if (name == nullptr) {
			throw gcnew ArgumentNullException("name");
		}

		auto hook = gcnew CommandHook(this, name, callback, help, priority);
		_hooks->Add(hook);
		hook->Hook();
		return hook;
	}

	PrintHook^ HexChatPlugin::CreatePrintHook(PrintEvent eventName, PrintCallback^ callback, Priority priority) {
		return CreatePrintHook(Extensions::ToHexChatString(eventName), callback, priority);
	}

	PrintHook^ HexChatPlugin::CreatePrintHook(String^ eventName, PrintCallback^ callback, Priority priority) {
		if (eventName == nullptr) {
			throw gcnew ArgumentNullException("eventName");
		}

		auto hook = gcnew PrintHook(this, eventName, callback, priority);
		_hooks->Add(hook);
		return hook;
	}

	ServerHook^ HexChatPlugin::CreateServerHook(ServerEvent eventName, ServerCallback^ callback, Priority priority) {
		return CreateServerHook(Extensions::ToHexChatString(eventName), callback, priority);
	}

	ServerHook^ HexChatPlugin::CreateServerHook(String^ eventName, ServerCallback^ callback, Priority priority) {
		if (eventName == nullptr) {
			throw gcnew ArgumentNullException("eventName");
		}

		auto hook = gcnew ServerHook(this, eventName, callback, priority);
		_hooks->Add(hook);
		return hook;
	}

	TimerHook^ HexChatPlugin::CreateTimerHook(int timeout, TimerCallback^ callback) {
		if (timeout <= 0) {
			throw gcnew ArgumentOutOfRangeException("timeout", "argument must be a positive integer");
		}

		auto hook = gcnew TimerHook(this, timeout, callback);
		_hooks->Add(hook);
		return hook;
	}
}
