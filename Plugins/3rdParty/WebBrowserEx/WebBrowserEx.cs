using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Runtime.InteropServices;
using System.Diagnostics;

namespace WebBrowserEx
{
	public delegate IntPtr GetDropTargetEventHandler(object sender, IntPtr defDropTarget);

    public class WebBrowserEx : WebBrowser
    {
		public event GetDropTargetEventHandler GetDropTarget;

		// -------------------------------------------------------------

		private void SetZoom(int percent)
		{
			int OLECMDID_OPTICAL_ZOOM = 63;
			int OLECMDEXECOPT_DONTPROMPTUSER = 2;

			if ((percent >= 10) && (percent <= 1000))
			{
				dynamic ax = this.ActiveXInstance;

				if (ax != null)
					ax.ExecWB(OLECMDID_OPTICAL_ZOOM, OLECMDEXECOPT_DONTPROMPTUSER, new VariantWrapper(percent), new VariantWrapper(0));
			}
		}

		/// <summary>
		/// provide custom WebBrowserSite,
		/// where we override IDocHostUIHandler and call the base implementation
		/// More info: http://stackoverflow.com/a/19739699/1768303
		/// </summary>
		protected override WebBrowserSiteBase CreateWebBrowserSiteBase()
		{
			return new WebBrowserSiteEx(this);
		}

		protected virtual IntPtr GetAlternativeDropTarget(IntPtr pDropTarget)
		{
			if (GetDropTarget != null)
				return GetDropTarget(this, pDropTarget);

			// else
			return IntPtr.Zero;
		}

		#region WebBrowserExSite

