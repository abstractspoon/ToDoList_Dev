using System;
using System.Collections.Generic;
using System.Linq;
using System.Drawing;
using System.Windows.Forms;

namespace UIComponents
{
	[System.ComponentModel.DesignerCategory("")]

	public class OwnerdrawComboBoxBase : ComboBox
	{
		public OwnerdrawComboBoxBase(bool drawFixed = true)
		{
			this.DrawMode = (drawFixed ? DrawMode.OwnerDrawFixed : DrawMode.OwnerDrawVariable);
		}

		public Color TextColor(DrawItemEventArgs e)
		{
			return (Enabled ? e.ForeColor : SystemColors.ControlDarkDark);
		}

		public Brush TextBrush(DrawItemEventArgs e)
		{
			return new SolidBrush(TextColor(e));
		}

		protected override void OnDrawItem(DrawItemEventArgs e)
		{
			base.OnDrawItem(e);
			
			if (Enabled)
			{
				e.DrawBackground();
				e.DrawFocusRectangle();
			}
			else
			{
				using (var brush = new SolidBrush(SystemColors.Control))
					e.Graphics.FillRectangle(brush, e.Bounds);
			}
		}
	}
}
