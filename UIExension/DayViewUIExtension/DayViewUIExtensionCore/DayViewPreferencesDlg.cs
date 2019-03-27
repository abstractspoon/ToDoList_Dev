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
        public DayViewPreferencesDlg(Translator trans)
        {
            InitializeComponent();
            ScaleByDPIFactor();

            trans.Translate(this);
        }

        public void SavePreferences(Preferences prefs, String key)
		{
            string prefsKey = (key + "\\Preferences");

            prefs.WriteProfileBool(prefsKey, "HideParentTasks", HideParentTasks);
            prefs.WriteProfileBool(prefsKey, "HideTasksWithoutTimes", HideTasksWithoutTimes);
            prefs.WriteProfileBool(prefsKey, "HideTasksSpanningWeekends", HideTasksSpanningWeekends);
            prefs.WriteProfileBool(prefsKey, "HideTasksSpanningDays", HideTasksSpanningDays);
		}

        public void LoadPreferences(Preferences prefs, String key)
        {
            string prefsKey = (key + "\\Preferences");

            m_HideParentTasks.Checked = prefs.GetProfileBool(prefsKey, "HideParentTasks", true);
            m_HideTasksWithoutTimes.Checked = prefs.GetProfileBool(prefsKey, "HideTasksWithoutTimes", true);
            m_HideTasksSpanningWeekends.Checked = prefs.GetProfileBool(prefsKey, "HideTasksSpanningWeekends", false);
            m_HideTasksSpanningDays.Checked = prefs.GetProfileBool(prefsKey, "HideTasksSpanningDays", false);
        }

        public Boolean HideParentTasks { get { return m_HideParentTasks.Checked; } }
        public Boolean HideTasksWithoutTimes { get { return m_HideTasksWithoutTimes.Checked; } }
        public Boolean HideTasksSpanningWeekends { get { return m_HideTasksSpanningWeekends.Checked; } }
        public Boolean HideTasksSpanningDays { get { return m_HideTasksSpanningDays.Checked; } }

        protected void ScaleByDPIFactor()
        {
            if (DPIScaling.WantScaling())
            {
                SuspendLayout();

                this.Bounds = DPIScaling.Scale(this.Bounds);
                this.Width -= 10;
                this.Height -= 20;

                this.panel1.Bounds = DPIScaling.Scale(this.panel1.Bounds);
                this.m_HideParentTasks.Bounds = DPIScaling.Scale(this.m_HideParentTasks.Bounds);
                this.m_HideTasksWithoutTimes.Bounds = DPIScaling.Scale(this.m_HideTasksWithoutTimes.Bounds);
                this.m_HideTasksSpanningWeekends.Bounds = DPIScaling.Scale(this.m_HideTasksSpanningWeekends.Bounds);
                this.m_HideTasksSpanningDays.Bounds = DPIScaling.Scale(this.m_HideTasksSpanningDays.Bounds);
                this.BtnCancel.Bounds = DPIScaling.Scale(this.BtnCancel.Bounds);
                this.BtnOK.Bounds = DPIScaling.Scale(this.BtnOK.Bounds);

                // Make sure border is the same all the way around
//                 int leftBorder = this.panel1.Left;
//                 int topBorder = this.panel1.Top;
// 
//                 this.Width = this.panel1.Width + 2 * leftBorder;
//                 this.Height = this.BtnOK.Bottom + topBorder;

                ResumeLayout();
            }
        }
    }
}
