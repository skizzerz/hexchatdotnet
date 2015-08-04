#include "stdafx.h"

using namespace System;
using namespace System::Reflection;
using namespace System::Runtime::CompilerServices;
using namespace System::Runtime::InteropServices;
using namespace System::Security::Permissions;

//
// General Information about an assembly is controlled through the following
// set of attributes. Change these attribute values to modify the information
// associated with an assembly.
//
[assembly:AssemblyTitleAttribute(L"hcdotnet")];
[assembly:AssemblyDescriptionAttribute(L"HexChat .NET Plugin Interface")];
[assembly:AssemblyConfigurationAttribute(L"")];
[assembly:AssemblyCompanyAttribute(L"Ryan Schmidt")];
[assembly:AssemblyProductAttribute(L"HexChat .NET Plugin Interface")];
[assembly:AssemblyCopyrightAttribute(L"Copyright (c) 2015 Ryan Schmidt. Licensed under GPL v2 or later.")];
[assembly:AssemblyTrademarkAttribute(L"")];
[assembly:AssemblyCultureAttribute(L"")];

[assembly:InternalsVisibleToAttribute(L"hcdotnet.graphics, PublicKey=002400000480000094000000060200000024000052534131000400000100010011ee3bd4013d5b7cff9f6946d50b4598b07da4a69b27f9433a4a683f64b15388de12f5848b5bc162e60016ffb793eb8315ac9d212bdf90da16286aba16b6085d55acd6b5bfc9092eb9b27a9dc8deca583792e588a831a3a08b2f194ddf84c96d6ecb308ae173163f8004d82d3db393170d95937328c8ada23fd0fdbe15acea9f")]

//
// Version information for an assembly consists of the following four values:
//
//      Major Version
//      Minor Version
//      Build Number
//      Revision
//
// You can specify all the value or you can default the Revision and Build Numbers
// by using the '*' as shown below:

[assembly:AssemblyVersionAttribute(MAJOR_VERSION "." MINOR_VERSION ".0.0")];
[assembly:AssemblyInformationalVersionAttribute(API_VERSION)]

[assembly:ComVisible(false)];

[assembly:CLSCompliantAttribute(true)];