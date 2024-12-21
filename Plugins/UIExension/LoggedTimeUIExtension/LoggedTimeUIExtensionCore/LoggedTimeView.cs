using System;
using System.Collections.Generic;
using System.Drawing;
using System.Windows.Forms;
using System.Diagnostics;
using System.IO;
using System.Threading;

using Abstractspoon.Tdl.PluginHelpers;

namespace LoggedTimeUIExtension
{
	// ------------------------------------------------------------------------------


// 	public delegate void TDLAppointmentEventHandler(object sender, TDLMoveAppointmentEventArgs args);
	public delegate bool TDLContextMenuEventHandler(object sender, MouseEventArgs args);

	public class LogAccessEventArgs
	{
		public bool Success;
		public bool Loading;
	}

	public delegate void LogAccessStatusEventHandler(object sender, LogAccessEventArgs args);

	// ------------------------------------------------------------------------------

	public partial class LoggedTimeView : Calendar.DayView, ILabelTipHandler
	{
		private uint m_SelectedLogEntryId = 0;
		private int m_UserMinSlotHeight = -1;

		private TaskItems m_TaskItems;
		private LogEntries m_LogEntries;
		private LogEntry m_CachedLogEntry;

		private string m_TasklistPath = @"\."; // something valid
		private FileSystemWatcher m_LogFileWatcher;
		private bool m_LastLogAccessSucceeded = true;

//		private DateSortedTasks m_DateSortedTasks;

		//private TDLRenderer m_Renderer;
		private LabelTip m_LabelTip;
		private Translator m_Trans;
		private UIThemeToolbarRenderer m_ToolbarRenderer;

		private int LabelTipBorder { get { return DPIScaling.Scale(4); } }

		// ----------------------------------------------------------------

		public new event TDLContextMenuEventHandler ContextMenu;
		public event LogAccessStatusEventHandler LogAccessStatusChanged;

		// ----------------------------------------------------------------

		public LoggedTimeView(Translator trans, UIExtension.TaskIcon taskIcons, UIExtension.TaskRecurrences taskRecurrences, int minSlotHeight)
		{
			minHourLabelWidth = DPIScaling.Scale(minHourLabelWidth);
			hourLabelIndent = DPIScaling.Scale(hourLabelIndent);
			minDayHeaderHeight = DPIScaling.Scale(minDayHeaderHeight);
			longAppointmentSpacing = DPIScaling.Scale(longAppointmentSpacing);
			dayGripWidth = 1; // to match app styling

			m_Trans = trans;
			m_RenderHelper.TaskIcons = taskIcons;
			m_ToolbarRenderer = new UIThemeToolbarRenderer();
			m_UserMinSlotHeight = minSlotHeight;
			m_LabelTip = new LabelTip(this);

			m_LogEntries = new LogEntries();
			m_TaskItems = new TaskItems();
//			m_DateSortedTasks = new DateSortedTasks(m_LogEntries);

// 			base.NotifyDayWidth += new Calendar.DayWidthEventHandler(OnNotifyDayWidth);

			// Create a 5 minute timer for updating the line indicating time of day 'today'
			var timer = new System.Windows.Forms.Timer()
			{
				Enabled = true,
				Interval = (1000 * 5 * 60)
			};
			timer.Tick += (s, e) => { UpdateTodayTime(); };

			InitializeComponent();
		}

		public IEnumerable<TaskItem> TaskItems { get { return m_TaskItems.Values; } }
		public UIExtension.TaskIcon TaskIcons { get { return m_RenderHelper.TaskIcons; } }

		public TaskItem GetTask(uint taskId) { return m_TaskItems.GetItem(taskId); }
		public Control GetOwner() { return this; } // ILabelTipHandler implementation
		
		public bool ReadOnly;
		public bool ForceShowSelection;

		protected override bool WantDrawDaySelection { get { return base.WantDrawDaySelection || ForceShowSelection; } }

