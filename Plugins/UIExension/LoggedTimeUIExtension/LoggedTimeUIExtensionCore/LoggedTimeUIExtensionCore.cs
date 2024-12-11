﻿
using System;
using System.Diagnostics;
using System.Reflection;
using System.Linq;
using System.Drawing;
using System.Windows.Forms;
using System.Windows.Forms.VisualStyles;

using UIComponents;

using Abstractspoon.Tdl.PluginHelpers;
using Abstractspoon.Tdl.PluginHelpers.ColorUtil;

namespace LoggedTimeUIExtension
{

	[System.ComponentModel.DesignerCategory("")]
	public class LoggedTimeUIExtensionCore : Panel, IUIExtension
	{
		private IntPtr m_HwndParent = IntPtr.Zero;
		private LoggedTimeView m_TimeLog = null;
		private Translator m_Trans = null;
		private String m_TypeId, m_UiName;
		private WorkingWeek m_WorkWeek = null;

		private const string FontName = "Tahoma";

		private static Color DefGridColor = Color.FromArgb(192, 192, 192);
        private static int LabelTop = DPIScaling.Scale(2);
        private static int ComboTop = (LabelTop + DPIScaling.Scale(2));

		private bool m_SettingMonthYear = false;
		private bool m_SettingTimeLogStartDate = false;

		private WeekLabel m_WeekLabel;
		private MonthComboBox m_MonthCombo;
		private YearComboBox m_YearCombo;
        private LoggedTimePreferencesDlg m_PrefsDlg;

        private IIControls.ToolStripEx m_Toolbar;
		private ImageList m_TBImageList;
		private UIThemeToolbarRenderer m_ToolbarRenderer;
//		private LinkLabelEx.LinkLabelEx m_SelectedTaskDatesLabel;
		private Font m_ControlsFont;

// 		private LogEntry m_DefaultNewLogEntryAttributes;
		
		// --------------------------------------------------------------------------------------

		public LoggedTimeUIExtensionCore(String typeID, String uiName, IntPtr hwndParent, Translator trans)
		{
			m_HwndParent = hwndParent;
			m_Trans = trans;
			m_TypeId = typeID;
			m_UiName = uiName;

// 			m_DefaultNewLogEntryAttributes = new LogEntry(0);

			InitializeComponent();
		}
		
		public bool SelectTask(UInt32 dwTaskID)
		{
			return false; // not supported
		}

		public bool SelectTasks(UInt32[] pdwTaskIDs)
		{
			return false; // not supported
		}

		public void UpdateTasks(TaskList tasks, UIExtension.UpdateType type)
		{
			m_TimeLog.UpdateTasks(tasks, type);
		}

		public bool WantTaskUpdate(Task.Attribute attrib)
		{
			switch (attrib)
			{
			case Task.Attribute.Title:
			case Task.Attribute.AllocatedTo:
			case Task.Attribute.Icon:
			case Task.Attribute.Color:
				return true;
			}

			// all else
			return false;
		}

		public bool WantSortUpdate(Task.Attribute attrib)
		{
			return false; // not supported
		}
	   
		public bool PrepareNewTask(ref Task task)
		{
			return false; // not supported
		}

		public bool DoIdleProcessing()
		{
			return m_TimeLog.DoIdleProcessing();
		}

		public bool ProcessMessage(IntPtr hwnd, UInt32 message, UInt32 wParam, UInt32 lParam, UInt32 time, Int32 xPos, Int32 yPos)
		{
			const int WM_KEYDOWN = 0x0100;

			switch (message)
			{
			case WM_KEYDOWN:
				{
					Keys keys = (Keys)wParam;

					switch (keys)
					{
					case Keys.Escape:
						if (m_TimeLog.CancelAppointmentResizing())
						{
							UpdateToolbarButtonStates();
							return true;
						}
						break;

					case Keys.Delete:
						OnDeleteLogEntry(this, null);
						return true;

					case (Keys.Control | Keys.F2):
						OnEditLogEntry(this, null);
						return true;
					}
				}
				break;
			}

			return false;
		}

		public bool GetLabelEditRect(ref Int32 left, ref Int32 top, ref Int32 right, ref Int32 bottom)
		{
            return false; // not supported
		}

