using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Drawing.Text;
using System.Windows.Forms;
using System.Windows.Forms.VisualStyles;

using Abstractspoon.Tdl.PluginHelpers;

namespace DayViewUIExtension
{

	public class TDLDayView : Calendar.DayView
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

		// ----------------------------------------------------------------
    
        public Boolean ReadOnly { get; set; }

        public TDLDayView(UIExtension.TaskIcon taskIcons, int minSlotHeight)
        {
            hourLabelWidth = DPIScaling.Scale(hourLabelWidth);
            hourLabelIndent = DPIScaling.Scale(hourLabelIndent);
            dayHeadersHeight = DPIScaling.Scale(dayHeadersHeight);
            appointmentGripWidth = DPIScaling.Scale(appointmentGripWidth);
            headerBorder = DPIScaling.Scale(headerBorder);
            longAppointmentSpacing = DPIScaling.Scale(longAppointmentSpacing);

            m_Renderer = new TDLRenderer(Handle, taskIcons);
			m_Items = new System.Collections.Generic.Dictionary<UInt32, CalendarItem>();
			m_UserMinSlotHeight = minSlotHeight;

			InitializeComponent();
            RefreshHScrollSize();
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
            this.WeekView = true;
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

		public bool GetSelectedItemLabelRect(ref Rectangle rect)
		{
			if (GetAppointmentRect(SelectedAppointment, ref rect))
			{
				CalendarItem selItem = (SelectedAppointment as CalendarItem);

				bool hasIcon = m_Renderer.TaskHasIcon(selItem);

				if (SelectedAppointment.IsLongAppt())
				{
					// Gripper
					if (SelectedAppointment.StartDate >= StartDate)
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

			// else
			return false;
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
			if (IsResizing() && (item == SelectedAppointment))
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

		protected override void DrawAppointment(Graphics g, Rectangle rect, Calendar.Appointment appointment, bool isSelected, Rectangle gripRect)
		{
			// Our custom gripper bar
			gripRect = rect;
			gripRect.Inflate(-2, -2);
			gripRect.Width = 5;

			// If the start date precedes the start of the week then extend the
			// draw rect to the left so the edge is clipped and likewise for the right.
			if (appointment.StartDate < StartDate)
			{
                rect.X -= 4;
                rect.Width += 4;

				gripRect.X = rect.X;
				gripRect.Width = 0;
			}

			if (appointment.EndDate >= EndDate)
			{
				rect.Width += 5;
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
			// eat this if our task is not editable
			Calendar.Appointment appointment = GetAppointmentAt(e.Location.X, e.Location.Y);

			if ((appointment != null) && !IsAppointmentEditable(appointment, e.Location))
			{
				SelectedAppointment = appointment;
				return;
			}

			// else
			base.OnMouseDown(e);
		}

		private Calendar.SelectionTool.Mode GetMode(Calendar.Appointment appointment, Point mousePos)
		{
			if (ReadOnly || (appointment == null))
				return Calendar.SelectionTool.Mode.None;

            var selTool = new Calendar.SelectionTool();
			selTool.DayView = this;

			return selTool.GetMode(mousePos, appointment);
		}
		
		private bool IsAppointmentEditable(Calendar.Appointment appointment, Point mousePos)
		{
			var taskItem = (appointment as CalendarItem);

			if ((taskItem == null) || taskItem.IsLocked)
				return false;

			// Disable start date editing for tasks with dependencies
			switch (GetMode(appointment, mousePos))
			{
				case Calendar.SelectionTool.Mode.ResizeLeft:
				case Calendar.SelectionTool.Mode.ResizeTop:
				case Calendar.SelectionTool.Mode.Move:
					return !taskItem.HasDependencies;
			}

			return true;
		}

		protected override void OnMouseMove(MouseEventArgs e)
        {
			if (!IsResizing())
			{
				// We do all cursor handling so that we can show 
				// resize cursors even when item is not selected
				Calendar.Appointment appointment = GetAppointmentAt(e.Location.X, e.Location.Y);

				if (appointment != null)
				{
					Cursor cursor = null;

					if (IsAppointmentEditable(appointment, e.Location))
					{
						if ((appointment as CalendarItem).IconRect.Contains(e.Location))
						{
							cursor = UIExtension.HandCursor();
						}
						else
						{
							switch (GetMode(appointment, e.Location))
							{
								case Calendar.SelectionTool.Mode.ResizeBottom:
								case Calendar.SelectionTool.Mode.ResizeTop:
									cursor = Cursors.SizeNS;
									break;

								case Calendar.SelectionTool.Mode.ResizeLeft:
								case Calendar.SelectionTool.Mode.ResizeRight:
									cursor = Cursors.SizeWE;
									break;

								case Calendar.SelectionTool.Mode.Move:
								default:
									cursor = Cursors.Default;
									break;
							}
						}
					}
					else
					{
						cursor = UIExtension.AppCursor(UIExtension.AppCursorType.LockedTask);
					}

					if (cursor != null)
					{
						Cursor = cursor;
						return;
					}
				}
			}

			// default handling
			base.OnMouseMove(e);
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
