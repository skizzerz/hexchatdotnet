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

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using HexChatDotNet;
using HexChatDotNet.Graphics;

namespace ExamplePlugin
{
	/// <summary>
	/// Example plugin for HexChatDotNet, adds a /meow command.
	/// </summary>
	public class Plugin : HexChatPlugin
	{
		public override string Name
		{
			get { return "Example"; }
		}

		public override string Desc
		{
			get { return "Example plugin for .NET"; }
		}

		public override string Version
		{
			get { return "1.0.0"; }
		}

		public override void Init()
		{
			CommandHook meow = CreateCommandHook("meow", "Usage: /meow");
			meow.Callback += Meow;
			CommandHook woof = CreateCommandHook("woof", "Usage: /woof");
			woof.Callback += Woof;
			HexChat.Print("Example .NET plugin loaded!");
		}

		public Eat Meow(string[] words, string[] wordsEol)
		{
			HexChat.Print("meow!");
			return Eat.All;
		}

		public Eat Woof(string[] words, string[] wordsEol)
		{
			HexChatWindow window = HexChat.GetContext().GetWindow();
			window.MessageDialog(MessageType.Info, ButtonsType.Ok, "Woof!");

			return Eat.All;
		}
	}
}
