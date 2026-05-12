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

		// --------------------------------------------------------

		protected override bool IsItemSelected(ListViewItem lvItem)
		{
			return (m_Selected && base.IsItemSelected(lvItem));
		}

	}
}
