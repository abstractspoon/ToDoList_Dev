using System;
using System.Collections.Generic;
using System.Drawing;
using System.Windows.Forms;
using System.Diagnostics;

using IIControls;
using Abstractspoon.Tdl.PluginHelpers;
using Abstractspoon.Tdl.PluginHelpers.ColorUtil;

namespace DayViewUIExtension
{
	// ------------------------------------------------------------------------------

	public class TDLMoveAppointmentEventArgs : Calendar.MoveAppointmentEventArgs
	{
		public string CustomAttributeId { get; private set; }

		public TDLMoveAppointmentEventArgs(Calendar.Appointment appointment, Calendar.SelectionTool.Mode mode, bool finished) : base(appointment, mode, finished)
		{
			CustomAttributeId = "";
		}

		public TDLMoveAppointmentEventArgs(Calendar.Appointment appointment, string attribId, Calendar.SelectionTool.Mode mode, bool finished) : base(appointment, mode, finished)
		{
			CustomAttributeId = attribId;
		}
	}

	public delegate void TDLAppointmentEventHandler(object sender, TDLMoveAppointmentEventArgs args);
	public delegate bool TDLContextMenuEventHandler(object sender, MouseEventArgs args);

	// ------------------------------------------------------------------------------

	public partial class TDLDayView : Calendar.DayView, ILabelTipHandler
	{
		private uint m_SelectedTaskID = 0;
		private uint m_VisibleSelectedTaskID = 0;
		private uint m_MaxTaskID = 0;

		private int m_UserMinSlotHeight = -1;

		private string m_HideParentTasksTag;

		private bool m_HideParentTasks = true;
		private bool m_HideTasksWithoutTimes = true;
		private bool m_HideTasksSpanningWeekends = false;
		private bool m_HideTasksSpanningDays = false;
		private bool m_ShowFutureOcurrences = true;

		private TaskItems m_TaskItems;
		private TimeBlocks m_TimeBlocks;

		private Dictionary<uint, TaskExtensionItem> m_ExtensionItems;
		private List<CustomAttributeDefinition> m_CustomDateDefs;

		//private TDLRenderer m_Renderer;
		private LabelTip m_LabelTip;
		private UIExtension.TaskRecurrences m_TaskRecurrences;
		private Translator m_Trans;
		private UIThemeToolbarRenderer m_ToolbarRenderer;

		private int LabelTipBorder
		{
			get { return DPIScaling.Scale(4); }
		}

		public bool ReadOnly { get; set; }

		// ----------------------------------------------------------------

		public new event TDLAppointmentEventHandler AppointmentMove;
		public new event TDLContextMenuEventHandler ContextMenu;

		// ----------------------------------------------------------------

		public TDLDayView(Translator trans, UIExtension.TaskIcon taskIcons, UIExtension.TaskRecurrences taskRecurrences, int minSlotHeight)
		{
			minHourLabelWidth = DPIScaling.Scale(minHourLabelWidth);
			hourLabelIndent = DPIScaling.Scale(hourLabelIndent);
			minDayHeaderHeight = DPIScaling.Scale(minDayHeaderHeight);
			longAppointmentSpacing = DPIScaling.Scale(longAppointmentSpacing);
			dayGripWidth = 1; // to match app styling

			m_Trans = trans;
			m_TaskIcons = taskIcons;
			m_ToolbarRenderer = new UIThemeToolbarRenderer();
			m_UserMinSlotHeight = minSlotHeight;
			m_LabelTip = new LabelTip(this);
			m_TaskRecurrences = taskRecurrences;

			m_TaskItems = new TaskItems();
			m_TimeBlocks = new TimeBlocks();
			m_ExtensionItems = new Dictionary<uint, TaskExtensionItem>();
			m_CustomDateDefs = new List<CustomAttributeDefinition>();

			base.AppointmentMove += new Calendar.AppointmentEventHandler(OnDayViewAppointmentChanged);
			base.NotifyDayWidth += new Calendar.DayWidthEventHandler(OnNotifyDayWidth);

			InitializeComponent();
		}

