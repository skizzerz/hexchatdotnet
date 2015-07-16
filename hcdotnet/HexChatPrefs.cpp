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
using namespace System::Globalization;

// lifted from hexchat's src/common/cfgfiles.c and modified to be specific cultures
// if a culture is not specific, it means no such culture exists in Win7, however it may in the future
// I made an assumption that az and sr in hexchat is latin and not cryllic, if this proves to be wrong then whoops
static const char * const languages[] = {
	"af-ZA", "sq-AL", "am-ET", "ast", "az-Latn-AZ", "eu-ES", "be-BY", "bg-BG", "ca-ES", "zh-CA",
	"zh-TW", "cs-CZ", "da-DK", "nl-NL", "en-GB", "en-US", "et-EE", "fi-FI", "fr-FR", "gl-ES",
	"de-DE", "el-GR", "gu-IN", "hi-IN", "hu-HU", "id-ID", "it-IT", "ja-JP", "kn-IN", "rw-RW",
	"ko-KR", "lv-LV", "lt-LT", "mk-MK", "ml-IN", "ms-MY", "nb-NO", "nn-NO", "pl-PL", "pt-PT",
	"pt-BR", "pa-IN", "ru-RU", "sr-Latn-RS", "sk-SK", "sl-SI", "es-ES", "sv-SE", "th-TH", "tr-TR",
	"uk-UA", "vi-VN", "wa"
};

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

	CultureInfo^ HexChatPrefs::GuiCulture::get() {
		String^ s = gcnew String(languages[GetPref_int("gui_lang")]);
		CultureInfo^ info;

		try {
			info = gcnew CultureInfo(s);
		} catch (CultureNotFoundException^) {
			info = CultureInfo::InvariantCulture;
		} catch (Exception^ e) {
			String^ errid = HexChatInternal::RegisterException(e);
			HexChat::Print("An exception occurred while trying to get culture information");
			HexChat::Print("Execute \"/dotnet errinfo {0}\" for more detailed information.", errid);
			info = CultureInfo::InvariantCulture;
		}

		return info;
	}
}
