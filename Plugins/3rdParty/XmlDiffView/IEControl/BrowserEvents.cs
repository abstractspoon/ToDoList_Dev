using System;
using System.Drawing;
using System.ComponentModel;

namespace System.Windows.Forms.Html
{
	#region Navigate events
	public delegate void BrowserNavigateEventHandler(object sender, BrowserNavigateEventArgs e);
	public delegate void BrowserNavigateCancelEventHandler(object sender, BrowserNavigateCancelEventArgs e);
	public delegate void BrowserNewWindowCancelEventHandler(object sender, BrowserNavigateCancelEventArgs e);
	public delegate void BrowserTranslateUrlEventHandler(object sender, BrowserTranslateUrlEventArgs e);
	public delegate void BrowserNavigateErrorCancelEventHandler(object sender, BrowserNavigateCancelEventArgs e);

	public class BrowserNavigateEventArgs : EventArgs {
		string url;
		public BrowserNavigateEventArgs(string url)	: base()	{	this.url = url;	}
		public string Url { get { return this.url;	} 	}
	} 

	public class BrowserNavigateCancelEventArgs : CancelEventArgs {
		string url;
    	public BrowserNavigateCancelEventArgs(string url, bool cancel)	: base(cancel)	{ this.url = url;	}
		public string Url { get { return this.url;	} 	}
	} 

	public class BrowserTranslateUrlEventArgs: BrowserNavigateEventArgs {
		string translatedUrl;
		public string TranslatedUrl { get { return this.translatedUrl; } set { this.translatedUrl = value; } 	}
		public BrowserTranslateUrlEventArgs(string url)	: base(url)	{	this.translatedUrl = url; /* assume to be the same */	}
	}

	public class BrowserNavigateErrorCancelEventArgs : BrowserNavigateCancelEventArgs {
		int statusCode;
		public BrowserNavigateErrorCancelEventArgs(string url, int statusCode, bool cancel)	: base(url, cancel)	{ this.statusCode = statusCode;	}
		public int StatusCode { get { return this.statusCode;	} 	}
	} 
	#endregion

	#region TitleChange event
	public delegate void BrowserTitleChangeEventHandler(object sender, BrowserTitleChangeEventArgs e);
	
	public class BrowserTitleChangeEventArgs : EventArgs {
		string title;
		public string Title { get {	return title;	}	}
		public BrowserTitleChangeEventArgs(string title) { this.title = title;	}
	}
	#endregion

	#region StatusTextChange event
	public delegate void BrowserStatusTextChangedEventHandler(object sender, BrowserStatusChangeEventArgs e);
	public class BrowserStatusChangeEventArgs : EventArgs {
		string status;
		public string Status { get {	return status;	}	}
		public BrowserStatusChangeEventArgs(string status) { this.status = status;	}
	}
	#endregion

	#region ProgressChange event
	public delegate void BrowserProgressChangedEventHandler(object sender, BrowserProgressChangeEventArgs e);
	public class BrowserProgressChangeEventArgs : EventArgs {
		int progress, progressMax;
		public int Progress { get {	return progress;	}	}
		public int ProgressMax { get {	return progressMax;	}	}
		public BrowserProgressChangeEventArgs(int progress, int progressMax) { this.progress = progress; this.progressMax = progressMax;	}
	}
	#endregion

	#region CommandStateChange event
	public enum BrowserCommandStates {
		UpdateCommands	= 0x0fffffff,
		NavigateForward	= 0x1,
		NavigateBack	= 0x2
	}
	public delegate void BrowserCommandStateChangedEventHandler(object sender, BrowserCommandStateChangeEventArgs e);
	public class BrowserCommandStateChangeEventArgs : EventArgs {
		BrowserCommandStates commandState;
		bool enable;
		public bool Enable { get {	return enable;	}	}
		public BrowserCommandStates CommandState { get {	return commandState;	}	}
		public BrowserCommandStateChangeEventArgs(int commandState, bool enable) { 
			this.enable = enable; 
			this.commandState = BrowserCommandStates.UpdateCommands;
			if (commandState == 1)
				this.commandState = BrowserCommandStates.NavigateForward;	
			else if  (commandState == 2)
				this.commandState = BrowserCommandStates.NavigateBack;	
		}
	}
	#endregion

	#region PropertyChange event
	public delegate void BrowserPropertyChangeEventHandler(object sender, BrowserPropertyChangeEventArgs e);
	
	public class BrowserPropertyChangeEventArgs : EventArgs {
		string property;
		public string Property { get {	return property;	}	}
		public BrowserPropertyChangeEventArgs(string property) { this.property = property;	}
	}
	#endregion

	#region ContextMenu event
	public delegate void BrowserContextMenuCancelEventHandler(object sender, BrowserContextMenuCancelEventArgs e);

	public class BrowserContextMenuCancelEventArgs : CancelEventArgs {
		Point  location;
		public BrowserContextMenuCancelEventArgs(Point loaction, bool cancel)	: base(cancel)	{ this.location = location;	}
		public Point Location { get { return this.location;	} 	}
	} 
	#endregion

} 

