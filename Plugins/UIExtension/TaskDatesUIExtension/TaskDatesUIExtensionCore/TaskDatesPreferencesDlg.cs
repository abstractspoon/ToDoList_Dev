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
	public partial class TaskDatesPreferencesDlg : Form
	{
		private Translator m_Trans;

		private HashSet<string> m_VisibleDateAttribIds;
		private string m_OffsetAttribId = TaskDatesControl.TodayAttributeId;

		// ---------------------------------------------

		public TaskDatesPreferencesDlg(Translator trans)
		{
			m_Trans = trans;

			InitializeComponent();

			m_Error.ForeColor = DrawingColor.GetErrorLabelTextColor(BackColor);
		}

		public IEnumerable<string> SelectedDateAttributeIds
		{
			get { return m_VisibleDateAttribIds; }
		}

		public string SelectedOffsetAttributeId
		{
			get { return m_OffsetAttribId; }
		}

		public DialogResult ShowDialog(IEnumerable<TaskAttributeItem> dateAttribs,
									   IEnumerable<TaskAttributeItem> dateOffsetAttribs)
		{
			m_VisibleDateListBox.Initialise(dateAttribs, m_VisibleDateAttribIds);
			m_OffsetDateComboBox.Initialise(dateOffsetAttribs, m_OffsetAttribId);

			var dlgRes = base.ShowDialog();

			switch (dlgRes)
			{
			case DialogResult.OK:
				m_VisibleDateAttribIds = m_VisibleDateListBox.SelectedDateAttributeIds;
				m_OffsetAttribId = m_OffsetDateComboBox.SelectedOffsetAttributeId;
				break;
			}

			return dlgRes;
		}

		public void LoadPreferences(Preferences prefs, String key)
		{
			var selIds = prefs.GetProfileString(key, "VisibleDateAttribIds", "StartDate|DueDate");
			m_VisibleDateAttribIds = new HashSet<string>(selIds.Split('|'));

			m_OffsetAttribId = prefs.GetProfileString(key, "OffsetDateAttribId", TaskDatesControl.TodayAttributeId);
		}

		public void SavePreferences(Preferences prefs, String key)
		{
			prefs.WriteProfileString(key, "VisibleDateAttribIds", string.Join("|", m_VisibleDateAttribIds));
			prefs.WriteProfileString(key, "OffsetDateAttribId", m_OffsetAttribId);
		}

		// ------------------------------------------------------

		protected override void OnHandleCreated(EventArgs e)
		{
			base.OnHandleCreated(e);

			// TODO
		}
	}
}


;