using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Windows.Forms;
using System.IO;

using unvell.ReoGrid;
using unvell.ReoGrid.Editor;

using Abstractspoon.Tdl.PluginHelpers;
using Abstractspoon.Tdl.PluginHelpers.ColorUtil;

using Command.Handling;

///////////////////////////////////////////////////////////////////////////////

namespace SpreadsheetContentControl
{
	[System.ComponentModel.DesignerCategory("")]
	public class TDLGridEditorControl : ReoGridEditorControl
	{
		private UIThemeToolbarRenderer m_toolbarRenderer;
		private Font m_ControlsFont;
		private Translator m_Trans;

		private Byte[] m_PrevContent;

		// --------------------------------------------

		public event EventHandler ContentChanged;
		public new EventHandler LostFocus;

		// --------------------------------------------

		public TDLGridEditorControl(Font font, Translator trans)
		{
			m_toolbarRenderer = new UIThemeToolbarRenderer();
			m_ControlsFont = font;
			m_Trans = trans;

			InitialiseFeatures();
			InitialiseToolbars();
			InitialiseChangeCallbacks();

			Worksheet.DefaultFontName = m_ControlsFont.Name;
			Worksheet.DefaultFontSize = m_ControlsFont.SizeInPoints;

			m_Trans.Translate(MenuBar.Items);
			m_Trans.Translate(ToolBar.Items);
			m_Trans.Translate(FontBar.Items);
//			m_Trans.Translate(FormulaBar.Items);
//			m_Trans.Translate(StatusBar.Items);
			m_Trans.Translate(RowContextMenu.Items);
			m_Trans.Translate(CellContextMenu.Items);
			m_Trans.Translate(HeaderContextMenu.Items);
			m_Trans.Translate(ColumnContextMenu.Items);
		}

		public Byte[] GetContent()
		{
			try
			{
				var memStream = new MemoryStream();
				GridControl.Save(memStream, unvell.ReoGrid.IO.FileFormat.Excel2007);

				return Compression.Compress(memStream.GetBuffer());
			}
			catch (Exception)
			{
			}

			return new Byte[1];
		}

		public bool SetContent(Byte[] content, bool resetSelection)
		{
 			try
 			{
 				var memStream = new MemoryStream(Compression.Decompress(content));
 				GridControl.Load(memStream, unvell.ReoGrid.IO.FileFormat.Excel2007);

				// Restore the content font because loading changes it
				CurrentWorksheet.FontName = Worksheet.DefaultFontName;
				CurrentWorksheet.FontSize = Worksheet.DefaultFontSize;

				return true;
 			}
 			catch (Exception)
 			{
 			}

			return false;
		}

		private RangePosition ContentRange()
		{
			var worksheet = GridControl.CurrentWorksheet;

			return new RangePosition(0, 0, worksheet.MaxContentRow + 1, worksheet.MaxContentCol + 1);
		}

		// text content if supported. return false if not supported
		public String GetTextContent()
		{
			if (GridControl.CurrentWorksheet.IsEditing)
				GridControl.CurrentWorksheet.EndEdit(EndEditReason.NormalFinish);

			var text = GridControl.CurrentWorksheet.StringifyRange(ContentRange()).Trim();

			text = text.Replace("\t\t", ""); // leaves single tabs as spacers
			text = text.Replace("\n", " ");

			return text;
		}

		public bool SetTextContent(String content, bool resetSelection)
		{
			return false;
		}

		public bool InsertTextContent(String content, bool bAtEnd)
		{
			var selection = GridControl.CurrentWorksheet.SelectionRange;

			if (selection == null)
				return false;

			for (int row = selection.StartPos.Row; row <= selection.EndPos.Row; row++)
			{
				for (int col = selection.StartPos.Col; col <= selection.EndPos.Col; col++)
				{
					GridControl.CurrentWorksheet.PasteFromString(new CellPosition(row, col), content);
				}
			}

			NotifyParentContentChange();
			return true;
		}

