using System;
using System.Collections.Generic;
using System.Windows.Forms;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Drawing.Text;
using System.Drawing.Imaging;
using System.Globalization;
using System.Linq;
using System.Threading;
using System.Diagnostics;

namespace Calendar
{
    public static class FirstDayOfWeekUtility
    {
        /// <summary>
        /// Returns the first day of the week that the specified
        /// date is in using the current culture. 
        /// </summary>
        public static DateTime GetFirstDayOfWeek(DateTime dayInWeek)
        {
            CultureInfo defaultCultureInfo = CultureInfo.CurrentCulture;
            return GetFirstDayOfWeek(dayInWeek, defaultCultureInfo);
        }

        /// <summary>
        /// Returns the first day of the week that the specified date 
        /// is in. 
        /// </summary>
        public static DateTime GetFirstDayOfWeek(DateTime dayInWeek, CultureInfo cultureInfo)
        {
            DayOfWeek firstDay = cultureInfo.DateTimeFormat.FirstDayOfWeek;
            DateTime firstDayInWeek = dayInWeek.Date;
            while (firstDayInWeek.DayOfWeek != firstDay)
                firstDayInWeek = firstDayInWeek.AddDays(-1);

            return firstDayInWeek;
        }
    }

	public delegate void DayWidthEventHandler(object sender, Graphics g, int colWidth);

	[System.ComponentModel.DesignerCategory("")]
	public class DayView : UserControl
    {
        #region Variables

        private TextBox editbox;
        private VScrollBar vscroll;
        private HScrollBar hscroll;
        private ToolTip tooltip;
        private DrawTool drawTool;
        private SelectionTool selectionTool;
		private bool legacyScrollbars = false;

        #endregion

        #region Constants

        protected int minHourLabelWidth = 50;
        protected int hourLabelIndent = 2;
        protected int minDayHeaderHeight = 19;
        protected int appointmentGripWidth = 5;
        protected int dayGripWidth = 5;
        protected int allDayEventsHeaderHeight = 0;
        protected int longAppointmentSpacing = 2;
        protected int appointmentSpacing = 1;
        protected int groupSpacing = 1;
        protected int daySpacing = 1;
		protected int rightClickSelectionMinutes = 60;
		protected int minSlotHeight = 5;

		public event DayWidthEventHandler NotifyDayWidth;

        public enum AppHeightDrawMode
        {
            TrueHeightAll = 0, // all appointments have height proportional to true length
            FullHalfHourBlocksAll = 1, // all appts drawn in half hour blocks
            EndHalfHourBlocksAll = 2, // all appts boxes start at actual time, end in half hour blocks
            FullHalfHourBlocksShort = 3, // short (< 30 mins) appts drawn in half hour blocks
            EndHalfHourBlocksShort = 4, // short appts boxes start at actual time, end in half hour blocks
        }

        #endregion

        #region c.tor

        public DayView()
        {
			slotHeight = minSlotHeight;

			StartDate = DateTime.Now;

            SetStyle(ControlStyles.OptimizedDoubleBuffer, true);
			SetStyle(ControlStyles.UserPaint, true);
			SetStyle(ControlStyles.AllPaintingInWmPaint, true);
            SetStyle(ControlStyles.Selectable, true);

            vscroll = new VScrollBar();
            vscroll.Visible = allowScroll;
            vscroll.Scroll += new ScrollEventHandler(OnVScroll);
			vscroll.Value = 0;

            Controls.Add(vscroll);

            hscroll = new HScrollBar();
            hscroll.Visible = true;
            hscroll.Scroll += new ScrollEventHandler(OnHScroll);
            hscroll.Minimum = -10000;
            hscroll.Maximum = 10000;
            hscroll.Value = 0;

            Controls.Add(hscroll);

            editbox = new TextBox();
            editbox.Multiline = true;
            editbox.Visible = false;
            editbox.ScrollBars = ScrollBars.Vertical;
            editbox.BorderStyle = BorderStyle.None;
            editbox.KeyUp += new KeyEventHandler(editbox_KeyUp);
            editbox.Margin = Padding.Empty;

            Controls.Add(editbox);

            tooltip = new ToolTip() { Active = LegacyScrollbars };

            drawTool = NewDrawTool();
            drawTool.DayView = this;

            selectionTool = NewSelectionTool();
            selectionTool.DayView = this;
            selectionTool.Complete += new EventHandler(selectionTool_Complete);

            activeTool = drawTool;

            Renderer = new Office12Renderer();
        }

		#endregion

		#region Properties

		virtual protected SelectionTool NewSelectionTool()
		{
			return new SelectionTool();
		}

		virtual protected DrawTool NewDrawTool()
		{
			return new DrawTool();
		}

		public Boolean IsResizingAppointment()
		{
			return ((activeTool == selectionTool) && selectionTool.IsEditing);
		}

		public bool CancelAppointmentResizing()
		{
			if (IsResizingAppointment())
			{
				selectionTool.Reset();
				activeTool = null;

				return true;
			}

			return false;
		}

		public int HScrollStep { get { return Math.Min(DaysShowing, 7); } }

		public String HScrollTooltipText
		{
			get { return tooltip.GetToolTip(hscroll); }
			set { tooltip.SetToolTip(hscroll, value); }
		}

		public bool LegacyScrollbars
		{
			get { return legacyScrollbars; }

			set
			{
				if (value != legacyScrollbars)
				{
					legacyScrollbars = value;
					tooltip.Active = value;

					RepositionVScrollbar();
					RepositionHScrollbar();
				}
			}
		}

		private int HourLabelWidth
		{
			get
			{
				if (SavingToImage)
					return minHourLabelWidth;

				// We use the hour label width to absorb any
				// leftover from dividing up the width into days
				int daysWidth = (ClientRectangle.Width - vscroll.Width - minHourLabelWidth - hourLabelIndent);
				int leftover = (daysWidth % DaysShowing);

				return (minHourLabelWidth + leftover);
			}
		}

		// ------------------------------------------------------------------

		protected bool SavingToImage { get; set; }

		// ------------------------------------------------------------------

		private AppHeightDrawMode appHeightMode = AppHeightDrawMode.TrueHeightAll;

        public AppHeightDrawMode AppHeightMode
        {
            get { return appHeightMode; }
            set { appHeightMode = value; }
        }
		
		// ------------------------------------------------------------------
		
		protected int RightClickSelectionMinutes
		{
			get
			{
				int slotMins = (60 / slotsPerHour);

				if (rightClickSelectionMinutes < slotMins)
					return slotMins;

				return ((rightClickSelectionMinutes / slotMins) * slotMins);
			}
		}

		// ------------------------------------------------------------------

		private bool showWorkingHoursOnly = false;

		public bool ShowWorkingHoursOnly
		{
			get { return showWorkingHoursOnly; }

			set
			{
				if (value != showWorkingHoursOnly)
				{
					showWorkingHoursOnly = value;
					Invalidate();
				}
			}
		}

		public int VisibleStartHour
		{
			get
			{
				if (!showWorkingHoursOnly)
					return 0;
				
				return workStart.Hour;
			}
		}

		public int VisibleEndHour
		{
			get
			{
				if (!showWorkingHoursOnly)
					return 24;

				if (workEnd.Min > 0)
					return (workEnd.Hour + 1);

				return workEnd.Hour;
			}
		}

		public int VisibleHours { get { return (VisibleEndHour - VisibleStartHour); } }

		// ------------------------------------------------------------------

		private int slotHeight = 5;

        public int SlotHeight
        {
            get
            {
                return slotHeight;
            }
            set
            {
				int curHeight = slotHeight;
				slotHeight = Math.Max(value, minSlotHeight);

				if (slotHeight != curHeight)
	                OnSlotHeightChanged();
            }
        }

        private void OnSlotHeightChanged()
        {
            Invalidate();
        }
		
		// ------------------------------------------------------------------

        private IRenderer renderer;

