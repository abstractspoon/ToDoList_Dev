using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Diagnostics;
using System.Linq;
using System.Windows.Forms;

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

					// Change title font to Bold
					// TODO

					Invalidate();
				}
			}
		}

		// --------------------------------------------------------

		protected override bool IsItemSelected(ListViewItem lvItem)
		{
			return (m_Selected && base.IsItemSelected(lvItem));
		}

	}
}