		public int FindReplaceAll(string findText, string replaceText, bool matchWhole, bool matchCase)
		{
			var contentRange = ContentRange();
			int numChanges = 0;

			for (int row = contentRange.StartPos.Row; row <= contentRange.EndPos.Row; row++)
			{
				for (int col = contentRange.StartPos.Col; col <= contentRange.EndPos.Col; col++)
				{
					var cell = GridControl.CurrentWorksheet.GetCell(row, col);

					if (cell == null)
						continue;

					// Only handle native text elements
					var cellText = cell.GetData<String>();

					if (cellText == null)
						continue;

					try
					{
						String resultText = "";

						if (StringUtil.FindReplace(cellText, findText, replaceText, matchWhole, matchCase, ref resultText))
						{
							cell.Data = resultText;
							numChanges++;
						}
					}
					catch (Exception)
					{

					}
				}
			}

			if (numChanges > 0)
				NotifyParentContentChange();

			return numChanges;
		}

		public bool ProcessMessage(IntPtr hwnd, UInt32 message, UInt32 wParam, UInt32 lParam, UInt32 time, Int32 xPos, Int32 yPos)
		{
			const int WM_KEYDOWN = 0x0100;
			const int WM_SYSKEYDOWN = 0x0104;

			switch (message)
			{
			case WM_KEYDOWN:
			case WM_SYSKEYDOWN:
				if (CurrentWorksheet.IsEditing)
				{
					switch (wParam)
					{
					case (UInt32)Keys.Escape:
						CurrentWorksheet.EndEdit(EndEditReason.Cancel);
						return true;

					default:
						break;
					}
				}
				else
				{
					switch (wParam)
					{
					case (UInt32)Keys.F2:
						CurrentWorksheet.StartEdit();
						return true;

					default:
						break;
					}
				}
				// else
				return CommandHandling.ProcessMenuShortcut(wParam, MenuBar.Items);
			}

			return false;
		}

		public void SetContentFont(String fontName, int pointSize)
		{
			AllowParentChangeNotifications = false;

			Worksheet.DefaultFontName = fontName;
			Worksheet.DefaultFontSize = pointSize;

			if (CurrentWorksheet != null)
			{
				CurrentWorksheet.FontName = fontName;
				CurrentWorksheet.FontSize = pointSize;
			}

			this.FontBar.Items["fontToolStripComboBox"].Text = fontName;
			this.FontBar.Items["fontSizeToolStripComboBox"].Text = pointSize.ToString();

			AllowParentChangeNotifications = true;
		}

		private bool AllowParentChangeNotifications { get; set; }

		protected override void OnLoad(EventArgs e)
		{
			AllowParentChangeNotifications = false;

			base.OnLoad(e);

			AllowParentChangeNotifications = true;
		}

		private void InitialiseToolbars()
		{
            this.MenuBar.Renderer = m_toolbarRenderer;
            this.RowContextMenu.Renderer = m_toolbarRenderer;
            this.CellContextMenu.Renderer = m_toolbarRenderer;
            this.HeaderContextMenu.Renderer = m_toolbarRenderer;
            this.ColumnContextMenu.Renderer = m_toolbarRenderer;

            this.ToolBar.Renderer = m_toolbarRenderer;
			this.FontBar.Renderer = m_toolbarRenderer;
			this.StatusBar.Renderer = m_toolbarRenderer;

			Toolbars.FixupButtonSizes(this.ToolBar);
			Toolbars.FixupButtonSizes(this.FontBar);

            if (m_ControlsFont != null)
			{
				this.MenuBar.Font = m_ControlsFont;
				this.ToolBar.Font = m_ControlsFont;
				this.StatusBar.Font = m_ControlsFont;
				this.FontBar.Font = m_ControlsFont;

				this.FontBar.Items["fontToolStripComboBox"].Font = m_ControlsFont;
				this.FontBar.Items["fontSizeToolStripComboBox"].Font = m_ControlsFont;

				this.RowContextMenu.Font = m_ControlsFont;
				this.CellContextMenu.Font = m_ControlsFont;
				this.HeaderContextMenu.Font = m_ControlsFont;
				this.ColumnContextMenu.Font = m_ControlsFont;
			}

			if (DPIScaling.WantScaling())
			{
				int imageSize = DPIScaling.Scale(16);
				var scalingSize = new Size(imageSize, imageSize);

				this.MenuBar.ImageScalingSize = scalingSize;
				this.FontBar.ImageScalingSize = scalingSize;
				this.ToolBar.ImageScalingSize = scalingSize;
			
				this.RowContextMenu.ImageScalingSize = scalingSize;
				this.CellContextMenu.ImageScalingSize = scalingSize;
				this.HeaderContextMenu.ImageScalingSize = scalingSize;
				this.ColumnContextMenu.ImageScalingSize = scalingSize;
			}

			this.ToolBar.GripStyle = ToolStripGripStyle.Hidden;
			this.FontBar.GripStyle = ToolStripGripStyle.Hidden;
			this.StatusBar.SizingGrip = false;

            // After all resizing has occurred, set menu bar height to 
            // match application and make its top-level items full height
            this.MenuBar.AutoSize = false;
            this.MenuBar.Height = Win32.GetSystemMetric(15); // SM_CYMENU
			this.MenuBar.Padding = new Padding(0);

		}