        [System.ComponentModel.Browsable(false)]
        [System.ComponentModel.DesignerSerializationVisibility(System.ComponentModel.DesignerSerializationVisibility.Hidden)]
        public IRenderer Renderer
        {
            get
            {
                return renderer;
            }
            set
            {
                renderer = value;
                OnRendererChanged();
            }
        }

        private void OnRendererChanged()
        {
            Font = renderer.BaseFont();
            Invalidate();
        }
		
		// ------------------------------------------------------------------

        private bool ampmdisplay = false;

        public bool AmPmDisplay
        {
            get
            {
                return ampmdisplay;
            }
            set
            {
                ampmdisplay = value;
                OnAmPmDisplayChanged();
            }
        }

        private void OnAmPmDisplayChanged()
        {
            Invalidate();
        }
		
		// ------------------------------------------------------------------

        private bool drawAllAppBorder = false;

        public bool DrawAllAppBorder
        {
            get
            {
                return drawAllAppBorder;
            }
            set
            {
                drawAllAppBorder = value;
                OnDrawAllAppBorderChanged();
            }
        }

        private void OnDrawAllAppBorderChanged()
        {
            Invalidate();
        }
		
		// ------------------------------------------------------------------

		private int DayHeaderHeight
		{
			get
			{
				if ((renderer != null) && (renderer.BaseFont() != null))
					return Math.Max(minDayHeaderHeight, (renderer.BaseFont().Height + 2 + 2));

				// else
				return minDayHeaderHeight;
			}
		}
		
		// ------------------------------------------------------------------

        private int daysToShow = 7;

        [System.ComponentModel.DefaultValue(7)]
        public int DaysShowing
        {
            get
            {
                return daysToShow;
            }

            set
            {
                if ((value > 0) && (value <= 28) && (value != daysToShow))
                {
                    daysToShow = value; // must come before resetting start date
					StartDate = startDate;

					switch (daysToShow)
					{
					case 1:	 HScrollTooltipText = "Change Day";		break;
					case 7:	 HScrollTooltipText = "Change Week";	break;
					default: HScrollTooltipText = "Change";			break;
					}

                    EnsureVisible(SelectedAppointment, true);
					RepositionHScrollbar();

                    OnDaysToShowChanged();
				}
			}
        }

        protected virtual void OnDaysToShowChanged()
        {
            if (CurrentlyEditing)
                FinishEditing(true);

            Invalidate();
        }
		
		// ------------------------------------------------------------------

        private SelectionType selectionType;

        [System.ComponentModel.Browsable(false)]
        public SelectionType SelectionType
        {
            get
            {
                return selectionType;
            }
        }
		
		// ------------------------------------------------------------------

        private DateTime startDate;

        public DateTime StartDate
        {
            get
            {
                return startDate;
            }
            set
            {
				// Move start date to start of week if the number
                // of days showing is a multiple of 7
                if ((DaysShowing % 7) == 0)
				{
					DateTime firstDayOfWeek = FirstDayOfWeekUtility.GetFirstDayOfWeek(value);

					if (startDate != firstDayOfWeek)
						startDate = firstDayOfWeek;
				}
				else
				{
					startDate = value.Date;
				}

				OnStartDateChanged();
			}
		}

        public DateTime EndDate
        {
            get
            {
                return StartDate.AddDays(daysToShow);
            }
        }

        protected virtual void OnStartDateChanged()
        {
            startDate = startDate.Date;

            selectedAppointment = null;
            selectedAppointmentIsNew = false;
            selectionType = SelectionType.DateRange;

            Invalidate();
			RaiseWeekChange(new WeekChangeEventArgs(StartDate, DaysShowing));
        }
		
		// ------------------------------------------------------------------

        private Appointment selectedAppointment;

        [System.ComponentModel.Browsable(false)]
        public Appointment SelectedAppointment
        {
            get { return selectedAppointment; }
			set
			{
				if (value == null)
				{
					selectedAppointment = value;
				}
				else if (value != selectedAppointment)
				{
					// Validate against visible items
					if (ResolveAppointments == null)
						return;

					ResolveAppointmentsEventArgs args = new ResolveAppointmentsEventArgs(StartDate, EndDate);
					ResolveAppointments(this, args);

					foreach (Appointment appt in args.Appointments)
					{
						if (appt == value)
						{
                            // Initialise selection tool
							selectedAppointment = appt;
							selectionType = SelectionType.Appointment;
                            ActiveTool = selectionTool;

							// Ensure short appointments are at least partly visible
							if (!appt.IsLongAppt())
								EnsureVisible(appt, true);

							break;
						}
					}
				}

				Refresh();
			}
        }
		
		// ------------------------------------------------------------------

		static float GetTime(int hours, int mins)
		{
			return (hours + (mins / 60.0f));
		}

		virtual public bool EnsureVisible(Appointment appt, bool partialOK)
		{
			if ((appt == null) || !appt.HasValidDates())
			{
				return false;
			}

			if (appt.EndDate <= StartDate)
			{
				StartDate = appt.EndDate;
			}
			else if (appt.StartDate >= EndDate)
			{
				StartDate = appt.StartDate;
			}

			if (!IsLongAppt(appt))
			{
				// Ensure at least one hour of the task is in view
				float scrollStart = (vscroll.Value / (float)(slotsPerHour * slotHeight)) + VisibleStartHour;
				float scrollEnd = (scrollStart + (vscroll.LargeChange / (float)(slotsPerHour * slotHeight)));
				float scrollDiff = (scrollEnd - scrollStart);

				float apptStart = GetTime(appt.StartDate.Hour, appt.StartDate.Minute);
				float apptEnd = GetTime(appt.EndDate.Hour, appt.EndDate.Minute);

				if (partialOK)
				{
					if (apptStart > (scrollEnd - 1))
					{
						ScrollToHour(apptStart - scrollDiff + 1);
					}
					else if (apptEnd < (scrollStart + 1))
					{
						ScrollToHour(apptEnd - 1);
					}
				}
				else
				{
					if (apptStart <= scrollStart)
					{
						ScrollToHour(apptStart);
					}
					else if (apptEnd > scrollEnd)
					{
						ScrollToHour(apptEnd - scrollDiff);
					}
				}
			}
			
			return true;
		}

		public UInt32 SelectedAppointmentId
        {
            get { return ((selectedAppointment == null) ? 0 : selectedAppointment.Id); }
        }

        private List<System.DayOfWeek> weekendDays = new List<System.DayOfWeek> { System.DayOfWeek.Saturday, System.DayOfWeek.Sunday };

        public List<System.DayOfWeek> WeekendDays
        {
            get { return weekendDays; }
            set 
            { 
                weekendDays = value;
                Invalidate();
            }
        }

		private AppointmentDates selectedDates = new AppointmentDates();

		public AppointmentDates SelectedDates
		{
			get { return selectedDates; }
		}

		public bool HasSelection
		{
			get { return (SelectedDates.Length.Ticks > 0); }
		}

		private ITool activeTool;

        [System.ComponentModel.Browsable(false)]
        public ITool ActiveTool
        {
            get { return activeTool; }
            set { activeTool = value; }
        }

        [System.ComponentModel.Browsable(false)]
        public bool CurrentlyEditing
        {
            get
            {
                return editbox.Visible;
            }
        }

        public class HourMin
        {
            public HourMin(int hour, int min)
            {
                Set(hour, min);
            }

            public HourMin(double hours)
            {
                Set(hours);
            }

            public int Hour
            {
                get;
                private set;
            }

            public int Min
            {
                get;
                private set;
            }
            
            public bool Set(double hours)
            {
                return Set((int)hours, (int)Math.Round((hours - (int)hours) * 60));
            }

			public int TotalMins
			{
				get { return ((Hour * 60) + Min); }
			}

            public bool Set(int hour, int min)
            {
                if ((hour < 0) || (hour > 24))
                    return false;

                if ((min < 0) || (min > 59))
                    return false;

                if ((hour == 24) && (min != 0))
                    return false;

                Hour = hour;
                Min = min;

                return true;
            }

