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
using namespace System::Net;
using namespace msclr::interop;

namespace HexChatDotNet {
	HexChatList::HexChatList(String^ name, void* list) : _name(name), _list(list) {
		marshal_context ctx;
		_fields = gcnew List<String^>();

		auto fields = hexchat_list_fields(ph, ctx.marshal_as<const char*, String>(name));

		for (int i = 0; fields[i] != nullptr; ++i) {
			_fields->Add(gcnew String(fields[i]));
		}
	}

	HexChatList::~HexChatList() {
		this->!HexChatList();
	}

	HexChatList::!HexChatList() {
		if (_list != nullptr) {
			hexchat_list_free(ph, static_cast<hexchat_list*>(_list));
			_list = nullptr;
		}
	}

	List<String^>^ HexChatList::Fields::get() {
		return _fields;
	}

	bool HexChatList::MoveNext() {
		return hexchat_list_next(ph, static_cast<hexchat_list*>(_list)) == 1;
	}

	String^ HexChatList::GetString(String^ key) {
		marshal_context ctx;

		const char* str = hexchat_list_str(ph, static_cast<hexchat_list*>(_list), ctx.marshal_as<const char*, String>(key));

		if (str == nullptr) {
			return nullptr;
		}

		return gcnew String(str);
	}

	int HexChatList::GetInt(String^ key) {
		marshal_context ctx;

		return hexchat_list_int(ph, static_cast<hexchat_list*>(_list), ctx.marshal_as<const char*, String>(key));
	}

	DateTime get_time(time_t tvar) {
		tm* t = gmtime(&tvar);

		return DateTime(t->tm_year + 1900, t->tm_mon + 1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec, 0, DateTimeKind::Utc);
	}

	DateTime HexChatList::GetTime(String^ key) {
		marshal_context ctx;

		time_t hctime = hexchat_list_time(ph, static_cast<hexchat_list*>(_list), ctx.marshal_as<const char*, String>(key));

		return get_time(hctime);
	}

	ChannelInfo::ChannelInfo(IntPtr list) {
		hexchat_list* l = static_cast<hexchat_list*>(list.ToPointer());
		const char* s;

		_Channel = gcnew String(hexchat_list_str(ph, l, "channel"));
		
		s = hexchat_list_str(ph, l, "channelkey");
		_ChannelKey = (s == nullptr) ? nullptr : gcnew String(s);

		_ChanTypes = gcnew String(hexchat_list_str(ph, l, "chantypes"));
		_Context = gcnew HexChatContext((void*)(hexchat_list_str(ph, l, "context")));
		_Flags = safe_cast<ChannelInfoFlags>(hexchat_list_int(ph, l, "flags"));
		_Id = hexchat_list_int(ph, l, "id");
		_Lag = hexchat_list_int(ph, l, "lag");
		_MaxModes = hexchat_list_int(ph, l, "maxmodes");
		_Network = gcnew String(hexchat_list_str(ph, l, "network"));
		_NickPrefixes = gcnew String(hexchat_list_str(ph, l, "nickprefixes"));
		_NickModes = gcnew UserModes(gcnew String(hexchat_list_str(ph, l, "nickmodes")));
		_SendQueueLength = hexchat_list_int(ph, l, "queue");
		_Server = gcnew String(hexchat_list_str(ph, l, "server"));
		_Type = safe_cast<ChannelInfoType>(hexchat_list_int(ph, l, "type"));
		_NumUsers = hexchat_list_int(ph, l, "users");
	}

	DccTransferInfo::DccTransferInfo(IntPtr list) {
		hexchat_list* l = static_cast<hexchat_list*>(list.ToPointer());

		_Address = gcnew IPAddress(hexchat_list_int(ph, l, "address32"));
		_BytesPerSecond = hexchat_list_int(ph, l, "cps");
		_FullName = gcnew String(hexchat_list_str(ph, l, "destfile"));
		_FileName = gcnew String(hexchat_list_str(ph, l, "file"));
		_Nick = gcnew String(hexchat_list_str(ph, l, "nick"));
		_Port = hexchat_list_int(ph, l, "port");
		_BytesSent = (unsigned long long)hexchat_list_int(ph, l, "pos") | ((unsigned long long)hexchat_list_int(ph, l, "poshigh") << 32);
		_ResumePos = (unsigned long long)hexchat_list_int(ph, l, "resume") | ((unsigned long long)hexchat_list_int(ph, l, "resumehigh") << 32);
		_FileSize = (unsigned long long)hexchat_list_int(ph, l, "size") | ((unsigned long long)hexchat_list_int(ph, l, "sizehigh") << 32);
		_Status = safe_cast<DccTransferStatus>(hexchat_list_int(ph, l, "status"));
		_Type = safe_cast<DccTransferType>(hexchat_list_int(ph, l, "type"));
	}

	IgnoreListInfo::IgnoreListInfo(IntPtr list) {
		hexchat_list* l = static_cast<hexchat_list*>(list.ToPointer());

		_Mask = gcnew String(hexchat_list_str(ph, l, "mask"));
		_Flags = safe_cast<IgnoreListFlags>(hexchat_list_int(ph, l, "flags"));
	}

	NotifyListInfo::NotifyListInfo(IntPtr list) {
		hexchat_list* l = static_cast<hexchat_list*>(list.ToPointer());
		const char* s;

		s = hexchat_list_str(ph, l, "networks");
		_Networks = (s == nullptr) ? nullptr : gcnew List<String^>((gcnew String(s))->Split(','));

		_Nick = gcnew String(hexchat_list_str(ph, l, "nick"));
		_IsOnline = hexchat_list_int(ph, l, "flags") == 1;
		_LastOn = get_time(hexchat_list_time(ph, l, "on"));
		_LastOff = get_time(hexchat_list_time(ph, l, "off"));
		_LastSeen = get_time(hexchat_list_time(ph, l, "seen"));
	}

	UserInfo::UserInfo(IntPtr list) {
		hexchat_list* l = static_cast<hexchat_list*>(list.ToPointer());
		const char* s;

		s = hexchat_list_str(ph, l, "account");
		_Account = (s == nullptr) ? nullptr : gcnew String(s);

		_IsAway = hexchat_list_int(ph, l, "away") == 1;
		_LastTalk = get_time(hexchat_list_time(ph, l, "lasttalk"));
		_Nick = gcnew String(hexchat_list_str(ph, l, "nick"));

		s = hexchat_list_str(ph, l, "host");
		_Host = (s == nullptr) ? nullptr : gcnew String(s);

		_Prefix = gcnew String(hexchat_list_str(ph, l, "prefix"));

		s = hexchat_list_str(ph, l, "realname");
		_RealName = (s == nullptr) ? nullptr : gcnew String(s);

		_IsSelected = hexchat_list_int(ph, l, "selected") == 1;
	}
}
