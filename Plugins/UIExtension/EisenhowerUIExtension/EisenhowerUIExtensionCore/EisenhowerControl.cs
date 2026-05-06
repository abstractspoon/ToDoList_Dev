using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace EisenhowerUIExtension
{
	public partial class EisenhowerControl : UserControl
	{
		const int SplitWidth = 6;

		// ---------------------------------------------

		PointF m_splitPos = new PointF(0.5f, 0.5f); // 0.0f-1.0f

		// ---------------------------------------------

		public EisenhowerControl()
		{
			InitializeComponent();
		}

		// Message handlers --------------------------------

		protected override void OnSizeChanged(EventArgs e)
		{
			base.OnSizeChanged(e);

			const int HalfSplit = (SplitWidth / 2);

			// Top-left
			Rectangle topLeftRect = ClientRectangle;

			topLeftRect.Width = (int)(m_splitPos.X * Width) - HalfSplit;
			topLeftRect.Height = (int)(m_splitPos.Y * Height) - HalfSplit;

			m_TopLeftPane.Bounds = topLeftRect;

			// Top-right
			Rectangle topRightRect = ClientRectangle;

			topRightRect.X = topLeftRect.Right + SplitWidth;
			topRightRect.Y = topLeftRect.Y;
			topRightRect.Width = (Width - (topLeftRect.Width + SplitWidth));
			topRightRect.Height = topLeftRect.Height;

			m_TopRightPane.Bounds = topRightRect;
			
			// Bottom-left
			Rectangle botLeftRect = ClientRectangle;

			botLeftRect.Y = topLeftRect.Bottom + SplitWidth;
			botLeftRect.Width = topLeftRect.Width;
			botLeftRect.Height = (Height - (topLeftRect.Height + SplitWidth));

			m_BotLeftPane.Bounds = botLeftRect;

			// Bottom-right
			Rectangle botRightRect = ClientRectangle;

			botRightRect.X = topRightRect.X;
			botRightRect.Y = botLeftRect.Y;
			botRightRect.Width = topRightRect.Width;
			botRightRect.Height = botLeftRect.Height;

			m_BotRightPane.Bounds = botRightRect;
		}

	}
}
