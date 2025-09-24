// -- FILE ------------------------------------------------------------------
// name       : RtfHtmlStyle.cs
// project    : RTF Framelet
// created    : Jani Giannoudis - 2008.06.02
// language   : c#
// environment: .NET 2.0
// copyright  : (c) 2004-2009 by Itenso GmbH, Switzerland
// --------------------------------------------------------------------------
using Itenso.Sys;

namespace Itenso.Rtf.Converter.Html
{

	// ------------------------------------------------------------------------
	public class RtfHtmlStyle : IRtfHtmlStyle
	{

		// ----------------------------------------------------------------------
		public static RtfHtmlStyle Empty = new RtfHtmlStyle();

		// ----------------------------------------------------------------------
		public RtfHtmlStyle()
		{
		} // RtfHtmlStyle

		// ----------------------------------------------------------------------
		public string ForegroundColor
		{
			get { return this.foregroundColor; }
			set { this.foregroundColor = value; }
		} // ForegroundColor

		// ----------------------------------------------------------------------
		public string BackgroundColor
		{
			get { return this.backgroundColor; }
			set { this.backgroundColor = value; }
		} // BackgroundColor

		// ----------------------------------------------------------------------
		public string FontFamily
		{
			get { return this.fontFamily; }
			set { this.fontFamily = value; }
		} // FontFamily

		// ----------------------------------------------------------------------
		public string FontSize
		{
			get { return this.fontSize; }
			set { this.fontSize = value; }
		} // FontSize

		// ----------------------------------------------------------------------
		public bool IsEmpty
		{
			get { return this.Equals( Empty ); }
		} // IsEmpty

		// ----------------------------------------------------------------------
		public sealed override bool Equals( object obj )
		{
			if ( obj == this )
			{
				return true;
			}
			
			if ( obj == null || GetType() != obj.GetType() )
			{
				return false;
			}

			return IsEqual( obj );
		} // Equals

		// ----------------------------------------------------------------------
		public sealed override int GetHashCode()
		{
			return HashTool.AddHashCode( GetType().GetHashCode(), ComputeHashCode() );
		} // GetHashCode

		// ----------------------------------------------------------------------
		private bool IsEqual( object obj )
		{
			RtfHtmlStyle compare = obj as RtfHtmlStyle; // guaranteed to be non-null
			return
				compare != null &&
				string.Equals( this.foregroundColor, compare.foregroundColor ) &&
				string.Equals( this.backgroundColor, compare.backgroundColor ) &&
				string.Equals( this.fontFamily, compare.fontFamily ) &&
				string.Equals( this.fontSize, compare.fontSize );
		} // IsEqual

		// ----------------------------------------------------------------------
		private int ComputeHashCode()
		{
			int hash = this.foregroundColor.GetHashCode();
			hash = HashTool.AddHashCode( hash, this.backgroundColor );
			hash = HashTool.AddHashCode( hash, this.fontFamily );
			hash = HashTool.AddHashCode( hash, this.fontSize );
			return hash;
		} // ComputeHashCode

		// ----------------------------------------------------------------------
		// members
		private string foregroundColor;
		private string backgroundColor;
		private string fontFamily;
		private string fontSize;

	} // class RtfHtmlStyle

} // namespace Itenso.Rtf.Converter.Html
// -- EOF -------------------------------------------------------------------
