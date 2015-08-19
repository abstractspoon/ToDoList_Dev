
namespace SampleUIExtension
{
    public class SampleUIExtensionCore : System.Windows.Controls.Canvas
    {
        public SampleUIExtensionCore()
        {
            InitializeComponent();
        }

        private void InitializeComponent()
        {
            this.label = new System.Windows.Controls.Label();

            this.label.Content = "WPF Label -- it works!";
            this.label.Background = System.Windows.Media.Brushes.Red;
            this.label.Visibility = System.Windows.Visibility.Visible;
            this.label.Width = 200;
            this.label.Height = 40;

            this.Children.Add(label);
            System.Windows.Controls.Canvas.SetLeft(this.label, 30);
            System.Windows.Controls.Canvas.SetTop(this.label, 30);

            this.Background = System.Windows.Media.Brushes.White;

//             this.listView1 = new System.Windows.Forms.ListView();
//             this.Attribute = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
//             this.Value = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
// //            this.SuspendLayout();
//             // 
//             // listView1
//             // 
//             this.listView1.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
//             this.Attribute,
//             this.Value});
//             this.listView1.Location = new System.Drawing.Point(0, 0);
//             this.listView1.MultiSelect = false;
//             this.listView1.Name = "listView1";
//             this.listView1.Size = new System.Drawing.Size(260, 238);
//             this.listView1.TabIndex = 0;
//             this.listView1.UseCompatibleStateImageBehavior = false;
//             this.listView1.View = System.Windows.Forms.View.Details;
//             this.listView1.Visible = true;
//             // 
//             // Attribute
//             // 
//             this.Attribute.Width = 118;
//             // 
//             // Value
//             // 
//             this.Value.Width = 134;
//             // 
//             // Form1
//             // 
// //             this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
// //             this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
// //             this.ClientSize = new System.Drawing.Size(284, 262);
//              this.AddVisualChild(this.listView1);
// //             this.Name = "Form1";
// //             this.Text = "Form1";
// //             this.ResumeLayout(false);
// //             this.Visible = true;
        }

        private System.Windows.Controls.Label label;
//         private System.Windows.Forms.ListView listView1;
//         private System.Windows.Forms.ColumnHeader Attribute;
//         private System.Windows.Forms.ColumnHeader Value;
    }
}