		public UIExtension.HitTestResult HitTest(Int32 xScreen, Int32 yScreen, UIExtension.HitTestReason reason)
		{
			return m_TimeLog.HitTest(xScreen, yScreen, reason);
		}

		public UInt32 HitTestTask(Int32 xScreen, Int32 yScreen, UIExtension.HitTestReason reason)
		{
			return 0; // not supported
		}

		public void SetUITheme(UITheme theme)
		{
			BackColor = m_Toolbar.BackColor = theme.GetAppDrawingColor(UITheme.AppColor.AppBackLight);

			// Set the toolbar colors to be the same as the back color
			theme.SetAppDrawingColor(UITheme.AppColor.ToolbarDark, BackColor);
			theme.SetAppDrawingColor(UITheme.AppColor.ToolbarLight, BackColor);

			m_TimeLog.SetUITheme(theme);
			m_ToolbarRenderer.SetUITheme(theme);

			m_WeekLabel.ForeColor = theme.GetAppDrawingColor(UITheme.AppColor.AppText);
//			m_SelectedTaskDatesLabel.ForeColor = m_WeekLabel.ForeColor;
		}

		public void SetTaskFont(String faceName, int pointSize)
		{
			m_TimeLog.SetFont(faceName, pointSize);
		}

		public void SetReadOnly(bool bReadOnly)
		{
            m_TimeLog.ReadOnly = bReadOnly;
		}

		public void SavePreferences(Preferences prefs, String key)
		{
// 			prefs.WriteProfileEnum(key, "DefaultTimeBlockEditMask", m_DefaultTimeBlockEditMask);
// 			prefs.WriteProfileString(key, "DefaultNewTimeBlockAttributes", m_DefaultNewLogEntryAttributes.Encode());

			m_TimeLog.SavePreferences(prefs, key);
			m_PrefsDlg.SavePreferences(prefs, key);

			prefs.WriteProfileInt(key, "DaysShowing", m_TimeLog.DaysShowing);
		}

		public void LoadPreferences(Preferences prefs, String key, bool appOnly)
		{
			m_TimeLog.TaskColorIsBackground = prefs.GetProfileBool("Preferences", "ColorTaskBackground", false);
			m_TimeLog.ShowParentsAsFolder = prefs.GetProfileBool("Preferences", "ShowParentsAsFolders", false);
            m_TimeLog.StrikeThruDoneTasks = prefs.GetProfileBool("Preferences", "StrikethroughDone", true);
			m_TimeLog.ShowLabelTips = !prefs.GetProfileBool("Preferences", "ShowInfoTips", false);
			m_TimeLog.DisplayDatesInISO = prefs.GetProfileBool("Preferences", "DisplayDatesInISO", false);

			m_WorkWeek.Load(prefs);
            m_TimeLog.WeekendDays = m_WorkWeek.WeekendDays();

            int gridColor = -1;

            if (prefs.GetProfileBool("Preferences", "SpecifyGridColor", true))
                gridColor = prefs.GetProfileInt("Preferences\\Colors", "GridLines", -1);

            m_TimeLog.GridlineColor = ((gridColor == -1) ? DefGridColor : DrawingColor.ToColor((UInt32)gridColor));
            
            if (appOnly)
			{
				UpdateWorkingHourDisplay();
				//UpdatedSelectedTaskDatesText();
			}
			else
            {
				// private settings
// 				m_DefaultTimeBlockEditMask = prefs.GetProfileEnum(key, "DefaultTimeBlockEditMask", m_DefaultTimeBlockEditMask);

// 				var newTimeBlockAttrib = TimeBlockSeriesAttributes.Decode(prefs.GetProfileString(key, "DefaultNewTimeBlockAttributes", string.Empty));
// 
// 				if (newTimeBlockAttrib != null)
// 				{
// 					m_DefaultNewLogEntryAttributes = newTimeBlockAttrib;
// 				}
// 				else
// 				{
// 					m_DefaultNewLogEntryAttributes.FromTime = m_WorkWeek.WorkDay().StartOfDay();
// 					m_DefaultNewLogEntryAttributes.ToTime = m_WorkWeek.WorkDay().StartOfLunch();
// 				}

				m_TimeLog.LoadPreferences(prefs, key);
				m_PrefsDlg.LoadPreferences(prefs, key);

				UpdateTimeLogPreferences();
				SetDaysShowing(prefs.GetProfileInt(key, "DaysShowing", 7));
            }
 		}

