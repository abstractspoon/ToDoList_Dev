using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

using Abstractspoon.Tdl.PluginHelpers;
using Abstractspoon.Tdl.PluginHelpers.ColorUtil;

namespace TaskDatesUIExtension
{
	public partial class TaskDatesPreferencesDlg : PreferencesFormBase
	{
		private Translator m_Trans;

		private HashSet<string> m_VisibleDateAttribIds;

		// ---------------------------------------------

		public TaskDatesPreferencesDlg(Translator trans)
		{
			m_Trans = trans;

			InitializeComponent();

			FormsUtil.SetFont(this, UIExtension.ControlFont());
			m_Trans.Translate(this);
		}

		public IEnumerable<string> SelectedDateAttributeIds
		{
			get { return m_VisibleDateAttribIds; }
		}

		public DialogResult ShowDialog(IEnumerable<TaskAttributeItem> dateAttribs)
		{
			m_VisibleDateListBox.Initialise(dateAttribs, m_VisibleDateAttribIds);

			var dlgRes = base.ShowDialog();

			switch (dlgRes)
			{
			case DialogResult.OK:
				m_VisibleDateAttribIds = m_VisibleDateListBox.SelectedDateAttributeIds;
				break;
			}

			return dlgRes;
		}

		public void LoadPreferences(Preferences prefs, String key)
		{
			var selIds = prefs.GetProfileString(key, "VisibleDateAttribIds", "StartDate|DueDate");
			m_VisibleDateAttribIds = new HashSet<string>(selIds.Split('|'));
		}

		public void SavePreferences(Preferences prefs, String key)
		{
			prefs.WriteProfileString(key, "VisibleDateAttribIds", string.Join("|", m_VisibleDateAttribIds));
		}

		// ------------------------------------------------------

		protected override void OnHandleCreated(EventArgs e)
		{
			base.OnHandleCreated(e);

			// TODO
		}

		private void OnMouseUpDateAttribListBox(object sender, MouseEventArgs e)
		{
			EnableOK(m_VisibleDateListBox.CheckedItems.Count > 0);
		}
	}
}


