using System;
using System.Collections.Generic;
using System.Drawing;
using System.Windows.Forms;
using System.Diagnostics;

using Abstractspoon.Tdl.PluginHelpers;
using Abstractspoon.Tdl.PluginHelpers.ColorUtil;

namespace DayViewUIExtension
{

	public class TDLDayView : Calendar.DayView, ILabelTipHandler
    {
        private UInt32 m_SelectedTaskID = 0;
		private UInt32 m_VisibleSelectedTaskID = 0;

		private int m_UserMinSlotHeight = -1;

        private Boolean m_HideParentTasks = true;
		private Boolean m_DisplayTasksContinuous = true;
		private Boolean m_HideTasksWithoutTimes = true;
        private Boolean m_HideTasksSpanningWeekends = false;
        private Boolean m_HideTasksSpanningDays = false;

        private System.Collections.Generic.Dictionary<UInt32, CalendarItem> m_Items;
        private TDLRenderer m_Renderer;
		private LabelTip m_LabelTip;

		private int LabelTipBorder
		{
			get { return DPIScaling.Scale(4); }
		}

        public Color GridlineColor
        {
            set
            {
                if (value != m_Renderer.GridlineColor)
                {
                    m_Renderer.GridlineColor = value;
                    Invalidate();
                }
            }
        }

		// ----------------------------------------------------------------

		public Boolean ReadOnly { get; set; }

        public TDLDayView(UIExtension.TaskIcon taskIcons, int minSlotHeight)
        {
            hourLabelWidth = DPIScaling.Scale(hourLabelWidth);
            hourLabelIndent = DPIScaling.Scale(hourLabelIndent);
            dayHeadersHeight = DPIScaling.Scale(dayHeadersHeight);
            longAppointmentSpacing = DPIScaling.Scale(longAppointmentSpacing);
			appointmentGripWidth = 1; // to match app styling

            m_Renderer = new TDLRenderer(Handle, taskIcons);
			m_Items = new System.Collections.Generic.Dictionary<UInt32, CalendarItem>();
			m_UserMinSlotHeight = minSlotHeight;
            m_LabelTip = new LabelTip(this);

            InitializeComponent();
            RefreshHScrollSize();
        }

        // ILabelTipHandler implementation
        public Font GetFont()
        {
            return m_Renderer.BaseFont;
        }

        public Control GetOwner()
        {
            return this;
        }

        public UInt32 ToolHitTest(Point ptScreen, ref String tipText, ref Rectangle toolRect, ref bool multiLine)
        {
			if (IsResizingAppointment())
				return 0;

            var pt = PointToClient(ptScreen);
            Calendar.Appointment appointment = GetAppointmentAt(pt.X, pt.Y);

            if (appointment == null)
                return 0;

            var taskItem = appointment as CalendarItem;

            if ((taskItem == null) || !taskItem.TextRect.Contains(pt))
                return 0;

			toolRect = taskItem.TextRect;
			toolRect.Inflate(m_Renderer.TextPadding, m_Renderer.TextPadding);

			if (appointment.IsLongAppt())
			{
				// single line tooltips
				if (m_LabelTip.CalcTipHeight(taskItem.Title, toolRect.Width) <= toolRect.Height)
					return 0;

				multiLine = false; // always
			}
			else
			{
				var availRect = GetTrueRectangle();

				if (taskItem.TextRect.Top < availRect.Top)
				{
					// If the top of the text rectangle is hidden we always 
					// need a label tip so we just clip to the avail space
					toolRect.Intersect(availRect);
				}
				else
				{
					// Determine if text will fit in what's visible of the task
					toolRect.Intersect(availRect);

					if (m_LabelTip.CalcTipHeight(taskItem.Title, toolRect.Width) < toolRect.Height)
						return 0;
				}

				multiLine = true; // always
			}

			tipText = taskItem.Title;

            return taskItem.Id;
        }

