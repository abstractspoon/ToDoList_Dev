#region Using directives

using System;
using System.Drawing;
using System.Globalization;

#endregion

namespace MSDN.Html.Editor
{

	/// <summary>
	/// Struct used to define a Html Table cell
	/// Html Defaults are based on FrontPage default table
	/// </summary>
	[Serializable]
	public class HtmlTableBaseProperty
	{
		// common properties defined for the table
		public Color BackColor, BorderColor;

		/// <summary>
		/// Constructor defining a base cell with default attributes
		/// </summary>
		protected HtmlTableBaseProperty()
		{
			BackColor = Color.Empty;
			BorderColor = Color.Empty;
		}

		public bool Set(object bgColor, object borderColor)
		{
			try
			{
				if (bgColor == null)
					this.BackColor = Color.Empty;
				else
					this.BackColor = ColorTranslator.FromHtml(bgColor.ToString());

				if (borderColor == null)
					this.BorderColor = Color.Empty;
				else
					this.BorderColor = ColorTranslator.FromHtml(borderColor.ToString());
			}
			catch (Exception ex)
			{
				// throw an exception indicating table structure change be determined
				throw new HtmlEditorException("Unable to determine Html Cell properties.", "HtmlTableBaseProperty.Set", ex);
			}

			return true;
		}

		public void Get(out object bgColor, out object borderColor)
		{
			try
			{
				if (this.BackColor == Color.Empty)
					bgColor = null;
				else
					bgColor = ColorTranslator.ToHtml(this.BackColor);

				if (this.BorderColor == Color.Empty)
					borderColor = null;
				else
					borderColor = ColorTranslator.ToHtml(this.BorderColor);
			}
			catch (Exception ex)
			{
				// throw an exception indicating table structure change be determined
				throw new HtmlEditorException("Unable to determine Html Cell properties.", "HtmlTableBaseProperty.Get", ex);
			}
		}
	} //HtmlCellProperty

	/// <summary>
	/// Struct used to define a Html Table
	/// Html Defaults are based on FrontPage default table
	/// </summary>
	[Serializable]
    public class HtmlTableProperty : HtmlTableBaseProperty
    {
        // properties defined for the table
        public string					CaptionText;
        public HorizontalAlignOption	CaptionAlignment;
        public VerticalAlignOption		CaptionLocation;
        public byte						BorderSize;
        public HorizontalAlignOption	TableAlignment;
        public byte						TableRows;
        public byte						TableColumns;
        public ushort					TableWidth;
        public MeasurementOption		TableWidthMeasurement;
        public byte						CellPadding;
        public byte						CellSpacing;
		
        /// <summary>
        /// Constructor defining a base table with default attributes
        /// </summary>
        public HtmlTableProperty(bool htmlDefaults) : base()
        {
            //define base values
            CaptionText = string.Empty;
            CaptionAlignment = HorizontalAlignOption.Default;
            CaptionLocation = VerticalAlignOption.Default;
            TableAlignment = HorizontalAlignOption.Default;

            // define values based on whether HTML defaults are required
            if (htmlDefaults)
            {
                BorderSize = 2;
                TableRows = 3;
                TableColumns = 3;
                TableWidth = 50;
                TableWidthMeasurement = MeasurementOption.Percent;
                CellPadding = 1;
                CellSpacing = 2;
            }
            else
            {
                BorderSize = 0;
                TableRows = 1;
                TableColumns = 1;
                TableWidth = 0;
                TableWidthMeasurement = MeasurementOption.Pixel;
                CellPadding = 0;
                CellSpacing = 0;
            }
        }

		public HtmlTableProperty(MSHTML.IHTMLTable table, bool htmlDefaults) : this(htmlDefaults)
		{
			Set(table);
		}