		private void InitialiseFeatures()
		{
			// Hide unwanted menubar options
			CommandHandling.RemoveCommand("scriptToolStripMenuItem", this.MenuBar.Items);
			CommandHandling.RemoveCommand("toolsToolStripMenuItem", this.MenuBar.Items);

			CommandHandling.RemoveCommand("printPreviewToolStripMenuItem", this.MenuBar.Items);
			CommandHandling.RemoveCommand("printSettingsToolStripMenuItem", this.MenuBar.Items);
			CommandHandling.RemoveCommand("printToolStripMenuItem", this.MenuBar.Items);
			CommandHandling.RemoveCommand("exitToolStripMenuItem", this.MenuBar.Items);
			CommandHandling.RemoveCommand("componentsToolStripMenuItem", this.MenuBar.Items);
			CommandHandling.RemoveCommand("visibleToolStripMenuItem", this.MenuBar.Items);

			CommandHandling.RemoveCommand("newWindowToolStripMenuItem", this.MenuBar.Items);
			CommandHandling.RemoveCommand("exportAsHtmlToolStripMenuItem", this.MenuBar.Items);
			CommandHandling.RemoveCommand("editXMLToolStripMenuItem", this.MenuBar.Items);
			CommandHandling.RemoveCommand("exportAsCSVToolStripMenuItem", this.MenuBar.Items);
			CommandHandling.RemoveCommand("saveToolStripMenuItem", this.MenuBar.Items);

			CommandHandling.RemoveCommand("focusCellStyleToolStripMenuItem", this.MenuBar.Items);
			CommandHandling.RemoveCommand("selectionToolStripMenuItem", this.MenuBar.Items);

			// Remove keyboard shortcuts which conflict with the main app
			CommandHandling.SetMenuItemShortcut("mergeCellsToolStripMenuItem", this.MenuBar.Items, Keys.None);
			CommandHandling.SetMenuItemShortcut("unmergeCellsToolStripMenuItem", this.MenuBar.Items, Keys.None);
			CommandHandling.SetMenuItemShortcut("filterToolStripMenuItem", this.MenuBar.Items, Keys.None);

			// Hide unwanted toolbar options
			CommandHandling.RemoveCommand("printPreviewToolStripButton", this.ToolBar.Items);
			CommandHandling.RemoveCommand("saveToolStripButton", this.ToolBar.Items);
			CommandHandling.RemoveCommand("showDebugFormToolStripButton", this.ToolBar.Items);
			CommandHandling.RemoveCommand("slashLeftSolidToolStripButton", this.ToolBar.Items);
			CommandHandling.RemoveCommand("slashRightSolidToolStripButton", this.ToolBar.Items);

			CommandHandling.RemoveCommand("zoomToolStripDropDownButton", this.FontBar.Items);


		}

