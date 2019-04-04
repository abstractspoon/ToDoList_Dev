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
		protected class SlotMinutesItem
		{
			public SlotMinutesItem(int minutes)
			{
				numMinutes = minutes;
				text = String.Format("{0} minutes", minutes);
			}

			public override String ToString()
			{
				return text;
			}

			private String text;
			public int numMinutes;
		}

        public DayViewPreferencesDlg(Translator trans, Font font)
        {
            InitializeComponent();

			DialogUtils.SetFont(this, font);
			trans.Translate(this);

			// Build 'slot's combo
			for (int numSlots = 1; numSlots <= 12; numSlots++)
			{
				if (TDLDayView.IsValidSlotsPerHour(numSlots))
					m_SlotMinuteCombo.Items.Add(new SlotMinutesItem(60 / numSlots));
			}

			// Build 'slot height' combo
			var slotHeights = new int[] { 5, 10, 15, 20, 25 };

			foreach (var height in slotHeights)
				m_MinSlotHeightCombo.Items.Add(height);
		}

		public void SavePreferences(Preferences prefs, String key)
		{
            string prefsKey = (key + "\\Preferences");

            prefs.WriteProfileBool(prefsKey, "HideParentTasks", HideParentTasks);
            prefs.WriteProfileBool(prefsKey, "HideTasksWithoutTimes", HideTasksWithoutTimes);
            prefs.WriteProfileBool(prefsKey, "HideTasksSpanningWeekends", HideTasksSpanningWeekends);
            prefs.WriteProfileBool(prefsKey, "HideTasksSpanningDays", HideTasksSpanningDays);

			prefs.WriteProfileInt(prefsKey, "SlotMinutes", SlotMinutes);
			prefs.WriteProfileInt(prefsKey, "MinSlotHeight", MinSlotHeight);
		}

		public void LoadPreferences(Preferences prefs, String key)
        {
            string prefsKey = (key + "\\Preferences");

            m_HideParentTasks.Checked = prefs.GetProfileBool(prefsKey, "HideParentTasks", true);
            m_HideTasksWithoutTimes.Checked = prefs.GetProfileBool(prefsKey, "HideTasksWithoutTimes", true);
            m_HideTasksSpanningWeekends.Checked = prefs.GetProfileBool(prefsKey, "HideTasksSpanningWeekends", false);
            m_HideTasksSpanningDays.Checked = prefs.GetProfileBool(prefsKey, "HideTasksSpanningDays", false);

			SlotMinutes = prefs.GetProfileInt(prefsKey, "SlotMinutes", 15);
			MinSlotHeight = prefs.GetProfileInt(prefsKey, "MinSlotHeight", 5);

		}

		public Boolean HideParentTasks { get { return m_HideParentTasks.Checked; } }
        public Boolean HideTasksWithoutTimes { get { return m_HideTasksWithoutTimes.Checked; } }
        public Boolean HideTasksSpanningWeekends { get { return m_HideTasksSpanningWeekends.Checked; } }
        public Boolean HideTasksSpanningDays { get { return m_HideTasksSpanningDays.Checked; } }

		public int SlotMinutes
		{
			get
			{
				int numMins = 15;

				if (m_SlotMinuteCombo.SelectedItem != null)
					numMins = (int)m_MinSlotHeightCombo.SelectedItem;

				return numMins;
			}

			set
			{
				int numMins = 15;

				if (TDLDayView.IsValidSlotsPerHour(60 / value))
					numMins = value;

				int index = m_SlotMinuteCombo.FindStringExact(numMins.ToString());

				if (index != -1)
					m_SlotMinuteCombo.SelectedIndex = index;
			}
		}

		public int MinSlotHeight
		{
			get
			{
				int height = 5;

				if (m_MinSlotHeightCombo.SelectedItem != null)
					height = (int)m_MinSlotHeightCombo.SelectedItem;

				return DPIScaling.Scale(height);
			}

			set
			{
				int minSlotHeight = ((value / 5) * 5);
				minSlotHeight = Math.Min(30, minSlotHeight);
				minSlotHeight = Math.Max(5, minSlotHeight);

				int index = m_MinSlotHeightCombo.FindStringExact(minSlotHeight.ToString());

				if (index != -1)
					m_MinSlotHeightCombo.SelectedIndex = index;
			}
		}

	}
}
