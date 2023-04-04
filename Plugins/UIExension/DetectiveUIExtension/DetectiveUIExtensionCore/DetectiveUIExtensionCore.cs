
using System;
using System.Drawing;
using System.Drawing.Imaging;
using System.Drawing.Drawing2D;
using System.Windows.Forms;
using System.Collections.Generic;
using System.Windows.Forms.VisualStyles;

using Abstractspoon.Tdl.PluginHelpers;
using Abstractspoon.Tdl.PluginHelpers.ColorUtil;

namespace DetectiveUIExtension
{
	[System.ComponentModel.DesignerCategory("")]
	public class DetectiveUIExtensionCore : Panel, IUIExtension
    {
        private const string FontName = "Tahoma";

        // ----------------------------------------------------------------------------

        private IntPtr m_HwndParent = IntPtr.Zero;
        private String m_TypeId, m_UiName;

        private Translator m_Trans;
        private UIExtension.TaskIcon m_TaskIcons;
        private System.Drawing.Font m_ControlsFont;

        private TDLNodeControl m_Control;

		private Label m_OptionsLabel;
		private DetectiveOptionsComboBox m_OptionsCombo;

		// ----------------------------------------------------------------------------

		public DetectiveUIExtensionCore(String typeId, String uiName, IntPtr hwndParent, Translator trans)
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
			return m_Control.SelectNode(dwTaskID);
        }

        public bool SelectTasks(UInt32[] pdwTaskIDs)
        {
            return false;
        }

        public bool SelectTask(String text, UIExtension.SelectTask selectTask, bool caseSensitive, bool wholeWord, bool findReplace)
        {
            return m_Control.SelectTask(text, selectTask, caseSensitive, wholeWord, findReplace);
        }

        public void UpdateTasks(TaskList tasks, UIExtension.UpdateType type)
        {
			m_Control.UpdateTasks(tasks, type);
        }

