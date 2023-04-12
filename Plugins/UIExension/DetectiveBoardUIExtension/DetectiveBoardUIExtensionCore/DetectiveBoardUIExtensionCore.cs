﻿
using System;
using System.Diagnostics;
using System.Reflection;
using System.Drawing;
using System.Collections.Generic;
using System.Windows.Forms;
using System.Windows.Forms.VisualStyles;

using Abstractspoon.Tdl.PluginHelpers;
using Abstractspoon.Tdl.PluginHelpers.ColorUtil;

namespace DetectiveBoardUIExtension
{
	[System.ComponentModel.DesignerCategory("")]
	public class DetectiveBoardUIExtensionCore : Panel, IUIExtension
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
		private DetectiveBoardOptionsComboBox m_OptionsCombo;
		private IIControls.ToolStripEx m_Toolbar;
		private ImageList m_TBImageList;
		private UIThemeToolbarRenderer m_TBRenderer;

		// ----------------------------------------------------------------------------

		public DetectiveBoardUIExtensionCore(String typeId, String uiName, IntPtr hwndParent, Translator trans)
        {
			TaskItem.MetaDataKey = typeId;

			m_UiName = uiName;
			m_HwndParent = hwndParent;
            m_Trans = trans;

            InitializeComponent();
        }

        // IUIExtension ------------------------------------------------------------------

        public bool SelectTask(uint dwTaskID)
        {
			bool success = m_Control.SelectTask(dwTaskID);

			UpdateToolbarButtonStates();

			return success;
        }

        public bool SelectAll()
        {
			m_Control.SelectAllNodes();

			UpdateToolbarButtonStates();

			return true;
        }

        public bool SelectTasks(uint[] pdwTaskIDs)
        {
			bool success = m_Control.SelectNodes(new List<uint>(pdwTaskIDs));

			UpdateToolbarButtonStates();

			return success;
		}

		public bool SelectTask(String text, UIExtension.SelectTask selectTask, bool caseSensitive, bool wholeWord, bool findReplace)
        {
            bool success = m_Control.SelectTask(text, selectTask, caseSensitive, wholeWord, findReplace);

			UpdateToolbarButtonStates();

			return success;
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
			const int WM_KEYDOWN = 0x0100;

			switch (message)
			{
			case WM_KEYDOWN:
				{
					Keys keyPress = (Keys)wParam;

					switch (keyPress)
					{
					case Keys.Delete:
						return m_Control.DeleteSelectedUserLink();

					case Keys.F2:
						if (m_Control.HasSelectedUserLink)
						{
							OnEditUserLink(this, null);
							return true;
						}
						break;
					}
				}
				break;
			}

			return false;
		}

