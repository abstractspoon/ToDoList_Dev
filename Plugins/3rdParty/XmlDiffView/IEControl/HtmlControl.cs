using System;
using System.IO;
using System.Windows.Forms;
using System.ComponentModel;
using System.Runtime.InteropServices;

namespace System.Windows.Forms.Html {
	
	public class HtmlControl : AxHost, IWebBrowserEvents {

		public event BrowserNavigateCancelEventHandler BeforeNavigate;
		public event BrowserTranslateUrlEventHandler TranslateUrl;
		public event BrowserNavigateErrorCancelEventHandler NavigateError;	
		public event BrowserNavigateEventHandler NavigateComplete;
		public event BrowserNavigateEventHandler DocumentComplete;

		public event BrowserTitleChangeEventHandler TitleChanged;
		public event BrowserStatusTextChangedEventHandler StatusTextChanged;
		public event BrowserProgressChangedEventHandler ProgressChanged;
		public event BrowserCommandStateChangedEventHandler CommandStateChanged;
		public event EventHandler DownloadBegin, DownloadCompleted;
		public event BrowserNewWindowCancelEventHandler NewWindow;
		public event BrowserPropertyChangeEventHandler PropertyChanged;
		public event BrowserContextMenuCancelEventHandler ShowContextMenu;
		public event KeyEventHandler TranslateAccelerator;
		public event EventHandler Quit;

		bool activate,
			allowInPlaceNavigation,
			border3d, 
			flatScrollBars,
			scriptEnabled,
			activeXEnabled,
			javaEnabled,
			bgSoundEnabled,
			imagesDownloadEnabled,
			videoEnabled,
			scrollBarsEnabled;
		
		object scriptObject;

		string url = String.Empty;
		string html = String.Empty;
		string body = String.Empty;

		private IHTMLDocument2 _document;

		IWebBrowser2 control = null;
		ConnectionPointCookie cookie;
		private DocHostUIHandler _uiHandler;

		public HtmlControl() : base("8856f961-340a-11d0-a96b-00c04fd705a2") {
			
			HandleCreated += new EventHandler(SelfHandleCreated);
			HandleDestroyed += new EventHandler(SelfHandleDestroyed);
			NavigateComplete += new BrowserNavigateEventHandler(SelfNavigateComplete);
			
			scrollBarsEnabled = true;
			imagesDownloadEnabled = true;
			border3d = true;
			flatScrollBars = true;
		}

		public IHTMLDocument2 Document {
			get { return _document; }
		}

		public string Html {
			get { return this.html;	}
			set { 
				this.html = value;	if (this.html == null) this.html = String.Empty; 
				this.body = String.Empty;
			}
		}

		public string Body {
			get { return this.body;	}
			set { 
				this.body = value;	if (this.body == null) this.body = String.Empty; 
				this.html  = String.Empty;
			}
		}

		public void Activate() {
			activate = true;
		}

		public bool AllowInPlaceNavigation {
			get {	return allowInPlaceNavigation; }
			set {	allowInPlaceNavigation = value;	}
		}

		public bool Border3d {
			get {	return border3d;}
			set { border3d = value;}
		}

		public bool FlatScrollBars {
			get { return flatScrollBars;	}
			set { flatScrollBars = value;}
		}

		public bool ScriptEnabled {
			get {	return scriptEnabled;		}
			set { scriptEnabled = value;	}
		}

		public bool ActiveXEnabled {
			get {	return activeXEnabled;		}
			set { activeXEnabled = value;	}
		}

		public bool JavaEnabled {
			get {	return javaEnabled;	}
			set { javaEnabled = value;	}
		}

		public bool BackroundSoundEnabled {
			get {	return bgSoundEnabled;		}
			set { bgSoundEnabled = value;	}
		}

		public bool ImagesDownloadEnabled {
			get {	return imagesDownloadEnabled;	}
			set { imagesDownloadEnabled = value;	}
		}

		public bool VideoEnabled {
			get {	return videoEnabled;	}
			set { videoEnabled = value;	}
		}

