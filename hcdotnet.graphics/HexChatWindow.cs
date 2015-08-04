using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using HexChatDotNet;
using HexChatDotNet.Graphics.Localization;

namespace HexChatDotNet.Graphics
{
	/// <summary>
	/// A class representing a window from the main HexChat application.
	/// </summary>
	public sealed class HexChatWindow
	{
		private IntPtr _handle;

		private NativeWindow Window
		{
			get
			{
				NativeWindow window = new NativeWindow();
				window.AssignHandle(_handle);
				return window;
			}
		}

		public delegate void MessageBoxCallback(HexChatWindow window, MessageBoxButton button);

		internal HexChatWindow(IntPtr winPtr)
		{
			_handle = winPtr;
		}

		/// <summary>
		/// Displays a message box to the user
		/// </summary>
		/// <param name="title">Message box title</param>
		/// <param name="message">Message shown to the user</param>
		/// <param name="callback">Function called when the user presses a button or closes the message box</param>
		/// <param name="type">Which buttons are available for the user to press</param>
		public void MessageBox(string title, string message, MessageBoxCallback callback, MessageBoxType type = MessageBoxType.Ok)
		{
			System.Windows.Forms.MessageBoxButtons buttons = System.Windows.Forms.MessageBoxButtons.OK;
			MessageBoxButton button = MessageBoxButton.None;

			if (title == null)
			{
				throw new ArgumentNullException("title");
			}

			if (message == null)
			{
				throw new ArgumentNullException("message");
			}

			switch (type)
			{
			case MessageBoxType.Ok:
				buttons = MessageBoxButtons.OK;
				break;
			case MessageBoxType.OkCancel:
				buttons = MessageBoxButtons.OKCancel;
				break;
			case MessageBoxType.RetryCancel:
				buttons = MessageBoxButtons.RetryCancel;
				break;
			case MessageBoxType.YesNo:
				buttons = MessageBoxButtons.YesNo;
				break;
			case MessageBoxType.YesNoCancel:
				buttons = MessageBoxButtons.YesNoCancel;
				break;
			case MessageBoxType.CancelTryAgainContinue:
				buttons = MessageBoxButtons.AbortRetryIgnore;
				break;
			}

			DialogResult result = System.Windows.Forms.MessageBox.Show(Window, message, title, buttons);

			switch (result)
			{
			case DialogResult.OK:
				button = MessageBoxButton.Ok;
				break;
			case DialogResult.Cancel:
				button = MessageBoxButton.Cancel;
				break;
			case DialogResult.Retry:
				button = MessageBoxButton.Retry;
				break;
			case DialogResult.Abort:
				button = MessageBoxButton.Cancel;
				break;
			case DialogResult.Yes:
				button = MessageBoxButton.Yes;
				break;
			case DialogResult.No:
				button = MessageBoxButton.No;
				break;
			case DialogResult.Ignore:
				button = MessageBoxButton.Continue;
				break;
			default:
				button = MessageBoxButton.None;
				break;
			}

			try
			{
				// TODO: run callback in new thread? Need to check if a misbehaving callback can hang the UI or not to see if that is necessary
				if (callback != null)
				{
					callback(this, button);
				}
			}
			catch (Exception e)
			{
				HexChatInternal.ReportException(e);
			}
		}
	}
}
