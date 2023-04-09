
using System;
using System.Reflection;
using System.Drawing;
using System.Drawing.Imaging;
using System.Drawing.Drawing2D;
using System.Collections.Generic;
using System.Windows.Forms;
using System.Windows.Forms.VisualStyles;

using Abstractspoon.Tdl.PluginHelpers;
using Abstractspoon.Tdl.PluginHelpers.ColorUtil;

namespace PinBoardUIExtension
{
	[System.ComponentModel.DesignerCategory("")]
	public class PinBoardUIExtensionCore : Panel, IUIExtension
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
		private PinBoardOptionsComboBox m_OptionsCombo;
		private IIControls.ToolStripEx m_Toolbar;
		private ImageList m_TBImageList;
		private UIThemeToolbarRenderer m_TBRenderer;

		// ----------------------------------------------------------------------------

		public PinBoardUIExtensionCore(String typeId, String uiName, IntPtr hwndParent, Translator trans)
        {
            m_TypeId = typeId;
			m_UiName = uiName;
			m_HwndParent = hwndParent;
            m_Trans = trans;

            InitializeComponent();
        }

        // IUIExtension ------------------------------------------------------------------

        public bool SelectTask(uint dwTaskID)
        {
			bool success = m_Control.SelectNode(dwTaskID);

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

			m_Toolbar.BackColor = theme.GetAppDrawingColor(UITheme.AppColor.AppBackLight);
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
				m_Control.Options = (PinBoardOption)prefs.GetProfileInt(key, "Options", (int)m_Control.Options);
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

			m_Control.SelectionChange += new SelectionChangeEventHandler(OnPinBoardSelectionChange);
			m_Control.TaskModified += new TaskModifiedEventHandler(OnPinBoardTaskModified);
			m_Control.EditTaskLabel += new EditTaskLabelEventHandler(OnPinBoardEditTaskLabel);
            m_Control.EditTaskIcon += new EditTaskIconEventHandler(OnPinBoardEditTaskIcon);
            m_Control.EditTaskDone += new EditTaskCompletionEventHandler(OnPinBoardEditTaskCompletion);

			m_Control.ZoomChange += (s, e) =>
			{
				UpdateToolbarButtonStates();
			};

			this.Controls.Add(m_Control);

			// Options combo and label
			m_OptionsLabel = CreateLabel("Options", null);
			this.Controls.Add(m_OptionsLabel);

			m_OptionsCombo = new PinBoardOptionsComboBox(m_Trans);
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

		bool OnPinBoardEditTaskLabel(object sender, UInt32 taskId)
		{
			var notify = new UIExtension.ParentNotify(m_HwndParent);

			return notify.NotifyEditLabel();
		}

        bool OnPinBoardEditTaskCompletion(object sender, UInt32 taskId, bool completed)
        {
            var notify = new UIExtension.ParentNotify(m_HwndParent);

            return notify.NotifyMod(Task.Attribute.DoneDate, 
                                    (completed ? DateTime.Now : DateTime.MinValue));
        }

        bool OnPinBoardEditTaskIcon(object sender, UInt32 taskId)
        {
            var notify = new UIExtension.ParentNotify(m_HwndParent);

            return notify.NotifyEditIcon();
        }

		void OnPinBoardSelectionChange(object sender, IList<uint> nodeIds)
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

		bool OnPinBoardTaskModified(object sender, IList<uint> nodeIds)
		{
			var notify = new UIExtension.ParentNotify(m_HwndParent);

			foreach (var nodeId in nodeIds)
			{
				var taskNode = m_Control.GetTaskNode(nodeId);
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
			var images = new Bitmap(assembly.GetManifestResourceStream("PinBoardUIExtension.toolbar_std.bmp"));

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
			btn2.Click += new EventHandler(OnNewConnection);
			btn2.ToolTipText = m_Trans.Translate("New Connection");
			m_Toolbar.Items.Add(btn2);

			var btn3 = new ToolStripButton();
			btn3.Name = "EditConnection";
			btn3.ImageIndex = 2;
			btn3.Click += new EventHandler(OnEditConnection);
			btn3.ToolTipText = m_Trans.Translate("Edit Connection");
			m_Toolbar.Items.Add(btn3);

			var btn4 = new ToolStripButton();
			btn4.Name = "DeleteConnection";
			btn4.ImageIndex = 3;
			btn4.Click += new EventHandler(OnDeleteConnection);
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

			(m_Toolbar.Items["NewConnection"] as ToolStripButton).Enabled = (m_Control.SelectedNodeCount == 2);
			(m_Toolbar.Items["NewConnection"] as ToolStripButton).Checked = false;

			(m_Toolbar.Items["EditConnection"] as ToolStripButton).Enabled = false;
			(m_Toolbar.Items["EditConnection"] as ToolStripButton).Checked = false;

			(m_Toolbar.Items["DeleteConnection"] as ToolStripButton).Enabled = false;
			(m_Toolbar.Items["DeleteConnection"] as ToolStripButton).Checked = false;
		}

		void OnZoomToExtents(object sender, EventArgs e)
		{
			m_Control.ZoomToExtents();
		}

		void OnNewConnection(object sender, EventArgs e)
		{
			if (m_Control.SelectedNodeCount == 2)
			{
				m_Control.CreateNewConnection(m_Control.SelectedNodeIds[0], m_Control.SelectedNodeIds[1], Color.Empty, 0, TaskLink.EndArrows.None);
			}
		}

		void OnEditConnection(object sender, EventArgs e)
		{
			// TODO
		}

		void OnDeleteConnection(object sender, EventArgs e)
		{
			// TODO
		}

		void OnPreferences(object sender, EventArgs e)
		{
			// TODO
		}

		void OnHelp(object sender, EventArgs e)
		{
			// TODO
		}

	}

}