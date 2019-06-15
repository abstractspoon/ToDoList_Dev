#region Using directives

using System;

#endregion

namespace MSDN.Html.Editor
{

    /// <summary>
    /// Enum used to insert a list
    /// </summary>
    public enum HtmlListType
    {
        Ordered,
        Unordered

    } //HtmlListType


    /// <summary>
    /// Enum used to insert a heading
    /// </summary>
    public enum HtmlHeadingType
    {
        H1 = 1,
        H2 = 2,
        H3 = 3,
        H4 = 4,
        H5 = 5

    } //HtmlHeadingType


    /// <summary>
    /// Enum used to define the navigate action on a user clicking a href
    /// </summary>
    public enum NavigateActionOption
    {
        Default,
        NewWindow,
        SameWindow

    } //NavigateActionOption


    /// <summary>
    /// Enum used to define the image align property
    /// </summary>
    public enum ImageAlignOption
    {
        Default,
        Bottom,
        Left,
        Middle,
        Right,
        Top

    } //ImageAlignOption


    /// <summary>
    /// Enum used to define the text alignment property
    /// </summary>
    public enum HorizontalAlignOption
    {
        Default,
        Left,
        Center,
        Right

    } //HorizontalAlignOption


    /// <summary>
    /// Enum used to define the vertical alignment property
    /// </summary>
    public enum VerticalAlignOption
    {
        Default,
        Top,
		Middle,
        Bottom

    } //VerticalAlignOption

	public enum LineStyle
	{
		None,
		Dotted,
		Dashed,
		Solid,
		Double
	}

    /// <summary>
    /// Enum used to define the visibility of the scroll bars
    /// </summary>
    public enum DisplayScrollBarOption
    {
        Yes,
        No,
        Auto

    } //DisplayScrollBarOption


    /// <summary>
    /// Enum used to define the unit of measure for a value
    /// </summary>
    public enum MeasurementOption
    {
        Percent,
        Pixel,

    } //MeasurementOption

}