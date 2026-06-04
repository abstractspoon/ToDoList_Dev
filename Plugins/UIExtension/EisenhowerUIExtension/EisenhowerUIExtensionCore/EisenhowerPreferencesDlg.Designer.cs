namespace EisenhowerUIExtension
{
	partial class EisenhowerPreferencesDlg
	{
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.IContainer components = null;

		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		/// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
		protected override void Dispose(bool disposing)
		{
			if (disposing && (components != null))
			{
				components.Dispose();
			}
			base.Dispose(disposing);
		}

		#region Windows Form Designer generated code

		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
			System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(EisenhowerPreferencesDlg));
			this.rangeSliderCtrl1 = new Abstractspoon.Tdl.PluginHelpers.RangeSliderCtrl();
			this.SuspendLayout();
			// 
			// rangeSliderCtrl1
			// 
			this.rangeSliderCtrl1.Location = new System.Drawing.Point(26, 87);
			this.rangeSliderCtrl1.Name = "rangeSliderCtrl1";
			this.rangeSliderCtrl1.Size = new System.Drawing.Size(207, 23);
			this.rangeSliderCtrl1.TabIndex = 0;
			this.rangeSliderCtrl1.Text = "rangeSliderCtrl1";
			// 
			// EisenhowerPreferencesDlg
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(284, 261);
			this.Controls.Add(this.rangeSliderCtrl1);
			this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
			this.Name = "EisenhowerPreferencesDlg";
			this.ShowInTaskbar = false;
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
			this.Text = "EisenhowerPreferencesDlg";
			this.ResumeLayout(false);

		}

		#endregion

		private Abstractspoon.Tdl.PluginHelpers.RangeSliderCtrl rangeSliderCtrl1;
	}
}