			public static bool operator <(HourMin lhs, HourMin rhs)
			{
				return (lhs.TotalMins < rhs.TotalMins);
			}

			public static bool operator >(HourMin lhs, HourMin rhs)
			{
				return (lhs.TotalMins > rhs.TotalMins);
			}
		};

        private HourMin workStart   = new HourMin(8, 30);  // 8.30 AM
        private HourMin workEnd     = new HourMin(18, 30); // 6.30 PM
        private HourMin lunchStart  = new HourMin(12, 0);  // Midday
        private HourMin lunchEnd    = new HourMin(12, 0);  // Midday

        public HourMin WorkStart
        {
            get { return workStart; }

			set
			{
				if (value != workStart)
				{
					workStart = value;
					Invalidate();
				}
			}
		}


        public HourMin WorkEnd
        {
            get { return workEnd; }

			set
			{ 
				if (value != workEnd)
				{
					workEnd = value;
					Invalidate();
				}
			}
        }
		
        public HourMin LunchStart
        {
            get { return lunchStart; }
            set { lunchStart = value; Invalidate(); }
        }

        public HourMin LunchEnd
        {
            get { return lunchEnd; }
            set { lunchEnd = value; Invalidate(); }
        }

        private int slotsPerHour = 4; // 15 min/slot

        [System.ComponentModel.DefaultValue(4)]
        public int SlotsPerHour
        {
            get
            {
                return slotsPerHour;
            }
            set
            {
                // Must be sensible values
				if (IsValidSlotsPerHour(value))
                {
                    slotsPerHour = value;
					
					AdjustVScrollbar();
                    Invalidate();
                }
            }
        }

		public static Boolean IsValidSlotsPerHour(int numSlots)
		{
			switch (numSlots)
			{
				case 1: return true;	// 60 min/slot
				case 2: return true; 	// 30 min/slot
				case 3: return true; 	// 20 min/slot
				case 4: return true; 	// 15 min/slot
				case 6: return true; 	// 10 min/slot
				case 12: return true;   //  5 min/slot
			}

			return false;
		}

        bool selectedAppointmentIsNew;

        public bool SelectedAppointmentIsNew
        {
            get
            {
                return selectedAppointmentIsNew;
            }
        }

        private bool allowScroll = true;

        [System.ComponentModel.DefaultValue(true)]
        public bool AllowScroll
        {
            get
            {
                return allowScroll;
            }
            set
            {
                allowScroll = value;
                OnAllowScrollChanged();
            }
        }

        private void OnAllowScrollChanged()
        {
            vscroll.Visible = AllowScroll;
            vscroll.Enabled = AllowScroll;
        }

        private bool allowInplaceEditing = true;

        [System.ComponentModel.DefaultValue(true)]
        public bool AllowInplaceEditing
        {
            get
            {
                return allowInplaceEditing;
            }
            set
            {
                allowInplaceEditing = value;
            }
        }

        private bool allowNew = true;

        [System.ComponentModel.DefaultValue(true)]
        public bool AllowNew
        {
            get
            {
                return allowNew;
            }
            set
            {
                allowNew = value;
            }
        }

        private int longAppointmentHeight = 20;

        public int LongAppointmentHeight
        {
            get { return longAppointmentHeight; }
            set 
            { 
                longAppointmentHeight = value;
                Invalidate();
            }
        }

        #endregion

        protected int HeaderHeight
        {
            get
            {
                return DayHeaderHeight + allDayEventsHeaderHeight;
            }
        }

        #region Event Handlers

        void editbox_KeyUp(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Escape)
            {
                e.Handled = true;
                FinishEditing(true);
            }
            else if (e.KeyCode == Keys.Enter)
            {
                e.Handled = true;
                FinishEditing(false);
            }
        }

        void selectionTool_Complete(object sender, EventArgs e)
        {
            if (selectedAppointment != null)
            {
                //System.Threading.ThreadPool.QueueUserWorkItem(new System.Threading.WaitCallback(EnterEditMode));
            }
        }

        void OnVScroll(object sender, ScrollEventArgs e)
        {
            Invalidate();

            //scroll text box too
            if (editbox.Visible)
                editbox.Top += e.OldValue - e.NewValue;
        }

		protected override void OnKeyDown(KeyEventArgs e)
		{
			switch (e.KeyCode)
			{
			case Keys.Left:
				DoHorizontalScroll(ScrollEventType.SmallDecrement);
				break;

			case Keys.Right:
				DoHorizontalScroll(ScrollEventType.SmallIncrement);
				break;
			}

			base.OnKeyDown(e);
		}

        void OnHScroll(object sender, ScrollEventArgs e)
        {
			int numDays = 0;

			switch (e.Type)
			{
			case ScrollEventType.First:
			case ScrollEventType.Last:
			case ScrollEventType.SmallDecrement:
			case ScrollEventType.SmallIncrement:
			case ScrollEventType.LargeDecrement:
			case ScrollEventType.LargeIncrement:
				DoHorizontalScroll(e.Type);
				break;

			case ScrollEventType.ThumbPosition:
			case ScrollEventType.ThumbTrack:
				if (e.NewValue != e.OldValue)
				{
					// Move in single days when showing fewer than 7 days
					numDays = ((e.NewValue > e.OldValue) ? 1 : -1);

					// else move in single weeks
					if ((DaysShowing % 7) == 0)
						numDays *= 7;

					StartDate = StartDate.AddDays(numDays);
				}
				break;

			case ScrollEventType.EndScroll: 
				{
					// Adjust thumb to give the semblance of infinite paging
					//
					// Some trickery is required because:
					// 1. We have to delay the reset until this message handling is complete
					// 2. The reset can only happen on the UI thread
					int newPos = hscroll.Value;

					if (Math.Abs(newPos) >= (hscroll.Maximum - hscroll.LargeChange))
						newPos /= 2;

					var reset = new Action(() => hscroll.Value = newPos);
					var thread = new Thread(() => hscroll.Invoke(reset));

					thread.Start();
				}
				break;
			}
        }

		protected void DoHorizontalScroll(ScrollEventType type)
		{
			int numDays = 0;

			switch (type)
			{
			case ScrollEventType.First:
				numDays = hscroll.Minimum;
				break;

			case ScrollEventType.Last:
				numDays = hscroll.Maximum;
				break;

			case ScrollEventType.SmallDecrement:
				numDays = -Math.Min(DaysShowing, 7);
				break;

			case ScrollEventType.SmallIncrement:
				numDays = Math.Min(DaysShowing, 7);
				break;

			case ScrollEventType.LargeDecrement:
				numDays = -Math.Max(DaysShowing, 7);
				break;

			case ScrollEventType.LargeIncrement:
				numDays = Math.Max(DaysShowing, 7);
				break;

			// Not supported by this function
			case ScrollEventType.ThumbPosition:
			case ScrollEventType.ThumbTrack:
			case ScrollEventType.EndScroll:
				Debug.Assert(false);
				return;
			}

			StartDate = StartDate.AddDays(numDays);
		}

		protected override void OnSizeChanged(EventArgs e)
		{
			base.OnSizeChanged(e);

			RepositionHScrollbar();
			RepositionVScrollbar();
		}

		protected void RepositionHScrollbar()
		{
			hscroll.SuspendLayout();

			if (LegacyScrollbars)
			{
				hscroll.Location = new Point(0, 0);
				hscroll.Width = (HourLabelWidth + hourLabelIndent);
				hscroll.Height = minDayHeaderHeight;
			}
			else
			{
				hscroll.Left = (HourLabelWidth + hourLabelIndent + 1);
				hscroll.Width = (Width - hscroll.Left - vscroll.Width - 2);
				hscroll.Top = (Height - hscroll.Height - 2);
			}

			hscroll.ResumeLayout();
		}

