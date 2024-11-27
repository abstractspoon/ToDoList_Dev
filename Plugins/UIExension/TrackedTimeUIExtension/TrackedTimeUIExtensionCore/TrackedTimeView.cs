using System;
using System.Collections.Generic;
using System.Drawing;
using System.Windows.Forms;
using System.Diagnostics;
using System.IO;

using IIControls;
using Abstractspoon.Tdl.PluginHelpers;
using Abstractspoon.Tdl.PluginHelpers.ColorUtil;

namespace TrackedTimeUIExtension
{
	// ------------------------------------------------------------------------------


// 	public delegate void TDLAppointmentEventHandler(object sender, TDLMoveAppointmentEventArgs args);
	public delegate bool TDLContextMenuEventHandler(object sender, MouseEventArgs args);

	// ------------------------------------------------------------------------------

	public partial class TrackedTimeView : Calendar.DayView, ILabelTipHandler
	{
		private uint m_SelectedEntryId = 0;
		private uint m_VisibleSelectedTaskID = 0;
		private uint m_MaxTaskID = 0;

		private int m_UserMinSlotHeight = -1;

		private TaskItems m_TaskItems;
		private LogEntries m_LogEntries;
//		private DateSortedTasks m_DateSortedTasks;

		//private TDLRenderer m_Renderer;
		private LabelTip m_LabelTip;
		private Translator m_Trans;
		private UIThemeToolbarRenderer m_ToolbarRenderer;

		private int LabelTipBorder
		{
			get { return DPIScaling.Scale(4); }
		}

		public bool ReadOnly { get; set; }

		// ----------------------------------------------------------------

// 		public new event TDLAppointmentEventHandler AppointmentMove;
		public new event TDLContextMenuEventHandler ContextMenu;

		// ----------------------------------------------------------------

		public TrackedTimeView(Translator trans, UIExtension.TaskIcon taskIcons, UIExtension.TaskRecurrences taskRecurrences, int minSlotHeight)
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

// 			base.AppointmentMove += new Calendar.AppointmentEventHandler(OnTimeLogAppointmentChanged);
// 			base.NotifyDayWidth += new Calendar.DayWidthEventHandler(OnNotifyDayWidth);

			// Create a 5 minute timer for updating the line indicating time of day 'today'
			var timer = new Timer()
			{
				Enabled = true,
				Interval = (1000 * 5 * 60)
			};
			timer.Tick += (s, e) => { UpdateTodayTime(); };

			InitializeComponent();
		}

	//	public IEnumerable<TaskItems> TaskItems { get { return m_TaskItems.Values; } }

		// ILabelTipHandler implementation
		public Control GetOwner()
		{
			return this;
		}

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