        protected override void WndProc(ref Message m)
        {
            if (m_LabelTip!= null)
                m_LabelTip.ProcessMessage(m);

            base.WndProc(ref m);
        }

        protected void InitializeComponent()
        {
            Calendar.DrawTool drawTool = new Calendar.DrawTool();
            drawTool.DayView = this;

            this.ActiveTool = drawTool;
            this.AllowInplaceEditing = true;
            this.AllowNew = false;
            this.AmPmDisplay = true;
            this.Anchor = (System.Windows.Forms.AnchorStyles.Bottom |
                                     System.Windows.Forms.AnchorStyles.Left |
                                     System.Windows.Forms.AnchorStyles.Right);
            this.AppHeightMode = Calendar.DayView.AppHeightDrawMode.TrueHeightAll;
            this.DrawAllAppBorder = false;
            this.Location = new System.Drawing.Point(0, 0);
            this.MinHalfHourApp = false;
            this.Name = "m_dayView";
            this.Renderer = m_Renderer;
            this.SelectionEnd = new System.DateTime(((long)(0)));
            this.SelectionStart = new System.DateTime(((long)(0)));
            this.Size = new System.Drawing.Size(798, 328);
            this.SlotsPerHour = 4;
            this.TabIndex = 0;
            this.Text = "m_dayView";
            this.ReadOnly = false;

			this.ResolveAppointments += new Calendar.ResolveAppointmentsEventHandler(this.OnResolveAppointments);
            this.SelectionChanged += new Calendar.AppointmentEventHandler(this.OnSelectionChanged);
            this.WeekChange += new Calendar.WeekChangeEventHandler(OnWeekChanged);

		}

		public bool ShowLabelTips
		{
			set { m_LabelTip.Active = value; }
			get { return m_LabelTip.Active; }
		}

		public bool IsTaskWithinRange(UInt32 dwTaskID)
        {
			CalendarItem item;

			if (m_Items.TryGetValue(dwTaskID, out item))
                return IsItemWithinRange(item, StartDate, EndDate);

            return false;
        }

        public bool IsTaskDisplayable(UInt32 dwTaskID)
        {
            if (dwTaskID == 0)
                return false;

			CalendarItem item;

			if (!m_Items.TryGetValue(dwTaskID, out item))
                return false;

            return IsItemDisplayable(item);
        }

        public Boolean HideParentTasks
        {
            get { return m_HideParentTasks; }
            set
            {
                if (value != m_HideParentTasks)
                {
                    m_HideParentTasks = value;
                    FixupSelection(false, true);
                }
            }
        }

		public Boolean AutoCalculateDependencyDates
		{
			get; set;
		}

		public Boolean DisplayTasksContinuous
		{
			get { return m_DisplayTasksContinuous; }
			set
			{
				if (value != m_DisplayTasksContinuous)
				{
					m_DisplayTasksContinuous = value;
					FixupSelection(false, true);
				}
			}
		}

		public Boolean HideTasksWithoutTimes
        {
            get { return m_HideTasksWithoutTimes; }
            set
            {
                if (value != m_HideTasksWithoutTimes)
                {
                    m_HideTasksWithoutTimes = value;
                    FixupSelection(false, true);
                }
            }
        }

        public Boolean HideTasksSpanningWeekends
        {
            get { return m_HideTasksSpanningWeekends; }
            set
            {
                if (value != m_HideTasksSpanningWeekends)
                {
                    m_HideTasksSpanningWeekends = value;
                    FixupSelection(false, true);
                }
            }
        }

        public Boolean HideTasksSpanningDays
        {
            get { return m_HideTasksSpanningDays; }
            set
            {
                if (value != m_HideTasksSpanningDays)
                {
                    m_HideTasksSpanningDays = value;
                    FixupSelection(false, true);
                }
            }
        }

        public UInt32 GetSelectedTaskID()
        {
            if (!IsTaskDisplayable(m_SelectedTaskID))
                return 0;
            
            return m_SelectedTaskID;
        }

