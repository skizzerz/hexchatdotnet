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
	String^ HexChatPrefs::GetPref_string(String^ pref) {
		String^ s;
		Nullable<int> i;
		Nullable<bool> b;

		Type^ t = HexChat::GetPref(pref, s, i, b);

		if (t != String::typeid) {
			throw gcnew PreferenceTypeMismatchException();
		}

		return s;
	}

	int HexChatPrefs::GetPref_int(String^ pref) {
		String^ s;
		Nullable<int> i;
		Nullable<bool> b;

		Type^ t = HexChat::GetPref(pref, s, i, b);

		if (t != Int32::typeid) {
			throw gcnew PreferenceTypeMismatchException();
		}

		return i.Value;
	}

	bool HexChatPrefs::GetPref_bool(String^ pref) {
		String^ s;
		Nullable<int> i;
		Nullable<bool> b;

		Type^ t = HexChat::GetPref(pref, s, i, b);

		if (t != Boolean::typeid) {
			throw gcnew PreferenceTypeMismatchException();
		}

		return b.Value;
	}
}
