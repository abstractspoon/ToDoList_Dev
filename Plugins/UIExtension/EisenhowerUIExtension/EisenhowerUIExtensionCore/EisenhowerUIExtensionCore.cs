using System;
using System.Drawing;
using System.Drawing.Imaging;
using System.Diagnostics;
using System.Windows.Forms;
using System.Collections.Generic;
using System.Windows.Forms.VisualStyles;

using Abstractspoon.Tdl.PluginHelpers;
using Abstractspoon.Tdl.PluginHelpers.ColorUtil;


namespace EisenhowerUIExtension
{
	public partial class EisenhowerUIExtensionCore : UserControl, IUIExtension
	{
		private const string FontName = "Tahoma";

		// ------------------------------------------------

		private string m_TypeID;
		private string m_UiName;
		private IntPtr m_HwndParent;
		private Translator m_Trans;

		private EisenhowerData m_Data;
		private EisenhowerFilters m_Filters;
		private UIExtension.TaskIcon m_TaskIcons;
		private Font m_ControlsFont;

		private string m_PrevFilters;
		private string m_PrevSelFilter;

		// ------------------------------------------------

		public EisenhowerUIExtensionCore(string typeID, string uiName, IntPtr parentHandle, Translator trans)
		{
			InitializeComponent();

			m_TypeID = typeID;
			m_UiName = uiName;
			m_Trans = trans;
			m_HwndParent = parentHandle;

			m_ControlsFont = new Font(FontName, 8, FontStyle.Regular);
			m_TaskIcons = new UIExtension.TaskIcon(parentHandle);
			m_Data = new EisenhowerData(trans);
			m_Filters = new EisenhowerFilters();

			m_EisenhowerCtrl.Initialize(m_Data.Tasks, trans, m_TaskIcons);

			m_EisenhowerCtrl.EditTaskDone    += new EditTaskCompletionEventHandler(OnEisenhowerCtrlEditTaskDone);
			m_EisenhowerCtrl.EditTaskIcon    += new EditTaskIconEventHandler(OnEisenhowerCtrlEditTaskIcon);
			m_EisenhowerCtrl.EditTaskLabel   += new EditTaskLabelEventHandler(OnEisenhowerCtrlEditTaskLabel);
			m_EisenhowerCtrl.SelectionChange += new SelectionChangeEventHandler(OnEisenhowerCtrlSelectionChange);
			m_EisenhowerCtrl.AttributeChange += new AttributeChangeEventHandler(OnEisenhowerCtrlAttributeChange);

			EnableFilterComboEvents(true);

			FormsUtil.SetFont(this, m_ControlsFont);
		}

		public void UpdateTasks(TaskList tasks, UIExtension.UpdateType type)
		{
			var selTaskIds = m_EisenhowerCtrl.SelectedTaskIds;

			// EisenhowerData.Update() returns:
			//
			// 1. A list of IDs of tasks which have been:
			//
			//	 1.1. Added
			//	 1.2. Removed
			//	 1.3. Modified
			//
			// 2. A list of variables which have been:
			//	 2.1. Added
			//   2.2. Removed
			//   2.3. Had their numeric type changed (int <=> double)
			//   2.4. Had their max/min range updated
			// 
			var result = m_Data.Update(tasks, type);

			if (!InitialiseFilters() && !UpdateFilters(result))
			{
				// In-place update
				m_EisenhowerCtrl.OnUpdateTasks(type, result.ModifiedTaskIds);
			}

			UpdateToolbarButtonStates();

			if (selTaskIds != null)
				m_EisenhowerCtrl.SelectTasks(selTaskIds);
		}

		public bool WantTaskUpdate(Task.Attribute attrib)
		{
			return m_EisenhowerCtrl.WantTaskUpdate(attrib);
		}

		public bool SelectTask(uint taskId)
		{
			return m_EisenhowerCtrl.SelectTask(taskId);
		}

		public bool SelectTasks(uint[] taskIDs)
		{
			return m_EisenhowerCtrl.SelectTasks(taskIDs);
		}

		public bool SelectTask(String text, UIExtension.SelectTask selectTask, bool caseSensitive, bool wholeWord, bool findReplace)
		{
			return m_EisenhowerCtrl.SelectTask(text, selectTask, caseSensitive, wholeWord, findReplace);
		}

