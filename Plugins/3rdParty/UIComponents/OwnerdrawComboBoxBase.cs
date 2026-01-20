using System;
using System.Collections.Generic;
using System.Linq;
using System.Drawing;
using System.Diagnostics;
using System.Windows.Forms;

namespace UIComponents
{
	[System.ComponentModel.DesignerCategory("")]

	public class OwnerdrawComboBoxBase : ComboBox
	{
		const int NoMatch = -1;

		// ------------------------------------

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

		public bool SelectNextItem(String text, bool bForward)
		{
			if (string.IsNullOrEmpty(text))
			{
				SelectedIndex = NoMatch;
				return false;
			}

			// else
			int nSel = SelectedIndex;
			int from = (bForward ? (nSel + 1) : (nSel - 1));

			int nNext = FindNextItem(text, from, bForward);

			if (nNext == nSel)
				return false;

			SelectedIndex = nNext;
			return true;
		}

		protected int FindNextItem(String text, int from, bool bForward)
		{
			int nNumItems = Items.Count;

			if (nNumItems == 0)
				return NoMatch;

			int next = NoMatch;

			if (bForward)
			{
				if ((from < 0) || (from >= nNumItems))
					from = 0;

				// From from to end of combo
				if (FindNextItem(text, from, (nNumItems - 1), 1, ref next))
					return next;

				// From start of combo to from
				if ((from > 0) && FindNextItem(text, 0, (from - 1), 1, ref next))
					return next;
			}
			else // backwards
			{
				if ((from < 0) || (from >= nNumItems))
					from = (nNumItems - 1);

				// From from to start of combo
				if (FindNextItem(text, from, 0, -1, ref next))
					return next;

				// From end of combo to from
				if (FindNextItem(text, (nNumItems - 1), (from + 1), -1, ref next))
					return next;
			}

			// else
			return NoMatch;
		}

		protected bool FindNextItem(String text, int from, int to, int increment, ref int next)
		{
			// Sanity checks
			if (!(((increment == 1) && (to >= from)) || ((increment == -1) && (to <= from))))
			{
				Debug.Assert(false);
				return false;
			}

			next = NoMatch;
			to += increment; // so the != will work to stop the loop

			for (int nItem = from; nItem != to; nItem += increment)
			{
				var itemText = Items[nItem].ToString();

				if (itemText.IndexOf(text, StringComparison.InvariantCultureIgnoreCase) != NoMatch)
				{
					next = nItem;
					break;
				}
			}

			return (next != NoMatch);
		}
	}
}
