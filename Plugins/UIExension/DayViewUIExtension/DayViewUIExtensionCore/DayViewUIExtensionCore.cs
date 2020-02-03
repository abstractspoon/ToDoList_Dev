
using System;
using System.IO;
using System.Reflection;
using System.Collections.Generic;
using System.Drawing;
using System.Windows.Forms;

using Abstractspoon.Tdl.PluginHelpers;

namespace DayViewUIExtension
{
	[System.ComponentModel.DesignerCategory("")]
	public class DayViewUIExtensionCore : Panel, IUIExtension
	{
		private IntPtr m_HwndParent = IntPtr.Zero;
		private TDLDayView m_DayView = null;
		private Translator m_Trans = null;
		private UIExtension.TaskIcon m_TaskIcons = null;
		private String m_TypeId;
		private WorkingWeek m_WorkWeek = null;

		// --------------------------------------------------------------------------------------

		private const string FontName = "Tahoma";

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
		private Label m_SelectedTaskDatesLabel;

		private Font m_ControlsFont;
		private int m_BannerHeight;

		// --------------------------------------------------------------------------------------

		public DayViewUIExtensionCore(String typeID, IntPtr hwndParent, Translator trans)
		{
			m_HwndParent = hwndParent;
			m_Trans = trans;
            m_TypeId = typeID;

			InitializeComponent();
		}
		
		public bool SelectTask(UInt32 dwTaskID)
		{
            if (m_DayView.GetSelectedTaskID() == dwTaskID)
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
			m_DayView.UpdateTasks(tasks, type);
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
					return true;
			}

			// all else
			return false;
		}

		private int LabelTop
		{
			get { return (m_BannerHeight + DPIScaling.Scale(2)); }
		}

		private int ComboTop
		{
			get { return LabelTop; }
		}

		public bool WantSortUpdate(Task.Attribute attrib)
		{
			return false;
		}
	   
		public bool PrepareNewTask(ref Task task)
		{
            // Set the start/due dates to match the current selection
            if (m_DayView.SelectionStart < m_DayView.SelectionEnd)
			{
                task.SetStartDate(m_DayView.SelectionStart);

                DateTime endDate = m_DayView.SelectionEnd;

                if (CalendarItem.IsStartOfDay(endDate))
                    endDate = endDate.AddSeconds(-1);

                task.SetDueDate(endDate);
            }

			return true;
		}

		public bool ProcessMessage(IntPtr hwnd, UInt32 message, UInt32 wParam, UInt32 lParam, UInt32 time, Int32 xPos, Int32 yPos)
		{
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
            m_DayView.SetUITheme(theme);
			m_Toolbar.BackColor = theme.GetAppDrawingColor(UITheme.AppColor.AppBackLight);
			m_TBRenderer.SetUITheme(theme);

            BackColor = theme.GetAppDrawingColor(UITheme.AppColor.AppBackLight);

			RhinoLicensing.SetUITheme(this, theme);
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
			switch (getTask)
			{
				case UIExtension.GetTask.GetNextTask:
					break;

				case UIExtension.GetTask.GetPrevTask:
					break;
			}

			// all else
			return false;
		}

        public bool SelectTask(String text, UIExtension.SelectTask selectTask, bool caseSensitive, bool wholeWord, bool findReplace)
		{
			switch (selectTask)
			{
				case UIExtension.SelectTask.SelectFirstTask:
					break;

				case UIExtension.SelectTask.SelectNextTask:
					break;

				case UIExtension.SelectTask.SelectNextTaskInclCurrent:
					break;

				case UIExtension.SelectTask.SelectPrevTask:
					break;

				case UIExtension.SelectTask.SelectLastTask:
					break;
			}

			// all else
			return false;
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
			m_TaskIcons = new UIExtension.TaskIcon(m_HwndParent);
			m_ControlsFont = new Font(FontName, 8);
			m_PrefsDlg = new DayViewPreferencesDlg(m_Trans, m_ControlsFont);
			m_WorkWeek = new WorkingWeek();

			m_BannerHeight = RhinoLicensing.CreateBanner(m_TypeId, this, m_Trans, -1);

			CreateMonthYearCombos();
			CreateToolbar();
			CreateWeekLabel();
			CreateSelectedTaskDatesLabel();

			// Day view always comes last
			CreateDayView();
		}

