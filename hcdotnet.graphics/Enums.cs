namespace HexChatDotNet.Graphics
{
	/// <summary>
	/// Defines which buttons are shown in a MessageBox
	/// </summary>
	public enum MessageBoxType
	{
		/// <summary>Shows a single OK button</summary>
		Ok = 0,
		/// <summary>Shows two buttons: OK and Cancel</summary>
		OkCancel,
		/// <summary>Shows two buttons: Retry and Cancel</summary>
		RetryCancel,
		/// <summary>Shows two buttons: Yes and No</summary>
		YesNo,
		/// <summary>Shows three buttons: Yes, No, and Cancel</summary>
		YesNoCancel,
		/// <summary>Shows three buttons: Cancel, Try Again, and Continue</summary>
		CancelTryAgainContinue
	}

	/// <summary>
	/// Contains which button was pressed for a MessageBox,
	/// or MessageBoxButton.None if the box was closed without a button being pressed
	/// </summary>
	public enum MessageBoxButton
	{
		None,
		Ok,
		Cancel,
		Retry,
		Yes,
		No,
		TryAgain,
		Continue
	}
}