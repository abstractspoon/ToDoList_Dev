
using System;
using System.Diagnostics;
using System.Reflection;
using System.Drawing;
using System.Windows.Forms;
using System.Windows.Forms.VisualStyles;

using UIComponents;

using Abstractspoon.Tdl.PluginHelpers;
using Abstractspoon.Tdl.PluginHelpers.ColorUtil;

namespace TrackedTimeUIExtension
{

	[System.ComponentModel.DesignerCategory("")]
	public class TrackedTimeUIExtensionCore : Panel, IUIExtension
	{
		private IntPtr m_HwndParent = IntPtr.Zero;
		private TrackedTimeView m_TimeLog = null;
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
        private TrackedTimePreferencesDlg m_PrefsDlg;

        private IIControls.ToolStripEx m_Toolbar;
		private ImageList m_TBImageList;
		private UIThemeToolbarRenderer m_ToolbarRenderer;
		private LinkLabelEx.LinkLabelEx m_SelectedTaskDatesLabel;
		private Font m_ControlsFont;

// 		private TimeBlockSeriesAttributes m_DefaultNewTimeBlockAttributes;
// 		private TimeBlockSeriesAttributes.EditMask m_DefaultTimeBlockEditMask;
		
		// --------------------------------------------------------------------------------------

		public TrackedTimeUIExtensionCore(String typeID, String uiName, IntPtr hwndParent, Translator trans)
		{
			m_HwndParent = hwndParent;
			m_Trans = trans;
			m_TypeId = typeID;
			m_UiName = uiName;

// 			m_DefaultNewTimeBlockAttributes = new TimeBlockSeriesAttributes();
// 			m_DefaultTimeBlockEditMask = (TimeBlockSeriesAttributes.EditMask.Dates | TimeBlockSeriesAttributes.EditMask.Times);

			InitializeComponent();
		}
		
		public bool SelectTask(UInt32 dwTaskID)
		{
            if (m_TimeLog.SelectedTaskId == dwTaskID)
                return true;

			bool selected = m_TimeLog.SelectTask(dwTaskID);

			m_SettingMonthYear = true;

// 			m_WeekLabel.StartDate = m_TimeLog.StartDate;
// 			m_MonthCombo.SelectedMonth = m_TimeLog.StartDate.Month;
// 			m_YearCombo.SelectedYear = m_TimeLog.StartDate.Year;

			m_SettingMonthYear = false;

			UpdatedSelectedTaskDatesText();

			return selected;
		}

		public bool SelectTasks(UInt32[] pdwTaskIDs)
		{
            // not currently supported
			return false;
		}

		public void UpdateTasks(TaskList tasks, UIExtension.UpdateType type)
		{
			m_TimeLog.UpdateTasks(tasks, type, m_TypeId);
		}

		public bool WantTaskUpdate(Task.Attribute attrib)
		{
			switch (attrib)
			{
			case Task.Attribute.Title:
			case Task.Attribute.DoneDate:
			case Task.Attribute.DueDate:
			case Task.Attribute.StartDate:
			case Task.Attribute.AllocatedTo:
			case Task.Attribute.Icon:
			case Task.Attribute.TimeEstimate:
			case Task.Attribute.Dependency:
			case Task.Attribute.Recurrence:
			case Task.Attribute.Tags:
			case Task.Attribute.CustomAttribute:
			case Task.Attribute.Color:
				return true;
			}

			// all else
			return false;
		}

		public bool WantSortUpdate(Task.Attribute attrib)
		{
			return false;
		}
	   
		public bool PrepareNewTask(ref Task task)
		{
			return m_TimeLog.PrepareNewTask(ref task);
		}

		public bool DoIdleProcessing()
		{
			return false;
		}

		public bool ProcessMessage(IntPtr hwnd, UInt32 message, UInt32 wParam, UInt32 lParam, UInt32 time, Int32 xPos, Int32 yPos)
		{
/*
			const int WM_KEYDOWN = 0x0100;

			switch (message)
			{
			case WM_KEYDOWN:
				{
					Keys keys = (Keys)wParam;

					switch (keys)
					{
					case Keys.Escape:
						return m_TimeLog.CancelAppointmentResizing();

					case Keys.Delete:
						{
							if (m_TimeLog.DeleteSelectedCustomDate())
								return true;

							if (m_TimeLog.DeleteSelectedTimeBlock())
							{
								UpdateToolbarButtonStates();
								return true;
							}
						}
						break;

					case (Keys.Control | Keys.F2):
						return EditSelectedTimeBlockSeries();

					case (Keys.Control | Keys.Delete):
						if (m_TimeLog.DeleteSelectedTimeBlockSeries())
						{
							UpdateToolbarButtonStates();
							return true;
						}
						break;
					}
				}
				break;
			}
*/

			return false;
		}