		protected void RepositionVScrollbar()
		{
			vscroll.SuspendLayout();

			vscroll.Left = (Width - vscroll.Width - 2);

			if (LegacyScrollbars)
			{
				vscroll.Top = 0;
				vscroll.Height = Height;
			}
			else
			{
				vscroll.Top = HeaderHeight;
				vscroll.Height = (Height - vscroll.Top - hscroll.Height - 2);
			}

			vscroll.ResumeLayout();
		}

		protected override void SetBoundsCore(int x, int y, int width, int height, BoundsSpecified specified)
        {
            base.SetBoundsCore(x, y, width, height, specified);

			Invalidate();
		}

        protected void AdjustVScrollbar()
        {
			RepositionVScrollbar();

			if (Height < HeaderHeight)
				return;

			// Preserve 'end of scroll' where possible
			bool scrollToBottom = (vscroll.Value >= (vscroll.Maximum - vscroll.LargeChange));

			// Auto-calculate best 'hour' height
			int availHeight = (Height - HeaderHeight);

			if (!LegacyScrollbars)
				availHeight -= hscroll.Height;

			slotHeight = ((availHeight / (VisibleHours * slotsPerHour)) + 1);

			if (slotHeight < minSlotHeight)
				slotHeight = minSlotHeight;

			int oneHourHeight = (slotHeight * slotsPerHour);
			AllowScroll = ((availHeight > 0) && ((oneHourHeight * VisibleHours) > availHeight));

			vscroll.Minimum = 0;
			vscroll.SmallChange = oneHourHeight;

			if (AllowScroll)
			{
				vscroll.Maximum = (oneHourHeight * VisibleHours) + 1;
				vscroll.LargeChange = availHeight;

				if (scrollToBottom)
					vscroll.Value = (vscroll.Maximum - vscroll.LargeChange);
			}
			else
			{
				vscroll.Value = 0;
				Invalidate(false);
			}
        }

		public void GetDateRange(out DateTime start, out DateTime end)
		{
			start = StartDate;
			end = EndDate;
		}

        protected override void OnPaintBackground(PaintEventArgs pevent)
        {
            // Flicker free
        }

        protected override void OnMouseDoubleClick(MouseEventArgs e)
        {
			if (ActiveTool == selectionTool)
				selectionTool.Reset();

			base.OnMouseDoubleClick(e);
		}

		protected override void OnMouseDown(MouseEventArgs e)
		{
			// Ignore all clicks on the day header
			if (e.Y > DayHeaderHeight)
			{
				if (e.Button == MouseButtons.Left)
				{
					// Capture focus
					Focus();

					if (CurrentlyEditing)
					{
						FinishEditing(false);
					}

					if (selectedAppointmentIsNew)
					{
						RaiseNewAppointment();
					}

					ITool newTool = null;
					Appointment appt = GetAppointmentAt(e.X, e.Y);

					if (appt == null)
					{
						if (e.Y < HeaderHeight && e.Y > DayHeaderHeight)
						{
							newTool = drawTool;
							selectionType = SelectionType.None;

							base.OnMouseDown(e);
							return;
						}

						newTool = drawTool;
						selectionType = SelectionType.DateRange;
					}
					else
					{
						newTool = selectionTool;
						selectedAppointment = appt;
						selectionType = SelectionType.Appointment;

						Invalidate();
					}

					if (activeTool != null)
					{
						activeTool.MouseDown(e);
					}

					if ((activeTool != newTool) && (newTool != null))
					{
						newTool.Reset();
						newTool.MouseDown(e);
					}

					activeTool = newTool;
				}
				else if (e.Button == MouseButtons.Right)
				{
					// If we right-click outside the area of selection,
					// select whatever is under the cursor
					Appointment appt = GetAppointmentAt(e.X, e.Y);
					bool redraw = false;

					if (appt == null)
					{
						if (selectedAppointment != null)
							redraw = true;

						selectedAppointment = null;
						RaiseSelectionChanged(selectedAppointment);

						if (GetTrueRectangle().Contains(e.Location))
						{
							DateTime click = GetDateTimeAt(e.X, e.Y);
							selectionType = SelectionType.DateRange;

							if ((click < SelectedDates.Start) || (click >= SelectedDates.End))
							{
								int slotMins = (60 / slotsPerHour);

								SelectedDates.Start = new DateTime(click.Year, click.Month, click.Day, click.Hour, ((click.Minute / slotMins) * slotMins), 0);
								SelectedDates.End = SelectedDates.Start.AddMinutes(RightClickSelectionMinutes);

								redraw = true;
							}
						}
					}
					else if (appt != selectedAppointment)
					{
						selectedAppointment = appt;
						redraw = true;
						RaiseSelectionChanged(selectedAppointment);

					}

					if (redraw)
					{
						Refresh();
					}
				}
			}

            base.OnMouseDown(e);
        }

        protected override void OnMouseHover(EventArgs e)
        {
            base.OnMouseHover(e);
        }

		protected override void OnMouseMove(MouseEventArgs e)
		{
			if (activeTool != null)
				activeTool.MouseMove(e);

			base.OnMouseMove(e);
		}

		protected override void OnMouseUp(MouseEventArgs e)
        {
            if (activeTool != null)
                activeTool.MouseUp(e);

            base.OnMouseUp(e);
        }

        protected override void OnMouseWheel(MouseEventArgs e)
        {
            base.OnMouseWheel(e);

			if (ModifierKeys.Equals(Keys.None))
			{
				MouseWheelScroll(e.Delta < 0);
			}
        }

        System.Collections.Hashtable cachedAppointments = new System.Collections.Hashtable();

        protected virtual void OnResolveAppointments(ResolveAppointmentsEventArgs args)
        {
            if (ResolveAppointments != null)
                ResolveAppointments(this, args);

            allDayEventsHeaderHeight = 0;

            // cache resolved appointments in hash-table by days.
            cachedAppointments.Clear();

            if ((selectedAppointmentIsNew) && (selectedAppointment != null))
            {
                if ((selectedAppointment.StartDate > args.StartDate) && (selectedAppointment.StartDate < args.EndDate))
                {
                    args.Appointments.Add(selectedAppointment);
                }
            }

            foreach (Appointment appt in args.Appointments)
            {
				int key = (IsLongAppt(appt) ? -1 : appt.StartDate.DayOfYear);
                AppointmentList list = (AppointmentList)cachedAppointments[key];

                if (list == null)
                {
                    list = new AppointmentList();
                    cachedAppointments[key] = list;
                }

                list.Add(appt);
            }
        }

		protected bool IsLongAppt(Appointment appt)
		{
			if (appt == null)
				return false;

			if ((appt == selectedAppointment) && (activeTool == selectionTool) && selectionTool.IsEditing)
			{
				return selectionTool.IsEditingLongAppt;
			}

			return appt.IsLongAppt();
		}

		protected void RaiseSelectionChanged(Appointment appt)
        {
			selectionType = SelectionType.Appointment;

            RaiseSelectionChanged(new AppointmentEventArgs(appt));
        }

		internal void RaiseSelectionChanged(AppointmentEventArgs e)
        {
            SelectionChanged?.Invoke(this, e);
        }

        internal void RaiseAppointmentMove(AppointmentEventArgs e)
        {
            AppointmentMove?.Invoke(this, e);
        }

		internal void RaiseWeekChange(WeekChangeEventArgs e)
		{
			WeekChange?.Invoke(this, e);
		}

        protected override void OnKeyPress(KeyPressEventArgs e)
        {
            if ((allowNew) && char.IsLetterOrDigit(e.KeyChar))
            {
                if ((SelectionType == SelectionType.DateRange))
                {
                    if (!selectedAppointmentIsNew)
                        EnterNewAppointmentMode(e.KeyChar);
                }
            }
        }

