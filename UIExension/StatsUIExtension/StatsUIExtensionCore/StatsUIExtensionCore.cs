
// PLS DON'T ADD OTHER 'USING' STATEMENTS WHILE I AM STILL LEARNING!
using System;
using TDLPluginHelpers;

namespace StatsUIExtension
{
    public class SampleListItem
    {
        public String Type { get; set; }
        public String Attrib { get; set; }
        public String Value { get; set; }
        public String Tasks { get; set; }
    }

    public class StatsUIExtensionCore : System.Windows.Controls.Grid, ITDLUIExtension
    {
        public StatsUIExtensionCore()
        {
            System.Windows.Forms.Form a;

            InitializeComponent();
        }

        // ITDLUIExtension ------------------------------------------------------------------

        public bool SelectTask(UInt32 dwTaskID)
        {
            return false;
        }

	    public bool SelectTasks(UInt32[] pdwTaskIDs)
        {
            return false;
        }

	    public void UpdateTasks(TDLTaskList tasks, 
                                TDLUIExtension.UpdateType type,
                                System.Collections.Generic.HashSet<TDLUIExtension.TaskAttribute> attribs)
        {
/*
            TDLTask task = tasks.GetFirstTask();
            SampleListItem item = new SampleListItem();

            System.Text.StringBuilder attrib = new System.Text.StringBuilder();
            System.Text.StringBuilder value = new System.Text.StringBuilder();

            if (attribs.Contains(TDLUIExtension.TaskAttribute.Title))
            {
                attrib.Append("Title");
                value.Append(task.GetTitle());
            }

            if (attribs.Contains(TDLUIExtension.TaskAttribute.DoneDate))
            {
                attrib.Append("Done Date");
                value.Append(task.GetDoneDateString());
            }

            if (attribs.Contains(TDLUIExtension.TaskAttribute.DueDate))
            {
                attrib.Append("Due Date");
                value.Append(task.GetDueDateString());
            }

            if (attribs.Contains(TDLUIExtension.TaskAttribute.StartDate))
            {
                attrib.Append("Start Date");
                value.Append(task.GetStartDateString());
            }

            if (attribs.Contains(TDLUIExtension.TaskAttribute.Priority))
            {
                attrib.Append("Priority");
                value.Append(task.GetPriority().ToString());
            }

            if (attribs.Contains(TDLUIExtension.TaskAttribute.Color))
            {
                attrib.Append("Color");
                value.Append(task.GetColor().ToString());
            }

            if (attribs.Contains(TDLUIExtension.TaskAttribute.AllocTo))
            {
                attrib.Append("Alloc To");
                value.Append(task.GetAllocatedTo(0));
            }

            if (attribs.Contains(TDLUIExtension.TaskAttribute.AllocBy))
            {
                attrib.Append("Alloc By");
                value.Append(task.GetAllocatedBy());
            }

            if (attribs.Contains(TDLUIExtension.TaskAttribute.Status))
            {
                attrib.Append("Status");
                value.Append(task.GetStatus());
            }

            if (attribs.Contains(TDLUIExtension.TaskAttribute.Category))
            {
                attrib.Append("Category");
                value.Append(task.GetCategory(0));
            }

            if (attribs.Contains(TDLUIExtension.TaskAttribute.Percent))
            {
                attrib.Append("Percent");
                value.Append(task.GetPercentDone().ToString());
            }

            if (attribs.Contains(TDLUIExtension.TaskAttribute.TimeEstimate))
            {
                attrib.Append("Time Estimate");

                TDLTask.TimeUnits units = TDLTask.TimeUnits.Hours;
                value.Append(task.GetTimeEstimate(ref units).ToString() + units);
            }

            if (attribs.Contains(TDLUIExtension.TaskAttribute.TimeSpent))
            {
                attrib.Append("Time Spent");

                TDLTask.TimeUnits units = TDLTask.TimeUnits.Hours;
                value.Append(task.GetTimeSpent(ref units).ToString() + units);
            }

            if (attribs.Contains(TDLUIExtension.TaskAttribute.FileReference))
            {
                attrib.Append("File Reference");
                value.Append(task.GetFileReference(0));
            }

            if (attribs.Contains(TDLUIExtension.TaskAttribute.Comments))
            {
                attrib.Append("Comments");
                value.Append(task.GetComments());
            }

            if (attribs.Contains(TDLUIExtension.TaskAttribute.Flag))
            {
                attrib.Append("Flag");
                value.Append(task.IsFlagged().ToString());
            }

            if (attribs.Contains(TDLUIExtension.TaskAttribute.CreationDate))
            {
                attrib.Append("Creation Date");
                value.Append(task.GetCreationDateString());
            }

            if (attribs.Contains(TDLUIExtension.TaskAttribute.CreatedBy))
            {
                attrib.Append("Created By");
                value.Append(task.GetCreatedBy());
            }

            if (attribs.Contains(TDLUIExtension.TaskAttribute.Risk))
            {
                attrib.Append("Risk");
                value.Append(task.GetRisk().ToString());
            }

            if (attribs.Contains(TDLUIExtension.TaskAttribute.ExternalId))
            {
                attrib.Append("External ID");
                value.Append(task.GetExternalID());
            }

            if (attribs.Contains(TDLUIExtension.TaskAttribute.Cost))
            {
                attrib.Append("Cost");
                value.Append(task.GetCost().ToString());
            }

            if (attribs.Contains(TDLUIExtension.TaskAttribute.Dependency))
            {
                attrib.Append("Dependency");
                value.Append(task.GetDependency(0));
            }

            if (attribs.Contains(TDLUIExtension.TaskAttribute.Recurrence))
            {
                attrib.Append("Recurrence");
                //value.Append(task.GetRecurrence());
            }

            if (attribs.Contains(TDLUIExtension.TaskAttribute.Version))
            {
                attrib.Append("Version");
                value.Append(task.GetVersion());
            }


            if (attribs.Contains(TDLUIExtension.TaskAttribute.Position))
            {
                attrib.Append("Position");
                value.Append(task.GetPositionString());
            }

            if (attribs.Contains(TDLUIExtension.TaskAttribute.Id))
            {
                attrib.Append("ID");
                value.Append(task.GetID().ToString());
            }

            if (attribs.Contains(TDLUIExtension.TaskAttribute.LastModified))
            {
                attrib.Append("Last Modified");
                //value.Append(task.GetLastModifiedString());
            }

            if (attribs.Contains(TDLUIExtension.TaskAttribute.Icon))
            {
                attrib.Append("Icon");
                value.Append(task.GetIcon());
            }

            if (attribs.Contains(TDLUIExtension.TaskAttribute.Tag))
            {
                attrib.Append("Tag");
                value.Append(task.GetTag(0));
            }

            if (attribs.Contains(TDLUIExtension.TaskAttribute.CustomAttribute))
            {
                attrib.Append("Custom Attribute");
                //value.Append(task.GetCustomAttributeData());
            }

            item.Attrib = attrib.ToString();
            item.Value = value.ToString();
            item.Tasks = task.GetID().ToString();

            switch (type)
            {
                case TDLUIExtension.UpdateType.Edit: item.Type = "Edit"; break;
                case TDLUIExtension.UpdateType.New: item.Type = "Add Task"; break;
                case TDLUIExtension.UpdateType.Delete: item.Type = "Move Task(s)"; break;
                case TDLUIExtension.UpdateType.Move: item.Type = "Delete Task(s)"; break;
                case TDLUIExtension.UpdateType.All: item.Type = "All"; break;
                case TDLUIExtension.UpdateType.Unknown: item.Type = "Unknown"; break;
            }
*/
        }