		public LabelTipInfo ToolHitTest(Point ptScreen)
		{
			return null;
/*
			if (IsResizingAppointment())
				return null;

			var tip = new LabelTipInfo()
			{
				Font = BaseFont(),
			};

			var pt = PointToClient(ptScreen);
			var tdlView = (GetAppointmentViewAt(pt.X, pt.Y, out tip.Rect) as TDLAppointmentView);

			if (tdlView == null)
				return null;

			bool startPortion = (tip.Rect.Right < tdlView.Rectangle.Right);

			tip.Rect.Offset(startPortion ? tdlView.TextHorzOffset : 0, TextOffset);

			var appt = tdlView.Appointment;
			tip.Id = appt.Id;

			if (appt is TaskExtensionItem)
			{
				// NOTE: - Must match 'Calendar' View in 'Core' project
				if (appt is TaskFutureOccurrence)
				{
					tip.Text = m_Trans.Translate("Future Occurrence", Translator.Type.ToolTip);
				}
				else if (appt is TaskCustomDateAttribute)
				{
					var apptDate = (appt as TaskCustomDateAttribute);
					var custAttrib = m_CustomDateDefs.Find(x => (x.Id == apptDate.AttributeId));

					tip.Text = string.Format(m_Trans.Translate("{0} (Custom)", Translator.Type.ToolTip), custAttrib.Label);
				}
				else if (appt is TaskTimeBlock)
				{
					tip.Text = m_Trans.Translate("Time Block", Translator.Type.ToolTip);
				}
				else
				{
					Debug.Assert(false);
				}

				var pos = PointToClient(MousePosition);
				pos.Offset(0, ToolStripEx.GetActualCursorHeight(Cursor));

				tip.Rect.Location = pos;
				tip.InitialDelay = 500;
			}
			else // 'Real' task
			{
				tip.Text = appt.Title;

				if (IsLongAppt(appt))
				{
					// single line tooltips
					Size tipSize = m_LabelTip.CalcTipSize(tip.Text, tip.Font, tip.Rect.Width);

					if ((tipSize.Width <= tip.Rect.Width) && (tipSize.Height <= tip.Rect.Height))
						return null;

					tip.MultiLine = false; // always
				}
				else
				{
					var availRect = GetTrueRectangle();

					if (tip.Rect.Top < availRect.Top)
					{
						// If the top of the text rectangle is hidden we always 
						// need a label tip so we just clip to the avail space
						tip.Rect.Intersect(availRect);
					}
					else
					{
						// Determine if text will fit in what's visible of the task
						tip.Rect.Intersect(availRect);

						Size tipSize = m_LabelTip.CalcTipSize(tip.Text, tip.Font, tip.Rect.Width);

						if ((tipSize.Width <= tip.Rect.Width) && (tipSize.Height <= tip.Rect.Height))
							return null;
					}

					tip.MultiLine = true; // always
				}
			}

			// Inflate the rect now we've done all our calculations
			tip.Rect.Inflate(TextPadding, TextPadding);

			return tip;
*/
		}

		public uint IconHitTest(Point ptScreen)
		{
			return 0;
/*
			var pt = PointToClient(ptScreen);
			Calendar.Appointment appt = GetAppointmentAt(pt.X, pt.Y);

			if (appt == null)
				return 0;

			var apptView = (GetAppointmentView(appt) as TDLAppointmentView);

			if ((apptView == null) || !apptView.IconRect.Contains(pt))
				return 0;

			return apptView.Appointment.Id;
*/
		}

		protected override void WndProc(ref Message m)
		{
			if (m_LabelTip != null)
				m_LabelTip.ProcessMessage(m);

			base.WndProc(ref m);
		}

// 		protected override Calendar.SelectionTool NewSelectionTool()
// 		{
// 			return new TDLSelectionTool();
// 		}
// 
// 		protected override Calendar.AppointmentView NewAppointmentView(Calendar.Appointment appt)
// 		{
// 			return new TDLAppointmentView(appt);
// 		}