        private void EnterNewAppointmentMode(char key)
        {
            Appointment appt = new Appointment();
            appt.StartDate = SelectedDates.Start;
            appt.EndDate = SelectedDates.End;
            appt.Title = key.ToString();

            selectedAppointment = appt;
            selectedAppointmentIsNew = true;

            activeTool = selectionTool;

            Invalidate();

            System.Threading.ThreadPool.QueueUserWorkItem(new System.Threading.WaitCallback(EnterEditMode));
        }

        private delegate void StartEditModeDelegate(object state);

        private void EnterEditMode(object state)
        {
            if (!allowInplaceEditing)
                return;

            if (InvokeRequired)
            {
                Appointment selectedApp = selectedAppointment;

                System.Threading.Thread.Sleep(200);

                if (selectedApp == selectedAppointment)
                    Invoke(new StartEditModeDelegate(EnterEditMode), state);
            }
            else
            {
                StartEditing();
            }
        }

        internal void RaiseNewAppointment()
        {
            NewAppointmentEventArgs args = new NewAppointmentEventArgs(selectedAppointment.Title, selectedAppointment.StartDate, selectedAppointment.EndDate);

            if (NewAppointment != null)
            {
                NewAppointment(this, args);
            }

            selectedAppointment = null;
            selectedAppointmentIsNew = false;

            Invalidate();
        }

        #endregion

        #region Public Methods

        public void ScrollToTop()
        {
            vscroll.Value = vscroll.Minimum;
            Invalidate();
        }

		public bool ScrollToHour(float hour) // 0-23
		{
			if (hour < 0f || hour > 23f)
				return false;

			int scrollVal = (int)((hour - VisibleStartHour) * slotsPerHour * slotHeight);
			vscroll.Value = Math.Max(0, scrollVal);

			Invalidate();

			return true;
		}

		protected void MouseWheelScroll(bool down)
        {
            if (AllowScroll)
            {
                if (down)
                {
					int maxRange = (vscroll.Maximum - vscroll.LargeChange);

					// mouse wheel scroll down
                    int newScrollValue = vscroll.Value + vscroll.SmallChange;

					if (newScrollValue < maxRange)
                        vscroll.Value = newScrollValue;
                    else
						vscroll.Value = maxRange;
                }
                else
                {
					// mouse wheel scroll up
                    int newScrollValue = vscroll.Value - vscroll.SmallChange;

                    if (newScrollValue > vscroll.Minimum)
                        vscroll.Value = newScrollValue;
                    else
                        vscroll.Value = vscroll.Minimum;
                }

                Invalidate();
            }
        }

        public Rectangle GetTrueRectangle()
        {
            Rectangle truerect;

            truerect = ClientRectangle;
            truerect.X += HourLabelWidth + hourLabelIndent;
            truerect.Width -= vscroll.Width + HourLabelWidth + hourLabelIndent;
            truerect.Y += HeaderHeight;
            truerect.Height -= HeaderHeight;

            return truerect;
        }

        public Rectangle GetFullDayApptsRectangle()
        {
            Rectangle fulldayrect;
            fulldayrect = ClientRectangle;
            fulldayrect.Height = HeaderHeight - DayHeaderHeight;
            fulldayrect.Y += DayHeaderHeight;
            fulldayrect.Width -= (HourLabelWidth + hourLabelIndent + vscroll.Width);
            fulldayrect.X += HourLabelWidth + hourLabelIndent;

            return fulldayrect;
        }

        public void StartEditing()
        {
            if (!selectedAppointment.Locked && appointmentViews.ContainsKey(selectedAppointment))
            {
                Rectangle editBounds = appointmentViews[selectedAppointment].Rectangle;

                editBounds.Inflate(-3, -3);
                editBounds.X += appointmentGripWidth - 2;
                editBounds.Width -= appointmentGripWidth - 5;

                editbox.Bounds = editBounds;
                editbox.Text = selectedAppointment.Title;
                editbox.Visible = true;
                editbox.SelectionStart = editbox.Text.Length;
                editbox.SelectionLength = 0;

                editbox.Focus();
            }
        }

        public void FinishEditing(bool cancel)
        {
            editbox.Visible = false;

            if (!cancel)
            {
                if (selectedAppointment != null)
                    selectedAppointment.Title = editbox.Text;
            }
            else
            {
                if (selectedAppointmentIsNew)
                {
                    selectedAppointment = null;
                    selectedAppointmentIsNew = false;
                }
            }

            Invalidate();
            Focus();
        }

        public DateTime GetDateTimeAt(int x, int y)
        {
			bool longAppt = GetFullDayApptsRectangle().Contains(x, y);

			return GetDateTimeAt(x, y, longAppt);
        }

		public DateTime GetDateTimeAt(int x, int y, bool longAppt)
		{
			DateTime dateTime = GetDateAt(x, longAppt);

			if (!longAppt)
			{
				TimeSpan time = GetTimeAt(y);
				dateTime = dateTime.Add(time);
			}

			return dateTime;
		}

        public virtual DateTime GetDateAt(int x, bool longAppt)
        {
            DateTime date = startDate.Date;
            int dayWidth = (ClientRectangle.Width - (vscroll.Width + HourLabelWidth + hourLabelIndent)) / daysToShow;

			if (longAppt)
			{
				int hours = ((24 * (x - HourLabelWidth)) / dayWidth);
				date = date.AddHours(hours);
			}
			else
			{
				date = date.AddDays((x - HourLabelWidth) / dayWidth).Date;

				if (date < startDate)
					date = startDate;

				if (date >= EndDate)
					date = EndDate.AddDays(-1);
			}

            return date;
        }

        public virtual TimeSpan GetTimeAt(int y)
        {
            double numSlots = (y - HeaderHeight + vscroll.Value) / (double)slotHeight;

			// Clip at top and bottom
			int maxSlots = (VisibleHours * slotsPerHour);
            numSlots = Math.Max(0, Math.Min(maxSlots, numSlots));

            // nearest slot
            int minsPerSlot = (60 / slotsPerHour);
            int minutes = ((int)numSlots * minsPerSlot);

            return new TimeSpan((VisibleStartHour + (minutes / 60)), (minutes % 60), 0);
        }

        public Appointment GetAppointmentAt(int x, int y)
        {
			var apptView = GetAppointmentViewAt(x, y);

			return (apptView == null) ? null : apptView.Appointment;
        }

		virtual public AppointmentView GetAppointmentViewAt(int x, int y, out Rectangle apptRect)
		{
			var apptView = GetAppointmentViewAt(x, y);

			if (apptView == null)
				apptRect = Rectangle.Empty;
			else
				apptRect = apptView.Rectangle;

			return apptView;
		}

		virtual protected bool AppointmentViewContains(AppointmentView view, int x, int y)
		{
			return view.Rectangle.Contains(x, y);
		}

		virtual protected AppointmentView GetAppointmentViewAt(int x, int y)
		{
			if (GetFullDayApptsRectangle().Contains(x, y))
			{
				foreach (AppointmentView view in longAppointmentViews.Values)
				{
					if (AppointmentViewContains(view, x, y))
						return view;
				}
			}
			else
			{
				foreach (AppointmentView view in appointmentViews.Values)
				{
					if (AppointmentViewContains(view, x, y))
						return view;
				}
			}

			return null;
		}

		#endregion

		#region Drawing Methods

		protected override void OnPaint(PaintEventArgs e)
        {
            if (CurrentlyEditing)
            {
                FinishEditing(false);
            }

            // Calculate visible rectangle
            Rectangle rect = new Rectangle(0, 0, ClientRectangle.Width, ClientRectangle.Height);
			DoPaint(e, rect);

			if (LegacyScrollbars)
				RepositionHScrollbar(); // because hour label width may have changed
        }