		[Browsable(false)]
		public object ScriptObject {
			get { return scriptObject; }
			set { scriptObject = value;}
		}

		public bool ScrollBarsEnabled {
			get {	return scrollBarsEnabled;	}
			set { scrollBarsEnabled = value;	}
		}

		/// <summary>
		/// Navigate to a specific web site or refresh the display with the Html or Body
		/// content previously set.
		/// </summary>
		/// <param name="url">Web Site to navigate. Use an empty string or null as parameter
		/// value to activate the display with the Html/Body content.</param>
		public void Navigate(string url) {
			if (url == null || url == String.Empty) 
				url = "about:blank";

			if (!IsHandleCreated) {
				this.url = url;
				return;
			}

			Object flags = 0;
			Object targetFrame = String.Empty;
			Object postData = String.Empty;
			Object headers = String.Empty;

			try {
				control.Navigate(url, ref flags, ref targetFrame, ref postData, ref headers);
			} catch {}
		}

		public void GoBack() {
			if (!IsHandleCreated) return;
			if (control != null) control.GoBack(); 
		}
        
		public void GoForward() {
			if (!IsHandleCreated) return;
			if (control != null) control.GoForward(); 
		}
        
		public void GoHome() {
			if (!IsHandleCreated) return;
			if (control != null) control.GoHome(); 
		}
        
		public void GoSearch() {
			if (!IsHandleCreated) return;
			if (control != null) control.GoSearch(); 
		}

		public override void Refresh() {
			if (!IsHandleCreated) return;
			if (control != null) control.Refresh(); 
		}
		
		public void Stop() {
			if (!IsHandleCreated) return;
			if (control != null) control.Stop(); 
		}

		public void Clear() {
			this.Navigate("about:blank");
		}

		public new void Focus() {
			IHTMLDocument2 wbDoc;
			IHTMLElement2 wbBody;

			control.SetFocus();
			wbDoc = control.Document;
			if (wbDoc == null) {}
			else {
				wbBody = wbDoc.GetBody();
				if (wbBody == null) {}
				else
					wbBody.focus();
			}
		}

		private bool Offline {
			get { 
				return false;
			}
			set {
				// TODO: impl. take over code from SetIEOffline()::WinGUIWidgetHelpers.cs / RSSBandit, then make it public
			}
		}

		void SelfNavigateComplete(Object s, BrowserNavigateEventArgs e) {
			if (activate) DoVerb(Interop.OLEIVERB_UIACTIVATE);

			if (html != String.Empty) {
				this.SetHtmlText(html);
				RaiseOnDocumentComplete(e);
				this.html = String.Empty;
			}

			if (body != String.Empty) {
				SetBodyText(body);
				RaiseOnDocumentComplete(e);
				this.body = String.Empty;
			}

		}

		//TODO: correct signature and provide a delegate...?
		private int OnScriptError() {
			return 0;
		}

		// called from SelfNavigateComplete()
		protected void SetHtmlText(string text) {

			if (text == null) 
				text = String.Empty;

			if (!IsHandleCreated) 
				return;

			if (control != null) {
				
				IHTMLDocument2 document = control.Document;
				if (document != null) {

					if (activate) DoVerb(Interop.OLEIVERB_UIACTIVATE);
					// this way we can provide the FULL HTML incl. <head><style> etc.
					document.Open("", null, null, null);
					object[] a = new object[]{text};
					document.Write(a);
					document.Close();
					_document = document;
					// Without the "Refresh" command, the browser control doesn't
					// react to links containing the # character in the hyperlink.
					try {
						if (text.IndexOf("#") > 0)
							document.ExecCommand("Refresh", false, null);
					} catch {}
				}
			}
		}

		// called from SelfNavigateComplete()
		protected void SetBodyText(string text) {
			
			if (text == null) 
				text = String.Empty;

			if (!IsHandleCreated) 
				return;

			if (control != null) {
				IHTMLDocument2 document = control.Document;
				if (document != null) {
					_document = document;
					IHTMLElement body = document.GetBody();
					if (body != null) {
						if (activate) DoVerb(Interop.OLEIVERB_UIACTIVATE);
						body.SetInnerHTML(text);
						return;
					}
				}
			}
		}

