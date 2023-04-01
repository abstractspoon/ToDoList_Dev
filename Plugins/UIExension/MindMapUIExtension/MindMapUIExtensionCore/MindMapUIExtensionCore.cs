
using System;
using System.Drawing;
using System.Drawing.Imaging;
using System.Drawing.Drawing2D;
using System.Windows.Forms;
using System.Collections.Generic;
using System.Windows.Forms.VisualStyles;

using Abstractspoon.Tdl.PluginHelpers;
using Abstractspoon.Tdl.PluginHelpers.ColorUtil;

namespace MindMapUIExtension
{

	[System.ComponentModel.DesignerCategory("")]
	public class MindMapUIExtensionCore : Panel, IUIExtension
    {
        private const string FontName = "Tahoma";

        // ----------------------------------------------------------------------------

        private IntPtr m_HwndParent = IntPtr.Zero;
        private String m_TypeId, m_UiName;

        private Translator m_Trans;
        private UIExtension.TaskIcon m_TaskIcons;
        private System.Drawing.Font m_ControlsFont;

        private TdlMindMapControl m_MindMap;

		private Label m_AlignmentLabel;
		private Label m_OptionsLabel;
		private MindMapAlignmentComboBox m_AlignmentCombo;
		private MindMapOptionsComboBox m_OptionsCombo;

		// ----------------------------------------------------------------------------

		public MindMapUIExtensionCore(String typeId, String uiName, IntPtr hwndParent, Translator trans)
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
			return m_MindMap.SetSelectedNode(dwTaskID);
        }

        public bool SelectTasks(UInt32[] pdwTaskIDs)
        {
            return false;
        }

        public bool SelectTask(String text, UIExtension.SelectTask selectTask, bool caseSensitive, bool wholeWord, bool findReplace)
        {
            return m_MindMap.SelectTask(text, selectTask, caseSensitive, wholeWord, findReplace);
        }

        public void UpdateTasks(TaskList tasks, UIExtension.UpdateType type)
        {
			m_MindMap.UpdateTasks(tasks, type);
        }

        public bool WantTaskUpdate(Task.Attribute attrib)
        {
            return m_MindMap.WantTaskUpdate(attrib);
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
			Rectangle labelRect = m_MindMap.GetSelectedItemLabelRect();

			if (labelRect.IsEmpty)
				return false;

			labelRect = m_MindMap.RectangleToScreen(labelRect);

			left = labelRect.Left;
			top = labelRect.Top;
			right = labelRect.Right;
			bottom = labelRect.Bottom;

            return true;
        }

        public UIExtension.HitResult HitTest(Int32 xPos, Int32 yPos)
        {
			var ptScreen = new Point(xPos, yPos);

			if (m_MindMap.HitTestTask(ptScreen) != 0)
				return UIExtension.HitResult.Task;

			// else
			if (m_MindMap.RectangleToScreen(m_MindMap.ClientRectangle).Contains(ptScreen))
				return UIExtension.HitResult.Tasklist;
			
			return UIExtension.HitResult.Nowhere;
        }

        public UInt32 HitTestTask(Int32 xPos, Int32 yPos)
        {
			return m_MindMap.HitTestTask(new Point(xPos, yPos));
        }

        public void SetUITheme(UITheme theme)
        {
			BackColor = theme.GetAppDrawingColor(UITheme.AppColor.AppBackLight);

			m_OptionsLabel.ForeColor = theme.GetAppDrawingColor(UITheme.AppColor.AppText);
			m_AlignmentLabel.ForeColor = theme.GetAppDrawingColor(UITheme.AppColor.AppText);

			// Connection colour
			var color = theme.GetAppDrawingColor(UITheme.AppColor.AppLinesDark);

            // Make sure it's dark enough
            m_MindMap.ConnectionColor = DrawingColor.SetLuminance(color, 0.6f);
		}

		public void SetTaskFont(String faceName, int pointSize)
		{
			m_MindMap.SetFont(faceName, pointSize);
		}

		public void SetReadOnly(bool bReadOnly)
        {
            m_MindMap.ReadOnly = bReadOnly;
        }

        public void SavePreferences(Preferences prefs, String key)
        {
			prefs.WriteProfileInt(key, "RootAlignment", (int)m_MindMap.Alignment);
			prefs.WriteProfileInt(key, "Options", (int)m_MindMap.Options);
		}

		public void LoadPreferences(Preferences prefs, String key, bool appOnly)
        {
            if (!appOnly)
            {
				// private settings
				m_MindMap.Alignment = (MindMapControl.RootAlignment)prefs.GetProfileInt(key, "RootAlignment", (int)m_MindMap.Alignment);
				m_AlignmentCombo.SelectedAlignment = m_MindMap.Alignment;

				m_MindMap.Options = (MindMapOption)prefs.GetProfileInt(key, "Options", (int)m_MindMap.Options);
				m_OptionsCombo.SelectedOptions = m_MindMap.Options;
			}

			bool taskColorIsBkgnd = prefs.GetProfileBool("Preferences", "ColorTaskBackground", false);
			m_MindMap.TaskColorIsBackground = taskColorIsBkgnd;

			bool showParentsAsFolders = prefs.GetProfileBool("Preferences", "ShowParentsAsFolders", false);
			m_MindMap.ShowParentsAsFolders = showParentsAsFolders;

            bool showDoneCheckboxes = prefs.GetProfileBool("Preferences", "AllowCheckboxAgainstTreeItem", false);
            m_MindMap.ShowCompletionCheckboxes = showDoneCheckboxes;
			
			m_MindMap.SetStrikeThruDone(prefs.GetProfileBool("Preferences", "StrikethroughDone", true));
        }

