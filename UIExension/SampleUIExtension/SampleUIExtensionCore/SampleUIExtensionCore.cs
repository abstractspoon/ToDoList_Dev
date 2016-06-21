
using System;
using TDLPluginHelpers;

// PLS DON'T ADD 'USING' STATEMENTS WHILE I AM STILL LEARNING!

namespace SampleUIExtension
{
    public class SampleListItem
    {
        public String Type { get; set; }
        public String Attrib { get; set; }
        public String Value { get; set; }
        public String Tasks { get; set; }
    }

    public class SampleUIExtensionCore : System.Windows.Controls.Grid, ITDLUIExtension
    {
        public SampleUIExtensionCore()
        {
            InitializeComponent();
        }

        // ITDLUIExtension ------------------------------------------------------------------

        public bool SelectTask(UInt32 dwTaskID)
        {
            SampleListItem item = new SampleListItem();

            item.Value = dwTaskID.ToString();
            item.Type = "Selection";

            m_Items.Add(item);
            m_ListView.Items.Refresh();

            return true;
        }

	    public bool SelectTasks(UInt32[] pdwTaskIDs)
        {
            return false;
        }

	    public void UpdateTasks(TDLTaskList tasks, 
                                TDLUIExtension.UpdateType type,
                                System.Collections.Generic.HashSet<TDLUIExtension.TaskAttribute> attribs)
        {
            TDLTask task = tasks.GetFirstTask();
            SampleListItem item = new SampleListItem();

            System.Collections.Generic.List<String> attrib = new System.Collections.Generic.List<String>();
            System.Collections.Generic.List<String> value = new System.Collections.Generic.List<String>();
            
            if (attribs.Contains(TDLUIExtension.TaskAttribute.Title))
            {
                attrib.Add("Title");
                value.Add(task.GetTitle());
            }

            if (attribs.Contains(TDLUIExtension.TaskAttribute.DoneDate))
            {
                attrib.Add("Done Date");
                value.Add(task.GetDoneDateString());
            }

            if (attribs.Contains(TDLUIExtension.TaskAttribute.DueDate))
            {
                attrib.Add("Due Date");
                value.Add(task.GetDueDateString());
            }

            if (attribs.Contains(TDLUIExtension.TaskAttribute.StartDate))
            {
                attrib.Add("Start Date");
                value.Add(task.GetStartDateString());
            }

            if (attribs.Contains(TDLUIExtension.TaskAttribute.Priority))
            {
                attrib.Add("Priority");
                value.Add(task.GetPriority().ToString());
            }

            if (attribs.Contains(TDLUIExtension.TaskAttribute.Color))
            {
                attrib.Add("Color");
                value.Add(task.GetColor().ToString());
            }

            if (attribs.Contains(TDLUIExtension.TaskAttribute.AllocTo))
            {
                attrib.Add("Alloc To");
                value.Add(String.Join(", ", task.GetAllocatedTo()));
            }

            if (attribs.Contains(TDLUIExtension.TaskAttribute.AllocBy))
            {
                attrib.Add("Alloc By");
                value.Add(task.GetAllocatedBy());
            }

            if (attribs.Contains(TDLUIExtension.TaskAttribute.Status))
            {
                attrib.Add("Status");
                value.Add(task.GetStatus());
            }

            if (attribs.Contains(TDLUIExtension.TaskAttribute.Category))
            {
                attrib.Add("Category");
                value.Add(String.Join(", ", task.GetCategory()));
            }

            if (attribs.Contains(TDLUIExtension.TaskAttribute.Percent))
            {
                attrib.Add("Percent");
                value.Add(task.GetPercentDone().ToString());
            }

            if (attribs.Contains(TDLUIExtension.TaskAttribute.TimeEstimate))
            {
                attrib.Add("Time Estimate");

                TDLTask.TimeUnits units = TDLTask.TimeUnits.Hours;
                value.Add(task.GetTimeEstimate(ref units).ToString() + units);
            }

            if (attribs.Contains(TDLUIExtension.TaskAttribute.TimeSpent))
            {
                attrib.Add("Time Spent");

                TDLTask.TimeUnits units = TDLTask.TimeUnits.Hours;
                value.Add(task.GetTimeSpent(ref units).ToString() + units);
            }

            if (attribs.Contains(TDLUIExtension.TaskAttribute.FileReference))
            {
                attrib.Add("File Reference");
                value.Add(String.Join(", ", task.GetFileReference()));
            }

            if (attribs.Contains(TDLUIExtension.TaskAttribute.Comments))
            {
                attrib.Add("Comments");
                value.Add(task.GetComments());
            }

            if (attribs.Contains(TDLUIExtension.TaskAttribute.Flag))
            {
                attrib.Add("Flag");
                value.Add(task.IsFlagged().ToString());
            }

            if (attribs.Contains(TDLUIExtension.TaskAttribute.CreationDate))
            {
                attrib.Add("Creation Date");
                value.Add(task.GetCreationDateString());
            }

            if (attribs.Contains(TDLUIExtension.TaskAttribute.CreatedBy))
            {
                attrib.Add("Created By");
                value.Add(task.GetCreatedBy());
            }

            if (attribs.Contains(TDLUIExtension.TaskAttribute.Risk))
            {
                attrib.Add("Risk");
                value.Add(task.GetRisk().ToString());
            }

            if (attribs.Contains(TDLUIExtension.TaskAttribute.ExternalId))
            {
                attrib.Add("External ID");
                value.Add(task.GetExternalID());
            }

            if (attribs.Contains(TDLUIExtension.TaskAttribute.Cost))
            {
                attrib.Add("Cost");
                value.Add(task.GetCost().ToString());
            }

            if (attribs.Contains(TDLUIExtension.TaskAttribute.Dependency))
            {
                attrib.Add("Dependency");
                value.Add(String.Join(", ", task.GetDependency()));
            }

            if (attribs.Contains(TDLUIExtension.TaskAttribute.Recurrence))
            {
                attrib.Add("Recurrence");
                //value.Add(task.GetRecurrence());
            }

            if (attribs.Contains(TDLUIExtension.TaskAttribute.Version))
            {
                attrib.Add("Version");
                value.Add(task.GetVersion());
            }


            if (attribs.Contains(TDLUIExtension.TaskAttribute.Position))
            {
                attrib.Add("Position");
                value.Add(task.GetPositionString());
            }

            if (attribs.Contains(TDLUIExtension.TaskAttribute.Id))
            {
                attrib.Add("ID");
                value.Add(task.GetID().ToString());
            }

            if (attribs.Contains(TDLUIExtension.TaskAttribute.LastModified))
            {
                attrib.Add("Last Modified");
                //value.Add(task.GetLastModifiedString());
            }

            if (attribs.Contains(TDLUIExtension.TaskAttribute.Icon))
            {
                attrib.Add("Icon");
                value.Add(task.GetIcon());
            }

            if (attribs.Contains(TDLUIExtension.TaskAttribute.Tag))
            {
                attrib.Add("Tag");
                value.Add(String.Join(", ", task.GetTag()));
            }

            if (attribs.Contains(TDLUIExtension.TaskAttribute.CustomAttribute))
            {
                attrib.Add("Custom Attribute");
                //value.Add(task.GetCustomAttributeData());
            }

            item.Attrib = String.Join(",", attrib.ToArray());
            item.Value = String.Join(",", value.ToArray());
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

            m_Items.Add(item);
            m_ListView.Items.Refresh();
        }