		[ClassInterface(ClassInterfaceType.None)]
		protected class WebBrowserSiteEx : WebBrowserSite,
			NativeMethods.IDocHostUIHandler,
			IDisposable,
			ICustomQueryInterface
		{
			WebBrowserEx _host;
			NativeMethods.IDocHostUIHandler _baseIDocHostUIHandler;

			IntPtr _unkInnerAggregated;
			IntPtr _unkOuter;
			Inner _inner;

			// constructor
			public WebBrowserSiteEx(WebBrowserEx host) : base(host)
			{
                // This code causes problems on XP
                if (host.Version.Major >= 9)
                {
				    _host = host;

				    // get the CCW object for this
				    _unkOuter = Marshal.GetIUnknownForObject(this);
				    Marshal.AddRef(_unkOuter);
				    try
				    {
					    // aggregate the CCW object with the helper Inner object
					    _inner = new Inner(this);
					    _unkInnerAggregated = Marshal.CreateAggregatedObject(_unkOuter, _inner);

					    // obtain private WebBrowserSite COM interfaces
					    _baseIDocHostUIHandler = (NativeMethods.IDocHostUIHandler)Marshal.GetTypedObjectForIUnknown(_unkInnerAggregated, typeof(NativeMethods.IDocHostUIHandler));
				    }
				    finally
				    {
					    Marshal.Release(_unkOuter);
				    }
                }
			}

			~WebBrowserSiteEx()
			{
				// need to work out the reference counting for GC to work correctly

				Debug.Print("WebBrowserExSite object finalized.");
			}

			void IDisposable.Dispose()
			{
				base.Dispose();

				_baseIDocHostUIHandler = null;

				if (_unkInnerAggregated != IntPtr.Zero)
				{
					Marshal.Release(_unkInnerAggregated);
					_unkInnerAggregated = IntPtr.Zero;
				}

				if (_unkOuter != IntPtr.Zero)
				{
					Marshal.Release(_unkOuter);
					_unkOuter = IntPtr.Zero;
				}
			}

			#region Inner
			// Inner as aggregated object
			class Inner : ICustomQueryInterface, IDisposable
			{
				object _outer;
				Type[] _interfaces;

				public Inner(object outer)
				{
					_outer = outer;
					_interfaces = _outer.GetType().BaseType.GetInterfaces();
				}

				public CustomQueryInterfaceResult GetInterface(ref Guid iid, out IntPtr ppv)
				{
					if (_outer != null)
					{
						var guid = iid;
						var iface = _interfaces.FirstOrDefault((t) => t.GUID == guid);
						if (iface != null)
						{
							var unk = Marshal.GetComInterfaceForObject(_outer, iface, CustomQueryInterfaceMode.Ignore);

							if (unk != IntPtr.Zero)
							{
								ppv = unk;
								return CustomQueryInterfaceResult.Handled;
							}
						}
					}
					ppv = IntPtr.Zero;
					return CustomQueryInterfaceResult.Failed;
				}

				~Inner()
				{
					// need to work out the reference counting for GC to work correctly

					Debug.Print("Inner object finalized.");
				}

				public void Dispose()
				{
					_outer = null;
					_interfaces = null;
				}
			}
			#endregion

			#region ICustomQueryInterface
			public CustomQueryInterfaceResult GetInterface(ref Guid iid, out IntPtr ppv)
			{
				// CustomQueryInterfaceMode.Ignore is to avoid infinite loop during IO

				if (iid == typeof(NativeMethods.IDocHostUIHandler).GUID)
				{
					ppv = Marshal.GetComInterfaceForObject(this, typeof(NativeMethods.IDocHostUIHandler), CustomQueryInterfaceMode.Ignore);
					return CustomQueryInterfaceResult.Handled;
				}

				// else
				ppv = IntPtr.Zero;
				return CustomQueryInterfaceResult.NotHandled;
			}
			#endregion

			#region IDocHostUIHandler
			int NativeMethods.IDocHostUIHandler.ShowContextMenu(int dwID, ref NativeMethods.POINT pt, IntPtr pcmdtReserved, IntPtr pdispReserved)
			{
                if (_baseIDocHostUIHandler != null)
				    return _baseIDocHostUIHandler.ShowContextMenu(dwID, ref pt, pcmdtReserved, pdispReserved);

                return NativeMethods.HRESULT.S_FALSE;
            }

            int NativeMethods.IDocHostUIHandler.GetHostInfo(ref NativeMethods.DOCHOSTUIINFO info)
			{
				Debug.Print("IDocHostUIHandler.GetHostInfo");

                if (_baseIDocHostUIHandler != null)
				    return _baseIDocHostUIHandler.GetHostInfo(ref info);

                return NativeMethods.HRESULT.S_FALSE;
            }

            int NativeMethods.IDocHostUIHandler.ShowUI(int dwID, IntPtr activeObject, IntPtr commandTarget, IntPtr frame, IntPtr doc)
			{
                if (_baseIDocHostUIHandler != null)
                    return _baseIDocHostUIHandler.ShowUI(dwID, activeObject, commandTarget, frame, doc);

                return NativeMethods.HRESULT.S_FALSE;
            }

            int NativeMethods.IDocHostUIHandler.HideUI()
			{
                if (_baseIDocHostUIHandler != null)
                    return _baseIDocHostUIHandler.HideUI();

                return NativeMethods.HRESULT.S_FALSE;
            }

            int NativeMethods.IDocHostUIHandler.UpdateUI()
			{
                if (_baseIDocHostUIHandler != null)
                    return _baseIDocHostUIHandler.UpdateUI();

                return NativeMethods.HRESULT.S_FALSE;
            }

            int NativeMethods.IDocHostUIHandler.EnableModeless(bool fEnable)
			{
                if (_baseIDocHostUIHandler != null)
                    return _baseIDocHostUIHandler.EnableModeless(fEnable);

                return NativeMethods.HRESULT.S_FALSE;
            }

            int NativeMethods.IDocHostUIHandler.OnDocWindowActivate(bool fActivate)
			{
                if (_baseIDocHostUIHandler != null)
                    return _baseIDocHostUIHandler.OnDocWindowActivate(fActivate);

                return NativeMethods.HRESULT.S_FALSE;
            }

            int NativeMethods.IDocHostUIHandler.OnFrameWindowActivate(bool fActivate)
			{
                if (_baseIDocHostUIHandler != null)
                    return _baseIDocHostUIHandler.OnFrameWindowActivate(fActivate);

                return NativeMethods.HRESULT.S_FALSE;
            }

            int NativeMethods.IDocHostUIHandler.ResizeBorder(ref NativeMethods.COMRECT rect, IntPtr doc, bool fFrameWindow)
			{
                if (_baseIDocHostUIHandler != null)
                    return _baseIDocHostUIHandler.ResizeBorder(ref rect, doc, fFrameWindow);

                return NativeMethods.HRESULT.S_FALSE;
            }

            int NativeMethods.IDocHostUIHandler.TranslateAccelerator(ref NativeMethods.MSG msg, ref Guid group, int nCmdID)
			{
                if (_baseIDocHostUIHandler != null)
                    return _baseIDocHostUIHandler.TranslateAccelerator(ref msg, ref group, nCmdID);

                return NativeMethods.HRESULT.S_FALSE;
            }

            int NativeMethods.IDocHostUIHandler.GetOptionKeyPath(string[] pbstrKey, int dw)
			{
                if (_baseIDocHostUIHandler != null)
                    return _baseIDocHostUIHandler.GetOptionKeyPath(pbstrKey, dw);

                return NativeMethods.HRESULT.S_FALSE;
            }

            int NativeMethods.IDocHostUIHandler.GetDropTarget(IntPtr pDropTarget, out IntPtr ppDropTarget)
			{
				Debug.Print("IDocHostUIHandler.GetDropTarget");

                if (_host != null)
                {
                    var altTarget = _host.GetAlternativeDropTarget(pDropTarget);

                    if (altTarget != IntPtr.Zero)
                    {
                        ppDropTarget = altTarget;
                        return NativeMethods.HRESULT.S_OK;
                    }
                }

                ppDropTarget = IntPtr.Zero;
                return NativeMethods.HRESULT.S_FALSE;
            }

            int NativeMethods.IDocHostUIHandler.GetExternal(out object ppDispatch)
			{
                if (_baseIDocHostUIHandler != null)
                    return _baseIDocHostUIHandler.GetExternal(out ppDispatch);

                ppDispatch = IntPtr.Zero;
                return NativeMethods.HRESULT.S_FALSE;
            }

            int NativeMethods.IDocHostUIHandler.TranslateUrl(int dwTranslate, string strURLIn, out string pstrURLOut)
			{
                if (_baseIDocHostUIHandler != null)
                    return _baseIDocHostUIHandler.TranslateUrl(dwTranslate, strURLIn, out pstrURLOut);

                pstrURLOut = string.Empty;
                return NativeMethods.HRESULT.S_FALSE;
            }

            int NativeMethods.IDocHostUIHandler.FilterDataObject(IntPtr pDO, out IntPtr ppDORet)
			{
                if (_baseIDocHostUIHandler != null)
                    return _baseIDocHostUIHandler.FilterDataObject(pDO, out ppDORet);

                ppDORet = IntPtr.Zero;
                return NativeMethods.HRESULT.S_FALSE;
            }
            #endregion
        }
		#endregion
	}

