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

		private bool m_FindSelecting = false;

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

		public void RefreshMaxDropWidth()
		{
			var graphics = Graphics.FromHwnd(Handle);
			int maxWidth = 0;

			foreach (var item in Items)
			{
				int width = (GetListItemTextOffset(item) + GetListItemTextLength(item, graphics));

				maxWidth = Math.Max(width, maxWidth);
			}

			DropDownWidth = (maxWidth + SystemInformation.VerticalScrollBarWidth);
		}

		virtual protected int GetListItemTextOffset(object obj)
		{
			return 0;
		}

		virtual protected int GetListItemTextLength(object obj, Graphics graphics)
		{
			return (int)graphics.MeasureString(obj.ToString(), Font).Width;
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

		public bool SelectNextItem(String text, bool forward)
		{
			// Prevent re-entrancy
			if (m_FindSelecting)
				return false;

			int next = NoMatch, sel = SelectedIndex;

			if (!string.IsNullOrEmpty(text))
			{
				int from = (forward ? (sel + 1) : (sel - 1));
				next = FindNextItem(text, from, forward);
			}

			{
				m_FindSelecting = true;

				SelectedIndex = next;

				m_FindSelecting = false;
			}

			return ((next != NoMatch) && (next != sel));
		}

		protected int FindNextItem(String text, int from, bool forward)
		{
			int numItems = Items.Count;

			if (numItems == 0)
				return NoMatch;

			int next = NoMatch;

			if (forward)
			{
				if ((from < 0) || (from >= numItems))
					from = 0;

				// From from to end of combo
				if (FindNextItem(text, from, (numItems - 1), 1, ref next))
					return next;

				// From start of combo to from
				if ((from > 0) && FindNextItem(text, 0, (from - 1), 1, ref next))
					return next;
			}
			else // backwards
			{
				if ((from < 0) || (from >= numItems))
					from = (numItems - 1);

				// From from to start of combo
				if (FindNextItem(text, from, 0, -1, ref next))
					return next;

				// From end of combo to from
				if (FindNextItem(text, (numItems - 1), (from + 1), -1, ref next))
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