		public bool GetTask(UIExtension.GetTask getTask, ref UInt32 taskID)
		{
            return false; // not supported
		}

        public bool SelectTask(String text, UIExtension.SelectTask selectTask, bool caseSensitive, bool wholeWord, bool findReplace)
		{
            return false;//m_TimeLog.SelectTask(text, selectTask, caseSensitive, wholeWord, findReplace);
		}
		
		public bool ScrollToSelectedTask()
		{
			return false; // not supported
		}

		public bool CanScrollToSelectedTask()
		{
			return false; // not supported
		}

		public new Boolean Focus()
        {
            if (Focused)
                return false;

            return m_TimeLog.Focus();
        }

        public new Boolean Focused
        {
            get
            {
                foreach (Control ctrl in Controls)
                {
                    if (ctrl.CanFocus && ctrl.Focused)
                        return true;
                }

                return false;
            }
        }

		// Internal ------------------------------------------------------------------------------

		protected override void Dispose(bool disposing)
        {
            base.Dispose(disposing);

            m_PrefsDlg.Dispose();
        }

		private void InitializeComponent()
		{
			m_ControlsFont = new Font(FontName, 8.25f);
			m_PrefsDlg = new LoggedTimePreferencesDlg(this, m_Trans, m_ControlsFont);
			m_WorkWeek = new WorkingWeek();

			CreateMonthYearCombos();
			CreateToolbar();
			CreateWeekLabel();
			//CreateSelectedTaskDatesLabel();

			// view always comes last
			CreateTimeLogView();
		}

		private void CreateTimeLogView()
		{
			m_TimeLog = new LoggedTimeView(m_Trans,
											new UIExtension.TaskIcon(m_HwndParent),
											new UIExtension.TaskRecurrences(m_HwndParent),
											DPIScaling.Scale(5));

			m_TimeLog.SelectionChanged += new Calendar.AppointmentEventHandler(OnTimeLogSelectionChanged);
// 			m_TimeLog.AppointmentMove += new TDLAppointmentEventHandler(OnTimeLogAppointmentChanged);
			m_TimeLog.WeekChange += new Calendar.WeekChangeEventHandler(OnTimeLogWeekChanged);
			m_TimeLog.MouseWheel += new MouseEventHandler(OnTimeLogMouseWheel);
			m_TimeLog.MouseDoubleClick += new MouseEventHandler(OnTimeLogMouseDoubleClick);
			m_TimeLog.ContextMenu += new TDLContextMenuEventHandler(OnTimeLogContextMenu);

			// Performing icon editing from a 'MouseUp' or 'MouseClick' event 
			// causes the edit icon dialog to fail to correctly get focus but
			// counter-intuitively it works from 'MouseDown'
			m_TimeLog.MouseDown += new MouseEventHandler(OnTimeLogMouseClick);

			m_TimeLog.StartDate = DateTime.Now;
			m_TimeLog.SetFont(FontName, 8);
			m_WeekLabel.StartDate = m_TimeLog.StartDate;

            if (VisualStyleRenderer.IsSupported)
                m_TimeLog.BorderStyle = BorderStyle.FixedSingle;
            else
                m_TimeLog.BorderStyle = BorderStyle.Fixed3D;

            Controls.Add(m_TimeLog);
		}

		ToolStripMenuItem AddMenuItem(ContextMenuStrip menu, string text, Keys keys, int imageIndex)
		{
			var item = new ToolStripMenuItem(text);
			item.ShortcutKeys = keys;
			item.ShowShortcutKeys = (keys != Keys.None);

			if (imageIndex >= 0)
				item.Image = m_TBImageList.Images[imageIndex];

			menu.Items.Add(item);
			return item;
		}

