using System;
using System.Drawing;
using System.Drawing.Imaging;
using System.Diagnostics;
using System.Windows.Forms;
using System.Collections.Generic;
using System.Windows.Forms.VisualStyles;

using Abstractspoon.Tdl.PluginHelpers;
using Abstractspoon.Tdl.PluginHelpers.ColorUtil;

///////////////////////////////////////////////////////////////////

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
		private EisenhowerMatrices m_Matrices;
		private UIExtension.TaskIcon m_TaskIcons;
		private Font m_ControlsFont;
		private UIThemeToolbarRenderer m_TBRenderer;
		private UIExtension.IdleRedraw m_IdleTasks = new UIExtension.IdleRedraw();

		private string m_PrevMatrices;
		private string m_PrevSelMatrix;

		// ------------------------------------------------

		public EisenhowerUIExtensionCore(string typeID, string uiName, IntPtr parentHandle, Translator trans)
		{
			InitializeComponent();

			// Fixup the control positions under high DPI
			if (DPIScaling.WantScaling())
			{
				foreach (Control ctrl in Controls)
				{
					ctrl.Size = DPIScaling.Scale(ctrl.Size);
					ctrl.Location = DPIScaling.Scale(ctrl.Location);
				}

				// Manual tweaking
				m_EisenhowerCtrl.Top -= 4;
				m_EisenhowerCtrl.Height += 4;

				int imageSize = DPIScaling.Scale(16);

				m_Toolbar.ImageScalingSize = new Size(imageSize, imageSize);
				m_Toolbar.Height = (imageSize + 7); // MFC
			}

			m_TypeID = typeID;
			m_UiName = uiName;
			m_Trans = trans;
			m_HwndParent = parentHandle;

			m_ControlsFont = new Font(FontName, 8, FontStyle.Regular);
			m_TaskIcons = new UIExtension.TaskIcon(parentHandle);
			m_Data = new EisenhowerData(trans);
			m_Matrices = new EisenhowerMatrices();

			m_TBRenderer = new UIThemeToolbarRenderer();
			m_Toolbar.Renderer = m_TBRenderer;
			Toolbars.FixupButtonSizes(m_Toolbar);

			m_EisenhowerCtrl.Initialize(m_Data.Tasks, trans, m_TaskIcons);

			m_EisenhowerCtrl.EditTaskDone    += new EditTaskCompletionEventHandler(OnEisenhowerCtrlEditTaskDone);
			m_EisenhowerCtrl.EditTaskIcon    += new EditTaskIconEventHandler(OnEisenhowerCtrlEditTaskIcon);
			m_EisenhowerCtrl.EditTaskLabel   += new EditTaskLabelEventHandler(OnEisenhowerCtrlEditTaskLabel);
			m_EisenhowerCtrl.SelectionChange += new SelectionChangeEventHandler(OnEisenhowerCtrlSelectionChange);
			m_EisenhowerCtrl.AttributeChange += new AttributeChangeEventHandler(OnEisenhowerCtrlAttributeChange);

			m_OptionsCombo.Initialise(trans);
			m_OptionsCombo.Sorted = true;
			m_OptionsCombo.DropDownClosed += new EventHandler(OnOptionsComboClosed);

			m_Trans.Translate(this);

			FormsUtil.SetFont(this, m_ControlsFont);
			EnableMatrixComboEvents(true);
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

			if (!InitialiseMatrices() && !UpdateMatrices(result))
			{
				// In-place update
				m_EisenhowerCtrl.OnUpdateTasks(type, result.ModifiedTaskIds);
			}

			UpdateToolbarButtonStates();

			if (selTaskIds != null)
				m_EisenhowerCtrl.SelectTasks(selTaskIds);

			// For reasons I don't yet understand, invalidation after a 
			// task update does NOT ALWAYS result in a subsequent repaint
			// so we solve it with a delayed-redraw
			m_IdleTasks.Redraw();
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

		public bool PrepareNewTask(ref Task task)
		{
			return m_EisenhowerCtrl.PrepareNewTask(ref task);
		}

		public bool ProcessMessage(IntPtr hwnd, UInt32 message, UInt32 wParam, UInt32 lParam, UInt32 time, Int32 xPos, Int32 yPos)
		{
			return false;
		}

		public void FilterToolTipMessage(IntPtr hwnd, UInt32 message, UInt32 wParam, UInt32 lParam, UInt32 time, Int32 xPos, Int32 yPos)
		{
			var msg = Message.Create(hwnd, (Int32)message, (IntPtr)(int)wParam, (IntPtr)(int)lParam);

			m_OptionsCombo.FilterTooltipMessage(msg);
		}

		public bool DoIdleProcessing()
		{
			return m_IdleTasks.Process(this);
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

		public UIExtension.HitTestResult HitTest(Int32 xScreen, Int32 yScreen, UIExtension.HitTestReason reason)
		{
			return m_EisenhowerCtrl.HitTest(new Point(xScreen, yScreen));
		}

		public UInt32 HitTestTask(Int32 xScreen, Int32 yScreen, UIExtension.HitTestReason reason)
		{
			return m_EisenhowerCtrl.HitTestTask(new Point(xScreen, yScreen), (reason == UIExtension.HitTestReason.ImageTip));
		}

		public void SetUITheme(UITheme theme)
		{
			BackColor = theme.GetAppDrawingColor(UITheme.AppColor.AppBackLight);
			m_MatricesLabel.ForeColor = theme.GetAppDrawingColor(UITheme.AppColor.AppText);

			m_EisenhowerCtrl.SetUITheme(theme);

			// Set the toolbar colors to be the same as the back color
			theme.SetAppDrawingColor(UITheme.AppColor.ToolbarDark, BackColor);
			theme.SetAppDrawingColor(UITheme.AppColor.ToolbarLight, BackColor);

			m_TBRenderer.SetUITheme(theme);
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
			prefs.WriteProfileString(key, "Matrices", m_Matrices.ToString());
			prefs.WriteProfileString(key, "SelMatrix", m_MatrixCombo.SelectedMatrix?.Id);

			m_EisenhowerCtrl.SavePreferences(prefs, key);
		}

		public void LoadPreferences(Preferences prefs, String key, bool appOnly)
		{
			m_Data.WorkingWeek = new WorkingWeek(prefs);

			if (!appOnly)
			{
				// Load prior matrices
				m_PrevSelMatrix = prefs.GetProfileString(key, "SelMatrix", "");
				m_PrevMatrices = prefs.GetProfileString(key, "Matrices", "");
			}

			m_EisenhowerCtrl.LoadPreferences(prefs, key, appOnly);
			m_OptionsCombo.SelectedOptions = m_EisenhowerCtrl.Options;

			SetPriorityRiskLevels(prefs.GetProfileInt("Preferences", "NumPriorityRiskLevels", 11));
		}

		private void SetPriorityRiskLevels(int numLevels)
		{
			if (numLevels != m_Data.NumPriorityRiskLevels)
			{
				m_Data.NumPriorityRiskLevels = numLevels;

				if (m_Data.Tasks.Count > 0)
				{
					var xVarAttribId = SelectedMatrix.XVariable.Attribute.AttributeId;
					var yVarAttribId = SelectedMatrix.YVariable.Attribute.AttributeId;

					if ((string.IsNullOrEmpty(SelectedMatrix.XCutoff) &&
						((xVarAttribId == Task.Attribute.Priority) ||
						 (xVarAttribId == Task.Attribute.Risk))) ||
						(string.IsNullOrEmpty(SelectedMatrix.YCutoff) &&
						((yVarAttribId == Task.Attribute.Priority) ||
						 (yVarAttribId == Task.Attribute.Risk))))
					{
						m_EisenhowerCtrl.OnUpdateTasks(UIExtension.UpdateType.All, null);
					}
				}
			}
		}

		private bool InitialiseMatrices()
		{
			if (m_PrevMatrices == null)
				return false;

			if (m_Matrices.FromString(m_PrevMatrices, m_Data.Variables) == 0)
			{
				// Default matrices
				m_Matrices.Add(new EisenhowerMatrix()
				{
					XVariable = m_Data.Variables.Find(Task.Attribute.Priority),
					YVariable = m_Data.Variables.Find(Task.Attribute.Risk),
					XCutoff = "5",
					YCutoff = "5"
				});

				m_Matrices.Add(new EisenhowerMatrix()
				{
					XVariable = m_Data.Variables.Find(Task.Attribute.Risk),
					YVariable = m_Data.Variables.Find(Task.Attribute.Priority),
					XCutoff = "5",
					YCutoff = "5"
				});
			}

			var selMatrix = m_Matrices.Find(m => (m.Id == m_PrevSelMatrix));
			
			if (selMatrix == null)
				selMatrix = m_Matrices.FirstOrNull;

			m_PrevMatrices = null;
			m_PrevSelMatrix = null;

			m_MatrixCombo.Populate(m_Matrices, m_Trans);
			m_MatrixCombo.SelectedMatrix = selMatrix;

			m_EisenhowerCtrl.SetMatrix(selMatrix);

			return true;
		}

		private bool UpdateMatrices(UpdateResult res)
		{
			var selMatrix = m_MatrixCombo.SelectedMatrix;

			m_MatrixCombo.Populate(m_Matrices, m_Trans);

			var xUpdatedVar = m_Data.Variables.Find(selMatrix.XVariable);
			var yUpdatedVar = m_Data.Variables.Find(selMatrix.YVariable);

			if (selMatrix != null)
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
				// If the existing matrix variables have been removed
				// then we need to revert to known types
				if ((xUpdatedVar == null) || (yUpdatedVar == null))
				{
					selMatrix = m_Matrices.FirstOrNull;
				}
				else if (xUpdatedVar == null)
				{
					selMatrix = new EisenhowerMatrix()
					{
						XVariable = EisenhowerVariable.Null,
						YVariable = yUpdatedVar
					};
				}
				else
				{
					Debug.Assert(yUpdatedVar == null);

					selMatrix = new EisenhowerMatrix()
					{
						XVariable = yUpdatedVar,
						YVariable = EisenhowerVariable.Null
					};
				}
			}

			m_EisenhowerCtrl.SetMatrix(selMatrix);
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

		private void EnableMatrixComboEvents(bool enable)
		{
			if (enable)
				m_MatrixCombo.SelectedIndexChanged += new EventHandler(OnMatrixComboSelectionChange);
			else
				m_MatrixCombo.SelectedIndexChanged -= new EventHandler(OnMatrixComboSelectionChange);
		}

		private void OnMatrixComboSelectionChange(object sender, EventArgs e)
		{
			// Update matrix
			m_EisenhowerCtrl.SetMatrix(m_MatrixCombo.SelectedMatrix);
			m_EisenhowerCtrl.Focus();

			UpdateToolbarButtonStates();
		}

		private void OnOptionsComboClosed(object sender, EventArgs e)
		{
			m_EisenhowerCtrl.Options = m_OptionsCombo.SelectedOptions;
		}

		private void UpdateToolbarButtonStates()
		{
			// TODO
		}

		private EisenhowerMatrix SelectedMatrix
		{
			get { return m_MatrixCombo.SelectedMatrix; }
		}

		private void OnPreferences(object sender, EventArgs e)
		{
			var prevSelMatrix = SelectedMatrix;

			var dlg = new EisenhowerPreferencesDlg(m_Trans, m_Data.Variables, m_Matrices);

			if (dlg.ShowDialog() == DialogResult.OK)
			{
				EnableMatrixComboEvents(true);

				m_Matrices = dlg.Matrices;
				m_MatrixCombo.Populate(m_Matrices, m_Trans);

				var newSelMatrix = (SelectedMatrix ?? m_Matrices.FirstOrNull ?? EisenhowerMatrix.Null);

				if (!newSelMatrix.Equals(prevSelMatrix))
					m_EisenhowerCtrl.SetMatrix(newSelMatrix);

				m_MatrixCombo.SelectedMatrix = newSelMatrix;
				EnableMatrixComboEvents(false);
			}
		}

		private void OnHelp(object sender, EventArgs e)
		{
			var notify = new UIExtension.ParentNotify(m_HwndParent);
			notify.NotifyDoHelp(m_TypeID);
		}
	}

}
