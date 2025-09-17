using System;
using System.Windows.Forms;
using System.Drawing;

namespace UIComponents
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
			ForeColor = SystemColors.ControlText;
		}

		protected override void OnPaint(PaintEventArgs pe)
		{
			base.OnPaint(pe);

			{
				Rectangle rColor = ClientRectangle;
				rColor.Inflate(-5, -5);
				rColor.Offset(0, -((rColor.Height + 1) % 2)); // To match app

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
					pe.Graphics.DrawRectangle(SystemPens.GrayText, rColor);
				}
			}
		}

		protected override void OnClick(EventArgs e)
		{
			base.OnClick(e);

			using (var dlg = new ColorDialogEx())
			{
				dlg.FullOpen = true;
				dlg.AnyColor = true;
				dlg.SolidColorOnly = true;
				dlg.AllowFullOpen = true;
				dlg.Color = this.Color;

				if (dlg.ShowDialog() == DialogResult.OK)
				{
					this.Color = dlg.Color;
					Invalidate();
				}
			}

		}
	}
}
