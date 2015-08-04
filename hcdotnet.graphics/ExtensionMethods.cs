using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using HexChatDotNet;

namespace HexChatDotNet.Graphics
{
	public static class ExtensionMethods
	{
		public static HexChatWindow GetWindow(this HexChatContext context)
		{
			// temporarily use raw HWINDOW instead of the Gtk window since I can't seem to get Gtk# working properly
			// may end up just writing my own P/Invoke wrapper for the underlying gtk libs so that this doesn't take a bazillion dependencies
			return new HexChatWindow(context.WinPtr);
		}
	}
}