		public bool GetLabelEditRect(ref Int32 left, ref Int32 top, ref Int32 right, ref Int32 bottom)
		{
            Rectangle rect = new Rectangle();

            if (m_TimeLog.GetSelectedItemLabelRect(ref rect))
            {
                rect = m_TimeLog.RectangleToScreen(rect);

                left = rect.Left;
                top = rect.Top;
                right = rect.Right;
				bottom = rect.Bottom;

                return true;
            }

            // else
            return false;
		}

		public UIExtension.HitTestResult HitTest(Int32 xScreen, Int32 yScreen, UIExtension.HitTestReason reason)
		{
			return m_TimeLog.HitTest(xScreen, yScreen, reason);
		}

		public UInt32 HitTestTask(Int32 xScreen, Int32 yScreen, UIExtension.HitTestReason reason)
		{
			return m_TimeLog.HitTestTask(xScreen, yScreen, reason);
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
			m_SelectedTaskDatesLabel.ForeColor = m_WeekLabel.ForeColor;
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
// 			prefs.WriteProfileString(key, "DefaultNewTimeBlockAttributes", m_DefaultNewTimeBlockAttributes.Encode());

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
				UpdatedSelectedTaskDatesText();
			}
			else
            {
				// private settings
// 				m_DefaultTimeBlockEditMask = prefs.GetProfileEnum(key, "DefaultTimeBlockEditMask", m_DefaultTimeBlockEditMask);

// 				var newTimeBlockAttrib = TimeBlockSeriesAttributes.Decode(prefs.GetProfileString(key, "DefaultNewTimeBlockAttributes", string.Empty));
// 
// 				if (newTimeBlockAttrib != null)
// 				{
// 					m_DefaultNewTimeBlockAttributes = newTimeBlockAttrib;
// 				}
// 				else
// 				{
// 					m_DefaultNewTimeBlockAttributes.FromTime = m_WorkWeek.WorkDay().StartOfDay();
// 					m_DefaultNewTimeBlockAttributes.ToTime = m_WorkWeek.WorkDay().StartOfLunch();
// 				}

				m_TimeLog.LoadPreferences(prefs, key);
				m_PrefsDlg.LoadPreferences(prefs, key);

				UpdateTimeLogPreferences();
				SetDaysShowing(prefs.GetProfileInt(key, "DaysShowing", 7));
            }
 		}

		public bool GetTask(UIExtension.GetTask getTask, ref UInt32 taskID)
		{
            return m_TimeLog.GetTask(getTask, ref taskID);
		}

        public bool SelectTask(String text, UIExtension.SelectTask selectTask, bool caseSensitive, bool wholeWord, bool findReplace)
		{
            return m_TimeLog.SelectTask(text, selectTask, caseSensitive, wholeWord, findReplace);
		}
		
		public bool ScrollToSelectedTask()
		{
			return m_TimeLog.EnsureSelectionVisible(false);
		}

		public bool CanScrollToSelectedTask()
		{
			return (m_TimeLog.SelectedTaskId != 0);
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
			m_PrefsDlg = new TrackedTimePreferencesDlg(this, m_Trans, m_ControlsFont);
			m_WorkWeek = new WorkingWeek();

			CreateMonthYearCombos();
			CreateToolbar();
			CreateWeekLabel();
			CreateSelectedTaskDatesLabel();

			// view always comes last
			CreateTimeLogView();
		}