	public static class NativeMethods
	{
		#region IDocHostUIHandler

		public enum DOCHOSTUIDBLCLICK
		{
			DEFAULT = 0x0,
			SHOWPROPERTIES = 0x1,
			SHOWCODE = 0x2
		}

		public enum DOCHOSTUIFLAG
		{
			DIALOG = 0x1,
			DISABLE_HELP_MENU = 0x2,
			NO3DBORDER = 0x4,
			SCROLL_NO = 0x8,
			DISABLE_SCRIPT_INACTIVE = 0x10,
			OPENNEWWIN = 0x20,
			DISABLE_OFFSCREEN = 0x40,
			FLAT_SCROLLBAR = 0x80,
			DIV_BLOCKDEFAULT = 0x100,
			ACTIVATE_CLIENTHIT_ONLY = 0x200,
			NO3DOUTERBORDER = 0x00200000,
			THEME = 0x00040000,
			NOTHEME = 0x80000,
			DISABLE_COOKIE = 0x400
		}

		public static class HRESULT
		{
			public const int S_OK = 0;
			public const int S_FALSE = 1;
		}

		[StructLayout(LayoutKind.Sequential)]
		public struct DOCHOSTUIINFO
		{
			[MarshalAs(UnmanagedType.U4)]
			public int cbSize;
			[MarshalAs(UnmanagedType.I4)]
			public int dwFlags;
			[MarshalAs(UnmanagedType.I4)]
			public int dwDoubleClick;
			[MarshalAs(UnmanagedType.I4)]
			public int dwReserved1;
			[MarshalAs(UnmanagedType.I4)]
			public int dwReserved2;
		}