		public bool GetSelectedTaskDates(ref DateTime from, ref DateTime to)
		{
			UInt32 selTaskID = GetSelectedTaskID();

			if (selTaskID == 0)
				return false;

			CalendarItem item;

			if (!m_Items.TryGetValue(selTaskID, out item))
				return false;

			from = item.StartDate;
			to = item.EndDate;

			return true;
		}

		public void FixupSelection(bool scrollToTask, bool allowNotify)
        {
			// Our base class clears the selected appointment whenever
			// the week changes so we can't rely on 'SelectedAppointmentId'
            UInt32 prevSelTaskID = m_VisibleSelectedTaskID;
            UInt32 selTaskID = GetSelectedTaskID();

			m_VisibleSelectedTaskID = selTaskID;
			
			if (selTaskID > 0)
            {
                CalendarItem item;

                if (m_Items.TryGetValue(selTaskID, out item))
                {
                    if (scrollToTask)
                    {
                        if (item.StartDate != DateTime.MinValue)
                        {
                            if (!IsItemWithinRange(item, StartDate, EndDate))
                                StartDate = item.StartDate;

                            SelectedAppointment = item;
                        }
                    }
                    else if (IsItemWithinRange(item, StartDate, EndDate))
                    {
                        SelectedAppointment = item;
                    }
                }
				else
				{
					SelectedAppointment = null;
				}
			}
			else
			{
				SelectedAppointment = null;
			}
			
			// Notify parent of changes
			if (allowNotify && (GetSelectedTaskID() != prevSelTaskID))
			{
				CalendarItem item = null;
				m_Items.TryGetValue(m_VisibleSelectedTaskID, out item);

				RaiseSelectionChanged(item);
			}
		}

		public bool SelectTask(UInt32 dwTaskID)
		{
            m_SelectedTaskID = dwTaskID;
            FixupSelection(true, false);

			return (GetSelectedTaskID() != 0);
		}

        public void GoToToday()
        {
            StartDate = DateTime.Now;

			// And scroll to first task
			var appointments = GetMatchingAppointments(StartDate, EndDate, true);

			if (appointments != null)
			{
				foreach (var appt in appointments)
				{
					if (EnsureVisible(appt, false))
						break;
				}
			}
			else
			{
				ScrollToTop();
			}

			Invalidate();
        }

		public UIExtension.HitResult HitTest(Int32 xScreen, Int32 yScreen)
		{
			System.Drawing.Point pt = PointToClient(new System.Drawing.Point(xScreen, yScreen));
			Calendar.Appointment appointment = GetAppointmentAt(pt.X, pt.Y);

			if (appointment != null)
			{
				return UIExtension.HitResult.Task;
			}
			else if (GetTrueRectangle().Contains(pt))
			{
				return UIExtension.HitResult.Tasklist;
			}

			// else
			return UIExtension.HitResult.Nowhere;
		}

		public UInt32 HitTestTask(Int32 xScreen, Int32 yScreen)
		{
			System.Drawing.Point pt = PointToClient(new System.Drawing.Point(xScreen, yScreen));
			Calendar.Appointment appointment = GetAppointmentAt(pt.X, pt.Y);

			if (appointment != null)
				return appointment.Id;

			return 0;
		}

		public bool GetItemLabelRect(Calendar.Appointment appointment, ref Rectangle rect)
		{
			if (GetAppointmentRect(appointment, ref rect))
			{
				CalendarItem item = (appointment as CalendarItem);
				bool hasIcon = m_Renderer.TaskHasIcon(item);

				if (appointment.IsLongAppt())
				{
					// Gripper
					if (appointment.StartDate >= StartDate)
						rect.X += 8;
					else
						rect.X -= 3;

					if (hasIcon)
						rect.X += 16;

					rect.X += 1;
					rect.Height += 1;
				}
				else
				{
					if (hasIcon)
					{
						rect.X += 18;
					}
					else
					{
						// Gripper
						rect.X += 8;
					}

					rect.X += 1;
					rect.Y += 1;

					rect.Height = (GetFontHeight() + 4); // 4 = border
				}

				return true;
			}

			return false;
		}

