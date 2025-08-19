using System;
using System.Runtime.InteropServices;

namespace System.Windows.Forms.Html
{
    [
        Guid(@"EAB22AC2-30C1-11CF-A7EB-0000C05BAE0B"), 
        InterfaceType(ComInterfaceType.InterfaceIsIDispatch)
    ]
    public interface IWebBrowserEvents 
    {	
		// disp Id's are from exdispid.h
        [DispId(100)] 
        void RaiseBeforeNavigate(string url, int flags, string targetFrameName, ref object postData, string headers, ref bool cancel); 
        [DispId(101)] 
        void RaiseNavigateComplete(string url); 
		[DispId(102)]
		void RaiseOnStatusTextChange(string text);
		[DispId(104)]
		void RaiseOnDownloadComplete();
		[DispId(105)]
		void RaiseOnCommandStateChange(int command, bool enable);
		[DispId(106)]
		void RaiseOnDownloadBegin();
		[DispId(107)]
		void RaiseOnNewWindow(string url, int flags, string targetFrameName, ref object postData, string headers, ref bool processed);
		[DispId(108)]
		void RaiseOnProgressChange(int progress, int progressMax);
		[DispId(112)]
		void RaiseOnPropertyChange(string property);
		[DispId(113)]
		void RaiseOnTitleChange(string url);

	} 

	[
	Guid(@"34A715A0-6587-11D0-924A-0020AFC7AC4D"), 
	InterfaceType(ComInterfaceType.InterfaceIsIDispatch)
	]
	public interface IWebBrowserEvents2 {	

		[DispId(250)]
		void BeforeNavigate2([In][MarshalAs(UnmanagedType.IDispatch)] object pDisp,
									string url, int flags, string targetFrameName, ref object postData, 
									string headers, ref bool cancel);

		[DispId(252)]
		void NavigateComplete2([In][MarshalAs(UnmanagedType.IDispatch)] object pDisp, string url);
		
		[DispId(259)]
		void DocumentComplete([In][MarshalAs(UnmanagedType.IDispatch)] object pDisp, string url );

		[DispId(263)]
		void WindowClosing(bool isChildWindow, ref bool cancel);

		/// <summary>
		/// http://msdn.microsoft.com/workshop/browser/webbrowser/reference/ifaces/dwebbrowserevents2/navigateerror.asp?frame=true
		/// </summary>
		[DispId(271)]
		void NavigateError([In][MarshalAs(UnmanagedType.IDispatch)] object pDisp,
			string url, string targetFrameName,	int statusCode, ref bool cancel);

	}
} 