        public bool WantEditUpdate(TDLUIExtension.TaskAttribute attrib)
	    {
			return true; // all updates
	    }

        public bool WantSortUpdate(TDLUIExtension.TaskAttribute attrib)
        {
            return false;
        }
	   
        public bool PrepareNewTask(ref TDLTask task)
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
            System.Windows.Media.Color bkColor = theme.GetAppColorAsMedia(TDLTheme.AppColor.AppBackDark);

            this.Background = new System.Windows.Media.SolidColorBrush(bkColor);
            //m_Chart.Background = new System.Windows.Media.SolidColorBrush(bkColor);

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
            this.Background = System.Windows.Media.Brushes.White;

            CreateChart();
            PopulateChart();
        }

        private void CreateChart()
        {
//             // Create a Chart
//             m_Chart = new System.Windows.Controls.DataVisualization.Charting.Chart();
//             m_Chart.Title = "New Stats";
//             m_Chart.Background = System.Windows.Media.Brushes.White;
//             m_Chart.BorderBrush = null;
// 
//             m_XAxis = new System.Windows.Controls.DataVisualization.Charting.DateTimeAxis();
//             m_XAxis.Orientation = System.Windows.Controls.DataVisualization.Charting.AxisOrientation.X;
// 
//             m_YAxis = new System.Windows.Controls.DataVisualization.Charting.CategoryAxis();
//             m_YAxis.Orientation = System.Windows.Controls.DataVisualization.Charting.AxisOrientation.Y;
// 
//             m_Chart.Axes.Add(m_XAxis);
// 
//             this.Children.Add(m_Chart);
        }

        private void PopulateChart()
        {
//             m_Items = new System.Collections.Generic.List<SampleListItem>();
//             m_ListView.ItemsSource = m_Items;
//             m_Chart.DataContext = m_Items;
        }

        // --------------------------------------------------------------------------------------
//         private System.Windows.Controls.DataVisualization.Charting.Chart m_Chart;
//         private System.Windows.Controls.DataVisualization.Charting.DateTimeAxis m_XAxis;
//         private System.Windows.Controls.DataVisualization.Charting.CategoryAxis m_YAxis;
        //private System.Windows.Controls.DataVisualization.Charting.ChartArea m_ChartArea;

    }
}
