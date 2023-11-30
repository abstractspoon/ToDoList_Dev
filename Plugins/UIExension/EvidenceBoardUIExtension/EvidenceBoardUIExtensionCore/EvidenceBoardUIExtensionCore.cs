
using System;
using System.Diagnostics;
using System.Reflection;
using System.Drawing;
using System.Collections.Generic;
using System.Windows.Forms;
using System.Windows.Forms.VisualStyles;

using Abstractspoon.Tdl.PluginHelpers;
using Abstractspoon.Tdl.PluginHelpers.ColorUtil;

namespace EvidenceBoardUIExtension
{
	[System.ComponentModel.DesignerCategory("")]
	public class EvidenceBoardUIExtensionCore : Panel, IUIExtension
    {
        private const string FontName = "Tahoma";

        // ----------------------------------------------------------------------------

        private IntPtr m_HwndParent = IntPtr.Zero;
        private String m_UiName;

        private Translator m_Trans;
        private UIExtension.TaskIcon m_TaskIcons;
        private Font m_ControlsFont;

        private TDLNodeControl m_Control;
		private EvidenceBoardPreferencesDlg m_PrefsDlg;

		private String m_LastBrowsedImageFolder;
		static string s_ImageFilter;

		private Label m_OptionsLabel;
		private EvidenceBoardOptionsComboBox m_OptionsCombo;
		private Label m_LinkVisibilityLabel;
		private EvidenceBoardLinkVisibilityComboBox m_LinkVisibilityCombo;

		private IIControls.ToolStripEx m_Toolbar;
		private ImageList m_TBImageList;
		private UIThemeToolbarRenderer m_TBRenderer;

		private Label m_DateSliderLabel;
		private MonthRangeSliderCtrl m_DateSlider;

		// ----------------------------------------------------------------------------

