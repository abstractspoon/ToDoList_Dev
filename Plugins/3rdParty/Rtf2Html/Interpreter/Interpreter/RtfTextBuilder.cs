// -- FILE ------------------------------------------------------------------
// name       : RtfTextBuilder.cs
// project    : RTF Framelet
// created    : Leon Poyyayil - 2008.05.23
// language   : c#
// environment: .NET 2.0
// copyright  : (c) 2004-2009 by Itenso GmbH, Switzerland
// --------------------------------------------------------------------------
using System.Text;
using Itenso.Rtf.Support;

namespace Itenso.Rtf.Interpreter
{

	// ------------------------------------------------------------------------
	public sealed class RtfTextBuilder : RtfElementVisitorBase
	{

		// ----------------------------------------------------------------------
		public RtfTextBuilder()
			: base( RtfElementVisitorOrder.DepthFirst )
		{
			Reset();
		} // RtfTextBuilder

		// ----------------------------------------------------------------------
		public string CombinedText
		{
			get { return this.buffer.ToString(); }
		} // CombinedText

		// ----------------------------------------------------------------------
		public void Reset()
		{
			this.buffer.Remove( 0, this.buffer.Length );
		} // Reset

		// ----------------------------------------------------------------------
		protected override void DoVisitText( IRtfText text )
		{
			this.buffer.Append( text.Text );
		} // DoVisitText

		// ----------------------------------------------------------------------
		// members
		private readonly StringBuilder buffer = new StringBuilder();

	} // class RtfTextBuilder

} // namespace Itenso.Rtf.Interpreter
// -- EOF -------------------------------------------------------------------
