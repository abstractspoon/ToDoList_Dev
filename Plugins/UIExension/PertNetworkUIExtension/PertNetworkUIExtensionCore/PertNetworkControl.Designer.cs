using System;
using System.Windows.Forms;

namespace PertNetworkUIExtension
{
    partial class PertNetworkControl
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

        #region Component Designer generated code

        /// <summary> 
        /// Required method for Designer support - do not modify 
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.SuspendLayout();

//             this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
//             this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Name = "PertNetworkControl";
            this.Size = new System.Drawing.Size(708, 398);
            this.BackColor = System.Drawing.SystemColors.Window;
			this.View = View.Details;
			this.HideSelection = true;
			this.HeaderStyle = ColumnHeaderStyle.None;
			this.OwnerDraw = true;

			this.ResumeLayout(false);
            this.PerformLayout();

			DragTimer = new Timer();
			DragTimer.Interval = (int)GetDoubleClickTime();
			DragTimer.Tick += new EventHandler(OnDragTimer);

// 			this.AutoScroll = true;
			this.AllowDrop = true;
			this.ReadOnly = false;

			SetStyle(ControlStyles.AllPaintingInWmPaint, true);
 			SetStyle(ControlStyles.EnableNotifyMessage, true);
			SetStyle(ControlStyles.OptimizedDoubleBuffer, true);
			SetStyle(ControlStyles.ResizeRedraw, true);

			base.BorderStyle = BorderStyle.None;
		}

		#endregion

    }
}
