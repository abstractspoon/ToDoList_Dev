
// PLS DON'T ADD OTHER 'USING' STATEMENTS WHILE I AM STILL LEARNING!
using System;
using Abstractspoon.Tdl.PluginHelpers;

namespace StatsUIExtension
{
    public class SampleListItem
    {
        public String Type { get; set; }
        public String Attrib { get; set; }
        public String Value { get; set; }
        public String Tasks { get; set; }
    }

    public class StatsUIExtensionCore : System.Windows.Controls.Grid, IUIExtension
    {
        public StatsUIExtensionCore()
        {
            System.Windows.Forms.Form a;

            InitializeComponent();
        }

        // IUIExtension ------------------------------------------------------------------

        public bool SelectTask(UInt32 dwTaskID)
        {
            return false;
        }

	    public bool SelectTasks(UInt32[] pdwTaskIDs)
        {
            return false;
        }

	    public void UpdateTasks(TaskList tasks, UIExtension.UpdateType type)
        {
/*
            Task task = tasks.GetFirstTask();
            SampleListItem item = new SampleListItem();

            System.Text.StringBuilder attrib = new System.Text.StringBuilder();
            System.Text.StringBuilder value = new System.Text.StringBuilder();

            if (tasks.HasAttribute(Task.Attribute.Title))
            {
                attrib.Append("Title");
                value.Append(task.GetTitle());
            }

            if (tasks.HasAttribute(Task.Attribute.DoneDate))
            {
                attrib.Append("Done Date");
                value.Append(task.GetDoneDateString());
            }

            if (tasks.HasAttribute(Task.Attribute.DueDate))
            {
                attrib.Append("Due Date");
                value.Append(task.GetDueDateString());
            }

            if (tasks.HasAttribute(Task.Attribute.StartDate))
            {
                attrib.Append("Start Date");
                value.Append(task.GetStartDateString());
            }

            if (tasks.HasAttribute(Task.Attribute.Priority))
            {
                attrib.Append("Priority");
                value.Append(task.GetPriority().ToString());
            }

            if (tasks.HasAttribute(Task.Attribute.Color))
            {
                attrib.Append("Color");
                value.Append(task.GetColor().ToString());
            }

            if (tasks.HasAttribute(Task.Attribute.AllocTo))
            {
                attrib.Append("Alloc To");
                value.Append(task.GetAllocatedTo(0));
            }

            if (tasks.HasAttribute(Task.Attribute.AllocBy))
            {
                attrib.Append("Alloc By");
                value.Append(task.GetAllocatedBy());
            }

            if (tasks.HasAttribute(Task.Attribute.Status))
            {
                attrib.Append("Status");
                value.Append(task.GetStatus());
            }

            if (tasks.HasAttribute(Task.Attribute.Category))
            {
                attrib.Append("Category");
                value.Append(task.GetCategory(0));
            }

            if (tasks.HasAttribute(Task.Attribute.Percent))
            {
                attrib.Append("Percent");
                value.Append(task.GetPercentDone().ToString());
            }

            if (tasks.HasAttribute(Task.Attribute.TimeEstimate))
            {
                attrib.Append("Time Estimate");

                Task.TimeUnits units = Task.TimeUnits.Hours;
                value.Append(task.GetTimeEstimate(ref units).ToString() + units);
            }

            if (tasks.HasAttribute(Task.Attribute.TimeSpent))
            {
                attrib.Append("Time Spent");

                Task.TimeUnits units = Task.TimeUnits.Hours;
                value.Append(task.GetTimeSpent(ref units).ToString() + units);
            }

            if (tasks.HasAttribute(Task.Attribute.FileReference))
            {
                attrib.Append("File Reference");
                value.Append(task.GetFileReference(0));
            }

            if (tasks.HasAttribute(Task.Attribute.Comments))
            {
                attrib.Append("Comments");
                value.Append(task.GetComments());
            }

            if (tasks.HasAttribute(Task.Attribute.Flag))
            {
                attrib.Append("Flag");
                value.Append(task.IsFlagged().ToString());
            }

            if (tasks.HasAttribute(Task.Attribute.CreationDate))
            {
                attrib.Append("Creation Date");
                value.Append(task.GetCreationDateString());
            }

            if (tasks.HasAttribute(Task.Attribute.CreatedBy))
            {
                attrib.Append("Created By");
                value.Append(task.GetCreatedBy());
            }

            if (tasks.HasAttribute(Task.Attribute.Risk))
            {
                attrib.Append("Risk");
                value.Append(task.GetRisk().ToString());
            }

            if (tasks.HasAttribute(Task.Attribute.ExternalId))
            {
                attrib.Append("External ID");
                value.Append(task.GetExternalID());
            }

            if (tasks.HasAttribute(Task.Attribute.Cost))
            {
                attrib.Append("Cost");
                value.Append(task.GetCost().ToString());
            }

            if (tasks.HasAttribute(Task.Attribute.Dependency))
            {
                attrib.Append("Dependency");
                value.Append(task.GetDependency(0));
            }

            if (tasks.HasAttribute(Task.Attribute.Recurrence))
            {
                attrib.Append("Recurrence");
                //value.Append(task.GetRecurrence());
            }

            if (tasks.HasAttribute(Task.Attribute.Version))
            {
                attrib.Append("Version");
                value.Append(task.GetVersion());
            }


            if (tasks.HasAttribute(Task.Attribute.Position))
            {
                attrib.Append("Position");
                value.Append(task.GetPositionString());
            }

            if (tasks.HasAttribute(Task.Attribute.Id))
            {
                attrib.Append("ID");
                value.Append(task.GetID().ToString());
            }

            if (tasks.HasAttribute(Task.Attribute.LastModified))
            {
                attrib.Append("Last Modified");
                //value.Append(task.GetLastModifiedString());
            }

            if (tasks.HasAttribute(Task.Attribute.Icon))
            {
                attrib.Append("Icon");
                value.Append(task.GetIcon());
            }

            if (tasks.HasAttribute(Task.Attribute.Tag))
            {
                attrib.Append("Tag");
                value.Append(task.GetTag(0));
            }

            if (tasks.HasAttribute(Task.Attribute.CustomAttribute))
            {
                attrib.Append("Custom Attribute");
                //value.Append(task.GetCustomAttributeData());
            }

            item.Attrib = attrib.ToString();
            item.Value = value.ToString();
            item.Tasks = task.GetID().ToString();

            switch (type)
            {
                case UIExtension.UpdateType.Edit: item.Type = "Edit"; break;
                case UIExtension.UpdateType.New: item.Type = "Add Task"; break;
                case UIExtension.UpdateType.Delete: item.Type = "Move Task(s)"; break;
                case UIExtension.UpdateType.Move: item.Type = "Delete Task(s)"; break;
                case UIExtension.UpdateType.All: item.Type = "All"; break;
                case UIExtension.UpdateType.Unknown: item.Type = "Unknown"; break;
            }
*/
        }

