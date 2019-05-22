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
				return trans.Translate(String.Format("{0} minutes", numMinutes));
			}

			public int numMinutes;
			protected Translator trans;
		}

		// ------------------------------------------------------------------------

        public DayViewPreferencesDlg(Translator trans, Font font)
        {
			m_Trans = trans;
			
            InitializeComponent();

			DialogUtils.SetFont(this, font);
			m_Trans.Translate(this);

			// Build 'slot minutes' combo
			var slotMins = new int[] { 5, 10, 15, 20, 30, 60 };

			foreach (var mins in slotMins)
				m_SlotMinuteCombo.Items.Add(new SlotMinutesItem(mins, m_Trans));

			// Build 'slot height' combo
			var slotHeights = new int[] { 5, 10, 15, 20, 25 };

			foreach (var height in slotHeights)
				m_MinSlotHeightCombo.Items.Add(height);

			PopulateTimeOfDayCombo(m_StartOfDay);
			PopulateTimeOfDayCombo(m_StartOfLunch);
			PopulateTimeOfDayCombo(m_EndOfLunch);
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

			prefs.WriteProfileDouble(prefsKey, "StartOfDay", GetTimeInHours(m_StartOfDay));
			prefs.WriteProfileDouble(prefsKey, "EndOfLunch", GetTimeInHours(m_EndOfLunch));
			prefs.WriteProfileDouble(prefsKey, "StartOfLunch", GetTimeInHours(m_StartOfLunch));
		}

		public void LoadPreferences(Preferences prefs, String key)
        {
            string prefsKey = (key + "\\Preferences");

            m_HideParentTasks.Checked = prefs.GetProfileBool(prefsKey, "HideParentTasks", true);
            m_HideTasksWithoutTimes.Checked = prefs.GetProfileBool(prefsKey, "HideTasksWithoutTimes", true);
            m_HideTasksSpanningWeekends.Checked = prefs.GetProfileBool(prefsKey, "HideTasksSpanningWeekends", false);
            m_HideTasksSpanningDays.Checked = prefs.GetProfileBool(prefsKey, "HideTasksSpanningDays", false);
			m_WantLunchBreak.Checked = prefs.GetProfileBool(prefsKey, "WantLunchBreak", true);

			SlotMinutes = prefs.GetProfileInt(prefsKey, "SlotMinutes", 15);
			MinSlotHeight = prefs.GetProfileInt(prefsKey, "MinSlotHeight", 5);

			SetTimeInHours(m_StartOfDay, prefs.GetProfileDouble(prefsKey, "StartOfDay", 9));
			SetTimeInHours(m_StartOfLunch, prefs.GetProfileDouble(prefsKey, "StartOfLunch", 12.5));
			SetTimeInHours(m_EndOfLunch, prefs.GetProfileDouble(prefsKey, "EndOfLunch", 13.5));
		}

		public bool HideParentTasks { get { return m_HideParentTasks.Checked; } }
        public bool HideTasksWithoutTimes { get { return m_HideTasksWithoutTimes.Checked; } }
        public bool HideTasksSpanningWeekends { get { return m_HideTasksSpanningWeekends.Checked; } }
        public bool HideTasksSpanningDays { get { return m_HideTasksSpanningDays.Checked; } }

		public double HoursInWorkingDay { get; set; }
		
		public double StartOfDayInHours { get { return GetTimeInHours(m_StartOfDay); } }
		public double EndOfDayInHours
		{
			get
			{
				double endOfDay = (StartOfDayInHours + HoursInWorkingDay);

				if (m_WantLunchBreak.Checked)
				{
					double lunchBreak = (GetTimeInHours(m_EndOfLunch) - GetTimeInHours(m_StartOfLunch));

					if (lunchBreak > 0)
						endOfDay += lunchBreak;
				}

				return endOfDay;
			}
		}

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

		private void OnWantLunchBreak_CheckedChanged(object sender, EventArgs e)
		{
			m_StartOfLunch.Enabled = m_WantLunchBreak.Checked;
			m_EndOfLunch.Enabled = m_WantLunchBreak.Checked;
		}

		private void PopulateTimeOfDayCombo(ComboBox combo)
		{
			// Once only
			if (combo.Items.Count != 0)
				return;

			for (int hour = 0; hour < 24; hour++)
			{
				String hourStr = new DateTime(1, 1, 1, hour, 0, 0).ToString("t", System.Globalization.CultureInfo.CurrentCulture);
				String halfHourStr = new DateTime(1, 1, 1, hour, 30, 0).ToString("t", System.Globalization.CultureInfo.CurrentCulture);

				combo.Items.Add(hourStr);
				combo.Items.Add(halfHourStr);
			}
		}

		private double GetTimeInHours(ComboBox combo)
		{
			DateTime time;

			if (DateTime.TryParse(combo.Text, out time))
				return (time.Hour + (time.Minute / 60.0));

			return -1;
		}

		private void SetTimeInHours(ComboBox combo, double hours)
		{
			int hour = (int)hours;
			int minute = (int)((hours - hour) * 60);

			combo.Text = new DateTime(1, 1, 1, hour, minute, 0).ToString("t", System.Globalization.CultureInfo.CurrentCulture);
		}
	}
}
