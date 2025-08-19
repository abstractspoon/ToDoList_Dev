using System;
using System.ComponentModel;
using System.Runtime.InteropServices;

namespace System.Windows.Forms.Html
{
    [Guid(@"EAB22AC1-30C1-11CF-A7EB-0000C05BAE0B")] 
    interface IWebBrowser 
    {
        void GoBack(); 
        
        void GoForward(); 
        
        void GoHome(); 
        
        void GoSearch(); 
        
        void Navigate(string Url, ref object Flags, ref object targetFrame, ref object postData, ref object headers); 
        
        void Refresh(); 
        
        void Refresh2(); 
        
        void Stop(); 
        
        void GetApplication(); 
        
        void GetParent(); 
        
        void GetContainer(); 
        
		[Browsable(false)]
		IHTMLDocument2 Document {
			[DispId(203)]
			[return: MarshalAs(UnmanagedType.Interface)]
			get;
		}

	} 

	[Guid(@"D30C1661-CDAF-11d0-8A3E-00C04FC9E26E")] 
	interface IWebBrowser2: IWebBrowser {

		[DispId(302)]
		void PutProperty ([MarshalAs(UnmanagedType.BStr)] string Property,
			object vtValue);

		[DispId(303)]
		object GetProperty ([MarshalAs(UnmanagedType.BStr)] string Property);

		[DispId(500)]
		void Navigate2 ([In] ref object URL, [In] ref object Flags, [In] ref
			object TargetFrameName, [In] ref object PostData, [In] ref object Headers);

		[DispId(501)]
		Interop.OLECMDF QueryStatusWB (int cmdID);

		[DispId(502)]
		void ExecWB (int cmdID, Interop.OLECMDEXECOPT cmdexecopt, [In] ref object
			pvaIn, [In, Out] ref object pvaOut);

		[DispId(-2147414016)]
		void SetFocus ();

		[DispId(-2147414015)]
		void ZOrder (object Position);

		[DispId(-2147414014)]
		void CtlMove (float Left, object Top, object Width, object Height);

		[DispId(-2147414013)]
		void CtlDrag (object Action);

		[DispId(-2147414012)]
		void ShowWhatsThis ();

		[Browsable(false)]
		bool Busy {
			[DispId(212)]
			[return: MarshalAs(UnmanagedType.VariantBool)]
			get;
		}

		[Browsable(false)]
		bool CtlCausesValidation {
			[DispId(-2147418040)]
			[return: MarshalAs(UnmanagedType.VariantBool)]
			get;

			[DispId(-2147418040)]
			set;
		}

		[Browsable(false)]
		object CtlDragIcon {
			[DispId(-2147418102)]
			get;
			[DispId(-2147418102)]
			set;
		}

		[Browsable(false)]
		short CtlDragMode {
			[DispId(-2147418103)]
			get;
			[DispId(-2147418103)]
			set;
		}

		[Browsable(false)]
		float CtlHeight {
			[DispId(-2147418106)]
			get;
			[DispId(-2147418106)]
			set;
		}

		[Browsable(false)]
		int HelpContextID {
			[DispId(-2147418062)]
			get;
			[DispId(-2147418062)]
			set;
		}

		[Browsable(false)]
		int HWND {
			[DispId(-515)]
			get;
		}

		[Browsable(false)]
		short index {
			[DispId(-2147418111)]
			get;
		}

		[Browsable(false)]
		float CtlLeft {
			[DispId(-2147418109)]
			get;
			[DispId(-2147418109)]
			set;
		}

		[Browsable(false)]
		Interop.READYSTATE ReadyState {
			[DispId(-525)]
			get;
		}

		[Browsable(false)]
		bool RegisterAsBrowser {
			[DispId(552)]
			[return: MarshalAs(UnmanagedType.VariantBool)]
			get;
			[DispId(552)]
			set;
		}

		[Browsable(false)]
		bool RegisterAsDropTarget {
			[DispId(553)]
			[return: MarshalAs(UnmanagedType.VariantBool)]
			get;
			[DispId(553)]
			set;
		}

		[Browsable(false)]
		bool Resizable {
			[DispId(556)]
			[return: MarshalAs(UnmanagedType.VariantBool)]
			get;
			[DispId(556)]
			set;
		}

		[Browsable(false)]
		short TabIndex {
			[DispId(-2147418097)]
			get;
			[DispId(-2147418097)]
			set;
		}

		[Browsable(false)]
		bool TabStop {
			[DispId(-2147418098)]
			[return: MarshalAs(UnmanagedType.VariantBool)]
			get;
			[DispId(-2147418098)]
			set;
		}

		[Browsable(false)]
		string ToolTipText {
			[DispId(-2147418038)]
			[return: MarshalAs(UnmanagedType.BStr)]
			get;
			[DispId(-2147418038)]
			set;
		}

		[Browsable(false)]
		float CtlTop {
			[DispId(-2147418108)]
			get;
			[DispId(-2147418108)]
			set;
		}

		[Browsable(false)]
		bool TopLevelContainer {
			[DispId(204)]
			[return: MarshalAs(UnmanagedType.VariantBool)]
			get;
		}

		[Browsable(false)]
		string Type {
			[DispId(205)]
			[return: MarshalAs(UnmanagedType.BStr)]
			get;
		}

		[Browsable(false)]
		bool CtlVisible {
			[DispId(-2147418105)]
			[return: MarshalAs(UnmanagedType.VariantBool)]
			get;
			[DispId(-2147418105)]
			set;
		}
		
		[Browsable(false)]
		float CtlWidth {
			[DispId(-2147418107)]
			get;
			[DispId(-2147418107)]
			set;

		}
	}
} 

