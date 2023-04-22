
using System;
using System.Diagnostics;
using System.Reflection;
using System.Drawing;
using System.Windows.Forms;
using System.Windows.Forms.VisualStyles;

using Abstractspoon.Tdl.PluginHelpers;
using Abstractspoon.Tdl.PluginHelpers.ColorUtil;

namespace DayViewUIExtension
{

	[System.ComponentModel.DesignerCategory("")]
	public class DayViewUIExtensionCore : Panel, IUIExtension
	{
		private IntPtr m_HwndParent = IntPtr.Zero;
		private TDLDayView m_DayView = null;
		private Translator m_Trans = null;
		private String m_TypeId, m_UiName;
		private WorkingWeek m_WorkWeek = null;

		private const string FontName = "Tahoma";

		private static Color DefGridColor = Color.FromArgb(192, 192, 192);
        private static int LabelTop = DPIScaling.Scale(2);
        private static int ComboTop = (LabelTop + DPIScaling.Scale(2));

		private bool m_SettingMonthYear = false;
		private bool m_SettingDayViewStartDate = false;
		private bool m_AllowModifyTimeEstimate = false;

		private DayViewWeekLabel m_WeekLabel;
		private DayViewMonthComboBox m_MonthCombo;
		private DayViewYearComboBox m_YearCombo;
        private DayViewPreferencesDlg m_PrefsDlg;

        private IIControls.ToolStripEx m_Toolbar;
		private ImageList m_TBImageList;
		private UIThemeToolbarRenderer m_TBRenderer;
		private LinkLabelEx.LinkLabelEx m_SelectedTaskDatesLabel;
		private Font m_ControlsFont;
		
		// --------------------------------------------------------------------------------------

		public DayViewUIExtensionCore(String typeID, String uiName, IntPtr hwndParent, Translator trans)
		{
			m_HwndParent = hwndParent;
			m_Trans = trans;
			m_TypeId = typeID;
			m_UiName = uiName;

			InitializeComponent();
		}
		
		public bool SelectTask(UInt32 dwTaskID)
		{
            if (m_DayView.SelectedTaskID == dwTaskID)
                return true;

			bool selected = m_DayView.SelectTask(dwTaskID);

			m_SettingMonthYear = true;

			m_WeekLabel.StartDate = m_DayView.StartDate;
			m_MonthCombo.SelectedMonth = m_DayView.StartDate.Month;
			m_YearCombo.SelectedYear = m_DayView.StartDate.Year;

			m_SettingMonthYear = false;

			UpdatedSelectedTaskDatesText();

			return selected;
		}

		public bool SelectTasks(UInt32[] pdwTaskIDs)
		{
            // not currently supported
			return false;
		}

		public void UpdateTasks(TaskList tasks, UIExtension.UpdateType type)
		{
			m_DayView.UpdateTasks(tasks, type, m_TypeId);
		}

		public bool WantTaskUpdate(Task.Attribute attrib)
		{
			switch (attrib)
			{
			case Task.Attribute.Title:
			case Task.Attribute.DoneDate:
			case Task.Attribute.DueDate:
			case Task.Attribute.StartDate:
			case Task.Attribute.AllocatedTo:
			case Task.Attribute.Icon:
			case Task.Attribute.TimeEstimate:
			case Task.Attribute.Dependency:
			case Task.Attribute.Recurrence:
			case Task.Attribute.Tags:
			case Task.Attribute.CustomAttribute:
				return true;
			}

			// all else
			return false;
		}

		public bool WantSortUpdate(Task.Attribute attrib)
		{
			return false;
		}
	   
		public bool PrepareNewTask(ref Task task)
		{
            // Set the start/due dates to match the current selection
            if (m_DayView.SelectedDates.Start < m_DayView.SelectedDates.End)
			{
                task.SetStartDate(m_DayView.SelectedDates.Start);

                DateTime endDate = m_DayView.SelectedDates.End;

                if (TaskItem.IsStartOfDay(endDate))
                    endDate = endDate.AddSeconds(-1);

                task.SetDueDate(endDate);
            }

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

					if (keyPress == Keys.Escape)
						return m_DayView.CancelAppointmentResizing();

					if (keyPress == Keys.Delete)
						return m_DayView.DeleteSelectedAppointment();
				}
				break;
			}

