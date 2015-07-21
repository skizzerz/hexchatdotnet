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
			return new HexChatWindow(context.GtkWinPtr);
		}
	}
}
