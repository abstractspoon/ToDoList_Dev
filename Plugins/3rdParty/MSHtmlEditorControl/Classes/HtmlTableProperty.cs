#region Using directives

using System;
using System.Drawing;

#endregion

namespace MSDN.Html.Editor
{

    /// <summary>
    /// Struct used to define a Html Table
    /// Html Defaults are based on FrontPage default table
    /// </summary>
    [Serializable]
    public class HtmlTableProperty
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
        public HtmlTableProperty(bool htmlDefaults)
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

    } //HtmlTableProperty
    
    /// <summary>
    /// Struct used to define a Html Table row
    /// Html Defaults are based on FrontPage default table
    /// </summary>
    [Serializable]
    public class HtmlRowProperty
    {
        // properties defined for the table
        public HorizontalAlignOption	HorzAlignment;
        public VerticalAlignOption		VertAlignment;
        public Color					BackColor;

        /// <summary>
        /// Constructor defining a base row with default attributes
        /// </summary>
        public HtmlRowProperty()
        {
            HorzAlignment = HorizontalAlignOption.Default;
            VertAlignment = VerticalAlignOption.Default;
			BackColor = Color.Empty;
        }

    } //HtmlRowProperty
    
    /// <summary>
    /// Struct used to define a Html Table cell
    /// Html Defaults are based on FrontPage default table
    /// </summary>
    [Serializable]
    public class HtmlCellProperty
    {
        // properties defined for the table
        public HorizontalAlignOption	HorzAlignment;
        public VerticalAlignOption		VertAlignment;
        public Color					BackColor, BorderColor;
		public int						ColSpan, RowSpan;
		public bool						NoWrap;

		/// <summary>
		/// Constructor defining a base cell with default attributes
		/// </summary>
		public HtmlCellProperty()
		{
			HorzAlignment = HorizontalAlignOption.Default;
			VertAlignment = VerticalAlignOption.Default;
			BackColor = Color.Empty;
			BorderColor = Color.Empty;
			ColSpan = 1;
			RowSpan = 1;
			NoWrap = false;
		}
	} //HtmlCellProperty

}