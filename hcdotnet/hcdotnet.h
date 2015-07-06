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

#define PROPERTY(n, s, t) static property t n { t get() { return GetPref_##t((s)); } void set(t value) { HexChat::Command("SET {0} {1}", (s), value); } }

#ifndef HEXCHAT_PLUGIN_H
struct hexchat_event_attrs;
#endif

namespace HexChatDotNet {
	ref class HexChatPlugin;
	ref class HexChat;
	ref class HexChatPrefs;

	interface class IHexChatHook;
	template<typename T> ref class HookBase;
	ref class CommandHook;
	ref class PrintHook;
	ref class ServerHook;
	ref class TimerHook;

	ref class CommandAttribute;
	ref class PrintEventAttribute;
	ref class ServerEventAttribute;

	ref class PrefDictionary;
	ref class HexChatEventAttrs;
	ref class HexChatContext;
	ref class HexChatList;
	ref class ChannelInfo;
	ref class DccTransferInfo;
	ref class IgnoreListInfo;
	ref class NotifyListInfo;
	ref class UserInfo;
	ref class ChannelModes;
	ref class UserModes;

	ref class Extensions;

	ref class PreferenceNotFoundException;
	ref class PreferenceTypeMismatchException;
	ref class ContextSwitchException;

	enum class Priority;
	enum class Eat;
	enum class Timer;
	enum class PrintEvent;
	enum class ServerEvent;
	enum class ModeAction : char;
	enum class StripFlags;
	enum class DccRecvMode;
	enum class FilePermissions;
	enum class LagometerType;
	enum class WindowStatus;
	enum class ChannelInfoFlags;
	enum class ChannelInfoType;
	enum class DccTransferStatus;
	enum class DccTransferType;
	enum class IgnoreListFlags;

	public delegate Eat CommandCallback(array<System::String^>^ word, array<System::String^>^ wordEol);
	public delegate Eat PrintCallback(array<System::String^>^ word, HexChatEventAttrs^ eventAttrs);
	public delegate Eat ServerCallback(array<System::String^>^ word, array<System::String^>^ wordEol, HexChatEventAttrs^ eventAttrs);
	public delegate Timer TimerCallback();

	#pragma region Enums

	public enum class Priority {
		Highest = 127,
		High = 64,
		Normal = 0,
		Low = -64,
		Lowest = -128
	};

	public enum class Eat {
		None = 0,
		HexChat = 1,
		Plugin = 2,
		All = 3
	};

	public enum class Timer {
		Remove = 0,
		Keep = 1
	};

	// list derived from src/common/textevents.in rev b41bd594b0324c4aa0d02ec35ad1efdcf2d4f6db
	public enum class PrintEvent {
		// Special events
		OpenContext,
		CloseContext,
		FocusTab,
		FocusWindow,
		DccChatText,
		KeyPress,
		// Actual text events
		AddNotify,
		BanList,
		Banned,
		Beep,
		CapabilityAcknowledgement,
		CapabilityList,
		CapabilityRequest,
		ChangeNick,
		ChannelAction,
		ChannelActionHilight,
		ChannelBan,
		ChannelCreation,
		ChannelDeHalfOp,
		ChannelDeOp,
		ChannelDeVoice,
		ChannelExempt,
		ChannelHalfOperator,
		ChannelInvite,
		ChannelList,
		ChannelMessage,
		ChannelModeGeneric,
		ChannelModes,
		ChannelMsgHilight,
		ChannelNotice,
		ChannelOperator,
		ChannelQuit,
		ChannelRemoveExempt,
		ChannelRemoveInvite,
		ChannelRemoveKeyword,
		ChannelRemoveLimit,
		ChannelSetKey,
		ChannelSetLimit,
		ChannelUnBan,
		ChannelUnQuiet,
		ChannelUrl,
		ChannelVoice,
		Connected,
		Connecting,
		ConnectionFailed,
		CtcpGeneric,
		CtcpGenericToChannel,
		CtcpSend,
		CtcpSound,
		CtcpSoundToChannel,
		DccChatAbort,
		DccChatConnect,
		DccChatFailed,
		DccChatOffer,
		DccChatOffering,
		DccChatReoffer,
		DccConnectionFailed,
		DccGenericOffer,
		DccHeader,
		DccMalformed,
		DccOffer,
		DccOfferNotValid,
		DccRecvAbort,
		DccRecvComplete,
		DccRecvConnect,
		DccRecvFailed,
		DccRecvFileOpenError,
		DccRename,
		DccResumeRequest,
		DccSendAbort,
		DccSendComplete,
		DccSendConnect,
		DccSendFailed,
		DccSendOffer,
		DccStall,
		DccTimeout,
		DeleteNotify,
		Disconnected,
		FoundIp,
		GenericMessage,
		IgnoreAdd,
		IgnoreChanged,
		IgnoreFooter,
		IgnoreHeader,
		IgnoreRemove,
		IgnorelistEmpty,
		Invite,
		Invited,
		Join,
		Keyword,
		Kick,
		Killed,
		MessageSend,
		Motd,
		MotdSkipped,
		NickClash,
		NickErroneous,
		NickFailed,
		NoDcc,
		NoRunningProcess,
		Notice,
		NoticeSend,
		NotifyAway,
		NotifyBack,
		NotifyEmpty,
		NotifyHeader,
		NotifyNumber,
		NotifyOffline,
		NotifyOnline,
		OpenDialog,
		Part,
		PartWithReason,
		PingReply,
		PingTimeout,
		PrivateAction,
		PrivateActionToDialog,
		PrivateMessage,
		PrivateMessageToDialog,
		ProcessAlreadyRunning,
		Quit,
		RawModes,
		ReceiveWallops,
		ResolvingUser,
		SaslAuthenticating,
		SaslResponse,
		ServerConnected,
		ServerError,
		ServerLookup,
		ServerNotice,
		ServerText,
		SslMessage,
		StopConnection,
		Topic,
		TopicChange,
		TopicCreation,
		UnknownHost,
		UserLimit,
		UsersOnChannel,
		WhoIsAuthenticated,
		WhoIsAwayLine,
		WhoIsChannelOperLine,
		WhoIsEnd,
		WhoIsIdentified,
		WhoIsIdleLine,
		WhoIsIdleLineWithSignon,
		WhoIsNameLine,
		WhoIsRealHost,
		WhoIsServerLine,
		WhoIsSpecial,
		YouJoin,
		YouKicked,
		YouPart,
		YouPartWithReason,
		YourAction,
		YourInvitation,
		YourMessage,
		YourNickChanging
	};

