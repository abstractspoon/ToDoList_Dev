using System;
using System.Collections.Generic;
using System.Drawing;
using System.Windows.Forms;
using System.Diagnostics;
using System.Linq;

using IIControls;
using Abstractspoon.Tdl.PluginHelpers;

namespace DayViewUIExtension
{
	// ------------------------------------------------------------------------------

	public class TDLMoveAppointmentEventArgs : Calendar.MoveAppointmentEventArgs
	{
		public string CustomAttributeId { get; private set; }

		public TDLMoveAppointmentEventArgs(Calendar.Appointment appointment, Calendar.SelectionTool.Mode mode, Calendar.SelectionTool.State state) 
			: 
			base(appointment, mode, state)
		{
			CustomAttributeId = "";
		}

		public TDLMoveAppointmentEventArgs(Calendar.Appointment appointment, string attribId, Calendar.SelectionTool.Mode mode, Calendar.SelectionTool.State state) 
			: 
			base(appointment, mode, state)
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
		private uint m_MaxTaskID = 0;

		private int m_UserMinSlotHeight = -1;

		private string m_HideParentTasksTag;

		private bool m_HideParentTasks = true;
		private bool m_HideTasksWithoutTimes = true;
		private bool m_HideTasksSpanningWeekends = false;
		private bool m_HideTasksSpanningDays = false;
		private bool m_ShowFutureOcurrences = true;
		private bool m_TreatOverdueTasksAsDueToday = false;

		private TaskItems m_TaskItems;
		private DateSortedTasks m_DateSortedTasks;
		private TimeBlocks m_TimeBlocks;

		private List<Calendar.Appointment> m_MatchingAppts;
		private List<CustomAttributeDefinition> m_CustomDateDefs;

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
			m_RenderHelper.TaskIcons = taskIcons;
			m_ToolbarRenderer = new UIThemeToolbarRenderer();
			m_UserMinSlotHeight = minSlotHeight;
			m_LabelTip = new LabelTip(this);
			m_TaskRecurrences = taskRecurrences;

			m_TaskItems = new TaskItems();
			m_DateSortedTasks = new DateSortedTasks(m_TaskItems);
			m_TimeBlocks = new TimeBlocks();
			m_MatchingAppts = new List<Calendar.Appointment>();
			m_CustomDateDefs = new List<CustomAttributeDefinition>();

			base.AppointmentMove += new Calendar.AppointmentEventHandler(OnDayViewAppointmentChanged);
			base.NotifyDayWidth += new Calendar.DayWidthEventHandler(OnNotifyDayWidth);

			// Create a 5 minute timer for updating the line indicating time of day 'today'
			var timer = new Timer()
			{
				Enabled = true,
				Interval = (1000 * 5 * 60)
			};
			timer.Tick += (s, e) => { UpdateTodayTime(); };

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

			tip.Rect.Offset(startPortion ? tdlView.TextHorzOffset : 0, m_RenderHelper.TextOffset);

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
			tip.Rect.Inflate(m_RenderHelper.TextPadding, m_RenderHelper.TextPadding);

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