		bool OnTimeLogContextMenu(object sender, MouseEventArgs e)
		{
			return false;
/*
			var appt = m_TimeLog.GetAppointmentAt(e.X, e.Y);

			if ((appt == null) || m_TimeLog.AppointmentSupportsTaskContextMenu(appt))
				return false;

			var menu = new ContextMenuStrip();

			if (appt is TaskCustomDateAttribute)
			{
				var item = AddMenuItem(menu, "Clear Custom Date", Keys.Delete, -1);
				item.Click += (s, a) => { m_TimeLog.DeleteSelectedCustomDate();	};
			}
			else if (appt is TaskTimeBlock)
			{
				var item = AddMenuItem(menu, "New Time Block", Keys.None, 6);
				item.Click += (s, a) => { CreateTimeBlock(); };

				item = AddMenuItem(menu, "Delete Time Block", Keys.Delete, 7);
				item.Click += (s, a) => { m_TimeLog.DeleteSelectedTimeBlock(); };

				item = AddMenuItem(menu, "Duplicate Time Block", (Keys.Control | Keys.D), 8);
				item.Click += (s, a) =>	{ m_TimeLog.DuplicateSelectedTimeBlock(); };

				menu.Items.Add(new ToolStripSeparator());

				item = AddMenuItem(menu, "Edit Time Block Series", Keys.Control | Keys.F2, 9);
				item.Click += (s, a) => { EditSelectedTimeBlockSeries(); };

				item = AddMenuItem(menu, "Delete Time Block Series", (Keys.Control | Keys.Delete), 10);
				item.Click += (s, a) =>	{ m_TimeLog.DeleteSelectedTimeBlockSeries(); };
			}
			else
			{
				Debug.Assert(false);
			}

			if (menu.Items.Count > 0)
			{
				menu.Items.Add(new ToolStripSeparator());
				menu.Items.Add("Cancel");

				menu.Renderer = m_ToolbarRenderer;
				menu.Show(m_TimeLog, e.Location);
			}

			m_Trans.Translate(menu.Items, true);

			return true; // handled
*/
		}

		private void CreateWeekLabel()
		{
			m_WeekLabel = new WeekLabel(m_Trans);

			m_WeekLabel.Font = new Font(FontName, 14);
            m_WeekLabel.Location = new Point(m_Toolbar.Right, LabelTop);
            m_WeekLabel.Height = m_Toolbar.Height;
			m_WeekLabel.TextAlign = System.Drawing.ContentAlignment.TopLeft;
			m_WeekLabel.AutoSize = true;

			Controls.Add(m_WeekLabel);
		}

/*
		private void CreateSelectedTaskDatesLabel()
		{
			m_SelectedTaskDatesLabel = new LinkLabelEx.LinkLabelEx();

			m_SelectedTaskDatesLabel.Font = m_ControlsFont;
			m_SelectedTaskDatesLabel.Location = new Point(m_Toolbar.Right, m_Toolbar.Top);
			m_SelectedTaskDatesLabel.Height = m_Toolbar.Height;
			m_SelectedTaskDatesLabel.Width = 1024;
			m_SelectedTaskDatesLabel.TextAlign = System.Drawing.ContentAlignment.BottomLeft;
			m_SelectedTaskDatesLabel.AutoSize = false;
			m_SelectedTaskDatesLabel.ActiveLinkColor = m_SelectedTaskDatesLabel.LinkColor;
			m_SelectedTaskDatesLabel.VisitedLinkColor = m_SelectedTaskDatesLabel.LinkColor;

			m_SelectedTaskDatesLabel.LinkClicked += new LinkLabelLinkClickedEventHandler(OnClickSelectedTaskDatesLink);
			
			Controls.Add(m_SelectedTaskDatesLabel);
		}
*/

		protected void OnClickSelectedTaskDatesLink(object sender, LinkLabelLinkClickedEventArgs e)
		{
			//m_TimeLog.EnsureSelectionVisible(false);
			m_TimeLog.Focus();
		}