		private void CreateDayView()
		{
			m_DayView = new TDLDayView(m_TaskIcons, DPIScaling.Scale(5));

			m_DayView.NewAppointment += new Calendar.NewAppointmentEventHandler(OnDayViewNewAppointment);
			m_DayView.SelectionChanged += new Calendar.AppointmentEventHandler(OnDayViewSelectionChanged);
			m_DayView.AppointmentMove += new Calendar.AppointmentEventHandler(OnDayViewAppointmentChanged);
			m_DayView.WeekChange += new Calendar.WeekChangeEventHandler(OnDayViewWeekChanged);
			m_DayView.MouseWheel += new MouseEventHandler(OnDayViewMouseWheel);
			m_DayView.MouseDoubleClick += new MouseEventHandler(OnDayViewMouseDoubleClick);

			// Performing icon editing from a 'MouseUp' or 'MouseClick' event 
			// causes the edit icon dialog to fail to correctly get focus but
			// counter-intuitively it works from 'MouseDown'
			m_DayView.MouseDown += new MouseEventHandler(OnDayViewMouseClick);

			m_DayView.StartDate = DateTime.Now;
			m_WeekLabel.StartDate = m_DayView.StartDate;
			m_DayView.SetFont(FontName, 8);

			Controls.Add(m_DayView);

		}

		private void CreateWeekLabel()
		{
			m_WeekLabel = new DayViewWeekLabel(m_Trans);

			m_WeekLabel.Font = new Font(FontName, 14);
            m_WeekLabel.Location = new Point(m_Toolbar.Right, LabelTop);
            m_WeekLabel.Height = m_Toolbar.Height;
			m_WeekLabel.TextAlign = ContentAlignment.TopLeft;
			m_WeekLabel.AutoSize = true;

			Controls.Add(m_WeekLabel);
		}

