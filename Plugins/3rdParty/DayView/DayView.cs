using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;
using System.Drawing;
using System.Globalization;
using System.Linq;

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

	[System.ComponentModel.DesignerCategory("")]
	public class DayView : Control
    {
        #region Variables

        private TextBox editbox;
        private VScrollBar vscroll;
        private HScrollBar hscroll;
        private ToolTip tooltip;
        private DrawTool drawTool;
        private SelectionTool selectionTool;

        #endregion

        #region Constants

        protected int hourLabelWidth = 50;
        protected int hourLabelIndent = 2;
        protected int dayHeadersHeight = 19;
        protected int appointmentGripWidth = 5;
        protected int dayGripWidth = 5;
        protected int allDayEventsHeaderHeight = 0;
        protected int longAppointmentSpacing = 2;
        protected int appointmentSpacing = 1;
        protected int groupSpacing = 1;
        protected int daySpacing = 1;

        static protected int minSlotHeight = 5;

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
            StartDate = DateTime.Now;

            SetStyle(ControlStyles.OptimizedDoubleBuffer, true);
			SetStyle(ControlStyles.UserPaint, true);
			SetStyle(ControlStyles.AllPaintingInWmPaint, true);
            SetStyle(ControlStyles.ResizeRedraw, true);
            SetStyle(ControlStyles.Selectable, true);

            vscroll = new VScrollBar();
            vscroll.Dock = DockStyle.Right;
            vscroll.Visible = allowScroll;
            vscroll.Scroll += new ScrollEventHandler(OnVScroll);
            AdjustScrollbar();
			vscroll.Value = 0;

            this.Controls.Add(vscroll);

            hscroll = new HScrollBar();
            hscroll.Visible = true;
            hscroll.Location = new System.Drawing.Point(0, 0);
            hscroll.Width = hourLabelWidth;
            hscroll.Height = dayHeadersHeight;
            hscroll.Scroll += new ScrollEventHandler(OnHScroll);
            hscroll.Minimum = -1000; // ~-20 years
            hscroll.Maximum = 1000;  // ~20 years
            hscroll.Value = 0;

            this.Controls.Add(hscroll);

            editbox = new TextBox();
            editbox.Multiline = true;
            editbox.Visible = false;
            editbox.ScrollBars = ScrollBars.Vertical;
            editbox.BorderStyle = BorderStyle.None;
            editbox.KeyUp += new KeyEventHandler(editbox_KeyUp);
            editbox.Margin = Padding.Empty;

            this.Controls.Add(editbox);

            tooltip = new ToolTip();

            drawTool = new DrawTool();
            drawTool.DayView = this;

            selectionTool = new SelectionTool();
            selectionTool.DayView = this;
            selectionTool.Complete += new EventHandler(selectionTool_Complete);

            activeTool = drawTool;

            this.Renderer = new Office12Renderer();
        }

		#endregion

		#region Properties

		public Boolean IsResizingAppointment()
		{
			return ((activeTool == selectionTool) && selectionTool.IsResizing);
		}

		public void CancelAppointmentResizing()
		{
			if (activeTool == selectionTool)
				selectionTool.Reset();
		}

		public String HScrollTooltipText
		{
			get { return tooltip.GetToolTip(hscroll); }
			set { tooltip.SetToolTip(hscroll, value); }
		}

		private AppHeightDrawMode appHeightMode = AppHeightDrawMode.TrueHeightAll;

        public AppHeightDrawMode AppHeightMode
        {
            get { return appHeightMode; }
            set { appHeightMode = value; }
        }

		private int slotHeight = minSlotHeight;

        public int SlotHeight
        {
            get
            {
                return slotHeight;
            }
            set
            {
                slotHeight = ((value < minSlotHeight) ? minSlotHeight : value);
                OnSlotHeightChanged();
            }
        }

        private void OnSlotHeightChanged()
        {
            AdjustScrollbar();
            Invalidate();
        }

        private AbstractRenderer renderer;

        [System.ComponentModel.Browsable(false)]
        [System.ComponentModel.DesignerSerializationVisibility(System.ComponentModel.DesignerSerializationVisibility.Hidden)]
        public AbstractRenderer Renderer
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
            this.Font = renderer.BaseFont;
            this.Invalidate();
        }

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
            this.Invalidate();
        }

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
            this.Invalidate();
        }

        private bool minHalfHourApp = false;

        public bool MinHalfHourApp
        {
            get
            {
                return minHalfHourApp;
            }
            set
            {
                minHalfHourApp = value;
                Invalidate();
            }
        }

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
                    daysToShow = value; // must come first

                    if (daysToShow == 7)
                    {
                        HScrollTooltipText = "Change Week";

                        // Move to start of week
                        StartDate = startDate;
                    }
                    else if (daysToShow == 1) // day view
                    {
                        HScrollTooltipText = "Change Day";

                        // make sure the day for the selected task is visible
                        if (SelectedAppointment != null)
                            StartDate = SelectedAppointment.StartDate;
                    }
                    else
                    {
                        HScrollTooltipText = "Change";
                    }

                    EnsureVisible(SelectedAppointment, true);
                    OnDaysToShowChanged();
                    AdjustScrollbar();
                }
            }
        }

        protected virtual void OnDaysToShowChanged()
        {
            if (this.CurrentlyEditing)
                FinishEditing(true);

            Invalidate();
        }

        private SelectionType selection;

        [System.ComponentModel.Browsable(false)]
        public SelectionType Selection
        {
            get
            {
                return selection;
            }
        }

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
					{
						startDate = firstDayOfWeek;
						OnStartDateChanged();
					}
				}
				else
				{
					startDate = value.Date;
				}
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
            selection = SelectionType.DateRange;

            Invalidate();
			RaiseWeekChange(new WeekChangeEventArgs(StartDate));
        }

        private int startHour = 8;

        [System.ComponentModel.DefaultValue(8)]
        public int StartHour
        {
            get
            {
                return startHour;
            }
            set
            {
                startHour = value;
                OnStartHourChanged();
            }
        }

        protected virtual void OnStartHourChanged()
        {
            if ((startHour * slotsPerHour * slotHeight) > vscroll.Maximum) //maximum is lower on larger forms
            {
                vscroll.Value = vscroll.Maximum;
            }
            else
            {
                vscroll.Value = (startHour * slotsPerHour * slotHeight);
            }

            Invalidate();
        }

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
				else // Validate against visible items
				{
					if (ResolveAppointments == null)
						return;

					ResolveAppointmentsEventArgs args = new ResolveAppointmentsEventArgs(this.StartDate, this.EndDate);
					ResolveAppointments(this, args);

					foreach (Appointment appt in args.Appointments)
					{
						if (appt == value)
						{
                            // Initialise selection tool
							selectedAppointment = appt;
							selection = SelectionType.Appointment;
                            ActiveTool = selectionTool;

							// Ensure at least part of the task is in view
							EnsureVisible(appt, true);
							break;
						}
					}
				}

				Refresh();
			}
        }

		public bool EnsureVisible(Appointment appt, bool partialOK)
		{
			if (appt == null)
				return false;

			if (appt.StartDate == DateTime.MinValue)
				return false;

			if (!appt.IsLongAppt())
			{
                if ((appt.StartDate < StartDate) || (appt.StartDate >= EndDate))
				    StartDate = appt.StartDate;

				// Ensure at least part of the task is in view
				int startHour = (vscroll.Value / (slotsPerHour * slotHeight));
				int endHour = (startHour + (vscroll.LargeChange / (slotsPerHour * slotHeight)));

				if (partialOK)
				{
					if (appt.StartDate.Hour > endHour)
					{
						ScrollToHour(appt.EndDate.Hour - 1);
					}
					else if (appt.EndDate.Hour < startHour)
					{
						ScrollToHour(appt.StartDate.Hour + 1);
					}
				}
				else
				{
					if (appt.StartDate.Hour < startHour)
					{
						ScrollToHour(appt.StartDate.Hour);
					}
					else if (appt.EndDate.Hour > endHour)
					{
						ScrollToHour(appt.EndDate.Hour - endHour);
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

        private DateTime selectionStart;

        public DateTime SelectionStart
        {
            get { return selectionStart; }
            set { selectionStart = value; }
        }

        private DateTime selectionEnd;

        public DateTime SelectionEnd
        {
            get { return selectionEnd; }
            set { selectionEnd = value; }
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
            set { workStart = value; Invalidate(); }
        }


        public HourMin WorkEnd
        {
            get { return workEnd; }
            set { workEnd = value; Invalidate(); }
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
            this.vscroll.Visible = this.AllowScroll;
            this.vscroll.Enabled = this.AllowScroll;
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
                return dayHeadersHeight + allDayEventsHeaderHeight;
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

            if (editbox.Visible)
                //scroll text box too
                editbox.Top += e.OldValue - e.NewValue;
        }

        void OnHScroll(object sender, ScrollEventArgs e)
        {
            if (e.NewValue > e.OldValue)
            {
                StartDate = StartDate.AddDays(daysToShow);
            }
            else if (e.NewValue < e.OldValue)
            {
                StartDate = StartDate.AddDays(-daysToShow);
            }

			AdjustScrollbar();
            Invalidate();
			RaiseWeekChange(new WeekChangeEventArgs(StartDate));
        }

        protected override void SetBoundsCore(int x, int y, int width, int height, BoundsSpecified specified)
        {
            base.SetBoundsCore(x, y, width, height, specified);
            AdjustScrollbar();
        }

        protected void AdjustScrollbar()
        {
			if (this.Height < this.HeaderHeight)
				return;

			// Auto-calculate best 'hour' height
			int availHeight = (this.Height - this.HeaderHeight);
			slotHeight = ((availHeight / (24 * slotsPerHour)) + 1);

			if (slotHeight < minSlotHeight)
				slotHeight = minSlotHeight;

			int oneHour = (slotHeight * slotsPerHour);
			AllowScroll = ((oneHour * 24) > availHeight);

			vscroll.Minimum = 0;
			vscroll.SmallChange = oneHour;

			if (AllowScroll)
			{
				vscroll.Maximum = (oneHour * 24);
				vscroll.LargeChange = availHeight;

				if (vscroll.Value > (vscroll.Maximum - vscroll.LargeChange))
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
			start = this.StartDate;
			end = this.EndDate;
		}

        protected override void OnPaintBackground(PaintEventArgs pevent)
        {
            // Flicker free
        }

        protected override void OnMouseDown(MouseEventArgs e)
        {
			// Ignore all clicks on the day header
			if (e.Y > dayHeadersHeight)
			{
				if (e.Button == MouseButtons.Left)
				{
					// Capture focus
					this.Focus();

					if (CurrentlyEditing)
					{
						FinishEditing(false);
					}

					if (selectedAppointmentIsNew)
					{
						RaiseNewAppointment();
					}

					ITool newTool = null;
					Appointment appointment = GetAppointmentAt(e.X, e.Y);

					if (appointment == null)
					{
						if (e.Y < HeaderHeight && e.Y > dayHeadersHeight)
						{
							newTool = drawTool;
							selection = SelectionType.None;

							base.OnMouseDown(e);
							return;
						}

						newTool = drawTool;
						selection = SelectionType.DateRange;
					}
					else
					{
						newTool = selectionTool;
						selectedAppointment = appointment;
						selection = SelectionType.Appointment;

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
					Appointment appointment = GetAppointmentAt(e.X, e.Y);
					bool redraw = false;

					if (appointment == null)
					{
						if (selectedAppointment != null)
							redraw = true;

						selectedAppointment = null;
						selection = SelectionType.Appointment;
						RaiseSelectionChanged(new AppointmentEventArgs(null));

						DateTime click = GetDateTimeAt(e.X, e.Y);
						selection = SelectionType.DateRange;

						if ((click < SelectionStart) || (click > SelectionEnd))
						{
							SelectionStart = new DateTime(click.Year, click.Month, click.Day, click.Hour, 0, 0);
							SelectionEnd = SelectionStart.AddMinutes(60);

							redraw = true;
						}
					}
					else if (appointment != selectedAppointment)
					{
						selectedAppointment = appointment;
						selection = SelectionType.Appointment;

						RaiseSelectionChanged(new AppointmentEventArgs(selectedAppointment));
						redraw = true;
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

            this.allDayEventsHeaderHeight = 0;

            // cache resolved appointments in hashtable by days.
            cachedAppointments.Clear();

            if ((selectedAppointmentIsNew) && (selectedAppointment != null))
            {
                if ((selectedAppointment.StartDate > args.StartDate) && (selectedAppointment.StartDate < args.EndDate))
                {
                    args.Appointments.Add(selectedAppointment);
                }
            }

            foreach (Appointment appointment in args.Appointments)
            {
                int key;
                AppointmentList list;

				bool longAppt = appointment.IsLongAppt();

				// If a long appointment is being resized and it now fits within a 
				// single day, still treat it as a long task until the edit finishes
				if (!longAppt && (appointment == selectedAppointment) && (activeTool != null))
				{
					Calendar.SelectionTool selTool = activeTool as Calendar.SelectionTool;

					if (selTool != null)
						longAppt = selTool.IsResizingLongAppt();
				}
				
				if (!longAppt)
                {
                    key = appointment.StartDate.Day;
                }
                else
                {
                    key = -1;
                }

                list = (AppointmentList)cachedAppointments[key];

                if (list == null)
                {
                    list = new AppointmentList();
                    cachedAppointments[key] = list;
                }

                list.Add(appointment);
            }
        }

        protected void RaiseSelectionChanged(Appointment appt)
        {
            RaiseSelectionChanged(new AppointmentEventArgs(appt));
        }

		internal void RaiseSelectionChanged(AppointmentEventArgs e)
        {
            if (SelectionChanged != null)
                SelectionChanged(this, e);
        }

        internal void RaiseAppointmentMove(AppointmentEventArgs e)
        {
            if (AppointmentMove != null)
                AppointmentMove(this, e);
        }

		internal void RaiseWeekChange(WeekChangeEventArgs e)
		{
			if (WeekChange != null)
				WeekChange(this, e);
		}

        protected override void OnKeyPress(KeyPressEventArgs e)
        {
            if ((allowNew) && char.IsLetterOrDigit(e.KeyChar))
            {
                if ((this.Selection == SelectionType.DateRange))
                {
                    if (!selectedAppointmentIsNew)
                        EnterNewAppointmentMode(e.KeyChar);
                }
            }
        }

        private void EnterNewAppointmentMode(char key)
        {
            Appointment appointment = new Appointment();
            appointment.StartDate = selectionStart;
            appointment.EndDate = selectionEnd;
            appointment.Title = key.ToString();

            selectedAppointment = appointment;
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

            if (this.InvokeRequired)
            {
                Appointment selectedApp = selectedAppointment;

                System.Threading.Thread.Sleep(200);

                if (selectedApp == selectedAppointment)
                    this.Invoke(new StartEditModeDelegate(EnterEditMode), state);
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
            this.vscroll.Value = this.vscroll.Minimum;
            this.Invalidate();
        }
		public bool ScrollToHour(int hour) // 0-23
		{
			if (hour < 0 || hour > 23)
				return false;

			vscroll.Value = (hour * slotsPerHour * slotHeight);
			Invalidate();

			return true;
		}

		public void MouseWheelScroll(bool down)
        {
            if (this.AllowScroll)
            {
                if (down)
                {
					int maxRange = (this.vscroll.Maximum - this.vscroll.LargeChange);

					// mouse wheel scroll down
                    int newScrollValue = this.vscroll.Value + this.vscroll.SmallChange;

					if (newScrollValue < maxRange)
                        this.vscroll.Value = newScrollValue;
                    else
						this.vscroll.Value = maxRange;
                }
                else
                {
					// mouse wheel scroll up
                    int newScrollValue = this.vscroll.Value - this.vscroll.SmallChange;

                    if (newScrollValue > this.vscroll.Minimum)
                        this.vscroll.Value = newScrollValue;
                    else
                        this.vscroll.Value = this.vscroll.Minimum;
                }

                this.Invalidate();
            }
        }

        public Rectangle GetTrueRectangle()
        {
            Rectangle truerect;

            truerect = this.ClientRectangle;
            truerect.X += hourLabelWidth + hourLabelIndent;
            truerect.Width -= vscroll.Width + hourLabelWidth + hourLabelIndent;
            truerect.Y += this.HeaderHeight;
            truerect.Height -= this.HeaderHeight;

            return truerect;
        }

        public Rectangle GetFullDayApptsRectangle()
        {
            Rectangle fulldayrect;
            fulldayrect = this.ClientRectangle;
            fulldayrect.Height = this.HeaderHeight - dayHeadersHeight;
            fulldayrect.Y += dayHeadersHeight;
            fulldayrect.Width -= (hourLabelWidth + hourLabelIndent + this.vscroll.Width);
            fulldayrect.X += hourLabelWidth + hourLabelIndent;

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
            this.Focus();
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
            int dayWidth = (this.Width - (vscroll.Width + hourLabelWidth + hourLabelIndent)) / daysToShow;

			if (longAppt)
			{
				int hours = ((24 * (x - hourLabelWidth)) / dayWidth);
				date = date.AddHours(hours);
			}
			else
			{
				date = date.AddDays((x - hourLabelWidth) / dayWidth).Date;

				if (date < startDate)
					date = startDate;

				if (date >= EndDate)
					date = EndDate.AddDays(-1);
			}

            return date;
        }

        public virtual TimeSpan GetTimeAt(int y)
        {
            double numSlots = (y - this.HeaderHeight + vscroll.Value) / (double)slotHeight;

			// Clip at top and bottom
			int maxSlots = (24 * slotsPerHour);
            numSlots = Math.Max(0, Math.Min(maxSlots, numSlots));

            // nearest slot
            //int minutes = (int)((60 * numSlots) / slotsPerHour);
            int minsPerSlot = (60 / slotsPerHour);
            int minutes = ((int)numSlots * minsPerSlot);

            return new TimeSpan((minutes / 60), (minutes % 60), 0);
        }

        public Appointment GetAppointmentAt(int x, int y)
        {
			if (GetFullDayApptsRectangle().Contains(x, y))
			{
				foreach (AppointmentView view in longAppointmentViews.Values)
					if (view.Rectangle.Contains(x, y))
						return view.Appointment;
			}
			else
			{
				foreach (AppointmentView view in appointmentViews.Values)
					if (view.Rectangle.Contains(x, y))
						return view.Appointment;
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

            // resolve appointments on visible date range.
            ResolveAppointmentsEventArgs args = new ResolveAppointmentsEventArgs(this.StartDate, this.EndDate);
            OnResolveAppointments(args);

            using (SolidBrush backBrush = new SolidBrush(renderer.BackColor))
                e.Graphics.FillRectangle(backBrush, this.ClientRectangle);

            e.Graphics.SmoothingMode = System.Drawing.Drawing2D.SmoothingMode.AntiAlias;
			e.Graphics.TextRenderingHint = System.Drawing.Text.TextRenderingHint.AntiAliasGridFit;

            // Calculate visible rectangle
            Rectangle rectangle = new Rectangle(0, 0, this.Width - vscroll.Width, this.Height);

            Rectangle headerRectangle = rectangle;
            headerRectangle.X += hourLabelWidth + hourLabelIndent;
            headerRectangle.Width -= (hourLabelWidth + hourLabelIndent);
            headerRectangle.Height = dayHeadersHeight;

            if (e.ClipRectangle.IntersectsWith(headerRectangle))
                DrawDayHeaders(e, headerRectangle);

            Rectangle daysRectangle = rectangle;
            daysRectangle.X += hourLabelWidth + hourLabelIndent;
            daysRectangle.Y += this.HeaderHeight;
            daysRectangle.Width -= (hourLabelWidth + hourLabelIndent);

            if (e.ClipRectangle.IntersectsWith(daysRectangle))
            {
                DrawDays(e, daysRectangle);
            }

            Rectangle hourLabelRectangle = rectangle;

            hourLabelRectangle.Y += this.HeaderHeight;

            DrawHourLabels(e, hourLabelRectangle);

            Rectangle scrollrect = rectangle;

            if (this.AllowScroll == false)
            {
                scrollrect.X = headerRectangle.Width + hourLabelWidth + hourLabelIndent;
                scrollrect.Width = vscroll.Width;
                using (SolidBrush backBrush = new SolidBrush(renderer.BackColor))
                    e.Graphics.FillRectangle(backBrush, scrollrect);
            }

			AdjustScrollbar();
        }

		protected void DrawHourLabels(PaintEventArgs e, Rectangle rect)
        {
            e.Graphics.SetClip(rect);

            for (int m_Hour = 0; m_Hour < 24; m_Hour++)
            {
                Rectangle hourRectangle = rect;

                hourRectangle.Y = rect.Y + (m_Hour * slotsPerHour * slotHeight) - vscroll.Value;
                hourRectangle.X += hourLabelIndent;
				hourRectangle.Width = hourLabelWidth - 1;

                var minuteRect = hourRectangle;
                minuteRect.Height = slotHeight;

                int interval = (60 / slotsPerHour);

                for (int minute = 0; minute < 60; minute += interval)
                {
                    renderer.DrawMinuteLine(e.Graphics, minuteRect, minute);
                    minuteRect.Y += slotHeight;
                }

                renderer.DrawHourLabel(e.Graphics, hourRectangle, m_Hour, this.ampmdisplay);
            }

            e.Graphics.ResetClip();

            // draw a line at the top for closure
            using (Pen m_Pen = new Pen(Color.DarkGray))
                e.Graphics.DrawLine(m_Pen, rect.Left, rect.Y, rect.Width, rect.Y);
        }

		protected void DrawDayHeaders(PaintEventArgs e, Rectangle rect)
        {
            int dayWidth = (rect.Width / daysToShow);

            // one day header rectangle
            Rectangle dayHeaderRectangle = new Rectangle(rect.Left, rect.Top, dayWidth, rect.Height);
            DateTime headerDate = startDate;

            for (int day = 0; day < daysToShow; day++)
            {
                // Take up any leftover from rounding on day 0
                if (day == 0)
                {
                    int day0Width = (rect.Width - ((daysToShow - 1) * dayWidth));
                    dayHeaderRectangle.Width = day0Width;
                }

                renderer.DrawDayHeader(e.Graphics, dayHeaderRectangle, headerDate);

                dayHeaderRectangle.X += dayWidth;
                headerDate = headerDate.AddDays(1);
            }
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

		protected Rectangle GetHourRangeRectangle(DateTime start, DateTime end, Rectangle baseRectangle)
        {
			Rectangle rect = Rectangle.Empty;

			if (start < end)
			{
				int startY = (start.Hour * slotHeight * slotsPerHour) + ((start.Minute * slotHeight) / (60 / slotsPerHour));

				// Special case: end time is 'end of day'
				if (end == start.Date.AddDays(1))
					end = end.AddSeconds(-1);

				int endY = (end.Hour * slotHeight * slotsPerHour) + ((end.Minute * slotHeight) / (60 / slotsPerHour));

				rect = baseRectangle;
				rect.Y = startY - vscroll.Value + this.HeaderHeight;
				rect.Height = System.Math.Max(1, endY - startY);
			}

			return rect;
        }

        protected void DrawWorkHours(PaintEventArgs e, HourMin start, HourMin end, Rectangle rect)
        {
            Rectangle hoursRect = GetHourRangeRectangle(start, end, rect);

            if (hoursRect.Y < this.HeaderHeight)
            {
                hoursRect.Height -= this.HeaderHeight - hoursRect.Y;
                hoursRect.Y = this.HeaderHeight;
            }

            renderer.DrawHourRange(e.Graphics, hoursRect, false, false);
        }

		protected void DrawDaySelection(PaintEventArgs e, Rectangle rect, DateTime time)
		{
			if ((selection == SelectionType.DateRange) && (time.Day == selectionStart.Day))
			{
				Rectangle selectionRectangle = GetHourRangeRectangle(selectionStart, selectionEnd, rect);

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
			for (int hour = 0; hour < 24 * slotsPerHour; hour++)
			{
				int y = rect.Top + (hour * slotHeight) - vscroll.Value;

				Color color1 = renderer.HourSeperatorColor;
				Color color2 = renderer.HalfHourSeperatorColor;

				using (Pen pen = new Pen(((hour % slotsPerHour) == 0 ? color1 : color2)))
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
			AppointmentList appointments = (AppointmentList)cachedAppointments[time.Day];

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

        internal Dictionary<Appointment, AppointmentView> appointmentViews = new Dictionary<Appointment, AppointmentView>();
        internal Dictionary<Appointment, AppointmentView> longAppointmentViews = new Dictionary<Appointment, AppointmentView>();

        public bool GetAppointmentRect(Appointment appointment, ref Rectangle rect)
        {
            if (appointment == null)
                return false;

            AppointmentView view;

			// Short appointments
            if (appointmentViews.TryGetValue(appointment, out view))
            {
                rect = view.Rectangle;
                return true;
            }

			// Long appointments
			if (longAppointmentViews.TryGetValue(appointment, out view))
			{
				rect = view.Rectangle;
				return true;
			}

            return false;
        }

        private void DrawDayGroupAppointments(PaintEventArgs e, Rectangle rect, DateTime time, string group)
        {
            AppointmentList appointments = (AppointmentList)cachedAppointments[time.Day];

            if (appointments == null)
                return;

            var groupAppts = new AppointmentList(appointments.Where(appt => (appt.Group == group)));

            if ((groupAppts == null) || (groupAppts.Count() == 0))
                return;

            int numLayers = RecalcAppointmentLayers(groupAppts);

            if (numLayers == 0)
                return;

            int layerWidth = (rect.Width / numLayers);
            int leftOverWidth = (rect.Width - (layerWidth * numLayers));

            foreach (var appointment in groupAppts)
            {
                Rectangle apptRect = rect;
                apptRect.X += (appointment.Layer * layerWidth);
                apptRect.Width = (layerWidth - ((numLayers > 1) ? appointmentSpacing : 0));

                // Last column picks up the slack
                if (appointment.Layer == (numLayers - 1))
                    apptRect.Width += leftOverWidth;

                DateTime apptStart, apptEnd;
                GetAppointmentDrawTimes(appointment, out apptStart, out apptEnd);

                apptRect = GetHourRangeRectangle(apptStart, apptEnd, apptRect);
                appointmentViews[appointment] = new AppointmentView(appointment, apptRect);

                if (this.DrawAllAppBorder)
                    appointment.DrawBorder = true;

                Rectangle gripRect = GetHourRangeRectangle(appointment.StartDate, appointment.EndDate, apptRect);
                gripRect.Width = appointmentGripWidth;

                bool isSelected = ((activeTool != drawTool) && (appointment == selectedAppointment));
                DrawAppointment(e.Graphics, apptRect, appointment, isSelected, gripRect);
            }
        }

        private void GetAppointmentDrawTimes(Appointment appointment, out DateTime apptStart, out DateTime apptEnd)
        {
            apptStart = appointment.StartDate;
            apptEnd = appointment.EndDate;

            // Adjust depending on the height display mode 
            switch (this.AppHeightMode)
            {
                case AppHeightDrawMode.FullHalfHourBlocksShort:
                    if (appointment.EndDate.Subtract(appointment.StartDate).TotalMinutes < (60 / slotsPerHour))
                    {
                        // Round the start/end time to the last/next halfhour
                        apptStart = appointment.StartDate.AddMinutes(-appointment.StartDate.Minute);
                        apptEnd = appointment.EndDate.AddMinutes((60 / slotsPerHour) - appointment.EndDate.Minute);

                        // Make sure we've rounded it to the correct halfhour :)
                        if (appointment.StartDate.Minute >= (60 / slotsPerHour))
                            apptStart = apptStart.AddMinutes((60 / slotsPerHour));

                        if (appointment.EndDate.Minute > (60 / slotsPerHour))
                            apptEnd = apptEnd.AddMinutes((60 / slotsPerHour));
                    }
                    break;

                case AppHeightDrawMode.FullHalfHourBlocksAll:
                    {
                        apptStart = appointment.StartDate.AddMinutes(-appointment.StartDate.Minute);

                        if (appointment.EndDate.Minute != 0 && appointment.EndDate.Minute != (60 / slotsPerHour))
                            apptEnd = appointment.EndDate.AddMinutes((60 / slotsPerHour) - appointment.EndDate.Minute);
                        else
                            apptEnd = appointment.EndDate;

                        if (appointment.StartDate.Minute >= (60 / slotsPerHour))
                            apptStart = apptStart.AddMinutes((60 / slotsPerHour));

                        if (appointment.EndDate.Minute > (60 / slotsPerHour))
                            apptEnd = apptEnd.AddMinutes((60 / slotsPerHour));
                    }
                    break;

                // Based on previous code
                case AppHeightDrawMode.EndHalfHourBlocksShort:
                    if (appointment.EndDate.Subtract(appointment.StartDate).TotalMinutes < (60 / slotsPerHour))
                    {
                        // Round the end time to the next halfhour
                        apptEnd = appointment.EndDate.AddMinutes((60 / slotsPerHour) - appointment.EndDate.Minute);

                        // Make sure we've rounded it to the correct halfhour :)
                        if (appointment.EndDate.Minute > (60 / slotsPerHour))
                            apptEnd = apptEnd.AddMinutes((60 / slotsPerHour));
                    }
                    break;

                case AppHeightDrawMode.EndHalfHourBlocksAll:
                    {
                        // Round the end time to the next halfhour
                        if (appointment.EndDate.Minute != 0 && appointment.EndDate.Minute != (60 / slotsPerHour))
                            apptEnd = appointment.EndDate.AddMinutes((60 / slotsPerHour) - appointment.EndDate.Minute);
                        else
                            apptEnd = appointment.EndDate;

                        // Make sure we've rounded it to the correct halfhour :)
                        if (appointment.EndDate.Minute > (60 / slotsPerHour))
                            apptEnd = apptEnd.AddMinutes((60 / slotsPerHour));
                    }
                    break;
            }
        }

        private void DrawDays(PaintEventArgs e, Rectangle rect)
        {
            DrawLongAppointments(e.Graphics, rect);

            // Draw the day appointments
            int dayWidth = rect.Width / daysToShow;
            appointmentViews.Clear();

            DateTime time = startDate;
            Rectangle rectangle = rect;
            rectangle.Width = dayWidth;
            rectangle.Y += allDayEventsHeaderHeight;
            rectangle.Height -= allDayEventsHeaderHeight;

            for (int day = 0; day < daysToShow; day++)
            {
                // last day takes up any slack
                if (day == (daysToShow - 1))
                    rectangle.Width = (rect.Right - rectangle.Left);

				e.Graphics.SetClip(rectangle);

				DrawDay(e, rectangle, time);

				e.Graphics.ResetClip();

                rectangle.X += dayWidth;
				time = time.AddDays(1);
            }
        }

        private int RecalcAppointmentLayers(AppointmentList appointments)
        {
            if ((appointments == null) || (appointments.Count == 0))
                return 0;

            AppointmentList processed = new AppointmentList();
            List<int> layers = new List<int>();

            foreach (Appointment appointment in appointments)
            {
                appointment.Layer = 0;

                if (processed.Count != 0)
                {
                    bool intersect = false;

                    foreach (Appointment appt in processed)
                    {
                        if (!layers.Contains(appt.Layer))
                            layers.Add(appt.Layer);
                    }

                    foreach (int lay in layers)
                    {
                        foreach (Appointment app in processed)
                        {
                            if (app.Layer == lay)
                            {
                                if (appointment.StartDate >= app.EndDate || appointment.EndDate <= app.StartDate)
                                {
                                    intersect = false;
                                }
                                else
                                {
                                    intersect = true;
                                    break;
                                }
                            }

                            appointment.Layer = lay;
                        }

                        if (!intersect)
                            break;
                    }

                    if (intersect)
                        appointment.Layer = layers.Count;
                }

                processed.Add(appointment);
            }

            foreach (Appointment app in processed)
                if (!layers.Contains(app.Layer))
                    layers.Add(app.Layer);

            return layers.Count;
        }

        private void DrawLongAppointments(Graphics g, Rectangle rect)
        {
            AppointmentList appointments = (AppointmentList)cachedAppointments[-1];

            int numLayers = RecalcAppointmentLayers(appointments);

            if (numLayers == 0)
                return;

            int dayWidth = rect.Width / daysToShow;
            int y = dayHeadersHeight;
            longAppointmentViews.Clear();

            allDayEventsHeaderHeight = ((numLayers * (longAppointmentHeight + longAppointmentSpacing)) + longAppointmentSpacing);

            Rectangle backRectangle = rect;
            backRectangle.Y = y;
            backRectangle.Height = allDayEventsHeaderHeight;

            renderer.DrawAllDayBackground(g, backRectangle);
            g.SetClip(backRectangle);

            var endOfLastDay = EndDate.AddSeconds(-1);

            foreach (Appointment appointment in appointments)
            {
                Rectangle appointmenRect = rect;

                double startDay = (appointment.StartDate - startDate).TotalDays;
                int startPos = (rect.X + (int)(startDay * dayWidth));

                if (startPos <= rect.Left)
                    startPos = (rect.Left + 2);

                double endDay = (appointment.EndDate - startDate).TotalDays;
                int endPos = (rect.X + (int)(endDay * dayWidth));

                if ((endPos >= rect.Right) || (appointment.EndDate >= endOfLastDay))
                    endPos = (rect.Right - 1);

                appointmenRect.X = startPos;
                appointmenRect.Width = (endPos - startPos);

                appointmenRect.Y = y + (appointment.Layer * (longAppointmentHeight + longAppointmentSpacing)) + longAppointmentSpacing;
                appointmenRect.Height = longAppointmentHeight;

                longAppointmentViews[appointment] = new AppointmentView(appointment, appointmenRect);

                Rectangle gripRect = appointmenRect;
                gripRect.Width = appointmentGripWidth;

                bool isSelected = ((activeTool != drawTool) && (appointment == selectedAppointment));
                DrawAppointment(g, appointmenRect, appointment, isSelected, gripRect);
            }

            // Draw a vertical line to close off the long appointments on the left
            using (Pen m_Pen = new Pen(Color.DarkGray))
                g.DrawLine(m_Pen, backRectangle.Left, backRectangle.Top, backRectangle.Left, rect.Bottom);

            g.SetClip(rect);
        }

        protected virtual void DrawAppointment(Graphics g, Rectangle rect, Appointment appointment, bool isSelected, Rectangle gripRect)
		{
			// Pass draw rect as the clip rect by default
			renderer.DrawAppointment(g, rect, appointment, isSelected, gripRect);
		}
		
        #endregion

        #region Internal Utility Classes

        protected void RefreshHScrollSize()
        {
            hscroll.Width = (hourLabelWidth + hourLabelIndent);
            hscroll.Height = dayHeadersHeight;
        }

        internal class AppointmentView
        {
            public AppointmentView(Appointment appt, Rectangle rect)
            {
                Appointment = appt;
                Rectangle = rect;
            }

            public Appointment Appointment;
            public Rectangle Rectangle;
        }

        class AppointmentList : List<Appointment>
        {
            public AppointmentList() : base() { }
            public AppointmentList(IEnumerable<Appointment> appts) : base(appts) { }
        }

        #endregion

        #region Events

		public event AppointmentEventHandler SelectionChanged;
        public event ResolveAppointmentsEventHandler ResolveAppointments;
        public event NewAppointmentEventHandler NewAppointment;
        public event AppointmentEventHandler AppointmentMove;
		public event WeekChangeEventHandler WeekChange;

        #endregion
    }
}
