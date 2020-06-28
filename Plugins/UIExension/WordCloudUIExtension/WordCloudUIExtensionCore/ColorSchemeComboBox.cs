using System;
using System.Drawing;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;

using Abstractspoon.Tdl.PluginHelpers;
using Abstractspoon.Tdl.PluginHelpers.ColorUtil;

namespace WordCloudUIExtension
{
	[System.ComponentModel.DesignerCategory("")]

	class ColorSchemeComboBox : ComboBox
	{
		protected class ColorScheme
		{
			public ColorScheme(Color[] colors)
			{
				m_Colors = new List<Color>();

				foreach (Color color in colors)
					m_Colors.Add(color);
			}

			public ColorScheme(String[] hexColors)
			{
				m_Colors = new List<Color>();

				FromHexColors(hexColors);
			}

			public override string ToString()
			{
				StringBuilder hex = new StringBuilder();

				foreach (Color color in m_Colors)
				{
					hex.Append(ColorTranslator.ToHtml(color));
					hex.Append(',');
				}
				
				return hex.ToString();
			}

			private bool FromHexColors(String[] hexColors)
			{
				m_Colors.Clear();

				foreach (String color in hexColors)
				{
					if ((color.Length != 0) && (color[0] == '#'))
					{
						Color c = ColorTranslator.FromHtml(color);

						if (c != null)
							m_Colors.Add(c);
					}
				}

				return (m_Colors.Count > 0);
			}

			public List<Color> m_Colors { private set; get; }
		}

		public ColorSchemeComboBox()
		{
			Initialise();

			this.DropDownStyle = ComboBoxStyle.DropDownList;
			this.DrawMode = DrawMode.OwnerDrawFixed;
			this.MaxDropDownItems = 20;
			this.DropDownHeight = 200;
			this.ForeColor = Color.White;
			this.DoubleBuffered = true;
		}

		public void Initialise()
		{
			Items.Add(new ColorScheme(new String[] { "#CC0000", "#99FF00", "#FFCC00", "#3333FF" }));
			Items.Add(new ColorScheme(new String[] { "#76988F", "#AC8C18", "#3F3C29", "#262416" }));
			Items.Add(new ColorScheme(new String[] { "#097054", "#FFDE00", "#6599FF", "#FF9900" }));
			Items.Add(new ColorScheme(new String[] { "#FF3333", "#999999", "#669999", "#003333" }));
			Items.Add(new ColorScheme(new String[] { "#663300", "#CC9900", "#333300", "#CC6600" }));
			Items.Add(new ColorScheme(new String[] { "#FFD800", "#587058", "#587498", "#E86850" }));
			Items.Add(new ColorScheme(new String[] { "#669966", "#336699", "#FFFF00", "#990033" }));
			Items.Add(new ColorScheme(new String[] { "#7A3E48", "#EECD86", "#E18942", "#B95835", "#3D3242" }));
			Items.Add(new ColorScheme(new String[] { "#020731", "#3862C6", "#6E7587", "#806641", "#AE956D" }));
			Items.Add(new ColorScheme(new String[] { "#EBC137", "#E38C2D", "#DB4C2C", "#771E10", "#48110C" }));
			Items.Add(new ColorScheme(new String[] { "#EF597B", "#FF6D31", "#73B66B", "#FFCB18", "#29A2C6" }));
            Items.Add(new ColorScheme(new String[] { "#78C679", "#41AB5D", "#238443", "#006837", "#004529" }));
            Items.Add(new ColorScheme(new String[] { "#41B6C4", "#1D91C0", "#225EA8", "#253494", "#081D58" }));
            Items.Add(new ColorScheme(new String[] { "#7BCCC4", "#4EB3D3", "#2B8CBE", "#0868AC", "#084081" }));
            Items.Add(new ColorScheme(new String[] { "#66C2A4", "#41AE76", "#238B45", "#006D2C", "#00441B" }));
            Items.Add(new ColorScheme(new String[] { "#67A9CF", "#3690C0", "#02818A", "#016C59", "#014636" }));
            Items.Add(new ColorScheme(new String[] { "#74A9CF", "#3690C0", "#0570B0", "#045A8D", "#023858" }));
            Items.Add(new ColorScheme(new String[] { "#8C96C6", "#8C6BB1", "#88419D", "#810F7C", "#4D004B" }));
            Items.Add(new ColorScheme(new String[] { "#F768A1", "#DD3497", "#AE017E", "#7A0177", "#49006A" }));
            Items.Add(new ColorScheme(new String[] { "#DF65B0", "#E7298A", "#CE1256", "#980043", "#67001F" }));
            Items.Add(new ColorScheme(new String[] { "#FC8D59", "#EF6548", "#D7301F", "#B30000", "#7F0000" }));
            Items.Add(new ColorScheme(new String[] { "#FD8D3C", "#FC4E2A", "#E31A1C", "#BD0026", "#800026" }));
            Items.Add(new ColorScheme(new String[] { "#FE9929", "#EC7014", "#CC4C02", "#993404", "#662506" }));
            Items.Add(new ColorScheme(new String[] { "#9E9AC8", "#807DBA", "#6A51A3", "#54278F", "#3F007D" }));
            Items.Add(new ColorScheme(new String[] { "#6BAED6", "#4292C6", "#2171B5", "#08519C", "#08306B" }));
            Items.Add(new ColorScheme(new String[] { "#74C476", "#41AB5D", "#238B45", "#006D2C", "#00441B" }));
            Items.Add(new ColorScheme(new String[] { "#FD8D3C", "#F16913", "#D94801", "#A63603", "#7F2704" }));
            Items.Add(new ColorScheme(new String[] { "#FB6A4A", "#EF3B2C", "#CB181D", "#A50F15", "#67000D" }));
            Items.Add(new ColorScheme(new String[] { "#969696", "#737373", "#525252", "#252525", "#000000" }));
			Items.Add(new ColorScheme(new String[] { "#7EB5D6", "#2A75A9", "#274257", "#DFC184", "#8F6048", "#644436" }));
			Items.Add(new ColorScheme(new String[] { "#21B6A8", "#177F75", "#B6212D", "#7F171F", "#B67721", "#7F5417" }));
			Items.Add(new ColorScheme(new String[] { "#9DAF72", "#566047", "#562F32", "#462D44", "#859731", "#640E27" }));
			Items.Add(new ColorScheme(new String[] { "#00904B", "#64BD4F", "#E13987", "#F191BA", "#FEEA65", "#835322" }));
            Items.Add(new ColorScheme(new String[] { "#1B9E77", "#D95F02", "#7570B3", "#E7298A", "#66A61E", "#E6AB02", "#A6761D", "#666666" }));
            Items.Add(new ColorScheme(new String[] { "#E41A1C", "#377EB8", "#4DAF4A", "#984EA3", "#FF7F00", "#FFFF33", "#A65628", "#F781BF", "#999999" }));

			Items.Add(new ColorScheme(new Color[] { Color.DarkRed, Color.DarkBlue, Color.DarkGreen, Color.Navy, Color.DarkCyan, Color.DarkOrange, Color.DarkGoldenrod, Color.DarkKhaki, Color.Blue, Color.Red, Color.Green }));

// 			Items.Add(new ColorScheme(new String[] { "", "", "", "", "", "", "", "", "", "", "" }));
// 			Items.Add(new ColorScheme(new String[] { "", "", "", "", "", "", "", "", "", "", "" }));

			SelectedIndex = -1;
		}