		private void CreateToolbar()
		{
			var assembly = Assembly.GetExecutingAssembly();
			var images = new Bitmap(assembly.GetManifestResourceStream("LoggedTimeUIExtension.toolbar_std.bmp"));
            
            m_TBImageList = new ImageList();
			m_TBImageList.ColorDepth = ColorDepth.Depth32Bit;
            m_TBImageList.ImageSize = new System.Drawing.Size(16, 16);
            m_TBImageList.TransparentColor = Color.Magenta;
            m_TBImageList.Images.AddStrip(images);
            
			m_Toolbar = new IIControls.ToolStripEx();
			m_Toolbar.Anchor = AnchorStyles.None;
			m_Toolbar.GripStyle = ToolStripGripStyle.Hidden;
            m_Toolbar.ImageList = m_TBImageList;

			int imageSize = DPIScaling.Scale(16);

			m_Toolbar.ImageScalingSize = new Size(imageSize, imageSize);
            m_Toolbar.Height = (imageSize + 7); // MFC

			m_ToolbarRenderer = new UIThemeToolbarRenderer();
			m_Toolbar.Renderer = m_ToolbarRenderer;

			var btn1 = new ToolStripButton();
			btn1.ImageIndex = 0;
			btn1.Click += new EventHandler(OnGoToToday);
			btn1.ToolTipText = "Go To Today";
			m_Toolbar.Items.Add(btn1);

			m_Toolbar.Items.Add(new ToolStripSeparator());
			string format = m_Trans.Translate("{0} Day View", Translator.Type.ToolTip);

			var btn2 = new ToolStripButton();
			btn2.Name = "Show1TimeLog";
			btn2.ImageIndex = 1;
			btn2.Click += new EventHandler(OnShow1Day);
			btn2.ToolTipText = string.Format(format, 1);
			m_Toolbar.Items.Add(btn2);

			var btn3 = new ToolStripButton();
			btn3.Name = "Show3TimeLog";
			btn3.ImageIndex = 2;
			btn3.Click += new EventHandler(OnShow3Day);
			btn3.ToolTipText = string.Format(format, 3);
			m_Toolbar.Items.Add(btn3);

			var btn4 = new ToolStripButton();
			btn4.Name = "Show7TimeLog";
			btn4.ImageIndex = 3;
			btn4.Click += new EventHandler(OnShow7Days);
			btn4.ToolTipText = string.Format(format, 7);
			m_Toolbar.Items.Add(btn4);

			var btn5 = new ToolStripButton();
			btn5.Name = "Show14TimeLog";
			btn5.ImageIndex = 4;
			btn5.Click += new EventHandler(OnShow14Days);
			btn5.ToolTipText = string.Format(format, 14);
			m_Toolbar.Items.Add(btn5);

			var btn6 = new ToolStripButton();
			btn6.Name = "Show28TimeLog";
			btn6.ImageIndex = 5;
			btn6.Click += new EventHandler(OnShow28Days);
			btn6.ToolTipText = string.Format(format, 28);
			m_Toolbar.Items.Add(btn6);

			m_Toolbar.Items.Add(new ToolStripSeparator());

			var btn7 = new ToolStripButton();
			btn7.Name = "NewLogEntry";
			btn7.ImageIndex = 6;
			btn7.Click += new EventHandler(OnCreateLogEntry);
			btn7.ToolTipText = "New Log Entry";
			m_Toolbar.Items.Add(btn7);

			var btn8 = new ToolStripButton();
			btn8.Name = "EditLogEntry";
			btn8.ImageIndex = 7;
			btn8.Click += new EventHandler(OnEditLogEntry);
			btn8.ToolTipText = "Edit Log Entry";
			m_Toolbar.Items.Add(btn8);

			var btn9 = new ToolStripButton();
			btn9.Name = "DeleteLogEntry";
			btn9.ImageIndex = 8;
			btn9.Click += new EventHandler(OnDeleteLogEntry);
			btn9.ToolTipText = "Delete Log Entry";
			m_Toolbar.Items.Add(btn9);

			m_Toolbar.Items.Add(new ToolStripSeparator());

			var btn12 = new ToolStripButton();
			btn12.ImageIndex = 9;
			btn12.Click += new EventHandler(OnPreferences);
			btn12.ToolTipText = "Preferences";
			m_Toolbar.Items.Add(btn12);

			m_Toolbar.Items.Add(new ToolStripSeparator());

			var btn13 = new ToolStripButton();
			btn13.ImageIndex = 10;
			btn13.Click += new EventHandler(OnHelp);
			btn13.ToolTipText = "Online Help";
			m_Toolbar.Items.Add(btn13);

			Toolbars.FixupButtonSizes(m_Toolbar);

			Controls.Add(m_Toolbar);
			m_Trans.Translate(m_Toolbar.Items, false);
		}

		private void OnGoToToday(object sender, EventArgs e)
		{
            m_TimeLog.GoToToday();
		}

