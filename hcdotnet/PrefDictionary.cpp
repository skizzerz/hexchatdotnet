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
using namespace System::Xml;
using namespace System::Xml::Serialization;
using namespace System::IO;

// NOTE: the hexchat api does have hexchat_pluginpref_* commands; however, they are very inflexible and sucky,
// so we're rolling our own here instead by serializing/unserializing a Dictionary<String, Object> to XML.

namespace HexChatDotNet {
	PrefDictionary::PrefDictionary(HexChatPlugin^ parent) : _parent(parent) {
		_dict = gcnew Dictionary<String^, Object^>();

		// TODO: initialize _dict from the config file, HexChat::ConfigDir gets the dir the file is in
		// need to figure out the filename scheme though...
	}

	Object^ PrefDictionary::default::get(String^ key) {
		return _dict[key];
	}

	void PrefDictionary::default::set(String^ key, Object^ value) {
		_dict[key] = value;

		// TODO: persist new value to config file
	}

	int PrefDictionary::Count::get() {
		return _dict->Count;
	}

	bool PrefDictionary::IsReadOnly::get() {
		return false;
	}

	Dictionary<String^, Object^>::KeyCollection^ PrefDictionary::Keys::get() {
		return _dict->Keys;
	}

	Dictionary<String^, Object^>::ValueCollection^ PrefDictionary::Values::get() {
		return _dict->Values;
	}

	IEnumerator<KeyValuePair<String^, Object^>>^ PrefDictionary::GetEnumerator() {
		return _dict->GetEnumerator();
	}

	bool PrefDictionary::ContainsKey(String^ key) {
		return _dict->ContainsKey(key);
	}

	void PrefDictionary::Add(String^ key, Object^ value) {
		_dict->Add(key, value);
	}

	bool PrefDictionary::Remove(String^ key) {
		return _dict->Remove(key);
	}

	void PrefDictionary::Clear() {
		return _dict->Clear();
	}

	bool PrefDictionary::TryGetValue(String^ key, Object^% value) {
		return _dict->TryGetValue(key, value);
	}

	ICollection<String^>^ PrefDictionary::KeysCollection::get() {
		return safe_cast<IDictionary<String^, Object^>^>(_dict)->Keys;
	}

	IEnumerable<String^>^ PrefDictionary::KeysEnumerable::get() {
		return safe_cast<IReadOnlyDictionary<String^, Object^>^>(_dict)->Keys;
	}
	
	ICollection<Object^>^ PrefDictionary::ValuesCollection::get() {
		return safe_cast<IDictionary<String^, Object^>^>(_dict)->Values;
	}

	IEnumerable<Object^>^ PrefDictionary::ValuesEnumerable::get() {
		return safe_cast<IReadOnlyDictionary<String^, Object^>^>(_dict)->Values;
	}

	Collections::IEnumerator^ PrefDictionary::GetNongenericEnumerator() {
		return safe_cast<Collections::IEnumerable^>(_dict)->GetEnumerator();
	}

	void PrefDictionary::AddPair(KeyValuePair<String^, Object^> item) {
		safe_cast<ICollection<KeyValuePair<String^, Object^>>^>(_dict)->Add(item);
	}

	bool PrefDictionary::ContainsPair(KeyValuePair<String^, Object^> item) {
		return safe_cast<ICollection<KeyValuePair<String^, Object^>>^>(_dict)->Contains(item);
	}

	void PrefDictionary::CopyTo(array<KeyValuePair<String^, Object^>>^ array, int arrayIndex) {
		safe_cast<ICollection<KeyValuePair<String^, Object^>>^>(_dict)->CopyTo(array, arrayIndex);
	}

	bool PrefDictionary::RemovePair(KeyValuePair<String^, Object^> item) {
		return safe_cast<ICollection<KeyValuePair<String^, Object^>>^>(_dict)->Remove(item);
	}
}