		public new Boolean Focus()
		{
			if (Focused)
				return false;

			// else
			return m_MindMap.Focus();
		}

		public new Boolean Focused
		{
			get { return m_MindMap.Focused; }
		}

		public Boolean Expand(MindMapControl.ExpandNode expand)
		{
			return m_MindMap.Expand(expand);
		}

		public Boolean CanExpand(MindMapControl.ExpandNode expand)
		{
			return m_MindMap.CanExpand(expand);
		}

		public Boolean CanMoveTask(UInt32 taskId, UInt32 destParentId, UInt32 destPrevSiblingId)
		{
			return m_MindMap.CanMoveTask(taskId, destParentId, destPrevSiblingId);
		}

		public Boolean MoveTask(UInt32 taskId, UInt32 destParentId, UInt32 destPrevSiblingId)
		{
			return m_MindMap.MoveTask(taskId, destParentId, destPrevSiblingId);
		}

		public bool GetTask(UIExtension.GetTask getTask, ref UInt32 taskID)
		{
			return m_MindMap.GetTask(getTask, ref taskID);
		}

        public Bitmap SaveToImage()
        {
            return m_MindMap.SaveToImage();
        }

        public Boolean CanSaveToImage()
        {
            return m_MindMap.CanSaveToImage();
        }
        		
        // Message handlers ---------------------------------------------------------------------

		protected override void OnGotFocus(EventArgs e)
		{
			base.OnGotFocus(e);

			m_MindMap.Focus();
		}

        // PRIVATE ------------------------------------------------------------------------------

        private void InitializeComponent()
        {
            m_TaskIcons = new UIExtension.TaskIcon(m_HwndParent);
            m_ControlsFont = new Font(FontName, 8, FontStyle.Regular);

			m_MindMap = new TdlMindMapControl(m_Trans, m_TaskIcons);
			m_MindMap.Anchor = AnchorStyles.Left | AnchorStyles.Top | AnchorStyles.Right | AnchorStyles.Bottom;
            m_MindMap.Font = m_ControlsFont;

            if (VisualStyleRenderer.IsSupported)
                m_MindMap.BorderStyle = BorderStyle.FixedSingle;
            else
                m_MindMap.BorderStyle = BorderStyle.Fixed3D;

			m_MindMap.SelectionChange += new SelectionChangeEventHandler(OnMindMapSelectionChange);
			m_MindMap.DragDropChange += new DragDropChangeEventHandler(OnMindMapDragDrop);
			m_MindMap.EditTaskLabel += new EditTaskLabelEventHandler(OnMindMapEditTaskLabel);
            m_MindMap.EditTaskIcon += new EditTaskIconEventHandler(OnMindMapEditTaskIcon);
            m_MindMap.EditTaskDone += new EditTaskCompletionEventHandler(OnMindMapEditTaskCompletion);

            this.Controls.Add(m_MindMap);

			// Alignment combo and label
 			m_AlignmentLabel = CreateLabel("Alignment", null);
			this.Controls.Add(m_AlignmentLabel);

			m_AlignmentCombo = new MindMapAlignmentComboBox(m_Trans);
			m_AlignmentCombo.DropDownClosed += new EventHandler(OnAlignmentComboClosed);

			InitialiseCombo(m_AlignmentCombo, m_AlignmentLabel, 100);
			this.Controls.Add(m_AlignmentCombo);

			// Options combo and label
 			m_OptionsLabel = CreateLabel("Options", m_AlignmentCombo);
			this.Controls.Add(m_OptionsLabel);

			m_OptionsCombo = new MindMapOptionsComboBox(m_Trans);
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

		void OnAlignmentComboClosed(object sender, EventArgs e)
		{
			m_MindMap.Alignment = m_AlignmentCombo.SelectedAlignment;
		}

		void OnOptionsComboClosed(object sender, EventArgs e)
		{
			if (!m_OptionsCombo.Cancelled)
				m_MindMap.Options = m_OptionsCombo.SelectedOptions;
		}

		Boolean OnMindMapEditTaskLabel(object sender, UInt32 taskId)
		{
			var notify = new UIExtension.ParentNotify(m_HwndParent);

			return notify.NotifyEditLabel();
		}

        Boolean OnMindMapEditTaskCompletion(object sender, UInt32 taskId, bool completed)
        {
            var notify = new UIExtension.ParentNotify(m_HwndParent);

            return notify.NotifyMod(Task.Attribute.DoneDate, 
                                    (completed ? DateTime.Now : DateTime.MinValue));
        }

        Boolean OnMindMapEditTaskIcon(object sender, UInt32 taskId)
        {
            var notify = new UIExtension.ParentNotify(m_HwndParent);

            return notify.NotifyEditIcon();
        }

		void OnMindMapSelectionChange(object sender, object itemData)
		{
			var taskItem = (itemData as MindMapTaskItem);
			var notify = new UIExtension.ParentNotify(m_HwndParent);

			notify.NotifySelChange(taskItem.ID);
		}

		Boolean OnMindMapDragDrop(object sender, MindMapDragEventArgs e)
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
			rect.Y = m_AlignmentCombo.Bottom + 6;
			rect.Height -= (m_AlignmentCombo.Bottom + 6);

            m_MindMap.Bounds = rect;

            Invalidate(true);
        }


    }

}