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
using namespace System::Runtime::InteropServices;
using namespace msclr::interop;

namespace HexChatDotNet {
	template<typename T>
	HookBase<T>::~HookBase() {
		if (_hook != nullptr) {
			hexchat_unhook(ph, static_cast<hexchat_hook*>(_hook));
			_gch.Free();
			_hook = nullptr;
		}
	}

	template<typename T>
	void HookBase<T>::Hook() {
		_hook = HookInternal();
	}

	template<typename T>
	void HookBase<T>::Unhook() {
		if (_hook != nullptr) {
			hexchat_unhook(ph, static_cast<hexchat_hook*>(_hook));
			_gch.Free();
			_hook = nullptr;
		}
	}

	typedef int(__stdcall *command_t)(char**, char**, void*);
	typedef int(__stdcall *print_t)(char**, hexchat_event_attrs*, void*);
	typedef int(__stdcall *server_t)(char**, char**, hexchat_event_attrs*, void*);
	typedef int(__stdcall *timer_t)(void*);

	delegate int command_d(char**, char**, void*);
	delegate int print_d(char**, hexchat_event_attrs*, void*);
	delegate int server_d(char**, char**, hexchat_event_attrs*, void*);
	delegate int timer_d(void*);

	template<typename T, typename... Args>
	static void* return_last(T x, Args... xs) {
		return return_last(xs...);
	}

	template<>
	void* return_last(void* x) {
		return x;
	}

	template<typename Fp, typename... Args>
	static int cb_shim(Args... args) {
		return static_cast<Fp>(return_last(args...))(args...);
	}

	void* CommandHook::HookInternal() {
		marshal_context ctx;

		const char* name = ctx.marshal_as<const char*, String>(_name);
		const char* help = nullptr;
		int priority = safe_cast<int>(_priority);

		if (_help != nullptr) {
			help = ctx.marshal_as<const char*, String>(_help);
		}

		command_d^ dp = gcnew command_d(this, &HexChatDotNet::CommandHook::CallbackInternal);
		_gch = GCHandle::Alloc(dp);
		command_t fp = static_cast<command_t>(Marshal::GetFunctionPointerForDelegate(dp).ToPointer());

		return hexchat_hook_command(ph, name, priority, cb_shim<command_t, char**, char**, void*>, help, fp);
	}

	int CommandHook::CallbackInternal(char** word, char** word_eol, void* user_data) {
		int numWords = 0;

		for (int i = 1; i < 32; ++i) {
			if (*reinterpret_cast<short*>(word[i]) == 0) {
				break;
			}

			++numWords;
		}

		array<String^>^ aWord = gcnew array<String^>(numWords);
		array<String^>^ aWordEol = gcnew array<String^>(numWords);

		for (int i = 0; i < numWords; ++i) {
			aWord[i] = gcnew String(word[i + 1]);
			aWordEol[i] = gcnew String(word_eol[i + 1]);
		}

		try {
			Eat eat = Callback(aWord, aWordEol);

			return static_cast<int>(eat);
		} catch (Exception^ e) {
			HexChatInternal::ReportException(e);

			return HEXCHAT_EAT_NONE;
		}
	}

	void* PrintHook::HookInternal() {
		marshal_context ctx;

		const char* name = ctx.marshal_as<const char*, String>(_eventName);
		int priority = safe_cast<int>(_priority);

		print_d^ dp = gcnew print_d(this, &HexChatDotNet::PrintHook::CallbackInternal);
		_gch = GCHandle::Alloc(dp);
		print_t fp = static_cast<print_t>(Marshal::GetFunctionPointerForDelegate(dp).ToPointer());

		return hexchat_hook_print_attrs(ph, name, priority, cb_shim<print_t, char**, hexchat_event_attrs*, void*>, fp);
	}

	int PrintHook::CallbackInternal(char** word, hexchat_event_attrs* attrs, void* user_data) {
		int numWords = 0;

		for (int i = 1; i < 32; ++i) {
			if (*reinterpret_cast<short*>(word[i]) == 0) {
				break;
			}

			++numWords;
		}

		array<String^>^ aWord = gcnew array<String^>(numWords);

		for (int i = 0; i < numWords; ++i) {
			aWord[i] = gcnew String(word[i + 1]);
		}

		try {
			Eat eat = Callback(aWord, gcnew HexChatEventAttrs(attrs));

			return static_cast<int>(eat);
		} catch (Exception^ e) {
			HexChatInternal::ReportException(e);

			// TODO: automatically remove hook if we're getting spammed by errors from it

			return HEXCHAT_EAT_NONE;
		}
	}

	void* ServerHook::HookInternal() {
		marshal_context ctx;

		const char* name = ctx.marshal_as<const char*, String>(_eventName);
		int priority = safe_cast<int>(_priority);

		server_d^ dp = gcnew server_d(this, &HexChatDotNet::ServerHook::CallbackInternal);
		_gch = GCHandle::Alloc(dp);
		server_t fp = static_cast<server_t>(Marshal::GetFunctionPointerForDelegate(dp).ToPointer());

		return hexchat_hook_server_attrs(ph, name, priority, cb_shim<server_t, char**, char**, hexchat_event_attrs*, void*>, fp);
	}

	int ServerHook::CallbackInternal(char** word, char** word_eol, hexchat_event_attrs* attrs, void* user_data) {
		int numWords = 0;

		for (int i = 1; i < 32; ++i) {
			if (*reinterpret_cast<short*>(word[i]) == 0) {
				break;
			}

			++numWords;
		}

		array<String^>^ aWord = gcnew array<String^>(numWords);
		array<String^>^ aWordEol = gcnew array<String^>(numWords);

		for (int i = 0; i < numWords; ++i) {
			aWord[i] = gcnew String(word[i + 1]);
			aWordEol[i] = gcnew String(word_eol[i + 1]);
		}

		try {
			Eat eat = Callback(aWord, aWordEol, gcnew HexChatEventAttrs(attrs));

			return static_cast<int>(eat);
		} catch (Exception^ e) {
			HexChatInternal::ReportException(e);

			// TODO: automatically remove hook if we're getting spammed by errors from it

			return HEXCHAT_EAT_NONE;
		}
	}

	void* TimerHook::HookInternal() {
		timer_d^ dp = gcnew timer_d(this, &HexChatDotNet::TimerHook::CallbackInternal);
		_gch = GCHandle::Alloc(dp);
		timer_t fp = static_cast<timer_t>(Marshal::GetFunctionPointerForDelegate(dp).ToPointer());

		return hexchat_hook_timer(ph, _timeout, cb_shim<timer_t, void*>, fp);
	}

	int TimerHook::CallbackInternal(void* user_data) {
		try {
			Timer timer = Callback();

			return static_cast<int>(timer);
		} catch (Exception^ e) {
			HexChatInternal::ReportException(e);

			return 0; // remove the offending timer
		}
	}
}