		void SelfHandleCreated(object s, EventArgs e) {

			HandleCreated -= new EventHandler(SelfHandleCreated);

			if (url == null)		url = String.Empty;
			if (html == null)	html = String.Empty;
			if (body == null)	body = String.Empty;

			if (body == String.Empty && html == String.Empty)
				url = "about:blank";
		
			_uiHandler = new DocHostUIHandler(this);
			
			// need to SetClientSite() to enable set properties in GetHostInfo() and setFlags():
			Interop.IOleObject oleObj = control as Interop.IOleObject;
			if (oleObj != null)
				oleObj.SetClientSite(_uiHandler);

			BrowserEvents2Multicaster bec = new BrowserEvents2Multicaster(this);

			Navigate(url);

		}

		// this can be called multiple times in the lifetime of the control!!!
		void SelfHandleDestroyed(object s, EventArgs e) {
			_uiHandler = null;
		}

		protected override void WndProc(ref Message m) {
			if (m.Msg == Interop.WM_CLOSE) {
				base.WndProc (ref m);
				this.RaiseOnQuit();
			} else {
				base.WndProc (ref m);
			}
		}

		protected override void CreateSink() {
			try {
				cookie = new ConnectionPointCookie(base.GetOcx(), this,
					typeof(IWebBrowserEvents));  
			} 
		catch (Exception e){ 
				Console.Error.WriteLine("Error in CreateSink: "+e.ToString()); 
			}
		}

		protected override void DetachSink() {
			try { 
				cookie.Disconnect(); } 
			catch { }
		}

		protected override void AttachInterfaces() {
			try { 
				control = (IWebBrowser2) base.GetOcx();
			}
 			catch { }
		}

		protected override void Dispose(bool disposing) {
			if( disposing ) {
				// free own resources
			}
			base.Dispose (disposing);
		}

		protected override Boolean IsInputKey(Keys keyData) {
			return (keyData == Keys.Escape) ? false : base.IsInputKey(keyData);
		}

		internal void OnShowContextMenu(BrowserContextMenuCancelEventArgs e) {
			if (ShowContextMenu != null)
				ShowContextMenu(this, e);
		}

		internal void OnTranslateUrl(BrowserTranslateUrlEventArgs e) {
			if (TranslateUrl != null)
				TranslateUrl(this, e);
		}

		internal void OnTranslateAccelerator(KeyEventArgs e) {
			if (TranslateAccelerator!= null)
				TranslateAccelerator(this, e);
		}

		internal void RaiseOnQuit() {
			if (Quit != null) {
				try {
					Quit(this, new EventArgs());
				} catch {}
			}
		}

		private void Reset() {
			this.body = String.Empty;
			this.html = String.Empty;
			this.url = String.Empty;
			if (control != null)
				this._document = control.Document;
		}

		#region DWebBrowserEvents interface impl.

		public virtual void RaiseBeforeNavigate(String url, int flags, String
			targetFrameName, ref Object postData, String headers, ref bool cancel) {
			BrowserNavigateCancelEventArgs e = new BrowserNavigateCancelEventArgs(url, false);
			if (BeforeNavigate != null) BeforeNavigate(this, e);
			cancel = e.Cancel;
			if (cancel) {
				this.Reset();
			}
		}

		public virtual void RaiseNavigateComplete(String url) {
			BrowserNavigateEventArgs e = new BrowserNavigateEventArgs(url);
			if (NavigateComplete != null) NavigateComplete(this, e);
		}

		public virtual void RaiseOnTitleChange(string text) {
			if (TitleChanged != null) {
				TitleChanged(this, new BrowserTitleChangeEventArgs(text));
			}
		}

		public void RaiseOnStatusTextChange(string text) {
			if (StatusTextChanged != null)
				StatusTextChanged(this, new BrowserStatusChangeEventArgs(text));
		}

