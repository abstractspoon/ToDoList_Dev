
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

        public void SetBackgroundColor(System.Windows.Media.Color bkColor)
        {
            this.Background = new System.Windows.Media.SolidColorBrush(bkColor);
        }

        private void InitializeComponent()
        {
            this.Background = System.Windows.Media.Brushes.White;

            CreateListView();
            PopulateListView();
        }

        private void CreateListView()
        {
            listView = new System.Windows.Controls.ListView();
            gridView = new System.Windows.Controls.GridView();

            AttribCol = new System.Windows.Controls.GridViewColumn();
            AttribCol.DisplayMemberBinding = new System.Windows.Data.Binding("Attrib");
            AttribCol.Header = "Attribute Changed";
            AttribCol.Width = 200;
            gridView.Columns.Add(AttribCol);

            ValueCol = new System.Windows.Controls.GridViewColumn();
            ValueCol.DisplayMemberBinding = new System.Windows.Data.Binding("Value");
            ValueCol.Header = "New Value";
            ValueCol.Width = 200;
            gridView.Columns.Add(ValueCol);

            TasksCol = new System.Windows.Controls.GridViewColumn();
            TasksCol.DisplayMemberBinding = new System.Windows.Data.Binding("Tasks");
            TasksCol.Header = "Tasks Changed";
            TasksCol.Width = 200;
            gridView.Columns.Add(TasksCol);

            listView.View = gridView;

            this.Children.Add(listView);
        }

        private void PopulateListView()
        {
            System.Collections.Generic.List<SampleListItem> items = new System.Collections.Generic.List<SampleListItem>();

            listView.ItemsSource = items;

            items.Add(new SampleListItem() { Attrib = "Item1Col1", Value = "Item1Col2", Tasks = "Item1Col3" });
            items.Add(new SampleListItem() { Attrib = "Item2Col1", Value = "Item2Col2", Tasks = "Item2Col3" });
            items.Add(new SampleListItem() { Attrib = "Item3Col1", Value = "Item3Col2", Tasks = "Item3Col3" });
        }

        private System.Windows.Controls.ListView listView;
        private System.Windows.Controls.GridView gridView;
        private System.Windows.Controls.GridViewColumn AttribCol;
        private System.Windows.Controls.GridViewColumn ValueCol;
        private System.Windows.Controls.GridViewColumn TasksCol;
    }
}
