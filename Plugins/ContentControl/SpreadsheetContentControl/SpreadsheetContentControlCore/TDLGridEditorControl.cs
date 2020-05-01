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

		// --------------------------------------------

		public event EventHandler ContentChanged;

		// --------------------------------------------

		public TDLGridEditorControl(Font font, Translator trans)
		{
			m_toolbarRenderer = new UIThemeToolbarRenderer();
			m_ControlsFont = font;
			m_Trans = trans;

			InitialiseFeatures();
			InitialiseToolbars();
			InitialiseChangeCallbacks();

			//System.Windows.Forms.ToolStripProfessionalRenderer var;


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

		public void SetContentFont(String fontName, int pointSize)
		{
			//SetBodyFont(fontName, pointSize);
		}

		private bool IsLoading { get; set; }

		protected override void OnLoad(EventArgs e)
		{
			IsLoading = true;

			base.OnLoad(e);

			IsLoading = false;
		}

		private void InitialiseToolbars()
		{
            // Note: menus keep default rendering for consistency with app
			this.ToolBar.Renderer = m_toolbarRenderer;
			this.FontBar.Renderer = m_toolbarRenderer;
			this.StatusBar.Renderer = m_toolbarRenderer;

			Toolbars.FixupButtonSizes(this.ToolBar);
			Toolbars.FixupButtonSizes(this.FontBar);

            if (m_ControlsFont != null)
			{
				this.MenuBar.Font = m_ControlsFont;
				this.ToolBar.Font = m_ControlsFont;
				this.FontBar.Font = m_ControlsFont;
				this.StatusBar.Font = m_ControlsFont;

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
				this.ToolBar.ImageScalingSize = scalingSize;
				this.FontBar.ImageScalingSize = scalingSize;

				this.RowContextMenu.ImageScalingSize = scalingSize;
				this.CellContextMenu.ImageScalingSize = scalingSize;
				this.HeaderContextMenu.ImageScalingSize = scalingSize;
				this.ColumnContextMenu.ImageScalingSize = scalingSize;
			}

			this.ToolBar.GripStyle = ToolStripGripStyle.Hidden;
			this.FontBar.GripStyle = ToolStripGripStyle.Hidden;
			this.StatusBar.SizingGrip = false;

            // After all resizing has occurred, set menu bar height to 
            // match toolbar and make its top-level items full height
            this.MenuBar.AutoSize = false;
            this.MenuBar.Height = this.ToolBar.Height;
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
			//CommandHandling.HideCommand("", this.MenuBar.Items);

			// Hide unwanted toolbar options
			CommandHandling.RemoveCommand("printPreviewToolStripButton", this.ToolBar.Items);
			//CommandHandling.HideCommand("", this.ToolBar.Items);
		}

		public void SetUITheme(UITheme theme)
		{
			m_toolbarRenderer.SetUITheme(theme);

			var backColor = theme.GetAppDrawingColor(UITheme.AppColor.ToolbarLight);

            MenuBar.BackColor = SystemColors.Menu;//BackColor;
            ToolBar.BackColor = backColor;
			FontBar.BackColor = backColor;
			StatusBar.BackColor = backColor;
			FormulaBar.BackColor = backColor;
		}

		private void InitialiseChangeCallbacks()
		{
			GridCtrl.ActionPerformed += (s, e) =>
			{
				NotifyParentContentChange();
			};

			GridCtrl.WorksheetCreated += (s, e) =>
			{
				NotifyParentContentChange();
			};

			GridCtrl.WorksheetInserted += (s, e) =>
			{
				NotifyParentContentChange();
			};

			GridCtrl.WorksheetRemoved += (s, e) =>
			{
				NotifyParentContentChange();
			};

			GridCtrl.WorksheetNameChanged += (s, e) =>
			{
				NotifyParentContentChange();
			};
		}

		protected void NotifyParentContentChange()
		{
			if (/*!m_SettingContent &&*/ !IsLoading && (ContentChanged != null))
			{
				ContentChanged(this, new EventArgs());
			}
		}
	}
}
