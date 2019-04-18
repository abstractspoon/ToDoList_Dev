
using System;
using Abstractspoon.Tdl.PluginHelpers;

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

    public class SampleUIExtensionCore : System.Windows.Controls.Grid, IUIExtension
    {
        public SampleUIExtensionCore()
        {
            InitializeComponent();
        }

        // IUIExtension ------------------------------------------------------------------

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

	    public void UpdateTasks(TaskList tasks, UIExtension.UpdateType type)
        {
            Task task = tasks.GetFirstTask();
            SampleListItem item = new SampleListItem();

            System.Collections.Generic.List<String> attrib = new System.Collections.Generic.List<String>();
            System.Collections.Generic.List<String> value = new System.Collections.Generic.List<String>();
            
            if (tasks.HasAttribute(Task.Attribute.Title))
            {
                attrib.Add("Title");
                value.Add(task.GetTitle());
            }

            if (tasks.HasAttribute(Task.Attribute.DoneDate))
            {
                attrib.Add("Done Date");
                value.Add(task.GetDoneDateString());
            }

            if (tasks.HasAttribute(Task.Attribute.DueDate))
            {
                attrib.Add("Due Date");
                value.Add(task.GetDueDateString(false));
            }

            if (tasks.HasAttribute(Task.Attribute.StartDate))
            {
                attrib.Add("Start Date");
                value.Add(task.GetStartDateString(false));
            }

            if (tasks.HasAttribute(Task.Attribute.Priority))
            {
                attrib.Add("Priority");
                value.Add(task.GetPriority(false).ToString());
            }

            if (tasks.HasAttribute(Task.Attribute.Color))
            {
                attrib.Add("Color");
                value.Add(task.GetColor().ToString());
            }

            if (tasks.HasAttribute(Task.Attribute.AllocatedTo))
            {
                attrib.Add("Alloc To");
                value.Add(task.FormatAllocatedTo(", "));
            }

            if (tasks.HasAttribute(Task.Attribute.AllocatedBy))
            {
                attrib.Add("Alloc By");
                value.Add(task.GetAllocatedBy());
            }

            if (tasks.HasAttribute(Task.Attribute.Status))
            {
                attrib.Add("Status");
                value.Add(task.GetStatus());
            }

            if (tasks.HasAttribute(Task.Attribute.Category))
            {
                attrib.Add("Category");
                value.Add(task.FormatCategory(", "));
            }

            if (tasks.HasAttribute(Task.Attribute.Percent))
            {
                attrib.Add("Percent");
                value.Add(task.GetPercentDone(false).ToString());
            }

            if (tasks.HasAttribute(Task.Attribute.TimeEstimate))
            {
                attrib.Add("Time Estimate");

                Task.TimeUnits units = Task.TimeUnits.Hours;
                value.Add(task.GetTimeEstimate(ref units, false).ToString() + units);
            }

            if (tasks.HasAttribute(Task.Attribute.TimeSpent))
            {
                attrib.Add("Time Spent");

                Task.TimeUnits units = Task.TimeUnits.Hours;
                value.Add(task.GetTimeSpent(ref units, false).ToString() + units);
            }

            if (tasks.HasAttribute(Task.Attribute.FileReference))
            {
                attrib.Add("File Reference");
                value.Add(task.FormatFileReference(", "));
            }

            if (tasks.HasAttribute(Task.Attribute.Comments))
            {
                attrib.Add("Comments");
                value.Add(task.GetComments());
            }

            if (tasks.HasAttribute(Task.Attribute.Flag))
            {
                attrib.Add("Flag");
                value.Add(task.IsFlagged(false).ToString());
            }

            if (tasks.HasAttribute(Task.Attribute.CreationDate))
            {
                attrib.Add("Creation Date");
                value.Add(task.GetCreationDateString());
            }

            if (tasks.HasAttribute(Task.Attribute.CreatedBy))
            {
                attrib.Add("Created By");
                value.Add(task.GetCreatedBy());
            }

            if (tasks.HasAttribute(Task.Attribute.Risk))
            {
                attrib.Add("Risk");
                value.Add(task.GetRisk(false).ToString());
            }

            if (tasks.HasAttribute(Task.Attribute.ExternalId))
            {
                attrib.Add("External ID");
                value.Add(task.GetExternalID());
            }

            if (tasks.HasAttribute(Task.Attribute.Cost))
            {
                attrib.Add("Cost");
                value.Add(task.GetCost(false).ToString());
            }

            if (tasks.HasAttribute(Task.Attribute.Dependency))
            {
                attrib.Add("Dependency");
                value.Add(task.FormatDependency(", "));
            }

            if (tasks.HasAttribute(Task.Attribute.Recurrence))
            {
                attrib.Add("Recurrence");
                //value.Add(task.GetRecurrence());
            }

            if (tasks.HasAttribute(Task.Attribute.Version))
            {
                attrib.Add("Version");
                value.Add(task.GetVersion());
            }
			
            if (tasks.HasAttribute(Task.Attribute.Position))
            {
                attrib.Add("Position");
                value.Add(task.GetPositionString());
            }

            if (tasks.HasAttribute(Task.Attribute.Id))
            {
                attrib.Add("ID");
                value.Add(task.GetID().ToString());
            }

            if (tasks.HasAttribute(Task.Attribute.LastModifiedDate))
            {
                attrib.Add("Last Modified");
                //value.Add(task.GetLastModifiedString());
            }

            if (tasks.HasAttribute(Task.Attribute.Icon))
            {
                attrib.Add("Icon");
                value.Add(task.GetIcon());
            }

            if (tasks.HasAttribute(Task.Attribute.Tags))
            {
                attrib.Add("Tag");
                value.Add(task.FormatTag(", "));
            }

            if (tasks.HasAttribute(Task.Attribute.CustomAttribute))
            {
                attrib.Add("Custom Attribute");
                //value.Add(task.GetCustomAttributeData());
            }

            item.Attrib = String.Join(",", attrib.ToArray());
            item.Value = String.Join(",", value.ToArray());
            item.Tasks = task.GetID().ToString();

            switch (type)
            {
                case UIExtension.UpdateType.Edit: item.Type = "Edit"; break;
                case UIExtension.UpdateType.New: item.Type = "Add Task"; break;
                case UIExtension.UpdateType.Delete: item.Type = "Move Task(s)"; break;
                case UIExtension.UpdateType.All: item.Type = "All"; break;
                case UIExtension.UpdateType.Unknown: item.Type = "Unknown"; break;
            }

            m_Items.Add(item);
            m_ListView.Items.Refresh();
        }

        public bool WantTaskUpdate(Task.Attribute attrib)
	    {
			return true; // all updates
	    }

        public bool WantSortUpdate(Task.Attribute attrib)
        {
			return true; // all updates
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