		protected void InitializeComponent()
		{
			Calendar.DrawTool drawTool = new Calendar.DrawTool();
			drawTool.DayView = this;

			ActiveTool = drawTool;
			AllowInplaceEditing = true;
			AllowNew = false;
			AmPmDisplay = true;
			Anchor = (System.Windows.Forms.AnchorStyles.Bottom |
									 System.Windows.Forms.AnchorStyles.Left |
									 System.Windows.Forms.AnchorStyles.Right);
 			AppHeightMode = Calendar.DayView.AppHeightDrawMode.TrueHeightAll;
			DrawAllAppBorder = false;
			Location = new Point(0, 0);
			MinHalfHourApp = false;
			Name = "m_dayView";
			Renderer = this;
			Size = new Size(798, 328);
			SlotsPerHour = 4;
			TabIndex = 0;
			Text = "m_dayView";
			ReadOnly = false;

			ResolveAppointments += new Calendar.ResolveAppointmentsEventHandler(OnResolveAppointments);
			SelectionChanged += new Calendar.AppointmentEventHandler(OnSelectionChanged);
			WeekChange += new Calendar.WeekChangeEventHandler(OnWeekChanged);
 			AppointmentMove += new Calendar.AppointmentEventHandler(OnAppointmentChanged);
		}

		public bool ShowLabelTips
		{
			set { m_LabelTip.Active = value; }
			get { return m_LabelTip.Active; }
		}

		public TaskItem SelectedLogEntryTaskItem
		{
			get
			{
				var entry = SelectedLogEntry;

				return (entry == null ? null : m_TaskItems.GetItem(entry.TaskId));
			}
		}

		public LogEntry SelectedLogEntry
		{
			get { return m_LogEntries.GetEntry(m_SelectedLogEntryId); }
		}

		public bool CanAddNewLogEntry
		{
			get
			{
				if (!m_LastLogAccessSucceeded)
					return false;

				// Must have a valid tasklist path -> valid log file path
				// and there must be no pending modifications
				return !string.IsNullOrEmpty(m_TasklistPath)/* && !m_LogEntries.IsModified*/;
			}
		}

		public bool CanModifySelectedLogEntry
		{
			get { return !ReadOnly && m_LastLogAccessSucceeded && m_LogEntries.HasEntry(m_SelectedLogEntryId); }
		}

		public bool CanDeleteSelectedLogEntry
		{
			get { return CanModifySelectedLogEntry; }
		}
		
		public bool AddNewLogEntry(TaskItem taskItem, Calendar.AppointmentDates dates, double timeSpentInHrs, string comment, string path, string type, Color fillColor)
		{
			if (!CanAddNewLogEntry)
				return false;

			if (!dates.IsValid)
				return false;

			if (dates.IsLongAppt)
				return false;

			uint taskId = ((taskItem == null) ? 0 : taskItem.Id);

			var newEntry = new TaskTimeLogEntry()
			{
				TaskId = taskId,
				From = dates.Start,
				To = dates.End,
				TimeInHours = timeSpentInHrs,
				TaskTitle = taskItem?.Title,
				Comment = comment,
				Person = Environment.UserName,
				TaskPath = path,
				Type = type,
				AltColor = fillColor
			};

			// Temporarily disable file watcher
			m_LogFileWatcher.EnableRaisingEvents = false;

			bool success = TaskTimeLog.Add(m_TasklistPath, newEntry, false);

			m_LogFileWatcher.EnableRaisingEvents = true;
			
			if (success)
			{
				m_LogEntries.AddEntry(new LogEntry(0, newEntry));
				Invalidate();
			}

			HandleLogAccessResult(true, success);

			return success;
		}

		public bool ModifySelectedLogEntry(Calendar.AppointmentDates dates, double timeSpentInHrs, string comment, Color fillColor)
		{
			if (!CanModifySelectedLogEntry)
				return false;

			CacheSelectedLogEntry();

			var entry = m_LogEntries.GetEntry(m_SelectedLogEntryId);

			if (!entry.Modify(dates, timeSpentInHrs, comment, fillColor))
			{
				ClearCachedLogEntry();
				return false;
			}

			Invalidate();

			return SaveLogFile();
		}

