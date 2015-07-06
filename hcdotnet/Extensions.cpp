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
using namespace System::Text::RegularExpressions;

namespace HexChatDotNet {
	String^ Extensions::ToHexChatString(PrintEvent eventName) {
		switch (eventName) {
		case PrintEvent::DccChatText:
			return "DCC Chat Text";
		case PrintEvent::ChannelDeHalfOp:
			return "Channel DeHalfOp";
		case PrintEvent::ChannelDeOp:
			return "Channel DeOp";
		case PrintEvent::ChannelDeVoice:
			return "Channel DeVoice";
		case PrintEvent::ChannelHalfOperator:
			return "Channel Half-Operator";
		case PrintEvent::ChannelInvite:
			return "Channel INVITE";
		case PrintEvent::ChannelUnBan:
			return "Channel UnBan";
		case PrintEvent::ChannelUnQuiet:
			return "Channel UnQuiet";
		case PrintEvent::CtcpGenericToChannel:
			return "CTCP Generic to Channel";
		case PrintEvent::CtcpSoundToChannel:
			return "CTCP Sound to Channel";
		case PrintEvent::FoundIp:
			return "Found IP";
		case PrintEvent::MotdSkipped:
			return "MOTD Skipped";
		case PrintEvent::PartWithReason:
			return "Part with Reason";
		case PrintEvent::PrivateActionToDialog:
			return "Private Action to Dialog";
		case PrintEvent::PrivateMessageToDialog:
			return "Private Message to Dialog";
		case PrintEvent::SslMessage:
			return "SSL Message";
		case PrintEvent::WhoIsChannelOperLine:
			return "WhoIs Channel/Oper Line";
		case PrintEvent::WhoIsIdleLineWithSignon:
			return "WhoIs Idle Line with Signon";
		case PrintEvent::YouPartWithReason:
			return "You Part with Reason";
		default:
			// Convert CamelCase into Camel Case
			// also uppercase keywords: CTCP, DCC, DCC CHAT, DCC SEND, DCC RECV, DCC RESUME, SASL
			// unspace words that should be camel case: Who Is -> WhoIs
			String^ str = eventName.ToString();
			str = Regex::Replace(str, "([A-Z])", " $1", RegexOptions::Compiled)->Trim();
			str = str->Replace("Ctcp", "CTCP");
			str = str->Replace("Dcc Chat", "DCC CHAT");
			str = str->Replace("Dcc Send", "DCC SEND");
			str = str->Replace("Dcc Recv", "DCC RECV");
			str = str->Replace("Dcc Resume", "DCC RESUME");
			str = str->Replace("Dcc", "DCC");
			str = str->Replace("Sasl", "SASL");
			str = str->Replace("Who Is", "WhoIs");
			return str;
		}
	}

	String^ Extensions::ToHexChatString(ServerEvent eventName) {
		switch (eventName) {
		case ServerEvent::RawLine:
			return "RAW LINE";
		case ServerEvent::ISupport:
			return "005";
		default:
			return eventName.ToString()->ToUpperInvariant();
		}
	}
}
