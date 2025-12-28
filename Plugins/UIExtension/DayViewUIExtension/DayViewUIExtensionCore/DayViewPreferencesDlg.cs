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
		private IWin32Window m_Owner;

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
				return String.Format(trans.Translate("{0} minutes", Translator.Type.Text), numMinutes);
			}

			public int numMinutes;
			protected Translator trans;
		}

		// ------------------------------------------------------------------------

        public DayViewPreferencesDlg(IWin32Window owner, Translator trans, Font font)
        {
			m_Trans = trans;
			m_Owner = owner;
			
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
			prefs.WriteProfileString(prefsKey, "HideParentTasksTag", m_HideParentTasksTag.Text);

			prefs.WriteProfileBool(prefsKey, "DisplayContinuous", DisplayTasksContinuous);
			prefs.WriteProfileBool(prefsKey, "ShowActiveToday", DisplayActiveTasksToday);
			prefs.WriteProfileBool(prefsKey, "HideTasksWithoutTimes", HideTasksWithoutTimes);
            prefs.WriteProfileBool(prefsKey, "HideTasksSpanningWeekends", HideTasksSpanningWeekends);
            prefs.WriteProfileBool(prefsKey, "HideTasksSpanningDays", HideTasksSpanningDays);
			prefs.WriteProfileBool(prefsKey, "ShowFutureOccurrences", ShowFutureOccurrences);
			prefs.WriteProfileBool(prefsKey, "TreatOverdueTasksAsDueToday", TreatOverdueTasksAsDueToday);
			prefs.WriteProfileBool(prefsKey, "LegacyScrollbars", LegacyScrollbars);

			prefs.WriteProfileInt(prefsKey, "SlotMinutes", SlotMinutes);
			prefs.WriteProfileInt(prefsKey, "MinSlotHeight", MinSlotHeight);
			prefs.WriteProfileBool(prefsKey, "ShowWorkingHoursOnly", m_ShowWorkingHoursOnly.Checked);
		}

		public void LoadPreferences(Preferences prefs, String key)
        {
            string prefsKey = (key + "\\Preferences");

            m_HideParentTasks.Checked = prefs.GetProfileBool(prefsKey, "HideParentTasks", false);
			m_HideParentTasksByTag.Checked = prefs.GetProfileBool(prefsKey, "HideParentTasksByTag", false);
			m_HideParentTasksTag.Text = prefs.GetProfileString(prefsKey, "HideParentTasksTag", "");

			m_DisplayDiscontinuous.Checked = !prefs.GetProfileBool(prefsKey, "DisplayContinuous", true);
			m_ShowActiveToday.Checked = prefs.GetProfileBool(prefsKey, "ShowActiveToday", true);
			m_HideTasksWithoutTimes.Checked = prefs.GetProfileBool(prefsKey, "HideTasksWithoutTimes", false);
            m_HideTasksSpanningWeekends.Checked = prefs.GetProfileBool(prefsKey, "HideTasksSpanningWeekends", false);
            m_HideTasksSpanningDays.Checked = prefs.GetProfileBool(prefsKey, "HideTasksSpanningDays", false);
			m_ShowFutureOccurrences.Checked = prefs.GetProfileBool(prefsKey, "ShowFutureOccurrences", true);
			m_TreatOverdueTasksAsDueToday.Checked = prefs.GetProfileBool(prefsKey, "TreatOverdueTasksAsDueToday", true);
			m_LegacyScrollbars.Checked = prefs.GetProfileBool(prefsKey, "LegacyScrollbars", false);

			SlotMinutes = prefs.GetProfileInt(prefsKey, "SlotMinutes", 15);
			MinSlotHeight = prefs.GetProfileInt(prefsKey, "MinSlotHeight", 5);
			m_ShowWorkingHoursOnly.Checked = prefs.GetProfileBool(prefsKey, "ShowWorkingHoursOnly", true);
		}

		public new DialogResult ShowDialog()
		{
			return ShowDialog(m_Owner);
		}

		public new DialogResult ShowDialog(IWin32Window owner)
		{
			// Snapshot current state
			var orgHideParents = m_HideParentTasks.Checked;
			var orgParentByTag = m_HideParentTasksByTag.Checked;
			var orgParentTagText = m_HideParentTasksTag.Text;

			var orgDisplayCont = DisplayTasksContinuous;
			var orgDisplayToday = DisplayActiveTasksToday;
			var orgHideNoTimes = HideTasksWithoutTimes;
			var orgHideSpanWeekends = HideTasksSpanningWeekends;
			var orgHideSpanDays = HideTasksSpanningDays;
			var orgShowFuture = ShowFutureOccurrences;
			var orgSlotMins = SlotMinutes;
			var orgSlotHeight = MinSlotHeight;
			var orgWorkingOnly = ShowWorkingHoursOnly;
			var orgDueToday = TreatOverdueTasksAsDueToday;
			var orgLegacyScrollbars = LegacyScrollbars;

			// Enable states
			m_HideParentTasksByTag.Enabled = m_HideParentTasks.Checked;
			m_HideParentTasksTag.Enabled = m_HideParentTasksByTag.Enabled && m_HideParentTasksByTag.Checked;
			m_ShowActiveToday.Enabled = !DisplayTasksContinuous;

			var ret = base.ShowDialog(owner);

			if (ret != DialogResult.OK)
			{
				// Restore previous state
				m_HideParentTasks.Checked = orgHideParents;
				m_HideParentTasksByTag.Checked = orgParentByTag;
				m_HideParentTasksTag.Text = orgParentTagText;

				m_DisplayDiscontinuous.Checked = orgDisplayCont;
				m_ShowActiveToday.Checked = orgDisplayToday;
				m_HideTasksWithoutTimes.Checked = orgHideNoTimes;
				m_HideTasksSpanningWeekends.Checked = orgHideSpanWeekends;
				m_HideTasksSpanningDays.Checked = orgHideSpanDays;
				m_ShowFutureOccurrences.Checked = orgShowFuture;
				m_ShowWorkingHoursOnly.Checked = orgWorkingOnly;
				m_TreatOverdueTasksAsDueToday.Checked = orgDueToday;
				m_LegacyScrollbars.Checked = orgLegacyScrollbars;

				SlotMinutes = orgSlotMins;
				MinSlotHeight = orgSlotHeight;
			}

			return ret;
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
		public bool DisplayTasksContinuous { get { return !m_DisplayDiscontinuous.Checked; } }
		public bool DisplayActiveTasksToday { get { return m_ShowActiveToday.Checked; } }
		public bool ShowFutureOccurrences { get { return m_ShowFutureOccurrences.Checked; } }
		public bool ShowWorkingHoursOnly { get { return m_ShowWorkingHoursOnly.Checked; } }
		public bool TreatOverdueTasksAsDueToday { get { return m_TreatOverdueTasksAsDueToday.Checked; } }
		public bool LegacyScrollbars { get { return m_LegacyScrollbars.Checked; } }

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

		private void OnDisplayTasksContinuous(object sender, EventArgs e)
		{
			m_ShowActiveToday.Enabled = !DisplayTasksContinuous;
		}
	}
}
