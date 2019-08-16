using System;
using System.Diagnostics;
using System.Runtime.InteropServices;
using System.Windows.Forms;

// CREDITS: https://stackoverflow.com/questions/28529000/get-itextdocument-for-a-richtextbox-in-c-sharp
// CREDITS: www.codeproject.com/Articles/4528/Use-IRichEditOle-from-C

namespace RichEditExtensions
{
	[ComImport, InterfaceType(ComInterfaceType.InterfaceIsIUnknown), Guid("00020D00-0000-0000-c000-000000000046")]
	public interface IRichEditOle
	{
		int GetClientSite(IntPtr lplpolesite);
		int GetObjectCount();
		int GetLinkCount();
		int GetObject(int iob, REOBJECT lpreobject, [MarshalAs(UnmanagedType.U4)] GetObjectOptions flags);
		int InsertObject(REOBJECT lpreobject);
		int ConvertObject(int iob, CLSID rclsidNew, string lpstrUserTypeNew);
		int ActivateAs(CLSID rclsid, CLSID rclsidAs);
		int SetHostNames(string lpstrContainerApp, string lpstrContainerObj);
		int SetLinkAvailable(int iob, int fAvailable);
		int SetDvaspect(int iob, uint dvaspect);
		int HandsOffStorage(int iob);
		int SaveCompleted(int iob, IntPtr lpstg);
		int InPlaceDeactivate();

		int ContextSensitiveHelp(int fEnterMode);
		//int GetClipboardData(CHARRANGE FAR * lpchrg, uint reco, IntPtr lplpdataobj);
		//int ImportDataObject(IntPtr lpdataobj, CLIPFORMAT cf, HGLOBAL hMetaPict);
	}

	public enum GetObjectOptions
	{
		REO_GETOBJ_NO_INTERFACES = 0x00000000,
		REO_GETOBJ_POLEOBJ = 0x00000001,
		REO_GETOBJ_PSTG = 0x00000002,
		REO_GETOBJ_POLESITE = 0x00000004,
		REO_GETOBJ_ALL_INTERFACES = 0x00000007
	}

	[StructLayout(LayoutKind.Sequential)]
	public struct CLSID
	{
		public int a;
		public short b;
		public short c;
		public byte d;
		public byte e;
		public byte f;
		public byte g;
		public byte h;
		public byte i;
		public byte j;
		public byte k;
	}

	[StructLayout(LayoutKind.Sequential)]
	public struct SIZEL
	{
		public int x;
		public int y;
	}

	[StructLayout(LayoutKind.Sequential)]
	public class REOBJECT
	{
		public int cbStruct = Marshal.SizeOf(typeof(REOBJECT)); // Size of structure
		public CLSID clsid = new CLSID(); // Class ID of object
		public int cp = 0; // Character position of object
		public uint dvaspect = 0; // Display aspect to use
		public uint dwFlags = 0; // Object status flags
		public uint dwUser = 0; // Dword for user's use
		public IntPtr poleobj = IntPtr.Zero; // OLE object interface
		public IntPtr polesite = IntPtr.Zero; // Associated client site interface
		public IntPtr pstg = IntPtr.Zero; // Associated storage interface
		public SIZEL sizel = new SIZEL(); // Size of object (may be 0,0)
	}

	public class API
	{
		[DllImport("User32.dll", CharSet = CharSet.Auto)]
		public static extern int SendMessage(IntPtr hWnd, int message, IntPtr wParam, IntPtr lParam);
	}

	public class Messages
	{
		public const int WM_USER = 0x0400;
		public const int EM_GETOLEINTERFACE = WM_USER + 60;
	}


	[ComImport, InterfaceType(ComInterfaceType.InterfaceIsIUnknown), Guid("8CC497C0-A1DF-11ce-8098-00AA0047BE5D")]
	public interface ITextDocument
	{
		// IDispath methods (We never use them)
		int GetIDsOfNames(Guid riid, IntPtr rgszNames, uint cNames, uint lcid, ref int rgDispId);
		int GetTypeInfo(uint iTInfo, uint lcid, IntPtr ppTInfo);
		int GetTypeInfoCount(ref uint pctinfo);

		int Invoke(uint dispIdMember, Guid riid, uint lcid, uint wFlags, IntPtr pDispParams, IntPtr pvarResult,
			IntPtr pExcepInfo, ref uint puArgErr);

		// ITextDocument methods
		int GetName( /* [retval][out] BSTR* */ [In, Out, MarshalAs(UnmanagedType.BStr)]
			ref string pName);