		public void SetUITheme(UITheme theme)
		{
			m_toolbarRenderer.SetUITheme(theme);

			var backColor = theme.GetAppDrawingColor(UITheme.AppColor.ToolbarLight);

			this.BackColor = backColor;
            MenuBar.BackColor = SystemColors.Menu;
            ToolBar.BackColor = backColor;
			FontBar.BackColor = backColor;
			StatusBar.BackColor = backColor;
			FormulaBar.BackColor = backColor;

			// Unfocused colours
			var color = theme.GetAppDrawingColor(UITheme.AppColor.ToolbarHot);
			var gridColor = new unvell.ReoGrid.Graphics.SolidColor(color.A, color.R, color.G, color.B);
			
			GridControl.ControlStyle.SetColor(ControlAppearanceColors.ColHeadSelectedNotFocusedStart, gridColor);
			GridControl.ControlStyle.SetColor(ControlAppearanceColors.ColHeadSelectedNotFocusedEnd, gridColor);
			GridControl.ControlStyle.SetColor(ControlAppearanceColors.ColHeadFullSelectedNotFocusedStart, gridColor);
			GridControl.ControlStyle.SetColor(ControlAppearanceColors.ColHeadFullSelectedNotFocusedEnd, gridColor);
			GridControl.ControlStyle.SetColor(ControlAppearanceColors.RowHeadSelectedNotFocused, gridColor);
			GridControl.ControlStyle.SetColor(ControlAppearanceColors.RowHeadFullSelectedNotFocused, gridColor);

			// Focused colours
			color = DrawingColor.AdjustLighting(color, -0.15f, false);
			gridColor = new unvell.ReoGrid.Graphics.SolidColor(color.A, color.R, color.G, color.B);

			GridControl.ControlStyle.SetColor(ControlAppearanceColors.ColHeadSelectedStart, gridColor);
			GridControl.ControlStyle.SetColor(ControlAppearanceColors.ColHeadSelectedEnd, gridColor);
			GridControl.ControlStyle.SetColor(ControlAppearanceColors.ColHeadFullSelectedStart, gridColor);
			GridControl.ControlStyle.SetColor(ControlAppearanceColors.ColHeadFullSelectedEnd, gridColor);
			GridControl.ControlStyle.SetColor(ControlAppearanceColors.RowHeadSelected, gridColor);
			GridControl.ControlStyle.SetColor(ControlAppearanceColors.RowHeadFullSelected, gridColor);
		}

		private void InitialiseChangeCallbacks()
		{
			GridControl.ActionPerformed += (s, e) =>
			{
				if (e.Action is unvell.Common.IUndoableAction)
					NotifyParentContentChange();
			};

			GridControl.EditTextChanged += (s, e) =>
			{
				NotifyParentContentChange();
			};

			GridControl.LostFocus += (s, e) =>
			{
				if (!GridControl.CurrentWorksheet.IsEditing)
					LostFocus?.Invoke(this, e);
			};

			GridControl.WorksheetCreated += (s, e) =>
			{
				NotifyParentContentChange();
			};

			GridControl.WorksheetInserted += (s, e) =>
			{
				NotifyParentContentChange();
			};

			GridControl.WorksheetRemoved += (s, e) =>
			{
				NotifyParentContentChange();
			};

			GridControl.WorksheetNameChanged += (s, e) =>
			{
				NotifyParentContentChange();
			};
		}

		protected void NotifyParentContentChange()
		{
			if (AllowParentChangeNotifications)
				ContentChanged?.Invoke(this, new EventArgs());
		}

		override public void Undo()
		{
			if (base.CanUndo())
			{
				base.Undo();
			}
			else if (m_PrevContent != null)
			{
				SetContent(m_PrevContent, true);
				m_PrevContent = null;
			}
		}

		override public bool CanUndo()
		{
			if (base.CanUndo())
				return true;

			return (m_PrevContent != null);
		}

		override public bool CloseDocument()
		{
			// Snapshot the current state because closing the document
			// also destroys the undo stack
			m_PrevContent = GetContent();

			return true; // handled
		}

		protected override DialogResult ShowDialog(Form dialog)
		{
			FormsUtil.SetFont(dialog, m_ControlsFont);

			// Add icon for identification
			dialog.Icon = SpreadsheetContentControl.Resource.Spreadsheet;
			dialog.ShowIcon = true;

			// Per dialog customisations
// 			if (ISTYPE(dialog, ))
// 			{
// 			}
// 			else // all others
			{
				m_Trans.Translate(dialog);
			}

			Win32.ActivateApp(Handle);

			return base.ShowDialog(dialog);
		}

		protected override void OnGotFocus(EventArgs e)
		{
			GridControl.Focus();
		}

	}
}