        public bool WantEditUpdate(TDLUIExtension.TaskAttribute attrib)
	    {
			return true; // all updates
	    }

        public bool WantSortUpdate(TDLUIExtension.TaskAttribute attrib)
        {
			return true; // all updates
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

            CreateListView();
            PopulateListView();
        }

        private void CreateListView()
        {
            m_ListView = new System.Windows.Controls.ListView();
            m_GridView = new System.Windows.Controls.GridView();

            m_TypeCol = new System.Windows.Controls.GridViewColumn();
            m_TypeCol.DisplayMemberBinding = new System.Windows.Data.Binding("Type");
            m_TypeCol.Header = "Change Type";
            m_TypeCol.Width = 200;
            m_GridView.Columns.Add(m_TypeCol);

            m_AttribCol = new System.Windows.Controls.GridViewColumn();
            m_AttribCol.DisplayMemberBinding = new System.Windows.Data.Binding("Attrib");
            m_AttribCol.Header = "Attribute Changed";
            m_AttribCol.Width = 200;
            m_GridView.Columns.Add(m_AttribCol);

            m_ValueCol = new System.Windows.Controls.GridViewColumn();
            m_ValueCol.DisplayMemberBinding = new System.Windows.Data.Binding("Value");
            m_ValueCol.Header = "New Value";
            m_ValueCol.Width = 200;
            m_GridView.Columns.Add(m_ValueCol);

            m_TasksCol = new System.Windows.Controls.GridViewColumn();
            m_TasksCol.DisplayMemberBinding = new System.Windows.Data.Binding("Tasks");
            m_TasksCol.Header = "Tasks Changed";
            m_TasksCol.Width = 200;
            m_GridView.Columns.Add(m_TasksCol);

            m_ListView.View = m_GridView;

            this.Children.Add(m_ListView);
        }

        private void PopulateListView()
        {
            m_Items = new System.Collections.Generic.List<SampleListItem>();
            m_ListView.ItemsSource = m_Items;
        }

        // --------------------------------------------------------------------------------------
        private System.Collections.Generic.List<SampleListItem> m_Items;
        private System.Windows.Controls.ListView m_ListView;
        private System.Windows.Controls.GridView m_GridView;
        private System.Windows.Controls.GridViewColumn m_TypeCol;
        private System.Windows.Controls.GridViewColumn m_AttribCol;
        private System.Windows.Controls.GridViewColumn m_ValueCol;
        private System.Windows.Controls.GridViewColumn m_TasksCol;
    }
}