		public bool GetSelectedItemLabelRect(ref Rectangle rect)
		{
			return GetItemLabelRect(SelectedAppointment, ref rect);
		}

		public bool IsItemDisplayable(CalendarItem item)
		{
			// Provide infinite range because this check
			// is uninterested in the current week, it's more
			// of a 'static' kind of check
			return IsItemWithinRange(item, DateTime.MinValue, DateTime.MaxValue);
		}

		private bool IsItemWithinRange(CalendarItem item, DateTime startDate, DateTime endDate)
		{
			// Always show a task if it is currently being dragged
			if (IsResizingAppointment() && (item == SelectedAppointment))
				return true;

            if (HideParentTasks && item.IsParent)
                return false;

			if (!item.HasValidDates())
				return false;

			// Task must at least intersect the range
			if ((item.StartDate >= endDate) || (item.EndDate <= startDate))
			{
				return false;
			}

			if (!DisplayTasksContinuous)
			{
				if ((item.StartDate < startDate) && (item.EndDate > endDate))
					return false;
			}

			if (HideTasksSpanningWeekends)
			{
				if (DateUtil.WeekOfYear(item.StartDate) != DateUtil.WeekOfYear(item.EndDate))
					return false;
			}

            if (HideTasksSpanningDays)
            {
                if (item.IsLongAppt())
                    return false;
            }

			if (HideTasksWithoutTimes)
			{
				if (CalendarItem.IsStartOfDay(item.StartDate) && CalendarItem.IsEndOfDay(item.EndDate))
					return false;
			}

            return true;
		}

		public void UpdateTasks(TaskList tasks,	UIExtension.UpdateType type)
		{
            UInt32 selTaskId = SelectedAppointmentId;

			switch (type)
			{
				case UIExtension.UpdateType.Delete:
				case UIExtension.UpdateType.All:
					// Rebuild
					m_Items.Clear();
                    SelectedAppointment = null;
					break;

				case UIExtension.UpdateType.New:
				case UIExtension.UpdateType.Edit:
					// In-place update
					break;
			}

			Task task = tasks.GetFirstTask();

			while (task.IsValid() && ProcessTaskUpdate(task, type))
				task = task.GetNextTask();

			SelectionStart = SelectionEnd;

            AdjustScrollbar();
            Invalidate();
        }

		private bool ProcessTaskUpdate(Task task, UIExtension.UpdateType type)
		{
			if (!task.IsValid())
				return false;

			CalendarItem item;
			UInt32 taskID = task.GetID();

			if (m_Items.TryGetValue(taskID, out item))
			{
				item.UpdateTaskAttributes(task, type, false);
			}
			else
			{
				item = new CalendarItem();
				item.UpdateTaskAttributes(task, type, true);
			}

			m_Items[taskID] = item;

			// Process children
			Task subtask = task.GetFirstSubtask();

			while (subtask.IsValid() && ProcessTaskUpdate(subtask, type))
				subtask = subtask.GetNextTask();

			return true;
		}

		public Boolean StrikeThruDoneTasks
		{
			get { return m_Renderer.StrikeThruDoneTasks; }
			set
			{
                if (m_Renderer.StrikeThruDoneTasks != value)
				{
                    m_Renderer.StrikeThruDoneTasks = value;
					Invalidate();
				}
			}
		}

        public Boolean TaskColorIsBackground
        {
            get { return m_Renderer.TaskColorIsBackground; }
            set
            {
                if (m_Renderer.TaskColorIsBackground != value)
                {
                    m_Renderer.TaskColorIsBackground = value;
                    Invalidate();
                }
            }
        }

		public Boolean ShowParentsAsFolder
		{
			get { return m_Renderer.ShowParentsAsFolder; }
			set
			{
				if (m_Renderer.ShowParentsAsFolder != value)
				{
					m_Renderer.ShowParentsAsFolder = value;
					Invalidate();
				}
			}
		}