        public bool WantTaskUpdate(Task.Attribute attrib)
	    {
			return true; // all updates
	    }

        public bool WantSortUpdate(Task.Attribute attrib)
        {
            return false;
        }
	   
        public bool PrepareNewTask(ref Task task)
	    {
		    return false;
    	}

        public bool ProcessMessage(IntPtr hwnd, UInt32 message, UInt32 wParam, UInt32 lParam, UInt32 time, Int32 xPos, Int32 yPos)
	    {
		    return false;
	    }
	   
	    public bool GetLabelEditRect(ref Int32 left, ref Int32 top, ref Int32 right, ref Int32 bottom)
	    {
			return false;
	    }

        public UIExtension.HitResult HitTest(Int32 xPos, Int32 yPos)
	    {
            return UIExtension.HitResult.Nowhere;
	    }

        public void SetUITheme(UITheme theme)
        {
            System.Windows.Media.Color bkColor = theme.GetAppMediaColor(UITheme.AppColor.AppBackDark);

            this.Background = new System.Windows.Media.SolidColorBrush(bkColor);
            //m_Chart.Background = new System.Windows.Media.SolidColorBrush(bkColor);

        }

        public void SetReadOnly(bool bReadOnly)
	    {
	    }

	    public void SavePreferences(Preferences prefs, String key)
	    {
	    }

	    public void LoadPreferences(Preferences prefs, String key, bool appOnly)
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