	// some common numerics are supported below, for numerics not listed use the
	// other overload to hook into them where you pass in a string
	public enum class ServerEvent {
		// special events
		RawLine,
		// commands
		PrivMsg,
		Notice,
		Join,
		Part,
		Quit,
		Kick,
		Nick,
		Away,
		Ping,
		// numerics
		ISupport
	};

	public enum class ModeAction : char {
		Add = '+',
		Remove = '-'
	};

	[System::FlagsAttribute]
	public enum class StripFlags {
		None = 0,
		Colors = 1,
		TextAttributes = 2
	};

	public enum class DccRecvMode {
		AskForConfirmation = 0,
		AskForDownloadFolder = 1,
		SaveWithoutInteraction = 2
	};

	[System::FlagsAttribute]
	public enum class FilePermissions {
		None = 0,
		// Individual Flags
		OtherExecute = 00001,
		OtherWrite = 00002,
		OtherRead = 00004,
		GroupExecute = 00010,
		GroupWrite = 00020,
		GroupRead = 00040,
		UserExecute = 00100,
		UserWrite = 00200,
		UserRead = 00400,
		Sticky = 01000,
		SetGid = 02000,
		SetUid = 04000,
		// Grouped Flags for easier setting
		AllExecute = 00111,
		AllWrite = 00222,
		AllRead = 00444,
		Standard = 00644,
		StandardExecute = 00755,
		Secure = 00600,
		SecureExecute = 00700
	};

	public enum class LagometerType {
		Off = 0,
		Graph = 1,
		Text = 2,
		Both = 3
	};

	public enum class WindowStatus {
		Unknown,
		Normal,
		Active,
		Hidden
	};

	[System::FlagsAttribute]
	public enum class ChannelInfoFlags {
		Connected = 0x000001,
		Connecting = 0x000002,
		MarkedAway = 0x000004,
		EndOfMotd = 0x000008,
		HasWhox = 0x000010,
		HasIdmsg = 0x000020,
		HideJoinParts = 0x000040,
		HideJoinPartsUnset = 0x000080,
		BeepOnMessage = 0x000100,
		BlinkTray = 0x000200,
		BlinkTaskbar = 0x000400,
		Logging = 0x000800,
		LoggingUnset = 0x001000,
		Scrollback = 0x002000,
		ScrollbackUnset = 0x004000,
		StripColors = 0x008000,
		StripColorsUnset = 0x010000
	};

	public enum class ChannelInfoType {
		Server = 1,
		Channel = 2,
		Dialog = 3,
		Notice = 4,
		SNotice = 5
	};

	public enum class DccTransferStatus {
		Queued = 0,
		Active = 1,
		Failed = 2,
		Done = 3,
		Connecting = 4,
		Aborted = 5
	};

	public enum class DccTransferType {
		Send = 0,
		Receive = 1,
		ChatRecv = 2,
		ChatSend = 3
	};

	[System::FlagsAttribute]
	public enum class IgnoreListFlags {
		Private = 0x01,
		Notice = 0x02,
		Channel = 0x04,
		Ctcp = 0x08,
		Invite = 0x10,
		Unignore = 0x20,
		NoSave = 0x40,
		Dcc = 0x80
	};

	#pragma endregion

	/// <summary>
	/// Base class that all plugins should inherit from.
	/// Each .dll file should only contain ONE non-abstract class that inherits from HexChatPlugin.
	/// </summary>
	public ref class HexChatPlugin abstract : System::IDisposable {
	public:
		HexChatPlugin();
		!HexChatPlugin();
		/// <summary>Plugin Name</summary>
		virtual property System::String^ Name { System::String^ get() abstract; }
		/// <summary>Plugin Description</summary>
		virtual property System::String^ Desc { System::String^ get() abstract; }
		/// <summary>Plugin Version</summary>
		virtual property System::String^ Version { System::String^ get() abstract; }

		virtual void Init() abstract;
		virtual ~HexChatPlugin();

	protected:
		CommandHook^ CreateCommandHook(System::String^ name, [System::Runtime::InteropServices::OptionalAttribute] System::String^ help, [System::Runtime::InteropServices::OptionalAttribute] Priority priority);
		PrintHook^ CreatePrintHook(PrintEvent eventName, [System::Runtime::InteropServices::OptionalAttribute] Priority priority);
		PrintHook^ CreatePrintHook(System::String^ eventName, [System::Runtime::InteropServices::OptionalAttribute] Priority priority);
		ServerHook^ CreateServerHook(ServerEvent eventName, [System::Runtime::InteropServices::OptionalAttribute] Priority priority);
		ServerHook^ CreateServerHook(System::String^ eventName, [System::Runtime::InteropServices::OptionalAttribute] Priority priority);
		TimerHook^ CreateTimerHook(int timeout);

		initonly PrefDictionary^ PluginPrefs;

	private:
		System::Collections::Generic::List<IHexChatHook^>^ _hooks;

	internal:
		System::IntPtr _ph;
		System::String^ _path;
	};

