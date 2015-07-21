using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using HexChatDotNet;
using Gtk;

namespace HexChatDotNet.Graphics
{
	/// <summary>
	/// A class representing a window from the main HexChat application.
	/// </summary>
	public sealed class HexChatWindow
	{
		private Window Window;

		public HexChatWindow(IntPtr gtkWinPtr)
		{
			Window = new Window(gtkWinPtr);
			
		}
	}
}