		protected override void OnDrawItem(DrawItemEventArgs e)
		{
			base.OnDrawItem(e);
			
			e.DrawBackground();
			e.DrawFocusRectangle();

			if (e.Index > -1)
			{
				ColorScheme scheme = (ColorScheme)Items[e.Index];
				
				if (scheme != null)
				{
					Rectangle rect = e.Bounds;
					rect.Inflate(-2, -2);
					rect.Width = rect.Height;

					foreach (Color color in scheme.m_Colors)
					{
						using (Brush fill = new SolidBrush(color))
						{
							e.Graphics.FillRectangle(fill, rect);
						}

						using (Pen border = new Pen(DrawingColor.AdjustLighting(color, -0.2f, true)))
						{
							e.Graphics.DrawRectangle(border, rect);
						}

						// Next colour
						rect.Offset(rect.Height + 2, 0);
					}
				}
			}
		}

		public String GetSelectedSchemeAsString()
		{
			ColorScheme selItem = (ColorScheme)SelectedItem;

			if (selItem == null)
				return "";

			// else
			return selItem.ToString();
		}

		public Color[] GetSelectedScheme()
		{
			ColorScheme selItem = (ColorScheme)SelectedItem;

			if (selItem == null)
				return new Color[] { Color.Black };

			return selItem.m_Colors.ToArray();
		}

		public bool SetSelectedScheme(Color[] colors)
		{
			var item = FindScheme(colors);

			if (item == null)
			{
				// Create a new scheme for this arrangement
				item = new ColorScheme(colors);
				Items.Add(item);
			}

			SelectedItem = item;
			return true;
		}

		public bool SetSelectedScheme(String hex)
		{
			string[] hexColors = hex.Split(',');

			ColorScheme scheme = new ColorScheme(hexColors);

			if (scheme.m_Colors.Count == 0)
				return false;

			return SetSelectedScheme(scheme.m_Colors.ToArray());
		}

		protected ColorScheme FindScheme(Color[] colors)
		{
			if (colors.Length > 0)
			{
				foreach (ColorScheme scheme in Items)
				{
					if (colors.Length == scheme.m_Colors.Count)
					{
						int numMatches = 0;

						foreach (Color color in colors)
						{
							if (!scheme.m_Colors.Contains(color))
								break;

							// else
							numMatches++;
						}

						if (numMatches == colors.Length)
							return scheme;
					}
				}
			}

			// else
			return null;
		}
	}
}