		private void OnShow1Day(object sender, EventArgs e)
		{
			if (m_TimeLog.DaysShowing != 1)
				SetDaysShowing(1);
		}

		private void OnShow3Day(object sender, EventArgs e)
		{
			if (m_TimeLog.DaysShowing != 3)
				SetDaysShowing(3);
		}

        private void OnShow7Days(object sender, EventArgs e)
        {
            if (m_TimeLog.DaysShowing != 7)
                SetDaysShowing(7);
        }

		private void OnShow14Days(object sender, EventArgs e)
		{
			if (m_TimeLog.DaysShowing != 14)
				SetDaysShowing(14);
		}

		private void OnShow28Days(object sender, EventArgs e)
		{
			if (m_TimeLog.DaysShowing != 28)
				SetDaysShowing(28);
		}

		private void SetDaysShowing(int numDays)
		{
			m_TimeLog.DaysShowing = numDays;
            m_WeekLabel.NumDays = numDays;

			string format = m_Trans.Translate("Next/Previous {0} days", Translator.Type.ToolTip);
			m_TimeLog.HScrollTooltipText = String.Format(format, m_TimeLog.HScrollStep);

			UpdateToolbarButtonStates();
            //UpdatedSelectedTaskDatesPosition();
		}

		private void OnDeleteLogEntry(object sender, EventArgs e)
		{
			if (m_TimeLog.DeleteSelectedLogEntry())
				UpdateToolbarButtonStates();
		}

		private void OnCreateLogEntry(object sender, EventArgs e)
		{
			if (!m_TimeLog.CanAddNewLogEntry)
				return;

			// Get initial attributes
			var attrib = new LogEntry(0,
									0,
									"",
									m_TimeLog.SelectedDates.Start,
									m_TimeLog.SelectedDates.End,
									m_TimeLog.SelectedDates.Length.TotalHours,
									"",
									"",
									"",
									"",
									Color.Empty);

			var dlg = new CreateLoggedEntryDlg(m_TimeLog.TaskItems, m_TimeLog.TaskIcons, m_WorkWeek, attrib);

			FormsUtil.SetFont(dlg, m_ControlsFont);
			m_Trans.Translate(dlg);

			m_TimeLog.ForceShowSelection = true;

			var res = dlg.ShowDialog();

			m_TimeLog.ForceShowSelection = false;

			if (res == DialogResult.OK)
			{
				var taskItem = m_TimeLog.GetTask(dlg.TaskId);

				if (m_TimeLog.AddNewLogEntry(taskItem, dlg.From, dlg.To, dlg.TimeSpent, dlg.Comment, /*taskItem?.Path*/"", /*TODO*/"", dlg.FillColor))
				{
					m_TimeLog.Invalidate();

					if (dlg.WantAddToTimeSpent)
					{
						// TODO
					}
					UpdateToolbarButtonStates();
				}
			}
		}

		private void OnEditLogEntry(object sender, EventArgs e)
		{
			var entry = m_TimeLog.SelectedLogEntry;
			var taskItem = m_TimeLog.SelectedLogEntryTaskItem;

			var dlg = new EditLoggedEntryDlg(entry, m_WorkWeek, (m_TimeLog.ReadOnly || (taskItem == null) || taskItem.Locked));

			FormsUtil.SetFont(dlg, m_ControlsFont);
			m_Trans.Translate(dlg);

			m_TimeLog.ForceShowSelection = true;

			var res = dlg.ShowDialog();

			m_TimeLog.ForceShowSelection = false;

			if (res == DialogResult.OK)
			{
				if (m_TimeLog.ModifySelectedLogEntry(dlg.From, dlg.To, dlg.TimeSpent, dlg.Comment, dlg.FillColor))
				{
					m_TimeLog.Invalidate();

					if (dlg.WantAddToTimeSpent)
					{
						// TODO
					}
				}
			}
		}

