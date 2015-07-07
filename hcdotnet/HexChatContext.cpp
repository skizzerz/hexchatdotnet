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

namespace HexChatDotNet {
	static String^ get_info_str(void* _context, const char* info_key) {
		hexchat_context* oldContext = hexchat_get_context(ph);
		hexchat_context* context = static_cast<hexchat_context*>(_context);
		int success = hexchat_set_context(ph, context);

		if (success == 0) {
			throw gcnew ContextSwitchException();
		}

		String^ info = HexChatInternal::GetInfo(info_key);
		hexchat_set_context(ph, oldContext);

		return info;
	}

	static IntPtr get_info_ptr(void* _context, const char* info_key) {
		hexchat_context* oldContext = hexchat_get_context(ph);
		hexchat_context* context = static_cast<hexchat_context*>(_context);
		int success = hexchat_set_context(ph, context);

		if (success == 0) {
			throw gcnew ContextSwitchException();
		}

		char* info = const_cast<char*>(hexchat_get_info(ph, info_key));
		hexchat_set_context(ph, oldContext);

		return IntPtr(static_cast<void*>(info));
	}

	String^ HexChatContext::AwayReason::get() {
		return get_info_str(_handle, "away");
	}

	String^ HexChatContext::Channel::get() {
		return get_info_str(_handle, "channel");
	}

	String^ HexChatContext::Charset::get() {
		return get_info_str(_handle, "charset");
	}

	IntPtr HexChatContext::GtkWinPtr::get() {
		return get_info_ptr(_handle, "gtkwin_ptr");
	}

	String^ HexChatContext::Host::get() {
		return get_info_str(_handle, "host");
	}

	String^ HexChatContext::InputBoxText::get() {
		return get_info_str(_handle, "inputbox");
	}

	void HexChatContext::InputBoxText::set(String^ value) {
		hexchat_context* oldContext = hexchat_get_context(ph);
		hexchat_context* context = static_cast<hexchat_context*>(_handle);
		int success = hexchat_set_context(ph, context);

		if (success == 0) {
			throw gcnew ContextSwitchException();
		}

		HexChat::Command("SETTEXT {0}", value);
		hexchat_set_context(ph, oldContext);
	}

	ChannelModes^ HexChatContext::ChannelModes::get() {
		String^ modesStr = get_info_str(_handle, "modes");

		if (modesStr == nullptr) {
			return nullptr;
		}

		return gcnew HexChatDotNet::ChannelModes(modesStr);
	}
	
	String^ HexChatContext::Network::get() {
		return get_info_str(_handle, "network");
	}

	String^ HexChatContext::Nick::get() {
		return get_info_str(_handle, "nick");
	}

	String^ HexChatContext::NickServPassword::get() {
		return get_info_str(_handle, "nickserv");
	}

	String^ HexChatContext::Server::get() {
		return get_info_str(_handle, "server");
	}

	String^ HexChatContext::Topic::get() {
		return get_info_str(_handle, "topic");
	}

	IntPtr HexChatContext::WinPtr::get() {
		return get_info_ptr(_handle, "win_ptr");
	}

	WindowStatus HexChatContext::WindowStatus::get() {
		String^ status = get_info_str(_handle, "win_status");

		if (status == "active") {
			return HexChatDotNet::WindowStatus::Active;
		} else if (status == "hidden") {
			return HexChatDotNet::WindowStatus::Hidden;
		} else if (status == "normal") {
			return HexChatDotNet::WindowStatus::Normal;
		}

		return HexChatDotNet::WindowStatus::Unknown;
	}

	void HexChatContext::SetCursorPos(int newPos, bool relative) {
		if (!relative && newPos < 0) {
			throw gcnew ArgumentOutOfRangeException("newPos", "argument must be positive if not using relative positioning");
		}

		hexchat_context* oldContext = hexchat_get_context(ph);
		hexchat_context* context = static_cast<hexchat_context*>(_handle);
		int success = hexchat_set_context(ph, context);

		if (success == 0) {
			throw gcnew ContextSwitchException();
		}

		HexChat::Command("SETCURSOR {0}{1}", (newPos >= 0 && relative) ? "+" : String::Empty, newPos);
		hexchat_set_context(ph, oldContext);
	}
}
