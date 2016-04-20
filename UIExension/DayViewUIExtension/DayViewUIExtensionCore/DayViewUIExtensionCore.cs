
using System;
using TDLPluginHelpers;

// PLS DON'T ADD 'USING' STATEMENTS WHILE I AM STILL LEARNING!

namespace DayViewUIExtension
{
    public class CalendarItem
    {
        public String Title { get; set; }
        public String AllocTo { get; set; }
        public System.DateTime Start { get; set; }
        public System.DateTime End { get; set; }
        public UInt32 TaskID { get; set; }
        public System.Boolean IsParent { get; set; }
    }

    public class DayViewUIExtensionCore : System.Windows.Forms.Panel, ITDLUIExtension
    {
        public DayViewUIExtensionCore()
        {
            InitializeComponent();
        }

        private void CreateDayView()
        {
            Calendar.DrawTool drawTool = new Calendar.DrawTool();
            this.m_dayView = new Calendar.DayView();

            drawTool.DayView = this.m_dayView;
            this.m_dayView.ActiveTool = drawTool;
            this.m_dayView.AllowInplaceEditing = true;
            this.m_dayView.AmPmDisplay = false;
            this.m_dayView.AppHeightMode = Calendar.DayView.AppHeightDrawMode.TrueHeightAll;
            this.m_dayView.DaysToShow = 7;
            this.m_dayView.Dock = System.Windows.Forms.DockStyle.Fill;
            this.m_dayView.DrawAllAppBorder = false;
            this.m_dayView.Font = new System.Drawing.Font("Tahoma", 8F);
            this.m_dayView.HalfHourHeight = 15;
            this.m_dayView.Location = new System.Drawing.Point(0, 0);
            this.m_dayView.MinHalfHourApp = false;
            this.m_dayView.Name = "m_dayView";
            this.m_dayView.SelectionEnd = new System.DateTime(((long)(0)));
            this.m_dayView.SelectionStart = new System.DateTime(((long)(0)));
            this.m_dayView.Size = new System.Drawing.Size(798, 328);
            this.m_dayView.StartDate = DateTime.Now;
            this.m_dayView.TabIndex = 0;
            this.m_dayView.Text = "m_dayView";
            this.m_dayView.WorkingHourEnd = 19;
            this.m_dayView.WorkingHourStart = 9;
            this.m_dayView.WorkingMinuteEnd = 0;
            this.m_dayView.WorkingMinuteStart = 0;
            
            this.Controls.Add(m_dayView);
        }

        private void PopulateDayView()
        {
            // TODO
        }

        // ITDLUIExtension ------------------------------------------------------------------

        public bool SelectTask(UInt32 dwTaskID)
        {
            return true;
        }

	    public bool SelectTasks(UInt32[] pdwTaskIDs)
        {
            return false;
        }

	    public void UpdateTasks(TDLTaskList tasks, 
                                TDLUIExtension.UpdateType type, 
                                TDLUIExtension.TaskAttribute attrib)
        {
            TDLTask task = tasks.GetFirstTask();

            while (task.IsValid() && ProcessTaskUpdate(task, type, attrib))
                task = task.GetNextTask();

            PopulateDayView();
        }

        private bool ProcessTaskUpdate(TDLTask task, 
                                       TDLUIExtension.UpdateType type,
                                       TDLUIExtension.TaskAttribute attrib)
        {
            if (!task.IsValid())
                return false;

            CalendarItem item;

            if (m_Items.TryGetValue(task.GetID(), out item))
            {
                switch (attrib)
                {
                    case TDLUIExtension.TaskAttribute.Title:
                        item.Title = task.GetTitle();
                        break;

                    case TDLUIExtension.TaskAttribute.DoneDate:
                        item.End = ToDateTime(task.GetDoneDate());
                        break;

                    case TDLUIExtension.TaskAttribute.DueDate:
                        item.End = ToDateTime(task.GetDueDate());
                        break;

                    case TDLUIExtension.TaskAttribute.StartDate:
                        item.Start = ToDateTime(task.GetStartDate());
                        break;
                }
            }
            else
            {
                item = new CalendarItem();

                item.Title = task.GetTitle();
                item.End = ToDateTime(task.GetDueDate());
                item.Start = ToDateTime(task.GetStartDate());
                item.TaskID = task.GetID();
                item.IsParent = task.GetFirstSubtask().IsValid();
            }

            m_Items[task.GetID()] = item;

            // Process children
            TDLTask subtask = task.GetFirstSubtask();

            while (subtask.IsValid() && ProcessTaskUpdate(subtask, type, attrib))
                subtask = subtask.GetNextTask();

            return true;
        }

        private System.DateTime ToDateTime(Int64 tDate)
        {
            return new System.DateTime(1970, 1, 1, 0, 0, 0, System.DateTimeKind.Local).AddMilliseconds(tDate);
        }

        public bool WantUpdate(TDLUIExtension.TaskAttribute attrib)
	    {
            switch (attrib)
            {
                case TDLUIExtension.TaskAttribute.Title:
                case TDLUIExtension.TaskAttribute.DoneDate:
                case TDLUIExtension.TaskAttribute.DueDate:
                case TDLUIExtension.TaskAttribute.StartDate:
                    return true;
            }

            // all else
            return false;
        }
	   
        public bool PrepareNewTask(TDLTaskList task)
	    {
		    return false;
    	}

        public bool ProcessMessage(IntPtr hwnd, UInt32 message, UInt32 wParam, UInt32 lParam, UInt32 time, Int32 xPos, Int32 yPos)
	    {
		    return false;
	    }
	   
		public bool DoAppCommand(TDLUIExtension.AppCommand cmd, UInt32 dwExtra)
		{
		    return false;
		}

	    public bool CanDoAppCommand(TDLUIExtension.AppCommand cmd, UInt32 dwExtra)
	    {
		    return false;
	    }

	    public bool GetLabelEditRect(ref Int32 left, ref Int32 top, ref Int32 right, ref Int32 bottom)
	    {
			return false;
	    }

        public TDLUIExtension.HitResult HitTest(Int32 xPos, Int32 yPos)
	    {
            return TDLUIExtension.HitResult.Nowhere;
	    }

        public void SetUITheme(TDLTheme theme)
        {
            System.Windows.Media.Color temp = theme.GetAppColor(TDLTheme.AppColor.AppBackDark);

            this.BackColor = System.Drawing.Color.FromArgb(temp.R, temp.G, temp.B);
        }

        public void SetReadOnly(bool bReadOnly)
	    {
	    }

	    public void SavePreferences(TDLPreferences prefs, String key)
	    {
	    }

	    public void LoadPreferences(TDLPreferences prefs, String key, bool appOnly)
	    {
	    }
        
        // PRIVATE ------------------------------------------------------------------------------

        private void InitializeComponent()
        {
            this.BackColor = System.Drawing.Color.White;

            CreateDayView();
            PopulateDayView();
        }

        // --------------------------------------------------------------------------------------
        private System.Collections.Generic.Dictionary<UInt32, CalendarItem> m_Items;
        private Calendar.DayView m_dayView;
    }
}