	public ref class HexChat abstract sealed {
	public:
		static void Command(System::String^ command);
		static void Command(System::String^ format, ... array<System::Object^>^ args);
		static void Print(System::String^ text);
		static void Print(System::String^ format, ... array<System::Object^>^ args);
		static bool EmitPrint(PrintEvent eventName, ... array<System::String^>^ args);
		static bool EmitPrint(PrintEvent eventName, HexChatEventAttrs^ eventAttrs, ... array<System::String^>^ args);
		static bool EmitPrint(System::String^ eventName, ... array<System::String^>^ args);
		static bool EmitPrint(System::String^ eventName, HexChatEventAttrs^ eventAttrs, ... array<System::String^>^ args);
		static void SendModes(System::Collections::Generic::IEnumerable<System::String^>^ targets, ModeAction modeAction, char mode);
		static void SendModes(System::Collections::Generic::IEnumerable<System::String^>^ targets, int modesPerLine, ModeAction modeAction, char mode);
		static int IrcStrCmp(System::String^ string1, System::String^ string2);
		static System::String^ Strip(System::String^ str, StripFlags flags);
		static HexChatContext^ FindContext(System::String^ server, System::String^ channel);
		static HexChatContext^ GetContext();
		static bool SetContext(HexChatContext^ context);
		static System::Collections::Generic::IEnumerable<ChannelInfo^>^ GetChannels();
		static System::Collections::Generic::IEnumerable<DccTransferInfo^>^ GetDccTransfers();
		static System::Collections::Generic::IEnumerable<IgnoreListInfo^>^ GetIgnoreList();
		static System::Collections::Generic::IEnumerable<NotifyListInfo^>^ GetNotifyList();
		static System::Collections::Generic::IEnumerable<UserInfo^>^ GetUsers();
		static System::String^ GetEventText(PrintEvent eventName);
		static System::String^ GetEventText(System::String^ eventName);
		static System::String^ GetPluginApiVersion(bool includeBuildMetadata);
		static System::String^ GetTranslatedMessage(System::String^ msgId);

		static property System::String^ ConfigDir { System::String^ get(); }
		static property System::String^ LibraryDir { System::String^ get(); }
		static property System::String^ Version { System::String^ get(); }

		static System::IntPtr PluginGuiAdd(System::String^ filename, System::String^ name, System::String^ desc, System::String^ version);
		static void PluginGuiRemove(System::IntPtr pluginHandle);

		// Low-level methods; read the documentation for preferred alternatives and caveats
		static System::String^ GetInfo(System::String^ id);
		static System::Type^ GetPref(System::String^ pref, [System::Runtime::InteropServices::OutAttribute] System::String^% sval, [System::Runtime::InteropServices::OutAttribute] System::Nullable<int>% ival, [System::Runtime::InteropServices::OutAttribute] System::Nullable<bool>% bval);
		static HexChatList^ GetList(System::String^ name);
	};

