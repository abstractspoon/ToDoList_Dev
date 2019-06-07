using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace MSDN.Html.Editor.Classes
{
	public partial class ColorButton : Button
	{
		public Color Color
		{
			get; set;
		}

		public ColorButton()
		{
			InitializeComponent();

			Color = Color.Empty;
		}

		protected override void OnPaint(PaintEventArgs pe)
		{
			base.OnPaint(pe);

			{
				Rectangle rColor = new Rectangle(ClientRectangle.Location, ClientRectangle.Size);
				rColor.Inflate(-5, -5);
				//rColor.Offset(0, -((rColor.Height + 1) % 2));

				if (this.TextAlign != ContentAlignment.TopRight &&
					this.TextAlign != ContentAlignment.MiddleRight &&
					this.TextAlign != ContentAlignment.BottomRight)
				{
					rColor.X = (rColor.Right - Math.Max(12, rColor.Height));
				}

				rColor.Width = Math.Max(12, rColor.Height);

				if (Color != Color.Empty)
				{
					pe.Graphics.FillRectangle(new SolidBrush(Color), rColor);

					// darker border
					const float darker = 0.7f;
					Color borderColor = Color.FromArgb(255,	(int)(Color.R * darker), (int)(Color.G * darker), (int)(Color.B * darker));
					pe.Graphics.DrawRectangle(new Pen(borderColor), rColor);
				}
				else
				{
					pe.Graphics.DrawRectangle(SystemPens.ControlDarkDark, rColor);
				}
			}
		}

		protected override void OnClick(EventArgs e)
		{
			base.OnClick(e);

			using (ColorDialog colorDialog = new ColorDialog())
			{
				colorDialog.AnyColor = true;
				colorDialog.SolidColorOnly = true;
				colorDialog.AllowFullOpen = true;
				colorDialog.Color = this.Color;
				//colorDialog.CustomColors = _customColors;

				if (colorDialog.ShowDialog(/*this.ParentForm*/) == DialogResult.OK)
				{
					//_customColors = colorDialog.CustomColors;
					this.Color = colorDialog.Color;
					Invalidate();
				}
			}

		}
	}
}