		public EvidenceBoardUIExtensionCore(String typeId, String uiName, IntPtr hwndParent, Translator trans)
        {
			TaskItem.MetaDataKey = typeId;

			m_UiName = uiName;
			m_HwndParent = hwndParent;
            m_Trans = trans;

			if (s_ImageFilter == null)
				s_ImageFilter = m_Trans.Translate("Image Files") + " (*.png, *.bmp, *.ico, *.jpg, *.jpeg, *.tiff, *.gif)|*.png;*.bmp;*.ico;*.jpg;*.jpeg;*.tiff;*.gif||";

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

		public bool ScrollToSelectedTask()
		{
			return m_Control.ScrollToSelection(false);
		}

		public bool CanScrollToSelectedTask()
		{
			return (m_Control.SelectedNodeCount > 0);
		}

		public void UpdateTasks(TaskList tasks, UIExtension.UpdateType type)
        {
			m_Control.UpdateTasks(tasks, type);

			bool dateChange = false;

			switch (type)
			{
			case UIExtension.UpdateType.Edit:
			case UIExtension.UpdateType.New:
				dateChange = (tasks.IsAttributeAvailable(Task.Attribute.StartDate) ||
								tasks.IsAttributeAvailable(Task.Attribute.DueDate) ||
								tasks.IsAttributeAvailable(Task.Attribute.DoneDate));
				break;

			case UIExtension.UpdateType.Delete:
			case UIExtension.UpdateType.All:
				m_LinkVisibilityCombo.UserLinkTypes = m_Control.UserLinkTypes;
				m_Control.VisibleLinkTypes = m_LinkVisibilityCombo.SelectedLinkTypes;

				dateChange = true;
				break;
			}

			if (dateChange)
			{
				DateTime min, max;

				if (m_Control.GetMinMaxDateRange(out min, out max))
				{
					m_DateSlider.SetMinMax(min, max);
					m_DateSlider.ResizeToFit(ClientRectangle.Right - m_DateSlider.Left - 10);
				}
			}
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

        public UIExtension.HitTestResult HitTest(Int32 xPos, Int32 yPos, UIExtension.HitTestReason reason)
        {
			UInt32 taskId = m_Control.HitTest(new Point(xPos, yPos));

			if (taskId != 0)
				return UIExtension.HitTestResult.Task;

			// else
            return UIExtension.HitTestResult.Tasklist;
        }

        public UInt32 HitTestTask(Int32 xPos, Int32 yPos, UIExtension.HitTestReason reason)
        {
			return m_Control.HitTest(new Point(xPos, yPos));
        }

        public void SetUITheme(UITheme theme)
        {
			BackColor = m_Toolbar.BackColor = theme.GetAppDrawingColor(UITheme.AppColor.AppBackLight);
			m_DateSlider.SetParentBackColor(BackColor);

			// Set the toolbar colors to be the same as the back color
			theme.SetAppDrawingColor(UITheme.AppColor.ToolbarDark, BackColor);
			theme.SetAppDrawingColor(UITheme.AppColor.ToolbarLight, BackColor);


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
			prefs.WriteProfileString(key, "LastBrowsedImageFolder", m_LastBrowsedImageFolder);

			m_PrefsDlg.SavePreferences(prefs, key);
			m_OptionsCombo.SavePreferences(prefs, key);
			m_LinkVisibilityCombo.SavePreferences(prefs, key);
			m_Control.SavePreferences(prefs, key);
		}

		public void LoadPreferences(Preferences prefs, String key, bool appOnly)
        {
            if (!appOnly) // private settings
            {
				m_LastBrowsedImageFolder = prefs.GetProfileString(key, "LastBrowsedImageFolder", @"C:\");

				m_Control.LoadPreferences(prefs, key);
				m_Control.Options = m_OptionsCombo.LoadPreferences(prefs, key);
				m_Control.VisibleLinkTypes = m_LinkVisibilityCombo.LoadPreferences(prefs, key);

				m_DateSlider.Visible = m_DateSliderLabel.Visible = m_Control.Options.HasFlag(EvidenceBoardOption.ShowDateSlider);

				// Preferences
				m_PrefsDlg.LoadPreferences(prefs, key);
				UpdateEvidenceBoardPreferences();
			}

			// App preferences
			m_Control.TaskColorIsBackground = prefs.GetProfileBool("Preferences", "ColorTaskBackground", false);
			m_Control.ShowParentsAsFolders = prefs.GetProfileBool("Preferences", "ShowParentsAsFolders", false);
            m_Control.ShowCompletionCheckboxes = prefs.GetProfileBool("Preferences", "AllowCheckboxAgainstTreeNode", false);
			m_Control.SetStrikeThruDone(prefs.GetProfileBool("Preferences", "StrikethroughDone", true));
			m_Control.ShowLabelTips = !prefs.GetProfileBool("Preferences", "ShowInfoTips", false);
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

		public bool ExpandSelectedTaskImages() { return m_Control.ExpandSelectedTaskImages(); }
		public bool CollapseSelectedTaskImages() { return m_Control.CollapseSelectedTaskImages(); }

		public bool CanExpandSelectedTasks { get { return m_Control.CanExpandSelectedTaskImages; } }
		public bool CanCollapseSelectedTasks	{ get { return m_Control.CanCollapseSelectedTaskImages; } }

		public bool ExpandAllTaskImages()	{ return m_Control.ExpandAllTaskImages(); }
		public bool CollapseAllTaskImages() { return m_Control.CollapseAllTaskImages(); }

		public bool CanExpandAllTaskImages { get { return m_Control.CanExpandAllTaskImages; } }
		public bool CanCollapseAllTaskImages { get { return m_Control.CanCollapseAllTaskImages; } }


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

			m_PrefsDlg = new EvidenceBoardPreferencesDlg(this, m_Trans, m_ControlsFont);

			m_Control = new TDLNodeControl(m_Trans, m_TaskIcons);
			m_Control.Anchor = AnchorStyles.Left | AnchorStyles.Top | AnchorStyles.Right | AnchorStyles.Bottom;
            m_Control.SetFont(FontName, 8);

			if (VisualStyleRenderer.IsSupported)
                m_Control.BorderStyle = BorderStyle.FixedSingle;
            else
                m_Control.BorderStyle = BorderStyle.Fixed3D;

			m_Control.NodeSelectionChange += new NodeSelectionChangeEventHandler(OnEvidenceBoardSelectionChange);
			m_Control.TaskModified += new TaskModifiedEventHandler(OnEvidenceBoardTaskModified);
			m_Control.ImageDropped += new ImageDroppedEventHandler(OnEvidenceBoardImageDropped);

			m_Control.ConnectionCreated += new ConnectionCreatedEventHandler(OnEvidenceBoardTaskLinkCreated);
			m_Control.ConnectionEdited += new ConnectionEditedEventHandler(OnEvidenceBoardTaskLinkEdited);
			m_Control.ConnectionDeleted += new ConnectionDeletedEventHandler(OnEvidenceBoardTaskLinkDeleted);

			m_Control.EditTaskLabel += new EditTaskLabelEventHandler(OnEvidenceBoardEditTaskLabel);
            m_Control.EditTaskIcon += new EditTaskIconEventHandler(OnEvidenceBoardEditTaskIcon);
            m_Control.EditTaskDone += new EditTaskCompletionEventHandler(OnEvidenceBoardEditTaskCompletion);

			m_Control.ZoomChange += (s, e) => { UpdateToolbarButtonStates(); };
			m_Control.UserLinkSelectionChange += (s, e) => { UpdateToolbarButtonStates(); };
			m_Control.BackgroundImageChanged += (s, e) => 
			{
				var notify = new UIExtension.ParentNotify(m_HwndParent);
				notify.NotifyTasklistMetaData(m_Control.EncodeBackgroundImageState());

				UpdateToolbarButtonStates();
			};

			m_Control.DoubleClickUserLink += new EventHandler(OnEditUserLink);

			this.Controls.Add(m_Control);

			// Options combo and label
			m_OptionsLabel = CreateLabel("Options", null);
			this.Controls.Add(m_OptionsLabel);

			m_OptionsCombo = new EvidenceBoardOptionsComboBox();
			m_OptionsCombo.Translate(m_Trans);

			m_OptionsCombo.DropDownClosed += new EventHandler(OnOptionsComboClosed);
			m_OptionsCombo.DrawMode = DrawMode.OwnerDrawFixed;

			InitialiseCtrl(m_OptionsCombo, m_OptionsLabel, 150);
			this.Controls.Add(m_OptionsCombo);

			// Link vis combo and label
			m_LinkVisibilityLabel = CreateLabel("Connection Visibility", m_OptionsCombo);
			this.Controls.Add(m_LinkVisibilityLabel);

			m_LinkVisibilityCombo = new EvidenceBoardLinkVisibilityComboBox();
			m_LinkVisibilityCombo.Translate(m_Trans);
			m_LinkVisibilityCombo.DropDownClosed += new EventHandler(OnLinkVisibilityComboClosed);

			InitialiseCtrl(m_LinkVisibilityCombo as ComboBox, m_LinkVisibilityLabel, 150);
			this.Controls.Add(m_LinkVisibilityCombo);

			// Toolbar 
			CreateToolbar();
			m_Toolbar.Location = ToolbarLocation;

			// Date slider combo and label
			m_DateSliderLabel = CreateLabel("Visible Date Range", m_Toolbar);
			this.Controls.Add(m_DateSliderLabel);

			m_DateSlider = new MonthRangeSliderCtrl();
			m_DateSlider.Height = MonthRangeSliderCtrl.GetRequiredHeight();

			InitialiseCtrl(m_DateSlider, m_DateSliderLabel, 250);

			this.Controls.Add(m_DateSlider);

			m_DateSlider.ChangeEvent += new EventHandler(OnDateSliderChange);
		}

		protected void OnDateSliderChange(object sender, EventArgs e)
		{
			DateTime from = DateTime.MinValue, to = DateTime.MaxValue;

			if (m_DateSlider.GetSelectedRange(ref from, ref to))
				m_Control.SetSelectedDateRange(from, to);
			else
				m_Control.ClearSelectedDateRange();

			m_DateSliderLabel.Text = string.Format("{0} ({1})", m_Trans.Translate("Visible Date Range"), m_DateSlider.FormatRange());
		}

		private int ControlTop
		{
			get
			{
				if (m_LinkVisibilityCombo == null)
					return 0;
				
				//else
				return (m_LinkVisibilityCombo.Bottom + DPIScaling.Scale(4));
			}
		}

		private Point ToolbarLocation
		{
			get
			{
				if (m_LinkVisibilityCombo == null)
					return Point.Empty;

				// Centre the toolbar vertically on the combo
				return new Point(m_LinkVisibilityCombo.Right + 10, m_LinkVisibilityCombo.Top - (m_Toolbar.Height - m_LinkVisibilityCombo.Height) / 2);
			}
		}

		Label CreateLabel(string untranslatedText, Control prevControl)
		{
			var label = new Label();

			label.Font = m_ControlsFont;
			label.Text = m_Trans.Translate(untranslatedText);
			label.AutoSize = true;
			label.ForeColor = SystemColors.WindowText;

			if (prevControl != null)
				label.Location = new Point((prevControl.Bounds.Right + DPIScaling.Scale(7)), DPIScaling.Scale(5));
			else
				label.Location = new Point(0, DPIScaling.Scale(5));

			return label;
		}

		void InitialiseCtrl(Control ctrl, Label associatedLabel, int width)
		{
			ctrl.Font = m_ControlsFont;
			ctrl.Width = DPIScaling.Scale(width);

			if (ctrl is ComboBox)
			{
				ctrl.Location = new Point(associatedLabel.Left, associatedLabel.Bottom + DPIScaling.Scale(3));
				ctrl.Height = DPIScaling.Scale(200);
				(ctrl as ComboBox).DropDownStyle = ComboBoxStyle.DropDownList;
				(ctrl as ComboBox).Sorted = true;
			}
			else
			{
				ctrl.Location = new Point(associatedLabel.Left, associatedLabel.Bottom + DPIScaling.Scale(2));
			}
		}

		void OnOptionsComboClosed(object sender, EventArgs e)
		{
			if (!m_OptionsCombo.Cancelled)
			{
				m_Control.Options = m_OptionsCombo.SelectedOptions;

				if (m_Control.Options.HasFlag(EvidenceBoardOption.ShowDateSlider))
				{
					m_DateSlider.Visible = m_DateSliderLabel.Visible = true;
				}
				else
				{
					m_DateSlider.Visible = m_DateSliderLabel.Visible = false;
					m_Control.ClearSelectedDateRange();
				}
			}
		}

		void OnLinkVisibilityComboClosed(object sender, EventArgs e)
		{
 			if (!m_LinkVisibilityCombo.Cancelled)
 				m_Control.VisibleLinkTypes = m_LinkVisibilityCombo.SelectedLinkTypes;
		}

		bool OnEvidenceBoardEditTaskLabel(object sender, UInt32 taskId)
		{
			var notify = new UIExtension.ParentNotify(m_HwndParent);

			return notify.NotifyEditLabel();
		}

        bool OnEvidenceBoardEditTaskCompletion(object sender, UInt32 taskId, bool completed)
        {
            var notify = new UIExtension.ParentNotify(m_HwndParent);

            return notify.NotifyMod(Task.Attribute.DoneDate, 
                                    (completed ? DateTime.Now : DateTime.MinValue));
        }

        bool OnEvidenceBoardEditTaskIcon(object sender, UInt32 taskId)
        {
            var notify = new UIExtension.ParentNotify(m_HwndParent);

            return notify.NotifyEditIcon();
        }

		void OnEvidenceBoardSelectionChange(object sender, IList<uint> nodeIds)
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

		bool OnEvidenceBoardTaskModified(object sender, IList<uint> nodeIds)
		{
			var notify = new UIExtension.ParentNotify(m_HwndParent);

			foreach (var nodeId in nodeIds)
			{
				var taskItem = m_Control.GetTaskItem(nodeId);
				notify.AddMod(nodeId, Task.Attribute.MetaData, taskItem.EncodeMetaData());
			}

			return notify.NotifyMod();
		}

		bool OnEvidenceBoardImageDropped(object sender, uint taskId, string[] imagePaths)
		{
			var notify = new UIExtension.ParentNotify(m_HwndParent);

			return notify.NotifyMod(Task.Attribute.FileLink, string.Join("\n", imagePaths));
		}

		bool OnEvidenceBoardTaskLinkCreated(object sender, UserLink link)
		{
			m_Control.SelectedUserLink = link;

			if (!m_Control.HasSelectedUserLink)
				return false;

			var dlg = new EvidenceBoardAddLinkDlg(m_Trans, m_Control.UserLinkTypes);

			if (dlg.ShowDialog() == DialogResult.OK)
			{
				m_Control.EditSelectedUserLink(dlg.LinkAttributes, UserLinkAttributes.Mask.All, false);

				m_LinkVisibilityCombo.UserLinkTypes = m_Control.UserLinkTypes;
				m_Control.VisibleLinkTypes = m_LinkVisibilityCombo.SelectedLinkTypes;

				NotifyParentTaskModified(link.FromId);
				UpdateToolbarButtonStates();
				Invalidate();

				return true;
			}

			// else
			return false;
		}

		bool OnEvidenceBoardTaskLinkEdited(object sender, UserLink link)
		{
			return NotifyParentTaskModified(link.FromId);
		}

		bool OnEvidenceBoardTaskLinkDeleted(object sender, uint taskId)
		{
			return NotifyParentTaskModified(taskId);
		}

		bool NotifyParentTaskModified(uint taskId)
		{
			var notify = new UIExtension.ParentNotify(m_HwndParent);
			var taskItem = m_Control.GetTaskItem(taskId);

			return notify.NotifyMod(Task.Attribute.MetaData, taskItem.EncodeMetaData());
		}

		protected override void OnSizeChanged(EventArgs e)
        {
			base.OnSizeChanged(e);

			// Somewhere in WinForms the toolbar gets repositioned even though the toolbar
			// is not anchored, so we have to restore the correct position each time
			m_Toolbar.Location = ToolbarLocation;

			// Resize the slider to take up the rest of the width
			Rectangle rect = ClientRectangle;
			m_DateSlider.ResizeToFit(rect.Right - m_DateSlider.Left - 10);

			// Node control
			rect.Y = ControlTop;
			rect.Height -= ControlTop;
			
			m_Control.Bounds = rect;

			Invalidate(true);
			UpdateToolbarButtonStates();
		}

		private void CreateToolbar()
		{
			var assembly = Assembly.GetExecutingAssembly();
			var images = new Bitmap(assembly.GetManifestResourceStream("EvidenceBoardUIExtension.toolbar_std.bmp"));

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

			var btn5 = new ToolStripButton();
			btn5.Name = "SetBackgroundImage";
			btn5.ImageIndex = 4;
			btn5.Click += new EventHandler(OnSetBackgroundImage);
			btn5.ToolTipText = m_Trans.Translate("Set Background Image");
			m_Toolbar.Items.Add(btn5);

			var btn6 = new ToolStripButton();
			btn6.Name = "ClearBackgroundImage";
			btn6.ImageIndex = 5;
			btn6.Click += new EventHandler(OnClearBackgroundImage);
			btn6.ToolTipText = m_Trans.Translate("Clear Background Image");
			m_Toolbar.Items.Add(btn6);

			m_Toolbar.Items.Add(new ToolStripSeparator());

			var btn9 = new ToolStripButton();
			btn9.ImageIndex = 6;
			btn9.Click += new EventHandler(OnPreferences);
			btn9.ToolTipText = m_Trans.Translate("Preferences");
			m_Toolbar.Items.Add(btn9);

			m_Toolbar.Items.Add(new ToolStripSeparator());

			var btn10 = new ToolStripButton();
			btn10.ImageIndex = 7;
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

			(m_Toolbar.Items["SetBackgroundImage"] as ToolStripButton).Enabled = !m_Control.ReadOnly;
			(m_Toolbar.Items["ClearBackgroundImage"] as ToolStripButton).Enabled = (!m_Control.ReadOnly && m_Control.HasBackgroundImage);
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

				if (m_Control.SelectedNodeCount == 2)
				{
					// The 'from' task is always the second in the
					// selection list because as tasks are selected
					// they get added to the head of the list
					uint fromId = m_Control.SelectedNodeIds[1];

					return !m_Control.IsTaskLocked(fromId);
				}

				return true;
			}
		}

		private void OnNewTaskLink(object sender, EventArgs e)
		{
			if (!CanCreateNewConnection)
				return;

			switch (m_Control.SelectedNodeCount)
			{
			case 0:
			case 1:
				MessageBox.Show(m_Trans.Translate("To create a new connection you need to preselect two tasks"), m_UiName, MessageBoxButtons.OK, MessageBoxIcon.Information);
				break;

			case 2:
				{
					uint fromId = m_Control.SelectedNodeIds[1];
					uint toId = m_Control.SelectedNodeIds[0];

					if (m_Control.UserLinkExists(fromId, toId))
					{
						MessageBox.Show(m_Trans.Translate("A connection already exists between the two selected tasks"), m_UiName, MessageBoxButtons.OK, MessageBoxIcon.Hand);
						return;
					}

					var dlg = new EvidenceBoardAddLinkDlg(m_Trans, m_Control.UserLinkTypes);

					if (dlg.ShowDialog() == DialogResult.OK)
					{
						var link = m_Control.CreateUserLink(fromId, toId, dlg.LinkAttributes);

						if (link != null)
						{
							m_LinkVisibilityCombo.UserLinkTypes = m_Control.UserLinkTypes;
							m_Control.VisibleLinkTypes = m_LinkVisibilityCombo.SelectedLinkTypes;

							NotifyParentTaskModified(fromId);
						}

						UpdateToolbarButtonStates();
					}
				}
				break;
			}
		}

		private void OnEditUserLink(object sender, EventArgs e)
		{
			EditSelectedUserLink();
		}

		private bool EditSelectedUserLink()
		{
			Debug.Assert(m_Control.HasSelectedUserLink);

			var dlg = new EvidenceBoardEditLinkDlg(m_Trans, m_Control.SelectedUserLink, m_Control.UserLinkTypes);

			if (dlg.ShowDialog() == DialogResult.OK)
			{
				string prevType = m_Control.SelectedUserLink.Attributes.Type;

				if (m_Control.EditSelectedUserLink(dlg.Attributes, dlg.AttributeMask, dlg.WantModifyAllOfSameType))
				{
					if (dlg.Attributes.Type != prevType)
					{
						m_LinkVisibilityCombo.UserLinkTypes = m_Control.UserLinkTypes;
						m_Control.VisibleLinkTypes = m_LinkVisibilityCombo.SelectedLinkTypes;
					}
				}

				UpdateToolbarButtonStates();
				return true;
			}

			// else
			return false;
		}

		private void OnDeleteTaskLink(object sender, EventArgs e)
		{
			m_Control.DeleteSelectedUserLink();
		}

		private void OnPreferences(object sender, EventArgs e)
		{
			m_PrefsDlg.StartPosition = FormStartPosition.CenterParent;

			if (m_PrefsDlg.ShowDialog(this) == DialogResult.OK)
			{
				UpdateEvidenceBoardPreferences();
			}
		}

		private void UpdateEvidenceBoardPreferences()
		{
			UserLinkAttributes.Defaults.Copy(m_PrefsDlg.DefaultUserLinkAttributes);

			m_Control.DependencyColor = m_PrefsDlg.DependencyColor;
			m_Control.ParentConnectionColor = m_PrefsDlg.ParentConnectionColor;
		}

		private void OnHelp(object sender, EventArgs e)
		{
			// TODO
		}

		private void OnSetBackgroundImage(object sender, EventArgs e)
		{
			var dialog = new OpenFileDialog
			{
				InitialDirectory = m_LastBrowsedImageFolder,
				Title = m_Trans.Translate("Select Background Image"),

				AutoUpgradeEnabled = true,
				CheckFileExists = true,
				CheckPathExists = true,

				Filter = s_ImageFilter,
				FilterIndex = 0,
				RestoreDirectory = true,

				ShowReadOnly = false
			};

			if (dialog.ShowDialog() == DialogResult.OK)
			{
				m_LastBrowsedImageFolder = System.IO.Path.GetDirectoryName(dialog.FileName);

				m_Control.SetBackgroundImage(dialog.FileName);
			}
		}

		private void OnClearBackgroundImage(object sender, EventArgs e)
		{
			m_Control.ClearBackgroundImage();
		}

	}

}