	public ref class HexChatPrefs abstract sealed {
		static System::String^ GetPref_string(System::String^ pref);
		static int GetPref_int(System::String^ pref);
		static bool GetPref_bool(System::String^ pref);
		typedef System::String^ string;

	public:
		PROPERTY(AwayAutoUnmark, "away_auto_unmark", bool)
		PROPERTY(AwayOmitAlerts, "away_omit_alerts", bool)
		PROPERTY(AwayReason, "away_reason", string)
		PROPERTY(AwayShowOnce, "away_show_once", bool)
		PROPERTY(AwaySizeMax, "away_size_max", int)
		PROPERTY(AwayTimeout, "away_timeout", int)
		PROPERTY(AwayTrack, "away_track", bool)

		PROPERTY(CompletionAmount, "completion_amount", int)
		PROPERTY(CompletionAuto, "completion_auto", bool)
		PROPERTY(CompletionSort, "completion_sort", int) // TODO: determine if enum
		PROPERTY(CompletionSuffix, "completion_suffix", string)

		PROPERTY(DccAutoChat, "dcc_auto_chat", bool)
		static property DccRecvMode DccAutoRecv { // TODO: check enum values
			DccRecvMode get() { return safe_cast<DccRecvMode>(GetPref_int("dcc_auto_recv")); }
			void set(DccRecvMode value) { HexChat::Command("SET dcc_recv_mode {0}", safe_cast<int>(value)); }
		}
		PROPERTY(DccAutoResume, "dcc_auto_resume", bool)
		PROPERTY(DccBlocksize, "dcc_blocksize", int)
		PROPERTY(DccCompletedDir, "dcc_completed_dir", string)
		PROPERTY(DccDir, "dcc_dir", string)
#ifndef WIN32
		PROPERTY(DccFastSend, "dcc_fast_send", bool)
#endif
		PROPERTY(DccGlobalMaxGetCps, "dcc_global_max_get_cps", int)
		PROPERTY(DccGlobalMaxSendCps, "dcc_global_max_send_cps", int)
		PROPERTY(DccIp, "dcc_ip", string)
		PROPERTY(DccIpFromServer, "dcc_ip_from_server", bool)
		PROPERTY(DccMaxGetCps, "dcc_max_get_cps", int)
		PROPERTY(DccMaxSendCps, "dcc_max_send_cps", int)
		static property FilePermissions DccPermissions {
			FilePermissions get() { return safe_cast<FilePermissions>(GetPref_int("dcc_permissions")); }
			void set(FilePermissions value) { HexChat::Command("SET dcc_permissions {0}", safe_cast<int>(value)); }
		}
		PROPERTY(DccPortFirst, "dcc_port_first", int)
		PROPERTY(DccPortLast, "dcc_port_last", int)
		PROPERTY(DccRemove, "dcc_remove", bool)
		PROPERTY(DccSaveNick, "dcc_save_nick", bool)
		PROPERTY(DccSendFillspaces, "dcc_send_fillspaces", bool)
		PROPERTY(DccStallTimeout, "dcc_stall_timeout", int)
		PROPERTY(DccTimeout, "dcc_timeout", int)

		PROPERTY(FloodCtcpNum, "flood_ctcp_num", int)
		PROPERTY(FloodCtcpTime, "flood_ctcp_time", int)
		PROPERTY(FloodMsgNum, "flood_msg_num", int)
		PROPERTY(FloodMsgTime, "flood_msg_time", int)

		PROPERTY(GuiAutoopenChat, "gui_autoopen_chat", bool)
		PROPERTY(GuiAutoopenDialog, "gui_autoopen_dialog", bool)
		PROPERTY(GuiAutoopenRecv, "gui_autoopen_recv", bool)
		PROPERTY(GuiAutoopenSend, "gui_autoopen_send", bool)
		PROPERTY(GuiChanlistMaxusers, "gui_chanlist_maxusers", int)
		PROPERTY(GuiChanlistMinusers, "gui_chanlist_minusers", int)
		PROPERTY(GuiCompact, "gui_compact", bool)
		PROPERTY(GuiDialogHeight, "gui_dialog_height", int)
		PROPERTY(GuiDialogLeft, "gui_dialog_left", int)
		PROPERTY(GuiDialogTop, "gui_dialog_top", int)
		PROPERTY(GuiDialogWidth, "gui_dialog_width", int)
		PROPERTY(GuiFilesizeIec, "gui_filesize_iec", bool)
		PROPERTY(GuiFocusOmitalerts, "gui_focus_omitalerts", bool)
		PROPERTY(GuiHideMenu, "gui_hide_menu", bool)
		PROPERTY(GuiInputAttr, "gui_input_attr", bool)
		PROPERTY(GuiInputIcon, "gui_input_icon", bool)
		PROPERTY(GuiInputNick, "gui_input_nick", bool)
		PROPERTY(GuiInputSpell, "gui_input_spell", bool)
		PROPERTY(GuiInputStyle, "gui_input_style", bool)
		PROPERTY(GuiJoinDialog, "gui_join_dialog", bool)
		static property LagometerType GuiLagometer { // TODO: check enum values
			LagometerType get() { return safe_cast<LagometerType>(GetPref_int("gui_lagometer")); }
			void set(LagometerType value) { HexChat::Command("SET gui_lagometer {0}", safe_cast<int>(value)); }
		}
		PROPERTY(GuiLang, "gui_lang", int) // TODO: make into enum property
		PROPERTY(GuiModeButtons, "gui_mode_buttons", bool)
		PROPERTY(GuiPaneDividerPosition, "gui_pane_divider_position", int)
		PROPERTY(GuiPaneLeftSize, "gui_pane_left_size", int)
		PROPERTY(GuiPaneRightSize, "gui_pane_right_size", int)
		PROPERTY(GuiPaneRightSizeMin, "gui_pane_right_size_min", int)
		PROPERTY(GuiQuitDialog, "gui_quit_dialog", bool)
		PROPERTY(GuiSearchPos, "gui_search_pos", int) // TODO: determine if enum
		/* PROPERTY(GuiSingle, "gui_single", bool) */
		PROPERTY(GuiSlistFav, "gui_slist_fav", bool)
		PROPERTY(GuiSlistSelect, "gui_slist_select", int)
		PROPERTY(GuiSlistSkip, "gui_slist_skip", bool)
		PROPERTY(GuiTabChans, "gui_tab_chans", bool)
		PROPERTY(GuiTabDialogs, "gui_tab_dialogs", bool)
		PROPERTY(GuiTabDots, "gui_tab_dots", bool)
		PROPERTY(GuiTabIcons, "gui_tab_icons", bool)
		PROPERTY(GuiTabLayout, "gui_tab_layout", int) // TODO: make into enum property
		PROPERTY(GuiTabMiddleclose, "gui_tab_middleclose", bool)
		PROPERTY(GuiTabNewtofront, "gui_tab_newtofront", int) // TODO: make into enum property
		PROPERTY(GuiTabPos, "gui_tab_pos", int) // TODO: make into enum property
		PROPERTY(GuiTabScrollchans, "gui_tab_scrollchans", bool)
		PROPERTY(GuiTabServer, "gui_tab_server", bool)
		PROPERTY(GuiTabSmall, "gui_tab_small", int) // TODO: make into enum property
		PROPERTY(GuiTabSort, "gui_tab_sort", bool)
		PROPERTY(GuiTabTrunc, "gui_tab_trunc", int)
		PROPERTY(GuiTabUtils, "gui_tab_utils", bool)
		PROPERTY(GuiThrottlemeter, "gui_throttlemeter", int) // TODO: make into enum property
		PROPERTY(GuiTopicbar, "gui_topicbar", bool)
		PROPERTY(GuiTransparency, "gui_transparency", int) // TODO: determine if enum
		PROPERTY(GuiTray, "gui_tray", bool)
		PROPERTY(GuiTrayAway, "gui_tray_away", bool)
		PROPERTY(GuiTrayBlink, "gui_tray_blink", bool)
		PROPERTY(GuiTrayClose, "gui_tray_close", bool)
		PROPERTY(GuiTrayMinimize, "gui_tray_minimize", bool)
		PROPERTY(GuiTrayQuiet, "gui_tray_quiet", bool)
		PROPERTY(GuiUlistButtons, "gui_ulist_buttons", bool)
		PROPERTY(GuiUlistColor, "gui_ulist_color", bool)
		PROPERTY(GuiUlistCount, "gui_ulist_count", bool)
		PROPERTY(GuiUlistDoubleclick, "gui_ulist_doubleclick", string)
		PROPERTY(GuiUlistHide, "gui_ulist_hide", bool)
		PROPERTY(GuiUlistIcons, "gui_ulist_icons", bool)
		PROPERTY(GuiUlistPos, "gui_ulist_pos", int) // TODO: make into enum property
		PROPERTY(GuiUlistResizable, "gui_ulist_resizable", bool)
		PROPERTY(GuiUlistShowHosts, "gui_ulist_show_hosts", bool)
		PROPERTY(GuiUlistSort, "gui_ulist_sort", int) // TODO: make into enum property
		PROPERTY(GuiUlistStyle, "gui_ulist_style", bool)
		PROPERTY(GuiUrlMod, "gui_url_mod", int) // TODO: make into enum property
		PROPERTY(GuiUsermenu, "gui_usermenu", bool)
		PROPERTY(GuiWinHeight, "gui_win_height", int)
		PROPERTY(GuiWinFullscreen, "gui_win_fullscreen", int) // TODO: determine if enum
		PROPERTY(GuiWinLeft, "gui_win_left", int)
		PROPERTY(GuiWinModes, "gui_win_modes", bool)
		PROPERTY(GuiWinSave, "gui_win_save", bool)
		PROPERTY(GuiWinState, "gui_win_state", int) // TODO: determine if enum
		PROPERTY(GuiWinSwap, "gui_win_swap", bool)
		PROPERTY(GuiWinTop, "gui_win_top", int)
		PROPERTY(GuiWinUcount, "gui_win_ucount", bool)
		PROPERTY(GuiWinWidth, "gui_win_width", int)

		PROPERTY(Identd, "identd", bool)
		PROPERTY(IdentdPort, "identd_port", int)

		PROPERTY(InputBalloonChans, "input_balloon_chans", bool)
		PROPERTY(InputBalloonHilight, "input_balloon_hilight", bool)
		PROPERTY(InputBalloonPriv, "input_balloon_priv", bool)
		PROPERTY(InputBeepChans, "input_beep_chans", bool)
		PROPERTY(InputBeepHilight, "input_beep_hilight", bool)
		PROPERTY(InputBeepPriv, "input_beep_priv", bool)
		PROPERTY(InputCommandChar, "input_command_char", string)
		PROPERTY(InputFilterBeep, "input_filter_beep", bool)
		PROPERTY(InputFlashChans, "input_flash_chans", bool)
		PROPERTY(InputFlashHilight, "input_flash_hilight", bool)
		PROPERTY(InputFlashPriv, "input_flash_priv", bool)
		PROPERTY(InputPercAscii, "input_perc_ascii", bool)
		PROPERTY(InputPercColor, "input_perc_color", bool)
		PROPERTY(InputTrayChans, "input_tray_chans", bool)
		PROPERTY(InputTrayHilight, "input_tray_hilight", bool)
		PROPERTY(InputTrayPriv, "input_tray_priv", bool)

		PROPERTY(IrcAutoRejoin, "irc_auto_rejoin", bool)
		PROPERTY(IrcBanType, "irc_ban_type", int) // TODO: make into enum property
		PROPERTY(IrcCapServerTime, "irc_cap_server_time", bool)
		PROPERTY(IrcConfMode, "irc_conf_mode", bool)
		PROPERTY(IrcExtraHilight, "irc_extra_hilight", string)
		PROPERTY(IrcHideNickchange, "irc_hide_nickchange", bool)
		PROPERTY(IrcHideVersion, "irc_hide_version", bool)
		PROPERTY(IrcHidehost, "irc_hidehost", bool)
		PROPERTY(IrcIdNtext, "irc_id_ntext", string)
		PROPERTY(IrcIdYtext, "irc_id_ytext", string)
		PROPERTY(IrcInvisible, "irc_invisible", bool)
		PROPERTY(IrcJoinDelay, "irc_join_delay", int)
		PROPERTY(IrcLogging, "irc_logging", bool)
		PROPERTY(IrcLogmask, "irc_logmask", string)
		PROPERTY(IrcNick1, "irc_nick1", string)
		PROPERTY(IrcNick2, "irc_nick2", string)
		PROPERTY(IrcNick3, "irc_nick3", string)
		PROPERTY(IrcNickHilight, "irc_nick_hilight", string)
		PROPERTY(IrcNoHilight, "irc_no_hilight", string)
		PROPERTY(IrcNoticePos, "irc_notice_pos", int) // TODO: determine if enum
		PROPERTY(IrcPartReason, "irc_part_reason", string)
		PROPERTY(IrcQuitReason, "irc_quit_reason", string)
		PROPERTY(IrcRawModes, "irc_raw_modes", bool)
		PROPERTY(IrcRealName, "irc_real_name", string)
		PROPERTY(IrcServernotice, "irc_servernotice", bool)
		PROPERTY(IrcSkipMotd, "irc_skip_motd", bool)
		PROPERTY(IrcUserName, "irc_user_name", string)
		PROPERTY(IrcWallops, "irc_wallops", bool)
		PROPERTY(IrcWhoJoin, "irc_who_join", bool)
		PROPERTY(IrcWhoisFront, "irc_whois_front", bool)

		PROPERTY(NetAutoReconnect, "net_auto_reconnect", bool)
#ifndef WIN32
		PROPERTY(NetAutoReconnectonfail, "net_auto_reconnectonfail", bool)
#endif
		PROPERTY(NetBindHost, "net_bind_host", string)
		PROPERTY(NetPingTimeout, "net_ping_timeout", int)
		PROPERTY(NetProxyAuth, "net_proxy_auth", bool)
		PROPERTY(NetProxyHost, "net_proxy_host", string)
		PROPERTY(NetProxyPass, "net_proxy_pass", string)
		PROPERTY(NetProxyPort, "net_proxy_port", int)
		PROPERTY(NetProxyType, "net_proxy_type", int) // TODO: determine if enum
		PROPERTY(NetProxyUser, "net_proxy_user", string)
		PROPERTY(NetReconnectDelay, "net_reconnect_delay", int)
		PROPERTY(NetThrottle, "net_throttle", bool)

		PROPERTY(NotifyTimeout, "notify_timeout", int)
		PROPERTY(NotifyWhoisOnline, "notify_whois_online", bool)

		PROPERTY(PerlWarnings, "perl_warnings", bool)

		PROPERTY(StampLog, "stamp_log", bool)
		PROPERTY(StampLogFormat, "stamp_log_format", string)
		PROPERTY(StampText, "stamp_text", bool)
		PROPERTY(StampTextFormat, "stamp_text_format", string)

		PROPERTY(TextAutocopyColor, "text_autocopy_color", bool)
		PROPERTY(TextAutocopyStamp, "text_autocopy_stamp", bool)
		PROPERTY(TextAutocopyText, "text_autocopy_text", bool)
		PROPERTY(TextBackground, "text_background", string)
		PROPERTY(TextColorNicks, "text_color_nicks", bool)
		PROPERTY(TextFont, "text_font", string)
		PROPERTY(TextFontMain, "text_font_main", string)
		PROPERTY(TextFontAlternative, "text_font_alternative", string)
		PROPERTY(TextIndent, "text_indent", bool)
		PROPERTY(TextMaxIndent, "text_max_indent", int)
		PROPERTY(TextMaxLines, "text_max_lines", int)
		PROPERTY(TextReplay, "text_replay", bool)
		PROPERTY(TextSearchCaseMatch, "text_search_case_match", bool)
		PROPERTY(TextSearchHighlightAll, "text_search_highlight_all", bool)
		PROPERTY(TextSearchFollow, "text_search_follow", bool)
		PROPERTY(TextSearchRegexp, "text_search_regexp", bool)
		PROPERTY(TextShowMarker, "text_show_marker", bool)
		PROPERTY(TextShowSep, "text_show_sep", bool)
		PROPERTY(TextSpellLangs, "text_spell_langs", string)
		PROPERTY(TextStripcolorMsg, "text_stripcolor_msg", bool)
		PROPERTY(TextStripcolorReplay, "text_stripcolor_replay", bool)
		PROPERTY(TextStripcolorTopic, "text_stripcolor_topic", bool)
		PROPERTY(TextThinSep, "text_thin_sep", bool)
		PROPERTY(TextTransparent, "text_transparent", bool)
		PROPERTY(TextWordwrap, "text_wordwrap", bool)

		PROPERTY(UrlGrabber, "url_grabber", bool)
		PROPERTY(UrlGrabberLimit, "url_grabber_limit", int)
		PROPERTY(UrlLogging, "url_logging", bool)
	};

