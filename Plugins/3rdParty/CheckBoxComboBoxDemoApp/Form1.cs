using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using PresentationControls;

namespace DemoApp
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private StatusList _StatusList;

        private ListSelectionWrapper<Status> StatusSelections;

        private void Form1_Load(object sender, EventArgs e)
        {

            PopulateManualCombo();
            
            #region POPULATED USING A CUSTOM "IList" DATASOURCE

            _StatusList = new StatusList();

            _StatusList.Add(new Status(1, "New"));
            _StatusList.Add(new Status(2, "Loaded"));
            _StatusList.Add(new Status(3, "Inserted"));
            Status UpdatedStatus = new Status(4, "Updated");
            _StatusList.Add(UpdatedStatus);
            _StatusList.Add(new Status(5, "Deleted"));

            StatusSelections = new ListSelectionWrapper<Status>(_StatusList, "Name");

            cmbIListDataSource.DataSource = StatusSelections;
            cmbIListDataSource.DisplayMemberSingleItem = "Name";
            cmbIListDataSource.DisplayMember = "NameConcatenated";
            cmbIListDataSource.ValueMember = "Selected";

            cmbIListDataSource.CheckBoxItems[3].DataBindings.DefaultDataSourceUpdateMode
                = DataSourceUpdateMode.OnPropertyChanged;
            cmbIListDataSource.DataBindings.DefaultDataSourceUpdateMode 
                = DataSourceUpdateMode.OnPropertyChanged;

            StatusSelections.FindObjectWithItem(UpdatedStatus).Selected = true;

            #endregion
            
            #region POPULATED USING A DATATABLE

            DataTable DT = new DataTable("TEST TABLE FOR DEMO PURPOSES");
            DT.Columns.AddRange(
                new DataColumn[]
                {
                    new DataColumn("Id", typeof(int)),
                    new DataColumn("SomePropertyOrColumnName", typeof(string)),                    
                    new DataColumn("Description", typeof(string)),
                });
            DT.Rows.Add(1, "AAAA", "AAAAA");
            DT.Rows.Add(2, "BBBB", "BBBBB");
            DT.Rows.Add(3, "CCCC", "CCCCC");
            DT.Rows.Add(3, "DDDD", "DDDDD");

            cmbDataTableDataSource.DataSource = 
                new ListSelectionWrapper<DataRow>(
                    DT.Rows,
                    "SomePropertyOrColumnName" // "SomePropertyOrColumnName" will populate the Name on ObjectSelectionWrapper.
                    ); 
            cmbDataTableDataSource.DisplayMemberSingleItem = "Name"; 
            cmbDataTableDataSource.DisplayMember = "NameConcatenated";
            cmbDataTableDataSource.ValueMember = "Selected";

            #endregion
             
        }

        private void PopulateManualCombo()
        {
            cmbManual.Items.Add("Item 1");
            cmbManual.Items.Add("Item 2");
            cmbManual.Items.Add("Item 3");
            cmbManual.Items.Add("Item 4");
            cmbManual.Items.Add("Item 5");
            cmbManual.Items.Add("Item 6");
            cmbManual.Items.Add("Item 7");
            cmbManual.Items.Add("Item 8");

            cmbManual.CheckBoxItems[1].Checked = true;

			cmbManual.EmptyPrompt = "<any>";
        }

        private void btnCheckItem1_Click(object sender, EventArgs e)
        {
            cmbManual.CheckBoxItems["Item 1"].Checked = !cmbManual.CheckBoxItems["Item 1"].Checked;
        }

        private void btnCheckItem5_Click(object sender, EventArgs e)
        {
            checkBoxComboBox1.CheckBoxItems["Item 5"].Checked = !checkBoxComboBox1.CheckBoxItems["Item 5"].Checked;
        }

        private void btnCheckDDDD_Click(object sender, EventArgs e)
        {
            cmbDataTableDataSource.CheckBoxItems["DDDD"].Checked = !cmbDataTableDataSource.CheckBoxItems["DDDD"].Checked;
        }

        private void btnCheckInserted_Click(object sender, EventArgs e)
        {
            cmbIListDataSource.CheckBoxItems["Inserted"].Checked = !cmbIListDataSource.CheckBoxItems["Inserted"].Checked;
        }

        private void btnClear_Click(object sender, EventArgs e)
        {
            cmbManual.Clear();

            PopulateManualCombo();
        }
    }
}
