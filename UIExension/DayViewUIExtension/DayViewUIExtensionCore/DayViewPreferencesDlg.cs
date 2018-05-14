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

            this.Text = trans.Translate(this.Text);

            m_HideParentTasks.Text = trans.Translate(m_HideParentTasks.Text);
            m_HideTasksWithoutTimes.Text = trans.Translate(m_HideTasksWithoutTimes.Text);
            m_HideTasksSpanningWeekends.Text = trans.Translate(m_HideTasksSpanningWeekends.Text);
            m_HideTasksSpanningDays.Text = trans.Translate(m_HideTasksSpanningDays.Text);

            BtnCancel.Text = trans.Translate(BtnCancel.Text);
            BtnOK.Text = trans.Translate(BtnOK.Text);
            
            ScaleByDPIFactor();
        }

        public void SavePreferences(Preferences prefs, String key)
		{
            string prefsKey = (key + "\\Preferences");

            prefs.WriteProfileInt(prefsKey, "HideParentTasks", (HideParentTasks ? 1 : 0));
            prefs.WriteProfileInt(prefsKey, "HideTasksWithoutTimes", (HideTasksWithoutTimes ? 1 : 0));
            prefs.WriteProfileInt(prefsKey, "HideTasksSpanningWeekends", (HideTasksSpanningWeekends ? 1 : 0));
            prefs.WriteProfileInt(prefsKey, "HideTasksSpanningDays", (HideTasksSpanningDays ? 1 : 0));
		}

        public void LoadPreferences(Preferences prefs, String key)
        {
            string prefsKey = (key + "\\Preferences");

            m_HideParentTasks.Checked = (prefs.GetProfileInt(prefsKey, "HideParentTasks", 1) != 0);
            m_HideTasksWithoutTimes.Checked = (prefs.GetProfileInt(prefsKey, "HideTasksWithoutTimes", 1) != 0);
            m_HideTasksSpanningWeekends.Checked = (prefs.GetProfileInt(prefsKey, "HideTasksSpanningWeekends", 0) != 0);
            m_HideTasksSpanningDays.Checked = (prefs.GetProfileInt(prefsKey, "HideTasksSpanningDays", 0) != 0);
        }

        public Boolean HideParentTasks { get { return m_HideParentTasks.Checked; } }
        public Boolean HideTasksWithoutTimes { get { return m_HideTasksWithoutTimes.Checked; } }
        public Boolean HideTasksSpanningWeekends { get { return m_HideTasksSpanningWeekends.Checked; } }
        public Boolean HideTasksSpanningDays { get { return m_HideTasksSpanningDays.Checked; } }

        protected void ScaleByDPIFactor()
        {
            if (Win32.WantScaleByDPIFactor())
            {
                SuspendLayout();

                this.Bounds = Win32.ScaleRectByDPIFactor(this.Bounds);
                this.Width -= 10;
                this.Height -= 20;

                this.panel1.Bounds = Win32.ScaleRectByDPIFactor(this.panel1.Bounds);
                this.m_HideParentTasks.Bounds = Win32.ScaleRectByDPIFactor(this.m_HideParentTasks.Bounds);
                this.m_HideTasksWithoutTimes.Bounds = Win32.ScaleRectByDPIFactor(this.m_HideTasksWithoutTimes.Bounds);
                this.m_HideTasksSpanningWeekends.Bounds = Win32.ScaleRectByDPIFactor(this.m_HideTasksSpanningWeekends.Bounds);
                this.m_HideTasksSpanningDays.Bounds = Win32.ScaleRectByDPIFactor(this.m_HideTasksSpanningDays.Bounds);
                this.BtnCancel.Bounds = Win32.ScaleRectByDPIFactor(this.BtnCancel.Bounds);
                this.BtnOK.Bounds = Win32.ScaleRectByDPIFactor(this.BtnOK.Bounds);

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