	#pragma region Hooks

	public interface class IHexChatHook {
	public:
		void Hook();
		void Unhook();
	};

	template<typename T>
	public ref class HookBase abstract : public IHexChatHook, public System::IDisposable {
		void* _hook;
		
	protected:
		initonly HexChatPlugin^ Parent;
		System::Runtime::InteropServices::GCHandle _gch;

		virtual void* HookInternal() abstract;

	internal:
		HookBase(HexChatPlugin^ parent) : Parent(parent), _hook(nullptr) { }
		~HookBase();

	public:
		event T Callback;
		virtual void Hook() sealed;
		virtual void Unhook() sealed;
	};

	public ref class CommandHook sealed : public HookBase<CommandCallback^> {
		System::String^ _name;
		System::String^ _help;
		Priority _priority;

	internal:
		CommandHook(HexChatPlugin^ parent, System::String^ name, System::String^ help, Priority priority)
			: HookBase<CommandCallback^>(parent), _name(name), _help(help), _priority(priority) { }

		int CallbackInternal(char** word, char** word_eol, void* user_data);

	protected:
		virtual void* HookInternal() override;
	};

	public ref class PrintHook sealed : public HookBase<PrintCallback^> {
		System::String^ _eventName;
		Priority _priority;

	internal:
		PrintHook(HexChatPlugin^ parent, System::String^ eventName, Priority priority)
			: HookBase<PrintCallback^>(parent), _eventName(eventName), _priority(priority) { }

		int CallbackInternal(char** word, hexchat_event_attrs* attrs, void* user_data);

	protected:
		virtual void* HookInternal() override;
	};