		private void UpdateToolbarButtonStates()
		{
			(m_Toolbar.Items["Show1TimeLog"] as ToolStripButton).Checked = (m_TimeLog.DaysShowing == 1);
			(m_Toolbar.Items["Show3TimeLog"] as ToolStripButton).Checked = (m_TimeLog.DaysShowing == 3);
            (m_Toolbar.Items["Show7TimeLog"] as ToolStripButton).Checked = (m_TimeLog.DaysShowing == 7);
			(m_Toolbar.Items["Show14TimeLog"] as ToolStripButton).Checked = (m_TimeLog.DaysShowing == 14);
            (m_Toolbar.Items["Show28TimeLog"] as ToolStripButton).Checked = (m_TimeLog.DaysShowing == 28);

			m_Toolbar.Items["NewLogEntry"].Enabled = m_TimeLog.CanAddNewLogEntry;
			m_Toolbar.Items["EditLogEntry"].Enabled = m_TimeLog.CanModifySelectedLogEntry;
			m_Toolbar.Items["DeleteLogEntry"].Enabled = m_TimeLog.CanDeleteSelectedLogEntry;
		}

		private void OnPreferences(object sender, EventArgs e)
		{
			m_PrefsDlg.StartPosition = FormStartPosition.CenterParent;

            if (m_PrefsDlg.ShowDialog() == DialogResult.OK)
            {
				UpdateTimeLogPreferences();
            }
		}

		private void UpdateTimeLogPreferences()
		{
			m_TimeLog.ShowWorkingHoursOnly = m_PrefsDlg.ShowWorkingHoursOnly;
			m_TimeLog.SlotsPerHour = (60 / m_PrefsDlg.SlotMinutes);
			m_TimeLog.MinSlotHeight = DPIScaling.Scale(m_PrefsDlg.MinSlotHeight);

			UpdateWorkingHourDisplay();

			m_TimeLog.Invalidate();
		}

		private void UpdateWorkingHourDisplay()
		{
            m_TimeLog.WorkStart.Set(m_WorkWeek.WorkDay().StartOfDayInHours());
            m_TimeLog.WorkEnd.Set(m_WorkWeek.WorkDay().EndOfDayInHours());

            m_TimeLog.LunchStart.Set(m_WorkWeek.WorkDay().StartOfLunchInHours());
            m_TimeLog.LunchEnd.Set(m_WorkWeek.WorkDay().EndOfLunchInHours());
		}

		private void OnHelp(object sender, EventArgs e)
		{
            UIExtension.ParentNotify notify = new UIExtension.ParentNotify(m_HwndParent);

            notify.NotifyDoHelp(m_TypeId);
		}

		private void CreateMonthYearCombos()
		{
			m_MonthCombo = new MonthComboBox();

			m_MonthCombo.Font = m_ControlsFont;
            m_MonthCombo.Location = new Point(DPIScaling.Scale(0), ComboTop);
            m_MonthCombo.Size = DPIScaling.Scale(new Size(100, 16));
			
			m_MonthCombo.SelectedMonth = DateTime.Now.Month;
			m_MonthCombo.SelectedIndexChanged += new EventHandler(OnMonthYearSelChanged);
			
			Controls.Add(m_MonthCombo);

			m_YearCombo = new YearComboBox();

			m_YearCombo.Font = m_ControlsFont;
            m_YearCombo.Location = new Point(DPIScaling.Scale(105), ComboTop);
            m_YearCombo.Size = DPIScaling.Scale(new Size(100, 16));

			m_YearCombo.SelectedYear = DateTime.Now.Year;
			m_YearCombo.SelectedIndexChanged += new EventHandler(OnMonthYearSelChanged);
			
			Controls.Add(m_YearCombo);
		}

		// Message handlers ------------------------------------------------------------------
        protected override void OnGotFocus(EventArgs e)
        {
            base.OnGotFocus(e);

            m_TimeLog.Focus();
        }

        protected override void OnSizeChanged(EventArgs e)
        {
            base.OnSizeChanged(e);

			m_YearCombo.Location = new Point(m_MonthCombo.Right + 10, ComboTop);
			m_Toolbar.Location = new Point(m_YearCombo.Right + 10, LabelTop - 2);
			m_WeekLabel.Location = new Point(m_Toolbar.Right + 10, LabelTop);

			//UpdatedSelectedTaskDatesPosition(); // called elsewhere also

			Rectangle dayViewRect = ClientRectangle;

			dayViewRect.Y = ControlTop;
			dayViewRect.Height -= ControlTop;
			dayViewRect.Inflate(-1, -1);

			m_TimeLog.Bounds = dayViewRect;

            Invalidate(true);
        }

