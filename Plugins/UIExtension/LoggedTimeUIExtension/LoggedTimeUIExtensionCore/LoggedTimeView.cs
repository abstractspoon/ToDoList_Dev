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
		private LogFiles m_LogFiles;
		private LogEntry m_CachedLogEntry;
		private TaskTimeLogUtil m_LogUtil;

		private string m_TasklistPath = string.Empty;

		private FileSystemWatcher m_MainLogFileWatcher = new FileSystemWatcher();
		private FileSystemWatcher m_TaskLogFolderWatcher = new FileSystemWatcher();
		private FileSystemWatcher m_TaskLogFileWatcher = new FileSystemWatcher();

//		private DateSortedTasks m_DateSortedTasks;

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
			rightClickSelectionMinutes = 30;

			m_Trans = trans;
			m_RenderHelper.TaskIcons = taskIcons;
			m_ToolbarRenderer = new UIThemeToolbarRenderer();
			m_UserMinSlotHeight = minSlotHeight;
			m_LabelTip = new LabelTip(this);

			m_TaskItems = new TaskItems();
			m_LogUtil = new TaskTimeLogUtil(trans);
			m_LogFiles = new LogFiles(m_LogUtil);
//			m_DateSortedTasks = new DateSortedTasks(m_LogEntries);

			base.NotifyDayWidth += new Calendar.DayWidthEventHandler(OnNotifyDayWidth);

			// Create a 5 minute timer for updating the line indicating time of day 'today'
			var timer = new System.Windows.Forms.Timer()
			{
				Enabled = true,
				Interval = (1000 * 5 * 60)
			};
			timer.Tick += (s, e) => { UpdateTodayTime(); };

			InitializeComponent();
			InitializeFileWatchers();
		}

		public IEnumerable<TaskItem> TaskItems { get { return m_TaskItems.Values; } }
		public UIExtension.TaskIcon TaskIcons { get { return m_RenderHelper.TaskIcons; } }
		public bool HasTasklistPath { get { return !string.IsNullOrWhiteSpace(m_TasklistPath); } }

		public TaskItem GetTask(uint taskId) { return m_TaskItems.GetItem(taskId); }
		public Control GetOwner() { return this; } // ILabelTipHandler implementation
		
		public bool ReadOnly;
		public bool ForceShowSelection;
		public bool LogTasksSeparately;

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
			TabStop = true;
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
			get { return m_LogFiles.GetEntry(m_SelectedLogEntryId); }
		}

		public bool CanAddNewLogEntry
		{
			get
			{
				if (!HasTasklistPath)
					return false;

				if (LogTasksSeparately)
				{
					// We've no idea what task the user is going to check
					// so we can't verify that the log file is not accessible
					return true;
				}

				var mainLogFile = m_LogFiles.GetLogFile(m_TasklistPath, 0);

				return ((mainLogFile != null) && mainLogFile.IsAccessible);
			}
		}

		public bool CanModifySelectedLogEntry
		{
			get { return CanModifyLogEntry(m_SelectedLogEntryId); }
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
			bool logSeparately = (LogTasksSeparately && (taskId != 0));

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
			EnableFileWatching(false);

			bool success = m_LogUtil.AddEntry(m_TasklistPath, newEntry, logSeparately);

			EnableFileWatching(true);

			if (success)
			{
				m_LogFiles.AddEntry(new LogEntry(0, newEntry), logSeparately);
				Invalidate();
			}

			var logPath = TaskTimeLogUtil.GetLogPath(m_TasklistPath, (logSeparately ? taskId : 0));
			HandleLogAccessResult(logPath, false);

			return success;
		}

		public bool ModifySelectedLogEntry(Calendar.AppointmentDates dates, double timeSpentInHrs, string comment, Color fillColor)
		{
			if (!CanModifySelectedLogEntry)
				return false;

			if (!CacheSelectedLogEntry())
				Debug.Assert(false);

			var entry = m_LogFiles.GetEntry(m_SelectedLogEntryId);

			if (!entry.Modify(dates, timeSpentInHrs, comment, fillColor))
			{
				ClearCachedLogEntry();
				return false;
			}

			Invalidate();

			return SaveLogFile(m_SelectedLogEntryId);
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
					if (!CacheSelectedLogEntry())
						Debug.Assert(false);
					break;

				case Calendar.SelectionTool.State.Finished:
					Debug.Assert(e.Appointment.Id == m_CachedLogEntry.Id);
					SaveLogFile(m_SelectedLogEntryId);
					break;
				}
			}
		}

		public bool DeleteSelectedLogEntry()
		{
			if (!CanDeleteSelectedLogEntry)
				return false;

			var logFile = m_LogFiles.GetLogFile(m_SelectedLogEntryId);

			if (logFile == null)
			{
				Debug.Assert(false);
				return false;
			}

			if (!CacheSelectedLogEntry())
				Debug.Assert(false);

			if (!logFile.DeleteEntry(m_SelectedLogEntryId))
				Debug.Assert(false);

			if (!logFile.SaveEntries(m_TasklistPath))
				return false;

			// else
			m_SelectedLogEntryId = 0;
			SelectedAppointment = null;

			Invalidate();
			return true;
		}

		public bool CacheSelectedLogEntry()
		{
			var entry = m_LogFiles.GetEntry(m_SelectedLogEntryId);

			if (entry == null)
				return false;

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

			var logFile = m_LogFiles.GetLogFile(m_CachedLogEntry.Id);

			if (logFile == null)
			{
				Debug.Assert(false);
				return false;
			}

			uint unused = 0;

			logFile.DeleteEntry(m_CachedLogEntry.Id);
			logFile.AddEntry(m_CachedLogEntry, ref unused);

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
/*
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
			LogEntry item = m_LogFiles.GetEntry(m_SelectedLogEntryId);

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

		List<string> m_IdleReloadLogFiles = new List<string>();

		public bool DoIdleProcessing()
		{
			if (m_IdleReloadLogFiles.Count > 0)
			{
				string logPath = m_IdleReloadLogFiles[0];

				LoadLogFile(logPath);
				m_IdleReloadLogFiles.Remove(logPath);
			}

			return (m_IdleReloadLogFiles.Count > 0);
		}

		private void AddIdleReload(string logPath, bool mainLogFile)
		{
			if (m_LogFiles.IsLogFile(logPath, mainLogFile))
			{
				if (m_IdleReloadLogFiles.Find(x => LogFiles.IsSamePath(x, logPath)) == null)
					m_IdleReloadLogFiles.Add(logPath);
			}
		}

		private bool SaveLogFile(uint entryId)
		{
			var logFile = m_LogFiles.GetLogFile(entryId);

			if (logFile == null)
			{
				Debug.Assert(false);
				return false;
			}

			// Temporarily disable file watcher
			EnableFileWatching(false);

			bool success = logFile.SaveEntries(m_TasklistPath);
			HandleLogAccessResult(logFile.FilePath, false);

			EnableFileWatching(true);

			if (success)
				ClearCachedLogEntry();
			else
				RestoreCachedLogEntry();

			return success;
		}

		private void HandleLogAccessResult(string logPath, bool loading)
		{
			var logFile = m_LogFiles.GetLogFile(logPath);
			Debug.Assert(logFile != null);

			if (logFile.IsAccessible != logFile.WasAccessible)
			{
				LogAccessStatusChanged?.Invoke(this, new LogAccessEventArgs()
				{
					Loading = loading,
					Success = logFile.IsAccessible
				});

				if (!logFile.IsAccessible)
				{
					MessageBox.Show(m_LogUtil.FormatLogAccessError(loading),
									m_Trans.Translate("Logged Time", Translator.Type.Text),
									MessageBoxButtons.OK,
									MessageBoxIcon.Exclamation);
				}

			}

			if (logFile.IsAccessible)
				m_IdleReloadLogFiles.Remove(logPath);
			else
				m_IdleReloadLogFiles.Add(logPath);
		}

		///////////////////////////////////////////////////////////

		public void UpdateTasks(TaskList tasks,	UIExtension.UpdateType type)
		{
			string tasklistPath = tasks.GetFilePath();

			if (!string.IsNullOrWhiteSpace(tasklistPath) &&
				!LogFiles.IsSamePath(tasklistPath, m_TasklistPath))
			{
				m_TasklistPath = tasklistPath;

				LoadLogFile(null); // all
				EnableFileWatching(true);
			}

			// Update the tasks
			Task task = tasks.GetFirstTask();

			while (ProcessTaskUpdate(task, type, 0))
				task = task.GetNextTask();

            Invalidate();
        }

		private void LoadLogFile(string logPath)
		{
			bool hadSelection = CacheSelectedLogEntry();

			if (logPath == null)
			{
				m_LogFiles.LoadLogFiles(m_TasklistPath);
				HandleLogAccessResult(TaskTimeLogUtil.GetLogPath(m_TasklistPath), true);
			}
			else
			{
				m_LogFiles.ReloadLogFile(logPath);
				HandleLogAccessResult(logPath, true);
			}

			if (hadSelection)
			{
				var entry = m_LogFiles.FindEntry(m_CachedLogEntry.TaskId, m_CachedLogEntry.Dates, m_CachedLogEntry.TimeSpentInHrs, m_CachedLogEntry.Comment, m_CachedLogEntry.FillColor);
				m_SelectedLogEntryId = ((entry == null) ? 0 : entry.Id);

				ClearCachedLogEntry();
			}
			Invalidate();
		}

		private void EnableFileWatching(bool enable)
		{
			string mainLogFolder = Path.GetDirectoryName(m_TasklistPath);

			if (enable && Directory.Exists(mainLogFolder))
			{
				m_MainLogFileWatcher.Path = mainLogFolder;
				m_MainLogFileWatcher.EnableRaisingEvents = true;

				m_TaskLogFolderWatcher.Path = mainLogFolder;
				m_TaskLogFolderWatcher.EnableRaisingEvents = true;

				string taskLogFolder = Path.ChangeExtension(m_TasklistPath, null);

				if (Directory.Exists(taskLogFolder))
				{
					m_TaskLogFileWatcher.Path = taskLogFolder;
					m_TaskLogFileWatcher.EnableRaisingEvents = true;
				}
				else
				{
					m_TaskLogFileWatcher.EnableRaisingEvents = false;
				}
			}
			else
			{
				m_MainLogFileWatcher.EnableRaisingEvents = false;
				m_TaskLogFolderWatcher.EnableRaisingEvents = false;
				m_TaskLogFileWatcher.EnableRaisingEvents = false;
			}
		}

		private void InitializeFileWatchers()
		{
			m_MainLogFileWatcher.Filter = "*.csv";
			m_MainLogFileWatcher.NotifyFilter = (NotifyFilters.CreationTime | NotifyFilters.LastWrite);
			m_MainLogFileWatcher.Changed += (s, e) =>
			{
				AddIdleReload(e.FullPath, true);
			};

			m_TaskLogFolderWatcher.NotifyFilter = (NotifyFilters.CreationTime | NotifyFilters.DirectoryName);
			m_TaskLogFolderWatcher.Created += (s, e) =>
			{
				EnableFileWatching(true);
			};
			m_TaskLogFolderWatcher.Deleted += (s, e) =>
			{
				m_LogFiles.DeleteAllTaskLogFiles();
				EnableFileWatching(true);
			};

			m_TaskLogFileWatcher.Filter = "*.csv";
			m_TaskLogFileWatcher.NotifyFilter = (NotifyFilters.CreationTime | NotifyFilters.LastWrite | NotifyFilters.FileName);
			m_TaskLogFileWatcher.Created += (s, e) =>
			{
				AddIdleReload(e.FullPath, false);
			};
			m_TaskLogFileWatcher.Changed += (s, e) =>
			{
				AddIdleReload(e.FullPath, false);
			};
			m_TaskLogFileWatcher.Deleted += (s, e) =>
			{
				m_LogFiles.DeleteLogFile(e.FullPath);
			};
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
			get { return m_RenderHelper.DisplayDatesInISO; }

			set
			{
				m_RenderHelper.DisplayDatesInISO = value;
				base.AmPmDisplay = (value == false);
			}
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
				appt = m_LogFiles.GetEntry(m_SelectedLogEntryId);

			return base.EnsureVisible(appt, partialOK);
		}

		private void OnResolveAppointments(object sender, Calendar.ResolveAppointmentsEventArgs args)
		{
			args.Appointments = GetMatchingAppointments(args.StartDate, args.EndDate);
		}

		private List<Calendar.Appointment> GetMatchingAppointments(DateTime start, DateTime end)
		{
			return m_LogFiles.GetEntries(start, end).ConvertAll(x => (Calendar.Appointment)x);
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
			if (IsResizingAppointment() && !CanModifySelectedLogEntry)
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
			if (!CanModifyLogEntry(appt.Id))
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

		private bool CanModifyLogEntry(uint entryId)
		{
			if (ReadOnly)
				return false;

			var logFile = m_LogFiles.GetLogFile(entryId);

			if (logFile == null)
			{
				Debug.Assert((logFile != null) || (entryId == 0));
				return false;
			}

			return logFile.IsAccessible;

		}

		protected override void OnMouseMove(MouseEventArgs e)
        {
			// default handling
			base.OnMouseMove(e);

			Cursor = GetCursor(e);
		}

		protected override void OnScroll(ScrollEventArgs e)
		{
			base.OnScroll(e);

			// Handle 'shift + mouse-wheel' scrolling
			if (e.ScrollOrientation == ScrollOrientation.HorizontalScroll)
				DoHorizontalScroll(e.Type);
		}

		private Cursor GetCursor(MouseEventArgs e)
        {
			if (IsResizingAppointment())
				return Cursor;

			// Note: base class only shows 'resize' cursors for the currently
			// selected item but we want them for all log entries
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

				if (CanModifyLogEntry(appt.Id))
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
					Invalidate();
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