		public bool Set(MSHTML.IHTMLTable table)
		{
			if (table == null)
				return false;

			try
			{
				base.Set((object)table.bgColor, (object)table.borderColor);

				// have a table so extract the properties
				MSHTML.IHTMLTableCaption caption = table.caption;
				// if have a caption persist the values
				if (caption != null)
				{
					this.CaptionText = ((MSHTML.IHTMLElement)table.caption).innerText;

					if (caption.align != null)
						this.CaptionAlignment = (HorizontalAlignOption)Utils.TryParseEnum(typeof(HorizontalAlignOption), caption.align, HorizontalAlignOption.Default);

					if (caption.vAlign != null)
						this.CaptionLocation = (VerticalAlignOption)Utils.TryParseEnum(typeof(VerticalAlignOption), caption.vAlign, VerticalAlignOption.Default);
				}
				// look at the table properties
				if (table.border != null)
					this.BorderSize = Utils.TryParseByte(table.border.ToString(), this.BorderSize);

				if (table.align != null)
					this.TableAlignment = (HorizontalAlignOption)Utils.TryParseEnum(typeof(HorizontalAlignOption), table.align, HorizontalAlignOption.Default);

				// define the table rows and columns
				int rows = Math.Min(table.rows.length, Byte.MaxValue);
				int cols = Math.Min(table.cols, Byte.MaxValue);
				if (cols == 0 && rows > 0)
				{
					// cols value not set to get the maxiumn number of cells in the rows
					foreach (MSHTML.IHTMLTableRow tableRow in table.rows)
					{
						cols = Math.Max(cols, (int)tableRow.cells.length);
					}
				}
				this.TableRows = (byte)Math.Min(rows, byte.MaxValue);
				this.TableColumns = (byte)Math.Min(cols, byte.MaxValue);

				// define the remaining table properties
				if (table.cellPadding != null)
					this.CellPadding = Utils.TryParseByte(table.cellPadding.ToString(), this.CellPadding);

				if (table.cellSpacing != null)
					this.CellSpacing = Utils.TryParseByte(table.cellSpacing.ToString(), this.CellSpacing);

				if (table.width != null)
				{
					string tableWidth = table.width.ToString();

					if (tableWidth.TrimEnd(null).EndsWith("%"))
					{
						this.TableWidth = Utils.TryParseUshort(tableWidth.Remove(tableWidth.LastIndexOf("%"), 1), this.TableWidth);
						this.TableWidthMeasurement = MeasurementOption.Percent;
					}
					else
					{
						this.TableWidth = Utils.TryParseUshort(tableWidth, this.TableWidth);
						this.TableWidthMeasurement = MeasurementOption.Pixel;
					}
				}
				else
				{
					this.TableWidth = 0;
					this.TableWidthMeasurement = MeasurementOption.Pixel;
				}
			}
			catch (Exception ex)
			{
				// throw an exception indicating table structure change be determined
				throw new HtmlEditorException("Unable to determine Html Table properties.", "GetTableProperties", ex);
			}

			return true;
		}

