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
	HexChatEventAttrs::HexChatEventAttrs() {
		_handle = hexchat_event_attrs_create(ph);
		_handle->server_time_utc = 0;
	}

	HexChatEventAttrs::HexChatEventAttrs(hexchat_event_attrs* handle) : _handle(handle) { }

	HexChatEventAttrs::~HexChatEventAttrs() {
		this->!HexChatEventAttrs();
	}

	HexChatEventAttrs::!HexChatEventAttrs() {
		if (_handle != nullptr) {
			hexchat_event_attrs_free(ph, _handle);
			_handle = nullptr;
		}
	}

	DateTime HexChatEventAttrs::ServerTimeUtc::get() {
		tm* t = gmtime(&_handle->server_time_utc);

		return DateTime(t->tm_year + 1900, t->tm_mon + 1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec, 0, DateTimeKind::Utc);
	}

	void HexChatEventAttrs::ServerTimeUtc::set(DateTime value) {
		tm t;

		t.tm_year = value.Year - 1900;
		t.tm_mon = value.Month - 1;
		t.tm_mday = value.Day;
		t.tm_hour = value.Hour;
		t.tm_min = value.Minute;
		t.tm_sec = value.Second;

		_handle->server_time_utc = mktime(&t);
	}
}