		public IEnumerable<TaskItem> TaskItems { get { return m_TaskItems.Values; } }

		// ILabelTipHandler implementation
		public Control GetOwner()
		{
			return this;
		}

		public bool ForceShowSelection;

		protected override bool WantDrawDaySelection { get { return base.WantDrawDaySelection || ForceShowSelection; } }

		public LabelTipInfo ToolHitTest(Point ptScreen)
		{
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
			tip.Rect.Inflate(TextPadding, TextPadding);

			var appt = tdlView.Appointment;
			tip.Id = appt.Id;

			if (appt is TaskExtensionItem)
			{
				// NOTE: - Must match 'Calendar' View in 'Core' project
				if (appt is FutureTaskOccurrence)
				{
					tip.Text = m_Trans.Translate("Future Occurrence");
				}
				else if (appt is CustomTaskDateAttribute)
				{
					var apptDate = (appt as CustomTaskDateAttribute);
					var custAttrib = m_CustomDateDefs.Find(x => (x.Id == apptDate.AttributeId));

					tip.Text = string.Format(m_Trans.Translate("{0} (Custom)"), custAttrib.Label);
				}
				else if (appt is TaskTimeBlock)
				{
					tip.Text = m_Trans.Translate("Time Block");
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

			return tip;
		}

		private void OnDayViewAppointmentChanged(object sender, Calendar.AppointmentEventArgs args)
		{
			// Repackage and forward to parent
			if (AppointmentMove != null)
			{
				var move = (args as Calendar.MoveAppointmentEventArgs);

				if (move != null)
				{
					TaskItem taskItem = (GetRealAppointment(move.Appointment) as TaskItem);
					string custAttribId = String.Empty;

					if (args.Appointment is CustomTaskDateAttribute)
					{
						custAttribId = (args.Appointment as CustomTaskDateAttribute).AttributeId;
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
		}

		public uint IconHitTest(Point ptScreen)
		{
			var pt = PointToClient(ptScreen);
			Calendar.Appointment appt = GetAppointmentAt(pt.X, pt.Y);

			if (appt == null)
				return 0;

			var apptView = (GetAppointmentView(appt) as TDLAppointmentView);

			if ((apptView == null) || !apptView.IconRect.Contains(pt))
				return 0;

			return apptView.Appointment.Id;
		}

		protected override void WndProc(ref Message m)
		{
			if (m_LabelTip != null)
				m_LabelTip.ProcessMessage(m);

			base.WndProc(ref m);
		}

		protected override Calendar.SelectionTool NewSelectionTool()
		{
			return new TDLSelectionTool();
		}

		protected override Calendar.AppointmentView NewAppointmentView(Calendar.Appointment appt)
		{
			return new TDLAppointmentView(appt);
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
			Text = "m_dayView";
			ReadOnly = false;

			ResolveAppointments += new Calendar.ResolveAppointmentsEventHandler(OnResolveAppointments);
			SelectionChanged += new Calendar.AppointmentEventHandler(OnSelectionChanged);
			WeekChange += new Calendar.WeekChangeEventHandler(OnWeekChanged);
			AppointmentMove += new TDLAppointmentEventHandler(OnAppointmentChanged);
		}

		protected void OnAppointmentChanged(object sender, TDLMoveAppointmentEventArgs e)
		{
			if ((SelectedAppointment != null) && 
				(SelectedAppointment is TaskItem) && 
				SelectedAppointment.IsLongAppt() && 
				e.Finished)
			{
				m_TimeBlocks.SynchroniseDates(SelectedAppointment as TaskItem);
			}
		}

		public bool ShowLabelTips
		{
			set { m_LabelTip.Active = value; }
			get { return m_LabelTip.Active; }
		}

		public bool IsTaskDisplayable(uint dwTaskID)
		{
			if (dwTaskID == 0)
				return false;

			TaskExtensionItem extItem;

			if (m_ExtensionItems.TryGetValue(dwTaskID, out extItem))
				return IsItemDisplayable(extItem);

			return IsItemDisplayable(m_TaskItems.GetItem(dwTaskID));
		}

		public void SetHideParentTasks(bool hide, string tag)
		{
			m_HideParentTasks = hide;

			if (hide)
				m_HideParentTasksTag = tag;
			else
				m_HideParentTasksTag = string.Empty;

			FixupSelection(false, true);
		}

		public bool ShowFutureOccurrences
		{
			get { return m_ShowFutureOcurrences; }
			set
			{
				if (value != m_ShowFutureOcurrences)
				{
					m_ShowFutureOcurrences = value;
					Invalidate();
				}
			}
		}

		public bool DependencyDatesAreCalculated
		{
			get; set;
		}

		private bool m_DisplayLongTasksContinuous = true;

		public bool DisplayLongTasksContinuous
		{
			get { return m_DisplayLongTasksContinuous; }
			set
			{
				if (value != m_DisplayLongTasksContinuous)
				{
					m_DisplayLongTasksContinuous = value;
					FixupSelection(false, true);
				}
			}
		}

		private bool m_DisplayActiveTasksToday = true;

		public bool DisplayActiveTasksToday
		{
			get { return m_DisplayActiveTasksToday; }
			set
			{
				if (value != m_DisplayActiveTasksToday)
				{
					m_DisplayActiveTasksToday = value;
					FixupSelection(false, true);
				}
			}
		}

		public bool HideTasksWithoutTimes
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

		public bool HideTasksSpanningWeekends
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

		public bool HideTasksSpanningDays
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

		public uint SelectedTaskId
		{
			get
			{
				if (!IsTaskDisplayable(m_SelectedTaskID))
					return 0;

				// If an extension item is selected, return the 'real' task Id
				TaskExtensionItem extItem;

				if (m_ExtensionItems.TryGetValue(m_SelectedTaskID, out extItem))
					return extItem.RealTaskId;

				// else
				return m_SelectedTaskID;
			}
		}

		public bool GetSelectedTaskDates(out DateTime from, out DateTime to)
		{
			from = to = Calendar.Appointment.NullDate;

			uint selTaskID = SelectedTaskId;

			if (selTaskID == 0)
				return false;

			TaskItem item = m_TaskItems.GetItem(selTaskID);

			if ((item == null) || !item.HasValidDates())
				return false;

			from = item.StartDate;
			to = item.EndDate;

			return true;
		}

		public bool GetTask(UIExtension.GetTask getTask, ref uint taskID)
		{
			switch (getTask)
			{
			case UIExtension.GetTask.GetNextTask:
				// TODO
				break;

			case UIExtension.GetTask.GetPrevTask:
				// TODO
				break;

			case UIExtension.GetTask.GetNextVisibleTask:
				// TODO
				break;

			case UIExtension.GetTask.GetNextTopLevelTask:
				// TODO
				break;

			case UIExtension.GetTask.GetPrevVisibleTask:
				// TODO
				break;

			case UIExtension.GetTask.GetPrevTopLevelTask:
				// TODO
				break;
			}

			// all else
			return false;
		}

		public bool SelectTask(String text, UIExtension.SelectTask selectTask, bool caseSensitive, bool wholeWord, bool findReplace)
		{
			if (text == String.Empty)
				return false;

			switch (selectTask)
			{
			case UIExtension.SelectTask.SelectFirstTask:
				// TODO
				break;

			case UIExtension.SelectTask.SelectNextTask:
				// TODO
				break;

			case UIExtension.SelectTask.SelectNextTaskInclCurrent:
				// TODO
				break;

			case UIExtension.SelectTask.SelectPrevTask:
				// TODO
				break;

			case UIExtension.SelectTask.SelectLastTask:
				// TODO
				break;
			}

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
				TaskItem item = m_TaskItems.GetItem(selTaskID);

				if (item != null)
				{
					if (scrollToTask)
					{
						if (item.StartDate != Calendar.Appointment.NullDate)
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
			if (allowNotify && (SelectedTaskId != prevSelTaskID))
			{
				TaskItem item = m_TaskItems.GetItem(m_VisibleSelectedTaskID);
				RaiseSelectionChanged(item);
			}

			return SelectedAppointment;
		}

		public bool SelectTask(uint dwTaskID)
		{
			m_SelectedTaskID = dwTaskID;
			FixupSelection(true, false);

			return (SelectedTaskId != 0);
		}

		public bool CanDuplicateSelectedTimeBlock
		{
			get { return (SelectedAppointment is TaskTimeBlock); }
		}

		public bool CanCreateNewTaskBlockSeries
		{
			get
			{
				return ((SelectedAppointment == null) || 
						(SelectedAppointment is TaskItem) || 
						(SelectedAppointment is TaskTimeBlock));
			}
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

		public void GoToToday()
        {
            StartDate = DateTime.Now;

			// And scroll vertically to first short task
			var appointments = GetMatchingAppointments(StartDate, EndDate, true);

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
			return ((appt != null) && ((appt is TaskItem) || (appt is FutureTaskOccurrence)));
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
				if (appt is TaskExtensionItem)
					return (appt as TaskExtensionItem).RealTaskId;

				return appt.Id;
			}

			return 0;
		}

		public Calendar.Appointment GetRealAppointmentAt(int x, int y)
		{
			return GetRealAppointment(GetAppointmentAt(x, y));
		}

		override public Calendar.AppointmentView GetAppointmentViewAt(int x, int y, out Rectangle apptRect)
		{
			var view = base.GetAppointmentViewAt(x, y, out apptRect);

			if ((view != null) && view.IsLong && !DisplayLongTasksContinuous)
			{
				var tdlView = (view as TDLAppointmentView);

				if (x < tdlView.EndOfStart)
				{
					apptRect.Width = (tdlView.EndOfStart - apptRect.X);
				}
				else
				{
					apptRect.Width = (apptRect.Right - tdlView.StartOfEnd);
					apptRect.X = tdlView.StartOfEnd;
				}
			}

			return view;
		}

		override protected Calendar.AppointmentView GetAppointmentViewAt(int x, int y)
		{
			var view = base.GetAppointmentViewAt(x, y);

			if ((view != null) && view.IsLong && !DisplayLongTasksContinuous)
			{
				var tdlView = (view as TDLAppointmentView);

				if ((x < tdlView.EndOfStart) || (x > tdlView.StartOfEnd))
					return view;

				if (DisplayActiveTasksToday && IsTodayVisible)
				{
					if ((x > tdlView.StartOfToday) && (x < tdlView.EndOfToday))
						return view;
				}
			}

			return view;
		}

		public Calendar.Appointment GetAppointment(uint taskID)
		{
			TaskExtensionItem extItem;

			if (m_ExtensionItems.TryGetValue(taskID, out extItem))
				return extItem;

			return m_TaskItems.GetItem(taskID);
		}

		public Calendar.Appointment GetRealAppointment(Calendar.Appointment appt)
		{
			if ((appt != null) && (appt is TaskExtensionItem))
				return (appt as TaskExtensionItem).RealTask;

			return appt;
		}

		public bool GetSelectedItemLabelRect(ref Rectangle rect)
		{
			FixupSelection(true, false);
			var appt = GetRealAppointment(SelectedAppointment);

			EnsureVisible(appt, false);
			Update(); // make sure draw rects are updated

			if (GetAppointmentRect(appt, ref rect))
			{
				TaskItem item = (appt as TaskItem);
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

		public bool IsItemDisplayable(Calendar.Appointment appt)
		{
			if (appt == null)
				return false;

			// Always show a task if it is currently being dragged
			if (IsResizingAppointment())
			{
				if (appt.Id == SelectedAppointment.Id)
					return true;
			}

			var	realAppt = GetRealAppointment(appt);

			if (m_HideParentTasks)
			{
				var item = (realAppt as TaskItem);
				
				if (item.IsParent)
				{
					if (string.IsNullOrWhiteSpace(m_HideParentTasksTag))
						return false;
					
					if (item.HasTag(m_HideParentTasksTag))
						return false;
				}
			}

			if (!appt.HasValidDates())
				return false;

			if (HideTasksSpanningWeekends)
			{
				if (DateUtil.WeekOfYear(appt.StartDate) != DateUtil.WeekOfYear(appt.EndDate))
					return false;
			}

            if (HideTasksSpanningDays)
            {
                if (appt.StartDate.Date != appt.EndDate.Date)
                    return false;
            }

			if (HideTasksWithoutTimes)
			{
				if (TaskItem.IsStartOfDay(appt.StartDate) && TaskItem.IsEndOfDay(appt.EndDate))
					return false;
			}

			return true;
		}

		private bool IsItemWithinRange(Calendar.Appointment appt, DateTime startDate, DateTime endDate)
		{
			// sanity check
			if (!appt.HasValidDates())
				return false;

			// Task must at least intersect the range
			if ((appt.StartDate >= endDate) || (appt.EndDate <= startDate))
				return false;

			if (!DisplayLongTasksContinuous)
			{
				if (DisplayActiveTasksToday && IsTodayInRange(startDate, endDate))
				{
					if (appt.IntersectsToday)
						return true;
				}

				if ((appt.StartDate < startDate) && (appt.EndDate > endDate))
					return false;
			}

            return true;
		}

		public void SavePreferences(Preferences prefs, String key)
		{
			m_TimeBlocks.Save(prefs, key);
		}

		public void LoadPreferences(Preferences prefs, String key)
		{
			m_TimeBlocks.Load(prefs, key);
		}

		protected int FindTimeBlock(TaskTimeBlock block, List<Calendar.AppointmentDates> timeBlocks)
		{
			if (timeBlocks == null)
			{
				Debug.Assert(timeBlocks != null);
				return -1;
			}

			return timeBlocks.FindIndex(x => ((x.Start == block.StartDate) && (x.End == block.EndDate)));
		}

		public void UpdateTasks(TaskList tasks,	UIExtension.UpdateType type, string metaDataKey)
		{
			// Make sure the selected task remains visible
			// after any changes if it was visible to start with
			var selItem = (SelectedAppointment as TaskItem);

			bool selTaskWasVisible = (selItem != null) &&
									 IsItemDisplayable(selItem) &&
									 IsItemWithinRange(selItem, StartDate, EndDate);

			bool tasksWasEmpty = (m_TaskItems.Count == 0);

            switch (type)
			{
				case UIExtension.UpdateType.Delete:
				case UIExtension.UpdateType.All:
				{
					// Rebuild
					m_TaskItems.Clear();
					m_MaxTaskID = 0;
					SelectedAppointment = null;
				}
				break;

				case UIExtension.UpdateType.New:
				case UIExtension.UpdateType.Edit:
					// In-place update
					break;
			}

			// Update custom attribute definitions
			if (tasks.IsAttributeAvailable(Task.Attribute.CustomAttribute))
				m_CustomDateDefs = tasks.GetCustomAttributes(CustomAttributeDefinition.Attribute.Date);

			// Update the tasks
			Task task = tasks.GetFirstTask();

			while (task.IsValid() && ProcessTaskUpdate(task, type, metaDataKey, 0))
				task = task.GetNextTask();

			// Scroll to the selected item if it was modified and was 'visible'
			if ((selTaskWasVisible || tasksWasEmpty) && tasks.HasTask(m_SelectedTaskID))
                EnsureVisible(SelectedAppointment, true);

			SelectedDates.Start = SelectedDates.End;

            AdjustVScrollbar();
            Invalidate();
        }

		private bool ProcessTaskUpdate(Task task, UIExtension.UpdateType type, string metaDataKey, int depth)
		{
			if (!task.IsValid())
				return false;

			uint taskId = task.GetID();
			m_MaxTaskID = Math.Max(m_MaxTaskID, taskId); // needed for extension occurrences

			// Get or create a new task and update it
			bool newTask = (m_TaskItems.ContainsKey(taskId) == false);

			TaskItem taskItem = m_TaskItems.GetItem(taskId, newTask);
			taskItem.UpdateTaskAttributes(task, m_CustomDateDefs, type, newTask, metaDataKey, depth);

			// Update Time Blocks
			if (newTask || 
				task.HasAttribute(Task.Attribute.StartDate) || 
				task.HasAttribute(Task.Attribute.DueDate))
			{
				m_TimeBlocks.SynchroniseDates(taskItem);
			}

			// Process children
			Task subtask = task.GetFirstSubtask();

			while (subtask.IsValid() && ProcessTaskUpdate(subtask, type, metaDataKey, depth + 1))
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

				if (TaskItem.IsStartOfDay(endDate))
					endDate = endDate.AddSeconds(-1);

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

		private bool m_StrikeThruDoneTasks;

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

		private bool m_TaskColorIsBackground;

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

		private bool m_ShowParentsAsFolder;

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

			if (SystemInformation.HighContrast)
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
			if (!Theme.HasAppColor(UITheme.AppColor.Today))
				return false;
			
			return (time.Date == DateTime.Now.Date);
		}

		protected void DrawToday(PaintEventArgs e, Rectangle rect, DateTime time)
		{
			if (!WantDrawToday(time))
				return;

			using (var brush = new SolidBrush(Theme.GetAppDrawingColor(UITheme.AppColor.Today, 128)))
				e.Graphics.FillRectangle(brush, rect);
		}

		protected void DrawNonWorkHours(PaintEventArgs e, Rectangle rect, DateTime time)
		{
			if (Theme.HasAppColor(UITheme.AppColor.Weekends) && WeekendDays.Contains(time.DayOfWeek))
			{
				var weekendColor = Theme.GetAppDrawingColor(UITheme.AppColor.Weekends, 128);

				// If this is also 'today' then convert to gray so it doesn't 
				// impose too much when the today colour is laid on top
				if (WantDrawToday(time))
					weekendColor = DrawingColor.ToGray(weekendColor);

				using (var brush = new SolidBrush(weekendColor))
					e.Graphics.FillRectangle(brush, rect);
			}
			else if (Theme.HasAppColor(UITheme.AppColor.NonWorkingHours))
			{
				var nonWorkColor = Theme.GetAppDrawingColor(UITheme.AppColor.NonWorkingHours, 128);

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

				if (hoursRect.Y < HeaderHeight)
				{
					hoursRect.Height -= HeaderHeight - hoursRect.Y;
					hoursRect.Y = HeaderHeight;
				}

				e.Graphics.FillRectangle(brush, hoursRect);
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
			if ((appt == null) && (m_SelectedTaskID != 0))
			{
				TaskItem item;

				if (m_TaskItems.TryGetValue(m_SelectedTaskID, out item))
					appt = item;
			}

			return base.EnsureVisible(appt, partialOK);
		}

		protected override bool WantDrawAppointmentSelected(Calendar.Appointment appt)
		{
			if (base.SavingToImage)
				return false;

			if (m_SelectedTaskID == appt.Id)
				return true;

			// Show real task as selected when a future item 
			// is selected and vice versa
			var selAppt = GetAppointment(m_SelectedTaskID);

			if ((selAppt is TaskItem) || (selAppt is FutureTaskOccurrence))
			{
				var selRealID = SelectedTaskId;

				if (appt is FutureTaskOccurrence)
					return (selRealID == (appt as FutureTaskOccurrence).RealTaskId);

				if (selAppt is FutureTaskOccurrence)
					return (selRealID == appt.Id);
			}

			return false;
		}

		private void OnResolveAppointments(object sender, Calendar.ResolveAppointmentsEventArgs args)
		{
			args.Appointments = GetMatchingAppointments(args.StartDate, args.EndDate);
		}

		private List<Calendar.Appointment> GetMatchingAppointments(DateTime start, DateTime end, bool sorted = false)
		{
			// Extension items are always populated on demand
			m_ExtensionItems.Clear();

			var appts = new List<Calendar.Appointment>();
			uint nextExtId = (((m_MaxTaskID / 1000) + 1) * 1000);

			foreach (var pair in m_TaskItems)
			{
				TaskItem item = pair.Value;

				if (IsItemDisplayable(item))
				{
					if (IsItemWithinRange(item, start, end))
						appts.Add(item);

					if (m_ShowFutureOcurrences && item.IsRecurring && !item.IsDoneOrGoodAsDone)
					{
						// Add this task's future items for the current date range
						// Note: we deliberately double the range else we lose 
						// future items which overlap the the current item
						var futureItems = m_TaskRecurrences.Get(item.Id, StartDate, EndDate.AddDays(DaysShowing));

						if (futureItems != null)
						{
							foreach (var futureItem in futureItems)
							{
								var futureAppt = new FutureTaskOccurrence(item, nextExtId, futureItem.Item1, futureItem.Item2);

								if (IsItemWithinRange(futureAppt, start, end))
								{
									m_ExtensionItems[nextExtId++] = futureAppt;
									appts.Add(futureAppt);
								}
							}
						}
					}
				}

				if (m_CustomDateDefs.Count > 0)
				{
					foreach (var attrib in m_CustomDateDefs)
					{
						DateTime date;

						if (item.CustomDates.TryGetValue(attrib.Id, out date))
						{
							var customDate = new CustomTaskDateAttribute(item, nextExtId, attrib.Id, date);

							if (IsItemDisplayable(customDate) && IsItemWithinRange(customDate, start, end))
							{
								m_ExtensionItems[nextExtId++] = customDate;
								appts.Add(customDate);
							}
						}
					}
				}

				var seriesList = m_TimeBlocks.GetTaskSeries(pair.Key, false);

				if (seriesList != null)
				{
					foreach (var series in seriesList.Series)
					{
						foreach (var block in series.Blocks)
						{
							var timeBlock = new TaskTimeBlock(item, nextExtId, block);

							if (IsItemWithinRange(timeBlock, start, end))
							{
								m_ExtensionItems[nextExtId++] = timeBlock;
								appts.Add(timeBlock);
							}
						}
					}
				}
			}

			if (sorted)
				appts.Sort((a, b) => (int)(b.StartDate.Ticks - a.StartDate.Ticks));

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
				var taskItem = (SelectedAppointment as TaskItem);

				if (taskItem != null)
				{
					taskItem.RestoreOriginalDates();
				}
				else if (SelectedAppointment is CustomTaskDateAttribute)
				{
					var custDate = (SelectedAppointment as CustomTaskDateAttribute);
					custDate.RestoreOriginalDate();
				}
				else if (SelectedAppointment is TaskTimeBlock)
				{
					var taskBlock = (SelectedAppointment as TaskTimeBlock);
					taskBlock.RestoreTimeBlock();
				}

				Invalidate();

				return true;
			}

			return false;
		}

		public bool CanDeleteSelectedCustomDate
		{
			get { return (SelectedAppointment is CustomTaskDateAttribute); }
		}
		
		public bool DeleteSelectedCustomDate()
		{
			if (CanDeleteSelectedCustomDate)
			{
				var custDate = (SelectedAppointment as CustomTaskDateAttribute);
				custDate.ClearDate();

				// Notify parent of change
				AppointmentMove?.Invoke(this, new TDLMoveAppointmentEventArgs(custDate.RealTask, custDate.AttributeId, Calendar.SelectionTool.Mode.None, true));

				// Move selection to 'real' task
				SelectTask(custDate.RealTaskId);
				Invalidate();

				return true;
			}

			// else
			return false;
		}

		public bool CanDeleteSelectedTimeBlock
		{
			get { return CanEditSelectedTimeBlockSeries; }
		}

		public bool DeleteSelectedTimeBlock()
		{
			if (CanDeleteSelectedTimeBlock)
			{
				var block = (SelectedAppointment as TaskTimeBlock);
				var seriesList = m_TimeBlocks.GetTaskSeries(block.RealTaskId, false);

				if (seriesList.DeleteBlock(block.TimeBlock))
				{
					// Move selection to 'real' task
					SelectTask(block.RealTaskId);
					Invalidate();

					return true;
				}
			}

			// else
			return false;
		}

		public bool CanDeleteSelectedTimeBlockSeries
		{
			get { return CanDeleteSelectedTimeBlock; }
		}

		public bool DeleteSelectedTimeBlockSeries()
		{
			if (CanDeleteSelectedTimeBlockSeries)
			{
				var block = (SelectedAppointment as TaskTimeBlock);
				var seriesList = m_TimeBlocks.GetTaskSeries(block.RealTaskId, false);

				if (seriesList.DeleteSeries(block.TimeBlock))
				{
					// Move selection to 'real' task
					SelectTask(block.RealTaskId);
					Invalidate();

					return true;
				}
			}

			// else
			return false;
		}

		public TimeBlockSeries SelectedTimeBlockSeries
		{
			get
			{
				if (SelectedAppointment is TaskTimeBlock)
				{
					var block = (SelectedAppointment as TaskTimeBlock);
					var seriesList = m_TimeBlocks.GetTaskSeries(block.RealTaskId, false);

					return seriesList?.GetSeries(block.TimeBlock);
				}

				// else
				return null;
			}
		}

		public bool CanEditSelectedTimeBlockSeries
		{
			get { return (SelectedAppointment is TaskTimeBlock); }
		}

		public bool EditSelectedTimeBlockSeries(TimeBlockSeriesAttributes attribs, TimeBlockSeriesAttributes.EditMask mask)
		{
			if (mask == TimeBlockSeriesAttributes.EditMask.None)
				return false;

			var series = SelectedTimeBlockSeries;

			if (series == null)
				return false;

			if (!series.EditAttributes(attribs, mask))
				return false;

			SelectedAppointment = null;
			Invalidate();

			return true;
		}

		private bool CanModifyAppointmentDates(Calendar.Appointment appt, Calendar.SelectionTool.Mode mode)
		{
			if ((appt != null) && !appt.Locked)
			{
				var taskItem = (appt as TaskItem);

				// Disable modification of parents with calculated dates
				if ((taskItem != null) && taskItem.IsCalculatedParent)
					return false;

				// Disable start date editing for tasks with dependencies that are auto-calculated
				// Disable resizing for custom date attributes
				bool isCustomDate = (appt is CustomTaskDateAttribute);
				bool isTimeBlock = (appt is TaskTimeBlock);
				bool hasDepends = ((taskItem != null) && taskItem.HasDependencies);
				bool hasLockedDepends = (hasDepends && DependencyDatesAreCalculated);

				switch (mode)
				{
				case Calendar.SelectionTool.Mode.Move:
					return (isTimeBlock || (!ReadOnly && !hasLockedDepends));

				case Calendar.SelectionTool.Mode.ResizeTop:
					return (isTimeBlock || (!ReadOnly && (!isCustomDate && !hasLockedDepends)));

				case Calendar.SelectionTool.Mode.ResizeLeft:
					return (!isTimeBlock && !isCustomDate && !hasLockedDepends);

				case Calendar.SelectionTool.Mode.ResizeBottom:
					return (isTimeBlock || !isCustomDate);

				case Calendar.SelectionTool.Mode.ResizeRight:
					return (!isTimeBlock && !isCustomDate);
				}
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
				minDayWidth = CalculateMinimumDayWidthForImage(g);

			image.Width = (minHourLabelWidth + hourLabelIndent + (DaysShowing * Math.Max(dayWidth, minDayWidth)));

			return image;
		}

	}
}