		public bool Get(ref MSHTML.IHTMLTable table, bool tableCreated)
		{
			if (table == null)
				return false;

			// define the table border, width, cell padding and spacing
			object bgColor, borderColor;
			base.Get(out bgColor, out borderColor);

			table.bgColor = bgColor;
			table.borderColor = borderColor;

			if (this.TableWidth > 0)
				table.width = (this.TableWidthMeasurement == MeasurementOption.Pixel) ? string.Format("{0}", this.TableWidth) : string.Format("{0}%", this.TableWidth);
			else
				table.width = string.Empty;

			if (this.TableAlignment != HorizontalAlignOption.Default)
				table.align = this.TableAlignment.ToString().ToLower();
			else
				table.align = string.Empty;

			table.border = this.BorderSize;
			table.cellPadding = this.CellPadding.ToString();
			table.cellSpacing = this.CellSpacing.ToString();

			// define the given table caption and alignment
			string caption = this.CaptionText;
			MSHTML.IHTMLTableCaption tableCaption = table.caption;

			if (caption != null && caption != string.Empty)
			{
				// ensure table caption correctly defined
				if (tableCaption == null)
					tableCaption = table.createCaption();

				((MSHTML.IHTMLElement)tableCaption).innerText = caption;

				if (this.CaptionAlignment != HorizontalAlignOption.Default)
					tableCaption.align = this.CaptionAlignment.ToString().ToLower();

				if (this.CaptionLocation != VerticalAlignOption.Default)
					tableCaption.vAlign = this.CaptionLocation.ToString().ToLower();
			}
			else
			{
				// if no caption specified remove the existing one
				if (tableCaption != null)
				{
					// prior to deleting the caption the contents must be cleared
					((MSHTML.IHTMLElement)tableCaption).innerText = null;
					table.deleteCaption();
				}
			}

			// determine the number of rows one has to insert
			int numberRows, numberCols;
			if (tableCreated)
			{
				numberRows = Math.Max((int)this.TableRows, 1);
			}
			else
			{
				numberRows = Math.Max((int)this.TableRows, 1) - (int)table.rows.length;
			}

			// layout the table structure in terms of rows and columns
			table.cols = (int)this.TableColumns;
			if (tableCreated)
			{
				// this section is an optimization based on creating a new table
				// the section below works but not as efficiently
				numberCols = Math.Max((int)this.TableColumns, 1);
				// insert the appropriate number of rows
				MSHTML.IHTMLTableRow tableRow;
				for (int idxRow = 0; idxRow < numberRows; idxRow++)
				{
					tableRow = (MSHTML.IHTMLTableRow)table.insertRow(-1);
					// add the new columns to the end of each row
					for (int idxCol = 0; idxCol < numberCols; idxCol++)
					{
						tableRow.insertCell(-1);
					}
				}
			}
			else
			{
				// if the number of rows is increasing insert the decrepency
				if (numberRows > 0)
				{
					// insert the appropriate number of rows
					for (int idxRow = 0; idxRow < numberRows; idxRow++)
					{
						table.insertRow(-1);
					}
				}
				else
				{
					// remove the extra rows from the table
					for (int idxRow = numberRows; idxRow < 0; idxRow++)
					{
						table.deleteRow(table.rows.length - 1);
					}
				}
				// have the rows constructed
				// now ensure the columns are correctly defined for each row
				MSHTML.IHTMLElementCollection rows = table.rows;
				foreach (MSHTML.IHTMLTableRow tableRow in rows)
				{
					numberCols = Math.Max((int)this.TableColumns, 1) - (int)tableRow.cells.length;
					if (numberCols > 0)
					{
						// add the new column to the end of each row
						for (int idxCol = 0; idxCol < numberCols; idxCol++)
						{
							tableRow.insertCell(-1);
						}
					}
					else
					{
						// reduce the number of cells in the given row
						// remove the extra rows from the table
						for (int idxCol = numberCols; idxCol < 0; idxCol++)
						{
							tableRow.deleteCell(tableRow.cells.length - 1);
						}
					}
				}
			}

			return true;
		}

	} //HtmlTableProperty

    /// <summary>
    /// Struct used to define a Html Table row
    /// Html Defaults are based on FrontPage default table
    /// </summary>
    [Serializable]
    public class HtmlTableRowProperty : HtmlTableBaseProperty
    {
        // properties defined for the table
        public HorizontalAlignOption	HorzAlignment;
        public VerticalAlignOption		VertAlignment;

        /// <summary>
        /// Constructor defining a base row with default attributes
        /// </summary>
        public HtmlTableRowProperty() : base()
        {
            HorzAlignment = HorizontalAlignOption.Default;
            VertAlignment = VerticalAlignOption.Default;
        }

		public HtmlTableRowProperty(MSHTML.IHTMLTableRow row) : this()
		{
			Set(row);
		}

