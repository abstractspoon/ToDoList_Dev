using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace TodoTxtImpExp
{
	public partial class TodoTxtImporterOptionsForm : Form
	{
		public TodoTxtImporterOptionsForm()
		{
			InitializeComponent();
		}

		public bool ImportContextAsCategory		{ get { return m_ContextAsCategoryBtn.Checked; } }
		public bool ImportContextAsTag			{ get { return m_ContextAsTagBtn.Checked; } }

		public bool ImportProjectAsCategory		{ get { return m_ProjectAsCategoryBtn.Checked; } }
		public bool ImportProjectAsTag			{ get { return m_ProjectAsTagBtn.Checked; } }
		public bool ImportProjectAsParentTask	{ get { return m_ProjectAsParentTaskBtn.Checked; } }
	}
}
