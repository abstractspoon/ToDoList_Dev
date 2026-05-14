using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Diagnostics;
using System.Linq;
using System.Windows.Forms;
using System.Drawing;

using Abstractspoon.Tdl.PluginHelpers;

namespace EisenhowerUIExtension
{
	public partial class EisenhowerPaneListView : TaskListView
	{
		const int XColIndex = 1;
		const int YColIndex = 2;

		// --------------------------------------------------------

		private bool m_Selected;

		// --------------------------------------------------------

		public EisenhowerPaneListView()
		{
		}

		public bool Selected
		{
			get { return m_Selected; }

			set
			{
				if (m_Selected != value)
				{
					m_Selected = value;

					if (!m_Selected)
						SelectedIndices.Clear();

					Invalidate();
				}
			}
		}

		public uint HitTestTask(Point screenPos)
		{
			Point ptClient = PointToClient(screenPos);
			var lvHit = HitTest(ptClient);

			if (lvHit.Item != null)
				return GetTaskId(lvHit.Item.Index);

			// else
			return 0;
		}

		public uint GetTaskId(UIExtension.GetTask getTask)
		{
			return GetTaskIdEx(getTask, m_Selected);
		}

		public bool SelectTask(String text, UIExtension.SelectTask selectTask, bool caseSensitive, bool wholeWord, bool findReplace)
		{
			return SelectTaskEx(text, selectTask, caseSensitive, wholeWord, findReplace);
		}

		public string XAttribTitle
		{
			get { return Columns[XColIndex].Text; }
			set { Columns[XColIndex].Text = value; }
		}

		public string YAttribTitle
		{
			get { return Columns[YColIndex].Text; }
			set { Columns[YColIndex].Text = value; }
		}

		public uint FirstTaskId { get { return base.GetTaskId(0); } }
		public uint LastTaskId  { get { return base.GetTaskId(Items.Count - 1); } }

		// --------------------------------------------------------

		protected override bool IsItemSelected(ListViewItem lvItem)
		{
			return (m_Selected && base.IsItemSelected(lvItem));
		}
	}
}