		private void CreateSelectedTaskDatesLabel()
		{
			m_SelectedTaskDatesLabel = new Label();

			m_SelectedTaskDatesLabel.Font = m_ControlsFont;
			m_SelectedTaskDatesLabel.Location = new Point(m_Toolbar.Right, m_Toolbar.Bottom);
			m_SelectedTaskDatesLabel.Height = m_Toolbar.Height;
			m_SelectedTaskDatesLabel.TextAlign = ContentAlignment.MiddleLeft;
			m_SelectedTaskDatesLabel.AutoSize = true;

			Controls.Add(m_SelectedTaskDatesLabel);
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

			var sep1 = new ToolStripSeparator();
			m_Toolbar.Items.Add(sep1);

			var btn2 = new ToolStripButton();
			btn2.Name = "Show1DayView";
			btn2.ImageIndex = 1;
			btn2.Click += new EventHandler(OnShow1DayView);
			btn2.ToolTipText = m_Trans.Translate("1 Day View");
			m_Toolbar.Items.Add(btn2);

			var btn3 = new ToolStripButton();
			btn3.Name = "Show3DayView";
			btn3.ImageIndex = 2;
			btn3.Click += new EventHandler(OnShow3DayView);
			btn3.ToolTipText = m_Trans.Translate("3 Day View");
			m_Toolbar.Items.Add(btn3);

			var btn4 = new ToolStripButton();
			btn4.Name = "Show7DayView";
			btn4.ImageIndex = 3;
			btn4.Click += new EventHandler(OnShow7DayView);
			btn4.ToolTipText = m_Trans.Translate("7 day View");
			m_Toolbar.Items.Add(btn4);

			var sep2 = new ToolStripSeparator();
			m_Toolbar.Items.Add(sep2);

			var btn5 = new ToolStripButton();
			btn5.ImageIndex = 4;
			btn5.Click += new EventHandler(OnPreferences);
			btn5.ToolTipText = m_Trans.Translate("Preferences");
			m_Toolbar.Items.Add(btn5);

			var btn6 = new ToolStripButton();
			btn6.ImageIndex = 5;
			btn6.Click += new EventHandler(OnHelp);
			btn6.ToolTipText = m_Trans.Translate("Online Help");
			m_Toolbar.Items.Add(btn6);

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

        private void SetDaysShowing(int numDays)
		{
			m_DayView.DaysShowing = numDays;
            m_WeekLabel.NumDays = numDays;

            String tooltip = String.Format("Next/Previous {0} days", numDays);
			m_DayView.HScrollTooltipText = m_Trans.Translate(tooltip);

			UpdateToolbarButtonStates();
		}

		private void UpdateToolbarButtonStates()
		{
			(m_Toolbar.Items["Show1DayView"] as ToolStripButton).Checked = (m_DayView.DaysShowing == 1);
			(m_Toolbar.Items["Show3DayView"] as ToolStripButton).Checked = (m_DayView.DaysShowing == 3);
            (m_Toolbar.Items["Show7DayView"] as ToolStripButton).Checked = (m_DayView.DaysShowing == 7);
        }

        private void OnPreferences(object sender, EventArgs e)
		{
			m_PrefsDlg.StartPosition = FormStartPosition.CenterParent;

            if (m_PrefsDlg.ShowDialog(Control.FromHandle(m_HwndParent)) == DialogResult.OK)
            {
				UpdateDayViewPreferences();
            }
		}

		private void UpdateDayViewPreferences()
		{
            m_DayView.HideParentTasks = m_PrefsDlg.HideParentTasks;
			m_DayView.DisplayTasksContinuous = m_PrefsDlg.DisplayTasksContinuous;
			m_DayView.HideTasksWithoutTimes = m_PrefsDlg.HideTasksWithoutTimes;
            m_DayView.HideTasksSpanningWeekends = m_PrefsDlg.HideTasksSpanningWeekends;
            m_DayView.HideTasksSpanningDays = m_PrefsDlg.HideTasksSpanningDays;

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
		protected override void OnPaint(System.Windows.Forms.PaintEventArgs e)
        {
            Rectangle Border = new Rectangle(ClientRectangle.Location, ClientRectangle.Size);
			Border.Y = ControlTop;
			Border.Height -= ControlTop;

            System.Windows.Forms.ControlPaint.DrawBorder(e.Graphics, Border, Color.DarkGray, System.Windows.Forms.ButtonBorderStyle.Solid);
        }

        protected override void OnGotFocus(EventArgs e)
        {
            base.OnGotFocus(e);

            m_DayView.Focus();
        }

        protected override void OnSizeChanged(EventArgs e)
        {
            base.OnSizeChanged(e);

			m_YearCombo.Location = new Point(m_MonthCombo.Right + 10, m_YearCombo.Top);
			m_Toolbar.Location = new Point(m_YearCombo.Right + 10, m_YearCombo.Top - 2);
			m_WeekLabel.Location = new Point(m_Toolbar.Right + 10, m_YearCombo.Top);
			UpdatedSelectedTaskDatesPosition(); // called elsewhere also

			Rectangle dayViewRect = new Rectangle(ClientRectangle.Location, ClientRectangle.Size);

			dayViewRect.Y = ControlTop;
			dayViewRect.Height -= ControlTop;
			dayViewRect.Inflate(-1, -1);

			m_DayView.Location = dayViewRect.Location;
			m_DayView.Size = dayViewRect.Size;

            Invalidate(true);
        }

		private void HandleDayViewMouseClick(MouseEventArgs e, bool doubleClick)
		{
			if (m_DayView.ReadOnly)
				return;

			Calendar.Appointment appointment = m_DayView.GetAppointmentAt(e.Location.X, e.Location.Y);

			if (appointment == null)
				return;

			var taskItem = (appointment as CalendarItem);

			if ((taskItem == null) || taskItem.IsLocked)
				return;

			if (taskItem.IconRect.Contains(e.Location))
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

		private void OnDayViewNewAppointment(object sender, Calendar.NewAppointmentEventArgs args)
		{
		}

		private void OnDayViewSelectionChanged(object sender, Calendar.AppointmentEventArgs args)
		{
            UIExtension.ParentNotify notify = new UIExtension.ParentNotify(m_HwndParent);

			if (m_DayView.Selection != Calendar.SelectionType.None)
			{
				UpdatedSelectedTaskDatesText();

				if (args.Appointment != null)
					notify.NotifySelChange(args.Appointment.Id);
				else
					notify.NotifySelChange(0);
			}
		}

		private void UpdatedSelectedTaskDatesText()
		{
			DateTime from = DateTime.MinValue, to = DateTime.MinValue;

			if (m_DayView.GetSelectedTaskDates(ref from, ref to) &&
				(from != DateTime.MinValue) &&
				(to != DateTime.MinValue))
			{
				String toDate = to.ToString((from.DayOfYear == to.DayOfYear) ? "t" : "g");

				m_SelectedTaskDatesLabel.Text = String.Format("{0}: {1}-{2}",
												m_Trans.Translate("Selected Task Date Range"),
												from.ToString("g"), toDate);
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

        private void OnDayViewAppointmentChanged(object sender, Calendar.AppointmentEventArgs args)
		{
			var move = args as Calendar.MoveAppointmentEventArgs;

			// Ignore moves whilst they are occurring
			if ((move == null) || !move.Finished)
				return;

			var item = args.Appointment as CalendarItem;

			if (item == null)
				return;

			var notify = new UIExtension.ParentNotify(m_HwndParent);

			switch (move.Mode)
			{
				case Calendar.SelectionTool.Mode.Move:
					if (item.StartDateDiffersFromOriginal())
					{
						if (notify.NotifyMod(Task.Attribute.OffsetTask, item.StartDate))
						{
							item.UpdateOriginalDates();
							m_DayView.FixupSelection(true, false);
						}
						else
						{
							item.RestoreOriginalDates();
							m_DayView.Invalidate();
						}
					}
					break;

				case Calendar.SelectionTool.Mode.ResizeLeft:
				case Calendar.SelectionTool.Mode.ResizeTop:
					if (item.StartDateDiffersFromOriginal())
					{
                        notify.AddMod(Task.Attribute.StartDate, item.StartDate);

                        // Update the Time estimate if it is zero or 
                        // it used to match the previous date range
                        bool modifyTimeEst = WantModifyTimeEstimate(item);

                        if (modifyTimeEst)
                            notify.AddMod(Task.Attribute.TimeEstimate, item.LengthAsTimeEstimate(m_WorkWeek, false), item.TimeEstUnits);

						if (notify.NotifyMod())
						{
							item.UpdateOriginalDates();

							if (modifyTimeEst)
                                item.TimeEstimate = item.LengthAsTimeEstimate(m_WorkWeek, false);
						}
						else
						{
							item.RestoreOriginalDates();
							m_DayView.Invalidate();
						}
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

						// Update the Time estimate if used to match the previous date range
						bool modifyTimeEst = WantModifyTimeEstimate(item);

                        if (modifyTimeEst)
                            notify.AddMod(Task.Attribute.TimeEstimate, item.LengthAsTimeEstimate(m_WorkWeek, false), item.TimeEstUnits);

						if (notify.NotifyMod())
						{
							item.UpdateOriginalDates();

							if (modifyTimeEst)
                                item.TimeEstimate = item.LengthAsTimeEstimate(m_WorkWeek, false);
						}
						else
						{
							item.RestoreOriginalDates();
							m_DayView.Invalidate();
						}
					}
					break;
			}
		}

        private bool WantModifyTimeEstimate(CalendarItem item)
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
                    return m_MonthCombo.Bounds.Bottom + new DlgUnits(m_HwndParent).ToPixelsY(4);

                // else
                return 0;
            }
        }

	}


}