		public bool ScrollToSelectedTask()
		{
			if (CanScrollToSelectedTask())
			{
				m_EisenhowerCtrl.EnsureSelectionVisible();
				return true;
			}

			return false;
		}

		public bool CanScrollToSelectedTask()
		{
			return m_EisenhowerCtrl.HasSelection;
		}

		public bool WantSortUpdate(Task.Attribute attrib)
		{
			return false;
		}

		public bool PrepareNewTask(ref Task task)
		{
			return m_EisenhowerCtrl.PrepareNewTask(ref task);
		}

		public bool ProcessMessage(IntPtr hwnd, UInt32 message, UInt32 wParam, UInt32 lParam, UInt32 time, Int32 xPos, Int32 yPos)
		{
			return false;
		}

		public bool DoIdleProcessing()
		{
			return m_EisenhowerCtrl.DoIdleProcessing();
		}

		public bool GetLabelEditRect(ref Int32 left, ref Int32 top, ref Int32 right, ref Int32 bottom)
		{
			Rectangle labelRect = m_EisenhowerCtrl.SelectedTaskLabelRect;
			
			if (labelRect.IsEmpty)
				return false;
			
			labelRect = m_EisenhowerCtrl.RectangleToScreen(labelRect);
			
			left = labelRect.Left;
			top = labelRect.Top;
			right = labelRect.Right;
			bottom = labelRect.Bottom;

			return true;
		}

		public UIExtension.HitTestResult HitTest(Int32 xPos, Int32 yPos, UIExtension.HitTestReason reason)
		{
			var ptScreen = new Point(xPos, yPos);
			
			if (m_EisenhowerCtrl.HitTestTask(ptScreen) != 0)
				return UIExtension.HitTestResult.Task;
			
			// else
			if (m_EisenhowerCtrl.RectangleToScreen(m_EisenhowerCtrl.ClientRectangle).Contains(ptScreen))
				return UIExtension.HitTestResult.Tasklist;

			return UIExtension.HitTestResult.Nowhere;
		}

		public UInt32 HitTestTask(Int32 xPos, Int32 yPos, UIExtension.HitTestReason reason)
		{
			return m_EisenhowerCtrl.HitTestTask(new Point(xPos, yPos));
		}

		public void SetUITheme(UITheme theme)
		{
			BackColor = theme.GetAppDrawingColor(UITheme.AppColor.AppBackLight);

			m_EisenhowerCtrl.SetUITheme(theme);

			m_FilterLabel.ForeColor = theme.GetAppDrawingColor(UITheme.AppColor.AppText);
		}

		public void SetTaskFont(String faceName, int pointSize)
		{
			m_EisenhowerCtrl.SetFont(faceName, pointSize);
		}

		public void SetReadOnly(bool bReadOnly)
		{
			m_EisenhowerCtrl.ReadOnly = bReadOnly;
		}

		public void SavePreferences(Preferences prefs, String key)
		{
			prefs.WriteProfileString(key, "Filters", m_Filters.ToString());
			prefs.WriteProfileString(key, "SelFilter", m_FilterCombo.SelectedFilter?.Id);

			m_EisenhowerCtrl.SavePreferences(prefs, key);
		}

		public void LoadPreferences(Preferences prefs, String key, bool appOnly)
		{
			m_Data.WorkingWeek = new WorkingWeek(prefs);

			if (!appOnly)
			{
				// Load prior filters
				m_PrevSelFilter = prefs.GetProfileString(key, "SelFilter", "");
				m_PrevFilters = prefs.GetProfileString(key, "Filters", "");
			}

			m_EisenhowerCtrl.LoadPreferences(prefs, key, appOnly);

			SetPriorityRiskLevels(prefs.GetProfileInt("Preferences", "NumPriorityRiskLevels", 11));
		}

		private void SetPriorityRiskLevels(int numLevels)
		{
			if (numLevels != m_Data.NumPriorityRiskLevels)
			{
				m_Data.NumPriorityRiskLevels = numLevels;

				if (m_Data.Tasks.Count > 0)
				{
					var xVarAttribId = SelectedFilter.XVariable.Attribute.AttributeId;
					var yVarAttribId = SelectedFilter.YVariable.Attribute.AttributeId;

					if ((string.IsNullOrEmpty(SelectedFilter.XCutoff) &&
						((xVarAttribId == Task.Attribute.Priority) ||
						 (xVarAttribId == Task.Attribute.Risk))) ||
						(string.IsNullOrEmpty(SelectedFilter.YCutoff) &&
						((yVarAttribId == Task.Attribute.Priority) ||
						 (yVarAttribId == Task.Attribute.Risk))))
					{
						m_EisenhowerCtrl.OnUpdateTasks(UIExtension.UpdateType.All, null);
					}
				}
			}
		}

