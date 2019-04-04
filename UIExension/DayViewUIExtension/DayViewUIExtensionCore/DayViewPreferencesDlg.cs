using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Linq;
using System.Text;
using System.Drawing;
using System.Windows.Forms;

using Abstractspoon.Tdl.PluginHelpers;

namespace DayViewUIExtension
{
    public partial class DayViewPreferencesDlg : Form
    {
        public DayViewPreferencesDlg(Translator trans, Font font)
        {
            InitializeComponent();

			DialogUtils.SetFont(this, font);
			trans.Translate(this);

			// Build 'slot's combo
			for (int numSlots = 1; numSlots <= 12; numSlots++)
			{
				if (TDLDayView.IsValidSlotsPerHour(numSlots))
					m_NumSlotsCombo.Items.Add(new Slot(numSlots));
			}

			// Build 'slot height' combo
			// TODO
		}

        public void SavePreferences(Preferences prefs, String key)
		{
            string prefsKey = (key + "\\Preferences");

            prefs.WriteProfileBool(prefsKey, "HideParentTasks", HideParentTasks);
            prefs.WriteProfileBool(prefsKey, "HideTasksWithoutTimes", HideTasksWithoutTimes);
            prefs.WriteProfileBool(prefsKey, "HideTasksSpanningWeekends", HideTasksSpanningWeekends);
            prefs.WriteProfileBool(prefsKey, "HideTasksSpanningDays", HideTasksSpanningDays);

			var slot = (m_NumSlotsCombo.SelectedItem as Slot);

			if (slot != null)
				prefs.WriteProfileInt(prefsKey, "SlotsPerHour", slot.numSlots);
		}

        public void LoadPreferences(Preferences prefs, String key)
        {
            string prefsKey = (key + "\\Preferences");

            m_HideParentTasks.Checked = prefs.GetProfileBool(prefsKey, "HideParentTasks", true);
            m_HideTasksWithoutTimes.Checked = prefs.GetProfileBool(prefsKey, "HideTasksWithoutTimes", true);
            m_HideTasksSpanningWeekends.Checked = prefs.GetProfileBool(prefsKey, "HideTasksSpanningWeekends", false);
            m_HideTasksSpanningDays.Checked = prefs.GetProfileBool(prefsKey, "HideTasksSpanningDays", false);

			int numSlots = prefs.GetProfileInt(prefsKey, "SlotsPerHour", 4);

			if (!TDLDayView.IsValidSlotsPerHour(numSlots))
				numSlots = 4;

			int index = m_NumSlotsCombo.FindStringExact(Slot.FormatSlotText(numSlots));

			if (index != -1)
				m_NumSlotsCombo.SelectedIndex = index;
		}

        public Boolean HideParentTasks { get { return m_HideParentTasks.Checked; } }
        public Boolean HideTasksWithoutTimes { get { return m_HideTasksWithoutTimes.Checked; } }
        public Boolean HideTasksSpanningWeekends { get { return m_HideTasksSpanningWeekends.Checked; } }
        public Boolean HideTasksSpanningDays { get { return m_HideTasksSpanningDays.Checked; } }

		public int SlotsPerHour
		{
			get
			{
				Slot slot = (m_NumSlotsCombo.SelectedItem as Slot);
				return ((slot == null) ? 4 : slot.numSlots);
			}
		}

		protected class Slot
		{
			public Slot(int slots)
			{
				text = FormatSlotText(slots);
				numSlots = slots;
			}

			public override String ToString()
			{
				return text;
			}

			public static String FormatSlotText(int numSlots)
			{
				return String.Format("{0} ({1} minutes)", numSlots, (60 / numSlots));
			}

			public String text;
			public int numSlots;
		}
	}
}
