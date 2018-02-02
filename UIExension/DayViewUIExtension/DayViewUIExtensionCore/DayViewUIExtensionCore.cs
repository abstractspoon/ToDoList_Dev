
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
		private IntPtr m_hwndParent = IntPtr.Zero;
		private TDLDayView m_DayView;
		private UInt32 m_SelectedTaskID = 0;
		private Translator m_Trans;
		private UIExtension.TaskIcon m_TaskIcons;
        private String m_HelpID;

        [Flags] private enum WeekendDays
        {
	        SUNDAY		= 0X01,
	        MONDAY		= 0X02,
	        TUESDAY		= 0X04,
	        WEDNESDAY	= 0X08,
	        THURSDAY	= 0X10,
	        FRIDAY		= 0X20,
	        SATURDAY	= 0X40,
        }

		// --------------------------------------------------------------------------------------

		private const int LabelTop = 2;
		private const int ComboTop = 2;
		private const int ControlTop = 28;
		private const string FontName = "Tahoma";

		private bool m_SettingMonthYear = false;
		private bool m_SettingDayViewStartDate = false;

		private DayViewWeekLabel m_WeekLabel;
		private DayViewMonthComboBox m_MonthCombo;
		private DayViewYearComboBox m_YearCombo;
        private DayViewPreferencesDlg m_PrefsDlg;

        private ToolStrip m_Toolbar;
		private ImageList m_TBImageList;
		private UIThemeToolbarRenderer m_TBRenderer;

		private Font m_ControlsFont;

		// --------------------------------------------------------------------------------------

		public DayViewUIExtensionCore(IntPtr hwndParent, Translator trans, String helpID)
		{
			m_hwndParent = hwndParent;
			m_Trans = trans;
            m_HelpID = helpID;

			m_TaskIcons = new UIExtension.TaskIcon(hwndParent);
			m_ControlsFont = new Font(FontName, 8);
            m_PrefsDlg = new DayViewPreferencesDlg();

			InitializeComponent();
		}
		
		public bool SelectTask(UInt32 dwTaskID)
		{
			// Don't select already selected task
			if (dwTaskID == m_SelectedTaskID)
				return true;

			if (m_DayView.SelectTask(dwTaskID, false))
			{
				m_SelectedTaskID = dwTaskID;
				return true;
			}

			// all else 
			m_DayView.SelectedAppointment = null;
			return true;
		}

		public bool SelectTasks(UInt32[] pdwTaskIDs)
		{
            // not currently supported
			return false;
		}

		public void UpdateTasks(TaskList tasks, 
								UIExtension.UpdateType type, 
								HashSet<UIExtension.TaskAttribute> attribs)
		{
			m_DayView.UpdateTasks(tasks, type, attribs);

            // Refresh selection
            RestoreSelectedItem();
		}

		public bool WantEditUpdate(UIExtension.TaskAttribute attrib)
		{
			switch (attrib)
			{
				case UIExtension.TaskAttribute.Title:
				case UIExtension.TaskAttribute.DoneDate:
				case UIExtension.TaskAttribute.DueDate:
				case UIExtension.TaskAttribute.StartDate:
				case UIExtension.TaskAttribute.AllocTo:
				case UIExtension.TaskAttribute.Icon:
					return true;
			}

			// all else
			return false;
		}
	   
		public bool WantSortUpdate(UIExtension.TaskAttribute attrib)
		{
			return false;
		}
	   
		public bool PrepareNewTask(ref Task task)
		{
            // Set the start/due dates to match the current selection
            if (m_DayView.SelectionStart < m_DayView.SelectionEnd)
			{
                task.SetStartDate(m_DayView.SelectionStart);
                task.SetDueDate(m_DayView.SelectionEnd);
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

		public void SetUITheme(UITheme theme)
		{
            m_DayView.SetUITheme(theme);
			m_Toolbar.BackColor = theme.GetAppDrawingColor(UITheme.AppColor.AppBackLight);
			m_TBRenderer.SetUITheme(theme);

            BackColor = theme.GetAppDrawingColor(UITheme.AppColor.AppBackLight);
		}

		public void SetReadOnly(bool bReadOnly)
		{
		}

		public void SavePreferences(Preferences prefs, String key)
		{
            m_PrefsDlg.SavePreferences(prefs, key);
		}

		public void LoadPreferences(Preferences prefs, String key, bool appOnly)
		{
            if (!appOnly)
            {
                // private settings
                m_PrefsDlg.LoadPreferences(prefs, key);
				UpdateDayViewPreferences();
            }

			bool taskColorIsBkgnd = (prefs.GetProfileInt("Preferences", "ColorTaskBackground", 0) != 0);
			m_DayView.TaskColorIsBackground = taskColorIsBkgnd;

			bool showParentsAsFolder = (prefs.GetProfileInt("Preferences", "ShowParentsAsFolders", 0) != 0);
			m_DayView.ShowParentsAsFolder = showParentsAsFolder;

			if (prefs.GetProfileInt("Preferences", "SpecifyTreeFont", 0) != 0)
            {
                m_DayView.SetFont(prefs.GetProfileString("Preferences", "TreeFont", FontName),
                                  prefs.GetProfileInt("Preferences", "FontSize", 8));
            }
            else
            {
                m_DayView.SetFont(FontName, 8);
            }

            // Weekends
            WeekendDays dwWeekends = (WeekendDays)prefs.GetProfileInt("Preferences", "Weekends", 0);

            List<System.DayOfWeek> weekendDays = new List<System.DayOfWeek>();

            if ((dwWeekends & WeekendDays.SUNDAY) == WeekendDays.SUNDAY)
                weekendDays.Add(System.DayOfWeek.Sunday);

            if ((dwWeekends & WeekendDays.SATURDAY) == WeekendDays.SATURDAY)
                weekendDays.Add(System.DayOfWeek.Saturday);

            if ((dwWeekends & WeekendDays.MONDAY) == WeekendDays.MONDAY)
                weekendDays.Add(System.DayOfWeek.Monday);

            if ((dwWeekends & WeekendDays.TUESDAY) == WeekendDays.TUESDAY)
                weekendDays.Add(System.DayOfWeek.Tuesday);

            if ((dwWeekends & WeekendDays.WEDNESDAY) == WeekendDays.WEDNESDAY)
                weekendDays.Add(System.DayOfWeek.Wednesday);

            if ((dwWeekends & WeekendDays.THURSDAY) == WeekendDays.THURSDAY)
                weekendDays.Add(System.DayOfWeek.Thursday);

            if ((dwWeekends & WeekendDays.FRIDAY) == WeekendDays.FRIDAY)
                weekendDays.Add(System.DayOfWeek.Friday);

            m_DayView.WeekendDays = weekendDays;

            // Working hours
            double hoursInDay = prefs.GetProfileDouble("Preferences", "HoursInDay", 8.0);

            // assume working days pivot about 1.30pm
            // eg. a standard working day of 8 hours (+1 for lunch)
            // starts at 9am (13.50 - 4.5) and 
            // ends at 6pm (13.30 + 4.5)
            const double MIDDAY = 13.5;
            const double LUNCHSTARTTIME = (MIDDAY - 0.5);
            const double LUNCHENDTIME = (MIDDAY + 0.5);

            double StartOfDay = LUNCHSTARTTIME - (hoursInDay / 2);
            double EndOfDay = LUNCHENDTIME + (hoursInDay / 2);

            m_DayView.WorkingHourStart = (int)StartOfDay;
            m_DayView.WorkingMinuteStart = (int)((StartOfDay - m_DayView.WorkingHourStart) * 60);

            m_DayView.WorkingHourEnd = (int)EndOfDay;
            m_DayView.WorkingMinuteEnd = (int)((EndOfDay - m_DayView.WorkingHourEnd) * 60);
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
	
		// PRIVATE ------------------------------------------------------------------------------

        protected override void Dispose(bool disposing)
        {
            base.Dispose(disposing);

            m_PrefsDlg.Dispose();
        }

		private void InitializeComponent()
		{
            // Day view always comes last
			CreateToolbar();
			CreateWeekLabel();
			CreateMonthYearCombos();
            CreateDayView();
		}

		private void CreateDayView()
		{
			m_DayView = new TDLDayView(m_TaskIcons);

			m_DayView.NewAppointment += new Calendar.NewAppointmentEventHandler(OnDayViewNewAppointment);
			m_DayView.SelectionChanged += new Calendar.AppointmentEventHandler(OnDayViewSelectionChanged);
			m_DayView.AppointmentMove += new Calendar.AppointmentEventHandler(OnDayViewAppointmentChanged);
			m_DayView.WeekChange += new Calendar.WeekChangeEventHandler(OnDayViewWeekChanged);
			m_DayView.StartDate = DateTime.Now;

            m_DayView.SetFont(FontName, 8);

			Controls.Add(m_DayView);
		}

		private void CreateWeekLabel()
		{
			m_WeekLabel = new DayViewWeekLabel();

			m_WeekLabel.Font = new Font(FontName, 14);
			m_WeekLabel.Location = new Point(m_Toolbar.Right, LabelTop);
			m_WeekLabel.Size = new Size(350, 20);
			m_WeekLabel.TextAlign = ContentAlignment.MiddleLeft;

			Controls.Add(m_WeekLabel);
		}

		private void CreateToolbar()
		{
			var assembly = Assembly.GetExecutingAssembly();
			var imageStream = assembly.GetManifestResourceStream("DayViewUIExtension.toolbar_std.bmp");

			m_TBImageList = new ImageList();
			m_TBImageList.ImageSize = new System.Drawing.Size(16, 16);
			m_TBImageList.TransparentColor = Color.Magenta;
			m_TBImageList.Images.AddStrip(new Bitmap(imageStream));

			m_Toolbar = new ToolStrip();
			m_Toolbar.ImageList = m_TBImageList;
			m_Toolbar.Anchor = AnchorStyles.None;
			m_Toolbar.GripStyle = ToolStripGripStyle.Hidden;

			m_TBRenderer = new UIThemeToolbarRenderer();
			m_Toolbar.Renderer = m_TBRenderer;
	
			var btn1 = new ToolStripButton();
			btn1.ImageIndex = 0;
			btn1.Click += new EventHandler(OnGoToToday);
			btn1.ToolTipText = m_Trans.Translate("Go To Today");
			m_Toolbar.Items.Add(btn1);

			var btn2 = new ToolStripButton();
			btn2.ImageIndex = 1;
			btn2.Click += new EventHandler(OnPreferences);
			btn2.ToolTipText = m_Trans.Translate("Preferences");
			m_Toolbar.Items.Add(btn2);

			var btn3 = new ToolStripButton();
			btn3.ImageIndex = 2;
			btn3.Click += new EventHandler(OnHelp);
			btn3.ToolTipText = m_Trans.Translate("Online Help");
			m_Toolbar.Items.Add(btn3);

			m_Toolbar.Size = new Size(72, 24);
			
			Controls.Add(m_Toolbar);
		}

		protected void OnGoToToday(object sender, EventArgs e)
		{
            m_DayView.GoToToday();
		}

		protected void OnPreferences(object sender, EventArgs e)
		{
			m_PrefsDlg.StartPosition = FormStartPosition.CenterParent;

            if (m_PrefsDlg.ShowDialog(Control.FromHandle(m_hwndParent)) == DialogResult.OK)
            {
				UpdateDayViewPreferences();
            }
		}

		private void UpdateDayViewPreferences()
		{
			m_DayView.HideTasksSpanningDays = m_PrefsDlg.HideTasksSpanningDays;
			m_DayView.HideTasksSpanningWeekends = m_PrefsDlg.HideTasksSpanningWeekends;
			m_DayView.HideTasksWithoutTimes = m_PrefsDlg.HideTasksWithoutTimes;

			m_DayView.Invalidate();
		}

		protected void OnHelp(object sender, EventArgs e)
		{
            UIExtension.ParentNotify notify = new UIExtension.ParentNotify(m_hwndParent);

            notify.NotifyDoHelp(m_HelpID);
		}

		private void CreateMonthYearCombos()
		{
			m_MonthCombo = new DayViewMonthComboBox();

			m_MonthCombo.Font = m_ControlsFont;
			m_MonthCombo.Location = new Point(0, ComboTop);
			m_MonthCombo.Size = new Size(100, 16);
			
			m_MonthCombo.SelectedMonth = DateTime.Now.Month;
			m_MonthCombo.SelectedIndexChanged += new EventHandler(OnMonthYearSelChanged);
			
			Controls.Add(m_MonthCombo);

			m_YearCombo = new DayViewYearComboBox();

			m_YearCombo.Font = m_ControlsFont;
			m_YearCombo.Location = new Point(105, ComboTop);
			m_YearCombo.Size = new Size(100, 16);

			m_YearCombo.SelectedYear = DateTime.Now.Year;
			m_YearCombo.SelectedIndexChanged += new EventHandler(OnMonthYearSelChanged);
			
			Controls.Add(m_YearCombo);
		}

		// IUIExtension ------------------------------------------------------------------
		protected override void OnPaint(System.Windows.Forms.PaintEventArgs e)
        {
            Rectangle Border = new Rectangle(ClientRectangle.Location, ClientRectangle.Size);
			Border.Y = ControlTop;
			Border.Height -= ControlTop;

            System.Windows.Forms.ControlPaint.DrawBorder(e.Graphics, Border, Color.DarkGray, System.Windows.Forms.ButtonBorderStyle.Solid);
        }

        public new Boolean Focus()
        {
            if (m_DayView.Focused)
                return false;

            // else
            if (!m_DayView.CanFocus)
                return false;

            return m_DayView.Focus();
        }

        protected override void OnSizeChanged(EventArgs e)
        {
            base.OnSizeChanged(e);

			m_YearCombo.Location = new Point(m_MonthCombo.Right + 10, m_YearCombo.Top);
			m_Toolbar.Location = new Point(m_YearCombo.Right + 10, m_YearCombo.Top - 2);
			m_WeekLabel.Location = new Point(m_Toolbar.Right + 10, m_YearCombo.Top);
			
            Rectangle dayViewRect = new Rectangle(ClientRectangle.Location, ClientRectangle.Size);

			dayViewRect.Y = ControlTop;
			dayViewRect.Height -= ControlTop;
			dayViewRect.Inflate(-1, -1);

			m_DayView.Location = dayViewRect.Location;
			m_DayView.Size = dayViewRect.Size;

            Invalidate(true);
        }

		private void OnDayViewNewAppointment(object sender, Calendar.NewAppointmentEventArgs args)
		{
		}

		private void OnDayViewSelectionChanged(object sender, Calendar.AppointmentEventArgs args)
		{
            UIExtension.ParentNotify notify = new UIExtension.ParentNotify(m_hwndParent);

			switch (m_DayView.Selection)
			{
				case Calendar.SelectionType.DateRange:
					break;

                case Calendar.SelectionType.Appointment:
                    {
                        UInt32 prevSelTaskID = m_SelectedTaskID;

                        if (args.Appointment != null)
                            m_SelectedTaskID = args.Appointment.Id;
                        else
                            m_SelectedTaskID = 0;

                        if (m_SelectedTaskID != prevSelTaskID)
                            notify.NotifySelChange(m_SelectedTaskID);
                    }
					break;
			}
		}

        private void RestoreSelectedItem()
        {
            // Restore the selection if it is present in this week
			m_DayView.SelectTask(m_SelectedTaskID, true);
        }

		private void OnDayViewWeekChanged(object sender, Calendar.WeekChangeEventArgs args)
		{
			m_WeekLabel.StartDate = args.StartDate;

			if (!m_SettingDayViewStartDate)
			{
				m_SettingMonthYear = true;

				m_MonthCombo.SelectedMonth = args.StartDate.Month;
				m_YearCombo.SelectedYear = args.StartDate.Year;

				m_SettingMonthYear = false;
			}

			RestoreSelectedItem();
		}

		private void OnMonthYearSelChanged(object sender, EventArgs args)
		{
			if (!m_SettingMonthYear)
			{
				m_SettingDayViewStartDate = true;

				m_DayView.StartDate = new DateTime(m_YearCombo.SelectedYear, m_MonthCombo.SelectedMonth, 1);

				m_SettingDayViewStartDate = false;
			}
		}

        private void OnDayViewAppointmentChanged(object sender, Calendar.AppointmentEventArgs args)
		{
			Calendar.MoveAppointmentEventArgs move = args as Calendar.MoveAppointmentEventArgs;

			// Ignore moves whilst they are occurring
			if ((move == null) || !move.Finished)
				return;

			CalendarItem item = args.Appointment as CalendarItem;

			if (item == null)
				return;

			UIExtension.ParentNotify notify = new UIExtension.ParentNotify(m_hwndParent);

			switch (move.Mode)
			{
				case Calendar.SelectionTool.Mode.Move:
					if ((item.StartDate - item.OrgStartDate).TotalSeconds != 0.0)
					{
						if (notify.NotifyMod(UIExtension.TaskAttribute.OffsetTask, item.StartDate))
						{
							item.OrgStartDate = item.StartDate;
							item.OrgEndDate = item.EndDate;
						}
						else
						{
							item.StartDate = item.OrgStartDate;
							item.EndDate = item.OrgEndDate;
							m_DayView.Invalidate();
						}
					}
					break;

				case Calendar.SelectionTool.Mode.ResizeLeft:
				case Calendar.SelectionTool.Mode.ResizeTop:
					if ((item.StartDate - item.OrgStartDate).TotalSeconds != 0.0)
					{
						if (notify.NotifyMod(UIExtension.TaskAttribute.StartDate, item.StartDate))
						{
							item.OrgStartDate = item.StartDate;
						}
						else
						{
							item.StartDate = item.OrgStartDate;
							m_DayView.Invalidate();
						}
					}
					break;

				case Calendar.SelectionTool.Mode.ResizeRight:
				case Calendar.SelectionTool.Mode.ResizeBottom:
					if ((item.EndDate - item.OrgEndDate).TotalSeconds != 0.0)
					{
						if (notify.NotifyMod(UIExtension.TaskAttribute.DueDate, item.EndDate))
						{
							item.OrgEndDate = item.EndDate;
						}
						else
						{
							item.EndDate = item.OrgEndDate;
							Invalidate();
						}
					}
					break;
			}
		}

	}


}