		int GetSelection( /* [retval][out] ITextSelection** */ IntPtr ppSel);
		int GetStoryCount( /* [retval][out] */ ref int pCount);
		int GetStoryRanges( /* [retval][out] ITextStoryRanges** */ IntPtr ppStories);
		int GetSaved( /* [retval][out] */ ref int pValue);
		int SetSaved( /* [in] */ int Value);
		int GetDefaultTabStop( /* [retval][out] */ ref float pValue);
		int SetDefaultTabStop( /* [in] */ float Value);
		int New();
		int Open( /* [in] VARIANT **/ IntPtr pVar, /* [in] */ int Flags, /* [in] */ int CodePage);
		int Save( /* [in] VARIANT * */ IntPtr pVar, /* [in] */ int Flags, /* [in] */ int CodePage);
		int Freeze( /* [retval][out] */ ref int pCount);
		int Unfreeze( /* [retval][out] */ ref int pCount);
		int BeginEditCollection();
		int EndEditCollection();
		int Undo( /* [in] */ int Count, /* [retval][out] */ ref IntPtr prop);
		int Redo( /* [in] */ int Count, /* [retval][out] */ ref IntPtr prop);
		int Range( /* [in] */ int cp1, /* [in] */ int cp2, /* [retval][out] ITextRange** */ IntPtr ppRange);
		int RangeFromPoint( /* [in] */ int x, /* [in] */ int y, /* [retval][out] ITextRange** */ IntPtr ppRange);
	}

	public class TomConstants
	{
		public const int tomSuspend = -9999995;
		public const int tomResume = -9999994;
	}

	// =============================================================================================
	//
	// RichOLE
	//
	// =============================================================================================

	public class RichOLE
	{
		// ---------------------------------------------------------------------------------------------
		// Attributes
		// ---------------------------------------------------------------------------------------------

		private RichTextBox mRichBox;
		protected IntPtr IRichEditOlePtr = IntPtr.Zero;

		protected IRichEditOle IRichEditOleValue;
		protected IntPtr ITextDocumentPtr = IntPtr.Zero;

		protected ITextDocument ITextDocumentValue;

		// ---------------------------------------------------------------------------------------------
		// Constructor
		// ---------------------------------------------------------------------------------------------

		public RichOLE(RichTextBox richBox)
		{
			mRichBox = richBox;
			mRichBox.Disposed += (sender, args) =>
			{
				ReleaseRichEditOleInterface();
				mRichBox = null;
			};
		}

		// ---------------------------------------------------------------------------------------------
		// OLE
		// ---------------------------------------------------------------------------------------------

		public IRichEditOle GetRichEditOleInterface()
		{
			if (IRichEditOleValue == null)
			{
				// Allocate the ptr that EM_GETOLEINTERFACE will fill in
				var ptr = Marshal.AllocCoTaskMem(Marshal.SizeOf(typeof(IntPtr))); // Alloc the ptr.
				Marshal.WriteIntPtr(ptr, IntPtr.Zero); // Clear it.
				try
				{
					if (0 != API.SendMessage(mRichBox.Handle, Messages.EM_GETOLEINTERFACE, IntPtr.Zero, ptr))
					{
						// Read the returned pointer
						var pRichEdit = Marshal.ReadIntPtr(ptr);
						try
						{
							if (pRichEdit != IntPtr.Zero)
							{
								// Query for the IRichEditOle interface
								var guid = new Guid("00020D00-0000-0000-c000-000000000046");
								Marshal.QueryInterface(pRichEdit, ref guid, out IRichEditOlePtr);

								// Wrap it in the C# interface for IRichEditOle
								IRichEditOleValue =
									(IRichEditOle)Marshal.GetTypedObjectForIUnknown(IRichEditOlePtr,
										typeof(IRichEditOle));

								if (IRichEditOleValue == null)
								{
									throw new Exception(
										"Failed to get the object wrapper for the IRichEditOle interface.");
								}

								// IID_ITextDocument
								guid = new Guid("8CC497C0-A1DF-11CE-8098-00AA0047BE5D");
								Marshal.QueryInterface(pRichEdit, ref guid, out ITextDocumentPtr);

								// Wrap it in the C# interface for IRichEditOle
								ITextDocumentValue =
									(ITextDocument)Marshal.GetTypedObjectForIUnknown(ITextDocumentPtr,
										typeof(ITextDocument));

								if (ITextDocumentValue == null)
								{
									throw new Exception(
										"Failed to get the object wrapper for the ITextDocument interface.");
								}
							}
							else
							{
								throw new Exception("Failed to get the pointer.");
							}
						}
						finally
						{
							Marshal.Release(pRichEdit);
						}
					}
					else
					{
						throw new Exception("EM_GETOLEINTERFACE failed.");
					}
				}
				catch (Exception err)
				{
					Trace.WriteLine(err.ToString());
					ReleaseRichEditOleInterface();
				}
				finally
				{
					// Free the ptr memory
					Marshal.FreeCoTaskMem(ptr);
				}
			}

			return IRichEditOleValue;
		}

		public void ReleaseRichEditOleInterface()
		{
			if (IRichEditOlePtr != IntPtr.Zero)
			{
				Marshal.Release(IRichEditOlePtr);
			}

			IRichEditOlePtr = IntPtr.Zero;
			IRichEditOleValue = null;
		}

		public void EnableUndo(bool enable)
		{
			if (IRichEditOleValue == null)
			{
				GetRichEditOleInterface();
			}

			var ptr = IntPtr.Zero;

			ITextDocumentValue.Undo(enable ? TomConstants.tomResume : TomConstants.tomSuspend, ptr);
		}
	}
}