		public bool GetLabelEditRect(ref Int32 left, ref Int32 top, ref Int32 right, ref Int32 bottom)
        {
			Rectangle labelRect = m_Control.GetSelectedTaskLabelRect();

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

			m_Toolbar.BackColor = BackColor;
			m_TBRenderer.SetUITheme(theme);
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
				m_Control.Options = (DetectiveBoardOption)prefs.GetProfileInt(key, "Options", (int)m_Control.Options);
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

			m_Control = new TDLNodeControl(m_Trans, m_TaskIcons);
			m_Control.Anchor = AnchorStyles.Left | AnchorStyles.Top | AnchorStyles.Right | AnchorStyles.Bottom;
            m_Control.SetFont(FontName, 8);

			if (VisualStyleRenderer.IsSupported)
                m_Control.BorderStyle = BorderStyle.FixedSingle;
            else
                m_Control.BorderStyle = BorderStyle.Fixed3D;

			m_Control.NodeSelectionChange += new NodeSelectionChangeEventHandler(OnDetectiveBoardSelectionChange);
			m_Control.TaskModified += new TaskModifiedEventHandler(OnDetectiveBoardTaskModified);
			m_Control.EditTaskLabel += new EditTaskLabelEventHandler(OnDetectiveBoardEditTaskLabel);
            m_Control.EditTaskIcon += new EditTaskIconEventHandler(OnDetectiveBoardEditTaskIcon);
            m_Control.EditTaskDone += new EditTaskCompletionEventHandler(OnDetectiveBoardEditTaskCompletion);

			m_Control.ZoomChange += (s, e) => { UpdateToolbarButtonStates(); };
			m_Control.UserLinkSelectionChange += (s, e) => { UpdateToolbarButtonStates(); };
			m_Control.DoubleClickUserLink += new EventHandler(OnEditUserLink);

			this.Controls.Add(m_Control);

			// Options combo and label
			m_OptionsLabel = CreateLabel("Options", null);
			this.Controls.Add(m_OptionsLabel);

			m_OptionsCombo = new DetectiveBoardOptionsComboBox(m_Trans);
			m_OptionsCombo.DropDownClosed += new EventHandler(OnOptionsComboClosed);
			m_OptionsCombo.DrawMode = DrawMode.OwnerDrawFixed;

			InitialiseCombo(m_OptionsCombo as ComboBox, m_OptionsLabel, 150);
			this.Controls.Add(m_OptionsCombo);

			CreateToolbar();
			UpdateToolbarButtonStates();
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

		bool OnDetectiveBoardEditTaskLabel(object sender, UInt32 taskId)
		{
			var notify = new UIExtension.ParentNotify(m_HwndParent);

			return notify.NotifyEditLabel();
		}

        bool OnDetectiveBoardEditTaskCompletion(object sender, UInt32 taskId, bool completed)
        {
            var notify = new UIExtension.ParentNotify(m_HwndParent);

            return notify.NotifyMod(Task.Attribute.DoneDate, 
                                    (completed ? DateTime.Now : DateTime.MinValue));
        }

        bool OnDetectiveBoardEditTaskIcon(object sender, UInt32 taskId)
        {
            var notify = new UIExtension.ParentNotify(m_HwndParent);

            return notify.NotifyEditIcon();
        }

		void OnDetectiveBoardSelectionChange(object sender, IList<uint> nodeIds)
		{
			var notify = new UIExtension.ParentNotify(m_HwndParent);

			switch (nodeIds.Count)
			{
			case 0:
				notify.NotifySelChange(0);
				break;

			case 1:
				notify.NotifySelChange(nodeIds[0]);
				break;

			default:
				{
					uint[] pdwTaskIds = new uint[nodeIds.Count];
					nodeIds.CopyTo(pdwTaskIds, 0);

					notify.NotifySelChange(pdwTaskIds);
				}
				break;
			}

			UpdateToolbarButtonStates();
		}

		bool OnDetectiveBoardTaskModified(object sender, IList<uint> nodeIds)
		{
			var notify = new UIExtension.ParentNotify(m_HwndParent);

			foreach (var nodeId in nodeIds)
			{
				var taskNode = m_Control.GetTaskItem(nodeId);
				notify.AddMod(nodeId, Task.Attribute.MetaData, taskNode.EncodeMetaData());
			}

			return notify.NotifyMod();


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

			m_Toolbar.Location = new Point(m_OptionsCombo.Right + 10, 0);

			Rectangle rect = ClientRectangle;
			rect.Y = m_OptionsCombo.Bottom + 6;
			rect.Height -= (m_OptionsCombo.Bottom + 6);
			
			m_Control.Bounds = rect;

			Invalidate(true);
			UpdateToolbarButtonStates();
		}

		private void CreateToolbar()
		{
			var assembly = Assembly.GetExecutingAssembly();
			var images = new Bitmap(assembly.GetManifestResourceStream("DetectiveBoardUIExtension.toolbar_std.bmp"));

			m_TBImageList = new ImageList();
			m_TBImageList.ColorDepth = ColorDepth.Depth32Bit;
			m_TBImageList.ImageSize = new System.Drawing.Size(16, 16);
			m_TBImageList.TransparentColor = Color.Magenta;
			m_TBImageList.Images.AddStrip(images);

			m_Toolbar = new IIControls.ToolStripEx();
			m_Toolbar.Anchor = AnchorStyles.None;
			m_Toolbar.GripStyle = ToolStripGripStyle.Hidden;
			m_Toolbar.ImageList = m_TBImageList;

			int imageSize = DPIScaling.Scale(16);

			m_Toolbar.ImageScalingSize = new Size(imageSize, imageSize);
			m_Toolbar.Height = (imageSize + 7); // MFC

			m_TBRenderer = new UIThemeToolbarRenderer();
			m_Toolbar.Renderer = m_TBRenderer;

			var btn1 = new ToolStripButton();
			btn1.Name = "ZoomToExtents";
			btn1.ImageIndex = 0;
			btn1.Click += new EventHandler(OnZoomToExtents);
			btn1.ToolTipText = m_Trans.Translate("Zoom to Extents");
			m_Toolbar.Items.Add(btn1);

			m_Toolbar.Items.Add(new ToolStripSeparator());

			var btn2 = new ToolStripButton();
			btn2.Name = "NewConnection";
			btn2.ImageIndex = 1;
			btn2.Click += new EventHandler(OnNewTaskLink);
			btn2.ToolTipText = m_Trans.Translate("New Connection");
			m_Toolbar.Items.Add(btn2);

			var btn3 = new ToolStripButton();
			btn3.Name = "EditConnection";
			btn3.ImageIndex = 2;
			btn3.Click += new EventHandler(OnEditUserLink);
			btn3.ToolTipText = m_Trans.Translate("Edit Connection");
			m_Toolbar.Items.Add(btn3);

			var btn4 = new ToolStripButton();
			btn4.Name = "DeleteConnection";
			btn4.ImageIndex = 3;
			btn4.Click += new EventHandler(OnDeleteTaskLink);
			btn4.ToolTipText = m_Trans.Translate("Delete Connection");
			m_Toolbar.Items.Add(btn4);

			m_Toolbar.Items.Add(new ToolStripSeparator());

			var btn9 = new ToolStripButton();
			btn9.ImageIndex = 4;
			btn9.Click += new EventHandler(OnPreferences);
			btn9.ToolTipText = m_Trans.Translate("Preferences");
			m_Toolbar.Items.Add(btn9);

			m_Toolbar.Items.Add(new ToolStripSeparator());

			var btn10 = new ToolStripButton();
			btn10.ImageIndex = 5;
			btn10.Click += new EventHandler(OnHelp);
			btn10.ToolTipText = m_Trans.Translate("Online Help");
			m_Toolbar.Items.Add(btn10);

			Toolbars.FixupButtonSizes(m_Toolbar);

			Controls.Add(m_Toolbar);
		}

		private void UpdateToolbarButtonStates()
		{
			(m_Toolbar.Items["ZoomToExtents"] as ToolStripButton).Enabled = m_Control.CanZoomOut;

			(m_Toolbar.Items["NewConnection"] as ToolStripButton).Checked = false;
			(m_Toolbar.Items["NewConnection"] as ToolStripButton).Enabled = CanCreateNewConnection;

			(m_Toolbar.Items["EditConnection"] as ToolStripButton).Enabled = m_Control.HasSelectedUserLink;
			(m_Toolbar.Items["EditConnection"] as ToolStripButton).Checked = false;

			(m_Toolbar.Items["DeleteConnection"] as ToolStripButton).Enabled = m_Control.HasSelectedUserLink;
			(m_Toolbar.Items["DeleteConnection"] as ToolStripButton).Checked = false;
		}

		private void OnZoomToExtents(object sender, EventArgs e)
		{
			m_Control.ZoomToExtents();
		}

		private bool CanCreateNewConnection
		{
			get
			{
				if (m_Control.ReadOnly)
					return false;

				if (m_Control.SelectedNodeCount > 2)
					return false;

				if ((m_Control.SelectedNodeCount == 2))
				{
					uint task1Id = m_Control.SelectedNodeIds[0], task2Id = m_Control.SelectedNodeIds[1];

					if (m_Control.IsTaskLocked(task1Id) && m_Control.IsTaskLocked(task2Id))
						return false;
				}

				return true;
			}
		}

		private void OnNewTaskLink(object sender, EventArgs e)
		{
			switch (m_Control.SelectedNodeCount)
			{
			case 0:
			case 1:
				MessageBox.Show(m_Trans.Translate("To create a new connection you need to preselect two tasks"), m_UiName, MessageBoxButtons.OK, MessageBoxIcon.Information);
				break;

			case 2:
				{
					uint firstId = m_Control.SelectedNodeIds[0], secondId = m_Control.SelectedNodeIds[1];

					if (!m_Control.CanCreateUserLink(firstId, secondId))
					{
						// Try the reverse
						if (m_Control.CanCreateUserLink(secondId, firstId))
						{
							uint temp = firstId;

							firstId = secondId;
							secondId = temp;
						}
						else
						{
							MessageBox.Show(m_Trans.Translate("A connection already exists between the two selected tasks"), m_UiName, MessageBoxButtons.OK, MessageBoxIcon.Hand);
							return;
						}
					}

					var dlg = new DetectiveBoardAddEditLinkDlg(m_Trans.Translate("New Connection"), null);

					if (dlg.ShowDialog() == DialogResult.OK)
					{
						m_Control.CreateUserLink(firstId, secondId, dlg.Color, dlg.Thickness, dlg.Arrows);
						UpdateToolbarButtonStates();
					}
				}
				break;
			}
		}

		private void OnEditUserLink(object sender, EventArgs e)
		{
			Debug.Assert(m_Control.HasSelectedUserLink);

			var dlg = new DetectiveBoardAddEditLinkDlg(m_Trans.Translate("Edit Connection"), m_Control.SelectedUserLink);

			if (dlg.ShowDialog() == DialogResult.OK)
			{
				m_Control.EditSelectedUserLink(dlg.Color, dlg.Thickness, dlg.Arrows);
				UpdateToolbarButtonStates();
			}
			else
			{
				m_Control.ClearUserLinkSelection();
			}
		}

		private void OnDeleteTaskLink(object sender, EventArgs e)
		{
			m_Control.DeleteSelectedUserLink();
		}

		private void OnPreferences(object sender, EventArgs e)
		{
			// TODO
		}

		private void OnHelp(object sender, EventArgs e)
		{
			// TODO
		}

	}

}