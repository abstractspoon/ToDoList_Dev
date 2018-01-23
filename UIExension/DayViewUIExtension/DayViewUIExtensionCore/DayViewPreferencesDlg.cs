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
        public DayViewPreferencesDlg()
        {
            InitializeComponent();
        }

        public void SavePreferences(Preferences prefs, String key)
		{
            string prefsKey = (key + "\\Preferences");

            prefs.WriteProfileInt(prefsKey, "HideTasksWithoutTimes", (HideTasksWithoutTimes ? 1 : 0));
            prefs.WriteProfileInt(prefsKey, "HideTasksSpanningWeekends", (HideTasksSpanningWeekends ? 1 : 0));
            prefs.WriteProfileInt(prefsKey, "HideTasksSpanningDays", (HideTasksSpanningDays ? 1 : 0));
		}

        public void LoadPreferences(Preferences prefs, String key)
        {
            string prefsKey = (key + "\\Preferences");

            m_HideTasksWithoutTimes.Checked = (prefs.GetProfileInt(prefsKey, "HideTasksWithoutTimes", 1) != 0);
            m_HideTasksSpanningWeekends.Checked = (prefs.GetProfileInt(prefsKey, "HideTasksSpanningWeekends", 0) != 0);
            m_HideTasksSpanningDays.Checked = (prefs.GetProfileInt(prefsKey, "HideTasksSpanningDays", 0) != 0);
        }

        public Boolean HideTasksWithoutTimes { get { return m_HideTasksWithoutTimes.Checked; } }
        public Boolean HideTasksSpanningWeekends { get { return m_HideTasksSpanningWeekends.Checked; } }
        public Boolean HideTasksSpanningDays { get { return m_HideTasksSpanningDays.Checked; } }
    }
}
