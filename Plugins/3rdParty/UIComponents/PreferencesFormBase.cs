using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace UIComponents
{
	public partial class PreferencesFormBase : Form
	{
		public PreferencesFormBase()
		{
			InitializeComponent();

			m_Panel.Paint += (s, e) =>
			{
				using (var pen = new Pen(Color.FromArgb(192, 192, 192)))
					e.Graphics.DrawRectangle(pen, 0, 0, m_Panel.Width - 1, m_Panel.Height - 1);
			};

			m_Panel.SizeChanged += (s, e) =>
			{
				m_Panel.Invalidate(false);
			};
		}
	}
}