		private void HandleTimeLogLeftMouseClick(MouseEventArgs e, bool doubleClick)
		{
			var appt = m_TimeLog.GetAppointmentAt(e.Location.X, e.Location.Y);

			if (appt == null)
				return;

// 			// Handle icon click on all types
// 			var realAppt = m_TimeLog.GetRealAppointment(appt);
// 
// 			if (!m_TimeLog.ReadOnly && !realAppt.Locked &&
// 				(m_TimeLog.IconHitTest(m_TimeLog.PointToScreen(e.Location)) > 0))
// 			{
// 				var notify = new UIExtension.ParentNotify(m_HwndParent);
// 				notify.NotifyEditIcon();
// 
// 				return;
// 			}

			// Handle double-click differently for each type
			if (doubleClick)
				OnEditLogEntry(this, null);
		}

		private void OnTimeLogMouseClick(object sender, MouseEventArgs e)
        {
			if (e.Button == MouseButtons.Left)
				HandleTimeLogLeftMouseClick(e, false);
		}

		private void OnTimeLogMouseDoubleClick(object sender, MouseEventArgs e)
		{
			if (e.Button == MouseButtons.Left)
				HandleTimeLogLeftMouseClick(e, true);
		}

		private void OnTimeLogMouseWheel(object sender, MouseEventArgs e)
		{
			if (ModifierKeys.Equals(Keys.Control))
			{
				bool change = ((e.Delta < 0) ? m_PrefsDlg.DecrementMinSlotHeight() : m_PrefsDlg.IncrementMinSlotHeight());

				if (change)
					m_TimeLog.MinSlotHeight = m_PrefsDlg.MinSlotHeight;
			}
		}

		private void OnTimeLogSelectionChanged(object sender, Calendar.AppointmentEventArgs args)
		{
			//UpdatedSelectedTaskDatesText();
			UpdateToolbarButtonStates();
		}

/*
		private void UpdatedSelectedTaskDatesText()
		{
			DateTime from, to;

			if (m_TimeLog.GetSelectedTaskDates(out from, out to))
			{
				String label = String.Format("{0}: ", m_Trans.Translate("Selected Task Date Range", Translator.Type.Label));
				String dateRange = DateUtil.FormatRange(from, to, true, m_TimeLog.DisplayDatesInISO);

				m_SelectedTaskDatesLabel.Text = (label + dateRange);
				m_SelectedTaskDatesLabel.LinkArea = new LinkArea(label.Length, dateRange.Length);
			}
			else
			{
				m_SelectedTaskDatesLabel.Text = String.Empty;
			}
		}

		private void UpdatedSelectedTaskDatesPosition()
		{
			m_SelectedTaskDatesLabel.Location = new Point(m_WeekLabel.Right + 10, m_YearCombo.Bottom - m_SelectedTaskDatesLabel.Height);
		}
*/

		private void OnTimeLogWeekChanged(object sender, Calendar.WeekChangeEventArgs args)
		{
			m_WeekLabel.StartDate = args.StartDate;

			if (!m_SettingTimeLogStartDate)
			{
				m_SettingMonthYear = true;

				m_MonthCombo.SelectedMonth = args.StartDate.Month;
				m_YearCombo.SelectedYear = args.StartDate.Year;

				//UpdatedSelectedTaskDatesPosition();

				m_SettingMonthYear = false;
			}
		}

		private void OnMonthYearSelChanged(object sender, EventArgs args)
		{
			if (!m_SettingMonthYear)
			{
				m_SettingTimeLogStartDate = true;

				m_TimeLog.StartDate = new DateTime(m_YearCombo.SelectedYear, m_MonthCombo.SelectedMonth, 1);
				m_WeekLabel.StartDate = m_TimeLog.StartDate;

				//UpdatedSelectedTaskDatesPosition();

				m_SettingTimeLogStartDate = false;
			}
		}

        private int ControlTop
        {
            get
            {
                if (m_MonthCombo != null)
                    return m_MonthCombo.Bounds.Bottom + DPIScaling.Scale(4);

                // else
                return 0;
            }
        }

		public Bitmap SaveToImage()
		{
			return m_TimeLog.SaveToImage();
		}

		public Boolean CanSaveToImage()
		{
			return m_TimeLog.CanSaveToImage();
		}

	}


}
