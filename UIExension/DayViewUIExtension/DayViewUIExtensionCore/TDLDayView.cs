using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Drawing.Text;
using System.Windows.Forms.VisualStyles;

using Abstractspoon.Tdl.PluginHelpers;

namespace DayViewUIExtension
{
	public class CalendarItem : Calendar.Appointment
	{
		static DateTime NullDate = new DateTime();

		// --------------------

		public DateTime OrgStartDate { get; set; }
		public DateTime OrgEndDate { get; set; }

		private System.Drawing.Color taskTextColor;

		public Boolean HasTaskTextColor
		{
			get { return !taskTextColor.IsEmpty; }
		}

		public System.Drawing.Color TaskTextColor
		{
			get
			{
				if (taskTextColor.IsEmpty)
					return base.TextColor;

				return taskTextColor;
			}
			set { taskTextColor = value; }
		}

		public String AllocTo { get; set; }
		public Boolean IsParent { get; set; }
		public Boolean HasIcon { get; set; }
		public Boolean IsDone { get; set; }

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
			return (base.IsLongAppt() || (OrgStartDate.Day != OrgEndDate.Day) ||
					((OrgStartDate.TimeOfDay == TimeSpan.Zero) && IsEndOfDay(OrgEndDate)));
		}

		public bool HasValidDates()
		{
			return ((StartDate != NullDate) &&
					(EndDate != NullDate) &&
					(EndDate > StartDate));
		}
	}

	//////////////////////////////////////////////////////////////////////////////////
	
    public class TDLDayView : Calendar.DayView
    {
        private TDLRenderer m_Renderer;

		private System.Collections.Generic.Dictionary<UInt32, CalendarItem> m_Items;
		private System.Windows.Forms.Timer m_RedrawTimer;

		// ----------------------------------------------------------------

        public TDLDayView(UIExtension.TaskIcon taskIcons)
        {
            m_Renderer = new TDLRenderer(Handle, taskIcons);
			m_Items = new System.Collections.Generic.Dictionary<UInt32, CalendarItem>();
			m_RedrawTimer = new System.Windows.Forms.Timer();

            InitializeComponent();
        }

        protected void InitializeComponent()
        {
            Calendar.DrawTool drawTool = new Calendar.DrawTool();
            drawTool.DayView = this;

            this.ActiveTool = drawTool;
            this.AllowInplaceEditing = true;
            this.AllowNew = true;
            this.AmPmDisplay = true;
            this.Anchor = (System.Windows.Forms.AnchorStyles.Bottom |
                                     System.Windows.Forms.AnchorStyles.Left |
                                     System.Windows.Forms.AnchorStyles.Right);
            this.AppHeightMode = Calendar.DayView.AppHeightDrawMode.TrueHeightAll;
            this.DaysToShow = 7;
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
            this.WorkingHourEnd = 19;
            this.WorkingHourStart = 9;
            this.WorkingMinuteEnd = 0;
            this.WorkingMinuteStart = 0;

			this.ResolveAppointments += new Calendar.ResolveAppointmentsEventHandler(this.OnDayViewResolveAppointments);
        }

		public bool SelectTask(UInt32 dwTaskID, bool ifInRange)
		{
			if ((SelectedAppointment != null) && (SelectedAppointment.Id == dwTaskID))
				return true;

			CalendarItem item;

			if (m_Items.TryGetValue(dwTaskID, out item))
			{
                if (IsItemWithinRange(item, StartDate, EndDate))
                {
                    SelectedAppointment = item;
                    return true;
                }
				else if (item.StartDate != DateTime.MinValue)
				{
					StartDate = item.StartDate;
					SelectedAppointment = item;
					ScrollToTop();

					return true;
				}
			}

			// all else 
			SelectedAppointment = null;
			return false;
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

		private bool IsItemWithinRange(CalendarItem item, DateTime startDate, DateTime endDate)
		{
			if (!item.HasValidDates())
				return false;

			// Start or end date must be 'visible'
			return (((item.StartDate.Date >= startDate) && (item.StartDate.Date < endDate)) ||
					((item.EndDate.Date > startDate) && (item.EndDate.Date < endDate)));
		}

		public void UpdateTasks(TaskList tasks,
						UIExtension.UpdateType type,
						System.Collections.Generic.HashSet<UIExtension.TaskAttribute> attribs)
		{
			switch (type)
			{
				case UIExtension.UpdateType.Delete:
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

			SelectionStart = SelectionEnd;
			StartUpdateTimer();
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
				{
					item.EndDate = item.OrgEndDate = task.GetDoneDate();
					item.IsDone = (task.IsDone() || task.IsGoodAsDone());
				}

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
				item.IsDone = (task.IsDone() || task.IsGoodAsDone());
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

            LongAppointmentHeight = Math.Max(m_Renderer.BaseFont.Height + 4, 18);
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

		protected void StartUpdateTimer()
		{
			m_RedrawTimer.Tick += OnUpdateTimer;
			m_RedrawTimer.Interval = 10;
			m_RedrawTimer.Start();
		}

		protected void OnUpdateTimer(object sender, EventArgs e)
		{
			m_RedrawTimer.Stop();

			AdjustScrollbar();
			Invalidate();
			Update();
		}

		protected override void DrawAppointment(Graphics g, Rectangle rect, Calendar.Appointment appointment, bool isSelected, Rectangle gripRect)
		{
			// Our custom gripper bar
			gripRect = rect;
			gripRect.Inflate(-2, -2);
			gripRect.Width = 5;
			gripRect.Height--;

			// If the start date precedes the start of the week then extend the
			// draw rect to the left so the edge is clipped and likewise for the right.
			Rectangle clipRect = rect;

			if (appointment.StartDate < StartDate)
			{
				// Enough to hide the grip Rect
				int offset = (gripRect.Right - rect.Left);

                rect.X -= 4;// offset;
                rect.Width += 4;// offset;

				gripRect.Width = 0;
			}
			else if (appointment.EndDate >= EndDate)
			{
				rect.Width += 5;
			}
			
			m_Renderer.DrawAppointment(g, rect, appointment, isSelected, gripRect);
		}

		private void OnDayViewResolveAppointments(object sender, Calendar.ResolveAppointmentsEventArgs args)
		{
			System.Collections.Generic.List<Calendar.Appointment> appts =
				new System.Collections.Generic.List<Calendar.Appointment>();

			foreach (System.Collections.Generic.KeyValuePair<UInt32, CalendarItem> item in m_Items)
			{
				if (IsItemWithinRange(item.Value, args.StartDate, args.EndDate))
					appts.Add(item.Value);
			}

			args.Appointments = appts;
		}
	}
}
