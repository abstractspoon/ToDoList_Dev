using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace MSHtmlEditorDemo
{
	public partial class Form1 : Form
	{
		public Form1()
		{
			InitializeComponent();

			CenterToScreen();

			if (String.IsNullOrEmpty(htmlEditorControl1.InnerHtml))
				htmlEditorControl1.InnerHtml = @"<TABLE cellSpacing=2 cols=3 cellPadding=1 width=""50%"" border=2></TABLE>";
		}
	}
}