        public void SetFont(String fontName, int fontSize)
        {
            m_Renderer.SetFont(fontName, fontSize);

            LongAppointmentHeight = Math.Max(m_Renderer.BaseFont.Height + 4, 17);
        }
        
        public int GetFontHeight()
        {
            return m_Renderer.GetFontHeight();
        }

   		public void SetUITheme(UITheme theme)
		{
            m_Renderer.Theme = theme;
            Invalidate(true);
		}

		public override DateTime GetDateAt(int x, bool longAppt)
		{
			DateTime date = base.GetDateAt(x, longAppt);

			if (longAppt && (date >= EndDate))
			{
				date = EndDate.AddSeconds(-1);
			}

			return date;
		}

        public override TimeSpan GetTimeAt(int y)
        {
            TimeSpan time = base.GetTimeAt(y);
            
            if (time == new TimeSpan(1, 0, 0, 0))
                time = time.Subtract(new TimeSpan(0, 0, 1));

            return time;
        }

		protected override void DrawDay(PaintEventArgs e, Rectangle rect, DateTime time)
		{
			e.Graphics.FillRectangle(SystemBrushes.Window, rect);

			if (UIExtension.IsHighContrastActive())
			{
				// Draw selection first because it's opaque
				DrawDaySelection(e, rect, time);

				DrawDaySlotSeparators(e, rect, time);
				DrawNonWorkHours(e, rect, time);
				DrawToday(e, rect, time);
				DrawDayAppointments(e, rect, time);
			}
			else
			{
				DrawDaySlotSeparators(e, rect, time);
				DrawNonWorkHours(e, rect, time);
				DrawToday(e, rect, time);
				DrawDayAppointments(e, rect, time);

				// Draw selection last because it's translucent
				DrawDaySelection(e, rect, time);
			}

			DrawDayGripper(e, rect);
		}

		private bool WantDrawToday(DateTime time)
		{
			if (!m_Renderer.Theme.HasAppColor(UITheme.AppColor.Today))
				return false;
			
			return (time.Date == DateTime.Now.Date);
		}

		protected void DrawToday(PaintEventArgs e, Rectangle rect, DateTime time)
		{
			if (!WantDrawToday(time))
				return;

			using (var brush = new SolidBrush(m_Renderer.Theme.GetAppDrawingColor(UITheme.AppColor.Today, 128)))
				e.Graphics.FillRectangle(brush, rect);
		}

		protected void DrawNonWorkHours(PaintEventArgs e, Rectangle rect, DateTime time)
		{
			if (m_Renderer.Theme.HasAppColor(UITheme.AppColor.Weekends) && WeekendDays.Contains(time.DayOfWeek))
			{
				var weekendColor = m_Renderer.Theme.GetAppDrawingColor(UITheme.AppColor.Weekends, 128);

				// If this is also 'today' then convert to gray so it doesn't 
				// impose too much when the today colour is laid on top
				if (WantDrawToday(time))
					weekendColor = DrawingColor.ToGray(weekendColor);

				using (var brush = new SolidBrush(weekendColor))
					e.Graphics.FillRectangle(brush, rect);
			}
			else if (m_Renderer.Theme.HasAppColor(UITheme.AppColor.NonWorkingHours))
			{
				var nonWorkColor = m_Renderer.Theme.GetAppDrawingColor(UITheme.AppColor.NonWorkingHours, 128);

				// If this is also 'today' then convert to gray so it doesn't 
				// impose too much when the today colour is laid on top
				if (WantDrawToday(time))
					nonWorkColor = DrawingColor.ToGray(nonWorkColor);

				using (SolidBrush brush = new SolidBrush(nonWorkColor))
				{
					DrawNonWorkHours(e, new HourMin(0, 0), WorkStart, rect, brush);
					DrawNonWorkHours(e, LunchStart, LunchEnd, rect, brush);
					DrawNonWorkHours(e, WorkEnd, new HourMin(24, 0), rect, brush);
				}
			}
		}
		
