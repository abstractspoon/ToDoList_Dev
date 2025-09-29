// -- FILE ------------------------------------------------------------------
// name       : RtfFontTableBuilder.cs
// project    : RTF Framelet
// created    : Leon Poyyayil - 2008.05.21
// language   : c#
// environment: .NET 2.0
// copyright  : (c) 2004-2009 by Itenso GmbH, Switzerland
// --------------------------------------------------------------------------
using System;
using Itenso.Rtf.Model;
using Itenso.Rtf.Support;

namespace Itenso.Rtf.Interpreter
{

	// ------------------------------------------------------------------------
	public sealed class RtfFontTableBuilder : RtfElementVisitorBase
	{

		// ----------------------------------------------------------------------
		public RtfFontTableBuilder( RtfFontCollection fontTable ) :
			base( RtfElementVisitorOrder.NonRecursive )
		{
			// we iterate over our children ourselves -> hence non-recursive
			if ( fontTable == null )
			{
				throw new ArgumentNullException( "fontTable" );
			}
			this.fontTable = fontTable;
		} // RtfFontTableBuilder

		// ----------------------------------------------------------------------
		public void Reset()
		{
			this.fontTable.Clear();
		} // Reset

		// ----------------------------------------------------------------------
		protected override void DoVisitGroup( IRtfGroup group )
		{
			switch ( group.Destination )
			{
				case RtfSpec.TagFont:
				case RtfSpec.TagThemeFontLoMajor:
				case RtfSpec.TagThemeFontHiMajor:
				case RtfSpec.TagThemeFontDbMajor:
				case RtfSpec.TagThemeFontBiMajor:
				case RtfSpec.TagThemeFontLoMinor:
				case RtfSpec.TagThemeFontHiMinor:
				case RtfSpec.TagThemeFontDbMinor:
				case RtfSpec.TagThemeFontBiMinor:
					BuildFontFromGroup( group );
					break;
				case RtfSpec.TagFontTable:
					if ( group.Contents.Count > 1 )
					{
						if ( group.Contents[ 1 ].Kind == RtfElementKind.Group )
						{
							// the 'new' style where each font resides in a group of its own
							VisitGroupChildren( group );
						}
						else
						{
							// the 'old' style where individual fonts are 'terminated' by their
							// respective name content text (which ends with ';')
							// -> need to manually iterate from here
							int childCount = group.Contents.Count;
							this.fontBuilder.Reset();
							for ( int i = 1; i < childCount; i++ ) // skip over the initial \fonttbl tag
							{
								group.Contents[ i ].Visit( this.fontBuilder );
								if ( this.fontBuilder.FontName != null )
								{
									// fonts are 'terminated' by their name (as content text)
									AddCurrentFont();
									this.fontBuilder.Reset();
								}
							}
							//BuildFontFromGroup( group ); // a single font info
						}
					}
					break;
			}
		} // DoVisitGroup

		// ----------------------------------------------------------------------
		private void BuildFontFromGroup( IRtfGroup group )
		{
			this.fontBuilder.Reset();
			this.fontBuilder.VisitGroup( group );
			AddCurrentFont();
		} // BuildFontFromGroup

		// ----------------------------------------------------------------------
		private void AddCurrentFont()
		{
			if ( !this.fontTable.ContainsFontWithId( this.fontBuilder.FontId ) )
			{
				this.fontTable.Add( fontBuilder.CreateFont() );
			}
			else
			{
				throw new RtfFontTableFormatException( Strings.DuplicateFont( this.fontBuilder.FontId ) );
			}
		} // AddCurrentFont

		// ----------------------------------------------------------------------
		// members
		private readonly RtfFontBuilder fontBuilder = new RtfFontBuilder();
		private readonly RtfFontCollection fontTable;

	} // class RtfFontBuilder

} // namespace Itenso.Rtf.Interpreter
// -- EOF -------------------------------------------------------------------
