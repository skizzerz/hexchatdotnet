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
#include <vector>
#include "hcdotnet.h"

using std::vector;
using namespace System;
using namespace System::Collections::Generic;
using namespace System::Reflection;
using namespace System::IO;
using namespace msclr::interop;

namespace HexChatDotNet {
	void HexChat::Command(String^ command) {
		marshal_context ctx;

		hexchat_command(ph, ctx.marshal_as<const char*, String>(command));
	}

	void HexChat::Command(String^ format, ... array<Object^>^ args) {
		Command(String::Format(format, args));
	}

	void HexChat::Print(String^ text) {
		marshal_context ctx;

		hexchat_print(ph, ctx.marshal_as<const char*, String>(text));
	}

	void HexChat::Print(String^ format, ... array<Object^>^ args) {
		Print(String::Format(format, args));
	}

	bool HexChat::EmitPrint(PrintEvent eventName, ... array<String^>^ args) {
		return EmitPrint(Extensions::ToHexChatString(eventName), args);
	}

	bool HexChat::EmitPrint(PrintEvent eventName, HexChatEventAttrs^ attrs, ... array<String^>^ args) {
		return EmitPrint(Extensions::ToHexChatString(eventName), attrs, args);
	}

	bool HexChat::EmitPrint(String^ eventName, ... array<String^>^ args) {
		if (eventName == nullptr) {
			throw gcnew ArgumentNullException("eventName");
		}

		if (args->Length > 9) {
			throw gcnew ArgumentException("Too many arguments passed to EmitPrint.");
		}

		marshal_context ctx;
		const char* name = ctx.marshal_as<const char*, String>(eventName);
		const char* argv[9];

		for (int i = 0; i < args->Length; ++i) {
			argv[i] = ctx.marshal_as<const char*, String>(args[i]);
		}

		switch (args->Length) {
		case 0:
			return hexchat_emit_print(ph, name) == 1;
		case 1:
			return hexchat_emit_print(ph, name, argv[0]) == 1;
		case 2:
			return hexchat_emit_print(ph, name, argv[0], argv[1]) == 1;
		case 3:
			return hexchat_emit_print(ph, name, argv[0], argv[1], argv[2]) == 1;
		case 4:
			return hexchat_emit_print(ph, name, argv[0], argv[1], argv[2], argv[3]) == 1;
		case 5:
			return hexchat_emit_print(ph, name, argv[0], argv[1], argv[2], argv[3], argv[4]) == 1;
		case 6:
			return hexchat_emit_print(ph, name, argv[0], argv[1], argv[2], argv[3], argv[4], argv[5]) == 1;
		case 7:
			return hexchat_emit_print(ph, name, argv[0], argv[1], argv[2], argv[3], argv[4], argv[5], argv[6]) == 1;
		case 8:
			return hexchat_emit_print(ph, name, argv[0], argv[1], argv[2], argv[3], argv[4], argv[5], argv[6], argv[7]) == 1;
		case 9:
			return hexchat_emit_print(ph, name, argv[0], argv[1], argv[2], argv[3], argv[4], argv[5], argv[6], argv[7], argv[8]) == 1;
		}

		return false;
	}

	bool HexChat::EmitPrint(String^ eventName, HexChatEventAttrs^ eventAttrs, ... array<String^>^ args) {
		if (eventName == nullptr) {
			throw gcnew ArgumentNullException("eventName");
		}

		if (eventAttrs == nullptr) {
			throw gcnew ArgumentNullException("eventAttrs");
		}

		if (args->Length > 9) {
			throw gcnew ArgumentException("Too many arguments passed to EmitPrint.");
		}

		marshal_context ctx;
		const char* name = ctx.marshal_as<const char*, String>(eventName);
		const char* argv[9];

		for (int i = 0; i < args->Length; ++i) {
			argv[i] = ctx.marshal_as<const char*, String>(args[i]);
		}

		switch (args->Length) {
		case 0:
			return hexchat_emit_print_attrs(ph, static_cast<hexchat_event_attrs*>(eventAttrs->_handle), name) == 1;
		case 1:
			return hexchat_emit_print_attrs(ph, static_cast<hexchat_event_attrs*>(eventAttrs->_handle), name, argv[0]) == 1;
		case 2:
			return hexchat_emit_print_attrs(ph, static_cast<hexchat_event_attrs*>(eventAttrs->_handle), name, argv[0], argv[1]) == 1;
		case 3:
			return hexchat_emit_print_attrs(ph, static_cast<hexchat_event_attrs*>(eventAttrs->_handle), name, argv[0], argv[1], argv[2]) == 1;
		case 4:
			return hexchat_emit_print_attrs(ph, static_cast<hexchat_event_attrs*>(eventAttrs->_handle), name, argv[0], argv[1], argv[2], argv[3]) == 1;
		case 5:
			return hexchat_emit_print_attrs(ph, static_cast<hexchat_event_attrs*>(eventAttrs->_handle), name, argv[0], argv[1], argv[2], argv[3], argv[4]) == 1;
		case 6:
			return hexchat_emit_print_attrs(ph, static_cast<hexchat_event_attrs*>(eventAttrs->_handle), name, argv[0], argv[1], argv[2], argv[3], argv[4], argv[5]) == 1;
		case 7:
			return hexchat_emit_print_attrs(ph, static_cast<hexchat_event_attrs*>(eventAttrs->_handle), name, argv[0], argv[1], argv[2], argv[3], argv[4], argv[5], argv[6]) == 1;
		case 8:
			return hexchat_emit_print_attrs(ph, static_cast<hexchat_event_attrs*>(eventAttrs->_handle), name, argv[0], argv[1], argv[2], argv[3], argv[4], argv[5], argv[6], argv[7]) == 1;
		case 9:
			return hexchat_emit_print_attrs(ph, static_cast<hexchat_event_attrs*>(eventAttrs->_handle), name, argv[0], argv[1], argv[2], argv[3], argv[4], argv[5], argv[6], argv[7], argv[8]) == 1;
		}

		return false;
	}