		protected void DrawNonWorkHours(PaintEventArgs e, HourMin start, HourMin end, Rectangle rect, Brush brush)
		{
			if (start < end)
			{
				Rectangle hoursRect = GetHourRangeRectangle(start, end, rect);

				if (hoursRect.Y < this.HeaderHeight)
				{
					hoursRect.Height -= this.HeaderHeight - hoursRect.Y;
					hoursRect.Y = this.HeaderHeight;
				}

				e.Graphics.FillRectangle(brush, hoursRect);
			}
		}

		protected override void DrawAppointment(Graphics g, Rectangle rect, Calendar.Appointment appointment, bool isSelected, Rectangle gripRect)
		{
			// Our custom gripper bar
			gripRect = rect;
			gripRect.Inflate(-2, -2);
			gripRect.Width = 5;

            // If the start date precedes the start of the week then extend the
            // draw rect to the left so the edge is clipped and likewise for the right.
            CalendarItem taskItem = (appointment as CalendarItem);
            bool longAppt = taskItem.IsLongAppt();

            if (longAppt)
            {
                if (appointment.StartDate < StartDate)
                {
                    rect.X -= 4;
                    rect.Width += 4;

                    gripRect.X = rect.X;
                    gripRect.Width = 0;
                }
                else if (appointment.StartDate > StartDate)
                {
                    rect.X++;
                    rect.Width--;

                    gripRect.X++;
                }

                if (appointment.EndDate >= EndDate)
                {
                    rect.Width += 5;
                }
            }
            else // day appointment
            {
                if (taskItem.StartDate.TimeOfDay.TotalHours == 0.0)
                {
                    rect.Y++;
                    rect.Height--;
                }

                rect.Width -= 2;
            }
			
			m_Renderer.DrawAppointment(g, rect, appointment, isSelected, gripRect);
		}

		private void OnResolveAppointments(object sender, Calendar.ResolveAppointmentsEventArgs args)
		{
			args.Appointments = GetMatchingAppointments(args.StartDate, args.EndDate);
		}

		private List<Calendar.Appointment> GetMatchingAppointments(DateTime start, DateTime end, bool sorted = false)
		{
			var appts = new System.Collections.Generic.List<Calendar.Appointment>();

			foreach (System.Collections.Generic.KeyValuePair<UInt32, CalendarItem> item in m_Items)
			{
				if (IsItemWithinRange(item.Value, start, end))
					appts.Add(item.Value);
			}

			if (sorted)
				appts.Sort((a, b) => (int)(a.StartDate.Ticks - b.StartDate.Ticks));

			return appts;
		}

		private void OnSelectionChanged(object sender, Calendar.AppointmentEventArgs args)
        {
            if (args.Appointment != null)
			{
				m_SelectedTaskID = args.Appointment.Id;

				// User made this selection so the task must be visible
				m_VisibleSelectedTaskID = m_SelectedTaskID;
			}
		}

        private void OnWeekChanged(object sender, Calendar.WeekChangeEventArgs args)
        {
            FixupSelection(false, true);
        }

        protected override void OnGotFocus(EventArgs e)
        {
            base.OnGotFocus(e);

            Invalidate();
            Update();
        }

        protected override void OnLostFocus(EventArgs e)
        {
            base.OnLostFocus(e);

            Invalidate();
            Update();
        }

		protected override void OnMouseDown(MouseEventArgs e)
		{
			// let the base class initiate resizing if it wants
			base.OnMouseDown(e);

			// Cancel resizing if our task is not editable
			if (IsResizingAppointment())
			{
				var taskItem = SelectedAppointment as CalendarItem;
				var mode = GetMode(taskItem, e.Location);

				if (!CanResizeTask(taskItem, mode))
				{
					CancelAppointmentResizing();
				}
			}
		}

