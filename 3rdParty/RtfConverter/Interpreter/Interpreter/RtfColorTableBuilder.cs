// -- FILE ------------------------------------------------------------------
// name       : RtfColorTableBuilder.cs
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
	public sealed class RtfColorTableBuilder : RtfElementVisitorBase
	{

		// ----------------------------------------------------------------------
		public RtfColorTableBuilder( RtfColorCollection colorTable ) :
			base( RtfElementVisitorOrder.NonRecursive )
		{
			// we iterate over our children ourselves -> hence non-recursive
			if ( colorTable == null )
			{
				throw new ArgumentNullException( "colorTable" );
			}
			this.colorTable = colorTable;
		} // RtfColorTableBuilder

		// ----------------------------------------------------------------------
		public void Reset()
		{
			this.colorTable.Clear();
			this.curRed = 0;
			this.curGreen = 0;
			this.curBlue = 0;
		} // Reset

		// ----------------------------------------------------------------------
		protected override void DoVisitGroup( IRtfGroup group )
		{
			if ( RtfSpec.TagColorTable.Equals( group.Destination ) )
			{
				VisitGroupChildren( group );
			}
		} // DoVisitGroup

		// ----------------------------------------------------------------------
		protected override void DoVisitTag( IRtfTag tag )
		{
			switch ( tag.Name )
			{
				case RtfSpec.TagColorRed:
					this.curRed = tag.ValueAsNumber;
					break;
				case RtfSpec.TagColorGreen:
					this.curGreen = tag.ValueAsNumber;
					break;
				case RtfSpec.TagColorBlue:
					this.curBlue = tag.ValueAsNumber;
					break;
			}
		} // DoVisitTag

		// ----------------------------------------------------------------------
		protected override void DoVisitText( IRtfText text )
		{
			if ( RtfSpec.TagDelimiter.Equals( text.Text ) )
			{
				this.colorTable.Add( new RtfColor( curRed, curGreen, curBlue ) );
				this.curRed = 0;
				this.curGreen = 0;
				this.curBlue = 0;
			}
			else
			{
				throw new RtfColorTableFormatException( Strings.ColorTableUnsupportedText( text.Text ) );
			}
		} // DoVisitText

		// ----------------------------------------------------------------------
		// members
		private readonly RtfColorCollection colorTable;

		private int curRed;
		private int curGreen;
		private int curBlue;

	} // class RtfColorBuilder

} // namespace Itenso.Rtf.Interpreter
// -- EOF -------------------------------------------------------------------