	void HexChat::SendModes(IEnumerable<String^>^ targets, ModeAction modeAction, char mode) {
		SendModes(targets, 0, modeAction, mode);
	}

	void HexChat::SendModes(IEnumerable<String^>^ targets, int modesPerLine, ModeAction modeAction, char mode) {
		vector<const char*> tarr;
		marshal_context ctx;

		for each (String^ target in targets) {
			tarr.push_back(ctx.marshal_as<const char*, String>(target));
		}

		hexchat_send_modes(ph, &tarr[0], tarr.size(), modesPerLine, safe_cast<char>(modeAction), mode);
	}

	int HexChat::IrcStrCmp(String^ string1, String^ string2) {
		marshal_context ctx;

		return hexchat_nickcmp(ph, ctx.marshal_as<const char*, String>(string1), ctx.marshal_as<const char*, String>(string2));
	}

	String^ HexChat::Strip(String^ str, StripFlags flags) {
		if (flags == StripFlags::None) {
			return str;
		}

		marshal_context ctx;
		char* stripped = hexchat_strip(ph, ctx.marshal_as<const char*, String>(str), str->Length, safe_cast<int>(flags));
		String^ newstr = gcnew String(stripped);
		hexchat_free(ph, stripped);

		return newstr;
	}

	HexChatContext^ HexChat::FindContext(String^ server, String^ channel) {
		marshal_context ctx;
		const char* sv;
		const char* ch;

		if (server == nullptr) {
			sv = nullptr;
		} else {
			sv = ctx.marshal_as<const char*, String>(server);
		}

		if (channel == nullptr) {
			ch = nullptr;
		} else {
			ch = ctx.marshal_as<const char*, String>(channel);
		}

		hexchat_context* context = hexchat_find_context(ph, sv, ch);

		if (context == nullptr) {
			return nullptr;
		}

		return gcnew HexChatContext(context);
	}

	HexChatContext^ HexChat::GetContext() {
		return gcnew HexChatContext(hexchat_get_context(ph));
	}

	bool HexChat::SetContext(HexChatContext^ context) {
		if (context == nullptr) {
			throw gcnew ArgumentNullException("context");
		}

		return hexchat_set_context(ph, static_cast<hexchat_context*>(context->_handle)) == 1;
	}

	// enumerator class for get_list results, we don't use HexChatList because that one doesn't cache results
	generic<typename T> ref class GetListEnumerator;

	generic<typename T>
	ref class GetListResult sealed : public IEnumerable<T>, IDisposable {
	internal:
		hexchat_list* _list;
		List<T>^ _cached;

		GetListResult(hexchat_list* list) : _list(list) { _cached = gcnew List<T>(); }

	public:
		~GetListResult() {
			this->!GetListResult();
		}
		
		!GetListResult() {
			if (_list != nullptr) {
				hexchat_list_free(ph, _list);
				_list = nullptr;
			}
		}

		virtual IEnumerator<T>^ GetEnumerator();
		virtual Collections::IEnumerator^ GetNongenericEnumerator() = Collections::IEnumerable::GetEnumerator;
	};

	generic<typename T>
	ref class GetListEnumerator sealed : public IEnumerator<T> {
		GetListResult<T>^ _parent;
		int _currentIndex;

	internal:
		GetListEnumerator(GetListResult<T>^ parent) : _parent(parent), _currentIndex(0) { }

	public:
		~GetListEnumerator() { }

		virtual property T Current {
			T get() {
				if (_parent->_cached->Count <= _currentIndex) {
					_parent->_cached[_currentIndex] = safe_cast<T>(Activator::CreateInstance(T::typeid, IntPtr(_parent->_list)));
				}

				return _parent->_cached[_currentIndex];
			}
		}

		virtual property Object^ CurrentNongeneric {
			Object^ get() = Collections::IEnumerator::Current::get {
				return this->Current;
			}
		}

		virtual bool MoveNext() {
			++_currentIndex;

			return hexchat_list_next(ph, _parent->_list) == 1;
		}

		virtual void Reset() {
			_currentIndex = 0;
		}
	};

