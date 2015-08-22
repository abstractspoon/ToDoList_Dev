
using System;

// PLS DON'T ADD 'USING' STATEMENTS WHILE I AM STILL LEARNING!

namespace SampleUIExtension
{
    public class SampleListItem
    {
        public string Attrib { get; set; }
        public string Value { get; set; }
        public string Tasks { get; set; }
    }

    public class SampleUIExtensionCore : System.Windows.Controls.Canvas
    {
        public SampleUIExtensionCore()
        {
            InitializeComponent();
        }

        public void SetTheme(TDLPluginHelpers.TDLTheme theme)
        {
            System.Windows.Media.Color bkColor = theme.GetAppColor(TDLPluginHelpers.TDLTheme.AppColor.AppBackDark);

            this.Background = new System.Windows.Media.SolidColorBrush(bkColor);
        }

        // IUIExtension -------------------------------------------------------------------------

        public bool SelectTask(UInt32 dwTaskID)
        {
            return false;
        }

	    public bool SelectTasks(UInt32[] pdwTaskIDs, int nTaskCount)
        {
            return false;
        }

	    public void UpdateTasks(TDLPluginHelpers.TDLTaskList tasks, 
                                TDLPluginHelpers.TDLUIExtension.UpdateType type, 
                                TDLPluginHelpers.TDLUIExtension.TaskAttribute attrib)
        {
        }

        public bool WantUpdate(TDLPluginHelpers.TDLUIExtension.TaskAttribute attrib)
	    {
			return true; // all updates
	    }
	   
        public bool PrepareNewTask(TDLPluginHelpers.TDLTaskList task)
	    {
		    return false;
    	}

	    public bool ProcessMessage(IntPtr hwnd, UInt32 message, UInt32 wParam, UInt32 lParam, UInt32 time, System.Windows.Point pt)
	    {
		    return false;
	    }
	   
		public void DoAppCommand(TDLPluginHelpers.TDLUIExtension.AppCommand cmd, UInt32 dwExtra)
		{
		}

	    public bool CanDoAppCommand(TDLPluginHelpers.TDLUIExtension.AppCommand cmd, UInt32 dwExtra)
	    {
		    return false;
	    }

	    public bool GetLabelEditRect(System.Windows.Rect pEdit)
	    {
			return false;
	    }

        public TDLPluginHelpers.TDLUIExtension.HitTest HitTest(System.Windows.Point ptScreen)
	    {
            return TDLPluginHelpers.TDLUIExtension.HitTest.Nowhere;
	    }

        public void SetReadOnly(bool bReadOnly)
	    {
	    }

	    public void SavePreferences(TDLPluginHelpers.TDLPreferences prefs, String key)
	    {
	    }

	    public void LoadPreferences(TDLPluginHelpers.TDLPreferences prefs, String key, bool appOnly)
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

            m_Items.Add(new SampleListItem() { Attrib = "Item1Col1", Value = "Item1Col2", Tasks = "Item1Col3" });
            m_Items.Add(new SampleListItem() { Attrib = "Item2Col1", Value = "Item2Col2", Tasks = "Item2Col3" });
            m_Items.Add(new SampleListItem() { Attrib = "Item3Col1", Value = "Item3Col2", Tasks = "Item3Col3" });
        }



        // --------------------------------------------------------------------------------------
        private System.Collections.Generic.List<SampleListItem> m_Items;
        private System.Windows.Controls.ListView m_ListView;
        private System.Windows.Controls.GridView m_GridView;
        private System.Windows.Controls.GridViewColumn m_AttribCol;
        private System.Windows.Controls.GridViewColumn m_ValueCol;
        private System.Windows.Controls.GridViewColumn m_TasksCol;
    }
}