			return false;
		}

		public bool GetLabelEditRect(ref Int32 left, ref Int32 top, ref Int32 right, ref Int32 bottom)
		{
            Rectangle rect = new Rectangle();

            if (m_DayView.GetSelectedItemLabelRect(ref rect))
            {
                rect = m_DayView.RectangleToScreen(rect);

                left = rect.Left;
                top = rect.Top;
                right = rect.Right;
				bottom = rect.Bottom;

                return true;
            }

            // else
            return false;
		}

		public UIExtension.HitResult HitTest(Int32 xScreen, Int32 yScreen)
		{
			return m_DayView.HitTest(xScreen, yScreen);
		}

		public UInt32 HitTestTask(Int32 xScreen, Int32 yScreen)
		{
			return m_DayView.HitTestTask(xScreen, yScreen);
		}

		public void SetUITheme(UITheme theme)
		{
			BackColor = theme.GetAppDrawingColor(UITheme.AppColor.AppBackLight);

			// Set the toolbar colors to be the same as the back color
			theme.SetAppDrawingColor(UITheme.AppColor.ToolbarDark, BackColor);
			theme.SetAppDrawingColor(UITheme.AppColor.ToolbarLight, BackColor);
			m_Toolbar.BackColor = BackColor;

			m_DayView.SetUITheme(theme);
			m_TBRenderer.SetUITheme(theme);

			m_WeekLabel.ForeColor = theme.GetAppDrawingColor(UITheme.AppColor.AppText);
			m_SelectedTaskDatesLabel.ForeColor = m_WeekLabel.ForeColor;
		}

		public void SetTaskFont(String faceName, int pointSize)
		{
			m_DayView.SetFont(faceName, pointSize);
		}

		public void SetReadOnly(bool bReadOnly)
		{
            m_DayView.ReadOnly = bReadOnly;
		}

		public void SavePreferences(Preferences prefs, String key)
		{
            m_PrefsDlg.SavePreferences(prefs, key);

			prefs.WriteProfileInt(key, "DaysShowing", m_DayView.DaysShowing);
		}

		public void LoadPreferences(Preferences prefs, String key, bool appOnly)
		{
			m_DayView.TaskColorIsBackground = prefs.GetProfileBool("Preferences", "ColorTaskBackground", false);
			m_DayView.ShowParentsAsFolder = prefs.GetProfileBool("Preferences", "ShowParentsAsFolders", false);
			m_DayView.AutoCalculateDependencyDates = prefs.GetProfileBool("Preferences", "AutoAdjustDependents", false);
            m_DayView.StrikeThruDoneTasks = prefs.GetProfileBool("Preferences", "StrikethroughDone", true);
			m_DayView.ShowLabelTips = !prefs.GetProfileBool("Preferences", "ShowInfoTips", false);

			m_AllowModifyTimeEstimate = !prefs.GetProfileBool("Preferences", "SyncTimeEstAndDates", false);

			m_WorkWeek.Load(prefs);
            m_DayView.WeekendDays = m_WorkWeek.WeekendDays();

            int gridColor = -1;

            if (prefs.GetProfileBool("Preferences", "SpecifyGridColor", true))
                gridColor = prefs.GetProfileInt("Preferences\\Colors", "GridLines", -1);

            m_DayView.GridlineColor = ((gridColor == -1) ? DefGridColor : DrawingColor.ToColor((UInt32)gridColor));
            
            if (!appOnly)
            {
                // private settings
                m_PrefsDlg.LoadPreferences(prefs, key);
				UpdateDayViewPreferences();

				SetDaysShowing(prefs.GetProfileInt(key, "DaysShowing", 7));
            }
			else
			{
				UpdateWorkingHourDisplay();
			}
 		}

		public bool GetTask(UIExtension.GetTask getTask, ref UInt32 taskID)
		{
            return m_DayView.GetTask(getTask, ref taskID);
		}

        public bool SelectTask(String text, UIExtension.SelectTask selectTask, bool caseSensitive, bool wholeWord, bool findReplace)
		{
            return m_DayView.SelectTask(text, selectTask, caseSensitive, wholeWord, findReplace);
		}

        public new Boolean Focus()
        {
            if (Focused)
                return false;

            return m_DayView.Focus();
        }

        public new Boolean Focused
        {
            get
            {
                foreach (Control ctrl in Controls)
                {
                    if (ctrl.CanFocus && ctrl.Focused)
                        return true;
                }

                return false;
            }
        }

		// Internal ------------------------------------------------------------------------------

		protected override void Dispose(bool disposing)
        {
            base.Dispose(disposing);

            m_PrefsDlg.Dispose();
        }

		private void InitializeComponent()
		{
			m_ControlsFont = new Font(FontName, 8);
			m_PrefsDlg = new DayViewPreferencesDlg(this, m_Trans, m_ControlsFont);
			m_WorkWeek = new WorkingWeek();

			CreateMonthYearCombos();
			CreateToolbar();
			CreateWeekLabel();
			CreateSelectedTaskDatesLabel();

			// Day view always comes last
			CreateDayView();
		}

		private void CreateDayView()
		{
			m_DayView = new TDLDayView(m_Trans,
										new UIExtension.TaskIcon(m_HwndParent),
										new UIExtension.TaskRecurrences(m_HwndParent),
										DPIScaling.Scale(5));

			m_DayView.SelectionChanged += new Calendar.AppointmentEventHandler(OnDayViewSelectionChanged);
			m_DayView.AppointmentMove += new TDLAppointmentEventHandler(OnDayViewAppointmentChanged);
			m_DayView.WeekChange += new Calendar.WeekChangeEventHandler(OnDayViewWeekChanged);
			m_DayView.MouseWheel += new MouseEventHandler(OnDayViewMouseWheel);
			m_DayView.MouseDoubleClick += new MouseEventHandler(OnDayViewMouseDoubleClick);

			// Performing icon editing from a 'MouseUp' or 'MouseClick' event 
			// causes the edit icon dialog to fail to correctly get focus but
			// counter-intuitively it works from 'MouseDown'
			m_DayView.MouseDown += new MouseEventHandler(OnDayViewMouseClick);

			m_DayView.StartDate = DateTime.Now;
			m_DayView.SetFont(FontName, 8);
			m_WeekLabel.StartDate = m_DayView.StartDate;

            if (VisualStyleRenderer.IsSupported)
                m_DayView.BorderStyle = BorderStyle.FixedSingle;
            else
                m_DayView.BorderStyle = BorderStyle.Fixed3D;

            Controls.Add(m_DayView);
		}

		private void CreateWeekLabel()
		{
			m_WeekLabel = new DayViewWeekLabel(m_Trans);

			m_WeekLabel.Font = new Font(FontName, 14);
            m_WeekLabel.Location = new Point(m_Toolbar.Right, LabelTop);
            m_WeekLabel.Height = m_Toolbar.Height;
			m_WeekLabel.TextAlign = System.Drawing.ContentAlignment.TopLeft;
			m_WeekLabel.AutoSize = true;

			Controls.Add(m_WeekLabel);
		}

		private void CreateSelectedTaskDatesLabel()
		{
			m_SelectedTaskDatesLabel = new LinkLabelEx.LinkLabelEx();

			m_SelectedTaskDatesLabel.Font = m_ControlsFont;
			m_SelectedTaskDatesLabel.Location = new Point(m_Toolbar.Right, m_Toolbar.Top);
			m_SelectedTaskDatesLabel.Height = m_Toolbar.Height;
			m_SelectedTaskDatesLabel.Width = 1024;
			m_SelectedTaskDatesLabel.TextAlign = System.Drawing.ContentAlignment.BottomLeft;
			m_SelectedTaskDatesLabel.AutoSize = false;
			m_SelectedTaskDatesLabel.ActiveLinkColor = m_SelectedTaskDatesLabel.LinkColor;
			m_SelectedTaskDatesLabel.VisitedLinkColor = m_SelectedTaskDatesLabel.LinkColor;

			m_SelectedTaskDatesLabel.LinkClicked += new LinkLabelLinkClickedEventHandler(OnClickSelectedTaskDatesLink);
			
			Controls.Add(m_SelectedTaskDatesLabel);
		}

		protected void OnClickSelectedTaskDatesLink(object sender, LinkLabelLinkClickedEventArgs e)
		{
			m_DayView.EnsureSelectionVisible(false);
			m_DayView.Focus();
		}

		private void CreateToolbar()
		{
			var assembly = Assembly.GetExecutingAssembly();
			var images = new Bitmap(assembly.GetManifestResourceStream("DayViewUIExtension.toolbar_std.bmp"));
            
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
			btn1.ImageIndex = 0;
			btn1.Click += new EventHandler(OnGoToToday);
			btn1.ToolTipText = m_Trans.Translate("Go to Today");
			m_Toolbar.Items.Add(btn1);

			m_Toolbar.Items.Add(new ToolStripSeparator());
			string format = m_Trans.Translate("{0} Day View");

			var btn2 = new ToolStripButton();
			btn2.Name = "Show1DayView";
			btn2.ImageIndex = 1;
			btn2.Click += new EventHandler(OnShow1DayView);
			btn2.ToolTipText = string.Format(format, 1);
			m_Toolbar.Items.Add(btn2);

			var btn3 = new ToolStripButton();
			btn3.Name = "Show3DayView";
			btn3.ImageIndex = 2;
			btn3.Click += new EventHandler(OnShow3DayView);
			btn3.ToolTipText = string.Format(format, 3);
			m_Toolbar.Items.Add(btn3);

			var btn4 = new ToolStripButton();
			btn4.Name = "Show7DayView";
			btn4.ImageIndex = 3;
			btn4.Click += new EventHandler(OnShow7DayView);
			btn4.ToolTipText = string.Format(format, 7);
			m_Toolbar.Items.Add(btn4);

			var btn5 = new ToolStripButton();
			btn5.Name = "Show14DayView";
			btn5.ImageIndex = 4;
			btn5.Click += new EventHandler(OnShow14DayView);
			btn5.ToolTipText = string.Format(format, 14);
			m_Toolbar.Items.Add(btn5);

			var btn6 = new ToolStripButton();
			btn6.Name = "Show28DayView";
			btn6.ImageIndex = 5;
			btn6.Click += new EventHandler(OnShow28DayView);
			btn6.ToolTipText = string.Format(format, 28);
			m_Toolbar.Items.Add(btn6);

			m_Toolbar.Items.Add(new ToolStripSeparator());

			var btn7 = new ToolStripButton();
			btn7.Name = "NewTimeBlock";
			btn7.ImageIndex = 6;
			btn7.Click += new EventHandler(OnNewTimeBlock);
			btn7.ToolTipText = m_Trans.Translate("New Time Block");
			m_Toolbar.Items.Add(btn7);

			var btn8 = new ToolStripButton();
			btn8.Name = "DuplicateTimeBlock";
			btn8.ImageIndex = 7;
			btn8.Click += new EventHandler(OnDuplicateTimeBlock);
			btn8.ToolTipText = m_Trans.Translate("Duplicate Time Block");
			m_Toolbar.Items.Add(btn8);

			m_Toolbar.Items.Add(new ToolStripSeparator());

			var btn9 = new ToolStripButton();
			btn9.ImageIndex = 8;
			btn9.Click += new EventHandler(OnPreferences);
			btn9.ToolTipText = m_Trans.Translate("Preferences");
			m_Toolbar.Items.Add(btn9);

			m_Toolbar.Items.Add(new ToolStripSeparator());

			var btn10 = new ToolStripButton();
			btn10.ImageIndex = 9;
			btn10.Click += new EventHandler(OnHelp);
			btn10.ToolTipText = m_Trans.Translate("Online Help");
			m_Toolbar.Items.Add(btn10);

			Toolbars.FixupButtonSizes(m_Toolbar);

			Controls.Add(m_Toolbar);
		}

		private void OnGoToToday(object sender, EventArgs e)
		{
            m_DayView.GoToToday();
		}

		private void OnShow1DayView(object sender, EventArgs e)
		{
			if (m_DayView.DaysShowing != 1)
				SetDaysShowing(1);
		}

		private void OnShow3DayView(object sender, EventArgs e)
		{
			if (m_DayView.DaysShowing != 3)
				SetDaysShowing(3);
		}

        private void OnShow7DayView(object sender, EventArgs e)
        {
            if (m_DayView.DaysShowing != 7)
                SetDaysShowing(7);
        }

		private void OnShow14DayView(object sender, EventArgs e)
		{
			if (m_DayView.DaysShowing != 14)
				SetDaysShowing(14);
		}

		private void OnShow28DayView(object sender, EventArgs e)
		{
			if (m_DayView.DaysShowing != 28)
				SetDaysShowing(28);
		}

		private void SetDaysShowing(int numDays)
		{
			m_DayView.DaysShowing = numDays;
            m_WeekLabel.NumDays = numDays;

			string format = m_Trans.Translate("Next/Previous {0} days");
			m_DayView.HScrollTooltipText = String.Format(format, m_DayView.HScrollStep);

			UpdateToolbarButtonStates();
            UpdatedSelectedTaskDatesPosition();
		}

		private void OnNewTimeBlock(object sender, EventArgs e)
		{
			// Display a dialog to retrieve the task ID from a list
			// to support tasks without dates
			var taskID = m_DayView.SelectedTaskID;

			// TODO 


			if (taskID != 0)
			{
				if (m_DayView.CreateNewTaskBlock(taskID))
				{
					var notify = new UIExtension.ParentNotify(m_HwndParent);
					TaskItem task = null;

					if (m_DayView.SelectedAppointment is TaskExtensionItem)
						task = (m_DayView.SelectedAppointment as TaskExtensionItem).RealTask;
					else
						task = (m_DayView.SelectedAppointment as TaskItem);

					notify.NotifyMod(Task.Attribute.MetaData, task.EncodeTimeBlocks());
				}
			}
		}

		private void OnDuplicateTimeBlock(object sender, EventArgs e)
		{
			if (m_DayView.DuplicateSelectedTimeBlock())
			{
				var notify = new UIExtension.ParentNotify(m_HwndParent);
				TaskItem task = null;

				if (m_DayView.SelectedAppointment is TaskExtensionItem)
					task = (m_DayView.SelectedAppointment as TaskExtensionItem).RealTask;
				else
					task = (m_DayView.SelectedAppointment as TaskItem);

				notify.NotifyMod(Task.Attribute.MetaData, task.EncodeTimeBlocks());
			}
		}

		private void UpdateToolbarButtonStates()
		{
			(m_Toolbar.Items["Show1DayView"] as ToolStripButton).Checked = (m_DayView.DaysShowing == 1);
			(m_Toolbar.Items["Show3DayView"] as ToolStripButton).Checked = (m_DayView.DaysShowing == 3);
            (m_Toolbar.Items["Show7DayView"] as ToolStripButton).Checked = (m_DayView.DaysShowing == 7);
			(m_Toolbar.Items["Show14DayView"] as ToolStripButton).Checked = (m_DayView.DaysShowing == 14);
            (m_Toolbar.Items["Show28DayView"] as ToolStripButton).Checked = (m_DayView.DaysShowing == 28);

			m_Toolbar.Items["NewTimeBlock"].Enabled = m_DayView.CanCreateNewTimeBlock();
			m_Toolbar.Items["DuplicateTimeBlock"].Enabled = m_DayView.CanDuplicateTimeBlock();
		}

		private void OnPreferences(object sender, EventArgs e)
		{
			m_PrefsDlg.StartPosition = FormStartPosition.CenterParent;

            if (m_PrefsDlg.ShowDialog() == DialogResult.OK)
            {
				UpdateDayViewPreferences();
            }
		}

		private void UpdateDayViewPreferences()
		{
			string tag;
            m_DayView.SetHideParentTasks(m_PrefsDlg.GetHideParentTasks(out tag), tag);

			m_DayView.DisplayTasksContinuous = m_PrefsDlg.DisplayTasksContinuous;
			m_DayView.HideTasksWithoutTimes = m_PrefsDlg.HideTasksWithoutTimes;
            m_DayView.HideTasksSpanningWeekends = m_PrefsDlg.HideTasksSpanningWeekends;
            m_DayView.HideTasksSpanningDays = m_PrefsDlg.HideTasksSpanningDays;
			m_DayView.ShowFutureOccurrences = m_PrefsDlg.ShowFutureOccurrences;

			m_DayView.SlotsPerHour = (60 / m_PrefsDlg.SlotMinutes);
			m_DayView.MinSlotHeight = DPIScaling.Scale(m_PrefsDlg.MinSlotHeight);

			UpdateWorkingHourDisplay();

			m_DayView.Invalidate();
		}

		private void UpdateWorkingHourDisplay()
		{
            m_DayView.WorkStart.Set(m_WorkWeek.WorkDay().StartOfDayInHours());
            m_DayView.WorkEnd.Set(m_WorkWeek.WorkDay().EndOfDayInHours());

            m_DayView.LunchStart.Set(m_WorkWeek.WorkDay().StartOfLunchInHours());
            m_DayView.LunchEnd.Set(m_WorkWeek.WorkDay().EndOfLunchInHours());
		}

		private void OnHelp(object sender, EventArgs e)
		{
            UIExtension.ParentNotify notify = new UIExtension.ParentNotify(m_HwndParent);

            notify.NotifyDoHelp(m_TypeId);
		}

		private void CreateMonthYearCombos()
		{
			m_MonthCombo = new DayViewMonthComboBox();

			m_MonthCombo.Font = m_ControlsFont;
            m_MonthCombo.Location = new Point(DPIScaling.Scale(0), ComboTop);
            m_MonthCombo.Size = DPIScaling.Scale(new Size(100, 16));
			
			m_MonthCombo.SelectedMonth = DateTime.Now.Month;
			m_MonthCombo.SelectedIndexChanged += new EventHandler(OnMonthYearSelChanged);
			
			Controls.Add(m_MonthCombo);

			m_YearCombo = new DayViewYearComboBox();

			m_YearCombo.Font = m_ControlsFont;
            m_YearCombo.Location = new Point(DPIScaling.Scale(105), ComboTop);
            m_YearCombo.Size = DPIScaling.Scale(new Size(100, 16));

			m_YearCombo.SelectedYear = DateTime.Now.Year;
			m_YearCombo.SelectedIndexChanged += new EventHandler(OnMonthYearSelChanged);
			
			Controls.Add(m_YearCombo);
		}

		// Message handlers ------------------------------------------------------------------
        protected override void OnGotFocus(EventArgs e)
        {
            base.OnGotFocus(e);

            m_DayView.Focus();
        }

        protected override void OnSizeChanged(EventArgs e)
        {
            base.OnSizeChanged(e);

			m_YearCombo.Location = new Point(m_MonthCombo.Right + 10, ComboTop);
			m_Toolbar.Location = new Point(m_YearCombo.Right + 10, LabelTop - 2);
			m_WeekLabel.Location = new Point(m_Toolbar.Right + 10, LabelTop);

			UpdatedSelectedTaskDatesPosition(); // called elsewhere also

			Rectangle dayViewRect = ClientRectangle;

			dayViewRect.Y = ControlTop;
			dayViewRect.Height -= ControlTop;
			dayViewRect.Inflate(-1, -1);

			m_DayView.Bounds = dayViewRect;

            Invalidate(true);
        }

		private void HandleDayViewMouseClick(MouseEventArgs e, bool doubleClick)
		{
			if (m_DayView.ReadOnly)
				return;

			var appt = m_DayView.GetRealAppointmentAt(e.Location.X, e.Location.Y);

			if (appt == null || appt.Locked)
				return;

			if (m_DayView.IconHitTest(m_DayView.PointToScreen(e.Location)) > 0)
			{
				var notify = new UIExtension.ParentNotify(m_HwndParent);
				notify.NotifyEditIcon();
			}
			else if (doubleClick)
			{
				var notify = new UIExtension.ParentNotify(m_HwndParent);
				notify.NotifyEditLabel();
			}
		}

		private void OnDayViewMouseClick(object sender, MouseEventArgs e)
        {
			HandleDayViewMouseClick(e, false);
        }

		private void OnDayViewMouseDoubleClick(object sender, MouseEventArgs e)
		{
			HandleDayViewMouseClick(e, true);
		}

		private void OnDayViewMouseWheel(object sender, MouseEventArgs e)
		{
			if (ModifierKeys.Equals(Keys.Control))
			{
				bool change = ((e.Delta < 0) ? m_PrefsDlg.DecrementMinSlotHeight() : m_PrefsDlg.IncrementMinSlotHeight());

				if (change)
					m_DayView.MinSlotHeight = m_PrefsDlg.MinSlotHeight;
			}
		}

		private void OnDayViewSelectionChanged(object sender, Calendar.AppointmentEventArgs args)
		{
            UIExtension.ParentNotify notify = new UIExtension.ParentNotify(m_HwndParent);

			switch (m_DayView.SelectionType)
			{
			case Calendar.SelectionType.Appointment:
				UpdatedSelectedTaskDatesText();
				UpdateToolbarButtonStates();

				notify.NotifySelChange(m_DayView.SelectedTaskID);
				break;

			case Calendar.SelectionType.DateRange:
				UpdateToolbarButtonStates();
				break;
			}
		}

		private void UpdatedSelectedTaskDatesText()
		{
			DateTime from, to;

			if (m_DayView.GetSelectedTaskDates(out from, out to))
			{
				String label = String.Format("{0}: ", m_Trans.Translate("Selected Task Date Range"));

				String toDate = to.ToString((from.DayOfYear == to.DayOfYear) ? "t" : "g");
				String dateRange = String.Format("{0} - {1}", from.ToString("g"), toDate);

				m_SelectedTaskDatesLabel.Text = (label + dateRange);
				m_SelectedTaskDatesLabel.LinkArea = new LinkArea(label.Length, dateRange.Length);
			}
			else
			{
				m_SelectedTaskDatesLabel.Text = String.Empty;
			}
		}

		private void UpdatedSelectedTaskDatesPosition()
		{
			m_SelectedTaskDatesLabel.Location = new Point(m_WeekLabel.Right + 10, m_YearCombo.Bottom - m_SelectedTaskDatesLabel.Height);
		}

		private void OnDayViewWeekChanged(object sender, Calendar.WeekChangeEventArgs args)
		{
			m_WeekLabel.StartDate = args.StartDate;

			if (!m_SettingDayViewStartDate)
			{
				m_SettingMonthYear = true;

				m_MonthCombo.SelectedMonth = args.StartDate.Month;
				m_YearCombo.SelectedYear = args.StartDate.Year;

				UpdatedSelectedTaskDatesPosition();

				m_SettingMonthYear = false;
			}
		}

		private void OnMonthYearSelChanged(object sender, EventArgs args)
		{
			if (!m_SettingMonthYear)
			{
				m_SettingDayViewStartDate = true;

				m_DayView.StartDate = new DateTime(m_YearCombo.SelectedYear, m_MonthCombo.SelectedMonth, 1);
				m_WeekLabel.StartDate = m_DayView.StartDate;

				UpdatedSelectedTaskDatesPosition();

				m_SettingDayViewStartDate = false;
			}
		}

        private void OnDayViewAppointmentChanged(object sender, TDLMoveAppointmentEventArgs args)
		{
			ProcessTaskAppointmentChange(args);
		}

		private void ProcessTaskAppointmentChange(TDLMoveAppointmentEventArgs args)
		{
            if (!args.Finished && !args.IsTimeBlock)
            {
                UpdatedSelectedTaskDatesText();
                return;
            }

			var item = args.Appointment as TaskItem;

			if (item == null)
				return;

			var notify = new UIExtension.ParentNotify(m_HwndParent);

			if (args.IsTimeBlock)
			{
				notify.NotifyMod(Task.Attribute.MetaData, item.EncodeTimeBlocks());
				return;
			}

			// else
			if (!string.IsNullOrEmpty(args.CustomAttributeId))
			{
				var date = item.CustomDates[args.CustomAttributeId];

				if (date == DateTime.MinValue)
					notify.NotifyMod(args.CustomAttributeId, string.Empty);
				else
					notify.NotifyMod(args.CustomAttributeId, date.ToString());

				return;
			}

			// Start/Due change
			if (PrepareTaskNotify(item, args.Mode, notify))
			{
				bool modifyTimeEst = WantModifyTimeEstimate(item);

				if (notify.NotifyMod())
				{
					item.UpdateOriginalDates();

					if (modifyTimeEst)
						item.TimeEstimate = item.LengthAsTimeEstimate(m_WorkWeek, false);

					return;
				}
			}

			item.RestoreOriginalDates();
			m_DayView.Invalidate();
		}

		private bool PrepareTaskNotify(TaskItem item, Calendar.SelectionTool.Mode mode, UIExtension.ParentNotify notify, bool includeTimeEstimate = true)
		{
			switch (mode)
			{
			case Calendar.SelectionTool.Mode.Move:
				if (item.LengthDiffersFromOriginal())
				{
					// Start date WITHOUT TIME ESTIMATE
					PrepareTaskNotify(item, Calendar.SelectionTool.Mode.ResizeLeft, notify, false);

					// End date WITHOUT TIME ESTIMATE
					PrepareTaskNotify(item, Calendar.SelectionTool.Mode.ResizeRight, notify, false);

					if (includeTimeEstimate && WantModifyTimeEstimate(item))
					{
						notify.AddMod(Task.Attribute.TimeEstimate, item.LengthAsTimeEstimate(m_WorkWeek, false), item.TimeEstUnits);
					}

					return true;
				}
				else if (item.StartDateDiffersFromOriginal())
				{
					notify.AddMod(Task.Attribute.OffsetTask, item.StartDate);

					return true;
				}
				break;

			case Calendar.SelectionTool.Mode.ResizeLeft:
			case Calendar.SelectionTool.Mode.ResizeTop:
				if (item.StartDateDiffersFromOriginal())
				{
					notify.AddMod(Task.Attribute.StartDate, item.StartDate);

					if (includeTimeEstimate && WantModifyTimeEstimate(item))
					{
						notify.AddMod(Task.Attribute.TimeEstimate, item.LengthAsTimeEstimate(m_WorkWeek, false), item.TimeEstUnits);
					}

					return true;
				}
				break;

			case Calendar.SelectionTool.Mode.ResizeRight:
			case Calendar.SelectionTool.Mode.ResizeBottom:
				if (item.EndDateDiffersFromOriginal())
				{
					// Allow for end of day
					var endDate = item.EndDate;

					if (endDate == endDate.Date)
						endDate = endDate.AddDays(-1);

					if (item.IsDone)
						notify.AddMod(Task.Attribute.DoneDate, endDate);
					else
						notify.AddMod(Task.Attribute.DueDate, endDate);

					if (includeTimeEstimate && WantModifyTimeEstimate(item))
					{
						notify.AddMod(Task.Attribute.TimeEstimate, item.LengthAsTimeEstimate(m_WorkWeek, false), item.TimeEstUnits);
					}

					return true;
				}
				break;
			}

			return false;
		}

        private bool WantModifyTimeEstimate(TaskItem item)
        {
			if (!m_AllowModifyTimeEstimate)
				return false;

			if (!item.TimeEstimateIsMinsOrHours)
				return false;

			if (!item.IsSingleDay || (item.Length.Days != 0))
				return false;

			if (item.TimeEstimateMatchesOriginalLength(m_WorkWeek))
				return true;

			if (item.TimeEstimate != 0.0)
				return false;

			// else
			return true;
        }

        private int ControlTop
        {
            get
            {
                if (m_MonthCombo != null)
                    return m_MonthCombo.Bounds.Bottom + DPIScaling.Scale(4);

                // else
                return 0;
            }
        }

		public Bitmap SaveToImage()
		{
			return m_DayView.SaveToImage();
		}

		public Boolean CanSaveToImage()
		{
			return m_DayView.CanSaveToImage();
		}

	}


}
