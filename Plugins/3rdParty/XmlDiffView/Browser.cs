using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using System.Windows.Forms.Html;
using System.IO;
namespace VisualXmlDiff
{
	/// <summary>
	/// Summary description for Form1.
	/// </summary>
	public class Browser : System.Windows.Forms.Form
	{
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.Container components = null;
		HtmlControl hc = new HtmlControl();
		string navigateTo = null;

		public Browser( string url )
		{
			navigateTo = url;
			InitializeComponent();

			this.Controls.Add( hc );
		}

		private Browser()
		{
			//
			// Required for Windows Form Designer support
			//
			InitializeComponent();

			//
			// TODO: Add any constructor code after InitializeComponent call
			//
		}

		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		protected override void Dispose( bool disposing )
		{
			if( disposing )
			{
				if(components != null)
				{
					components.Dispose();
				}
			}
			base.Dispose( disposing );
		}

		#region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
			this.SuspendLayout();
			// 
			// Browser
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(533, 390);
			this.Icon = global::VisualXmlDiff.Resource.XmlDiffView;
			this.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
			this.Name = "Browser";
			this.Text = "Xml File Differences";
			this.Closed += new System.EventHandler(this.Browser_Closed);
			this.Load += new System.EventHandler(this.Browser_Load);
			this.Resize += new System.EventHandler(this.Browser_Resize);
			this.ResumeLayout(false);

		}
		#endregion

		private void Browser_Load(object sender, System.EventArgs e)
		{
            hc.Size = this.ClientSize;

            if (!Path.IsPathRooted(navigateTo))
            {
			    string currPath = AppDomain.CurrentDomain.BaseDirectory ;
			    hc.Navigate( "file:///" + currPath + navigateTo );
            }
            else
            {
			    hc.Navigate( "file:///" + navigateTo );
            }

			hc.Show();
		}

		private void Browser_Closed(object sender, System.EventArgs e)
		{
			if ( File.Exists ( navigateTo ) )
			{
				File.Delete( navigateTo );
			}
		}

		private void Browser_Resize(object sender, System.EventArgs e)
		{
			hc.Size = this.ClientSize;
		}
	}
}
