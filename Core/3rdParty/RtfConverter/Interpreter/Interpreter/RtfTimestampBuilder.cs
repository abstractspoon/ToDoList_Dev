// -- FILE ------------------------------------------------------------------
// name       : RtfTimestampBuilder.cs
// project    : RTF Framelet
// created    : Leon Poyyayil - 2008.05.23
// language   : c#
// environment: .NET 2.0
// copyright  : (c) 2004-2009 by Itenso GmbH, Switzerland
// --------------------------------------------------------------------------
using System;
using Itenso.Rtf.Support;

namespace Itenso.Rtf.Interpreter
{

	// ------------------------------------------------------------------------
	public sealed class RtfTimestampBuilder : RtfElementVisitorBase
	{

		// ----------------------------------------------------------------------
		public RtfTimestampBuilder()
			: base( RtfElementVisitorOrder.BreadthFirst )
		{
			Reset();
		} // RtfTimestampBuilder

		// ----------------------------------------------------------------------
		public void Reset()
		{
			this.year = 1970;
			this.month = 1;
			this.day = 1;
			this.hour = 0;
			this.minutes = 0;
			this.seconds = 0;
		} // Reset

		// ----------------------------------------------------------------------
		public DateTime CreateTimestamp()
		{
			return new DateTime( this.year, this.month, this.day, this.hour, this.minutes, this.seconds );
		} // CreateTimestamp

		// ----------------------------------------------------------------------
		protected override void DoVisitTag( IRtfTag tag )
		{
			switch ( tag.Name )
			{
				case RtfSpec.TagInfoYear:
					this.year = tag.ValueAsNumber;
					break;
				case RtfSpec.TagInfoMonth:
					this.month = tag.ValueAsNumber;
					break;
				case RtfSpec.TagInfoDay:
					this.day = tag.ValueAsNumber;
					break;
				case RtfSpec.TagInfoHour:
					this.hour = tag.ValueAsNumber;
					break;
				case RtfSpec.TagInfoMinute:
					this.minutes = tag.ValueAsNumber;
					break;
				case RtfSpec.TagInfoSecond:
					this.seconds = tag.ValueAsNumber;
					break;
			}
		} // DoVisitTag

		// ----------------------------------------------------------------------
		// members
		private int year;
		private int month;
		private int day;
		private int hour;
		private int minutes;
		private int seconds;

	} // class RtfTimestampBuilder

} // namespace Itenso.Rtf.Interpreter
// -- EOF -------------------------------------------------------------------