					if (args.Appointment is TaskCustomDateAttribute)
					{
						custAttribId = (args.Appointment as TaskCustomDateAttribute).AttributeId;
						AppointmentMove(this, new TDLMoveAppointmentEventArgs(taskItem, custAttribId, move.Mode, move.State));
					}
					else if (args.Appointment is TaskTimeBlock)
					{
						(args.Appointment as TaskTimeBlock).UpdateTimeBlock();
					}
					else
					{
						AppointmentMove(this, new TDLMoveAppointmentEventArgs(taskItem, move.Mode, move.State));
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
			TabStop = true;
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

		public bool IsTaskDisplayable(uint taskId)
		{
			if (taskId == 0)
				return false;

			var appt = GetAppointment(taskId);

			return IsItemDisplayable(appt);
		}

		public void SetHideParentTasks(bool hide, string tag)
		{
			m_HideParentTasks = hide;

			if (hide)
				m_HideParentTasksTag = tag;
			else
				m_HideParentTasksTag = string.Empty;

			RebuildMatchingAppointments();
		}

		public bool ShowFutureOccurrences
		{
			get { return m_ShowFutureOcurrences; }
			set
			{
				if (value != m_ShowFutureOcurrences)
				{
					m_ShowFutureOcurrences = value;
					RebuildMatchingAppointments();
				}
			}
		}

		public bool TreatOverdueTasksAsDueToday
		{
			get { return m_TreatOverdueTasksAsDueToday; }
			set
			{
				if (value != m_TreatOverdueTasksAsDueToday)
				{
					m_TreatOverdueTasksAsDueToday = value;
					m_TaskItems.TreatOverdueTasksAsDueToday = value;

					RebuildMatchingAppointments();
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
					RebuildMatchingAppointments();
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
					RebuildMatchingAppointments();
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
					RebuildMatchingAppointments();
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
					RebuildMatchingAppointments();
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
					RebuildMatchingAppointments();
				}
			}
		}

		public uint SelectedTaskId
		{
			// We always return the 'real task' Id
			get
			{
				if (!IsTaskDisplayable(m_SelectedTaskID))
					return 0;

				// If an extension item is selected, return the 'real' task Id
				var appt = GetAppointment(m_SelectedTaskID);

				if (appt is TaskExtensionItem)
					return (appt as TaskExtensionItem).RealTaskId;

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

			// all else
			return false;
		}

		public bool SelectTask(String text, UIExtension.SelectTask selectTask, bool caseSensitive, bool wholeWord, bool findReplace)
		{
			if (text == String.Empty)
				return false;

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

			// all else
			return false;
		}


		protected override void OnKeyPress(KeyPressEventArgs e)
		{
			if (SelectNextTask(new string(e.KeyChar, 1)))
				return;

			base.OnKeyPress(e);
		}

		protected bool SelectNextTask(string startingWith)
		{
			var taskItems = m_DateSortedTasks.Items;

			if (taskItems.Count == 0)
				return false;

			int start = taskItems.FindItem(m_SelectedTaskID);
			int next = taskItems.NextIndex(start, true, true); // wrap

			while ((next != -1) && (next != start))
			{
				var appt = taskItems[next];

				if (!IsItemDisplayable(appt))
				{
					// Skip hidden tasks
				}
				else if (appt.Title.StartsWith(startingWith, StringComparison.InvariantCultureIgnoreCase))
				{
					if (SelectTask(appt.Id, true))
						return true;

					Debug.Assert(false);
				}

				next = taskItems.NextIndex(next, true, true); // wrap
			}

			return false;
		}

		public Calendar.Appointment FixupSelection(bool scrollToTask, bool allowNotify)
		{
			// Our base class clears the selected appointment whenever
			// the week changes so we can't always rely on 'SelectedAppointmentId'
			var appt = GetAppointment(m_SelectedTaskID);

			if (appt != null)
			{
				if (scrollToTask)
				{
					if (appt.StartDate != Calendar.Appointment.NullDate)
					{
						if (!IsItemWithinRange(appt, StartDate, EndDate))
							StartDate = appt.StartDate;

						SelectedAppointment = appt;
					}
				}
				else if (IsItemWithinRange(appt, StartDate, EndDate))
				{
					SelectedAppointment = appt;
				}
			}
			else
			{
				SelectedAppointment = null;
			}

			// Notify parent of changes
			if (allowNotify)
				RaiseSelectionChanged(appt);

			return SelectedAppointment;
		}

		// External
		public bool SelectTask(uint taskId)
		{
			return SelectTask(taskId, false);
		}

		// Internal
		private bool SelectTask(uint taskId, bool allowNotify)
		{
			m_SelectedTaskID = taskId;
			FixupSelection(true, allowNotify);

			return (SelectedTaskId != 0);
		}

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

			m_SelectedTaskID = taskId;
			StartDate = attribs.FromDate; // will trigger RebuildMatchingAppointments

			return true;
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

			RebuildMatchingAppointments();
			return true;
		}

		public void GoToToday()
        {
            StartDate = DateTime.Now; // will trigger RebuildMatchingAppointments

			if (m_MatchingAppts.Count > 0)
			{
				// Scroll to the first 'in-day' appointment
				foreach (var appt in m_MatchingAppts)
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
			return ((appt != null) && ((appt is TaskItem) || (appt is TaskFutureOccurrence)));
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

		override protected bool AppointmentViewContains(Calendar.AppointmentView view, int x, int y)
		{
			if ((view == null) || !view.Rectangle.Contains(x, y))
				return false;

			if (view.IsLong && !DisplayLongTasksContinuous)
			{
				var tdlView = (view as TDLAppointmentView);

				if ((x < tdlView.EndOfStart) || (x > tdlView.StartOfEnd))
					return true;

				if (DisplayActiveTasksToday && IsTodayVisible)
				{
					if ((x > tdlView.StartOfToday) && (x < tdlView.EndOfToday))
						return true;
				}

				return false;
			}

			return true;
		}

		public Calendar.Appointment GetAppointment(uint taskID)
		{
			Calendar.Appointment appt = m_TaskItems.GetItem(taskID);

			if (appt == null)
				appt = m_MatchingAppts.Find(x => (x.Id == taskID));

			return appt;
		}

		public Calendar.Appointment GetRealAppointment(Calendar.Appointment appt)
		{
			if (appt is TaskExtensionItem)
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

					rect.Height = (m_RenderHelper.FontHeight + 4); // 4 = border
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
					m_DateSortedTasks.SetNeedsRebuild();
					m_MaxTaskID = 0;

					SelectedAppointment = null;
				}
				break;

				case UIExtension.UpdateType.New:
					// In-place update
					m_DateSortedTasks.SetNeedsRebuild();
					break;

				case UIExtension.UpdateType.Edit:
					// In-place update
					break;
			}

			// Update custom attribute definitions
			if (tasks.IsAttributeAvailable(Task.Attribute.CustomAttribute))
				m_CustomDateDefs = tasks.GetCustomAttributes(CustomAttributeDefinition.Attribute.Date);

			// Update the tasks
			Task task = tasks.GetFirstTask();
			bool datesChanged = false;

			while (ProcessTaskUpdate(task, type, metaDataKey, 0, ref datesChanged))
				task = task.GetNextTask();

			if (datesChanged)
				m_DateSortedTasks.SetNeedsResort();

			// Scroll to the selected item if it was modified and was 'visible'
			if ((selTaskWasVisible || tasksWasEmpty) && tasks.HasTask(m_SelectedTaskID))
                EnsureVisible(SelectedAppointment, true);

			SelectedDates.Start = SelectedDates.End;

			RebuildMatchingAppointments();
			Invalidate();
        }

		private bool ProcessTaskUpdate(Task task, UIExtension.UpdateType type, string metaDataKey, int depth, ref bool datesChanged)
		{
			if (!task.IsValid())
				return false;

			uint taskId = task.GetID();
			m_MaxTaskID = Math.Max(m_MaxTaskID, taskId); // needed for extension occurrences

			// Get or create a new task and update it
			bool newTask = (m_TaskItems.ContainsKey(taskId) == false);

			TaskItem taskItem = m_TaskItems.GetItem(taskId, newTask);

			taskItem.UpdateTaskAttributes(task, m_CustomDateDefs, type, newTask, metaDataKey, depth);
			taskItem.TreatOverdueTasksAsDueToday = m_TreatOverdueTasksAsDueToday;

			// Update Time Blocks
			if (newTask || 
				task.HasAttribute(Task.Attribute.StartDate) || 
				task.HasAttribute(Task.Attribute.DueDate))
			{
				datesChanged = true;
				m_TimeBlocks.SynchroniseDates(taskItem);
			}

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

				if (TaskItem.IsStartOfDay(endDate))
					endDate = endDate.AddSeconds(-1); // end of day before

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
			if ((appt == null) && (m_SelectedTaskID != 0))
			{
				TaskItem item;

				if (m_TaskItems.TryGetValue(m_SelectedTaskID, out item))
					appt = item;
			}

			return base.EnsureVisible(appt, partialOK);
		}

		private void OnResolveAppointments(object sender, Calendar.ResolveAppointmentsEventArgs args)
		{
			args.Appointments = m_MatchingAppts;
		}

		protected override bool AppointmentsIntersect(Calendar.Appointment appt, Calendar.Appointment apptOther)
		{
			if (!base.AppointmentsIntersect(appt, apptOther))
				return false;

			if (appt.IsLongAppt() && !DisplayLongTasksContinuous)
			{
				// Note: TaskItem.EndDate already takes account of TreatOverdueTasksAsDueToday

				// Test if one appointment's start/end dates 
				// match the start/end dates of the other
				if ((appt.StartDate.Date == apptOther.StartDate.Date) ||
					(appt.StartDate.Date == apptOther.EndDate.Date) ||
					(appt.EndDate.Date == apptOther.EndDate.Date) ||
					(appt.EndDate.Date == apptOther.StartDate.Date))
				{
					return true;
				}

				if (DisplayActiveTasksToday)
				{
					// If one appointment intersects with today, test if 
					// the other's start/end dates also matches today
					if (appt.IntersectsToday)
					{
						if (apptOther.IntersectsToday)
							return true;

						DateTime today = DateTime.Today;

						if ((today == appt.StartDate.Date) || 
							(today == appt.EndDate.Date))
						{
							return true;
						}
					}
					else if (apptOther.IntersectsToday)
					{
						DateTime today = DateTime.Today;

						if ((today == apptOther.StartDate.Date) ||
							(today == apptOther.EndDate.Date))
						{
							return true;
						}
					}
				}

				// else
				return false;
			}

			// else
			return true;
		}

		private void RebuildMatchingAppointments()
		{
			RebuildMatchingAppointments(StartDate, EndDate);
		}

		private void RebuildMatchingAppointments(DateTime start, DateTime end)
		{
			// Cache enough information to restore the selected item after the rebuild
			var selAppt = GetAppointment(m_SelectedTaskID);
			var realApptId = m_SelectedTaskID;

			string custDateAttribID = null;
			var custDate = (selAppt as TaskCustomDateAttribute);

			if (custDate != null)
			{
				custDateAttribID = custDate.AttributeId;
				realApptId = custDate.RealTaskId;
			}

			// Retrieve all items falling within the specified range
			// Extension items are generated on demand
			m_MatchingAppts = new List<Calendar.Appointment>();
			uint nextExtId = (((m_MaxTaskID / 1000) + 1) * 1000);

			foreach (var pair in m_TaskItems)
			{
				TaskItem item = pair.Value;

				if (IsItemDisplayable(item))
				{
					if (IsItemWithinRange(item, start, end))
						m_MatchingAppts.Add(item);

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
								var futureAppt = new TaskFutureOccurrence(item, nextExtId++, futureItem.Item1, futureItem.Item2);

								if (IsItemWithinRange(futureAppt, start, end))
								{
									m_MatchingAppts.Add(futureAppt);
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
							var customDate = new TaskCustomDateAttribute(item, nextExtId++, attrib.Id, date);

							// Note: We don't check IsItemWithinRange() because storing
							// ALL custom dates is the only way to preserve their selection 
							// when the custom dates are not visible
							if (IsItemDisplayable(customDate))
							{
								m_MatchingAppts.Add(customDate);
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
							var timeBlock = new TaskTimeBlock(item, nextExtId++, block);

							if (IsItemWithinRange(timeBlock, start, end))
							{
								m_MatchingAppts.Add(timeBlock);
							}
						}
					}
				}
			}
			m_MatchingAppts.Sort((a, b) => TaskItem.CompareDates(a, b));

			// Restore the previously selected item
			if (realApptId != 0)
			{
				if (custDateAttribID == null)
				{
					m_SelectedTaskID = realApptId;
				}
				else
				{
					foreach (var match in m_MatchingAppts)
					{
						var dateItem = (match as TaskCustomDateAttribute);

						if ((dateItem?.RealTaskId == realApptId) && (dateItem?.AttributeId == custDateAttribID))
						{
							m_SelectedTaskID = match.Id;
							break;
						}
					}
				}
			}

			FixupSelection(false, true);
			Invalidate();
		}

		private void OnSelectionChanged(object sender, Calendar.AppointmentEventArgs args)
        {
            if (args.Appointment != null)
			{
				m_SelectedTaskID = args.Appointment.Id;

				if (args.Appointment is TaskFutureOccurrence)
					m_SelectedTaskID = (args.Appointment as TaskFutureOccurrence).RealTaskId;
			}
		}

        private void OnWeekChanged(object sender, Calendar.WeekChangeEventArgs args)
        {
			RebuildMatchingAppointments(args.StartDate, args.EndDate);
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
				else if (SelectedAppointment is TaskCustomDateAttribute)
				{
					var custDate = (SelectedAppointment as TaskCustomDateAttribute);
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
			get { return (SelectedAppointment is TaskCustomDateAttribute); }
		}
		
		public bool DeleteSelectedCustomDate()
		{
			if (CanDeleteSelectedCustomDate)
			{
				var custDate = (SelectedAppointment as TaskCustomDateAttribute);
				custDate.ClearDate();

				// Notify parent of change
				AppointmentMove?.Invoke(this, new TDLMoveAppointmentEventArgs(custDate.RealTask, 
																			  custDate.AttributeId, 
																			  Calendar.SelectionTool.Mode.None, 
																			  Calendar.SelectionTool.State.Finished));

				// Move selection to 'real' task
				SelectTask(custDate.RealTaskId, true);
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
					SelectTask(block.RealTaskId, true);
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
					SelectTask(block.RealTaskId, true);
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

		private bool HasCalculatedStartDate(Calendar.Appointment appt)
		{
			if (appt is TaskItem)
				return (appt as TaskItem).HasCalculatedStartDate;

			return false;
		}

		private bool HasCalculatedEndDate(Calendar.Appointment appt)
		{
			if (appt is TaskItem)
				return (appt as TaskItem).HasCalculatedEndDate;

			if (appt is TaskCustomDateAttribute)
				return (appt as TaskCustomDateAttribute).HasCalculatedEndDate;

			return false;
		}

		private bool CanModifyAppointmentDates(Calendar.Appointment appt, Calendar.SelectionTool.Mode mode)
		{
			if (appt == null)
				return false;
			
			if (appt.Locked)
				return false;
			
			if (appt is TaskFutureOccurrence)
				return false;

			bool isTimeBlock = (appt is TaskTimeBlock);

			if (ReadOnly && !isTimeBlock)
				return false;

			var taskItem = (appt as TaskItem);

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

			// all else
			return true;
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