		public void RaiseOnDownloadComplete() {
			if (DownloadCompleted != null)
				DownloadCompleted(this, new EventArgs());
		}

		public void RaiseOnCommandStateChange(int command, bool enable) {
			if (CommandStateChanged != null)
				CommandStateChanged(this, new BrowserCommandStateChangeEventArgs(command, enable));
		}

		public void RaiseOnDownloadBegin() {
			if (DownloadBegin != null)
				DownloadBegin(this, new EventArgs());
		}

		public void RaiseOnNewWindow(string url, int flags, string targetFrameName, ref object postData, string headers, ref bool processed) {
			BrowserNavigateCancelEventArgs e = new BrowserNavigateCancelEventArgs(url, false);
			if (NewWindow != null) NewWindow(this, e);
			processed = e.Cancel;
			if (processed) this.Reset();
		}

		public void RaiseOnProgressChange(int progress, int progressMax) {
			if (ProgressChanged != null)
				ProgressChanged(this, new BrowserProgressChangeEventArgs(progress, progressMax));
		}

		public void RaiseOnPropertyChange(string property) {
			if (PropertyChanged != null)
				PropertyChanged(this, new BrowserPropertyChangeEventArgs(property));
		}

		#endregion

		#region IWebBrowserEvents2 Members

		public void RaiseOnNavigateError(BrowserNavigateErrorCancelEventArgs e) {
			_document = null;
			if (NavigateError != null) {
				NavigateError(this, e);
				if (e.Cancel) this.Reset();
			}
		}

		public void RaiseOnDocumentComplete(BrowserNavigateEventArgs e) {
			_document = control.Document;
			if (DocumentComplete != null) 
				DocumentComplete(this, e);
		}

		#endregion
	} 

	class BrowserEvents2Multicaster: IWebBrowserEvents2 {

		private UCOMIConnectionPoint icp;
		private int cookie = -1;
		private HtmlControl control;

		public BrowserEvents2Multicaster(HtmlControl hc) {
			// Call QueryInterface for IConnectionPointContainer
			control = hc ;
			UCOMIConnectionPointContainer icpc = 
				(UCOMIConnectionPointContainer)hc.GetOcx();

			// Find the connection point for the
			// DWebBrowserEvents2 source interface
			Guid g = typeof(IWebBrowserEvents2).GUID;
			icpc.FindConnectionPoint(ref g, out icp);
  
			// Pass a pointer to the host to the connection point
			icp.Advise(this, out cookie);
  
		}
		
		~BrowserEvents2Multicaster() {
			// End the connection
			try {
    			if (cookie != -1) 
                    icp.Unadvise(cookie);
			} catch { }
		}

		#region IWebBrowserEvents2 Members


		public void BeforeNavigate2(object pDisp, string url, int flags, string targetFrameName, ref object postData, string headers, ref bool cancel) {
			// not yet impl.
		}

		public void NavigateComplete2(object pDisp, string url) {
			// not yet impl.
		}

		public void DocumentComplete(object pDisp, string url) {

			try {
				IWebBrowser2 wb = (IWebBrowser2)pDisp;
				if (wb == control.GetOcx()) {
					// only fire on the topmost doc (frameset, not on every frame)
					// document is now ready (DOM accessable)
					control.RaiseOnDocumentComplete(new BrowserNavigateEventArgs(url));
				}
			} catch { }

		}

		public void WindowClosing(bool isChildWindow, ref bool cancel) {
			// why it did not get fired?
			control.RaiseOnQuit();
		}

		/// <summary>
		/// Available in IE6
		/// </summary>
		/// <param name="pDisp"></param>
		/// <param name="url"></param>
		/// <param name="frame"></param>
		/// <param name="statusCode"></param>
		/// <param name="cancel"></param>
		public void NavigateError(object pDisp, string url, string frame, int statusCode, ref bool cancel) {
			BrowserNavigateErrorCancelEventArgs e = new BrowserNavigateErrorCancelEventArgs(url, statusCode, false);
			control.RaiseOnNavigateError(e);
			cancel = e.Cancel;
		}

		#endregion
	}

} 