	public ref class ServerHook sealed : public HookBase<ServerCallback^>{
		System::String^ _eventName;
		Priority _priority;

	internal:
		ServerHook(HexChatPlugin^ parent, System::String^ eventName, Priority priority)
			: HookBase<ServerCallback^>(parent), _eventName(eventName), _priority(priority) { }

		int CallbackInternal(char** word, char** word_eol, hexchat_event_attrs* attrs, void* user_data);

	protected:
		virtual void* HookInternal() override;
	};

	public ref class TimerHook sealed : public HookBase<TimerCallback^>{
		int _timeout;

	internal:
		TimerHook(HexChatPlugin^ parent, int timeout)
			: HookBase<TimerCallback^>(parent), _timeout(timeout) { }

		int CallbackInternal(void* user_data);

	protected:
		virtual void* HookInternal() override;
	};

	#pragma endregion

	[System::Runtime::CompilerServices::ExtensionAttribute]
	public ref class Extensions abstract sealed {
	public:
		[System::Runtime::CompilerServices::ExtensionAttribute]
		static System::String^ ToHexChatString(PrintEvent eventName);

		[System::Runtime::CompilerServices::ExtensionAttribute]
		static System::String^ ToHexChatString(ServerEvent eventName);
	};

	#pragma region Attributes

