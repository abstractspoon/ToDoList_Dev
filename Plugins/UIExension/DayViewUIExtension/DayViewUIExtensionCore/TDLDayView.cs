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
		public bool IsTimeBlock { get; private set; }

		public TDLMoveAppointmentEventArgs(Calendar.Appointment appointment, string attribId, Calendar.SelectionTool.Mode mode, bool finished) : base(appointment, mode, finished)
		{
			CustomAttributeId = attribId;
		}

		public TDLMoveAppointmentEventArgs(Calendar.Appointment appointment, bool timeBlock, Calendar.SelectionTool.Mode mode, bool finished) : base(appointment, mode, finished)
		{
			IsTimeBlock = timeBlock;
		}
	}

	public delegate void TDLAppointmentEventHandler(object sender, TDLMoveAppointmentEventArgs args);

	// ------------------------------------------------------------------------------

	public class TDLDayView : Calendar.DayView, ILabelTipHandler
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

		private Dictionary<uint, TaskItem> m_Items;
		private Dictionary<uint, TaskExtensionItem> m_ExtensionItems;
		private List<CustomAttributeDefinition> m_CustomDateDefs;

		private TDLRenderer m_Renderer;
		private LabelTip m_LabelTip;
		private UIExtension.TaskRecurrences m_TaskRecurrences;
		private Translator m_Trans;
		private UIThemeToolbarRenderer m_ToolbarRenderer;

		private int LabelTipBorder
		{
			get { return DPIScaling.Scale(4); }
		}

		public Color GridlineColor
		{
			set
			{
				if (value != m_Renderer.GridlineColor)
				{
					m_Renderer.GridlineColor = value;
					Invalidate();
				}
			}
		}

		public bool ReadOnly { get; set; }

		// ----------------------------------------------------------------

		public new event TDLAppointmentEventHandler AppointmentMove;

		// ----------------------------------------------------------------

		public TDLDayView(Translator trans, UIExtension.TaskIcon taskIcons, UIExtension.TaskRecurrences taskRecurrences, int minSlotHeight)
		{
			minHourLabelWidth = DPIScaling.Scale(minHourLabelWidth);
			hourLabelIndent = DPIScaling.Scale(hourLabelIndent);
			minDayHeaderHeight = DPIScaling.Scale(minDayHeaderHeight);
			longAppointmentSpacing = DPIScaling.Scale(longAppointmentSpacing);
			dayGripWidth = 1; // to match app styling

			m_Trans = trans;
			m_Renderer = new TDLRenderer(Handle, taskIcons);
			m_ToolbarRenderer = new UIThemeToolbarRenderer();
			m_UserMinSlotHeight = minSlotHeight;
			m_LabelTip = new LabelTip(this);
			m_TaskRecurrences = taskRecurrences;

			m_Items = new Dictionary<uint, TaskItem>();
			m_ExtensionItems = new Dictionary<uint, TaskExtensionItem>();
			m_CustomDateDefs = new List<CustomAttributeDefinition>();

			base.AppointmentMove += new Calendar.AppointmentEventHandler(OnDayViewAppointmentChanged);

			InitializeComponent();
		}

		// ILabelTipHandler implementation
		public Font GetFont()
		{
			return m_Renderer.BaseFont;
		}

		public Control GetOwner()
		{
			return this;
		}

		public uint ToolHitTest(Point ptScreen, ref String tipText, ref Rectangle toolRect, ref bool multiLine)
		{
			if (IsResizingAppointment())
				return 0;

			var pt = PointToClient(ptScreen);
			var tdlView = (GetAppointmentViewAt(pt.X, pt.Y, out toolRect) as TDLAppointmentView);

			if (tdlView == null)
				return 0;

			bool startPortion = (toolRect.Right < tdlView.Rectangle.Right);

			toolRect.Offset(startPortion ? tdlView.TextHorzOffset : 0, m_Renderer.TextOffset);
			toolRect.Inflate(m_Renderer.TextPadding, m_Renderer.TextPadding);

			var appt = tdlView.Appointment;

			if (appt is TaskExtensionItem)
			{
				// NOTE: - Must match 'Calendar' View in 'Core' project
				if (appt is FutureOccurrence)
				{
					tipText = m_Trans.Translate("Future Occurrence");
				}
				else if (appt is CustomDateAttribute)
				{
					var apptDate = (appt as CustomDateAttribute);
					var custAttrib = m_CustomDateDefs.Find(x => (x.Id == apptDate.AttributeId));

					tipText = string.Format(m_Trans.Translate("{0} (Custom)"), custAttrib.Label);
				}
				else if (appt is TimeBlock)
				{
					tipText = m_Trans.Translate("Time Block");
				}
				else
				{
					Debug.Assert(false);
				}

				var pos = PointToClient(MousePosition);
				pos.Offset(0, ToolStripEx.GetActualCursorHeight(Cursor));
				toolRect.Location = pos;
			}
			else // 'Real' task
			{
				if (IsLongAppt(appt))
				{
					// single line tooltips
					Size tipSize = m_LabelTip.CalcTipSize(appt.Title, toolRect.Width);

					if ((tipSize.Width <= toolRect.Width) && (tipSize.Height <= toolRect.Height))
						return 0;

					multiLine = false; // always
				}
				else
				{
					var availRect = GetTrueRectangle();

					if (toolRect.Top < availRect.Top)
					{
						// If the top of the text rectangle is hidden we always 
						// need a label tip so we just clip to the avail space
						toolRect.Intersect(availRect);
					}
					else
					{
						// Determine if text will fit in what's visible of the task
						toolRect.Intersect(availRect);

						Size tipSize = m_LabelTip.CalcTipSize(appt.Title, toolRect.Width);

						if ((tipSize.Width <= toolRect.Width) && (tipSize.Height <= toolRect.Height))
							return 0;
					}

					multiLine = true; // always
				}

				tipText = appt.Title;
			}

			return appt.Id;
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

					if (args.Appointment is CustomDateAttribute)
					{
						custAttribId = (args.Appointment as CustomDateAttribute).AttributeId;
						AppointmentMove(this, new TDLMoveAppointmentEventArgs(taskItem, custAttribId, move.Mode, move.Finished));
					}
					else
					{
						bool isTimeBlock = (args.Appointment is TimeBlock);
						AppointmentMove(this, new TDLMoveAppointmentEventArgs(taskItem, isTimeBlock, move.Mode, move.Finished));
					}
				}
			}
		}

		public uint IconHitTest(Point ptScreen)
		{
			var pt = PointToClient(ptScreen);
			Calendar.Appointment appt = GetRealAppointmentAt(pt.X, pt.Y);

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

		protected override Calendar.AppointmentView NewAppointmentView(Calendar.Appointment appt, Rectangle rect, Rectangle gripRect,
																		bool isLong, bool drawLongContinuous,
																		int endOfStart = -1, int startOfEnd = -1)
		{
			return new TDLAppointmentView(appt, rect, gripRect, isLong, drawLongContinuous, endOfStart, startOfEnd);
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
			this.DrawAllAppBorder = false;
			this.Location = new System.Drawing.Point(0, 0);
			this.MinHalfHourApp = false;
			this.Name = "m_dayView";
			this.Renderer = m_Renderer;
			this.Size = new System.Drawing.Size(798, 328);
			this.SlotsPerHour = 4;
			this.TabIndex = 0;
			this.Text = "m_dayView";
			this.ReadOnly = false;

			this.ResolveAppointments += new Calendar.ResolveAppointmentsEventHandler(this.OnResolveAppointments);
			this.SelectionChanged += new Calendar.AppointmentEventHandler(this.OnSelectionChanged);
			this.WeekChange += new Calendar.WeekChangeEventHandler(OnWeekChanged);

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

			TaskItem item;

			if (m_Items.TryGetValue(dwTaskID, out item))
				return IsItemDisplayable(item);

			// else
			return false;
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

		public bool AutoCalculateDependencyDates
		{
			get; set;
		}

		public bool DisplayTasksContinuous
		{
			get { return DisplayLongAppointmentsContinuous; }
			set
			{
				if (value != DisplayLongAppointmentsContinuous)
				{
					DisplayLongAppointmentsContinuous = value;
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

		public uint SelectedTaskID
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

			uint selTaskID = SelectedTaskID;

			if (selTaskID == 0)
				return false;

			TaskItem item;

			if (!m_Items.TryGetValue(selTaskID, out item))
				return false;

			if (!item.HasValidDates())
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
			uint selTaskID = SelectedTaskID;

			m_VisibleSelectedTaskID = selTaskID;

			if (selTaskID > 0)
			{
				TaskItem item;

				if (m_Items.TryGetValue(selTaskID, out item))
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
			if (allowNotify && (SelectedTaskID != prevSelTaskID))
			{
				TaskItem item = null;
				m_Items.TryGetValue(m_VisibleSelectedTaskID, out item);

				RaiseSelectionChanged(item);
			}

			return SelectedAppointment;
		}

		public bool SelectTask(uint dwTaskID)
		{
			m_SelectedTaskID = dwTaskID;
			FixupSelection(true, false);

			return (SelectedTaskID != 0);
		}

		public bool CanDuplicateTimeBlock()
		{
			return (SelectedAppointment is TimeBlock);
		}

		public bool CanCreateNewTimeBlock()
		{
			// Enable button only if the user has dragged a time slot 
			return ((SelectedAppointment != null) &&
					(SelectedDates.Length.Ticks > 0) &&
					(SelectionType == Calendar.SelectionType.DateRange));
		}

		public bool CreateNewTaskBlock(uint taskID)
		{
			if (!HasSelection)
				return false;

			var task = (GetAppointment(taskID) as TaskItem);

			if (task == null)
				return false;

			if (!task.AddTimeBlock(SelectedDates.Start, SelectedDates.End))
				return false;

			Invalidate();
			return true;
		}

		public bool DuplicateSelectedTimeBlock()
		{
			var block = (SelectedAppointment as TimeBlock);

			if (block == null)
				return false;

			var task = block.RealTask;

			if (task == null)
				return false;

			if (!task.AddTimeBlock(block.StartDate, block.EndDate))
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

		private bool AppointmentSupportsTaskContextMenu(Calendar.Appointment appt)
		{
			return ((appt != null) && ((appt is TaskItem) || (appt is FutureOccurrence)));
		}

		public UIExtension.HitResult HitTest(Int32 xScreen, Int32 yScreen)
		{
			System.Drawing.Point pt = PointToClient(new System.Drawing.Point(xScreen, yScreen));
			Calendar.Appointment appt = GetAppointmentAt(pt.X, pt.Y);

			if (appt != null)
			{
				if (AppointmentSupportsTaskContextMenu(appt))
					return UIExtension.HitResult.Task;
			}
			else if (GetTrueRectangle().Contains(pt))
			{
				return UIExtension.HitResult.Tasklist;
			}

			// else
			return UIExtension.HitResult.Nowhere;
		}

		public uint HitTestTask(Int32 xScreen, Int32 yScreen)
		{
			System.Drawing.Point pt = PointToClient(new System.Drawing.Point(xScreen, yScreen));
			Calendar.Appointment appt = GetAppointmentAt(pt.X, pt.Y);

			if (AppointmentSupportsTaskContextMenu(appt))
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

		public Calendar.Appointment GetAppointment(uint taskID)
		{
			TaskExtensionItem extItem;

			if (m_ExtensionItems.TryGetValue(taskID, out extItem))
				return extItem;

			TaskItem item;

			if (m_Items.TryGetValue(taskID, out item))
				return item;

			return null;
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
				bool hasIcon = m_Renderer.TaskHasIcon(item);

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

		public bool IsItemDisplayable(Calendar.Appointment appt)
		{
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

			if (!DisplayTasksContinuous)
			{
				if ((appt.StartDate < startDate) && (appt.EndDate > endDate))
					return false;
			}

            return true;
		}

		public void UpdateTasks(TaskList tasks,	UIExtension.UpdateType type, string metaDataKey)
		{
			// Make sure the selected task remains visible
			// after any changes if it was visible to start with
			var selItem = (SelectedAppointment as TaskItem);

			bool selTaskWasVisible = (selItem != null) &&
									 IsItemDisplayable(selItem) &&
									 IsItemWithinRange(selItem, StartDate, EndDate);

            switch (type)
			{
				case UIExtension.UpdateType.Delete:
				case UIExtension.UpdateType.All:
					// Rebuild
					m_Items.Clear();
					m_MaxTaskID = 0;
					SelectedAppointment = null;
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

			while (task.IsValid() && ProcessTaskUpdate(task, type, metaDataKey))
				task = task.GetNextTask();

			// Scroll to the selected item if it was modified and was 'visible'
			if (selTaskWasVisible && tasks.HasTask(m_SelectedTaskID))
                EnsureVisible(SelectedAppointment, true);

			SelectedDates.Start = SelectedDates.End;

            AdjustVScrollbar();
            Invalidate();
        }

		private bool ProcessTaskUpdate(Task task, UIExtension.UpdateType type, string metaDataKey)
		{
			if (!task.IsValid())
				return false;

			TaskItem item;
			uint taskID = task.GetID();

			m_MaxTaskID = Math.Max(m_MaxTaskID, taskID); // needed for extension occurrences

			// Built-in attributes
			if (m_Items.TryGetValue(taskID, out item))
			{
				item.UpdateTaskAttributes(task, m_CustomDateDefs, type, false, metaDataKey);
			}
			else
			{
				item = new TaskItem();
				item.UpdateTaskAttributes(task, m_CustomDateDefs, type, true, metaDataKey);
			}

			m_Items[taskID] = item;

			// Process children
			Task subtask = task.GetFirstSubtask();

			while (subtask.IsValid() && ProcessTaskUpdate(subtask, type, metaDataKey))
				subtask = subtask.GetNextTask();

			return true;
		}

		public bool StrikeThruDoneTasks
		{
			get { return m_Renderer.StrikeThruDoneTasks; }
			set
			{
                if (m_Renderer.StrikeThruDoneTasks != value)
				{
                    m_Renderer.StrikeThruDoneTasks = value;
					Invalidate();
				}
			}
		}

        public bool TaskColorIsBackground
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

		public bool ShowParentsAsFolder
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

			// Long appt height to match Calendar in core app
			int fontHeight = 0;
			
			if (DPIScaling.WantScaling())
				fontHeight = m_Renderer.BaseFont.Height;
			else
				fontHeight = Win32.GetPixelHeight(m_Renderer.BaseFont.ToHfont());

			int itemHeight = (fontHeight + 6 - longAppointmentSpacing);

            LongAppointmentHeight = Math.Max(itemHeight, 17);
        }
        
        public int GetFontHeight()
        {
            return m_Renderer.GetFontHeight();
        }

   		public void SetUITheme(UITheme theme)
		{
            m_Renderer.Theme = theme;
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
			if (!m_Renderer.Theme.HasAppColor(UITheme.AppColor.Today))
				return false;
			
			return (time.Date == DateTime.Now.Date);
		}

		protected void DrawToday(PaintEventArgs e, Rectangle rect, DateTime time)
		{
			if (!WantDrawToday(time))
				return;

			using (var brush = new SolidBrush(m_Renderer.Theme.GetAppDrawingColor(UITheme.AppColor.Today, 128)))
				e.Graphics.FillRectangle(brush, rect);
		}

		protected void DrawNonWorkHours(PaintEventArgs e, Rectangle rect, DateTime time)
		{
			if (m_Renderer.Theme.HasAppColor(UITheme.AppColor.Weekends) && WeekendDays.Contains(time.DayOfWeek))
			{
				var weekendColor = m_Renderer.Theme.GetAppDrawingColor(UITheme.AppColor.Weekends, 128);

				// If this is also 'today' then convert to gray so it doesn't 
				// impose too much when the today colour is laid on top
				if (WantDrawToday(time))
					weekendColor = DrawingColor.ToGray(weekendColor);

				using (var brush = new SolidBrush(weekendColor))
					e.Graphics.FillRectangle(brush, rect);
			}
			else if (m_Renderer.Theme.HasAppColor(UITheme.AppColor.NonWorkingHours))
			{
				var nonWorkColor = m_Renderer.Theme.GetAppDrawingColor(UITheme.AppColor.NonWorkingHours, 128);

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

				if (hoursRect.Y < this.HeaderHeight)
				{
					hoursRect.Height -= this.HeaderHeight - hoursRect.Y;
					hoursRect.Y = this.HeaderHeight;
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

				if (m_Items.TryGetValue(m_SelectedTaskID, out item))
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

			if ((selAppt is TaskItem) || (selAppt is FutureOccurrence))
			{
				var selRealID = SelectedTaskID;

				if (appt is FutureOccurrence)
					return (selRealID == (appt as FutureOccurrence).RealTaskId);

				if (selAppt is FutureOccurrence)
					return (selRealID == appt.Id);
			}

			return false;
		}

		protected override void DrawAppointment(Graphics g, Calendar.AppointmentView apptView, bool isSelected)
		{
			var appt = apptView.Appointment;
			var rect = apptView.Rectangle;

			isSelected = WantDrawAppointmentSelected(appt);
			
			// Our custom gripper bar
			var gripRect = rect;
			gripRect.Inflate(-2, -2);
			gripRect.Width = 5;

            bool longAppt = apptView.IsLong;

            if (longAppt)
            {
				// If and the start date precedes the 
				// start of the week then extend the draw rect to the left 
				// so the edge is clipped and likewise for the end date.
				if (appt.StartDate < StartDate)
				{
					rect.X -= 4;
					rect.Width += 4;

					gripRect.X = rect.X;
					gripRect.Width = 0;
				}
				else if (appt.StartDate > StartDate)
				{
					rect.X++;
					rect.Width--;

					gripRect.X++;
				}

				if (appt.EndDate >= EndDate)
				{
					rect.Width += 5;
				}
			}
            else // day appt
            {
                if (appt.StartDate.TimeOfDay.TotalHours == 0.0)
                {
                    rect.Y++;
                    rect.Height--;
                }

                rect.Width -= 1;
            }

			apptView.Rectangle = rect;
			apptView.GripRect = gripRect;

			m_Renderer.DrawAppointment(g, apptView, isSelected);
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

			foreach (var pair in m_Items)
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
								var futureAppt = new FutureOccurrence(item, nextExtId, futureItem.Item1, futureItem.Item2);

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
							var customDate = new CustomDateAttribute(item, nextExtId, attrib.Id, date);

							if (IsItemDisplayable(customDate) && IsItemWithinRange(customDate, start, end))
							{
								m_ExtensionItems[nextExtId++] = customDate;
								appts.Add(customDate);
							}
						}
					}
				}

				if (item.TimeBlocks != null)
				{
					foreach (var block in item.TimeBlocks)
					{
						var timeBlock = new TimeBlock(item, nextExtId, block);

						if (IsItemWithinRange(timeBlock, start, end))
						{
							m_ExtensionItems[nextExtId++] = timeBlock;
							appts.Add(timeBlock);
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
			if (e.Button == MouseButtons.Right)
			{
				Calendar.Appointment appt = GetAppointmentAt(e.X, e.Y);

				if ((appt != null) && !AppointmentSupportsTaskContextMenu(appt))
				{
					var menu = new ContextMenuStrip();

					if (appt is CustomDateAttribute)
					{
						var item = new ToolStripMenuItem(m_Trans.Translate("Clear Custom Date"));
						item.ShortcutKeys = Keys.Delete;
						item.ShowShortcutKeys = true;

						item.Click += (s, a) =>
						{
							DeleteSelectedAppointment();
						};

						menu.Items.Add(item);
					}
					else if (appt is TimeBlock)
					{
						var item = new ToolStripMenuItem(m_Trans.Translate("Delete Time Block"));
						item.ShortcutKeys = Keys.Delete;
						item.ShowShortcutKeys = true;

						item.Click += (s, a) =>
						{
							DeleteSelectedAppointment();
						};

						menu.Items.Add(item);

						item = new ToolStripMenuItem(m_Trans.Translate("Duplicate Time Block"));

						item.Click += (s, a) =>
						{
							DuplicateSelectedTimeBlock();
						};

						menu.Items.Add(item);
					}
					else
					{
						Debug.Assert(false);
					}

					if (menu.Items.Count > 0)
					{
						menu.Items.Add(new ToolStripSeparator());
						menu.Items.Add(m_Trans.Translate("Cancel"));

						menu.Renderer = m_ToolbarRenderer;
						menu.Show(this, e.Location);
					}

					return; // always
				}
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
				else if (SelectedAppointment is CustomDateAttribute)
				{
					var custDate = (SelectedAppointment as CustomDateAttribute);
					custDate.RestoreOriginalDate();
				}
				else if (SelectedAppointment is TimeBlock)
				{
					var block = (SelectedAppointment as TimeBlock);
					block.RestoreOriginalDates();
				}

				Invalidate();

				return true;
			}

			return false;
		}

		public bool DeleteSelectedAppointment()
		{
			bool handled = false;

			if (SelectedAppointment is CustomDateAttribute)
			{
				var custDate = (SelectedAppointment as CustomDateAttribute);
				custDate.ClearDate();

				// Notify parent of change
				if (AppointmentMove != null)
					AppointmentMove(this, new TDLMoveAppointmentEventArgs(custDate.RealTask, custDate.AttributeId, Calendar.SelectionTool.Mode.None, true));

				handled = true;
			}
			else if (SelectedAppointment is TimeBlock)
			{
				var block = (SelectedAppointment as TimeBlock);
				block.DeleteBlock();

				// Notify parent of change
				if (AppointmentMove != null)
					AppointmentMove(this, new TDLMoveAppointmentEventArgs(block.RealTask, true, Calendar.SelectionTool.Mode.None, true));

				handled = true;
			}

			if (handled)
			{
				// Move selection to 'real' task
				SelectTask((SelectedAppointment as TaskExtensionItem).RealTaskId);
				Invalidate();
			}

			return handled;
		}

		private bool CanModifyAppointmentDates(Calendar.Appointment appt, Calendar.SelectionTool.Mode mode)
		{
			if (appt.Locked)
				return false;

			// Disable start date editing for tasks with dependencies that are auto-calculated
			// Disable resizing for custom date attributes
			bool isCustomDate = (appt is CustomDateAttribute);
			bool hasDepends = ((appt is TaskItem) && (appt as TaskItem).HasDependencies);

			switch (mode)
			{
			case Calendar.SelectionTool.Mode.Move:
				return (!hasDepends || !AutoCalculateDependencyDates);

			case Calendar.SelectionTool.Mode.ResizeTop:
			case Calendar.SelectionTool.Mode.ResizeLeft:
				return ((!hasDepends || !AutoCalculateDependencyDates) && !isCustomDate);

			case Calendar.SelectionTool.Mode.ResizeBottom:
			case Calendar.SelectionTool.Mode.ResizeRight:
				return !isCustomDate;
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

		private Cursor GetCursor(MouseEventArgs e)
        {
			if (IsResizingAppointment())
				return Cursor;

			// Note: base class only shows 'resize' cursors for the currently
			// selected item but we want them for all tasks
			if (!ReadOnly)
			{
				var appt = GetAppointmentAt(e.Location.X, e.Location.Y);

				if (appt != null)
				{
					if (appt.Locked)
					{
						if (appt is TaskExtensionItem)
							appt = GetRealAppointment(appt);

						if (appt.Locked)
							return UIExtension.AppCursor(UIExtension.AppCursorType.LockedTask);

						return UIExtension.AppCursor(UIExtension.AppCursorType.NoDrag);
					}

					var apptView = (GetAppointmentView(appt) as TDLAppointmentView);

					if ((apptView != null) && apptView.IconRect.Contains(e.Location))
						return UIExtension.HandCursor();

					var mode = GetMode(appt, e.Location);

					if (!CanModifyAppointmentDates(appt, mode))
						return UIExtension.AppCursor(UIExtension.AppCursorType.NoDrag);

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
			using (var g = Graphics.FromHwnd(this.Handle))
			{
				int minHourHeight = (int)g.MeasureString("0", Renderer.HourFont).Height;

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
				minDayWidth = m_Renderer.CalculateMinimumDayWidthForImage(g);

			image.Width = (minHourLabelWidth + hourLabelIndent + (DaysShowing * Math.Max(dayWidth, minDayWidth)));

			return image;
		}

	}
}
