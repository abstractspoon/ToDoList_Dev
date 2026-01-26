
using System;
using System.Diagnostics;
using System.Reflection;
using System.Drawing;
using System.Windows.Forms;
using System.Windows.Forms.VisualStyles;

using UIComponents;

using Abstractspoon.Tdl.PluginHelpers;
using Abstractspoon.Tdl.PluginHelpers.ColorUtil;

namespace DayViewUIExtension
{

	[System.ComponentModel.DesignerCategory("")]
	public class DayViewUIExtensionCore : Panel, IUIExtension
	{
		private IntPtr m_HwndParent = IntPtr.Zero;
		private TDLDayView m_DayView = null;
		private Translator m_Trans = null;
		private String m_TypeId, m_UiName;
		private WorkingWeek m_WorkWeek = null;

		private const string FontName = "Tahoma";

		private static Color DefGridColor = Color.FromArgb(192, 192, 192);
        private static int LabelTop = DPIScaling.Scale(2);
        private static int ComboTop = (LabelTop + DPIScaling.Scale(2));

		private bool m_SettingMonthYear = false;
		private bool m_SettingDayViewStartDate = false;
		private bool m_AllowModifyTimeEstimate = false;
		private uint m_SelectedTaskId = 0;

		private WeekLabel m_WeekLabel;
		private MonthComboBox m_MonthCombo;
		private YearComboBox m_YearCombo;
        private DayViewPreferencesDlg m_PrefsDlg;
		private DateRangeLink m_SelectedTaskDates;
		private Label m_SelectedTaskDatesLabel;

        private IIControls.ToolStripEx m_Toolbar;
		private ImageList m_TBImageList;
		private UIThemeToolbarRenderer m_ToolbarRenderer;
		private Font m_ControlsFont;

		private TimeBlockSeriesAttributes m_DefaultNewTimeBlockAttributes;
		private TimeBlockSeriesAttributes.EditMask m_DefaultTimeBlockEditMask;
		
		// --------------------------------------------------------------------------------------

		public DayViewUIExtensionCore(String typeID, String uiName, IntPtr hwndParent, Translator trans)
		{
			m_HwndParent = hwndParent;
			m_Trans = trans;
			m_TypeId = typeID;
			m_UiName = uiName;

			m_DefaultNewTimeBlockAttributes = new TimeBlockSeriesAttributes();
			m_DefaultTimeBlockEditMask = (TimeBlockSeriesAttributes.EditMask.Dates | TimeBlockSeriesAttributes.EditMask.Times);

			InitializeComponent();
			UpdateMonthYearCombos(DateTime.Now);
		}

		public bool SelectTask(UInt32 taskId)
		{
            if (taskId == m_SelectedTaskId)
                return true;

			bool selected = m_DayView.SelectTask(taskId);

			m_SelectedTaskId = (selected ? taskId : 0);
			m_WeekLabel.StartDate = m_DayView.StartDate;

			UpdateMonthYearCombos(m_DayView.StartDate);
			UpdatedSelectedTaskDatesText();

			return selected;
		}

		public bool SelectTasks(UInt32[] taskIds)
		{
            // not currently supported
			return false;
		}