	[System::AttributeUsageAttribute(System::AttributeTargets::Method)]
	public ref class CommandAttribute sealed : public System::Attribute {
	public:
		initonly System::String^ Name;
		initonly System::String^ Help;
		initonly Priority Priority;

		CommandAttribute(System::String^ name, System::String^ help, HexChatDotNet::Priority priority)
			: Name(name), Help(help), Priority(priority) { }
	};

	[System::AttributeUsageAttribute(System::AttributeTargets::Method)]
	public ref class PrintEventAttribute sealed : public System::Attribute {
	public:
		initonly System::String^ EventName;
		initonly Priority Priority;

		PrintEventAttribute(PrintEvent eventName, HexChatDotNet::Priority priority)
			: Priority(priority) { EventName = Extensions::ToHexChatString(eventName); }

		PrintEventAttribute(System::String^ eventName, HexChatDotNet::Priority priority)
			: EventName(eventName), Priority(priority) { }
	};

	[System::AttributeUsageAttribute(System::AttributeTargets::Method)]
	public ref class ServerEventAttribute sealed : public System::Attribute {
	public:
		initonly System::String^ EventName;
		initonly Priority Priority;

		ServerEventAttribute(ServerEvent eventName, HexChatDotNet::Priority priority)
			: Priority(priority) { EventName = Extensions::ToHexChatString(eventName); }

		ServerEventAttribute(System::String^ eventName, HexChatDotNet::Priority priority)
			: EventName(eventName), Priority(priority) { }
	};

	#pragma endregion

	public ref class PreferenceNotFoundException sealed : public System::Exception { };
	public ref class PreferenceTypeMismatchException sealed : public System::Exception { };
	public ref class ContextSwitchException sealed : public System::Exception { };

	public ref class PrefDictionary sealed :
		System::Collections::Generic::IDictionary<System::String^, System::Object^>,
		System::Collections::Generic::IReadOnlyDictionary<System::String^, System::Object^>
	{
		HexChatPlugin^ _parent;
		System::Collections::Generic::Dictionary<System::String^, System::Object^>^ _dict;

	internal:
		PrefDictionary(HexChatPlugin^ parent);

	public:
		virtual property System::Object^ default[System::String^] { System::Object^ get(System::String^ key); void set(System::String^ key, System::Object^ value); }
		virtual property int Count { int get(); }
		virtual property bool IsReadOnly { bool get(); }
		virtual property System::Collections::Generic::Dictionary<System::String^, System::Object^>::KeyCollection^ Keys { System::Collections::Generic::Dictionary<System::String^, System::Object^>::KeyCollection^ get(); }
		virtual property System::Collections::Generic::Dictionary<System::String^, System::Object^>::ValueCollection^ Values { System::Collections::Generic::Dictionary<System::String^, System::Object^>::ValueCollection^ get(); }
		
		virtual System::Collections::Generic::IEnumerator<System::Collections::Generic::KeyValuePair<System::String^, System::Object^>>^ GetEnumerator();
		virtual bool ContainsKey(System::String^ key);
		virtual void Add(System::String^ key, System::Object^ value);
		virtual bool Remove(System::String^ key);
		virtual void Clear();
		virtual bool TryGetValue(System::String^ key, [System::Runtime::InteropServices::OutAttribute] System::Object^% value);

		virtual property System::Collections::Generic::ICollection<System::String^>^ KeysCollection { System::Collections::Generic::ICollection<System::String^>^ get() = System::Collections::Generic::IDictionary<System::String^, System::Object^>::Keys::get; }
		virtual property System::Collections::Generic::IEnumerable<System::String^>^ KeysEnumerable { System::Collections::Generic::IEnumerable<System::String^>^ get() = System::Collections::Generic::IReadOnlyDictionary<System::String^, System::Object^>::Keys::get; }
		virtual property System::Collections::Generic::ICollection<System::Object^>^ ValuesCollection { System::Collections::Generic::ICollection<System::Object^>^ get() = System::Collections::Generic::IDictionary<System::String^, System::Object^>::Values::get; }
		virtual property System::Collections::Generic::IEnumerable<System::Object^>^ ValuesEnumerable { System::Collections::Generic::IEnumerable<System::Object^>^ get() = System::Collections::Generic::IReadOnlyDictionary<System::String^, System::Object^>::Values::get; }
		virtual System::Collections::IEnumerator^ GetNongenericEnumerator() = System::Collections::IEnumerable::GetEnumerator;
		virtual void AddPair(System::Collections::Generic::KeyValuePair<System::String^, System::Object^> item) = System::Collections::Generic::ICollection<System::Collections::Generic::KeyValuePair<System::String^, System::Object^>>::Add;
		virtual bool ContainsPair(System::Collections::Generic::KeyValuePair<System::String^, System::Object^> item) = System::Collections::Generic::ICollection<System::Collections::Generic::KeyValuePair<System::String^, System::Object^>>::Contains;
		virtual void CopyTo(array<System::Collections::Generic::KeyValuePair<System::String^, System::Object^>>^ array, int arrayIndex) = System::Collections::Generic::ICollection<System::Collections::Generic::KeyValuePair<System::String^, System::Object^>>::CopyTo;
		virtual bool RemovePair(System::Collections::Generic::KeyValuePair<System::String^, System::Object^> item) = System::Collections::Generic::ICollection<System::Collections::Generic::KeyValuePair<System::String^, System::Object^>>::Remove;
	};