	generic<typename T>
	IEnumerator<T>^ GetListResult<T>::GetEnumerator() {
		return gcnew GetListEnumerator<T>(this);
	}

	generic<typename T>
	Collections::IEnumerator^ GetListResult<T>::GetNongenericEnumerator() {
		return safe_cast<Collections::IEnumerator^>(gcnew GetListEnumerator<T>(this));
	}

	IEnumerable<ChannelInfo^>^ HexChat::GetChannels() {
		return gcnew GetListResult<ChannelInfo^>(hexchat_list_get(ph, "channels"));
	}

	IEnumerable<DccTransferInfo^>^ HexChat::GetDccTransfers() {
		return gcnew GetListResult<DccTransferInfo^>(hexchat_list_get(ph, "dcc"));
	}

	IEnumerable<IgnoreListInfo^>^ HexChat::GetIgnoreList() {
		return gcnew GetListResult<IgnoreListInfo^>(hexchat_list_get(ph, "ignore"));
	}

	IEnumerable<NotifyListInfo^>^ HexChat::GetNotifyList() {
		return gcnew GetListResult<NotifyListInfo^>(hexchat_list_get(ph, "notify"));
	}

	IEnumerable<UserInfo^>^ HexChat::GetUsers() {
		return gcnew GetListResult<UserInfo^>(hexchat_list_get(ph, "users"));
	}

	String^ HexChat::GetEventText(PrintEvent eventName) {
		return GetEventText(Extensions::ToHexChatString(eventName));
	}

	String^ HexChat::GetEventText(String^ eventName) {
		String^ str = String::Format("event_text {0}", eventName);

		return GetInfo(str);
	}

	String^ HexChat::GetPluginApiVersion(bool includeBuildMetadata) {
		String^ metadata = String::Empty;

		if (includeBuildMetadata) {
			auto versionInfo = Assembly::GetExecutingAssembly()->GetName()->Version;
			metadata = String::Format("+{0}.{1}", versionInfo->Build, versionInfo->Revision);
		}

		return API_VERSION + metadata;
	}

	String^ HexChat::GetTranslatedMessage(String^ msgId) {
		marshal_context ctx;

		return gcnew String(hexchat_gettext(ph, ctx.marshal_as<const char*, String>(msgId)));
	}

	String^ HexChat::ConfigDir::get() {
		return HexChatInternal::GetInfo("configdir");
	}

	String^ HexChat::LibraryDir::get() {
		return HexChatInternal::GetInfo("libdirfs");
	}

	String^ HexChat::Version::get() {
		return HexChatInternal::GetInfo("version");
	}
	
	String^ HexChat::GetInfo(String^ id) {
		marshal_context ctx;

		return HexChatInternal::GetInfo(ctx.marshal_as<const char*, String>(id));
	}

	Type^ HexChat::GetPref(String^ pref, String^% sval, Nullable<int>% ival, Nullable<bool>% bval) {
		marshal_context ctx;
		const char** s = nullptr;
		int i;

		sval = nullptr;
		ival = Nullable<int>();
		bval = Nullable<bool>();
		
		int t = hexchat_get_prefs(ph, ctx.marshal_as<const char*, String>(pref), s, &i);

		switch (t) {
		case 0: // Failed
			throw gcnew PreferenceNotFoundException();
		case 1: // String
			sval = gcnew String(*s);
			return String::typeid;
		case 2: // Int
			ival = i;
			return Int32::typeid;
		case 3: // Boolean
			bval = (i == 1);
			return Boolean::typeid;
		}

		throw gcnew Exception("Unknown preference type");
	}

	HexChatList^ HexChat::GetList(String^ name) {
		marshal_context ctx;

		return gcnew HexChatList(name, hexchat_list_get(ph, ctx.marshal_as<const char*, String>(name)));
	}

	IntPtr HexChat::PluginGuiAdd(String^ filename, String^ name, String^ desc, String^ version) {
		marshal_context ctx;
		const char* f = ctx.marshal_as<const char*, String>(filename);
		const char* n = ctx.marshal_as<const char*, String>(name);
		const char* d = ctx.marshal_as<const char*, String>(desc);
		const char* v = ctx.marshal_as<const char*, String>(version);

		return safe_cast<IntPtr>(hexchat_plugingui_add(ph, f, n, d, v, nullptr));
	}

	void HexChat::PluginGuiRemove(IntPtr pluginHandle) {
		hexchat_plugingui_remove(ph, pluginHandle.ToPointer());
	}
}