		private void CreateTimeLogView()
		{
			m_TimeLog = new TrackedTimeView(m_Trans,
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

		protected void OnClickSelectedTaskDatesLink(object sender, LinkLabelLinkClickedEventArgs e)
		{
			m_TimeLog.EnsureSelectionVisible(false);
			m_TimeLog.Focus();
		}

		private void CreateToolbar()
		{
			var assembly = Assembly.GetExecutingAssembly();
			var images = new Bitmap(assembly.GetManifestResourceStream("TimeLogUIExtension.toolbar_std.bmp"));
            
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
			btn2.Click += new EventHandler(OnShow1TimeLog);
			btn2.ToolTipText = string.Format(format, 1);
			m_Toolbar.Items.Add(btn2);

			var btn3 = new ToolStripButton();
			btn3.Name = "Show3TimeLog";
			btn3.ImageIndex = 2;
			btn3.Click += new EventHandler(OnShow3TimeLog);
			btn3.ToolTipText = string.Format(format, 3);
			m_Toolbar.Items.Add(btn3);

			var btn4 = new ToolStripButton();
			btn4.Name = "Show7TimeLog";
			btn4.ImageIndex = 3;
			btn4.Click += new EventHandler(OnShow7TimeLog);
			btn4.ToolTipText = string.Format(format, 7);
			m_Toolbar.Items.Add(btn4);

			var btn5 = new ToolStripButton();
			btn5.Name = "Show14TimeLog";
			btn5.ImageIndex = 4;
			btn5.Click += new EventHandler(OnShow14TimeLog);
			btn5.ToolTipText = string.Format(format, 14);
			m_Toolbar.Items.Add(btn5);

			var btn6 = new ToolStripButton();
			btn6.Name = "Show28TimeLog";
			btn6.ImageIndex = 5;
			btn6.Click += new EventHandler(OnShow28TimeLog);
			btn6.ToolTipText = string.Format(format, 28);
			m_Toolbar.Items.Add(btn6);

			m_Toolbar.Items.Add(new ToolStripSeparator());

			var btn7 = new ToolStripButton();
			btn7.Name = "NewLogEntry";
			btn7.ImageIndex = 6;
			btn7.Click += new EventHandler(OnNewLogEntry);
			btn7.ToolTipText = "New Log Entry";
			m_Toolbar.Items.Add(btn7);

			var btn8 = new ToolStripButton();
			btn8.Name = "DeleteLogEntry";
			btn8.ImageIndex = 7;
			btn8.Click += new EventHandler(OnDeleteLogEntry);
			btn8.ToolTipText = "Delete Log Entry";
			m_Toolbar.Items.Add(btn8);

			m_Toolbar.Items.Add(new ToolStripSeparator());

			var btn12 = new ToolStripButton();
			btn12.ImageIndex = 8;
			btn12.Click += new EventHandler(OnPreferences);
			btn12.ToolTipText = "Preferences";
			m_Toolbar.Items.Add(btn12);

			m_Toolbar.Items.Add(new ToolStripSeparator());

			var btn13 = new ToolStripButton();
			btn13.ImageIndex = 9;
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

		private void OnShow1TimeLog(object sender, EventArgs e)
		{
			if (m_TimeLog.DaysShowing != 1)
				SetDaysShowing(1);
		}

		private void OnShow3TimeLog(object sender, EventArgs e)
		{
			if (m_TimeLog.DaysShowing != 3)
				SetDaysShowing(3);
		}

        private void OnShow7TimeLog(object sender, EventArgs e)
        {
            if (m_TimeLog.DaysShowing != 7)
                SetDaysShowing(7);
        }

		private void OnShow14TimeLog(object sender, EventArgs e)
		{
			if (m_TimeLog.DaysShowing != 14)
				SetDaysShowing(14);
		}

		private void OnShow28TimeLog(object sender, EventArgs e)
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
            UpdatedSelectedTaskDatesPosition();
		}

		private void OnDeleteLogEntry(object sender, EventArgs e)
		{
			if (m_TimeLog.DeleteSelectedLogEntry())
				UpdateToolbarButtonStates();
		}

		private void OnNewLogEntry(object sender, EventArgs e)
		{
			if (CreateLogEntry())
				UpdateToolbarButtonStates();
		}

		private bool CreateLogEntry()
		{
			return false;
/*
			// Display a dialog to retrieve the task ID from a list
			// to support tasks without dates
			var attribs = new TimeBlockSeriesAttributes(m_DefaultNewTimeBlockAttributes);

			if (m_TimeLog.SelectionType == Calendar.SelectionType.DateRange)
			{
				var dates = m_TimeLog.SelectedDates;

				attribs.FromDate = attribs.ToDate = dates.Start.Date;
				attribs.FromTime = dates.Start.TimeOfDay;
				attribs.ToTime = dates.End.TimeOfDay;
				attribs.SyncToTaskDates = false;
			}
			else if (m_TimeLog.SelectedAppointment is TimeLogEntry)
			{
				var dates = m_TimeLog.SelectedAppointment.Dates;

				attribs.FromDate = dates.Start.Date;
				attribs.ToDate = dates.End.Date;

				// Use default times
			}
			else if (m_TimeLog.SelectedAppointment is TaskTimeBlock)
			{
				// Use time from time block
				var dates = m_TimeLog.SelectedAppointment.Dates;

				attribs.FromTime = dates.Start.TimeOfDay;
				attribs.ToTime = dates.End.TimeOfDay;

				// Use dates from real task
				dates = m_TimeLog.GetRealAppointment(m_TimeLog.SelectedAppointment).Dates;

				attribs.FromDate = dates.Start.Date;
				attribs.ToDate = dates.End.Date;
			}

			var dlg = new TimeLogCreateTimeBlockDlg(m_TimeLog.TaskItems, 
													new UIExtension.TaskIcon(m_HwndParent),
													m_WorkWeek,
													m_TimeLog.SelectedTaskId,
													attribs);

			FormsUtil.SetFont(dlg, m_ControlsFont);
			m_Trans.Translate(dlg);

			m_TimeLog.ForceShowSelection = true;

			var res = dlg.ShowDialog();

			m_TimeLog.ForceShowSelection = false;
			
			if (res != DialogResult.OK)
				return false;

			m_DefaultNewTimeBlockAttributes = dlg.Attributes;

			return m_TimeLog.CreateNewTaskBlockSeries(dlg.SelectedTaskId, dlg.Attributes);
*/
		}

		private void UpdateToolbarButtonStates()
		{
			(m_Toolbar.Items["Show1TimeLog"] as ToolStripButton).Checked = (m_TimeLog.DaysShowing == 1);
			(m_Toolbar.Items["Show3TimeLog"] as ToolStripButton).Checked = (m_TimeLog.DaysShowing == 3);
            (m_Toolbar.Items["Show7TimeLog"] as ToolStripButton).Checked = (m_TimeLog.DaysShowing == 7);
			(m_Toolbar.Items["Show14TimeLog"] as ToolStripButton).Checked = (m_TimeLog.DaysShowing == 14);
            (m_Toolbar.Items["Show28TimeLog"] as ToolStripButton).Checked = (m_TimeLog.DaysShowing == 28);

// 			m_Toolbar.Items["NewLogEntry"].Enabled = m_TimeLog.CanCreateNewLogEntry;
// 			m_Toolbar.Items["DeleteLogEntry"].Enabled = m_TimeLog.CanDeleteSelectedLogEntry;
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

			UpdatedSelectedTaskDatesPosition(); // called elsewhere also

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

			// Handle icon click on all types
			var realAppt = m_TimeLog.GetRealAppointment(appt);

			if (!m_TimeLog.ReadOnly && !realAppt.Locked &&
				(m_TimeLog.IconHitTest(m_TimeLog.PointToScreen(e.Location)) > 0))
			{
				var notify = new UIExtension.ParentNotify(m_HwndParent);
				notify.NotifyEditIcon();

				return;
			}

			// Handle double-click differently for each type
			if (doubleClick)
			{
				if (appt is LogEntry)
				{
					if (!m_TimeLog.ReadOnly && !appt.Locked)
					{
						var notify = new UIExtension.ParentNotify(m_HwndParent);
						notify.NotifyEditLabel();
					}
				}
			}
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
            UIExtension.ParentNotify notify = new UIExtension.ParentNotify(m_HwndParent);

			switch (m_TimeLog.SelectionType)
			{
			case Calendar.SelectionType.Appointment:
				UpdatedSelectedTaskDatesText();
				UpdateToolbarButtonStates();

				notify.NotifySelChange(m_TimeLog.SelectedTaskId);
				break;

			case Calendar.SelectionType.DateRange:
				UpdateToolbarButtonStates();
				break;
			}
		}

		private void UpdatedSelectedTaskDatesText()
		{
			DateTime from, to;

// 			if (m_TimeLog.GetSelectedTaskDates(out from, out to))
// 			{
// 				String label = String.Format("{0}: ", m_Trans.Translate("Selected Task Date Range", Translator.Type.Label));
// 				String dateRange = DateUtil.FormatRange(from, to, true, m_TimeLog.DisplayDatesInISO);
// 
// 				m_SelectedTaskDatesLabel.Text = (label + dateRange);
// 				m_SelectedTaskDatesLabel.LinkArea = new LinkArea(label.Length, dateRange.Length);
// 			}
// 			else
			{
				m_SelectedTaskDatesLabel.Text = String.Empty;
			}
		}

		private void UpdatedSelectedTaskDatesPosition()
		{
			m_SelectedTaskDatesLabel.Location = new Point(m_WeekLabel.Right + 10, m_YearCombo.Bottom - m_SelectedTaskDatesLabel.Height);
		}

		private void OnTimeLogWeekChanged(object sender, Calendar.WeekChangeEventArgs args)
		{
			m_WeekLabel.StartDate = args.StartDate;

			if (!m_SettingTimeLogStartDate)
			{
				m_SettingMonthYear = true;

				m_MonthCombo.SelectedMonth = args.StartDate.Month;
				m_YearCombo.SelectedYear = args.StartDate.Year;

				UpdatedSelectedTaskDatesPosition();

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

				UpdatedSelectedTaskDatesPosition();

				m_SettingTimeLogStartDate = false;
			}
		}

/*
        private void OnTimeLogAppointmentChanged(object sender, TDLMoveAppointmentEventArgs args)
		{
			ProcessTaskAppointmentChange(args);
		}

		private void ProcessTaskAppointmentChange(TDLMoveAppointmentEventArgs args)
		{
            if (!args.Finished)
            {
                UpdatedSelectedTaskDatesText();
                return;
            }

			var item = args.Appointment as TimeLogEntry;

			if (item == null)
				return;

			var notify = new UIExtension.ParentNotify(m_HwndParent);

			if (!string.IsNullOrEmpty(args.CustomAttributeId))
			{
				var date = item.CustomDates[args.CustomAttributeId];

				if (date == DateTime.MinValue)
					notify.NotifyMod(args.CustomAttributeId, string.Empty);
				else
					notify.NotifyMod(args.CustomAttributeId, date.ToString());

				return;
			}

			// Start/Due change
			if (PrepareTaskNotify(item, args.Mode, notify))
			{
				bool modifyTimeEst = WantModifyTimeEstimate(item);

				if (notify.NotifyMod())
				{
					item.UpdateOriginalDates();

					if (modifyTimeEst)
						item.RecalcTimeEstimate(m_WorkWeek);

					return;
				}
			}

			item.RestoreOriginalDates();
			m_TimeLog.Invalidate();
		}
*/

		private bool PrepareTaskNotify(LogEntry item, Calendar.SelectionTool.Mode mode, UIExtension.ParentNotify notify, bool includeTimeEstimate = true)
		{
/*
			switch (mode)
			{
			case Calendar.SelectionTool.Mode.Move:
				if (item.LengthDiffersFromOriginal())
				{
					// Start date WITHOUT TIME ESTIMATE
					PrepareTaskNotify(item, Calendar.SelectionTool.Mode.ResizeLeft, notify, false);

					// End date WITHOUT TIME ESTIMATE
					PrepareTaskNotify(item, Calendar.SelectionTool.Mode.ResizeRight, notify, false);

					if (includeTimeEstimate && WantModifyTimeEstimate(item))
					{
						notify.AddMod(Task.Attribute.TimeEstimate, item.LengthAsTimeEstimate(m_WorkWeek, false), item.TimeEstUnits);
					}

					return true;
				}
				else if (item.StartDateDiffersFromOriginal())
				{
					notify.AddMod(Task.Attribute.OffsetTask, item.StartDate);

					return true;
				}
				break;

			case Calendar.SelectionTool.Mode.ResizeLeft:
			case Calendar.SelectionTool.Mode.ResizeTop:
				if (item.StartDateDiffersFromOriginal())
				{
					notify.AddMod(Task.Attribute.StartDate, item.StartDate);

					if (includeTimeEstimate && WantModifyTimeEstimate(item))
					{
						notify.AddMod(Task.Attribute.TimeEstimate, item.LengthAsTimeEstimate(m_WorkWeek, false), item.TimeEstUnits);
					}

					return true;
				}
				break;

			case Calendar.SelectionTool.Mode.ResizeRight:
			case Calendar.SelectionTool.Mode.ResizeBottom:
				if (item.EndDateDiffersFromOriginal())
				{
					// Allow for end of day
					var endDate = item.EndDate;

					if (endDate == endDate.Date)
						endDate = endDate.AddDays(-1);

					if (item.IsDone)
						notify.AddMod(Task.Attribute.DoneDate, endDate);
					else
						notify.AddMod(Task.Attribute.DueDate, endDate);

					if (includeTimeEstimate && WantModifyTimeEstimate(item))
					{
						notify.AddMod(Task.Attribute.TimeEstimate, item.LengthAsTimeEstimate(m_WorkWeek, false), item.TimeEstUnits);
					}

					return true;
				}
				break;
			}
*/

			return false;
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