	public ref class HexChatEventAttrs sealed : public System::IDisposable {
	public:
		HexChatEventAttrs();
		~HexChatEventAttrs();
		!HexChatEventAttrs();

		property System::DateTime ServerTimeUtc {
			System::DateTime get();
			void set(System::DateTime value);
		}

	internal:
		hexchat_event_attrs* _handle;
		HexChatEventAttrs(hexchat_event_attrs* handle);
	};

	public ref class HexChatContext sealed {
	internal:
		void* _handle;
		HexChatContext(void* handle) : _handle(handle) { };

	public:
		property System::String^ AwayReason { System::String^ get(); }
		property System::String^ Channel { System::String^ get(); }
		property System::String^ Charset { System::String^ get(); }
		property System::IntPtr^ GtkWinPtr { System::IntPtr^ get(); }
		property System::String^ Host { System::String^ get(); }
		property System::String^ InputBoxText { System::String^ get(); void set(System::String^ value); }
		property ChannelModes^ ChannelModes { HexChatDotNet::ChannelModes^ get(); }
		property System::String^ Network { System::String^ get(); }
		property System::String^ Nick { System::String^ get(); }
		property System::String^ NickServPassword { System::String^ get(); }
		property System::String^ Server { System::String^ get(); }
		property System::String^ Topic { System::String^ get(); }
		property System::IntPtr^ WinPtr { System::IntPtr^ get(); }
		property WindowStatus WindowStatus { HexChatDotNet::WindowStatus get(); }

		void SetCursorPos(int newPos, bool relative);
	};

	public ref class HexChatList sealed : public System::IDisposable {
		void* _list;
		System::String^ _name;
		System::Collections::Generic::List<System::String^>^ _fields;

	internal:
		HexChatList(System::String^ name, void* list);

	public:
		~HexChatList();
		!HexChatList();

		property System::Collections::Generic::List<System::String^>^ Fields { System::Collections::Generic::List<System::String^>^ get(); }

		bool MoveNext();
		System::String^ GetString(System::String^ key);
		int GetInt(System::String^ key);
		System::DateTime GetTime(System::String^ key);
	};

	public ref class ChannelModes sealed {
		System::String^ _modes;

	public:
		ChannelModes(System::String^ modes) : _modes(modes) { }

		property System::String^ Modes { System::String^ get() { return _modes; } }
	};

	public ref class UserModes sealed {
		System::String^ _modes;

	public:
		UserModes(System::String^ modes) : _modes(modes) { }

		property System::String^ Modes { System::String^ get() { return _modes; } }
	};

#undef PROPERTY // Prevents compiler warnings from the redefinition below
#define PROPERTY(t, n) private: t _##n; public: property t n { t get() { return _##n; } private: void set(t value) { _##n = value; } }

	public ref class ChannelInfo sealed {
	internal:
		ChannelInfo(System::IntPtr list);

	public:
		PROPERTY(System::String^, Channel)
		PROPERTY(System::String^, ChannelKey)
		PROPERTY(System::String^, ChanTypes)
		PROPERTY(HexChatContext^, Context)
		PROPERTY(ChannelInfoFlags, Flags)
		PROPERTY(int, Id)
		PROPERTY(int, Lag)
		PROPERTY(int, MaxModes)
		PROPERTY(System::String^, Network)
		PROPERTY(System::String^, NickPrefixes)
		PROPERTY(UserModes^, NickModes)
		PROPERTY(int, SendQueueLength)
		PROPERTY(System::String^, Server)
		PROPERTY(ChannelInfoType, Type)
		PROPERTY(int, NumUsers)
	};

	public ref class DccTransferInfo sealed {
	internal:
		DccTransferInfo(System::IntPtr list);

	public:
		PROPERTY(System::Net::IPAddress^, Address)
		PROPERTY(int, BytesPerSecond)
		PROPERTY(System::String^, FullName)
		PROPERTY(System::String^, FileName)
		PROPERTY(System::String^, Nick)
		PROPERTY(int, Port)
		PROPERTY(unsigned long long, BytesSent)
		PROPERTY(unsigned long long, ResumePos)
		PROPERTY(unsigned long long, FileSize)
		PROPERTY(DccTransferStatus, Status)
		PROPERTY(DccTransferType, Type)
	};

	public ref class IgnoreListInfo sealed {
	internal:
		IgnoreListInfo(System::IntPtr list);

	public:
		PROPERTY(System::String^, Mask)
		PROPERTY(IgnoreListFlags, Flags)
	};

	public ref class NotifyListInfo sealed {
	internal:
		NotifyListInfo(System::IntPtr list);

	public:
		PROPERTY(System::Collections::Generic::List<System::String^>^, Networks)
		PROPERTY(System::String^, Nick)
		PROPERTY(bool, IsOnline)
		PROPERTY(System::DateTime^, LastOn)
		PROPERTY(System::DateTime^, LastOff)
		PROPERTY(System::DateTime^, LastSeen)
	};

	public ref class UserInfo sealed {
	internal:
		UserInfo(System::IntPtr list);

	public:
		PROPERTY(System::String^, Account)
		PROPERTY(bool, IsAway)
		PROPERTY(System::DateTime^, LastTalk)
		PROPERTY(System::String^, Nick)
		PROPERTY(System::String^, Host)
		PROPERTY(System::String^, Prefix)
		PROPERTY(System::String^, RealName)
		PROPERTY(bool, IsSelected)
	};
}

// clean up #defines
#undef PROPERTY
