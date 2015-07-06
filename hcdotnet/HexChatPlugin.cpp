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
		_ph = IntPtr::Zero;
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
		if (_ph != IntPtr::Zero) {
			HexChat::PluginGuiRemove(_ph);
			_ph = IntPtr::Zero;
		}
	}

	CommandHook^ HexChatPlugin::CreateCommandHook(String^ name, String^ help, Priority priority) {
		if (name == nullptr) {
			throw gcnew ArgumentNullException("name");
		}

		auto hook = gcnew CommandHook(this, name, help, priority);
		_hooks->Add(hook);
		return hook;
	}

	PrintHook^ HexChatPlugin::CreatePrintHook(PrintEvent eventName, Priority priority) {
		return CreatePrintHook(Extensions::ToHexChatString(eventName), priority);
	}

	PrintHook^ HexChatPlugin::CreatePrintHook(String^ eventName, Priority priority) {
		if (eventName == nullptr) {
			throw gcnew ArgumentNullException("eventName");
		}

		auto hook = gcnew PrintHook(this, eventName, priority);
		_hooks->Add(hook);
		return hook;
	}

	ServerHook^ HexChatPlugin::CreateServerHook(ServerEvent eventName, Priority priority) {
		return CreateServerHook(Extensions::ToHexChatString(eventName), priority);
	}

	ServerHook^ HexChatPlugin::CreateServerHook(String^ eventName, Priority priority) {
		if (eventName == nullptr) {
			throw gcnew ArgumentNullException("eventName");
		}

		auto hook = gcnew ServerHook(this, eventName, priority);
		_hooks->Add(hook);
		return hook;
	}

	TimerHook^ HexChatPlugin::CreateTimerHook(int timeout) {
		if (timeout <= 0) {
			throw gcnew ArgumentOutOfRangeException("timeout", "argument must be a positive integer");
		}

		auto hook = gcnew TimerHook(this, timeout);
		_hooks->Add(hook);
		return hook;
	}
}