		protected void DoPaint(PaintEventArgs e, Rectangle rect)
        {
            // resolve appointments on visible date range.
            ResolveAppointmentsEventArgs args = new ResolveAppointmentsEventArgs(StartDate, EndDate);
            OnResolveAppointments(args);

			if (LegacyScrollbars)
			{
				using (SolidBrush backBrush = new SolidBrush(renderer.BackColor()))
					e.Graphics.FillRectangle(backBrush, rect);
			}
			else
			{
				e.Graphics.FillRectangle(SystemBrushes.Control, rect);
			}

			e.Graphics.SmoothingMode = SmoothingMode.AntiAlias;
			e.Graphics.TextRenderingHint = TextRenderingHint.AntiAliasGridFit;

            Rectangle headerRectangle = rect;
            headerRectangle.X += (HourLabelWidth + hourLabelIndent);
            headerRectangle.Width -= (HourLabelWidth + hourLabelIndent + vscroll.Width);
            headerRectangle.Height = DayHeaderHeight;

            if (e.ClipRectangle.IntersectsWith(headerRectangle))
                DrawDayHeaders(e, headerRectangle);

            Rectangle daysRectangle = rect;
            daysRectangle.X = headerRectangle.X; // previously calculated
			daysRectangle.Width = headerRectangle.Width; // previously calculated
			daysRectangle.Y += HeaderHeight;
			daysRectangle.Height -= HeaderHeight;

			if (!LegacyScrollbars)
				daysRectangle.Height -= hscroll.Height;

            DrawDays(e, ref daysRectangle);

            Rectangle hourLabelRectangle = rect;
            hourLabelRectangle.Y = daysRectangle.Y; // previously calculated
			hourLabelRectangle.Height = daysRectangle.Height; // previously calculated

			if (!LegacyScrollbars)
			{
				hourLabelRectangle.X += hourLabelIndent;
				hourLabelRectangle.Width = HourLabelWidth;

				using (SolidBrush backBrush = new SolidBrush(renderer.BackColor()))
					e.Graphics.FillRectangle(backBrush, hourLabelRectangle);

				// Draw lines delineating the top of the vertical scrollbar
				// and the left of the horizontal scrollbar
				e.Graphics.DrawLine(SystemPens.ControlDark, vscroll.Left, vscroll.Top - 1, vscroll.Right, vscroll.Top -1);
				e.Graphics.DrawLine(SystemPens.ControlDark, hscroll.Left - 1, hscroll.Top, hscroll.Left - 1, hscroll.Bottom);
			}

			DrawHourLabels(e, hourLabelRectangle);

			if (LegacyScrollbars && !AllowScroll)
			{
				Rectangle scrollrect = rect;

				scrollrect.X = (headerRectangle.Width + HourLabelWidth + hourLabelIndent);
				scrollrect.Width = vscroll.Width;

				using (SolidBrush backBrush = new SolidBrush(renderer.BackColor()))
					e.Graphics.FillRectangle(backBrush, scrollrect);
			}
		}

		protected void DrawHourLabels(PaintEventArgs e, Rectangle rect)
        {
            e.Graphics.SetClip(rect);

            for (int hour = 0; hour < VisibleHours; hour++)
            {
                Rectangle hourRectangle = rect;
                hourRectangle.Y += ((hour * slotsPerHour * slotHeight) - vscroll.Value);

                var minuteRect = hourRectangle;
                minuteRect.Height = slotHeight;

                int interval = (60 / slotsPerHour);

                for (int minute = 0; minute < 60; minute += interval)
                {
                    renderer.DrawMinuteLine(e.Graphics, minuteRect, minute);
                    minuteRect.Y += slotHeight;
                }

                renderer.DrawHourLabel(e.Graphics, hourRectangle, (hour + VisibleStartHour), ampmdisplay);
            }

			// draw a line at the top for closure
			if (LegacyScrollbars)
				e.Graphics.DrawLine(SystemPens.ControlDark, rect.Left, rect.Y, rect.Width, rect.Y);

            e.Graphics.ResetClip();
		}

		protected void UpdateColumnWidth(Graphics g, int colWidth)
		{
			// For derived classes
		}

		protected void DrawDayHeaders(PaintEventArgs e, Rectangle rect)
        {
			e.Graphics.SetClip(rect);

			int dayWidth = (rect.Width / daysToShow);
			NotifyDayWidth?.Invoke(this, e.Graphics, dayWidth);

            // one day header rectangle
            Rectangle dayHeaderRectangle = new Rectangle(rect.Left, rect.Top, dayWidth, rect.Height);
            DateTime headerDate = startDate;

            for (int day = 0; day < daysToShow; day++)
            {
                renderer.DrawDayHeader(e.Graphics, dayHeaderRectangle, headerDate, (day == 0));

                dayHeaderRectangle.X += dayWidth;
                headerDate = headerDate.AddDays(1);
            }

			e.Graphics.ResetClip();
		}

		protected Rectangle GetHourRangeRectangle(HourMin start, HourMin end, Rectangle baseRectangle)
        {
			if (start < end)
			{
				var dateStart = new DateTime(1, 1, 1, start.Hour, start.Min, 0);
				var dateEnd = (end.Hour >= 24) ? new DateTime(1, 1, 2, 0, 0, 0) : new DateTime(1, 1, 1, end.Hour, end.Min, 0);

				return GetHourRangeRectangle(dateStart, dateEnd, baseRectangle);
			}

			return Rectangle.Empty;
		}

		protected int GetHourScrollPos(DateTime time)
		{
			int vPos = ((time.Hour - VisibleStartHour) * slotHeight * slotsPerHour) + ((time.Minute * slotHeight) / (60 / slotsPerHour));

			return (vPos - vscroll.Value + HeaderHeight);
		}

		protected Rectangle GetHourRangeRectangle(DateTime start, DateTime end, Rectangle baseRectangle)
        {
			Rectangle rect = Rectangle.Empty;

			if (start < end)
			{
				// Special case: end time is 'end of day'
				if (end == start.Date.AddDays(1))
					end = end.AddSeconds(-1);

				int startY = GetHourScrollPos(start);
				int endY = GetHourScrollPos(end);

				rect = baseRectangle;
				rect.Y = startY;
				rect.Height = System.Math.Max(1, endY - startY);
			}

			return rect;
        }

        protected void DrawWorkHours(PaintEventArgs e, HourMin start, HourMin end, Rectangle rect)
        {
            Rectangle hoursRect = GetHourRangeRectangle(start, end, rect);

            if (hoursRect.Y < HeaderHeight)
            {
                hoursRect.Height -= HeaderHeight - hoursRect.Y;
                hoursRect.Y = HeaderHeight;
            }

            renderer.DrawHourRange(e.Graphics, hoursRect, false, false);
        }

		protected virtual bool WantDrawDaySelection { get { return Focused; } }

		protected void DrawDaySelection(PaintEventArgs e, Rectangle rect, DateTime time)
		{
			if (WantDrawDaySelection && (selectionType == SelectionType.DateRange) && (time.Date == SelectedDates.Start.Date))
			{
				Rectangle selectionRectangle = GetHourRangeRectangle(SelectedDates.Start, SelectedDates.End, rect);

				selectionRectangle.X += (dayGripWidth + 1);
				selectionRectangle.Width -= (dayGripWidth + 1);

				// GDI+ off-by-one bug
				selectionRectangle.Width--;
				selectionRectangle.Height++;

				if (selectionRectangle.Height > 1)
					renderer.DrawHourRange(e.Graphics, selectionRectangle, false, true);
			}
		}

		protected void DrawDaySlotSeparators(PaintEventArgs e, Rectangle rect, DateTime time)
		{
			for (int slot = 0; slot < (VisibleHours * slotsPerHour); slot++)
			{
				int y = rect.Top + (slot * slotHeight) - vscroll.Value;

				Color color1 = renderer.HourSeperatorColor();
				Color color2 = renderer.HalfHourSeperatorColor();

				using (Pen pen = new Pen(((slot % slotsPerHour) == 0 ? color1 : color2)))
					e.Graphics.DrawLine(pen, rect.Left, y, rect.Right, y);

				if (y > rect.Bottom)
					break;
			}
		}

		protected void DrawDayGripper(PaintEventArgs e, Rectangle rect)
		{
			renderer.DrawDayGripper(e.Graphics, rect, dayGripWidth);
		}