		protected void OnAppointmentChanged(object sender, Calendar.AppointmentEventArgs e)
		{
			var me = (e as Calendar.MoveAppointmentEventArgs);

			if (me != null)
			{
				Debug.Assert(e.Appointment.Id == m_SelectedLogEntryId);

				switch (me.State)
				{
				case Calendar.SelectionTool.State.Started:
					CacheSelectedLogEntry();
					break;

				case Calendar.SelectionTool.State.Finished:
					Debug.Assert(e.Appointment.Id == m_CachedLogEntry.Id);
					SaveLogFile();
					break;
				}
			}
		}

		public bool DeleteSelectedLogEntry()
		{
			if (!CanDeleteSelectedLogEntry)
				return false;

			CacheSelectedLogEntry();

			if (!m_LogEntries.DeleteEntry(m_SelectedLogEntryId))
				Debug.Assert(false);

			if (!SaveLogFile())
				return false;

			// else
			m_SelectedLogEntryId = 0;
			Invalidate();

			return true;
		}

		public bool CacheSelectedLogEntry()
		{
			Debug.Assert(m_SelectedLogEntryId != 0);

			var entry = m_LogEntries.GetEntry(m_SelectedLogEntryId);

			if (entry == null)
			{
				Debug.Assert(false);
				return false;
			}

			m_CachedLogEntry = new LogEntry(m_SelectedLogEntryId, entry);
			return true;
		}

		public bool RestoreCachedLogEntry()
		{
			if (m_CachedLogEntry == null)
			{
				Debug.Assert(false);
				return false;
			}

			m_LogEntries.DeleteEntry(m_CachedLogEntry.Id);
			m_LogEntries.AddEntry(m_CachedLogEntry);

			m_SelectedLogEntryId = m_CachedLogEntry.Id;

			ClearCachedLogEntry();
			Invalidate();

			return true;
		}

		public void ClearCachedLogEntry()
		{
			m_CachedLogEntry = null;
		}
		
		public bool GetSelectedTaskDates(out DateTime from, out DateTime to)
		{
			from = to = Calendar.Appointment.NullDate;

// 			uint selTaskID = SelectedTaskId;
// 
// 			if (selTaskID == 0)
// 				return false;
// 
// 			LogEntry item = m_LogEntries.GetItem(selTaskID);
// 
// 			if ((item == null) || !item.HasValidDates())
// 				return false;
// 
// 			from = item.StartDate;
// 			to = item.EndDate;

			return true;
		}

		public bool GetTask(UIExtension.GetTask getTask, ref uint taskID)
		{
			bool forwards = true, topLevel = false;

			switch (getTask)
			{
			case UIExtension.GetTask.GetNextTask:
			case UIExtension.GetTask.GetNextVisibleTask:
				break;

			case UIExtension.GetTask.GetPrevTask:
			case UIExtension.GetTask.GetPrevVisibleTask:
				forwards = false;
				break;

			case UIExtension.GetTask.GetNextTopLevelTask:
				topLevel = true;
				break;

			case UIExtension.GetTask.GetPrevTopLevelTask:
				forwards = false;
				topLevel = true;
				break;
			}

/*
			var sortedTasks = m_DateSortedTasks.Items;
			int item = sortedTasks.NextIndex(taskID, forwards);

			while (item != -1)
			{
				var task = m_DateSortedTasks.Items[item];

				if (IsItemDisplayable(task))
				{
					if (!topLevel || task.IsTopLevel)
					{
						taskID = task.Id;
						return true;
					}
				}
				item = sortedTasks.NextIndex(item, forwards);
			}
*/

			// all else
			return false;
		}