		private void OnTimeLogAppointmentChanged(object sender, Calendar.AppointmentEventArgs args)
		{
/*
			// Repackage and forward to parent
			if (AppointmentMove != null)
			{
				var move = (args as Calendar.MoveAppointmentEventArgs);

				if (move != null)
				{
					TimeLogEntry taskItem = (GetRealAppointment(move.Appointment) as TimeLogEntry);
					string custAttribId = String.Empty;

					if (args.Appointment is TaskCustomDateAttribute)
					{
						custAttribId = (args.Appointment as TaskCustomDateAttribute).AttributeId;
						AppointmentMove(this, new TDLMoveAppointmentEventArgs(taskItem, custAttribId, move.Mode, move.Finished));
					}
					else if (args.Appointment is TaskTimeBlock)
					{
						(args.Appointment as TaskTimeBlock).UpdateTimeBlock();
					}
					else
					{
						AppointmentMove(this, new TDLMoveAppointmentEventArgs(taskItem, move.Mode, move.Finished));
					}
				}
			}
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
// 			AppointmentMove += new TDLAppointmentEventHandler(OnAppointmentChanged);
		}

/*
		protected void OnAppointmentChanged(object sender, TDLMoveAppointmentEventArgs e)
		{
			if ((SelectedAppointment != null) && 
				(SelectedAppointment is TimeLogEntry) && 
				SelectedAppointment.IsLongAppt() && 
				e.Finished)
			{
				m_TimeBlocks.SynchroniseDates(SelectedAppointment as TimeLogEntry);
			}
		}
*/

		public bool ShowLabelTips
		{
			set { m_LabelTip.Active = value; }
			get { return m_LabelTip.Active; }
		}

		public bool IsTaskDisplayable(uint dwTaskID)
		{
// 			if (dwTaskID == 0)
				return false;

// 			return IsItemDisplayable(m_TaskItems.GetItem(dwTaskID));
		}

		public uint SelectedTaskId
		{
			get
			{
				if (!IsTaskDisplayable(m_SelectedEntryId))
					return 0;

				return m_SelectedEntryId;
			}
		}

		public TaskItem SelectedTaskItem
		{
			get
			{
				var entry = SelectedLogEntry;

				return (entry == null ? null : m_TaskItems.GetItem(entry.TaskId));
			}
		}

		public LogEntry SelectedLogEntry
		{
			get
			{
// 				if (!IsTaskDisplayable(m_SelectedEntryId))
// 					return 0;

				return m_LogEntries.GetEntry(m_SelectedEntryId);
			}
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

		public bool SelectTask(String text, UIExtension.SelectTask selectTask, bool caseSensitive, bool wholeWord, bool findReplace)
		{
			if (text == String.Empty)
				return false;

/*
			var sortedTasks = m_DateSortedTasks.Items;
			bool forwards = true;
			int from = -1;

			switch (selectTask)
			{
			case UIExtension.SelectTask.SelectFirstTask:
				if (sortedTasks.Count > 0)
					from = 0;
				break;

			case UIExtension.SelectTask.SelectNextTask:
				from = sortedTasks.NextIndex(SelectedAppointmentId, true);
				break;

			case UIExtension.SelectTask.SelectNextTaskInclCurrent:
				from = sortedTasks.FindItem(SelectedAppointmentId);
				break;

			case UIExtension.SelectTask.SelectPrevTask:
				from = sortedTasks.NextIndex(SelectedAppointmentId, false);
				forwards = false;
				break;

			case UIExtension.SelectTask.SelectLastTask:
				from = (sortedTasks.Count - 1);
				forwards = false;
				break;
			}

			if (from >= 0)
			{
				do
				{
					var taskItem = sortedTasks[from];
					var words = text.Split(' ');

					if (IsItemDisplayable(taskItem) && taskItem.TitleMatches(words, caseSensitive, wholeWord))
					{
						if (SelectTask(taskItem.Id, true))
							return true;
					}

					from = sortedTasks.NextIndex(from, forwards);
				}
				while (from != -1);
			}
*/

			// all else
			return false;
		}

		public Calendar.Appointment FixupSelection(bool scrollToTask, bool allowNotify)
		{
			// Our base class clears the selected appointment whenever
			// the week changes so we can't always rely on 'SelectedAppointmentId'
			uint prevSelTaskID = m_VisibleSelectedTaskID;
			uint selTaskID = SelectedTaskId;

			m_VisibleSelectedTaskID = selTaskID;

			if (selTaskID > 0)
			{
// 				LogEntry item = m_LogEntries.GetItem(selTaskID);
// 
// 				if (item != null)
// 				{
// 					if (scrollToTask)
// 					{
// 						if (item.StartDate != Calendar.Appointment.NullDate)
// 						{
// 							if (!IsItemWithinRange(item, StartDate, EndDate))
// 								StartDate = item.StartDate;
// 
// 							SelectedAppointment = item;
// 						}
// 					}
// 					else if (IsItemWithinRange(item, StartDate, EndDate))
// 					{
// 						SelectedAppointment = item;
// 					}
// 				}
// 				else
// 				{
					SelectedAppointment = null;
// 				}
			}
			else
			{
				SelectedAppointment = null;
			}

			// Notify parent of changes
// 			if (allowNotify && (SelectedTaskId != prevSelTaskID))
// 			{
// 				LogEntry item = m_LogEntries.GetItem(m_VisibleSelectedTaskID);
// 				RaiseSelectionChanged(item);
// 			}

			return SelectedAppointment;
		}

		// External
		public bool SelectTask(uint dwTaskID)
		{
			return SelectTask(dwTaskID, false);
		}

		// Internal
		private bool SelectTask(uint dwTaskID, bool allowNotify)
		{
			m_SelectedEntryId = dwTaskID;
			FixupSelection(true, allowNotify);

			return (SelectedTaskId != 0);
		}

/*
		public bool CanDuplicateSelectedTimeBlock
		{
			get { return (SelectedAppointment is TaskTimeBlock); }
		}

		public bool CanCreateNewTaskBlockSeries
		{
			get { return true; }
		}

		public bool CreateNewTaskBlockSeries(uint taskId, TimeBlockSeriesAttributes attribs)
		{
			var seriesList = m_TimeBlocks.GetTaskSeries(taskId, true);

			if (seriesList == null)
				return false;

			if (!seriesList.AddSeries(attribs))
				return false;

			SelectedAppointment = null;

			return SelectTask(taskId);
		}

		public bool DuplicateSelectedTimeBlock()
		{
 			var block = (SelectedAppointment as TaskTimeBlock);
 
 			if (block == null)
 				return false;

			var seriesList = m_TimeBlocks.GetTaskSeries(block.RealTaskId, true);

			if (seriesList == null)
				return false;

			if (!seriesList.DuplicateBlock(block.TimeBlock))
				return false;

			Invalidate();
			return true;
		}
*/

		public void GoToToday()
        {
            StartDate = DateTime.Now;

			// And scroll vertically to first short task
			var appointments = GetMatchingAppointments(StartDate, EndDate);

			if (appointments != null)
			{
				foreach (var appt in appointments)
				{
					if (!IsLongAppt(appt) && EnsureVisible(appt, false))
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

			if (appt != null)
			{
				if ((reason != UIExtension.HitTestReason.ContextMenu) ||
					AppointmentSupportsTaskContextMenu(appt))
				{
					return UIExtension.HitTestResult.Task;
				}
			}
			else if (GetTrueRectangle().Contains(pt))
			{
				return UIExtension.HitTestResult.Tasklist;
			}

			// else
			return UIExtension.HitTestResult.Nowhere;
		}

		public uint HitTestTask(Int32 xScreen, Int32 yScreen, UIExtension.HitTestReason reason)
		{
			Point pt = PointToClient(new Point(xScreen, yScreen));
			Calendar.Appointment appt = GetAppointmentAt(pt.X, pt.Y);

			if ((appt != null) &&
				(reason != UIExtension.HitTestReason.ContextMenu) ||
				AppointmentSupportsTaskContextMenu(appt))
			{
				return appt.Id;
			}

			return 0;
		}

		public Calendar.Appointment GetRealAppointmentAt(int x, int y)
		{
			return GetRealAppointment(GetAppointmentAt(x, y));
		}

		public Calendar.Appointment GetAppointment(uint taskID)
		{
			return null;//m_LogEntries.GetItem(taskID);
		}

		public Calendar.Appointment GetRealAppointment(Calendar.Appointment appt)
		{
// 			if ((appt != null) && (appt is TaskExtensionItem))
// 				return (appt as TaskExtensionItem).RealTask;

			return appt;
		}

		public bool GetSelectedItemLabelRect(ref Rectangle rect)
		{
/*
			FixupSelection(true, false);
			var appt = GetRealAppointment(SelectedAppointment);

			EnsureVisible(appt, false);
			Update(); // make sure draw rects are updated

			if (GetAppointmentRect(appt, ref rect))
			{
				TimeLogEntry item = (appt as TimeLogEntry);
				bool hasIcon = TaskHasIcon(item);

				if (IsLongAppt(appt))
				{
					// Gripper
					if (appt.StartDate >= StartDate)
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
*/

			return false;
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

// 		public bool IsItemDisplayable(Calendar.Appointment appt)
// 		{
// 			if (appt == null)
// 				return false;
// 
// 			// Always show a task if it is currently being dragged
// 			if (IsResizingAppointment())
// 			{
// 				if (appt.Id == SelectedAppointment.Id)
// 					return true;
// 			}
// 
// 			if (!appt.HasValidDates())
// 				return false;
// 
// 			return true;
// 		}

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

		public void UpdateTasks(TaskList tasks,	UIExtension.UpdateType type, string metaDataKey)
		{
			// Make sure the selected task remains visible
			// after any changes if it was visible to start with
			var selItem = (SelectedAppointment as LogEntry);

			bool selTaskWasVisible = (selItem != null) &&
// 									 IsItemDisplayable(selItem) &&
									 IsItemWithinRange(selItem, StartDate, EndDate);

			bool tasksWasEmpty = m_LogEntries.IsEmpty;

			string filePath = tasks.GetFilePath();

			string logPath = Path.GetFileNameWithoutExtension(filePath) + "_Log.csv";
			logPath = Path.Combine(Path.GetDirectoryName(filePath), logPath);

			m_LogEntries.Load(logPath);

			switch (type)
			{
				case UIExtension.UpdateType.Delete:
				case UIExtension.UpdateType.All:
				{
					// Rebuild
//					m_LogEntries.Clear();
//					m_DateSortedTasks.SetNeedsRebuild();
					m_MaxTaskID = 0;

					SelectedAppointment = null;
				}
				break;

				case UIExtension.UpdateType.New:
					// In-place update
//					m_DateSortedTasks.SetNeedsRebuild();
					break;

				case UIExtension.UpdateType.Edit:
					// In-place update
					break;
			}

			// Update the tasks
			Task task = tasks.GetFirstTask();
			bool datesChanged = false;

			while (ProcessTaskUpdate(task, type, metaDataKey, 0, ref datesChanged))
				task = task.GetNextTask();

// 			if (datesChanged)
// 				m_DateSortedTasks.SetNeedsResort();

			// Scroll to the selected item if it was modified and was 'visible'
			if ((selTaskWasVisible || tasksWasEmpty) && tasks.HasTask(m_SelectedEntryId))
                EnsureVisible(SelectedAppointment, true);

			SelectedDates.Start = SelectedDates.End;

            AdjustVScrollbar();
            Invalidate();
        }

		private bool ProcessTaskUpdate(Task task, UIExtension.UpdateType type, string metaDataKey, int depth, ref bool datesChanged)
		{
			if (!task.IsValid())
				return false;

// 			uint taskId = task.GetID();
// 			m_MaxTaskID = Math.Max(m_MaxTaskID, taskId); // needed for extension occurrences
// 
// 			// Get or create a new task and update it
// 			bool newTask = (m_LogEntries.ContainsKey(taskId) == false);
// 
// 			LogEntry taskItem = m_LogEntries.GetItem(taskId, newTask);
// 
// // 			taskItem.UpdateTaskAttributes(task, m_CustomDateDefs, type, newTask, metaDataKey, depth);
// 
// 			// Update Time Blocks
// 			if (newTask || 
// 				task.HasAttribute(Task.Attribute.StartDate) || 
// 				task.HasAttribute(Task.Attribute.DueDate))
// 			{
// 				datesChanged = true;
// // 				m_TimeBlocks.SynchroniseDates(taskItem);
// 			}

			// Process children
			Task subtask = task.GetFirstSubtask();

			while (ProcessTaskUpdate(subtask, type, metaDataKey, (depth + 1), ref datesChanged))
				subtask = subtask.GetNextTask();

			return true;
		}

		public bool PrepareNewTask(ref Task task)
		{
			// Set the start/due dates to match the current selection
			if (SelectedDates.Start < SelectedDates.End)
			{
				task.SetStartDate(SelectedDates.Start);

				DateTime endDate = SelectedDates.End;

// 				if (TimeLogEntry.IsStartOfDay(endDate))
// 					endDate = endDate.AddSeconds(-1); // end of day before

				task.SetDueDate(endDate);
			}
			else if ((SelectedAppointmentId != 0) &&
					(task.GetParentID() == SelectedAppointmentId))
			{
				// Initialise the subtask to begin at the start of the parent
				var startDate = SelectedAppointment.StartDate;

				task.SetStartDate(startDate);
				task.SetDueDate(startDate.AddDays(1));
			}
			else
			{
				// Use 'today'
				var startDate = DateTime.Today.Date;

				task.SetStartDate(startDate);
				task.SetDueDate(startDate.AddDays(1));
			}

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

		public override DateTime GetDateAt(int x, bool longAppt)
		{
			DateTime date = base.GetDateAt(x, longAppt);

// 			if (longAppt && (date >= EndDate))
// 			{
// 				date = EndDate.AddSeconds(-1);
// 			}

			return date;
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

		public bool EnsureSelectionVisible(bool partialOK)
		{
			var appt = FixupSelection(true, false);

			if (appt == null)
				return false;

			return EnsureVisible(appt, partialOK);
		}

		public override bool EnsureVisible(Calendar.Appointment appt, bool partialOK)
		{
			if ((appt == null) && (m_SelectedEntryId != 0))
			{
// 				LogEntry item;
// 
// 				if (m_LogEntries.TryGetValue(m_SelectedTaskID, out item))
// 					appt = item;
			}

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
			{
				m_SelectedEntryId = args.Appointment.Id;
				m_VisibleSelectedTaskID = (args.Appointment as LogEntry).TaskId;
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
				var mode = GetMode(SelectedAppointment, e.Location);

				if (!CanModifyAppointmentDates(SelectedAppointment, mode))
				{
					CancelAppointmentResizing();
				}
			}
		}

		protected override void OnMouseClick(MouseEventArgs e)
		{
			if ((e.Button == MouseButtons.Right) && (ContextMenu?.Invoke(this, e) == true))
			{
				return; // handled
			}
						
			base.OnMouseClick(e);
		}

		private Calendar.SelectionTool.Mode GetMode(Calendar.Appointment appt, Point mousePos)
		{
			if (ReadOnly || (appt == null))
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
				var taskItem = (SelectedAppointment as LogEntry);

				if (taskItem != null)
				{
					taskItem.RestoreOriginalDates();
				}

				Invalidate();

				return true;
			}

			return false;
		}

		public bool CanDeleteSelectedTimeBlock
		{
			get { return false;/*CanEditSelectedTimeBlockSeries;*/ }
		}

		public bool DeleteSelectedLogEntry()
		{
			if (CanDeleteSelectedTimeBlock)
			{
// 				var block = (SelectedAppointment as TaskTimeBlock);
// 				var seriesList = m_TimeBlocks.GetTaskSeries(block.RealTaskId, false);
// 
// 				if (seriesList.DeleteBlock(block.TimeBlock))
// 				{
// 					// Move selection to 'real' task
// 					SelectTask(block.RealTaskId, true);
// 					Invalidate();
// 
// 					return true;
// 				}
			}

			// else
			return false;
		}

		private bool CanModifyAppointmentDates(Calendar.Appointment appt, Calendar.SelectionTool.Mode mode)
		{
/*
			if (appt == null)
				return false;
			
			if (appt.Locked)
				return false;
			
			bool isTimeBlock = (appt is TaskTimeBlock);

			if (ReadOnly && !isTimeBlock)
				return false;

			var taskItem = (appt as TimeLogEntry);

			// Disable start date editing for tasks with dependencies that are auto-calculated
			// Disable resizing for custom date attributes
			bool isCustomDate = (appt is TaskCustomDateAttribute);
			bool hasDepends = ((taskItem != null) && taskItem.HasDependencies);
			bool hasLockedDepends = (hasDepends && DependencyDatesAreCalculated);

			switch (mode)
			{
			case Calendar.SelectionTool.Mode.Move:
				if (hasLockedDepends)
				{
					// can't change the start date if it's dependent
					// on the end date of another task
					return false;
				}
				else if (HasCalculatedStartDate(appt))
				{
					// can't move a task with a calculated start date
					return false;
				}
				break;

			case Calendar.SelectionTool.Mode.ResizeTop:
				if (isCustomDate)
				{
					// custom dates are ALWAYS long tasks
					return false; 
				}
				else if (hasLockedDepends)
				{
					// can't change the start date if it's dependent
					// on the end date of another task
					return false; 
				}
				break;

			case Calendar.SelectionTool.Mode.ResizeLeft:
				if (isTimeBlock)
				{
					// time blocks are NEVER long tasks
					return false; 
				}
				else if (isCustomDate)
				{
					// custom dates are of FIXED length
					return false;
				}
				else if (hasLockedDepends)
				{
					// can't change the start date if it's dependent
					// on the end date of another task
					return false;
				}
				break;

			case Calendar.SelectionTool.Mode.ResizeBottom:
				if (isCustomDate)
				{
					// custom dates are ALWAYS long tasks
					return false;
				}
				break;

			case Calendar.SelectionTool.Mode.ResizeRight:
				if (isTimeBlock)
				{
					// time blocks are NEVER long tasks
					return false;
				}
				else if (isCustomDate)
				{
					// custom dates are of FIXED length
					return false;
				}
				else if (HasCalculatedEndDate(appt))
				{
					// can't resize a task with a calculated end date
					return false;
				}
				break;

			default:
				return false;
			}
*/

			// all else
			return true;
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
				var realAppt = GetRealAppointment(appt);

				if (!ReadOnly && (IconHitTest(PointToScreen(e.Location)) > 0))
				{
					if (realAppt.Locked)
						return UIExtension.AppCursor(UIExtension.AppCursorType.LockedTask);

					return UIExtension.HandCursor();
				}

				var mode = GetMode(appt, e.Location);

				if (CanModifyAppointmentDates(appt, mode)) // handles readonly flag
				{
					// Same as Calendar.SelectionTool
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
				else
				{
					if (realAppt.Locked)
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
// 			int dayWidth = ((ClientRectangle.Width - (minHourLabelWidth + hourLabelIndent)) / DaysShowing);
// 			int minDayWidth = 0;
// 
// 			using (Graphics g = Graphics.FromHwnd(Handle))
// 				minDayWidth = CalculateMinimumDayWidthForImage(g);
// 
// 			image.Width = (minHourLabelWidth + hourLabelIndent + (DaysShowing * Math.Max(dayWidth, minDayWidth)));
// 
			return image;
		}

	}
}