		private Calendar.SelectionTool.Mode GetMode(Calendar.Appointment appointment, Point mousePos)
		{
			if (ReadOnly || (appointment == null))
				return Calendar.SelectionTool.Mode.None;

			var selTool = (ActiveTool as Calendar.SelectionTool);

			if (selTool == null)
			{
				selTool = new Calendar.SelectionTool();
				selTool.DayView = this;
			}

			return selTool.GetMode(mousePos, appointment);
		}
		
		private bool CanResizeTask(CalendarItem taskItem, Calendar.SelectionTool.Mode mode)
		{
			switch (mode)
			{
				// Disable start date editing for tasks with dependencies that are auto-calculated
				case Calendar.SelectionTool.Mode.ResizeTop:
				case Calendar.SelectionTool.Mode.ResizeLeft:
				case Calendar.SelectionTool.Mode.Move:
					return (!taskItem.HasDependencies || !AutoCalculateDependencyDates);

				case Calendar.SelectionTool.Mode.ResizeBottom:
				case Calendar.SelectionTool.Mode.ResizeRight:
					return true;
			}

			// catch all
			return false;
		}

		protected override void OnMouseMove(MouseEventArgs e)
        {
			// default handling
			base.OnMouseMove(e);

			Cursor = GetCursor(e);
		}

		private Cursor GetCursor(MouseEventArgs e)
        {
			if (IsResizingAppointment())
				return Cursor;

			// Note: base class only shows 'resize' cursors for the currently
			// selected item but we want them for all tasks
			var taskItem = (GetAppointmentAt(e.Location.X, e.Location.Y) as CalendarItem);

			if ((taskItem != null) && !ReadOnly)
			{
				if (taskItem.IsLocked)
					return UIExtension.AppCursor(UIExtension.AppCursorType.LockedTask);

				if (taskItem.IconRect.Contains(e.Location))
					return UIExtension.HandCursor();

				var mode = GetMode(taskItem, e.Location);

				if (!CanResizeTask(taskItem, mode))
					return UIExtension.AppCursor(UIExtension.AppCursorType.NoDrag);

				switch (mode)
				{
					case Calendar.SelectionTool.Mode.ResizeBottom:
					case Calendar.SelectionTool.Mode.ResizeTop:
						return Cursors.SizeNS;

					case Calendar.SelectionTool.Mode.ResizeLeft:
					case Calendar.SelectionTool.Mode.ResizeRight:
						return Cursors.SizeWE;

					case Calendar.SelectionTool.Mode.Move:
						// default cursor below
						break;
				}
			}

			// All else
			return Cursors.Default;
		}

		public new int SlotsPerHour
		{
			get
			{
				return base.SlotsPerHour;
			}
			set
			{
				// Must be sensible values
				if (IsValidSlotsPerHour(value))
				{
					// If we're increasing the number of slots we force a 
					// recalculation of the min slot height else we just validate it
					if (value > base.SlotsPerHour)
					{
						minSlotHeight = m_UserMinSlotHeight;
					}
					base.SlotsPerHour = value;

					ValidateMinSlotHeight();
					AdjustScrollbar();
				}
			}
		}

		public int MinSlotHeight
		{
			get { return m_UserMinSlotHeight; }
			set
			{
				if (value != m_UserMinSlotHeight)
				{
					m_UserMinSlotHeight = value;
					minSlotHeight = m_UserMinSlotHeight;

					ValidateMinSlotHeight();
					AdjustScrollbar();
					Invalidate();
				}
			}

		}

		protected void ValidateMinSlotHeight()
		{
			using (var g = Graphics.FromHwnd(this.Handle))
			{
				int minHourHeight = (int)g.MeasureString("0", Renderer.HourFont).Height;

				if ((minSlotHeight * SlotsPerHour) < minHourHeight)
					minSlotHeight = ((minHourHeight / SlotsPerHour) + 1);

				if (SlotHeight < minSlotHeight)
					SlotHeight = minSlotHeight;
			}
		}
	}
}
