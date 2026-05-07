using System;
using System.Drawing;
using System.Drawing.Imaging;
using System.Drawing.Drawing2D;
using System.Windows.Forms;
using System.Collections.Generic;
using System.Windows.Forms.VisualStyles;

using Abstractspoon.Tdl.PluginHelpers;
using Abstractspoon.Tdl.PluginHelpers.ColorUtil;


namespace EisenhowerUIExtension
{
	public partial class EisenhowerUIExtensionCore : UserControl, IUIExtension
	{
		public EisenhowerUIExtensionCore()
		{
			InitializeComponent();
		}

		public bool SelectTask(UInt32 dwTaskID)
		{
			return false;
			//return m_Eisenhower.SetSelectedNode(dwTaskID);
		}

		public bool SelectTasks(UInt32[] pdwTaskIDs)
		{
			return false;
		}

		public bool SelectTask(String text, UIExtension.SelectTask selectTask, bool caseSensitive, bool wholeWord, bool findReplace)
		{
			return false;
			//return m_Eisenhower.SelectTask(text, selectTask, caseSensitive, wholeWord, findReplace);
		}

		public bool ScrollToSelectedTask()
		{
			// 			if (CanScrollToSelectedTask())
			// 			{
			// 				m_Eisenhower.EnsureSelectionVisible();
			// 				return true;
			// 			}

			return false;
		}

		public bool CanScrollToSelectedTask()
		{
			return false;
			//return m_Eisenhower.HasSelection;
		}

		public void UpdateTasks(TaskList tasks, UIExtension.UpdateType type)
		{
			//m_Eisenhower.UpdateTasks(tasks, type);
		}

		public bool WantTaskUpdate(Task.Attribute attrib)
		{
			return true;
			//return m_Eisenhower.WantTaskUpdate(attrib);
		}

		public bool WantSortUpdate(Task.Attribute attrib)
		{
			return false;
		}

		public bool PrepareNewTask(ref Task task)
		{
			return true;
		}

		public bool ProcessMessage(IntPtr hwnd, UInt32 message, UInt32 wParam, UInt32 lParam, UInt32 time, Int32 xPos, Int32 yPos)
		{
			return false;
		}

		public bool DoIdleProcessing()
		{
			return false;
			//return m_Eisenhower.DoIdleProcessing();
		}

		public bool GetLabelEditRect(ref Int32 left, ref Int32 top, ref Int32 right, ref Int32 bottom)
		{
			return false;
			// 			Rectangle labelRect = m_Eisenhower.GetSelectedItemLabelRect();
			// 
			// 			if (labelRect.IsEmpty)
			// 				return false;
			// 
			// 			labelRect = m_Eisenhower.RectangleToScreen(labelRect);
			// 
			// 			left = labelRect.Left;
			// 			top = labelRect.Top;
			// 			right = labelRect.Right;
			// 			bottom = labelRect.Bottom;
			// 
			//             return true;
		}

		public UIExtension.HitTestResult HitTest(Int32 xPos, Int32 yPos, UIExtension.HitTestReason reason)
		{
			// 			var ptScreen = new Point(xPos, yPos);
			// 
			// 			if (m_Eisenhower.HitTestTask(ptScreen) != 0)
			// 				return UIExtension.HitTestResult.Task;
			// 
			// 			// else
			// 			if (m_Eisenhower.RectangleToScreen(m_Eisenhower.ClientRectangle).Contains(ptScreen))
			// 				return UIExtension.HitTestResult.Tasklist;

			return UIExtension.HitTestResult.Nowhere;
		}

		public UInt32 HitTestTask(Int32 xPos, Int32 yPos, UIExtension.HitTestReason reason)
		{
			return 0;//m_Eisenhower.HitTestTask(new Point(xPos, yPos));
		}

		public void SetUITheme(UITheme theme)
		{
			BackColor = theme.GetAppDrawingColor(UITheme.AppColor.AppBackLight);

			m_EisenhowerCtrl.SetUITheme(theme);
		}

		public void SetTaskFont(String faceName, int pointSize)
		{
			// 			m_Eisenhower.SetFont(faceName, pointSize);
		}

		public void SetReadOnly(bool bReadOnly)
		{
			//             m_Eisenhower.ReadOnly = bReadOnly;
		}

		public void SavePreferences(Preferences prefs, String key)
		{
			// 			m_Eisenhower.SavePreferences(prefs, key);
		}

		public void LoadPreferences(Preferences prefs, String key, bool appOnly)
		{
			//             if (!appOnly)
			//             {
			// 				// private settings
			// 				m_Eisenhower.LoadPreferences(prefs, key);
			// 
			// 				m_AlignmentCombo.SelectedAlignment = m_Eisenhower.Alignment;
			// 				m_OptionsCombo.SelectedOptions = m_Eisenhower.Options;
			// 			}
			// 
			// 			m_Eisenhower.TaskColorIsBackground = prefs.GetProfileBool("Preferences", "ColorTaskBackground", false);
			// 			m_Eisenhower.ShowParentsAsFolders = prefs.GetProfileBool("Preferences", "ShowParentsAsFolders", false);
			//             m_Eisenhower.ShowCompletionCheckboxes = prefs.GetProfileBool("Preferences", "AllowCheckboxAgainstTreeItem", false);
			//             m_Eisenhower.ShowMixedCompletionState = prefs.GetProfileBool("Preferences", "ShowMixedCompletionState", true);
			// 			
			// 			m_Eisenhower.SetStrikeThruDone(prefs.GetProfileBool("Preferences", "StrikethroughDone", true));
		}

		public new Boolean Focus()
		{
			if (Focused)
				return false;

			// else
			return false;//m_Eisenhower.Focus();
		}

		public new Boolean Focused
		{
			// get { m_Eisenhower.Focused; }
			get { return false; }
		}

		// 		public Boolean Expand(EisenhowerControl.ExpandNode expand)
		// 		{
		// 			return false;
		// 			//return m_Eisenhower.Expand(expand);
		// 		}
		// 
		// 		public Boolean CanExpand(EisenhowerControl.ExpandNode expand)
		// 		{
		// 			return false;
		// 			//return m_Eisenhower.CanExpand(expand);
		// 		}

		public Boolean CanMoveTask(UInt32 taskId, UInt32 destParentId, UInt32 destPrevSiblingId)
		{
			return false;
			//return m_Eisenhower.CanMoveTask(taskId, destParentId, destPrevSiblingId);
		}

		public Boolean MoveTask(UInt32 taskId, UInt32 destParentId, UInt32 destPrevSiblingId)
		{
			return false;
			//return m_Eisenhower.MoveTask(taskId, destParentId, destPrevSiblingId);
		}

		public bool GetTask(UIExtension.GetTask getTask, ref UInt32 taskID)
		{
			return false;
			//return m_Eisenhower.GetTask(getTask, ref taskID);
		}

		public Bitmap SaveToImage()
		{
			return null;
			//return m_Eisenhower.SaveToImage();
		}

		public Boolean CanSaveToImage()
		{
			return false;
			//return m_Eisenhower.CanSaveToImage();
		}

		// Message handlers ---------------------------------------------------
		protected override void OnSizeChanged(EventArgs e)
		{
			base.OnSizeChanged(e);

			Rectangle rCtrl = m_EisenhowerCtrl.Bounds;

			rCtrl.X = 0;
			rCtrl.Width = Width;
			rCtrl.Height = (Height - rCtrl.Top);

			m_EisenhowerCtrl.Bounds = rCtrl;
		}
	}


}
