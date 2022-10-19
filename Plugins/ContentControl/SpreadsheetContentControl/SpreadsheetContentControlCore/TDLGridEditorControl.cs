using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Windows.Forms;
using System.Windows.Resources;
using System.IO;
using System.Net;
using System.Threading;
using System.Diagnostics;

using unvell.ReoGrid;
using unvell.ReoGrid.Events;
using unvell.ReoGrid.Editor;
using unvell.ReoGrid.CellTypes;
using unvell.ReoGrid.DataFormat;

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
		public new event EventHandler LostFocus; // replaces base class event

		public event NeedLinkTooltipEventHandler NeedLinkTooltip;
		public event NeedAttributeValuesEventHandler NeedAttributeValues;

		// --------------------------------------------

		public event EventHandler<LinkEventArgs> LinkNavigation;

		public class LinkEventArgs : EventArgs
		{
			public LinkEventArgs(string link)
			{
				LinkUrl = link;
				Handled = false;
			}

			public string LinkUrl;
			public bool Handled;
		}

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

			GridControl.WorksheetInserted += (s, e) =>
			{
				e.Worksheet.AfterPaste += new EventHandler<RangeEventArgs>(OnAfterPaste);
				e.Worksheet.AfterCellEdit += new EventHandler<CellAfterEditEventArgs>(OnAfterCellEdit);
				e.Worksheet.CellBodyChanged += new EventHandler<CellEventArgs>(OnCellBodyChanged);
				e.Worksheet.CellDataChanged += new EventHandler<CellEventArgs>(OnCellDataChanged);
				e.Worksheet.CellMouseEnter += new EventHandler<CellMouseEventArgs>(OnCellMouseEnter);
				e.Worksheet.CellMouseLeave += new EventHandler<CellMouseEventArgs>(OnCellMouseLeave);
				e.Worksheet.CellMouseMove += new EventHandler<CellMouseEventArgs>(OnCellMouseMove);
			};

			GridControl.WorksheetRemoved += (s, e) =>
			{
				e.Worksheet.AfterPaste -= new EventHandler<RangeEventArgs>(OnAfterPaste);
				e.Worksheet.AfterCellEdit -= new EventHandler<CellAfterEditEventArgs>(OnAfterCellEdit);
				e.Worksheet.CellBodyChanged -= new EventHandler<CellEventArgs>(OnCellBodyChanged);
				e.Worksheet.CellDataChanged -= new EventHandler<CellEventArgs>(OnCellDataChanged);
				e.Worksheet.CellMouseEnter -= new EventHandler<CellMouseEventArgs>(OnCellMouseEnter);
				e.Worksheet.CellMouseLeave -= new EventHandler<CellMouseEventArgs>(OnCellMouseLeave);
				e.Worksheet.CellMouseMove -= new EventHandler<CellMouseEventArgs>(OnCellMouseMove);
			};

			AllowDrop = true;
		}

		public Byte[] GetContent()
		{
			try
			{
				using (var memStream = new MemoryStream())
				{
					GridControl.Save(memStream, unvell.ReoGrid.IO.FileFormat.ReoGridFormat);

					return Compression.Compress(memStream.GetBuffer());
				}
			}
			catch (Exception)
			{
			}

			return new Byte[1];
		}

		public bool SetContent(Byte[] content, bool resetSelection)
		{
			GridControl.Reset();

			using (var memStream = new MemoryStream(Compression.Decompress(content)))
			{
				try
				{
					GridControl.Load(memStream, unvell.ReoGrid.IO.FileFormat.ReoGridFormat);
 				}
 				catch (Exception)
 				{
					try
					{
						// Backwards compatibility
						memStream.Position = 0;

 						GridControl.Load(memStream, unvell.ReoGrid.IO.FileFormat.Excel2007);
					}
					catch (Exception)
					{
						return false;
					}
				}
			}

			if (CurrentWorksheet == null)
				GridControl.CurrentWorksheet = GridControl.Worksheets[0];

			// Restore the content font because loading changes it
			CurrentWorksheet.FontName = Worksheet.DefaultFontName;
			CurrentWorksheet.FontSize = Worksheet.DefaultFontSize;

			// Initialise cell callbacks and attributes
			for (int row = 0; row <= CurrentWorksheet.MaxContentRow; row++)
			{
				for (int col = 0; col <= CurrentWorksheet.MaxContentCol; col++)
				{
					var cell = CurrentWorksheet.GetCell(row, col);

					if (HasCellBody<HyperlinkCell>(cell))
					{
						var link = CellBody<HyperlinkCell>(cell);

						link.AutoNavigate = false;
						link.Click += new EventHandler<HyperlinkCell.ClickEventArgs>(OnClickHyperlinkCell);
					}
					else if (HasCellBody<DropdownCell>(cell))
					{
						var drop = CellBody<DropdownCell>(cell);

						drop.PullDownOnClick = false;
					}
				}
			}

			m_PrevContent = null;

			return true;
		}

		private RangePosition ContentRange()
		{
			var worksheet = GridControl.CurrentWorksheet;

			return new RangePosition(0, 0, worksheet.MaxContentRow + 1, worksheet.MaxContentCol + 1);
		}

		public String GetTextContent()
		{
			var text = GridControl.CurrentWorksheet.StringifyRange(ContentRange()).Trim();

			return text;
		}

		public bool SetTextContent(String content, bool resetSelection)
		{
			// Clear and then Insert content into 'A:1'
			GridControl.CurrentWorksheet.Reset();
			GridControl.CurrentWorksheet.PasteFromString(new CellPosition(0, 0), content);

			m_PrevContent = null;

			return true;
		}

		private CellDataFormatFlag GetCellFormat(Cell cell, out string dateFormatStr)
		{
			var formatType = ((cell == null) ? CellDataFormatFlag.General : cell.DataFormat);
			dateFormatStr = string.Empty;

			if ((formatType == CellDataFormatFlag.DateTime) && (cell.DataFormatArgs != null))
			{
				var dateFormat = (DateTimeDataFormatter.DateTimeFormatArgs)cell.DataFormatArgs;
				dateFormatStr = dateFormat.Format;
			}

			return formatType;
		}

		bool UpdateCellFormat(Cell cell, DateTime date, CellDataFormatFlag prevFormatType, string prevFormatStr)
		{
			// Post-processing of dates/times to pragmatically match Excel's rules
			switch (prevFormatType)
			{
			case CellDataFormatFlag.General:
				{
					// Should have already been handled by our base class
					Debug.Assert(cell.DataFormat == CellDataFormatFlag.DateTime);
				}
				break;

			case CellDataFormatFlag.Percent:
			case CellDataFormatFlag.Currency:
				{
					Debug.Assert(cell.DataFormat == prevFormatType);

					//   Change the format type to 'DateTime' and set the format
					//   string to match the content
					cell.DataFormat = CellDataFormatFlag.DateTime;
					cell.DataFormatArgs = new DateTimeDataFormatter.DateTimeFormatArgs
					{
						CultureName = Thread.CurrentThread.CurrentCulture.Name,
						Format = GetDateFormat(date, ""),
					};
				}
				return true;

			case CellDataFormatFlag.Number:
			case CellDataFormatFlag.Text:
			case CellDataFormatFlag.Custom:
				{
					Debug.Assert(cell.DataFormat == prevFormatType);

					// Don't change the format
				}
				break;

			case CellDataFormatFlag.DateTime:
				{
					Debug.Assert(cell.DataFormat == CellDataFormatFlag.DateTime);

					cell.DataFormatArgs = new DateTimeDataFormatter.DateTimeFormatArgs
					{
						CultureName = Thread.CurrentThread.CurrentCulture.Name,
						Format = GetDateFormat(date, prevFormatStr),
					};
				}
				return true;
			}

			return false;
		}

		public bool InsertTextContent(String content, bool bAtEnd)
		{
			var selection = GridControl.CurrentWorksheet.SelectionRange;

			if (selection == null)
				return false;

			// Nothing more to do if cell editing because any further
			// processing only happens when the edit finishes
			if (GridControl.CurrentWorksheet.IsEditing)
				return GridControl.CurrentWorksheet.Paste(content);

			if (FormulaBar.ContainsFocus)
				return FormulaBar.EditControlPaste(content);

			// else insert into spreadsheet

			// Cache current cell properties to help with post-processing of dates/times
			DateTime contentDate;
			bool contentIsDateTime = DateTime.TryParse(content, null, System.Globalization.DateTimeStyles.NoCurrentDateDefault, out contentDate);

			var cell = GridControl.CurrentWorksheet.GetCell(selection.StartPos);

			string prevFormatStr;
			var prevFormatType = GetCellFormat(cell, out prevFormatStr);

			if (GridControl.CurrentWorksheet.Paste(content))
			{
				if (contentIsDateTime)
				{
					cell = GridControl.CurrentWorksheet.GetCell(selection.StartPos);

					if (UpdateCellFormat(cell, contentDate, prevFormatType, prevFormatStr))
					{
						cell.Data = contentDate;
						cell.Formula = null;
					}
				}

				FormulaBar.RefreshCurrentFormula();
			}

			NotifyParentContentChange();

			return true;
		}

		private string GetDateFormat(DateTime date, string prevFormat)
		{
			bool dateHasDate = (date.Date != DateTime.MinValue);
			bool dateHasTime = (date.TimeOfDay.TotalDays > 0);

			string dateFormat, timeFormat;
			ExtractFormatComponents(prevFormat, out dateFormat, out timeFormat);
			
			bool formatHasDate = !string.IsNullOrEmpty(dateFormat);
			bool formatHasTime = !string.IsNullOrEmpty(timeFormat);

			if ((dateHasDate == formatHasDate) && (dateHasTime == formatHasTime))
			{
				return prevFormat;
			}

			// Else if one or other of the required components is missing 
			// we rebuild the format preserving as much as we can
			var	newFormat = string.Empty;

			if (dateHasDate)
			{
				if (!formatHasDate)
					dateFormat = Thread.CurrentThread.CurrentCulture.DateTimeFormat.ShortDatePattern;

				newFormat = dateFormat;
			}

			if (dateHasTime)
			{
				if (!formatHasTime)
					timeFormat = Thread.CurrentThread.CurrentCulture.DateTimeFormat.ShortTimePattern;

				if (dateHasDate)
					newFormat = string.Concat(dateFormat, " ", timeFormat);
				else
					newFormat = timeFormat;
			}

			return newFormat;
		}

		void ExtractFormatComponents(string format, out string dateFormat, out string timeFormat)
		{
			dateFormat = string.Empty;
			timeFormat = string.Empty;

			int lastDateIndex = format.LastIndexOfAny(new char[] { 'D', 'd', 'M', 'Y', 'y' });
			int lastTimeIndex = format.LastIndexOfAny(new char[] { 'H', 'h', 'm' });

			if ((lastDateIndex != -1) && (lastTimeIndex != -1))
			{
				if (lastTimeIndex > lastDateIndex) // date comes first
				{
					dateFormat = format.Substring(0, lastDateIndex + 1);
					timeFormat = format.Substring(lastDateIndex + 1);
				}
				else // time comes first
				{
					timeFormat = format.Substring(0, lastDateIndex + 1);
					dateFormat = format.Substring(lastDateIndex + 1);
				}
			}
			else if (lastDateIndex == -1)
			{
				timeFormat = format;
			}
			else if (lastTimeIndex == -1)
			{
				dateFormat = format;
			}
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

					case (UInt32)Keys.Decimal:
						{
							string decSep = System.Threading.Thread.CurrentThread.CurrentCulture.NumberFormat.NumberDecimalSeparator;

							if (decSep != ".")
							{
								SendKeys.SendWait(decSep);
								return true;
							}
						}
						break;

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
			this.MenuBar.Height = SystemInformation.MenuHeight;
			this.MenuBar.Padding = new Padding(0);
		}

		private void InitialiseFeatures()
		{
			// Add Import/Export commands to File menu
			var fileMenu = (this.MenuBar.Items[0] as ToolStripMenuItem);

			if (fileMenu != null)
			{
				int insertAt = fileMenu.DropDownItems.IndexOfKey("openToolStripMenuItem");

				fileMenu.DropDownItems.Insert(insertAt, new ToolStripSeparator());
				
				// Import
				var menuItem = new ToolStripMenuItem();

				menuItem.Name = "importToolStripMenuItem";
				menuItem.Text = "&Import...";
				menuItem.Click += (s, e) => OpenDocument();

				fileMenu.DropDownItems.Insert(++insertAt, menuItem);

				// Export
				menuItem = new ToolStripMenuItem();

				menuItem.Name = "exportToolStripMenuItem";
				menuItem.Text = "&Export...";
				menuItem.Click += (s, e) => SaveAsDocument();

				fileMenu.DropDownItems.Insert(++insertAt, menuItem);
			}

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

			CommandHandling.RemoveCommand("openToolStripMenuItem", this.MenuBar.Items);
			CommandHandling.RemoveCommand("saveToolStripMenuItem", this.MenuBar.Items);
			CommandHandling.RemoveCommand("saveAsToolStripMenuItem", this.MenuBar.Items);

			CommandHandling.RemoveCommand("focusCellStyleToolStripMenuItem", this.MenuBar.Items);
			CommandHandling.RemoveCommand("selectionToolStripMenuItem", this.MenuBar.Items);
			
			// Remove keyboard shortcuts which conflict with the main app
			CommandHandling.SetMenuItemShortcut("mergeCellsToolStripMenuItem", this.MenuBar.Items, Keys.None);
			CommandHandling.SetMenuItemShortcut("unmergeCellsToolStripMenuItem", this.MenuBar.Items, Keys.None);
			CommandHandling.SetMenuItemShortcut("filterToolStripMenuItem", this.MenuBar.Items, Keys.None);

			// Hide unwanted toolbar options
			CommandHandling.RemoveCommand("printPreviewToolStripButton", this.ToolBar.Items);
			CommandHandling.RemoveCommand("loadToolStripButton", this.ToolBar.Items);
			CommandHandling.RemoveCommand("newToolStripButton", this.ToolBar.Items);
			CommandHandling.RemoveCommand("saveToolStripButton", this.ToolBar.Items);
			CommandHandling.RemoveCommand("showDebugFormToolStripButton", this.ToolBar.Items);
			CommandHandling.RemoveCommand("slashLeftSolidToolStripButton", this.ToolBar.Items);
			CommandHandling.RemoveCommand("slashRightSolidToolStripButton", this.ToolBar.Items);

			CommandHandling.RemoveCommand("zoomToolStripDropDownButton", this.FontBar.Items);

		}

		private void OnAfterPaste(object sender, RangeEventArgs e)
		{
			for (int row = e.Range.Row; row <= e.Range.EndRow; row++)
			{
				for (int col = e.Range.Col; col <= e.Range.EndCol; col++)
				{
					string newData = GridControl.CurrentWorksheet.GetCellText(row, col);

					HandleCellTextUpdate(row, col, newData);
				}
			}
		}

		private bool IsValidHref(ref string href)
		{
			var parser = new UrlParser();

			if (parser.GetUrlCount(href) == 1)
			{
				if (Uri.IsWellFormedUriString(href, UriKind.Absolute))
					return true;

				// try appending https://
				string fixedHref = ("https://" + href);

				if (Uri.IsWellFormedUriString(fixedHref, UriKind.Absolute))
				{
					href = fixedHref;
					return true;
				}
			}

			return false;
		}

		private void HandleCellTextUpdate(int row, int col, string newData)
		{
			newData = newData.Trim();

			// Start with URL
			if (IsValidHref(ref newData))
			{
				Image image = null;

/*
				using (var client = new WebClient())
				{
					using (var stream = client.OpenRead(newData))
					{
						try
						{
							image = new Bitmap(stream);
							GridControl.CurrentWorksheet.SetCellBody(row, col, new ImageCell(image, ImageCellViewMode.Clip));
						}
						catch (Exception / *e* /)
						{
							image = null;
						}
					}
				}
*/

				if (image == null)
				{
					GridControl.CurrentWorksheet.SetCellBody(row, col, new HyperlinkCell(newData, (LinkNavigation == null)));
				}

				return;
			}

			// Then try file path
			try
			{
				if (File.Exists(newData))
				{
					Image image = null;

/*
					try
					{
						image = new Bitmap(newData);
						GridControl.CurrentWorksheet.SetCellBody(row, col, new ImageCell(image, ImageCellViewMode.Clip));
					}
					catch (Exception / *e* /)
					{
						image = null;
					}
*/

					if (image == null)
					{
						var uri = new Uri(newData);
						GridControl.CurrentWorksheet.SetCellBody(row, col, new HyperlinkCell(uri.AbsoluteUri));
					}

					return;
				}
			}
			catch (Exception /*e*/)
			{
			}

			// Then other 'things'
			// TODO
		}

		private void OnAfterCellEdit(object sender, CellAfterEditEventArgs e)
		{
			if (e.EndReason == EndEditReason.NormalFinish)
			{
				string newData = (e.NewData as string);
				string oldData = (e.Cell.Data as string);

				if (!string.IsNullOrEmpty(newData) && !newData.Equals(oldData))
				{
					HandleCellTextUpdate(e.Cell.Row, e.Cell.Column, newData);
				}
			}
		}

		T CellBody<T>(CellMouseEventArgs e) where T : CellBody
		{
			if (e.Cell == null)
				e.Cell = CurrentWorksheet.GetCell(e.CellPosition);

			return CellBody<T>(e.Cell);
		}

		T CellBody<T>(CellEventArgs e) where T : CellBody
		{
			if (e.Cell == null)
				return null;

			return CellBody<T>(e.Cell);
		}

		static T CellBody<T>(Cell cell) where T : CellBody
		{
			if (cell == null)
				return null;

			return (cell.Body as T);
		}

		bool HasCellBody<T>(CellMouseEventArgs e) where T : CellBody
		{
			if (e.Cell == null)
				e.Cell = CurrentWorksheet.GetCell(e.CellPosition);

			return HasCellBody<T>(e.Cell);
		}

		static bool HasCellBody<T>(CellEventArgs e) where T : CellBody
		{
			return HasCellBody<T>(e.Cell);
		}

		static bool HasCellBody<T>(Cell cell) where T : CellBody
		{
			if (cell == null)
				return false;

			return (cell.Body is T);
		}

		private void OnCellMouseEnter(object sender, CellMouseEventArgs e)
		{
			var link = CellBody<HyperlinkCell>(e);

			if ((link != null) && (link.HasLinkURL))
			{
				string tooltip = link.LinkURL;

				if (NeedLinkTooltip != null)
				{
					var args = new NeedLinkTooltipEventArgs(link.LinkURL);
					NeedLinkTooltip(this, args);

					if (!String.IsNullOrWhiteSpace(args.tooltip))
						tooltip = args.tooltip;
				}

				if (tooltip.Contains(link.LinkURL))
					tooltip = String.Empty;
				else
					tooltip = tooltip + "\n";

				tooltip = tooltip + m_Trans.Translate("'CTRL + click' to follow link");

				GridControl.ShowTooltip(tooltip);
			}
		}

		private void OnCellMouseLeave(object sender, CellMouseEventArgs e)
		{
			var link = CellBody<HyperlinkCell>(e);

			if (link != null)
			{
				GridControl.HideTooltip();
				GridControl.ChangeSelectionCursor(unvell.ReoGrid.Interaction.CursorStyle.PlatformDefault);
			}
		}

		private void OnCellMouseMove(object sender, CellMouseEventArgs e)
		{
			if (HasCellBody<HyperlinkCell>(e))
			{
				var link = CellBody<HyperlinkCell>(e);
				var cursor = unvell.ReoGrid.Interaction.CursorStyle.PlatformDefault;

				if (link.HasLinkURL && (Control.ModifierKeys == Keys.Control))
					cursor = unvell.ReoGrid.Interaction.CursorStyle.Hand;

				GridControl.ChangeSelectionCursor(cursor);
			}
		}

		private void OnCellBodyChanged(object sender, CellEventArgs e)
		{
			// Initialise cell callbacks and attributes
			if (HasCellBody<HyperlinkCell>(e))
			{
				var link = CellBody<HyperlinkCell>(e.Cell);

				link.AutoNavigate = false;
				link.Click += new EventHandler<HyperlinkCell.ClickEventArgs>(OnClickHyperlinkCell);
			}
			else if (HasCellBody<DropdownCell>(e.Cell))
			{
				var drop = CellBody<DropdownCell>(e.Cell);

				drop.PullDownOnClick = false;
			}

			NotifyParentContentChange();
		}

		private void OnCellDataChanged(object sender, CellEventArgs e)
		{
			NotifyParentContentChange();
		}

		private void OnClickHyperlinkCell(object sender, HyperlinkCell.ClickEventArgs e)
		{
			if ((LinkNavigation != null) &&
				(!e.ByMouse || (Control.ModifierKeys == Keys.Control)))
			{
				var args = new LinkEventArgs(e.LinkURL);
				LinkNavigation(this, args);

				if (args.Handled)
				{
					var link = (sender as HyperlinkCell);

					if (link != null)
						link.Cell.Style.TextColor = link.VisitedColor;
				}
			}
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

			GridControl.EditTextLostFocus += (s, e) =>
			{
				// Only handle this if out edit cell is no longer editing
				if (!GridControl.CurrentWorksheet.IsEditing)
					LostFocus?.Invoke(this, e);
			};

			GridControl.LostFocus += (s, e) =>
			{
				// Handle only if it WASN'T our edit cell gaining the focus
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

		protected override void OnDragDrop(DragEventArgs drgevent)
		{
			if (CanDrop(drgevent))
			{
				string text = string.Empty;

				if (drgevent.Data.GetDataPresent("FileNameW"))
				{
					text = (drgevent.Data.GetData("FileNameW") as string[])[0];
				}
				else if (drgevent.Data.GetDataPresent("UnicodeText"))
				{
					text = drgevent.Data.GetData("UnicodeText").ToString();
				}

				if (!string.IsNullOrEmpty(text))
				{
					CurrentWorksheet.PasteFromString(CurrentWorksheet.HoverPos, text);
					NotifyParentContentChange();
				}
			}
			CurrentWorksheet.HotTracking = false;

			base.OnDragDrop(drgevent);
		}

		protected override void OnDragLeave(EventArgs e)
		{
			CurrentWorksheet.HotTracking = false;

			base.OnDragLeave(e);
		}

		protected override void OnDragOver(DragEventArgs drgevent)
		{
			bool canDrop = ((Control.MouseButtons == MouseButtons.Left) && CanDrop(drgevent));
			drgevent.Effect = canDrop ? DragDropEffects.Copy : DragDropEffects.None;

			base.OnDragOver(drgevent);
		}

		private bool CanDrop(DragEventArgs drgevent)
		{
			bool hotTracking = false;
			bool canDrop = false;

			string[] formats = drgevent.Data.GetFormats();

			if (formats.Contains("FileNameW") || formats.Contains("UnicodeText"))
			{
				var pos = GridControl.PointToClient(new Point(drgevent.X, drgevent.Y));

				if (CurrentWorksheet.SetHoverPos(pos))
				{
					hotTracking = true;

					var cell = CurrentWorksheet.GetCell(CurrentWorksheet.HoverPos);
					var cellType = Cell.GetBodyType(cell);

					if ((cellType == "") || (cellType == typeof(HyperlinkCell).ToString()))
					{
						canDrop = true;
					}
				}
			}

			CurrentWorksheet.HotTracking = hotTracking;

			return canDrop;
		}
	}
}
