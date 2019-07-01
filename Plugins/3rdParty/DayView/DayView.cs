using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;
using System.Drawing;
using System.Globalization;

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

        private DateTime workStart;
        private DateTime workEnd;

        #endregion

        #region Constants

        protected int hourLabelWidth = 50;
        protected int hourLabelIndent = 2;
        protected int dayHeadersHeight = 19;
        protected int appointmentGripWidth = 5;
        protected int allDayEventsHeaderHeight = 0;
        protected int headerBorder = 2;
        protected int longAppointmentSpacing = 2;
        
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
            tooltip.SetToolTip(hscroll, "Change Week");

            drawTool = new DrawTool();
            drawTool.DayView = this;

            selectionTool = new SelectionTool();
            selectionTool.DayView = this;
            selectionTool.Complete += new EventHandler(selectionTool_Complete);

            activeTool = drawTool;

            UpdateWorkingHours();

            this.Renderer = new Office12Renderer();
        }

        #endregion

        #region Properties

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

        private int daysToShow = 1;

        [System.ComponentModel.DefaultValue(1)]
        public int DaysToShow
        {
            get
            {
                return daysToShow;
            }
            set
            {
                daysToShow = value;
                OnDaysToShowChanged();
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
                // Move start date to start of week
				DateTime firstDayOfWeek = FirstDayOfWeekUtility.GetFirstDayOfWeek(value);

				if (startDate != firstDayOfWeek)
				{
					startDate = firstDayOfWeek;
					OnStartDateChanged();
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
							selectedAppointment = appt;

                            // Initialise selection tool
                            ActiveTool = selectionTool;
							break;
						}
					}
				}

				Refresh();
			}
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

        private int workingHourStart = 8;

        [System.ComponentModel.DefaultValue(8)]
        public int WorkingHourStart
        {
            get
            {
                return workingHourStart;
            }
            set
            {
                workingHourStart = value;
                UpdateWorkingHours();
            }
        }

        private int workingMinuteStart = 30;

        [System.ComponentModel.DefaultValue(30)]
        public int WorkingMinuteStart
        {
            get
            {
                return workingMinuteStart;
            }
            set
            {
                workingMinuteStart = value;
                UpdateWorkingHours();
            }
        }

        private int workingHourEnd = 18;

        [System.ComponentModel.DefaultValue(18)]
        public int WorkingHourEnd
        {
            get
            {
                return workingHourEnd;
            }
            set
            {
                workingHourEnd = value;
                UpdateWorkingHours();
            }
        }

        private int workingMinuteEnd = 30;

        [System.ComponentModel.DefaultValue(30)]
        public int WorkingMinuteEnd
        {
            get { return workingMinuteEnd; }
            set
            {
                workingMinuteEnd = value;
                UpdateWorkingHours();
            }
        }

        private int slotsPerHour = 4;

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
				case 1: return true;
				case 2: return true;
				case 3: return true;
				case 4: return true;
				case 6: return true;
				case 12: return true;
			}

			return false;
		}

		private void UpdateWorkingHours()
        {
            workStart = new DateTime(1, 1, 1, workingHourStart, workingMinuteStart, 0);

            if (workingHourEnd >= 24)
                workEnd = new DateTime(1, 1, 2, 0, 0, 0);
            else
                workEnd = new DateTime(1, 1, 1, workingHourEnd, workingMinuteEnd, 0);

            Invalidate();
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

        private int HeaderHeight
        {
            get
            {
                return dayHeadersHeight + allDayEventsHeaderHeight + headerBorder;
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
                StartDate = StartDate.AddDays(7);
            }
            else if (e.NewValue < e.OldValue)
            {
                StartDate = StartDate.AddDays(-7);
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

            base.OnMouseDown(e);
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
				ScrollMe(e.Delta < 0);
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

        public void ScrollMe(bool down)
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
            numSlots = Math.Max(0, Math.Min((24 * slotsPerHour), numSlots));

            int minutes = (int)((60 * numSlots) / slotsPerHour);

            int minsPerSlot = (60 / slotsPerHour);
            int lastSlotMins = ((24 * 60) - minsPerSlot);

            if (minutes > (lastSlotMins + (minsPerSlot / 2)))
                return new TimeSpan(1, 0, 0, 0);

            // nearest slot
            minutes = ((int)numSlots * minsPerSlot);

            return new TimeSpan((minutes / 60), (minutes % 60), 0);
        }

        public Appointment GetAppointmentAt(int x, int y)
        {

            foreach (AppointmentView view in appointmentViews.Values)
                if (view.Rectangle.Contains(x, y))
                    return view.Appointment;

            foreach (AppointmentView view in longAppointmentViews.Values)
                if (view.Rectangle.Contains(x, y))
                    return view.Appointment;

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

            Rectangle headerRectangle = rectangle;

            headerRectangle.X += hourLabelWidth + hourLabelIndent;
            headerRectangle.Width -= (hourLabelWidth + hourLabelIndent);
            headerRectangle.Height = dayHeadersHeight;

            if (e.ClipRectangle.IntersectsWith(headerRectangle))
                DrawDayHeaders(e, headerRectangle);

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

        private void DrawHourLabels(PaintEventArgs e, Rectangle rect)
        {
            e.Graphics.SetClip(rect);

            for (int m_Hour = 0; m_Hour < 24; m_Hour++)
            {
                Rectangle hourRectangle = rect;

                hourRectangle.Y = rect.Y + (m_Hour * slotsPerHour * slotHeight) - vscroll.Value;
                hourRectangle.X += hourLabelIndent;
                hourRectangle.Width = hourLabelWidth;

                renderer.DrawHourLabel(e.Graphics, hourRectangle, m_Hour, this.ampmdisplay);

                for (int slot = 0; slot < slotsPerHour; slot++)
                {
                    bool hour = ((slot % slotsPerHour) == 0);
                    renderer.DrawMinuteLine(e.Graphics, hourRectangle, hour);
                    hourRectangle.Y += slotHeight;
                }
            }

            e.Graphics.ResetClip();

            // draw a line at the top for closure
            using (Pen m_Pen = new Pen(Color.DarkGray))
                e.Graphics.DrawLine(m_Pen, rect.Left, rect.Y, rect.Width, rect.Y);
        }

        private void DrawDayHeaders(PaintEventArgs e, Rectangle rect)
        {
            int dayWidth = rect.Width / daysToShow;

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

        private Rectangle GetHourRangeRectangle(DateTime start, DateTime end, Rectangle baseRectangle)
        {
            Rectangle rect = baseRectangle;

            int startY;
            int endY;

            startY = (start.Hour * slotHeight * slotsPerHour) + ((start.Minute * slotHeight) / (60 / slotsPerHour));
            endY = (end.Hour * slotHeight * slotsPerHour) + ((end.Minute * slotHeight) / (60 / slotsPerHour));

            rect.Y = startY - vscroll.Value + this.HeaderHeight;
            rect.Height = System.Math.Max(1, endY - startY);

            return rect;
        }

        private void DrawDay(PaintEventArgs e, Rectangle rect, DateTime time)
        {
            renderer.DrawDayBackground(e.Graphics, rect);

            Rectangle workingHoursRectangle = GetHourRangeRectangle(workStart, workEnd, rect);

            if (workingHoursRectangle.Y < this.HeaderHeight)
            {
                workingHoursRectangle.Height -= this.HeaderHeight - workingHoursRectangle.Y;
                workingHoursRectangle.Y = this.HeaderHeight;
            }

            if (!weekendDays.Contains(time.DayOfWeek))
                renderer.DrawHourRange(e.Graphics, workingHoursRectangle, false, false);

            if ((selection == SelectionType.DateRange) && (time.Day == selectionStart.Day))
            {
                Rectangle selectionRectangle = GetHourRangeRectangle(selectionStart, selectionEnd, rect);

                if ((selectionRectangle.Height > 1) && (selectionRectangle.Top + 1 > this.HeaderHeight))
                {
                    renderer.DrawHourRange(e.Graphics, selectionRectangle, false, true);
                }
            }

            e.Graphics.SetClip(rect);

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

            renderer.DrawDayGripper(e.Graphics, rect, appointmentGripWidth);

            e.Graphics.ResetClip();

            AppointmentList appointments = (AppointmentList)cachedAppointments[time.Day];

            if (appointments != null)
            {
                List<string> groups = new List<string>();

                foreach (Appointment app in appointments)
                    if (!groups.Contains(app.Group))
                        groups.Add(app.Group);

                Rectangle rect2 = rect;
                rect2.X += (appointmentGripWidth + 2);

                rect2.Width -= appointmentGripWidth;
                rect2.Width = (rect2.Width / groups.Count) - 2;

                groups.Sort();

                foreach (string group in groups)
                {
                    DrawAppointments(e, rect2, time, group);

                    rect2.X += rect2.Width;
                }
            }
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

        private void DrawAppointments(PaintEventArgs e, Rectangle rect, DateTime time, string group)
        {
            DateTime timeStart = time.Date;
            DateTime timeEnd = timeStart.AddHours(24);
            timeEnd = timeEnd.AddSeconds(-1);

            AppointmentList appointments = (AppointmentList)cachedAppointments[time.Day];

            if (appointments != null)
            {
                HalfHourLayout[] layout = GetMaxParallelAppointments(appointments, this.slotsPerHour);

                List<Appointment> drawnItems = new List<Appointment>();

                for (int halfHour = 0; halfHour < 24 * slotsPerHour; halfHour++)
                {
                    HalfHourLayout hourLayout = layout[halfHour];

                    if ((hourLayout != null) && (hourLayout.Count > 0))
                    {
                        for (int appIndex = 0; appIndex < hourLayout.Count; appIndex++)
                        {
                            Appointment appointment = hourLayout.Appointments[appIndex];

                            if (appointment.Group != group)
                                continue;

                            if (drawnItems.IndexOf(appointment) < 0)
                            {
                                Rectangle appRect = rect;
                                int appointmentWidth;
                                AppointmentView view;

                                appointmentWidth = rect.Width / appointment.conflictCount;

                                int lastX = 0;

                                foreach (Appointment app in hourLayout.Appointments)
                                {
                                    if ((app != null) && (app.Group == appointment.Group) && (appointmentViews.ContainsKey(app)))
                                    {
                                        view = appointmentViews[app];

                                        if (lastX < view.Rectangle.X)
                                            lastX = view.Rectangle.X;
                                    }
                                }

                                if ((lastX + (appointmentWidth * 2)) > (rect.X + rect.Width))
                                    lastX = 0;

                                appRect.Width = appointmentWidth/* - 5*/;

                                if (lastX > 0)
                                    appRect.X = lastX + appointmentWidth;

                                DateTime appstart = appointment.StartDate;
                                DateTime append = appointment.EndDate;

                                // Draw the appts boxes depending on the height display mode                           

                                // If small appts are to be drawn in half-hour blocks
                                if (this.AppHeightMode == AppHeightDrawMode.FullHalfHourBlocksShort && appointment.EndDate.Subtract(appointment.StartDate).TotalMinutes < (60 / slotsPerHour))
                                {
                                    // Round the start/end time to the last/next halfhour
                                    appstart = appointment.StartDate.AddMinutes(-appointment.StartDate.Minute);
                                    append = appointment.EndDate.AddMinutes((60 / slotsPerHour) - appointment.EndDate.Minute);

                                    // Make sure we've rounded it to the correct halfhour :)
                                    if (appointment.StartDate.Minute >= (60 / slotsPerHour))
                                        appstart = appstart.AddMinutes((60 / slotsPerHour));
                                    if (appointment.EndDate.Minute > (60 / slotsPerHour))
                                        append = append.AddMinutes((60 / slotsPerHour));
                                }

                                // This is basically the same as previous mode, but for all appts
                                else if (this.AppHeightMode == AppHeightDrawMode.FullHalfHourBlocksAll)
                                {
                                    appstart = appointment.StartDate.AddMinutes(-appointment.StartDate.Minute);
                                    if (appointment.EndDate.Minute != 0 && appointment.EndDate.Minute != (60 / slotsPerHour))
                                        append = appointment.EndDate.AddMinutes((60 / slotsPerHour) - appointment.EndDate.Minute);
                                    else
                                        append = appointment.EndDate;

                                    if (appointment.StartDate.Minute >= (60 / slotsPerHour))
                                        appstart = appstart.AddMinutes((60 / slotsPerHour));
                                    if (appointment.EndDate.Minute > (60 / slotsPerHour))
                                        append = append.AddMinutes((60 / slotsPerHour));
                                }

                                // Based on previous code
                                else if (this.AppHeightMode == AppHeightDrawMode.EndHalfHourBlocksShort && appointment.EndDate.Subtract(appointment.StartDate).TotalMinutes < (60 / slotsPerHour))
                                {
                                    // Round the end time to the next halfhour
                                    append = appointment.EndDate.AddMinutes((60 / slotsPerHour) - appointment.EndDate.Minute);

                                    // Make sure we've rounded it to the correct halfhour :)
                                    if (appointment.EndDate.Minute > (60 / slotsPerHour))
                                        append = append.AddMinutes((60 / slotsPerHour));
                                }

                                else if (this.AppHeightMode == AppHeightDrawMode.EndHalfHourBlocksAll)
                                {
                                    // Round the end time to the next halfhour
                                    if (appointment.EndDate.Minute != 0 && appointment.EndDate.Minute != (60 / slotsPerHour))
                                        append = appointment.EndDate.AddMinutes((60 / slotsPerHour) - appointment.EndDate.Minute);
                                    else
                                        append = appointment.EndDate;
                                    // Make sure we've rounded it to the correct halfhour :)
                                    if (appointment.EndDate.Minute > (60 / slotsPerHour))
                                        append = append.AddMinutes((60 / slotsPerHour));
                                }

                                appRect = GetHourRangeRectangle(appstart, append, appRect);

                                view = new AppointmentView();
                                view.Rectangle = appRect;
                                view.Appointment = appointment;

                                appointmentViews[appointment] = view;

                                e.Graphics.SetClip(rect);

                                if (this.DrawAllAppBorder)
                                    appointment.DrawBorder = true;

                                // Procedure for gripper rectangle is always the same
                                Rectangle gripRect = GetHourRangeRectangle(appointment.StartDate, appointment.EndDate, appRect);
                                gripRect.Width = appointmentGripWidth;

                                DrawAppointment(e.Graphics, appRect, appointment, appointment == selectedAppointment, gripRect);

                                e.Graphics.ResetClip();

                                drawnItems.Add(appointment);
                            }
                        }
                    }
                }
            }
        }

        private static HalfHourLayout[] GetMaxParallelAppointments(List<Appointment> appointments, int slotsPerHour)
        {
            HalfHourLayout[] appLayouts = new HalfHourLayout[24 * 20];

            foreach (Appointment appointment in appointments)
            {
                appointment.conflictCount = 1;
            }

            foreach (Appointment appointment in appointments)
            {
                int firstHalfHour = appointment.StartDate.Hour * slotsPerHour + (appointment.StartDate.Minute / (60 / slotsPerHour));
                int lastHalfHour = appointment.EndDate.Hour * slotsPerHour + (appointment.EndDate.Minute / (60 / slotsPerHour));

                // Added to allow small parts been displayed
                if (lastHalfHour == firstHalfHour)
                {
                    if (lastHalfHour < 24 * slotsPerHour)
                        lastHalfHour++;
                    else
                        firstHalfHour--;
                }

                for (int halfHour = firstHalfHour; halfHour < lastHalfHour; halfHour++)
                {
                    HalfHourLayout layout = appLayouts[halfHour];

                    if (layout == null)
                    {
                        layout = new HalfHourLayout();
                        layout.Appointments = new Appointment[20];
                        appLayouts[halfHour] = layout;
                    }

                    layout.Appointments[layout.Count] = appointment;

                    layout.Count++;

                    List<string> groups = new List<string>();

                    foreach (Appointment app2 in layout.Appointments)
                    {
                        if ((app2 != null) && (!groups.Contains(app2.Group)))
                            groups.Add(app2.Group);
                    }

                    layout.Groups = groups;

                    // update conflicts
                    foreach (Appointment app2 in layout.Appointments)
                    {
                        if ((app2 != null) && (app2.Group == appointment.Group))
                            if (app2.conflictCount < layout.Count)
                                app2.conflictCount = layout.Count - (layout.Groups.Count - 1);
                    }
                }
            }

            return appLayouts;
        }

        private void DrawDays(PaintEventArgs e, Rectangle rect)
        {
            AppointmentList longAppointments = (AppointmentList)cachedAppointments[-1];
            AppointmentList drawnLongApps = new AppointmentList();
            AppointmentView view;

            int y = dayHeadersHeight;
            bool intersect = false;

            List<int> layers = new List<int>();

			int dayWidth = rect.Width / daysToShow;
			longAppointmentViews.Clear();

            if (longAppointments != null)
            {
                foreach (Appointment appointment in longAppointments)
                {
                    appointment.Layer = 0;

                    if (drawnLongApps.Count != 0)
                    {
                        foreach (Appointment app in drawnLongApps)
                            if (!layers.Contains(app.Layer))
                                layers.Add(app.Layer);

                        foreach (int lay in layers)
                        {
                            foreach (Appointment app in drawnLongApps)
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

                    drawnLongApps.Add(appointment); // changed by Gimlei
                }

                foreach (Appointment app in drawnLongApps)
                    if (!layers.Contains(app.Layer))
                        layers.Add(app.Layer);

				allDayEventsHeaderHeight = ((layers.Count * (longAppointmentHeight + longAppointmentSpacing)) + longAppointmentSpacing);

                Rectangle backRectangle = rect;
                backRectangle.Y = y;
                backRectangle.Height = allDayEventsHeaderHeight;

                renderer.DrawAllDayBackground(e.Graphics, backRectangle);
				e.Graphics.SetClip(backRectangle);

				var endOfLastDay = EndDate.AddSeconds(-1);

                foreach (Appointment appointment in longAppointments)
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

					appointmenRect.Y = y + (appointment.Layer * (longAppointmentHeight + longAppointmentSpacing)) + longAppointmentSpacing; // changed by Gimlei
                    appointmenRect.Height = longAppointmentHeight;

                    view = new AppointmentView();
                    view.Rectangle = appointmenRect;
                    view.Appointment = appointment;

                    longAppointmentViews[appointment] = view;

                    Rectangle gripRect = appointmenRect;
                    gripRect.Width = appointmentGripWidth;

                    DrawAppointment(e.Graphics, appointmenRect, appointment, (appointment == selectedAppointment), gripRect);
                }

				e.Graphics.SetClip(rect);
            }

            DateTime time = startDate;
            Rectangle rectangle = rect;
            rectangle.Width = dayWidth;
            rectangle.Y += allDayEventsHeaderHeight;
            rectangle.Height -= allDayEventsHeaderHeight;

            appointmentViews.Clear();
            layers.Clear();

            for (int day = 0; day < daysToShow; day++)
            {
                // last day takes up any slack
                if (day == (daysToShow - 1))
                    rectangle.Width = (rect.Right - rectangle.Left);

                DrawDay(e, rectangle, time);

                rectangle.X += dayWidth;

                time = time.AddDays(1);
            }
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
            hscroll.Width = hourLabelWidth;
            hscroll.Height = dayHeadersHeight;
        }

        class HalfHourLayout
        {
            public int Count;
            public List<string> Groups;
            public Appointment[] Appointments;
        }

        internal class AppointmentView
        {
            public Appointment Appointment;
            public Rectangle Rectangle;
        }

        class AppointmentList : List<Appointment>
        {
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
