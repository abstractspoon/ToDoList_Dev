
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
                                TDLUIExtension.TaskAttribute attrib)
        {
            TDLTask task = tasks.GetFirstTask();
            SampleListItem item = new SampleListItem();

            item.Tasks = task.GetID().ToString();
                    
            switch (attrib)
            {
                case TDLUIExtension.TaskAttribute.Title:            
                    item.Attrib = "Title";
                    item.Value = task.GetTitle();
                    break;

                case TDLUIExtension.TaskAttribute.DoneDate:         
                    item.Attrib = "Done Date";
                    item.Value = task.GetDoneDateString();
                    break;

                case TDLUIExtension.TaskAttribute.DueDate:          
                    item.Attrib = "Due Date";
                    item.Value = task.GetDueDateString();
                    break;
                
                case TDLUIExtension.TaskAttribute.StartDate:        
                    item.Attrib = "Start Date";
                    item.Value = task.GetStartDateString();
                    break;
                
                case TDLUIExtension.TaskAttribute.Priority:         
                    item.Attrib = "Priority";
                    item.Value = task.GetPriority().ToString();
                    break;
                
                case TDLUIExtension.TaskAttribute.Color:            
                    item.Attrib = "Color";
                    item.Value = task.GetColor().ToString();
                    break;
                
                case TDLUIExtension.TaskAttribute.AllocTo:          
                    item.Attrib = "Alloc To";
                    item.Value = task.GetAllocatedTo(0);
                    break;
                
                case TDLUIExtension.TaskAttribute.AllocBy:          
                    item.Attrib = "Alloc By";
                    item.Value = task.GetAllocatedBy();
                    break;
                
                case TDLUIExtension.TaskAttribute.Status:           
                    item.Attrib = "Status";
                    item.Value = task.GetStatus();
                    break;
                
                case TDLUIExtension.TaskAttribute.Category:         
                    item.Attrib = "Category";
                    item.Value = task.GetCategory(0);
                    break;
                
                case TDLUIExtension.TaskAttribute.Percent:          
                    item.Attrib = "Percent";
                    item.Value = task.GetPercentDone().ToString();
                    break;

                case TDLUIExtension.TaskAttribute.TimeEstimate:
                    {
                        item.Attrib = "Time Estimate";

                        TDLTask.TimeUnits units = TDLTask.TimeUnits.Hours;
                        item.Value = (task.GetTimeEstimate(ref units).ToString() + units);
                    }
                    break;

                case TDLUIExtension.TaskAttribute.TimeSpent:
                    {
                        item.Attrib = "Time Spent";

                        TDLTask.TimeUnits units = TDLTask.TimeUnits.Hours;
                        item.Value = (task.GetTimeSpent(ref units).ToString() + units);
                    }
                    break;
                
                case TDLUIExtension.TaskAttribute.FileReference:    
                    item.Attrib = "File Reference";
                    item.Value = task.GetFileReference(0);
                    break;
                
                case TDLUIExtension.TaskAttribute.Comments:         
                    item.Attrib = "Comments";
                    item.Value = task.GetComments();
                    break;
                
                case TDLUIExtension.TaskAttribute.Flag:             
                    item.Attrib = "Flag";
                    item.Value = task.IsFlagged().ToString();
                    break;
                
                case TDLUIExtension.TaskAttribute.CreationDate:     
                    item.Attrib = "Creation Date";
                    item.Value = task.GetCreationDateString();
                    break;
                
                case TDLUIExtension.TaskAttribute.CreatedBy:        
                    item.Attrib = "Created By";
                    item.Value = task.GetCreatedBy();
                    break;
                
                case TDLUIExtension.TaskAttribute.Risk:             
                    item.Attrib = "Risk";
                    item.Value = task.GetRisk().ToString();
                    break;
                
                case TDLUIExtension.TaskAttribute.ExternalId:       
                    item.Attrib = "External ID";
                    item.Value = task.GetExternalID();
                    break;
                
                case TDLUIExtension.TaskAttribute.Cost:             
                    item.Attrib = "Cost";
                    item.Value = task.GetCost().ToString();
                    break;
                
                case TDLUIExtension.TaskAttribute.Dependency:       
                    item.Attrib = "Dependency";
                    item.Value = task.GetDependency(0);
                    break;
                
                case TDLUIExtension.TaskAttribute.Recurrence:       
                    item.Attrib = "Recurrence";
                    //item.Value = task.GetRecurrence();
                    break;
                
                case TDLUIExtension.TaskAttribute.Version:          
                    item.Attrib = "Version";
                    item.Value = task.GetVersion();
                    break;
                
                case TDLUIExtension.TaskAttribute.Position:         
                    item.Attrib = "Position";
                    item.Value = task.GetPositionString();
                    break;
                
                case TDLUIExtension.TaskAttribute.Id:               
                    item.Attrib = "ID";
                    item.Value = task.GetID().ToString();
                    break;
                
                case TDLUIExtension.TaskAttribute.LastModified:     
                    item.Attrib = "Last Modified";
                    //item.Value = task.GetLastModifiedString();
                    break;
                
                case TDLUIExtension.TaskAttribute.Icon:             
                    item.Attrib = "Icon";
                    item.Value = task.GetIcon();
                    break;
                
                case TDLUIExtension.TaskAttribute.Tag:              
                    item.Attrib = "Tag";
                    item.Value = task.GetTag(0);
                    break;
                
                case TDLUIExtension.TaskAttribute.CustomAttribute:  
                    item.Attrib = "Custom Attribute";
                    //item.Value = task.GetCustomAttributeData();
                    break;
                
                case TDLUIExtension.TaskAttribute.All:              
                    item.Attrib = "All";
                    item.Value = "...";
                    break;
                
                case TDLUIExtension.TaskAttribute.Unknown:          
                    item.Attrib = "Unknown";            
                    break;
            }

            switch (type)
            {
                case TDLUIExtension.UpdateType.Edit:    item.Type = "Edit";             break;
                case TDLUIExtension.UpdateType.Add:     item.Type = "Add Task";         break;
                case TDLUIExtension.UpdateType.Delete:  item.Type = "Move Task(s)";     break;
                case TDLUIExtension.UpdateType.Move:    item.Type = "Delete Task(s)";   break;
                case TDLUIExtension.UpdateType.All:     item.Type = "All";              break;
                case TDLUIExtension.UpdateType.Unknown: item.Type = "Unknown";          break;
            }


//             m_Items.Add(item);
//             m_ListView.Items.Refresh();
        }

        public bool WantEditUpdate(TDLUIExtension.TaskAttribute attrib)
	    {
			return true; // all updates
	    }

        public bool WantSortUpdate(TDLUIExtension.TaskAttribute attrib)
        {
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
            System.Windows.Media.Color bkColor = theme.GetAppColor(TDLTheme.AppColor.AppBackDark);

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