		private bool InitialiseFilters()
		{
			if (m_PrevFilters == null)
				return false;

			if (m_Filters.FromString(m_PrevFilters, m_Data.Variables) == 0)
			{
				// Default filters
				m_Filters.Add(new EisenhowerFilter()
				{
					XVar = m_Data.Variables.Find(Task.Attribute.Priority),
					YVar = m_Data.Variables.Find(Task.Attribute.Risk),
					XCutoff = "5",
					YCutoff = "5"
				});

				m_Filters.Add(new EisenhowerFilter()
				{
					XVar = m_Data.Variables.Find(Task.Attribute.Risk),
					YVar = m_Data.Variables.Find(Task.Attribute.Priority),
					XCutoff = "5",
					YCutoff = "5"
				});
			}

			m_PrevFilters = null;

			m_FilterCombo.Populate(m_Filters, m_Trans);
			m_FilterCombo.SelectedFilter = m_Filters.FirstOrNull;

			m_EisenhowerCtrl.SetFilter(m_Filters.FirstOrNull);

			return true;
		}

		private bool UpdateFilters(UpdateResult res)
		{
			var selFilter = m_FilterCombo.SelectedFilter;

			m_FilterCombo.Populate(m_Filters, m_Trans);

			var xUpdatedVar = m_Data.Variables.Find(m_EisenhowerCtrl.XFilterVariable);
			var yUpdatedVar = m_Data.Variables.Find(m_EisenhowerCtrl.YFilterVariable);

			if (selFilter != null)
			{
				if (!res.ModifiedVariables.Contains(xUpdatedVar) &&
					!res.ModifiedVariables.Contains(yUpdatedVar))
				{
					return false;
				}

				// else ranges or types have changed => rebuild the lists below
			}
			else
			{
				// If the existing filter variables have been removed
				// then we need to revert to known types
				if ((xUpdatedVar == null) || (yUpdatedVar == null))
				{
					selFilter = m_Filters.FirstOrNull;
				}
				else if (xUpdatedVar == null)
				{
					selFilter = new EisenhowerFilter()
					{
						XVar = EisenhowerVariable.Null,
						YVar = yUpdatedVar
					};
				}
				else
				{
					Debug.Assert(yUpdatedVar == null);

					selFilter = new EisenhowerFilter()
					{
						XVar = yUpdatedVar,
						YVar = EisenhowerVariable.Null
					};
				}
			}

			m_EisenhowerCtrl.SetFilter(selFilter);
			return true;
		}

		public new bool Focused
		{
			get { return m_EisenhowerCtrl.Focused; }
		}

		public bool CanMoveTask(UInt32 taskId, UInt32 destParentId, UInt32 destPrevSiblingId)
		{
			// Not supported
			return false;
		}

		public bool MoveTask(UInt32 taskId, UInt32 destParentId, UInt32 destPrevSiblingId)
		{
			// Not supported
			return false;
		}

		public bool GetTask(UIExtension.GetTask getTask, ref UInt32 taskId)
		{
			taskId = m_EisenhowerCtrl.GetTaskId(getTask);

			return (taskId != 0);
		}

		public Bitmap SaveToImage()
		{
			return m_EisenhowerCtrl.SaveToImage();
		}

		public bool CanSaveToImage()
		{
			return m_EisenhowerCtrl.CanSaveToImage();
		}

		// Message handlers ---------------------------------------------------

		protected override void OnGotFocus(EventArgs e)
		{
			base.OnGotFocus(e);

			m_EisenhowerCtrl.Focus();
		}

		protected override void OnSizeChanged(EventArgs e)
		{
			base.OnSizeChanged(e);

			Rectangle rCtrl = m_EisenhowerCtrl.Bounds;

			rCtrl.X = 0;
			rCtrl.Width = Width;
			rCtrl.Height = (Height - rCtrl.Top - 1);

			m_EisenhowerCtrl.Bounds = rCtrl;
		}

