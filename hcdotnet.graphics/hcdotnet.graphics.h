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

#pragma once

#using <hcdotnet.dll> as_friend

#include <gtk/gtk.h>

namespace HexChatDotNet {
namespace Graphics {

interface class IPangoString;

ref class ExtensionMethods;
ref class HexChatWindow;
ref class PangoString;
ref class PangoBold;
ref class PangoBig;
ref class PangoItalic;
ref class PangoStrikethrough;
ref class PangoSubscript;
ref class PangoSuperscript;
ref class PangoSmall;
ref class PangoMonospace;
ref class PangoUnderline;
ref class PangoStringBuilder;

value class FontSize;
value class FontWeight;

enum class MessageType;
enum class ButtonsType;
enum class ResponseType;
enum class FontStyle;
enum class FontVariant;
enum class FontStretch;
enum class Underline;
enum class Gravity;
enum class GravityHint;

[System::Runtime::CompilerServices::ExtensionAttribute]
public ref class ExtensionMethods abstract sealed {
public:
	[System::Runtime::CompilerServices::ExtensionAttribute]
	static HexChatWindow^ GetWindow(HexChatContext^ context);
};

public ref class HexChatWindow sealed {
private:
	GtkWindow* window;

internal:
	HexChatWindow(System::IntPtr winPtr);

public:
	ResponseType MessageDialog(MessageType type, ButtonsType buttons, System::String^ format, ... array<System::String^>^ args);
};

public interface class IPangoString {
public:
	System::String^ ToMarkupString();
};

#define PROP3(t, n, b) property t n { t get() { return b; } void set(t value) { b = value; } }
#define PROP2(t, b) PROP3(HexChatDotNet::Graphics::##t, t, b)
#define PROPS(n, b) PROP3(System::String^, n, b)
#define PROPD(n, b) PROP3(System::Decimal, n, b)
#define PROPB(n, b) PROP3(bool, n, b)

public ref class PangoString : IPangoString {
protected:
	System::String^ _tag;
	System::String^ _contents;
	System::String^ _fontFamily;
	HexChatDotNet::Graphics::FontSize _fontSize;
	HexChatDotNet::Graphics::FontStyle _fontStyle;
	HexChatDotNet::Graphics::FontWeight _fontWeight;
	HexChatDotNet::Graphics::FontVariant _fontVariant;
	HexChatDotNet::Graphics::FontStretch _fontStretch;
	System::String^ _fontFeatures;
	HexChatDotNet::Graphics::Underline _underline;
	// Note: we store _rise in pixels, but pango expects pango units; we multiply this value by PANGO_SCALE to get that
	System::Decimal _rise;
	bool _strikethrough;
	bool _fallback;
	System::String^ _lang;
	// Note: we store _letterSpacing in points, but pango expects 1024ths of a point; we multiply this value by 1024 to get that
	System::Decimal _letterSpacing;
	HexChatDotNet::Graphics::Gravity _gravity;
	HexChatDotNet::Graphics::GravityHint _gravityHint;

	PangoString(System::String^ tag, System::String^ contents) : _tag(tag), _contents(contents) { }

public:
	PangoString(System::String^ contents) : _tag("span"), _contents(contents) { }

	PROPS(FontFamily, _fontFamily)
	PROP2(FontSize, _fontSize)
	PROP2(FontStyle, _fontStyle)
	PROP2(FontWeight, _fontWeight)
	PROP2(FontVariant, _fontVariant)
	PROP2(FontStretch, _fontStretch)
	PROPS(FontFeatures, _fontFeatures)
	// FG Color
	// BG Color
	// FG Alpha
	// BG Alpha
	PROP2(Underline, _underline)
	// Underline Color
	PROPD(Rise, _rise)
	PROPB(Strikethrough, _strikethrough)
	// Strikethrough Color
	PROPB(Fallback, _fallback)
	PROPS(Lang, _lang)
	PROPD(LetterSpacing, _letterSpacing)
	PROP2(Gravity, _gravity)
	PROP2(GravityHint, _gravityHint)

	virtual System::String^ ToMarkupString();
};

#undef PROP3
#undef PROP2
#undef PROPS
#undef PROPD
#undef PROPB

public ref class PangoBold : PangoString {
public:
	PangoBold(System::String^ contents) : PangoString("b", contents) { }
};

public ref class PangoBig : PangoString {
public:
	PangoBig(System::String^ contents) : PangoString("big", contents) { }
};

public ref class PangoItalic : PangoString {
public:
	PangoItalic(System::String^ contents) : PangoString("i", contents) { }
};

public ref class PangoStrikethrough : PangoString {
public:
	PangoStrikethrough(System::String^ contents) : PangoString("s", contents) { }
};

public ref class PangoSubscript : PangoString {
public:
	PangoSubscript(System::String^ contents) : PangoString("sub", contents) { }
};

public ref class PangoSuperscript : PangoString {
public:
	PangoSuperscript(System::String^ contents) : PangoString("sup", contents) { }
};

public ref class PangoSmall : PangoString {
public:
	PangoSmall(System::String^ contents) : PangoString("small", contents) { }
};

public ref class PangoMonospace : PangoString {
public:
	PangoMonospace(System::String^ contents) : PangoString("tt", contents) { }
};

public ref class PangoUnderline : PangoString {
public:
	PangoUnderline(System::String^ contents) : PangoString("u", contents) { }
};

public value class FontSize {
private:
	bool _useSizeStr;
	unsigned long long _size;
	System::String^ _sizeStr;

	FontSize(System::String^ sizeStr) : _useSizeStr(true), _sizeStr(sizeStr) { }

public:
	FontSize(System::Decimal size) : _useSizeStr(false) {
		_size = safe_cast<unsigned long long>(size * 1024);
	}

	System::String^ ToString() override {
		return _useSizeStr ? _sizeStr : _size.ToString();
	}

	static property FontSize XXSmall { FontSize get() { return FontSize("xx-small"); } }
	static property FontSize XSmall { FontSize get() { return FontSize("x-small"); } }
	static property FontSize Small { FontSize get() { return FontSize("small"); } }
	static property FontSize Medium { FontSize get() { return FontSize("medium"); } }
	static property FontSize Large { FontSize get() { return FontSize("large"); } }
	static property FontSize XLarge { FontSize get() { return FontSize("x-large"); } }
	static property FontSize XXLarge { FontSize get() { return FontSize("xx-large"); } }

	static property FontSize Smaller { FontSize get() { return FontSize("smaller"); } }
	static property FontSize Larger { FontSize get() { return FontSize("larger"); } }
};

public value class FontWeight {
private:
	bool _useWeightStr;
	System::Decimal _weight;
	System::String^ _weightStr;

	FontWeight(System::String^ weightStr) : _useWeightStr(true), _weightStr(weightStr) { }

public:
	FontWeight(System::Decimal weight) : _useWeightStr(false), _weight(weight) { }

	System::String^ ToString() override {
		return _useWeightStr ? _weightStr : _weight.ToString();
	}

	static property FontWeight UltraLight { FontWeight get() { return FontWeight("ultralight"); } }
	static property FontWeight Light { FontWeight get() { return FontWeight("light"); } }
	static property FontWeight Normal { FontWeight get() { return FontWeight("normal"); } }
	static property FontWeight Bold { FontWeight get() { return FontWeight("bold"); } }
	static property FontWeight UltraBold { FontWeight get() { return FontWeight("ultrabold"); } }
	static property FontWeight Heavy { FontWeight get() { return FontWeight("heavy"); } }
};

public enum class MessageType {
	Info = 0,
	Warning = 1,
	Question = 2,
	Error = 3,
	Other = 4
};

public enum class ButtonsType {
	None = 0,
	Ok = 1,
	Close = 2,
	Cancel = 3,
	YesNo = 4,
	OkCancel = 5
};

public enum class ResponseType {
	None = -1,
	Reject = -2,
	Accept = -3,
	DeleteEvent = -4,
	Ok = -5,
	Cancel = -6,
	Close = -7,
	Yes = -8,
	No = -9,
	Apply = -10,
	Help = -11
};

public enum class FontStyle {
	Normal,
	Oblique,
	Italic
};

public enum class FontVariant {
	Normal,
	Smallcaps
};

public enum class FontStretch {
	UltraCondensed,
	ExtraCondensed,
	Condensed,
	SemiCondensed,
	Normal,
	SemiExpanded,
	Expanded,
	ExtraExpanded,
	UltraExpanded
};

public enum class Gravity {
	Auto,
	South,
	East,
	North,
	West
};

public enum class GravityHint {
	Natural,
	Strong,
	Line
};

}
}