		public bool Set(MSHTML.IHTMLTableRow row)
		{
			// if user has selected a table extract those properties
			if (row == null)
				return false;

			try
			{
				base.Set((object)row.bgColor, (object)row.borderColor);

				if (row.align != null)
					this.HorzAlignment = (HorizontalAlignOption)Utils.TryParseEnum(typeof(HorizontalAlignOption), row.align, HorizontalAlignOption.Default);

				if (row.vAlign != null)
					this.VertAlignment = (VerticalAlignOption)Utils.TryParseEnum(typeof(VerticalAlignOption), row.vAlign, VerticalAlignOption.Default);

				if (row.bgColor != null)
					this.BackColor = ColorTranslator.FromHtml(row.bgColor.ToString());
			}
			catch (Exception ex)
			{
				// throw an exception indicating table structure change be determined
				throw new HtmlEditorException("Unable to determine Html Row properties.", "GetRowProperties", ex);
			}

			return true;
		}

		public bool Get(ref MSHTML.IHTMLTableRow row)
		{
			if (row == null)
				return false;

			object bgColor, borderColor;
			base.Get(out bgColor, out borderColor);

			row.bgColor = bgColor;
			row.borderColor = borderColor;

			if (this.HorzAlignment == HorizontalAlignOption.Default)
				row.align = null;
			else
				row.align = this.HorzAlignment.ToString().ToLower();

			if (this.VertAlignment == VerticalAlignOption.Default)
				row.vAlign = null;
			else
				row.vAlign = this.VertAlignment.ToString().ToLower();

			return true;
		}

	} //HtmlRowProperty

	/// <summary>
	/// Struct used to define a Html Table cell
	/// Html Defaults are based on FrontPage default table
	/// </summary>
	[Serializable]
    public class HtmlTableCellProperty : HtmlTableBaseProperty
    {
        // properties defined for the table
        public HorizontalAlignOption	HorzAlignment;
        public VerticalAlignOption		VertAlignment;
		public int						ColSpan, RowSpan;
		public bool						NoWrap;

		/// <summary>
		/// Constructor defining a base cell with default attributes
		/// </summary>
		public HtmlTableCellProperty() : base()
		{
			HorzAlignment = HorizontalAlignOption.Default;
			VertAlignment = VerticalAlignOption.Default;
			ColSpan = 1;
			RowSpan = 1;
			NoWrap = false;
		}

		public HtmlTableCellProperty(MSHTML.IHTMLTableCell cell) : this()
		{
			Set(cell);
		}

		public bool Set(MSHTML.IHTMLTableCell cell)
		{
			// if user has selected a table extract those properties
			if (cell == null)
				return false;

			try
			{
				base.Set((object)cell.bgColor, (object)cell.borderColor);

				if (cell.align != null)
					this.HorzAlignment = (HorizontalAlignOption)Utils.TryParseEnum(typeof(HorizontalAlignOption), cell.align, HorizontalAlignOption.Default);

				if (cell.vAlign != null)
					this.VertAlignment = (VerticalAlignOption)Utils.TryParseEnum(typeof(VerticalAlignOption), cell.vAlign, VerticalAlignOption.Default);

				this.ColSpan = cell.colSpan;
				this.RowSpan = cell.rowSpan;
				this.NoWrap = cell.noWrap;
			}
			catch (Exception ex)
			{
				// throw an exception indicating table structure change be determined
				throw new HtmlEditorException("Unable to determine Html Cell properties.", "GetCellProperties", ex);
			}

			return true;
		}

		public bool Get(ref MSHTML.IHTMLTableCell cell)
		{
			if (cell == null)
				return false;

			object bgColor, borderColor;
			base.Get(out bgColor, out borderColor);

			cell.bgColor = bgColor;
			cell.borderColor = borderColor;

			if (this.HorzAlignment == HorizontalAlignOption.Default)
				cell.align = null;
			else
				cell.align = this.HorzAlignment.ToString().ToLower();

			if (this.VertAlignment == VerticalAlignOption.Default)
				cell.vAlign = null;
			else
				cell.vAlign = this.VertAlignment.ToString().ToLower();

			cell.colSpan = this.ColSpan;
			cell.rowSpan = this.RowSpan;
			cell.noWrap = this.NoWrap;

			return true;
		}
	} //HtmlCellProperty

}