		protected void DrawDayAppointments(PaintEventArgs e, Rectangle rect, DateTime time)
		{
			AppointmentList appointments = (AppointmentList)cachedAppointments[time.DayOfYear];

			if (appointments != null)
			{
				List<string> groups = new List<string>();

				foreach (Appointment app in appointments)
					if (!groups.Contains(app.Group))
						groups.Add(app.Group);

				rect.X += (dayGripWidth + daySpacing);
				rect.Width -= (dayGripWidth + daySpacing);

                int numGroups = groups.Count;
                int groupWidth = (rect.Width / numGroups);
                int leftOverWidth = (rect.Width - (numGroups * groupWidth));

				Rectangle dayRect = rect;
				dayRect.Width = groupWidth - ((numGroups > 1) ? groupSpacing : 0);

				groups.Sort();

				for (int group = 0; group < numGroups; group++)
				{
                    // Add left-over width to last column
                    if (group == (numGroups - 1))
                        dayRect.Width += leftOverWidth;

					DrawDayGroupAppointments(e, dayRect, time, groups[group]);
					dayRect.X += groupWidth;
				}
			}
		}

		virtual protected void DrawDay(PaintEventArgs e, Rectangle rect, DateTime time)
        {
            renderer.DrawDayBackground(e.Graphics, rect);

            if (!weekendDays.Contains(time.DayOfWeek))
            {
                DrawWorkHours(e, workStart, lunchStart, rect);
                DrawWorkHours(e, lunchEnd, workEnd, rect);
            }

			DrawDaySelection(e, rect, time);
			DrawDaySlotSeparators(e, rect, time);
            DrawDayGripper(e, rect);
			DrawDayAppointments(e, rect, time);
        }

        private Dictionary<Appointment, AppointmentView> appointmentViews = new Dictionary<Appointment, AppointmentView>();
        private Dictionary<Appointment, AppointmentView> longAppointmentViews = new Dictionary<Appointment, AppointmentView>();

		virtual protected AppointmentView NewAppointmentView(Appointment appt)
		{
			return new AppointmentView(appt);
		}

		protected AppointmentView GetAppointmentView(Appointment appt)
        {
            AppointmentView view = null;

			// Short appointments
			if (!appointmentViews.TryGetValue(appt, out view))
			{
				// Long appointments
				longAppointmentViews.TryGetValue(appt, out view);
			}

			return view;
        }

		public bool GetAppointmentRect(Appointment appt, ref Rectangle rect)
        {
            AppointmentView view = GetAppointmentView(appt);

			if (view == null)
				return false;

			rect = view.Rectangle;
			return true;
        }

        private void DrawDayGroupAppointments(PaintEventArgs e, Rectangle rect, DateTime time, string group)
        {
            AppointmentList appointments = (AppointmentList)cachedAppointments[time.DayOfYear];

            if (appointments == null)
                return;

            var groupAppts = new AppointmentList(appointments.Where(appt => (appt.Group == group)));

            if ((groupAppts == null) || (groupAppts.Count() == 0))
                return;

            int numLayers = RecalcAppointmentLayers(groupAppts);

            if (numLayers == 0)
                return;

            // Sort them into buckets of independent tasks
            groupAppts.SortByStartDate();

            var buckets = new List<AppointmentList>();

            if (numLayers == 1)
            {
                buckets.Add(groupAppts);
            }
            else
            {
                AppointmentList currentBucket = null;
                DateTime maxBucketDate = DateTime.MinValue;

                foreach (var appt in groupAppts)
                {
                    if (appt.StartDate >= maxBucketDate)
                    {
                        currentBucket = new AppointmentList();
                        buckets.Add(currentBucket);
                    }

                    currentBucket.Add(appt);

                    if (appt.EndDate > maxBucketDate)
                        maxBucketDate = appt.EndDate;
                }
            }

            // Draw each bucket's contents as an independent list
            foreach (var bucket in buckets)
            {
                int numBucketLayers = Math.Min(numLayers, bucket.Count);
                int layerWidth = (rect.Width / numBucketLayers);
                int leftOverWidth = (rect.Width - (layerWidth * numBucketLayers));

                foreach (var appt in bucket)
                {
                    DateTime apptStart, apptEnd;
                    GetAppointmentDrawTimes(appt, out apptStart, out apptEnd);

                    Rectangle apptRect = rect;
                    apptRect.X += (appt.Layer * layerWidth);
                    apptRect.Width = (layerWidth - ((numBucketLayers > 1) ? appointmentSpacing : 0));

                    // Last column picks up the slack
                    if (appt.Layer == (numBucketLayers - 1))
                        apptRect.Width += leftOverWidth;

                    apptRect = GetHourRangeRectangle(apptStart, apptEnd, apptRect);

                    Rectangle gripRect = GetHourRangeRectangle(appt.StartDate, appt.EndDate, apptRect);
                    gripRect.Width = appointmentGripWidth;

					var apptView = NewAppointmentView(appt);
					appointmentViews[appt] = apptView;

					apptView.Rectangle = apptRect;
					apptView.GripRect = gripRect;

                    if (DrawAllAppBorder)
                        appt.DrawBorder = true;

                    renderer.DrawAppointment(e.Graphics, rect, apptView, WantDrawAppointmentSelected(appt));
                }
            }
        }

        private void GetAppointmentDrawTimes(Appointment appt, out DateTime apptStart, out DateTime apptEnd)
        {
            apptStart = appt.StartDate;
            apptEnd = appt.EndDate;

            // Adjust depending on the height display mode 
            switch (AppHeightMode)
            {
                case AppHeightDrawMode.FullHalfHourBlocksShort:
                    if (appt.EndDate.Subtract(appt.StartDate).TotalMinutes < (60 / slotsPerHour))
                    {
                        // Round the start/end time to the last/next halfhour
                        apptStart = appt.StartDate.AddMinutes(-appt.StartDate.Minute);
                        apptEnd = appt.EndDate.AddMinutes((60 / slotsPerHour) - appt.EndDate.Minute);

                        // Make sure we've rounded it to the correct halfhour :)
                        if (appt.StartDate.Minute >= (60 / slotsPerHour))
                            apptStart = apptStart.AddMinutes((60 / slotsPerHour));

                        if (appt.EndDate.Minute > (60 / slotsPerHour))
                            apptEnd = apptEnd.AddMinutes((60 / slotsPerHour));
                    }
                    break;

                case AppHeightDrawMode.FullHalfHourBlocksAll:
                    {
                        apptStart = appt.StartDate.AddMinutes(-appt.StartDate.Minute);

                        if (appt.EndDate.Minute != 0 && appt.EndDate.Minute != (60 / slotsPerHour))
                            apptEnd = appt.EndDate.AddMinutes((60 / slotsPerHour) - appt.EndDate.Minute);
                        else
                            apptEnd = appt.EndDate;

                        if (appt.StartDate.Minute >= (60 / slotsPerHour))
                            apptStart = apptStart.AddMinutes((60 / slotsPerHour));

                        if (appt.EndDate.Minute > (60 / slotsPerHour))
                            apptEnd = apptEnd.AddMinutes((60 / slotsPerHour));
                    }
                    break;

                // Based on previous code
                case AppHeightDrawMode.EndHalfHourBlocksShort:
                    if (appt.EndDate.Subtract(appt.StartDate).TotalMinutes < (60 / slotsPerHour))
                    {
                        // Round the end time to the next halfhour
                        apptEnd = appt.EndDate.AddMinutes((60 / slotsPerHour) - appt.EndDate.Minute);

                        // Make sure we've rounded it to the correct halfhour :)
                        if (appt.EndDate.Minute > (60 / slotsPerHour))
                            apptEnd = apptEnd.AddMinutes((60 / slotsPerHour));
                    }
                    break;

                case AppHeightDrawMode.EndHalfHourBlocksAll:
                    {
                        // Round the end time to the next halfhour
                        if (appt.EndDate.Minute != 0 && appt.EndDate.Minute != (60 / slotsPerHour))
                            apptEnd = appt.EndDate.AddMinutes((60 / slotsPerHour) - appt.EndDate.Minute);
                        else
                            apptEnd = appt.EndDate;

                        // Make sure we've rounded it to the correct halfhour :)
                        if (appt.EndDate.Minute > (60 / slotsPerHour))
                            apptEnd = apptEnd.AddMinutes((60 / slotsPerHour));
                    }
                    break;
            }
        }