		private bool OnEisenhowerCtrlEditTaskDone(object sender, ITaskBase task)
		{
			var notify = new UIExtension.ParentNotify(m_HwndParent);

			return notify.NotifyMod(Task.Attribute.DoneDate,
									(task.IsDone ? DateTime.Now : DateTime.MinValue));
		}

		private bool OnEisenhowerCtrlEditTaskIcon(object sender, ITaskBase task)
		{
			var notify = new UIExtension.ParentNotify(m_HwndParent);
			return notify.NotifyEditIcon();
		}

		private bool OnEisenhowerCtrlEditTaskLabel(object sender, ITaskBase task)
		{
			var notify = new UIExtension.ParentNotify(m_HwndParent);
			return notify.NotifyEditLabel();
		}

		private void OnEisenhowerCtrlSelectionChange(object sender, IList<UInt32> taskIds)
		{
			var notify = new UIExtension.ParentNotify(m_HwndParent);
			notify.NotifySelChange(taskIds);
		}

		private bool OnEisenhowerCtrlAttributeChange(Object sender, AttributeChangeEventArgs args)
		{
			var notify = new UIExtension.ParentNotify(m_HwndParent);

			args.TaskIds.ForEach(id =>
			{
				CheckAddNotification(id, args.XAttrib, args.XValue, notify);
				CheckAddNotification(id, args.YAttrib, args.YValue, notify);
			});

			return notify.NotifyMod();
		}

		static private void CheckAddNotification(uint taskId, EisenhowerVariable var, double value, UIExtension.ParentNotify notify)
		{
			if (var != null)
			{
				if (var.Attribute.IsCustom())
				{
					// Always as string
					notify.AddMod(taskId, var.Attribute.CustomAttributeId, value.ToString());
				}
				else
				{
					switch (var.Type)
					{
					case EisenhowerVariable.ValueType.Integer:
						notify.AddMod(taskId, var.Attribute.AttributeId, (int)value);
						break;

					case EisenhowerVariable.ValueType.Decimal:
						notify.AddMod(taskId, var.Attribute.AttributeId, value);
						break;

					case EisenhowerVariable.ValueType.Boolean:
						notify.AddMod(taskId, var.Attribute.AttributeId, (value != 0));
						break;

					case EisenhowerVariable.ValueType.TimePeriod:
						notify.AddMod(taskId, var.Attribute.AttributeId, value);
						break;

					case EisenhowerVariable.ValueType.Date:
						notify.AddMod(taskId, var.Attribute.AttributeId, DateTime.FromOADate(value));
						break;
					}
				}
			}
		}

		private void EnableFilterComboEvents(bool enable)
		{
			if (enable)
				m_FilterCombo.SelectedIndexChanged += new EventHandler(OnFilterComboSelectionChange);
			else
				m_FilterCombo.SelectedIndexChanged -= new EventHandler(OnFilterComboSelectionChange);
		}

		private void OnFilterComboSelectionChange(object sender, EventArgs e)
		{
			// Update filter
			var selTaskIds = m_EisenhowerCtrl.SelectedTaskIds;

			m_EisenhowerCtrl.SetFilter(m_FilterCombo.SelectedFilter);
			m_EisenhowerCtrl.SelectTasks(selTaskIds);
			m_EisenhowerCtrl.Focus();

			UpdateToolbarButtonStates();
		}

		private void UpdateToolbarButtonStates()
		{
			// TODO
		}

		private EisenhowerFilter SelectedFilter
		{
			get { return m_FilterCombo.SelectedFilter; }
		}

		private void OnPreferences(object sender, EventArgs e)
		{
			var prevSelFilter = SelectedFilter;

			var dlg = new EisenhowerPreferencesDlg(m_Trans, m_Data.Variables, m_Filters);

			if (dlg.ShowDialog() == DialogResult.OK)
			{
				EnableFilterComboEvents(true);

				m_Filters = dlg.Filters;
				m_FilterCombo.Populate(m_Filters, m_Trans);

				var newSelFilter = (SelectedFilter ?? m_Filters.FirstOrNull ?? EisenhowerFilter.Null);

				if (!newSelFilter.Equals(prevSelFilter))
					m_EisenhowerCtrl.SetFilter(newSelFilter);

				m_FilterCombo.SelectedFilter = newSelFilter;
				EnableFilterComboEvents(false);
			}
		}
	}

}