		[StructLayout(LayoutKind.Sequential)]
		public struct COMRECT
		{
			public int left;
			public int top;
			public int right;
			public int bottom;
		}

		[StructLayout(LayoutKind.Sequential)]
		public struct POINT
		{
			public int x;
			public int y;
		}

		[StructLayout(LayoutKind.Sequential)]
		public struct MSG
		{
			public IntPtr hwnd;
			public int message;
			public IntPtr wParam;
			public IntPtr lParam;
			public int time;
			POINT pt;
		}

		[ComImport(), Guid("BD3F23C0-D43E-11CF-893B-00AA00BDCE1A"),
		InterfaceTypeAttribute(ComInterfaceType.InterfaceIsIUnknown)]
		public interface IDocHostUIHandler
		{
			[return: MarshalAs(UnmanagedType.I4)]
			[PreserveSig]
			int ShowContextMenu(
				[In, MarshalAs(UnmanagedType.U4)]
				int dwID,
				[In]
				ref POINT pt,
				[In]
				IntPtr pcmdtReserved,
				[In]
				IntPtr pdispReserved);

			[return: MarshalAs(UnmanagedType.I4)]
			[PreserveSig]
			int GetHostInfo(
				[In, Out]
				ref DOCHOSTUIINFO info);

			[return: MarshalAs(UnmanagedType.I4)]
			[PreserveSig]
			int ShowUI(
				[In, MarshalAs(UnmanagedType.I4)]
				int dwID,
				[In]
				IntPtr activeObject,
				[In]
				IntPtr commandTarget,
				[In]
				IntPtr frame,
				[In]
				IntPtr doc);

			[return: MarshalAs(UnmanagedType.I4)]
			[PreserveSig]
			int HideUI();

			[return: MarshalAs(UnmanagedType.I4)]
			[PreserveSig]
			int UpdateUI();

			[return: MarshalAs(UnmanagedType.I4)]
			[PreserveSig]
			int EnableModeless(
				[In, MarshalAs(UnmanagedType.Bool)]
				bool fEnable);

			[return: MarshalAs(UnmanagedType.I4)]
			[PreserveSig]
			int OnDocWindowActivate(
				[In, MarshalAs(UnmanagedType.Bool)]
				bool fActivate);

			[return: MarshalAs(UnmanagedType.I4)]
			[PreserveSig]
			int OnFrameWindowActivate(
				[In, MarshalAs(UnmanagedType.Bool)]
				bool fActivate);

			[return: MarshalAs(UnmanagedType.I4)]
			[PreserveSig]
			int ResizeBorder(
				[In]
				ref COMRECT rect,
				[In]
				IntPtr doc,
				bool fFrameWindow);

			[return: MarshalAs(UnmanagedType.I4)]
			[PreserveSig]
			int TranslateAccelerator(
				[In]
				ref MSG msg,
				[In]
				ref Guid group,
				[In, MarshalAs(UnmanagedType.I4)]
				int nCmdID);

			[return: MarshalAs(UnmanagedType.I4)]
			[PreserveSig]
			int GetOptionKeyPath(
				[Out, MarshalAs(UnmanagedType.LPArray)]
				String[] pbstrKey,
				[In, MarshalAs(UnmanagedType.U4)]
				int dw);

			[return: MarshalAs(UnmanagedType.I4)]
			[PreserveSig]
			int GetDropTarget(
				[In]
				IntPtr pDropTarget,
				[Out]
				out IntPtr ppDropTarget);

			[return: MarshalAs(UnmanagedType.I4)]
			[PreserveSig]
			int GetExternal(
				[Out, MarshalAs(UnmanagedType.IDispatch)]
				out object ppDispatch);

			[return: MarshalAs(UnmanagedType.I4)]
			[PreserveSig]
			int TranslateUrl(
				[In, MarshalAs(UnmanagedType.U4)]
				int dwTranslate,
				[In, MarshalAs(UnmanagedType.LPWStr)]
				string strURLIn,
				[Out, MarshalAs(UnmanagedType.LPWStr)]
				out string pstrURLOut);

			[return: MarshalAs(UnmanagedType.I4)]
			[PreserveSig]
			int FilterDataObject(
				[In]
				IntPtr pDO,
				[Out]
				out IntPtr ppDORet);
		}

		#endregion
	}
}
