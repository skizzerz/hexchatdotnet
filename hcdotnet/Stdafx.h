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
#define _CRT_SECURE_NO_WARNINGS
// C4100: Unrefernced formal parameter
// Lots of managed functions take parameters that are not used (the "sender" in event handlers for instance)
// C4996: The POSIX name for this item is deprecated. Instead use the ISO C and C++ conformant name: _blah.
// Iditioc and unneeded warning
#pragma warning(disable: 4100 4996)

#include <msclr/marshal.h>
#include "hexchat-plugin.h"
#include "HexChatInternal.h"

// Version numbering follows the semantic versioning spec
// Compiled number can be obtained via the API_VERSION macro or by calling HexChat::GetPluginApiVersion(bool includeBuildMetadata)
#define MAJOR_VERSION "1"
#define MINOR_VERSION "0"
#define PATCH_VERSION "0"
#define PRERELEASE_SUFFIX "-alpha"

#define API_VERSION MAJOR_VERSION "." MINOR_VERSION "." PATCH_VERSION PRERELEASE_SUFFIX

extern hexchat_plugin* ph;
