
using System;
using Abstractspoon.Tdl.PluginHelpers;

// PLS DON'T ADD 'USING' STATEMENTS WHILE I AM STILL LEARNING!

namespace DayViewUIExtension
{
	[System.ComponentModel.DesignerCategory("")]
	public class DayViewUIExtensionCore : System.Windows.Forms.Panel, IUIExtension
	{
		private Boolean m_taskColorIsBkgnd = false;
		private IntPtr m_hwndParent = IntPtr.Zero;
		private TDLDayView m_DayView;
		private UInt32 m_SelectedTaskID = 0;
		private Translator m_trans;
		private UIExtension.TaskIcon m_TaskIcons;

		public DayViewUIExtensionCore(IntPtr hwndParent, Translator trans)
		{
			m_hwndParent = hwndParent;
			m_trans = trans;

			m_TaskIcons = new UIExtension.TaskIcon(hwndParent);
			m_ControlsFont = new System.Drawing.Font(FontName, 8);
			m_Items = new System.Collections.Generic.Dictionary<UInt32, CalendarItem>();

			InitializeComponent();
		}
		
		public bool SelectTask(UInt32 dwTaskID)
		{
			// Don't select already selected task
			if (dwTaskID == m_SelectedTaskID)
				return true;

			CalendarItem item;

			if (m_Items.TryGetValue(dwTaskID, out item) && (item.StartDate != DateTime.MinValue))
			{
				m_SelectedTaskID = dwTaskID;
                m_DayView.StartDate = item.StartDate;				
				m_DayView.SelectedAppointment = item;
                m_DayView.ScrollToTop();
                
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
								System.Collections.Generic.HashSet<UIExtension.TaskAttribute> attribs)
		{
			switch (type)
			{
				case UIExtension.UpdateType.Delete:
				case UIExtension.UpdateType.Move:
				case UIExtension.UpdateType.All:
					// Rebuild
					m_Items.Clear();
					break;
					
				case UIExtension.UpdateType.New:
				case UIExtension.UpdateType.Edit:
					// In-place update
					break;
			}

			Task task = tasks.GetFirstTask();

			while (task.IsValid() && ProcessTaskUpdate(task, type, attribs))
				task = task.GetNextTask();

            // Refresh selection
            RestoreSelectedItem();

            m_DayView.SelectionStart = m_DayView.SelectionEnd;
			m_DayView.OnUpdateTasks();
		}

		private bool ProcessTaskUpdate(Task task, 
									   UIExtension.UpdateType type,
                                       System.Collections.Generic.HashSet<UIExtension.TaskAttribute> attribs)
		{
			if (!task.IsValid())
				return false;

			CalendarItem item;
            UInt32 taskID = task.GetID();

			if (m_Items.TryGetValue(taskID, out item))
			{
                if (attribs.Contains(UIExtension.TaskAttribute.Title))
                    item.Title = task.GetTitle();

                if (attribs.Contains(UIExtension.TaskAttribute.DoneDate))
                    item.EndDate = item.OrgEndDate = task.GetDoneDate();

                if (attribs.Contains(UIExtension.TaskAttribute.DueDate))
                {
                    DateTime dueDate = GetEditableDueDate(task.GetDueDate());
                    item.EndDate = item.OrgEndDate = dueDate;
                }

                if (attribs.Contains(UIExtension.TaskAttribute.StartDate))
                    item.StartDate = item.OrgStartDate = task.GetStartDate();

                if (attribs.Contains(UIExtension.TaskAttribute.AllocTo))
                    item.AllocTo = String.Join(", ", task.GetAllocatedTo());

				if (attribs.Contains(UIExtension.TaskAttribute.Icon))
					item.HasIcon = task.HasIcon();

                item.TaskTextColor = task.GetTextDrawingColor();
			}
			else
			{
				item = new CalendarItem();

				item.Title = task.GetTitle();
				item.EndDate = item.OrgEndDate = GetEditableDueDate(task.GetDueDate());
				item.StartDate = item.OrgStartDate = task.GetStartDate();
				item.AllocTo = String.Join(", ", task.GetAllocatedTo());
				item.HasIcon = task.HasIcon();
                item.Id = taskID;
				item.IsParent = task.IsParent();
				item.TaskTextColor = task.GetTextDrawingColor();
				item.DrawBorder = true;
			}

            m_Items[taskID] = item;

			// Process children
			Task subtask = task.GetFirstSubtask();

			while (subtask.IsValid() && ProcessTaskUpdate(subtask, type, attribs))
				subtask = subtask.GetNextTask();

			return true;
		}

        private DateTime GetEditableDueDate(DateTime dueDate)
        {
            // Whole days
            if ((dueDate != DateTime.MinValue) && (dueDate.Date == dueDate))
            {
                // return end-of-day
                return dueDate.AddDays(1).AddSeconds(-1);
            }

            // else
            return dueDate;
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
	   
		public bool DoAppCommand(UIExtension.AppCommand cmd, UInt32 dwExtra)
		{
			switch (cmd)
			{
				case UIExtension.AppCommand.SelectTask:
					return SelectTask(dwExtra);
			}

			return false;
		}

		public bool CanDoAppCommand(UIExtension.AppCommand cmd, UInt32 dwExtra)
		{
			switch (cmd)
			{
				case UIExtension.AppCommand.SelectTask:
					return true;
			}

			// else
			return false;
		}

		public bool GetLabelEditRect(ref Int32 left, ref Int32 top, ref Int32 right, ref Int32 bottom)
		{
            System.Drawing.Rectangle rect = new System.Drawing.Rectangle();

            if (m_DayView.GetAppointmentRect(m_DayView.SelectedAppointment, ref rect))
            {
                rect = m_DayView.RectangleToScreen(rect);

                left = rect.Left;
                top = rect.Top;
                right = rect.Right;
                bottom = (rect.Top + m_DayView.GetFontHeight() + 4); // 4 = border

                return true;
            }

            // else
            return false;
		}

		public UIExtension.HitResult HitTest(Int32 xPos, Int32 yPos)
		{
            System.Drawing.Point pt = m_DayView.PointToClient(new System.Drawing.Point(xPos, yPos));
            Calendar.Appointment appointment = m_DayView.GetAppointmentAt(pt.X, pt.Y);

            if (appointment != null)
            {
                return UIExtension.HitResult.Task;
            }
            else if (m_DayView.GetTrueRectangle().Contains(pt))
            {
                return UIExtension.HitResult.Tasklist;
            }

            // else
			return UIExtension.HitResult.Nowhere;
		}

		public void SetUITheme(UITheme theme)
		{
            m_DayView.SetUITheme(theme);

            this.BackColor = theme.GetAppDrawingColor(UITheme.AppColor.AppBackLight);
		}

		public void SetReadOnly(bool bReadOnly)
		{
		}

		public void SavePreferences(Preferences prefs, String key)
		{
		}

		public void LoadPreferences(Preferences prefs, String key, bool appOnly)
		{
			if (appOnly)
			{
				bool taskColorIsBkgnd = (prefs.GetProfileInt("Preferences", "ColorTaskBackground", 0) != 0);

				if (taskColorIsBkgnd != m_taskColorIsBkgnd)
				{
					m_taskColorIsBkgnd = taskColorIsBkgnd;
					Invalidate();
				}
			}
		}
		
		// PRIVATE ------------------------------------------------------------------------------

		private void InitializeComponent()
		{
			CreateWeekLabel(); // create before day view
			CreateMonthYearCombos();
			CreateDayView();
		}

		private void CreateDayView()
		{
			this.m_DayView = new TDLDayView(m_ControlsFont, m_TaskIcons);

			this.m_DayView.NewAppointment += new Calendar.NewAppointmentEventHandler(this.OnDayViewNewAppointment);
			this.m_DayView.SelectionChanged += new Calendar.AppointmentEventHandler(this.OnDayViewSelectionChanged);
			this.m_DayView.ResolveAppointments += new Calendar.ResolveAppointmentsEventHandler(this.OnDayViewResolveAppointments);
			this.m_DayView.AppointmentMove += new Calendar.AppointmentEventHandler(this.OnDayViewAppointmentChanged);
			this.m_DayView.WeekChange += new Calendar.WeekChangeEventHandler(this.OnDayViewWeekChanged);
			this.m_DayView.StartDate = DateTime.Now;

			this.Controls.Add(m_DayView);
		}

		private void CreateWeekLabel()
		{
			this.m_WeekLabel = new DayViewWeekLabel();

			this.m_WeekLabel.Font = new System.Drawing.Font(FontName, 14);
			this.m_WeekLabel.Location = new System.Drawing.Point(210, LabelTop);
			this.m_WeekLabel.Size = new System.Drawing.Size(350, 20);
			this.m_WeekLabel.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;

			this.Controls.Add(m_WeekLabel);
		}

		private void CreateMonthYearCombos()
		{
			m_MonthCombo = new DayViewMonthComboBox();

			m_MonthCombo.Font = m_ControlsFont;
			m_MonthCombo.Location = new System.Drawing.Point(0, ComboTop);
			m_MonthCombo.Size = new System.Drawing.Size(100, 16);
			
			m_MonthCombo.SelectedMonth = DateTime.Now.Month;
			m_MonthCombo.SelectedIndexChanged += new EventHandler(OnMonthYearSelChanged);
			
			this.Controls.Add(m_MonthCombo);

			m_YearCombo = new DayViewYearComboBox();

			m_YearCombo.Font = m_ControlsFont;
			m_YearCombo.Location = new System.Drawing.Point(105, ComboTop);
			m_YearCombo.Size = new System.Drawing.Size(100, 16);

			m_YearCombo.SelectedYear = DateTime.Now.Year;
			m_YearCombo.SelectedIndexChanged += new EventHandler(OnMonthYearSelChanged);
			
			this.Controls.Add(m_YearCombo);
		}

		// IUIExtension ------------------------------------------------------------------
		protected override void OnPaint(System.Windows.Forms.PaintEventArgs e)
        {
            System.Drawing.Rectangle Border = new System.Drawing.Rectangle(ClientRectangle.Location, ClientRectangle.Size);
			Border.Y = ControlTop;
			Border.Height -= ControlTop;

            System.Windows.Forms.ControlPaint.DrawBorder(e.Graphics, Border, System.Drawing.Color.DarkGray, System.Windows.Forms.ButtonBorderStyle.Solid);
        }

        protected override void OnSizeChanged(EventArgs e)
        {
            base.OnSizeChanged(e);

            System.Drawing.Rectangle DayView = new System.Drawing.Rectangle(ClientRectangle.Location, ClientRectangle.Size);

            DayView.Y = ControlTop;
			DayView.Height -= ControlTop;
            DayView.Inflate(-1, -1);

            m_DayView.Location = DayView.Location;
            m_DayView.Size = DayView.Size;

            Invalidate(true);
        }

		private void OnDayViewNewAppointment(object sender, Calendar.NewAppointmentEventArgs args)
		{
//             Calendar.Appointment m_Appointment = new Calendar.Appointment();
// 
//             m_Appointment.StartDate = args.StartDate;
//             m_Appointment.EndDate = args.EndDate;
//             m_Appointment.Title = args.Title;
//             m_Appointment.Group = "2";
// 
//             m_Appointments.Add(m_Appointment);
		}

		private void OnDayViewSelectionChanged(object sender, Calendar.AppointmentEventArgs args)
		{
            UIExtension.ParentNotify notify = new UIExtension.ParentNotify(m_hwndParent);

			switch (this.m_DayView.Selection)
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
            if (m_SelectedTaskID > 0)
            {
                CalendarItem item;

                if (m_Items.TryGetValue(m_SelectedTaskID, out item))
                {
                    if (IsItemWithinRange(item, m_DayView.StartDate, m_DayView.EndDate))
                    {
                        m_DayView.SelectedAppointment = item;
                        return;
                    }
                }
            }

            m_DayView.SelectedAppointment = null;
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

		private void OnDayViewResolveAppointments(object sender, Calendar.ResolveAppointmentsEventArgs args)
		{
			System.Collections.Generic.List<Calendar.Appointment> appts =
				new System.Collections.Generic.List<Calendar.Appointment>();

			foreach (System.Collections.Generic.KeyValuePair<UInt32, CalendarItem> item in m_Items)
			{
				if (IsItemWithinRange(item.Value, args.StartDate, args.EndDate))
				{
					// Recalculate colours
					if (m_taskColorIsBkgnd)
					{
						item.Value.TextColor = ((item.Value.TaskTextColor.GetBrightness() > 0.5) ? System.Drawing.Color.Black : System.Drawing.Color.White);
						item.Value.BorderColor = ColorUtil.DarkerDrawing(item.Value.TaskTextColor, 0.5f);
						item.Value.BarColor = item.Value.TaskTextColor;
						item.Value.FillColor = item.Value.TaskTextColor;
					}
					else
					{
						item.Value.TextColor = item.Value.TaskTextColor;
						item.Value.BorderColor = item.Value.TaskTextColor;
						item.Value.FillColor = ColorUtil.LighterDrawing(item.Value.TaskTextColor, 0.9f); 
						item.Value.BarColor = item.Value.TaskTextColor;
					}

					appts.Add(item.Value);
				}
			}

			args.Appointments = appts;
		}

		private bool IsItemWithinRange(CalendarItem item, DateTime startDate, DateTime endDate)
		{
// 			return (item.IsSingleDay() && 
//                     (item.StartDate.Date >= startDate) && 
//                     (item.StartDate.Date <= endDate));
			return ((item.StartDate.Date >= startDate) && 
                    (item.EndDate.Date < endDate));
// 			return (((item.StartDate.Date >= startDate) && (item.StartDate.Date < endDate)) ||
// 					((item.EndDate.Date > startDate) && (item.EndDate.Date < endDate)));
		}

		// --------------------------------------------------------------------------------------
		private const int LabelTop = 2;
		private const int ComboTop = 0;
		private const int ControlTop = 25;
		private const string FontName = "Tahoma";

		private System.Collections.Generic.Dictionary<UInt32, CalendarItem> m_Items;
		private System.Windows.Forms.Timer m_redrawTimer;

		private bool m_SettingMonthYear = false, m_SettingDayViewStartDate = false;

		private DayViewWeekLabel m_WeekLabel;
		private DayViewMonthComboBox m_MonthCombo;
		private DayViewYearComboBox m_YearCombo;
		private System.Drawing.Font m_ControlsFont;

	}

	public class CalendarItem : Calendar.Appointment
	{
		public DateTime OrgStartDate { get; set; }
		public DateTime OrgEndDate { get; set; }

		public System.Drawing.Color TaskTextColor { get; set; }

		public String AllocTo { get; set; }
		public Boolean IsParent { get; set; }
		public Boolean HasIcon { get; set; }

        public override DateTime EndDate
        {
            get
            {
                return base.EndDate;
            }
            set
            {
                // Handle 'end of day'
                if ((value != DateTime.MinValue) && (value.Date == value))
                    base.EndDate = value.AddSeconds(-1);
                else
                    base.EndDate = value;
            }
        }

        public override TimeSpan Length
        {
            get 
            {
                // Handle 'end of day'
                if (IsEndOfDay(EndDate))
                    return (EndDate.Date.AddDays(1) - StartDate);

                return base.Length; 
            }
        }

        public static bool IsEndOfDay(DateTime date)
        {
            return (date == date.Date.AddDays(1).AddSeconds(-1));
        }

        public bool IsSingleDay()
        {
            return (StartDate.Date == EndDate.Date);
        }

		public override bool IsLongAppt()
		{
			return (base.IsLongAppt() || (OrgStartDate.Day != OrgEndDate.Day));
		}

    }

}