		public void UpdateTasks(TaskList tasks, UIExtension.UpdateType type)
		{
			m_DayView.UpdateTasks(tasks, type, m_TypeId);
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
			return m_DayView.PrepareNewTask(ref task);
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
						return m_DayView.CancelAppointmentResizing();

					case Keys.Delete:
						if (ModifierKeys.HasFlag(Keys.Control))
						{
							if (m_DayView.DeleteSelectedTimeBlockSeries())
							{
								UpdateToolbarButtonStates();
								return true;
							}
						}
						else if (m_DayView.DeleteSelectedTimeBlock())
						{
							UpdateToolbarButtonStates();
							return true;
						}
						else if (m_DayView.DeleteSelectedCustomDate())
						{
							return true;
						}
						break;

					case Keys.F2:
						if (ModifierKeys.HasFlag(Keys.Control))
						{
							return EditSelectedTimeBlockSeries();
						}
						break;
					}
				}
				break;
			}

			return false;
		}

		public bool DoIdleProcessing()
		{
			return false;
		}

		public bool GetLabelEditRect(ref Int32 left, ref Int32 top, ref Int32 right, ref Int32 bottom)
		{
            Rectangle rect = new Rectangle();

            if (m_DayView.GetSelectedItemLabelRect(ref rect))
            {
                rect = m_DayView.RectangleToScreen(rect);

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
			return m_DayView.HitTest(xScreen, yScreen, reason);
		}

		public UInt32 HitTestTask(Int32 xScreen, Int32 yScreen, UIExtension.HitTestReason reason)
		{
			return m_DayView.HitTestTask(xScreen, yScreen, reason);
		}

		public void SetUITheme(UITheme theme)
		{
			BackColor = m_Toolbar.BackColor = theme.GetAppDrawingColor(UITheme.AppColor.AppBackLight);

			// Set the toolbar colors to be the same as the back color
			theme.SetAppDrawingColor(UITheme.AppColor.ToolbarDark, BackColor);
			theme.SetAppDrawingColor(UITheme.AppColor.ToolbarLight, BackColor);

			m_DayView.SetUITheme(theme);
			m_ToolbarRenderer.SetUITheme(theme);

			m_WeekLabel.ForeColor = theme.GetAppDrawingColor(UITheme.AppColor.AppText);
			m_WeekLabel.BackColor = BackColor;

			m_SelectedTaskDatesLabel.ForeColor = m_WeekLabel.ForeColor;
			m_SelectedTaskDatesLabel.BackColor = BackColor;
			m_SelectedTaskDates.BackColor = BackColor;
		}

		public void SetTaskFont(String faceName, int pointSize)
		{
			m_DayView.SetFont(faceName, pointSize);
		}

		public void SetReadOnly(bool bReadOnly)
		{
            m_DayView.ReadOnly = bReadOnly;
		}

		public void SavePreferences(Preferences prefs, String key)
		{
			prefs.WriteProfileEnum(key, "DefaultTimeBlockEditMask", m_DefaultTimeBlockEditMask);
			prefs.WriteProfileString(key, "DefaultNewTimeBlockAttributes", m_DefaultNewTimeBlockAttributes.Encode());

			m_DayView.SavePreferences(prefs, key);
			m_PrefsDlg.SavePreferences(prefs, key);

			prefs.WriteProfileInt(key, "DaysShowing", m_DayView.DaysShowing);
		}

		public void LoadPreferences(Preferences prefs, String key, bool appOnly)
		{
			m_DayView.TaskColorIsBackground = prefs.GetProfileBool("Preferences", "ColorTaskBackground", false);
			m_DayView.ShowParentsAsFolder = prefs.GetProfileBool("Preferences", "ShowParentsAsFolders", false);
			m_DayView.DependencyDatesAreCalculated = prefs.GetProfileBool("Preferences", "AutoAdjustDependents", false);
            m_DayView.StrikeThruDoneTasks = prefs.GetProfileBool("Preferences", "StrikethroughDone", true);
			m_DayView.ShowLabelTips = !prefs.GetProfileBool("Preferences", "ShowInfoTips", false);
			m_DayView.DisplayDatesInISO = prefs.GetProfileBool("Preferences", "DisplayDatesInISO", false);

			m_SelectedTaskDates.SetISOFormat(m_DayView.DisplayDatesInISO);

			m_AllowModifyTimeEstimate = !prefs.GetProfileBool("Preferences", "SyncTimeEstAndDates", false);

			m_WorkWeek.Load(prefs);
            m_DayView.WeekendDays = m_WorkWeek.WeekendDays();

            int gridColor = -1;

            if (prefs.GetProfileBool("Preferences", "SpecifyGridColor", true))
                gridColor = prefs.GetProfileInt("Preferences\\Colors", "GridLines", -1);

            m_DayView.GridlineColor = ((gridColor == -1) ? DefGridColor : DrawingColor.ToColor((UInt32)gridColor));
            
            if (appOnly)
			{
				UpdateWorkingHourDisplay(); // else called in UpdateDayViewPreferences
			}
			else
            {
				// private settings
				m_DefaultTimeBlockEditMask = prefs.GetProfileEnum(key, "DefaultTimeBlockEditMask", m_DefaultTimeBlockEditMask);

				var newTimeBlockAttrib = TimeBlockSeriesAttributes.Decode(prefs.GetProfileString(key, "DefaultNewTimeBlockAttributes", string.Empty));

				if (newTimeBlockAttrib != null)
				{
					m_DefaultNewTimeBlockAttributes = newTimeBlockAttrib;
				}
				else
				{
					m_DefaultNewTimeBlockAttributes.FromTime = m_WorkWeek.WorkDay().StartOfDay();
					m_DefaultNewTimeBlockAttributes.ToTime = m_WorkWeek.WorkDay().StartOfLunch();
				}

				m_DayView.LoadPreferences(prefs, key);
				m_PrefsDlg.LoadPreferences(prefs, key);

				UpdateDayViewPreferences();
				SetDaysShowing(prefs.GetProfileInt(key, "DaysShowing", 7));
            }
 		}

		public bool GetTask(UIExtension.GetTask getTask, ref UInt32 taskID)
		{
            return m_DayView.GetTask(getTask, ref taskID);
		}

        public bool SelectTask(String text, UIExtension.SelectTask selectTask, bool caseSensitive, bool wholeWord, bool findReplace)
		{
            return m_DayView.SelectTask(text, selectTask, caseSensitive, wholeWord, findReplace);
		}
		
		public bool ScrollToSelectedTask()
		{
			return m_DayView.EnsureSelectionVisible(false);
		}

		public bool CanScrollToSelectedTask()
		{
			return (m_SelectedTaskId != 0);
		}

		public new Boolean Focus()
        {
            if (Focused)
                return false;

            return m_DayView.Focus();
        }

        public new Boolean Focused
        {
            get { return m_DayView.Focused; }
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
			m_PrefsDlg = new DayViewPreferencesDlg(this, m_Trans, m_ControlsFont);
			m_WorkWeek = new WorkingWeek();

			CreateMonthYearCombos();
			CreateToolbar();
			CreateWeekLabel();
			CreateSelectedTaskDates();

			// Day view always comes last
			CreateDayView();
		}

		private void CreateDayView()
		{
			m_DayView = new TDLDayView(m_Trans,
										new UIExtension.TaskIcon(m_HwndParent),
										new UIExtension.TaskRecurrences(m_HwndParent),
										DPIScaling.Scale(5));

			m_DayView.SelectionChanged += new Calendar.AppointmentEventHandler(OnDayViewSelectionChanged);
			m_DayView.AppointmentMove += new TDLAppointmentEventHandler(OnDayViewAppointmentChanged);
			m_DayView.WeekChange += new Calendar.WeekChangeEventHandler(OnDayViewWeekChanged);
			m_DayView.MouseWheel += new MouseEventHandler(OnDayViewMouseWheel);
			m_DayView.MouseDoubleClick += new MouseEventHandler(OnDayViewMouseDoubleClick);
			m_DayView.ContextMenu += new TDLContextMenuEventHandler(OnDayViewContextMenu);

			// Performing icon editing from a 'MouseUp' or 'MouseClick' event 
			// causes the edit icon dialog to fail to correctly get focus but
			// counter-intuitively it works from 'MouseDown'
			m_DayView.MouseDown += new MouseEventHandler(OnDayViewMouseClick);

			m_DayView.StartDate = DateTime.Now;
			m_DayView.SetFont(FontName, 8);
			m_WeekLabel.StartDate = m_DayView.StartDate;

            if (VisualStyleRenderer.IsSupported)
                m_DayView.BorderStyle = BorderStyle.FixedSingle;
            else
                m_DayView.BorderStyle = BorderStyle.Fixed3D;

            Controls.Add(m_DayView);
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

		bool OnDayViewContextMenu(object sender, MouseEventArgs e)
		{
			var appt = m_DayView.GetAppointmentAt(e.X, e.Y);

			if ((appt == null) || m_DayView.AppointmentSupportsTaskContextMenu(appt))
				return false;

			var menu = new ContextMenuStrip();

			if (appt is TaskCustomDateAttribute)
			{
				var item = AddMenuItem(menu, "Clear Custom Date", Keys.Delete, -1);
				item.ShortcutKeyDisplayString = "Delete";
				item.Click += (s, a) => { m_DayView.DeleteSelectedCustomDate();	};
			}
			else if (appt is TaskTimeBlock)
			{
				var item = AddMenuItem(menu, "New Time Block", Keys.None, 6);
				item.Click += (s, a) => { CreateTimeBlock(); };

				item = AddMenuItem(menu, "Delete Time Block", Keys.Delete, 7);
				item.ShortcutKeyDisplayString = "Delete";
				item.Click += (s, a) => { m_DayView.DeleteSelectedTimeBlock(); };

				item = AddMenuItem(menu, "Duplicate Time Block", (Keys.Control | Keys.D), 8);
				item.Click += (s, a) =>	{ m_DayView.DuplicateSelectedTimeBlock(); };

				menu.Items.Add(new ToolStripSeparator());

				item = AddMenuItem(menu, "Edit Time Block Series", Keys.Control | Keys.F2, 9);
				item.Click += (s, a) => { EditSelectedTimeBlockSeries(); };

				item = AddMenuItem(menu, "Delete Time Block Series", (Keys.Control | Keys.Delete), 10);
				item.Click += (s, a) =>	{ m_DayView.DeleteSelectedTimeBlockSeries(); };
			}
			else
			{
				Debug.Assert(false);
			}

			if (menu.Items.Count > 0)
			{
				menu.Items.Add(new ToolStripSeparator());
				menu.Items.Add("Cancel");

				m_Trans.Translate(menu.Items, true);

				menu.Renderer = m_ToolbarRenderer;
				menu.Show(m_DayView, e.Location);
			}

			return true; // handled
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

		private void CreateSelectedTaskDates()
		{
			// label
			m_SelectedTaskDatesLabel = new Label()
			{
				Text = m_Trans.Translate("Selected Task Date Range", Translator.Type.Label),
				Font = m_ControlsFont,
				BackColor = BackColor,
				AutoSize = true
			};

			// Date range
			m_SelectedTaskDates = new DateRangeLink()
			{
				Font = m_ControlsFont,
				Width = 300,
				BackColor = BackColor,
				AutoSize = false
			};

			m_SelectedTaskDates.ClickEvent += new EventHandler(OnClickSelectedTaskDatesLink);

			Controls.Add(m_SelectedTaskDatesLabel);
			Controls.Add(m_SelectedTaskDates);
		}

		protected void OnClickSelectedTaskDatesLink(object sender, EventArgs e)
		{
			m_DayView.EnsureSelectionVisible(false);
			m_DayView.Focus();
		}

		private void CreateToolbar()
		{
			var assembly = Assembly.GetExecutingAssembly();
			var images = new Bitmap(assembly.GetManifestResourceStream("DayViewUIExtension.toolbar_std.bmp"));
            
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
			btn2.Name = "Show1DayView";
			btn2.ImageIndex = 1;
			btn2.Click += new EventHandler(OnShow1DayView);
			btn2.ToolTipText = string.Format(format, 1);
			m_Toolbar.Items.Add(btn2);

			var btn3 = new ToolStripButton();
			btn3.Name = "Show3DayView";
			btn3.ImageIndex = 2;
			btn3.Click += new EventHandler(OnShow3DayView);
			btn3.ToolTipText = string.Format(format, 3);
			m_Toolbar.Items.Add(btn3);

			var btn4 = new ToolStripButton();
			btn4.Name = "Show7DayView";
			btn4.ImageIndex = 3;
			btn4.Click += new EventHandler(OnShow7DayView);
			btn4.ToolTipText = string.Format(format, 7);
			m_Toolbar.Items.Add(btn4);

			var btn5 = new ToolStripButton();
			btn5.Name = "Show14DayView";
			btn5.ImageIndex = 4;
			btn5.Click += new EventHandler(OnShow14DayView);
			btn5.ToolTipText = string.Format(format, 14);
			m_Toolbar.Items.Add(btn5);

			var btn6 = new ToolStripButton();
			btn6.Name = "Show28DayView";
			btn6.ImageIndex = 5;
			btn6.Click += new EventHandler(OnShow28DayView);
			btn6.ToolTipText = string.Format(format, 28);
			m_Toolbar.Items.Add(btn6);

			m_Toolbar.Items.Add(new ToolStripSeparator());

			var btn7 = new ToolStripButton();
			btn7.Name = "NewTimeBlock";
			btn7.ImageIndex = 6;
			btn7.Click += new EventHandler(OnNewTimeBlock);
			btn7.ToolTipText = "New Time Block";
			m_Toolbar.Items.Add(btn7);

			var btn8 = new ToolStripButton();
			btn8.Name = "DeleteTimeBlock";
			btn8.ImageIndex = 7;
			btn8.Click += new EventHandler(OnDeleteTimeBlock);
			btn8.ToolTipText = "Delete Time Block";
			m_Toolbar.Items.Add(btn8);

			var btn9 = new ToolStripButton();
			btn9.Name = "DuplicateTimeBlock";
			btn9.ImageIndex = 8;
			btn9.Click += new EventHandler(OnDuplicateTimeBlock);
			btn9.ToolTipText = "Duplicate Time Block";
			m_Toolbar.Items.Add(btn9);

			m_Toolbar.Items.Add(new ToolStripSeparator());

			var btn10 = new ToolStripButton();
			btn10.Name = "EditTimeBlockSeries";
			btn10.ImageIndex = 9;
			btn10.Click += new EventHandler(OnEditTimeBlockSeries);
			btn10.ToolTipText = "Edit Time Block Series";
			m_Toolbar.Items.Add(btn10);

			var btn11 = new ToolStripButton();
			btn11.Name = "DeleteTimeBlockSeries";
			btn11.ImageIndex = 10;
			btn11.Click += new EventHandler(OnDeleteTimeBlockSeries);
			btn11.ToolTipText = "Delete Time Block Series";
			m_Toolbar.Items.Add(btn11);

			m_Toolbar.Items.Add(new ToolStripSeparator());

			var btn12 = new ToolStripButton();
			btn12.ImageIndex = 11;
			btn12.Click += new EventHandler(OnPreferences);
			btn12.ToolTipText = "Preferences";
			m_Toolbar.Items.Add(btn12);

			m_Toolbar.Items.Add(new ToolStripSeparator());

			var btn13 = new ToolStripButton();
			btn13.ImageIndex = 12;
			btn13.Click += new EventHandler(OnHelp);
			btn13.ToolTipText = "Online Help";
			m_Toolbar.Items.Add(btn13);

			Toolbars.FixupButtonSizes(m_Toolbar);

			Controls.Add(m_Toolbar);
			m_Trans.Translate(m_Toolbar.Items, false);
		}

		private void OnGoToToday(object sender, EventArgs e)
		{
            m_DayView.GoToToday();
		}

		private void OnShow1DayView(object sender, EventArgs e)
		{
			if (m_DayView.DaysShowing != 1)
				SetDaysShowing(1);
		}

		private void OnShow3DayView(object sender, EventArgs e)
		{
			if (m_DayView.DaysShowing != 3)
				SetDaysShowing(3);
		}

        private void OnShow7DayView(object sender, EventArgs e)
        {
            if (m_DayView.DaysShowing != 7)
                SetDaysShowing(7);
        }

		private void OnShow14DayView(object sender, EventArgs e)
		{
			if (m_DayView.DaysShowing != 14)
				SetDaysShowing(14);
		}

		private void OnShow28DayView(object sender, EventArgs e)
		{
			if (m_DayView.DaysShowing != 28)
				SetDaysShowing(28);
		}

		private void SetDaysShowing(int numDays)
		{
			m_DayView.DaysShowing = numDays;
            m_WeekLabel.NumDays = numDays;

			string format = m_Trans.Translate("Next/Previous {0} days", Translator.Type.ToolTip);
			m_DayView.HScrollTooltipText = String.Format(format, m_DayView.HScrollStep);

			UpdateToolbarButtonStates();
            UpdatedSelectedTaskDatesPosition();
		}

		private void OnDeleteTimeBlock(object sender, EventArgs e)
		{
			if (m_DayView.DeleteSelectedTimeBlock())
				UpdateToolbarButtonStates();
		}

		private void OnDeleteTimeBlockSeries(object sender, EventArgs e)
		{
			if (m_DayView.DeleteSelectedTimeBlockSeries())
				UpdateToolbarButtonStates();
		}

		private void OnEditTimeBlockSeries(object sender, EventArgs e)
		{
			EditSelectedTimeBlockSeries();
		}

		private bool EditSelectedTimeBlockSeries()
		{
			if (m_DayView.SelectedAppointment is TaskTimeBlock)
			{
				var block = (m_DayView.SelectedAppointment as TaskTimeBlock);
				var series = m_DayView.SelectedTimeBlockSeries;

				if (series != null)
				{
					var dlg = new DayViewEditTimeBlockSeriesDlg(block.Title,
																block.RealTaskId, 
																m_WorkWeek, 
																m_DayView.DisplayDatesInISO,
																series.Attributes, 
																m_DefaultTimeBlockEditMask,
																m_Trans);
					FormsUtil.SetFont(dlg, m_ControlsFont);

					if (dlg.ShowDialog() != DialogResult.OK)
						return false;

					m_DefaultTimeBlockEditMask = dlg.EditMask;

					var attribs = dlg.Attributes;
					attribs.SynchroniseDates(block.RealTask);

					if (!m_DayView.EditSelectedTimeBlockSeries(attribs, dlg.EditMask))
						return false;

					return true;
				}
			}

			return false;
		}

		private void OnNewTimeBlock(object sender, EventArgs e)
		{
			if (CreateTimeBlock())
				UpdateToolbarButtonStates();
		}

		private bool CreateTimeBlock()
		{
			// Display a dialog to retrieve the task ID from a list
			// to support tasks without dates
			var attribs = new TimeBlockSeriesAttributes(m_DefaultNewTimeBlockAttributes);

			if (m_DayView.SelectionType == Calendar.SelectionType.DateRange)
			{
				var dates = m_DayView.SelectedDates;

				attribs.FromDate = attribs.ToDate = dates.Start.Date;
				attribs.FromTime = dates.Start.TimeOfDay;
				attribs.ToTime = dates.End.TimeOfDay;
				attribs.SyncToTaskDates = false;
			}
			else if (m_DayView.SelectedAppointment is TaskItem)
			{
				var dates = m_DayView.SelectedAppointment.Dates;

				attribs.FromDate = dates.Start.Date;
				attribs.ToDate = dates.End.Date;

				// Use default times
			}
			else if (m_DayView.SelectedAppointment is TaskTimeBlock)
			{
				// Use time from time block
				var dates = m_DayView.SelectedAppointment.Dates;

				attribs.FromTime = dates.Start.TimeOfDay;
				attribs.ToTime = dates.End.TimeOfDay;

				// Use dates from real task
				dates = m_DayView.GetRealAppointment(m_DayView.SelectedAppointment).Dates;

				attribs.FromDate = dates.Start.Date;
				attribs.ToDate = dates.End.Date;
			}

			var dlg = new DayViewCreateTimeBlockDlg(m_DayView.TaskItems, 
													new UIExtension.TaskIcon(m_HwndParent),
													m_WorkWeek,
													m_DayView.DisplayDatesInISO,
													m_SelectedTaskId,
													attribs,
													m_Trans);

			FormsUtil.SetFont(dlg, m_ControlsFont);

			m_DayView.ForceShowSelection = true;

			var res = dlg.ShowDialog();

			m_DayView.ForceShowSelection = false;
			
			if (res != DialogResult.OK)
				return false;

			m_DefaultNewTimeBlockAttributes = attribs = dlg.Attributes;
			attribs.SynchroniseDates(m_DayView.GetAppointment(dlg.SelectedTaskId));

			return m_DayView.CreateNewTaskBlockSeries(dlg.SelectedTaskId, attribs);
		}

		private void OnDuplicateTimeBlock(object sender, EventArgs e)
		{
			m_DayView.DuplicateSelectedTimeBlock();
		}

		private void UpdateToolbarButtonStates()
		{
			(m_Toolbar.Items["Show1DayView"] as ToolStripButton).Checked = (m_DayView.DaysShowing == 1);
			(m_Toolbar.Items["Show3DayView"] as ToolStripButton).Checked = (m_DayView.DaysShowing == 3);
            (m_Toolbar.Items["Show7DayView"] as ToolStripButton).Checked = (m_DayView.DaysShowing == 7);
			(m_Toolbar.Items["Show14DayView"] as ToolStripButton).Checked = (m_DayView.DaysShowing == 14);
            (m_Toolbar.Items["Show28DayView"] as ToolStripButton).Checked = (m_DayView.DaysShowing == 28);

			m_Toolbar.Items["NewTimeBlock"].Enabled = m_DayView.CanCreateNewTaskBlockSeries;
			m_Toolbar.Items["DeleteTimeBlock"].Enabled = m_DayView.CanDeleteSelectedTimeBlock;
			m_Toolbar.Items["DuplicateTimeBlock"].Enabled = m_DayView.CanDuplicateSelectedTimeBlock;

			m_Toolbar.Items["EditTimeBlockSeries"].Enabled = m_DayView.CanEditSelectedTimeBlockSeries;
			m_Toolbar.Items["DeleteTimeBlockSeries"].Enabled = m_DayView.CanDeleteSelectedTimeBlockSeries;
		}

		private void OnPreferences(object sender, EventArgs e)
		{
			m_PrefsDlg.StartPosition = FormStartPosition.CenterParent;

            if (m_PrefsDlg.ShowDialog() == DialogResult.OK)
				UpdateDayViewPreferences();
		}

		private void UpdateDayViewPreferences()
		{
			string tag;
            m_DayView.SetHideParentTasks(m_PrefsDlg.GetHideParentTasks(out tag), tag);

			m_DayView.DisplayLongTasksContinuous = m_PrefsDlg.DisplayTasksContinuous;
			m_DayView.DisplayActiveTasksToday = m_PrefsDlg.DisplayActiveTasksToday;
			m_DayView.HideTasksWithoutTimes = m_PrefsDlg.HideTasksWithoutTimes;
            m_DayView.HideTasksSpanningWeekends = m_PrefsDlg.HideTasksSpanningWeekends;
            m_DayView.HideTasksSpanningDays = m_PrefsDlg.HideTasksSpanningDays;
			m_DayView.ShowFutureOccurrences = m_PrefsDlg.ShowFutureOccurrences;
			m_DayView.ShowWorkingHoursOnly = m_PrefsDlg.ShowWorkingHoursOnly;
			m_DayView.TreatOverdueTasksAsDueToday = m_PrefsDlg.TreatOverdueTasksAsDueToday;
			m_DayView.LegacyScrollbars = m_PrefsDlg.LegacyScrollbars;

			m_DayView.SlotsPerHour = (60 / m_PrefsDlg.SlotMinutes);
			m_DayView.MinSlotHeight = DPIScaling.Scale(m_PrefsDlg.MinSlotHeight);

			UpdateWorkingHourDisplay();

			m_DayView.Invalidate();
		}

		private void UpdateWorkingHourDisplay()
		{
            m_DayView.WorkStart.Set(m_WorkWeek.WorkDay().StartOfDayInHours());
            m_DayView.WorkEnd.Set(m_WorkWeek.WorkDay().EndOfDayInHours());

            m_DayView.LunchStart.Set(m_WorkWeek.WorkDay().StartOfLunchInHours());
            m_DayView.LunchEnd.Set(m_WorkWeek.WorkDay().EndOfLunchInHours());
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
			m_MonthCombo.SelectedIndexChanged += new EventHandler(OnMonthYearSelChanged);

			Controls.Add(m_MonthCombo);

			m_YearCombo = new YearComboBox();

			m_YearCombo.Font = m_ControlsFont;
            m_YearCombo.Location = new Point(DPIScaling.Scale(105), ComboTop);
            m_YearCombo.Size = DPIScaling.Scale(new Size(100, 16));
			m_YearCombo.SelectedIndexChanged += new EventHandler(OnMonthYearSelChanged);

			Controls.Add(m_YearCombo);
		}

		// Message handlers ------------------------------------------------------------------
        protected override void OnGotFocus(EventArgs e)
        {
            base.OnGotFocus(e);

            m_DayView.Focus();
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

			m_DayView.Bounds = dayViewRect;

            Invalidate(true);
        }

		private void HandleDayViewLeftMouseClick(MouseEventArgs e, bool doubleClick)
		{
			var appt = m_DayView.GetAppointmentAt(e.Location.X, e.Location.Y);

			if (appt == null)
				return;

			// Handle icon click on all types
			var realAppt = m_DayView.GetRealAppointment(appt);

			if (!m_DayView.ReadOnly && !realAppt.Locked &&
				(m_DayView.IconHitTest(m_DayView.PointToScreen(e.Location)) > 0))
			{
				var notify = new UIExtension.ParentNotify(m_HwndParent);
				notify.NotifyEditIcon();

				return;
			}

			// Handle double-click differently for each type
			if (doubleClick)
			{
				if (appt is TaskItem)
				{
					if (!m_DayView.ReadOnly && !appt.Locked)
					{
						var notify = new UIExtension.ParentNotify(m_HwndParent);
						notify.NotifyEditLabel();
					}
				}
				else if (appt is TaskTimeBlock)
				{
					EditSelectedTimeBlockSeries();
				}
			}
		}

		private void OnDayViewMouseClick(object sender, MouseEventArgs e)
        {
			if (e.Button == MouseButtons.Left)
				HandleDayViewLeftMouseClick(e, false);
		}

		private void OnDayViewMouseDoubleClick(object sender, MouseEventArgs e)
		{
			if (e.Button == MouseButtons.Left)
				HandleDayViewLeftMouseClick(e, true);
		}

		private void OnDayViewMouseWheel(object sender, MouseEventArgs e)
		{
			if (ModifierKeys.Equals(Keys.Control))
			{
				bool change = ((e.Delta < 0) ? m_PrefsDlg.DecrementMinSlotHeight() : m_PrefsDlg.IncrementMinSlotHeight());

				if (change)
					m_DayView.MinSlotHeight = m_PrefsDlg.MinSlotHeight;
			}
		}

		private void OnDayViewSelectionChanged(object sender, Calendar.AppointmentEventArgs args)
		{
            UIExtension.ParentNotify notify = new UIExtension.ParentNotify(m_HwndParent);

			switch (m_DayView.SelectionType)
			{
			case Calendar.SelectionType.Appointment:
				if (m_DayView.SelectedTaskId != m_SelectedTaskId)
				{
					m_SelectedTaskId = m_DayView.SelectedTaskId;
					notify.NotifySelChange(m_SelectedTaskId);

					UpdatedSelectedTaskDatesText();
				}
				break;
			}

			UpdateToolbarButtonStates();
		}

		private void UpdatedSelectedTaskDatesText()
		{
			DateTime from, to;

			if (m_DayView.GetSelectedTaskDates(out from, out to))
			{
				m_SelectedTaskDates.SetRange(from, to);
				m_SelectedTaskDatesLabel.Visible = true;
			}
			else
			{
				m_SelectedTaskDates.ClearRange();
				m_SelectedTaskDatesLabel.Visible = false;
			}
		}

		private void UpdatedSelectedTaskDatesPosition()
		{
			// Align with the base of the combo text to match core app
			Point pt = new Point(m_WeekLabel.Right + 10, 0);

			pt.Y = (m_YearCombo.Top + m_YearCombo.Bottom + m_YearCombo.ItemHeight) / 2;
			pt.Y -= m_SelectedTaskDatesLabel.Height;

			// Label part
			m_SelectedTaskDatesLabel.Location = pt;

			// Link part
			pt.X = (m_SelectedTaskDatesLabel.Right + 10);

			m_SelectedTaskDates.Location = pt;
			m_SelectedTaskDates.Height = m_SelectedTaskDatesLabel.Height;
		}

		private void UpdateMonthYearCombos(DateTime date)
		{
			if (!m_SettingDayViewStartDate)
			{
				m_SettingMonthYear = true;

				int year = 0, month = 0, unused = 0;
				DateUtil.FromDate(date, ref year, ref month, ref unused);

				m_MonthCombo.SelectedMonth = month;
				m_YearCombo.SelectedYear = year;

				m_SettingMonthYear = false;
			}
		}

		private void OnDayViewWeekChanged(object sender, Calendar.WeekChangeEventArgs args)
		{
			m_WeekLabel.StartDate = args.StartDate;

			UpdateMonthYearCombos(args.StartDate);
			UpdatedSelectedTaskDatesPosition();
		}

		private void OnMonthYearSelChanged(object sender, EventArgs args)
		{
			if (!m_SettingMonthYear)
			{
				m_SettingDayViewStartDate = true;

				m_DayView.StartDate = DateUtil.ToDate(m_YearCombo.SelectedYear, m_MonthCombo.SelectedMonth, 1);
				m_WeekLabel.StartDate = m_DayView.StartDate;

				UpdatedSelectedTaskDatesPosition();

				m_SettingDayViewStartDate = false;
			}
		}

        private void OnDayViewAppointmentChanged(object sender, TDLMoveAppointmentEventArgs args)
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

			var item = args.Appointment as TaskItem;

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
			m_DayView.Invalidate();
		}

		private bool PrepareTaskNotify(TaskItem item, Calendar.SelectionTool.Mode mode, UIExtension.ParentNotify notify, bool includeTimeEstimate = true)
		{
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

			return false;
		}

        private bool WantModifyTimeEstimate(TaskItem item)
        {
			if (!m_AllowModifyTimeEstimate)
				return false;

			if (!item.TimeEstimateIsMinsOrHours)
				return false;

			if (!item.IsSingleDay || (item.Length.Days != 0))
				return false;

			if (item.TimeEstimateMatchesOriginalLength(m_WorkWeek))
				return true;

			if (item.TimeEstimate != 0.0)
				return false;

			// else
			return true;
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
			return m_DayView.SaveToImage();
		}

		public Boolean CanSaveToImage()
		{
			return m_DayView.CanSaveToImage();
		}

	}


}
