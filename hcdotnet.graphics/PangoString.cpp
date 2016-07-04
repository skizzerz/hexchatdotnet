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
#include "hcdotnet.graphics.h"

using namespace System;
using namespace System::Text;
using namespace System::Net;
using namespace System::Collections::Generic;

namespace HexChatDotNet {
namespace Graphics {

String^ PangoString::ToMarkupString() {
	StringBuilder sb("<");
	sb.Append(Tag);

	if (!String::IsNullOrEmpty(FontFamily)) {
		sb.AppendFormat(" font_family=\"{0}\"", FontFamily);
	}

	if (FontSize.HasValue) {
		sb.AppendFormat(" font_size=\"{0}\"", FontSize.Value.ToString());
	}

	if (FontStyle.HasValue) {
		sb.AppendFormat(" font_style=\"{0}\"", FontStyle.Value.ToString()->ToLowerInvariant());
	}

	if (FontWeight.HasValue) {
		sb.AppendFormat(" font_weight=\"{0}\"", FontWeight.Value.ToString());
	}

	if (FontStretch.HasValue) {
		sb.AppendFormat(" font_stretch=\"{0}\"", FontStretch.Value.ToString()->ToLowerInvariant());
	}

	if (!String::IsNullOrEmpty(FontFeatures)) {
		sb.AppendFormat(" font_features=\"{0}\"", FontFeatures);
	}

	if (!String::IsNullOrEmpty(ForegroundColor)) {
		sb.AppendFormat(" fgcolor=\"{0}\"", ForegroundColor);
	}

	if (!String::IsNullOrEmpty(BackgroundColor)) {
		sb.AppendFormat(" bgcolor=\"{0}\"", BackgroundColor);
	}

	if (!String::IsNullOrEmpty(ForegroundAlpha)) {
		sb.AppendFormat(" fgalpha=\"{0}\"", ForegroundAlpha);
	}

	if (!String::IsNullOrEmpty(BackgroundAlpha)) {
		sb.AppendFormat(" bgalpha=\"{0}\"", BackgroundAlpha);
	}

	if (Underline.HasValue) {
		sb.AppendFormat(" underline=\"{0}\"", Underline.Value.ToString()->ToLowerInvariant());
	}

	if (!String::IsNullOrEmpty(UnderlineColor)) {
		sb.AppendFormat(" underline_color=\"{0}\"", UnderlineColor);
	}

	if (Rise.HasValue) {
		sb.AppendFormat(" rise=\"{0}\"", pango_units_from_double(safe_cast<double>(Rise.Value)));
	}

	if (Strikethrough.HasValue) {
		sb.AppendFormat(" strikethrough=\"{0}\"", Strikethrough.Value.ToString());
	}

	if (!String::IsNullOrEmpty(StrikethroughColor)) {
		sb.AppendFormat(" strikethrough_color=\"{0}\"", StrikethroughColor);
	}

	if (Fallback.HasValue) {
		sb.AppendFormat(" fallback=\"{0}\"", Fallback.Value.ToString());
	}

	if (!String::IsNullOrEmpty(Lang)) {
		sb.AppendFormat(" lang=\"{0}\"", Lang);
	}

	if (LetterSpacing.HasValue) {
		sb.AppendFormat(" letter_spacing=\"{0}\"", safe_cast<int>(Decimal::Round(LetterSpacing.Value * 1024, MidpointRounding::AwayFromZero)));
	}

	if (Gravity.HasValue) {
		sb.AppendFormat(" gravity=\"{0}\"", Gravity.Value.ToString()->ToLowerInvariant());
	}

	if (GravityHint.HasValue) {
		sb.AppendFormat(" gravity_hint=\"{0}\"", GravityHint.Value.ToString()->ToLowerInvariant());
	}

	sb.AppendFormat(">{0}</{1}>", WebUtility::HtmlEncode(Contents), Tag);

	return sb.ToString();
}

PangoStringBuilder::PangoStringBuilder() : _contents("") { }
PangoStringBuilder::PangoStringBuilder(String^ contents) : _contents(WebUtility::HtmlEncode(contents)) { }
PangoStringBuilder::PangoStringBuilder(IPangoString^ contents) : _contents(contents->ToMarkupString()) { }

PangoStringBuilder^ PangoStringBuilder::Append(String^ value) {
	_contents.Append(WebUtility::HtmlEncode(value));

	return this;
}

PangoStringBuilder^ PangoStringBuilder::Append(IPangoString^ value) {
	_contents.Append(value->ToMarkupString());

	return this;
}

PangoStringBuilder^ PangoStringBuilder::AppendFormat(String^ format, ... array<Object^>^ args) {
	_contents.Append(WebUtility::HtmlEncode(String::Format(format, args)));

	return this;
}

PangoStringBuilder^ PangoStringBuilder::AppendFormat(IPangoString^ format, ... array<Object^>^ args) {
	List<String^>^ escapedArgs;

	for each (auto arg in args) {
		auto pangoArg = dynamic_cast<IPangoString^>(arg);

		if (pangoArg == nullptr) {
			escapedArgs->Add(WebUtility::HtmlEncode(arg->ToString()));
		} else {
			escapedArgs->Add(pangoArg->ToMarkupString());
		}
	}

	_contents.Append(String::Format(format->ToMarkupString(), escapedArgs->ToArray()));

	return this;
}

PangoStringBuilder^ PangoStringBuilder::AppendLine() {
	_contents.AppendLine();

	return this;
}

PangoStringBuilder^ PangoStringBuilder::AppendLine(String^ value) {
	_contents.AppendLine(WebUtility::HtmlEncode(value));

	return this;
}

PangoStringBuilder^ PangoStringBuilder::AppendLine(IPangoString^ value) {
	_contents.AppendLine(value->ToMarkupString());

	return this;
}

String^ PangoStringBuilder::ToMarkupString() {
	return _contents.ToString();
}

PangoText::PangoText(String^ contents) : _contents(contents) { }

String^ PangoText::ToMarkupString() {
	return WebUtility::HtmlEncode(_contents);
}

String^ PangoText::ToString() {
	return _contents;
}

}
}
