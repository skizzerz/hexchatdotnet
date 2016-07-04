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
#include <memory>

using namespace System;
using namespace msclr::interop;
using std::unique_ptr;

namespace HexChatDotNet {
namespace Graphics {

HexChatWindow::HexChatWindow(IntPtr winPtr) {
	window = static_cast<GtkWindow*>(winPtr.ToPointer());
}

// functor to wrap an RAII deleter, since we need a "pointer" typedef
// to work around intellisense being stupid and thinking that unique_ptr::get()
// returns <error-type> instead of a correct pointer type
template<typename T, typename D>
struct deleter {
	using pointer = T*;
	D func;

	deleter(D f) : func(f) { }

	void operator()(T* ptr) { func(ptr); }
};

template<typename T, typename D>
auto wrap_unique(T* ptr, D func) {
	return unique_ptr<T, deleter<T, D>>(ptr, deleter<T, D>(func));
}

template<typename T1, typename T2>
bool instanceof(T2 value) {
	return dynamic_cast<T1^>(value) != nullptr;
}

ResponseType HexChatWindow::MessageDialog(MessageType type, ButtonsType buttons, String^ title, String^ format, ... array<Object^>^ args) {
	if (format == nullptr) {
		throw gcnew ArgumentNullException("format");
	}

	marshal_context ctx;
	String^ text = String::Format(format, args);
	String^ main = (title == nullptr) ? text : title;

	auto dlog = wrap_unique(
		gtk_message_dialog_new(
			window,
			GTK_DIALOG_DESTROY_WITH_PARENT,
			static_cast<GtkMessageType>(type),
			static_cast<GtkButtonsType>(buttons),
			"%s",
			ctx.marshal_as<const char *, String>(main)),
		gtk_widget_destroy);

	if (title != nullptr) {
		gtk_message_dialog_format_secondary_text(
			GTK_MESSAGE_DIALOG(dlog.get()),
			"%s",
			ctx.marshal_as<const char *, String>(text));
	}

	gint result = gtk_dialog_run(GTK_DIALOG(dlog.get()));

	return safe_cast<ResponseType>(result);
}

ResponseType HexChatWindow::MessageDialog(MessageType type, ButtonsType buttons, IPangoString^ title, IPangoString^ text) {
	if (text == nullptr) {
		throw gcnew ArgumentNullException("text");
	}

	marshal_context ctx;
	auto create_func = gtk_message_dialog_new;
	IPangoString^ main = (title == nullptr) ? text : title;
	bool isMarkup = !instanceof<PangoText>(main);

	if (isMarkup) {
		create_func = gtk_message_dialog_new_with_markup;
	}

	auto dlog = wrap_unique(
		create_func(
			window,
			GTK_DIALOG_DESTROY_WITH_PARENT,
			static_cast<GtkMessageType>(type),
			static_cast<GtkButtonsType>(buttons),
			"%s",
			isMarkup ? "<placeholder>" : ctx.marshal_as<const char *, String>(main->ToString())),
		gtk_widget_destroy);

	if (isMarkup) {
		gtk_message_dialog_set_markup(
			GTK_MESSAGE_DIALOG(dlog.get()),
			ctx.marshal_as<const char *, String>(main->ToMarkupString()));
	}

	if (title != nullptr) {
		if (instanceof<PangoText>(text)) {
			gtk_message_dialog_format_secondary_text(
				GTK_MESSAGE_DIALOG(dlog.get()),
				"%s",
				ctx.marshal_as<const char *, String>(text->ToString()));
		} else {
			gtk_message_dialog_format_secondary_markup(
				GTK_MESSAGE_DIALOG(dlog.get()),
				"%s",
				ctx.marshal_as<const char *, String>(text->ToMarkupString()));
		}
	}

	gint result = gtk_dialog_run(GTK_DIALOG(dlog.get()));

	return safe_cast<ResponseType>(result);
}

}
}