        private void DrawDays(PaintEventArgs e, ref Rectangle rect)
        {
			// There are a nasty set of dependencies between the 
			// vertical scrollbar on the one hand, the long and day
			// appointments on the other:
			//
			// 1. The vertical scrollbar requires 'allDayEventsHeaderHeight'
			//    to have been pre-calculated which happens in 'DrawLongAppointments'.
			// 2. The day appointments depend on the slot height having
			//    been pre-calculated which happens in 'AdjustVScrollbar'.
			// 3. The long and day appointments are rendered here which means
			//    that the only place AdjustVScrollbar SHOULD be called is here.
			//    Any need to update the vertical scrollbar can therefore be
			//    achieved by calling 'Invalidate'.
			// 4. I could try to re-architect this more cleanly but not for now.

            DrawLongAppointments(e.Graphics, rect);
			AdjustVScrollbar();

            // Draw the day appointments
            appointmentViews.Clear();

            DateTime time = startDate;
            rect.Y += allDayEventsHeaderHeight;
            rect.Height -= allDayEventsHeaderHeight;

			Rectangle dayRect = rect;
			dayRect.Width = (rect.Width / daysToShow);

			for (int day = 0; day < daysToShow; day++)
            {
				e.Graphics.SetClip(dayRect);

				DrawDay(e, dayRect, time);

				e.Graphics.ResetClip();

                dayRect.X += dayRect.Width;
				time = time.AddDays(1);
            }
        }

		protected virtual bool AppointmentsIntersect(Appointment appt, Appointment apptOther)
		{
			return appt.Intersects(apptOther);
		}

        private int RecalcAppointmentLayers(AppointmentList appointments)
        {
            if ((appointments == null) || (appointments.Count == 0))
                return 0;

            var processed = new AppointmentList();
            var layers = new SortedSet<int>();

            foreach (var appt in appointments)
            {
                appt.Layer = 0;

                if (processed.Count != 0)
                {
					// Look through the previously processed 
					// appointments, one layer at a time, for 
					// any which intersect this appt
					foreach (int lay in layers)
                    {
						bool intersect = false;

						foreach (var processedAppt in processed)
                        {
                            if ((processedAppt.Layer == lay) && AppointmentsIntersect(appt, processedAppt))
                            {
								// If we find an intersection we update the current
								// appt's layer and move on to the next layer
								// looking for a further intersection
								appt.Layer = (lay + 1);
								intersect = true;

                                break;
                            }
                        }

						// If we looked through all the previously processed 
						// appointments within this layer without finding
						// an intersection then we can stop looking
                        if (!intersect)
                            break;
                    }
                }

                processed.Add(appt);
				layers.Add(appt.Layer);
			}

            return layers.Count;
        }

        protected void DrawLongAppointments(Graphics g, Rectangle daysRect)
        {
            AppointmentList appointments = (AppointmentList)cachedAppointments[-1];

            int numLayers = RecalcAppointmentLayers(appointments);

            if (numLayers == 0)
                return;

            g.SetClip(daysRect);

            int dayWidth = (daysRect.Width / daysToShow);
            int y = DayHeaderHeight;
            longAppointmentViews.Clear();

            allDayEventsHeaderHeight = ((numLayers * (longAppointmentHeight + longAppointmentSpacing)) + longAppointmentSpacing);

            Rectangle backRectangle = daysRect;
            backRectangle.Y = y;
            backRectangle.Height = allDayEventsHeaderHeight;

            renderer.DrawAllDayBackground(g, backRectangle);

            var endOfLastDay = EndDate.AddSeconds(-1);

            foreach (Appointment appt in appointments)
            {
                Rectangle apptRect = daysRect;

                double startDay = (appt.StartDate - startDate).TotalDays;
                int startPos = (daysRect.X + (int)(startDay * dayWidth));

                if (startPos <= daysRect.Left)
                    startPos = (daysRect.Left + 2);

                double endDay = (appt.EndDate - startDate).TotalDays;
                int endPos = (daysRect.X + (int)(endDay * dayWidth));

                if ((endPos >= daysRect.Right) || (appt.EndDate >= endOfLastDay))
                    endPos = (daysRect.Right - 2);

                apptRect.X = startPos;
                apptRect.Width = (endPos - startPos);

                apptRect.Y = y + (appt.Layer * (longAppointmentHeight + longAppointmentSpacing)) + longAppointmentSpacing;
                apptRect.Height = longAppointmentHeight;

                Rectangle gripRect = apptRect;
                gripRect.Width = appointmentGripWidth;

				var apptView = NewAppointmentView(appt);
				longAppointmentViews[appt] = apptView;

				apptView.Rectangle = apptRect;
				apptView.GripRect = gripRect;
				apptView.IsLong = true;

				renderer.DrawAppointment(g, daysRect, apptView, WantDrawAppointmentSelected(appt));
			}

			// Draw a vertical line to close off the long appointments on the left
            g.DrawLine(SystemPens.ControlDark, backRectangle.Left, backRectangle.Top, backRectangle.Left, daysRect.Bottom);

			g.ResetClip();
		}

		protected virtual bool WantDrawAppointmentSelected(Calendar.Appointment appt)
		{
			return (!SavingToImage && (activeTool != drawTool) && (appt == selectedAppointment));
		}

		#endregion

        #region Events

		public event AppointmentEventHandler SelectionChanged;
        public event ResolveAppointmentsEventHandler ResolveAppointments;
        public event NewAppointmentEventHandler NewAppointment;
        public event AppointmentEventHandler AppointmentMove;
		public event WeekChangeEventHandler WeekChange;

		#endregion


		public bool CanSaveToImage()
		{
			ResolveAppointmentsEventArgs args = new ResolveAppointmentsEventArgs(StartDate, EndDate);
			OnResolveAppointments(args);

			return ((args.Appointments != null) && (args.Appointments.Count > 0));
		}

		virtual protected Size CalculateImageSize()
		{
			AppointmentList temp = (AppointmentList)cachedAppointments[-1];
			int numLayers = RecalcAppointmentLayers(temp);

			Size image = new Size(0, 0);

			image.Height += DayHeaderHeight;
			image.Height += ((numLayers * (longAppointmentHeight + longAppointmentSpacing)) + longAppointmentSpacing);
			image.Height += ((VisibleEndHour - VisibleStartHour) * slotsPerHour * slotHeight);

			int dayWidth = ((ClientRectangle.Width - (minHourLabelWidth + hourLabelIndent)) / DaysShowing);
			image.Width = (minHourLabelWidth + hourLabelIndent + (DaysShowing * dayWidth));

			return image;
		}

		public Bitmap SaveToImage()
		{
			var imageRect = new Rectangle(new Point(0, 0), CalculateImageSize());

			// Cache and reset vertical scrollbar as this influences drawing
			int prevVScrollValue = vscroll.Value;
			int prevVScrollWidth = vscroll.Width;

			vscroll.Value = 0;
			vscroll.Width = 0;

			// The output image
			Bitmap image = new Bitmap(imageRect.Width, imageRect.Height, PixelFormat.Format32bppRgb);

			using (Graphics graphics = Graphics.FromImage(image))
			{
				SavingToImage = true;

				DoPaint(new PaintEventArgs(graphics, imageRect), imageRect);

				SavingToImage = false;
			}

			vscroll.Value = prevVScrollValue;
			vscroll.Width = prevVScrollWidth;

			return image;
		}

	}
}