		public Calendar.Appointment FixupSelection(bool scrollToTask, bool allowNotify)
		{
			// Our base class clears the selected appointment whenever
			// the week changes so we can't always rely on 'SelectedAppointmentId'
			LogEntry item = m_LogEntries.GetEntry(m_SelectedLogEntryId);

			if (item != null)
			{
				if (scrollToTask)
				{
					if (!IsItemWithinRange(item, StartDate, EndDate))
						StartDate = item.StartDate;

					SelectedAppointment = item;
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

			return SelectedAppointment;
		}

		public void GoToToday()
        {
            StartDate = DateTime.Now;

			// And scroll vertically to first log entry
			var appointments = GetMatchingAppointments(StartDate, EndDate);

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

		public bool AppointmentSupportsTaskContextMenu(Calendar.Appointment appt)
		{
			return ((appt != null) && (appt is LogEntry));
		}

		public UIExtension.HitTestResult HitTest(Int32 xScreen, Int32 yScreen, UIExtension.HitTestReason reason)
		{
			Point pt = PointToClient(new Point(xScreen, yScreen));
			Calendar.Appointment appt = GetAppointmentAt(pt.X, pt.Y);

			if ((appt != null) && GetTrueRectangle().Contains(pt))
				return UIExtension.HitTestResult.Tasklist;

			// else
			return UIExtension.HitTestResult.Nowhere;
		}

		private bool IsTodayVisible
		{
			get
			{
				return IsTodayInRange(StartDate, EndDate);
			}
		}

		private bool IsTodayInRange(DateTime start, DateTime end)
		{
			var today = DateTime.Now.Date;
			return ((today >= start) && (today < end));
		}

		private bool IsItemWithinRange(Calendar.Appointment appt, DateTime startDate, DateTime endDate)
		{
			// sanity check
			if (!appt.HasValidDates())
				return false;

			// Task must at least intersect the range
			if ((appt.StartDate >= endDate) || (appt.EndDate <= startDate))
				return false;

            return true;
		}

		public void SavePreferences(Preferences prefs, String key)
		{
		}

		public void LoadPreferences(Preferences prefs, String key)
		{
		}

		///////////////////////////////////////////////////////////
		// Idle processing

		bool m_WantIdleReload = false;

		public bool DoIdleProcessing()
		{
			if (m_WantIdleReload)
			{
				m_WantIdleReload = false;
				ReloadLogFile();
			}

			return false; // No more tasks
		}

		private static bool IsSamePath(string path1, string path2)
		{
			return (string.Compare(Path.GetFullPath(path1), Path.GetFullPath(path2), StringComparison.InvariantCultureIgnoreCase) == 0);
		}

		private void OnLogFileModified(object sender, FileSystemEventArgs e)
		{
			if (IsSamePath(TaskTimeLog.GetPath(m_TasklistPath), e.FullPath))
				m_WantIdleReload = true;
		}

		private void ReloadLogFile()
		{
			bool success = m_LogEntries.Load(m_TasklistPath);

			HandleLogAccessResult(true, success);
			Invalidate();
		}

		private bool SaveLogFile()
		{
			// Temporarily disable file watcher
			m_LogFileWatcher.EnableRaisingEvents = false;

			bool success = m_LogEntries.SaveLogFile(m_TasklistPath);
			HandleLogAccessResult(false, success);

			m_LogFileWatcher.EnableRaisingEvents = true;

			if (success)
				ClearCachedLogEntry();
			else
				RestoreCachedLogEntry();

			return success;
		}

		private void HandleLogAccessResult(bool loading, bool success)
		{
			if (success != m_LastLogAccessSucceeded)
			{
				LogAccessStatusChanged?.Invoke(this, new LogAccessEventArgs()
				{
					Loading = loading,
					Success = success
				});
			}

			m_LastLogAccessSucceeded = success;
			m_WantIdleReload |= !success;
		}

		///////////////////////////////////////////////////////////

		public void UpdateTasks(TaskList tasks,	UIExtension.UpdateType type)
		{
			string tasklistPath = tasks.GetFilePath();

			if (!IsSamePath(tasklistPath, m_TasklistPath))
			{
				m_TasklistPath = tasklistPath;

				if (m_LogFileWatcher == null)
				{
					m_LogFileWatcher = new FileSystemWatcher();

					m_LogFileWatcher.Filter = "*.csv";
					m_LogFileWatcher.NotifyFilter = NotifyFilters.LastWrite;
					m_LogFileWatcher.Changed += new FileSystemEventHandler(OnLogFileModified);
				}

				m_LogFileWatcher.Path = Path.GetDirectoryName(m_TasklistPath);
				m_LogFileWatcher.EnableRaisingEvents = true;

				ReloadLogFile();
			}

			// Update the tasks
			Task task = tasks.GetFirstTask();

			while (ProcessTaskUpdate(task, type, 0))
				task = task.GetNextTask();

            AdjustVScrollbar();
            Invalidate();
        }

		private bool ProcessTaskUpdate(Task task, UIExtension.UpdateType type, int depth)
		{
			if (!task.IsValid())
				return false;

			// Get or create a new task and update it
			uint taskId = task.GetID();
			bool newTask = (m_TaskItems.ContainsKey(taskId) == false);

			var taskItem = m_TaskItems.GetItem(taskId, newTask);
			taskItem.UpdateTaskAttributes(task, type, newTask, depth);

			// Process children
			Task subtask = task.GetFirstSubtask();

			while (ProcessTaskUpdate(subtask, type, (depth + 1)))
				subtask = subtask.GetNextTask();

			return true;
		}

		private bool m_StrikeThruDoneTasks = false;

		public bool StrikeThruDoneTasks
		{
			get { return m_StrikeThruDoneTasks; }
			set
			{
                if (m_StrikeThruDoneTasks != value)
				{
                    m_StrikeThruDoneTasks = value;
					Invalidate();
				}
			}
		}

		public bool DisplayDatesInISO
		{
			get { return (AmPmDisplay == false); }
			set { AmPmDisplay = (value == false); }
		}

		private bool m_TaskColorIsBackground = false;

		public bool TaskColorIsBackground
        {
            get { return m_TaskColorIsBackground; }
            set
            {
                if (m_TaskColorIsBackground != value)
                {
                    m_TaskColorIsBackground = value;
                    Invalidate();
                }
            }
        }

		private bool m_ShowParentsAsFolder = false;

		public bool ShowParentsAsFolder
		{
			get { return m_ShowParentsAsFolder; }
			set
			{
				if (m_ShowParentsAsFolder != value)
				{
					m_ShowParentsAsFolder = value;
					Invalidate();
				}
			}
		}

		public void SetUITheme(UITheme theme)
		{
            Theme = theme;
			m_ToolbarRenderer.SetUITheme(theme);

            Invalidate(true);
		}

        public override TimeSpan GetTimeAt(int y)
        {
            TimeSpan time = base.GetTimeAt(y);
            
            if (time == new TimeSpan(1, 0, 0, 0))
                time = time.Subtract(new TimeSpan(0, 0, 1));

            return time;
        }

		void UpdateTodayTime()
		{
			var today = DateTime.Now.Date;

			if ((today >= StartDate) && (today <= EndDate))
			{
				int vPos = GetHourScrollPos(DateTime.Now);

				if ((vPos >= HeaderHeight) && (vPos < ClientRectangle.Bottom))
					Invalidate();
			}
		}

		public override bool EnsureVisible(Calendar.Appointment appt, bool partialOK)
		{
			if ((appt == null) && (m_SelectedLogEntryId != 0))
				appt = m_LogEntries.GetEntry(m_SelectedLogEntryId);

			return base.EnsureVisible(appt, partialOK);
		}

		private void OnResolveAppointments(object sender, Calendar.ResolveAppointmentsEventArgs args)
		{
			args.Appointments = GetMatchingAppointments(args.StartDate, args.EndDate);
		}

		private List<Calendar.Appointment> GetMatchingAppointments(DateTime start, DateTime end)
		{
			return m_LogEntries.GetEntries(start, end).ConvertAll(x => (Calendar.Appointment)x);
		}

		private void OnSelectionChanged(object sender, Calendar.AppointmentEventArgs args)
        {
            if (args.Appointment != null)
				m_SelectedLogEntryId = args.Appointment.Id;
			else
				m_SelectedLogEntryId = 0;

			Invalidate();
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
			if (IsResizingAppointment() && !CanModifyAppointmentDates)
				CancelAppointmentResizing();
		}

		protected override void OnMouseClick(MouseEventArgs e)
		{
			if ((e.Button == MouseButtons.Right) && (ContextMenu?.Invoke(this, e) == true))
				return; // handled
						
			base.OnMouseClick(e);
		}

		private Calendar.SelectionTool.Mode GetMode(Calendar.Appointment appt, Point mousePos)
		{
			if (!CanModifyAppointmentDates || (appt == null))
				return Calendar.SelectionTool.Mode.None;

			var selTool = (ActiveTool as Calendar.SelectionTool);

			if (selTool == null)
			{
				selTool = new Calendar.SelectionTool();
				selTool.DayView = this;
			}

			return selTool.GetMode(mousePos, appt);
		}

		public new bool CancelAppointmentResizing()
		{
			if (base.CancelAppointmentResizing())
			{
				RestoreCachedLogEntry();
				return true;
			}

			return false;
		}

		private bool CanModifyAppointmentDates
		{
			get { return (!ReadOnly && m_LastLogAccessSucceeded); }
		}

		protected override void OnMouseMove(MouseEventArgs e)
        {
			// default handling
			base.OnMouseMove(e);

			Cursor = GetCursor(e);
		}

		protected override void OnScroll(ScrollEventArgs se)
		{
			base.OnScroll(se);

			if (se.ScrollOrientation == ScrollOrientation.HorizontalScroll)
				DoHorizontalScroll(se.NewValue > se.OldValue);
		}

		private Cursor GetCursor(MouseEventArgs e)
        {
			if (IsResizingAppointment())
				return Cursor;

			// Note: base class only shows 'resize' cursors for the currently
			// selected item but we want them for all tasks
			var appt = GetAppointmentAt(e.Location.X, e.Location.Y);

			if (appt != null)
			{
				if (!ReadOnly && (IconHitTest(PointToScreen(e.Location)) > 0))
				{
					if (appt.Locked)
						return UIExtension.AppCursor(UIExtension.AppCursorType.LockedTask);

					return UIExtension.HandCursor();
				}

				var mode = GetMode(appt, e.Location);

				if (CanModifyAppointmentDates)
				{
					// Same as Calendar.SelectionTool
					switch (mode)
					{
					case Calendar.SelectionTool.Mode.ResizeBottom:
					case Calendar.SelectionTool.Mode.ResizeTop:
						return Cursors.SizeNS;

					case Calendar.SelectionTool.Mode.Move:
						// default cursor below
						break;
					}
				}
				else
				{
					if (appt.Locked)
						return UIExtension.AppCursor(UIExtension.AppCursorType.LockedTask);

					if (mode == Calendar.SelectionTool.Mode.Move)
						return UIExtension.AppCursor(UIExtension.AppCursorType.NoDrag);
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
					AdjustVScrollbar();
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
					AdjustVScrollbar();
					Invalidate();
				}
			}

		}

		protected void ValidateMinSlotHeight()
		{
			using (var g = Graphics.FromHwnd(Handle))
			{
				int minHourHeight = (int)g.MeasureString("0", Renderer.HourFont()).Height;

				if ((minSlotHeight * SlotsPerHour) < minHourHeight)
					minSlotHeight = ((minHourHeight / SlotsPerHour) + 1);

				if (SlotHeight < minSlotHeight)
					SlotHeight = minSlotHeight;
			}
		}

		override protected Size CalculateImageSize()
		{
			Size image = base.CalculateImageSize();

			// Make sure image is wide enough to properly render day numbers
			int dayWidth = ((ClientRectangle.Width - (minHourLabelWidth + hourLabelIndent)) / DaysShowing);
			int minDayWidth = 0;

			using (Graphics g = Graphics.FromHwnd(Handle))
				minDayWidth = m_RenderHelper.CalculateMinimumDayWidthForImage(g);

			image.Width = (minHourLabelWidth + hourLabelIndent + (DaysShowing * Math.Max(dayWidth, minDayWidth)));

			return image;
		}

	}
}