        public bool WantTaskUpdate(Task.Attribute attrib)
        {
            return m_Control.WantTaskUpdate(attrib);
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

        public bool GetLabelEditRect(ref Int32 left, ref Int32 top, ref Int32 right, ref Int32 bottom)
        {
			Rectangle labelRect = m_Control.GetSelectedNodeLabelRect();

			if (labelRect.IsEmpty)
				return false;

			labelRect = m_Control.RectangleToScreen(labelRect);

			left = labelRect.Left;
			top = labelRect.Top;
			right = labelRect.Right;
			bottom = labelRect.Bottom;

            return true;
        }

        public UIExtension.HitResult HitTest(Int32 xPos, Int32 yPos)
        {
			UInt32 taskId = m_Control.HitTest(new Point(xPos, yPos));

			if (taskId != 0)
				return UIExtension.HitResult.Task;

			// else
            return UIExtension.HitResult.Tasklist;
        }

        public UInt32 HitTestTask(Int32 xPos, Int32 yPos)
        {
			return m_Control.HitTest(new Point(xPos, yPos));
        }

        public void SetUITheme(UITheme theme)
        {
			BackColor = theme.GetAppDrawingColor(UITheme.AppColor.AppBackLight);

			// Connection colour
			var color = theme.GetAppDrawingColor(UITheme.AppColor.AppLinesDark);

            // Make sure it's dark enough
           // m_Control.ConnectionColor = DrawingColor.SetLuminance(color, 0.6f);
		}

		public void SetTaskFont(String faceName, int pointSize)
		{
			m_Control.SetFont(faceName, pointSize);
		}

		public void SetReadOnly(bool bReadOnly)
        {
            m_Control.ReadOnly = bReadOnly;
        }

        public void SavePreferences(Preferences prefs, String key)
        {
			prefs.WriteProfileInt(key, "Options", (int)m_Control.Options);
		}

		public void LoadPreferences(Preferences prefs, String key, bool appOnly)
        {
            if (!appOnly)
            {
				// private settings
				m_Control.Options = (DetectiveOption)prefs.GetProfileInt(key, "Options", (int)m_Control.Options);
				m_OptionsCombo.SelectedOptions = m_Control.Options;
			}

			m_Control.TaskColorIsBackground = prefs.GetProfileBool("Preferences", "ColorTaskBackground", false);
			m_Control.ShowParentsAsFolders = prefs.GetProfileBool("Preferences", "ShowParentsAsFolders", false);
            m_Control.ShowCompletionCheckboxes = prefs.GetProfileBool("Preferences", "AllowCheckboxAgainstTreeNode", false);
			m_Control.SetStrikeThruDone(prefs.GetProfileBool("Preferences", "StrikethroughDone", true));
        }

		public new bool Focus()
		{
			if (Focused)
				return false;

			// else
			return m_Control.Focus();
		}

		public new bool Focused
		{
			get { return m_Control.Focused; }
		}

		public bool CanMoveTask(UInt32 taskId, UInt32 destParentId, UInt32 destPrevSiblingId)
		{
			return m_Control.CanMoveTask(taskId, destParentId, destPrevSiblingId);
		}

		public bool MoveTask(UInt32 taskId, UInt32 destParentId, UInt32 destPrevSiblingId)
		{
			return m_Control.MoveTask(taskId, destParentId, destPrevSiblingId);
		}

		public bool GetTask(UIExtension.GetTask getTask, ref UInt32 taskID)
		{
			return m_Control.GetTask(getTask, ref taskID);
		}

        public Bitmap SaveToImage()
        {
			return null;//m_Control.SaveToImage();
        }

        public bool CanSaveToImage()
        {
            return false;//m_Control.CanSaveToImage();
        }
        		
        // Message handlers ---------------------------------------------------------------------

		protected override void OnGotFocus(EventArgs e)
		{
			base.OnGotFocus(e);

			m_Control.Focus();
		}

        // PRIVATE ------------------------------------------------------------------------------

        private void InitializeComponent()
        {
            m_TaskIcons = new UIExtension.TaskIcon(m_HwndParent);
            m_ControlsFont = new Font(FontName, 8, FontStyle.Regular);

			m_Control = new TDLNodeControl(m_Trans, m_TaskIcons, m_TypeId);
			m_Control.Anchor = AnchorStyles.Left | AnchorStyles.Top | AnchorStyles.Right | AnchorStyles.Bottom;
            m_Control.SetFont(FontName, 8);

			if (VisualStyleRenderer.IsSupported)
                m_Control.BorderStyle = BorderStyle.FixedSingle;
            else
                m_Control.BorderStyle = BorderStyle.Fixed3D;

			m_Control.SelectionChange += new SelectionChangeEventHandler(OnDetectiveSelectionChange);
			m_Control.TaskDragDrop += new TaskDragDropEventHandler(OnDetectiveDragDrop);
			m_Control.EditTaskLabel += new EditTaskLabelEventHandler(OnDetectiveEditTaskLabel);
            m_Control.EditTaskIcon += new EditTaskIconEventHandler(OnDetectiveEditTaskIcon);
            m_Control.EditTaskDone += new EditTaskCompletionEventHandler(OnDetectiveEditTaskCompletion);

			this.Controls.Add(m_Control);

			// Options combo and label
			m_OptionsLabel = CreateLabel("Options", null);
			this.Controls.Add(m_OptionsLabel);

			m_OptionsCombo = new DetectiveOptionsComboBox(m_Trans);
			m_OptionsCombo.DropDownClosed += new EventHandler(OnOptionsComboClosed);
			m_OptionsCombo.DrawMode = DrawMode.OwnerDrawFixed;

			InitialiseCombo(m_OptionsCombo as ComboBox, m_OptionsLabel, 150);
			this.Controls.Add(m_OptionsCombo);
		}

		Label CreateLabel(string untranslatedText, Control prevControl)
		{
			var label = new Label();

			label.Font = m_ControlsFont;
			label.Text = m_Trans.Translate(untranslatedText);
			label.AutoSize = true;

			if (prevControl != null)
				label.Location = new Point((prevControl.Bounds.Right + 20), 8);
			else
				label.Location = new Point(-2, 8);

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

		void OnOptionsComboClosed(object sender, EventArgs e)
		{
			if (!m_OptionsCombo.Cancelled)
				m_Control.Options = m_OptionsCombo.SelectedOptions;
		}

		bool OnDetectiveEditTaskLabel(object sender, UInt32 taskId)
		{
			var notify = new UIExtension.ParentNotify(m_HwndParent);

			return notify.NotifyEditLabel();
		}

        bool OnDetectiveEditTaskCompletion(object sender, UInt32 taskId, bool completed)
        {
            var notify = new UIExtension.ParentNotify(m_HwndParent);

            return notify.NotifyMod(Task.Attribute.DoneDate, 
                                    (completed ? DateTime.Now : DateTime.MinValue));
        }

        bool OnDetectiveEditTaskIcon(object sender, UInt32 taskId)
        {
            var notify = new UIExtension.ParentNotify(m_HwndParent);

            return notify.NotifyEditIcon();
        }

		void OnDetectiveSelectionChange(object sender, uint nodeId)
		{
			var notify = new UIExtension.ParentNotify(m_HwndParent);

			notify.NotifySelChange(nodeId);
		}

		bool OnDetectiveDragDrop(object sender, uint nodeId)
		{
			var notify = new UIExtension.ParentNotify(m_HwndParent);

			return notify.NotifyMod(Task.Attribute. MetaData, m_Control.SelectedTaskNode.EncodeMetaData());


// 			if (e.copyNode)
// 				return notify.NotifyCopy(e.dragged.uniqueID, 
// 										 e.targetParent.uniqueID, 
// 										 e.afterSibling.uniqueID);
// 			
// 			// else
// 			return notify.NotifyMove(e.dragged.uniqueID,
// 									 e.targetParent.uniqueID,
// 									 e.afterSibling.uniqueID);
//			return true;
		}

        protected override void OnSizeChanged(EventArgs e)
        {
			base.OnSizeChanged(e);

			Rectangle rect = ClientRectangle;
			rect.Y = m_OptionsCombo.Bottom + 6;
			rect.Height -= (m_OptionsCombo.Bottom + 6);

			m_Control.Bounds = rect;

			Invalidate(true);
		}


	}

}