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
    partial class DayViewPreferencesDlg : Form
    {
		private Translator m_Trans;
		// ------------------------------------------------------------------------

		protected class SlotMinutesItem
		{
			public SlotMinutesItem(int minutes, Translator trans)
			{
				numMinutes = minutes;
				this.trans = trans;
			}

			public override String ToString()
			{
				return ToString(numMinutes, trans);
			}

			public static String ToString(int numMinutes, Translator trans)
			{
				return String.Format(trans.Translate("{0} minutes"), numMinutes);
			}

			public int numMinutes;
			protected Translator trans;
		}

		// ------------------------------------------------------------------------

        public DayViewPreferencesDlg(Translator trans, Font font)
        {
			m_Trans = trans;
			
            InitializeComponent();

			FormsUtil.SetFont(this, font);
			m_Trans.Translate(this);

			// Build 'slot minutes' combo
			var slotMins = new int[] { 5, 10, 15, 20, 30, 60 };

			foreach (var mins in slotMins)
				m_SlotMinuteCombo.Items.Add(new SlotMinutesItem(mins, m_Trans));

			// Build 'slot height' combo
			var slotHeights = new int[] { 5, 10, 15, 20, 25 };

			foreach (var height in slotHeights)
				m_MinSlotHeightCombo.Items.Add(height);
		}

		public void SavePreferences(Preferences prefs, String key)
		{
            string prefsKey = (key + "\\Preferences");

            prefs.WriteProfileBool(prefsKey, "HideParentTasks", m_HideParentTasks.Checked);
            prefs.WriteProfileBool(prefsKey, "HideParentTasksByTag", m_HideParentTasksByTag.Checked);
            prefs.WriteProfileBool(prefsKey, "DisplayContinuous", DisplayTasksContinuous);
            prefs.WriteProfileBool(prefsKey, "HideTasksWithoutTimes", HideTasksWithoutTimes);
            prefs.WriteProfileBool(prefsKey, "HideTasksSpanningWeekends", HideTasksSpanningWeekends);
            prefs.WriteProfileBool(prefsKey, "HideTasksSpanningDays", HideTasksSpanningDays);
			prefs.WriteProfileBool(prefsKey, "ShowFutureOccurrences", ShowFutureOccurrences);

			prefs.WriteProfileString(prefsKey, "HideParentTasksTag", m_HideParentTasksTag.Text);

			prefs.WriteProfileInt(prefsKey, "SlotMinutes", SlotMinutes);
			prefs.WriteProfileInt(prefsKey, "MinSlotHeight", MinSlotHeight);
		}

		public void LoadPreferences(Preferences prefs, String key)
        {
            string prefsKey = (key + "\\Preferences");

            m_HideParentTasks.Checked = prefs.GetProfileBool(prefsKey, "HideParentTasks", true);
			m_HideParentTasksByTag.Checked = prefs.GetProfileBool(prefsKey, "HideParentTasksByTag", false);
			m_DisplayContinuous.Checked = prefs.GetProfileBool(prefsKey, "DisplayContinuous", true);
			m_HideTasksWithoutTimes.Checked = prefs.GetProfileBool(prefsKey, "HideTasksWithoutTimes", true);
            m_HideTasksSpanningWeekends.Checked = prefs.GetProfileBool(prefsKey, "HideTasksSpanningWeekends", false);
            m_HideTasksSpanningDays.Checked = prefs.GetProfileBool(prefsKey, "HideTasksSpanningDays", false);
			m_ShowFutureOcurrences.Checked = prefs.GetProfileBool(prefsKey, "ShowFutureOccurrences", true);

			m_HideParentTasksTag.Text = prefs.GetProfileString(prefsKey, "HideParentTasksTag", "");

			SlotMinutes = prefs.GetProfileInt(prefsKey, "SlotMinutes", 15);
			MinSlotHeight = prefs.GetProfileInt(prefsKey, "MinSlotHeight", 5);

			// Enable states
			m_HideParentTasksByTag.Enabled = m_HideParentTasks.Checked;
			m_HideParentTasksTag.Enabled = m_HideParentTasksByTag.Enabled && m_HideParentTasksByTag.Checked;
		}

		public bool GetHideParentTasks(out string tag)
		{
			tag = string.Empty;

			if (!m_HideParentTasks.Checked)
				return false;

			if (m_HideParentTasksByTag.Checked)
				tag = m_HideParentTasksTag.Text;

			return true;
		}

        public bool HideTasksWithoutTimes { get { return m_HideTasksWithoutTimes.Checked; } }
        public bool HideTasksSpanningWeekends { get { return m_HideTasksSpanningWeekends.Checked; } }
        public bool HideTasksSpanningDays { get { return m_HideTasksSpanningDays.Checked; } }
		public bool DisplayTasksContinuous { get { return m_DisplayContinuous.Checked; } }
		public bool ShowFutureOccurrences { get { return m_ShowFutureOcurrences.Checked; } }

		public int SlotMinutes
		{
			get
			{
				int numMins = 15;
				var selSlot = (m_SlotMinuteCombo.SelectedItem as SlotMinutesItem);

				if (selSlot != null)
					numMins = selSlot.numMinutes;

				return numMins;
			}

			protected set // internal use only
			{
				int numMins = 15;

				if (TDLDayView.IsValidSlotsPerHour(60 / value))
					numMins = value;

				int index = m_SlotMinuteCombo.FindStringExact(SlotMinutesItem.ToString(numMins, m_Trans));

				if (index != -1)
					m_SlotMinuteCombo.SelectedIndex = index;
			}
		}

		public int MinSlotHeight
		{
			get
			{
				if (m_MinSlotHeightCombo.SelectedItem != null)
					return (int)m_MinSlotHeightCombo.SelectedItem;

				// else
				return 5;
			}

			protected set // internal use only
			{
				int minSlotHeight = ((value / 5) * 5);
				minSlotHeight = Math.Min(30, minSlotHeight);
				minSlotHeight = Math.Max(5, minSlotHeight);

				int index = m_MinSlotHeightCombo.FindStringExact(minSlotHeight.ToString());

				if (index != -1)
					m_MinSlotHeightCombo.SelectedIndex = index;
			}
		}

		public bool IncrementMinSlotHeight()
		{
			int oldSlotheight = MinSlotHeight;
			MinSlotHeight = (oldSlotheight + 5);

			return (MinSlotHeight != oldSlotheight);
		}

		public bool DecrementMinSlotHeight()
		{
			int oldSlotheight = MinSlotHeight;
			MinSlotHeight = (oldSlotheight - 5);

			return (MinSlotHeight != oldSlotheight);
		}

		private void OnHideParentTasks(object sender, EventArgs e)
		{
			m_HideParentTasksByTag.Enabled = m_HideParentTasks.Checked;
			m_HideParentTasksTag.Enabled = m_HideParentTasks.Checked && m_HideParentTasksByTag.Checked;
		}

		private void OnHideParentTasksByTag(object sender, EventArgs e)
		{
			m_HideParentTasksTag.Enabled = m_HideParentTasksByTag.Checked;
		}
	}
}
