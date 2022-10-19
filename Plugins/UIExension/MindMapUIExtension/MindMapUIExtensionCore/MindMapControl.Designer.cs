using System;
using System.Windows.Forms;

namespace MindMapUIExtension
{
    partial class MindMapControl
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
            this.m_TreeView = new System.Windows.Forms.TreeView();
            this.SuspendLayout();
            // 
            // DebugMode
            // 
#if DEBUG
            this.m_DebugMode = new System.Windows.Forms.CheckBox();
            this.m_DebugMode.AutoSize = true;
            this.m_DebugMode.Location = new System.Drawing.Point(0, 0);
            this.m_DebugMode.Name = "DebugMode";
            this.m_DebugMode.Size = new System.Drawing.Size(87, 17);
            this.m_DebugMode.TabIndex = 0;
            this.m_DebugMode.Text = "Debug mode";
            this.m_DebugMode.UseVisualStyleBackColor = true;

			m_DebugMode.CheckedChanged += new EventHandler(OnDebugModeChanged);
			m_DebugMode.Font = this.Font;
 
            this.Controls.Add(this.m_DebugMode);
#endif
            // 
            // TreeView
            // 
            this.m_TreeView.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left)));
            this.m_TreeView.Location = new System.Drawing.Point(0, 23);
            this.m_TreeView.Name = "TreeView";
            this.m_TreeView.Size = new System.Drawing.Size(300, 375);
            this.m_TreeView.TabIndex = 1;

			if (!DebugMode())
				m_TreeView.Visible = false;

			m_TreeView.AfterExpand += new TreeViewEventHandler(OnTreeViewAfterExpandCollapse);
			m_TreeView.AfterCollapse += new TreeViewEventHandler(OnTreeViewAfterExpandCollapse);
			m_TreeView.AfterSelect += new TreeViewEventHandler(OnTreeViewAfterSelect);
			m_TreeView.BeforeCollapse += new TreeViewCancelEventHandler(OnTreeViewBeforeCollapse);

			this.Controls.Add(this.m_TreeView);
            // 
            // MindMapControl
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Name = "MindMapControl";
            this.Size = new System.Drawing.Size(708, 398);
            this.BackColor = System.Drawing.SystemColors.Window;
            this.ResumeLayout(false);
            this.PerformLayout();

			m_DragTimer = new Timer();
			m_DragTimer.Interval = SystemInformation.DoubleClickTime;
			m_DragTimer.Tick += new EventHandler(OnDragTimer);

			this.AutoScroll = true;
			this.AllowDrop = true;
			this.ReadOnly = false;

			SetStyle(ControlStyles.AllPaintingInWmPaint, true);
			SetStyle(ControlStyles.UserPaint, true);
			SetStyle(ControlStyles.OptimizedDoubleBuffer, true);
			SetStyle(ControlStyles.ResizeRedraw, true);

			base.BorderStyle = BorderStyle.None;
		}

		#endregion

#if DEBUG
		private System.Windows.Forms.CheckBox m_DebugMode;
#endif
        private System.Windows.Forms.TreeView m_TreeView;
    }
}
