//////////////////////////////////////////////////////////////////////

using System;
using System.Runtime.InteropServices;
using Microsoft.Office.Interop.Outlook;

using Abstractspoon.Tdl.PluginHelpers;

//////////////////////////////////////////////////////////////////////

using IOleDropTarget = Microsoft.VisualStudio.OLE.Interop.IDropTarget;
using IOleDataObject = Microsoft.VisualStudio.OLE.Interop.IDataObject;
using IOlePoint		 = Microsoft.VisualStudio.OLE.Interop.POINTL;

//////////////////////////////////////////////////////////////////////

namespace HTMLContentControl
{
	public class TDLDropTarget : IOleDropTarget
	{
		public delegate void OutlookDropEventHandler(object sender, String title, String url);
		public delegate void RichTextDropEventHandler(object sender, String rtf);
		public delegate void FileDropEventHandler(object sender, String[] filePaths);

		// ---------------------------------------------------------------

		private IOleDropTarget m_DefaultDropTarget;
		private IOleDataObject m_CurrentObject;

		public event OutlookDropEventHandler OutlookDrop;
		public event RichTextDropEventHandler RichTextDrop;
		public event FileDropEventHandler FileDrop;

		// ---------------------------------------------------------------

		public TDLDropTarget(IntPtr defDropTarget)
		{
			m_DefaultDropTarget = (IOleDropTarget)Marshal.GetTypedObjectForIUnknown(defDropTarget, typeof(IOleDropTarget));
		}

		public void DragEnter(IOleDataObject pDataObj, uint grfKeyState, IOlePoint pt, ref uint pdwEffect)
		{
			if (((OutlookDrop != null)	&& OutlookUtil.IsOutlookItem(pDataObj)) ||
				((RichTextDrop != null) && ClipboardUtil.IsRtf(pDataObj)) ||
				((FileDrop != null)		&& ClipboardUtil.IsDropFile(pDataObj)))
			{
				m_CurrentObject = pDataObj;
				pdwEffect = DragDropUtil.DRAGDROP_COPY;
			}
			else if (m_DefaultDropTarget != null)
			{
				m_DefaultDropTarget.DragEnter(pDataObj, grfKeyState, pt, pdwEffect);

				if (pdwEffect != DragDropUtil.DRAGDROP_NONE)
					pdwEffect = DragDropUtil.DRAGDROP_COPY;
			}
		}

		public void DragLeave()
		{
			if (m_CurrentObject != null)
			{
				m_CurrentObject = null;
			}
			else if (m_DefaultDropTarget != null)
			{
				m_DefaultDropTarget.DragLeave();
			}
		}

		public void DragOver(uint grfKeyState, IOlePoint pt, ref uint pdwEffect)
		{
			if (m_CurrentObject != null)
			{
				pdwEffect = DragDropUtil.DRAGDROP_COPY;
			}
			else if (m_DefaultDropTarget != null)
			{
				m_DefaultDropTarget.DragOver(grfKeyState, pt, pdwEffect);

				if (pdwEffect != DragDropUtil.DRAGDROP_NONE)
					pdwEffect = DragDropUtil.DRAGDROP_COPY;
			}
		}

		public void Drop(IOleDataObject pDataObj, uint grfKeyState, IOlePoint pt, ref uint pdwEffect)
		{
			if (m_CurrentObject == pDataObj)
			{
				try
				{
					if (OutlookUtil.IsOutlookItem(pDataObj))
					{
						var outlook = new Application();
						var selection = outlook.ActiveExplorer().Selection;

						if ((selection != null) && (selection.Count >= 1))
						{
							var title = "";
							var id = "";

							var item = selection[1];

							if (item is MailItem)
							{
								title = (item as MailItem).Subject;
								id = (item as MailItem).EntryID;
							}
							else if (item is ContactItem)
							{
								title = (item as ContactItem).Subject;
								id = (item as ContactItem).EntryID;
							}
							else if (item is JournalItem)
							{
								title = (item as JournalItem).Subject;
								id = (item as JournalItem).EntryID;
							}
							else if (item is TaskItem)
							{
								title = (item as TaskItem).Subject;
								id = (item as TaskItem).EntryID;
							}
							else if (item is NoteItem)
							{
								title = (item as NoteItem).Subject;
								id = (item as NoteItem).EntryID;
							}
							else if (item is AppointmentItem)
							{
								title = (item as AppointmentItem).Subject;
								id = (item as AppointmentItem).EntryID;
							}

							if (!String.IsNullOrEmpty(title) && !String.IsNullOrEmpty(id))
							{
								var url = OutlookUtil.FormatItemAsUrl(id);
								OutlookDrop(this, title, url);

								pdwEffect = DragDropUtil.DRAGDROP_COPY;
							}
						}
					}
					else if (ClipboardUtil.IsRtf(pDataObj))
					{
						string rtf = ClipboardUtil.GetRtf(pDataObj);
						RichTextDrop(this, rtf);

						pdwEffect = DragDropUtil.DRAGDROP_COPY;
					}
					else if (ClipboardUtil.IsDropFile(pDataObj))
					{
						string[] filePaths = ClipboardUtil.GetDropFiles(pDataObj);
						FileDrop(this, filePaths);

						pdwEffect = DragDropUtil.DRAGDROP_COPY;
					}
				}
				catch (System.Exception /*e*/)
				{
					pdwEffect = DragDropUtil.DRAGDROP_NONE;
				}
			}
			else if (m_DefaultDropTarget != null)
			{
				m_DefaultDropTarget.Drop(pDataObj, grfKeyState, pt, pdwEffect);

				if (pdwEffect != DragDropUtil.DRAGDROP_NONE)
					pdwEffect = DragDropUtil.DRAGDROP_COPY;
			}

			m_CurrentObject = null;
		}
	}

}

//////////////////////////////////////////////////////////////////////

