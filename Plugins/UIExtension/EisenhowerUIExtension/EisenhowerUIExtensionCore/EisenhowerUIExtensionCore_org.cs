
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

	[System.ComponentModel.DesignerCategory("")]
	public class EisenhowerUIExtensionCore_Org : Panel, IUIExtension
    {
        private const string FontName = "Tahoma";

        // ----------------------------------------------------------------------------

        private IntPtr m_HwndParent = IntPtr.Zero;
        private String m_TypeId, m_UiName;

        private Translator m_Trans;
        private UIExtension.TaskIcon m_TaskIcons;
        private System.Drawing.Font m_ControlsFont;

        private TdlEisenhowerControl m_Eisenhower;

		// ----------------------------------------------------------------------------

		public EisenhowerUIExtensionCore_Org(String typeId, String uiName, IntPtr hwndParent, Translator trans)
        {
            m_TypeId = typeId;
			m_UiName = uiName;
			m_HwndParent = hwndParent;
            m_Trans = trans;

            InitializeComponent();
        }

        // IUIExtension ------------------------------------------------------------------

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
// 			BackColor = theme.GetAppDrawingColor(UITheme.AppColor.AppBackLight);
// 
// 			m_OptionsLabel.ForeColor = theme.GetAppDrawingColor(UITheme.AppColor.AppText);
// 			m_AlignmentLabel.ForeColor = theme.GetAppDrawingColor(UITheme.AppColor.AppText);
// 
// 			// Connection colour
// 			var color = theme.GetAppDrawingColor(UITheme.AppColor.AppLinesDark);
// 
//             // Make sure it's dark enough
//             m_Eisenhower.ConnectionColor = DrawingColor.SetLuminance(color, 0.6f);
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
			return m_Eisenhower.Focus();
		}

		public new Boolean Focused
		{
			get { return m_Eisenhower.Focused; }
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
        		
        // Message handlers ---------------------------------------------------------------------

		protected override void OnGotFocus(EventArgs e)
		{
			base.OnGotFocus(e);

			m_Eisenhower.Focus();
		}

        // PRIVATE ------------------------------------------------------------------------------

        private void InitializeComponent()
        {
            m_TaskIcons = new UIExtension.TaskIcon(m_HwndParent);
            m_ControlsFont = new Font(FontName, 8, FontStyle.Regular);

			m_Eisenhower = new TdlEisenhowerControl(m_Trans, m_TaskIcons);
/*
			m_Eisenhower.Anchor = AnchorStyles.Left | AnchorStyles.Top | AnchorStyles.Right | AnchorStyles.Bottom;
            m_Eisenhower.Font = m_ControlsFont;

            if (VisualStyleRenderer.IsSupported)
                m_Eisenhower.BorderStyle = BorderStyle.FixedSingle;
            else
                m_Eisenhower.BorderStyle = BorderStyle.Fixed3D;

			m_Eisenhower.SelectionChange += new SelectionChangeEventHandler(OnEisenhowerSelectionChange);
			m_Eisenhower.DragDropChange += new DragDropChangeEventHandler(OnEisenhowerDragDrop);
			m_Eisenhower.EditTaskLabel += new EditTaskLabelEventHandler(OnEisenhowerEditTaskLabel);
            m_Eisenhower.EditTaskIcon += new EditTaskIconEventHandler(OnEisenhowerEditTaskIcon);
            m_Eisenhower.EditTaskDone += new EditTaskCompletionEventHandler(OnEisenhowerEditTaskCompletion);

            this.Controls.Add(m_Eisenhower);

			// Alignment combo and label
 			m_AlignmentLabel = CreateLabel("Alignment", null);
			this.Controls.Add(m_AlignmentLabel);

			m_AlignmentCombo = new EisenhowerAlignmentComboBox(m_Trans);
			m_AlignmentCombo.DropDownClosed += new EventHandler(OnAlignmentComboClosed);

			InitialiseCombo(m_AlignmentCombo, m_AlignmentLabel, 100);
			this.Controls.Add(m_AlignmentCombo);

			// Options combo and label
 			m_OptionsLabel = CreateLabel("Options", m_AlignmentCombo);
			this.Controls.Add(m_OptionsLabel);

			m_OptionsCombo = new EisenhowerOptionsComboBox(m_Trans);
			m_OptionsCombo.DropDownClosed += new EventHandler(OnOptionsComboClosed);
			m_OptionsCombo.DrawMode = DrawMode.OwnerDrawFixed;
			
			InitialiseCombo(m_OptionsCombo as ComboBox, m_OptionsLabel, 150);
			this.Controls.Add(m_OptionsCombo);
*/
		}

		Label CreateLabel(string text, Control prevControl)
		{
			var label = new Label();

			label.Font = m_ControlsFont;
			label.Text = text;
			label.AutoSize = true;
			label.ForeColor = SystemColors.WindowText;

			if (prevControl != null)
				label.Location = new Point((prevControl.Bounds.Right + 20), 8);
			else
				label.Location = new Point(-2, 8);

			m_Trans.Translate(label);

			return label;
		}

		void InitialiseCombo(ComboBox combo, Label prevLabel, int width)
		{
			combo.Font = m_ControlsFont;
			combo.Width = DPIScaling.Scale(width);
			combo.Height = DPIScaling.Scale(200);
			combo.Location = new Point(prevLabel.Right + 5, 4);
			combo.DropDownStyle = ComboBoxStyle.DropDownList;
			combo.Sorted = true;
		}

		void OnAlignmentComboClosed(object sender, EventArgs e)
		{
			//m_Eisenhower.Alignment = m_AlignmentCombo.SelectedAlignment;
		}

		void OnOptionsComboClosed(object sender, EventArgs e)
		{
// 			if (!m_OptionsCombo.Cancelled)
// 				m_Eisenhower.Options = m_OptionsCombo.SelectedOptions;
		}

		Boolean OnEisenhowerEditTaskLabel(object sender, UInt32 taskId)
		{
			var notify = new UIExtension.ParentNotify(m_HwndParent);

			return notify.NotifyEditLabel();
		}

        Boolean OnEisenhowerEditTaskCompletion(object sender, UInt32 taskId, bool completed)
        {
            var notify = new UIExtension.ParentNotify(m_HwndParent);

            return notify.NotifyMod(Task.Attribute.DoneDate, 
                                    (completed ? DateTime.Now : DateTime.MinValue));
        }

        Boolean OnEisenhowerEditTaskIcon(object sender, UInt32 taskId)
        {
            var notify = new UIExtension.ParentNotify(m_HwndParent);

            return notify.NotifyEditIcon();
        }

		void OnEisenhowerSelectionChange(object sender, object itemData)
		{
			var taskItem = (itemData as EisenhowerTaskItem);
			var notify = new UIExtension.ParentNotify(m_HwndParent);

			notify.NotifySelChange(taskItem.ID);
		}

		Boolean OnEisenhowerDragDrop(object sender, EisenhowerDragEventArgs e)
		{
			var notify = new UIExtension.ParentNotify(m_HwndParent);

			if (e.copyItem)
				return notify.NotifyCopy(e.dragged.uniqueID, 
										 e.targetParent.uniqueID, 
										 e.afterSibling.uniqueID);

			// else
			return notify.NotifyMove(e.dragged.uniqueID,
									 e.targetParent.uniqueID,
									 e.afterSibling.uniqueID);
		}

        protected override void OnSizeChanged(EventArgs e)
        {
            base.OnSizeChanged(e);

			Rectangle rect = ClientRectangle;
// 			rect.Y = m_AlignmentCombo.Bottom + 6;
// 			rect.Height -= (m_AlignmentCombo.Bottom + 6);

            m_Eisenhower.Bounds = rect;

            Invalidate(true);
        }


    }

}