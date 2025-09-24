// -- FILE ------------------------------------------------------------------
// name       : RtfParserListenerStructureBuilder.cs
// project    : RTF Framelet
// created    : Leon Poyyayil - 2008.05.19
// language   : c#
// environment: .NET 2.0
// copyright  : (c) 2004-2009 by Itenso GmbH, Switzerland
// --------------------------------------------------------------------------
using System.Collections;
using Itenso.Rtf.Model;

namespace Itenso.Rtf.Parser
{

	// ------------------------------------------------------------------------
	public sealed class RtfParserListenerStructureBuilder : RtfParserListenerBase
	{

		// ----------------------------------------------------------------------
		public RtfParserListenerStructureBuilder()
		{
		} // RtfParserListenerStructureBuilder

		// ----------------------------------------------------------------------
		public IRtfGroup StructureRoot
		{
			get { return this.structureRoot; }
		} // StructureRoot

		// ----------------------------------------------------------------------
		protected override void DoParseBegin()
		{
			this.openGroupStack.Clear();
			this.curGroup = null;
			this.structureRoot = null;
		} // DoParseBegin

		// ----------------------------------------------------------------------
		protected override void DoGroupBegin()
		{
			RtfGroup newGroup = new RtfGroup();
			if ( this.curGroup != null )
			{
				this.openGroupStack.Push( this.curGroup );
				this.curGroup.WritableContents.Add( newGroup );
			}
			this.curGroup = newGroup;
		} // DoGroupBegin

		// ----------------------------------------------------------------------
		protected override void DoTagFound( IRtfTag tag )
		{
			if ( this.curGroup == null )
			{
				throw new RtfStructureException( Strings.MissingGroupForNewTag );
			}
			this.curGroup.WritableContents.Add( tag );
		} // DoTagFound

		// ----------------------------------------------------------------------
		protected override void DoTextFound( IRtfText text )
		{
			if ( this.curGroup == null )
			{
				throw new RtfStructureException( Strings.MissingGroupForNewText );
			}
			this.curGroup.WritableContents.Add( text );
		} // DoTextFound

		// ----------------------------------------------------------------------
		protected override void DoGroupEnd()
		{
			if ( this.openGroupStack.Count > 0 )
			{
				this.curGroup = (RtfGroup)this.openGroupStack.Pop();
			}
			else
			{
				if ( structureRoot != null )
				{
					throw new RtfStructureException( Strings.MultipleRootLevelGroups );
				}
				this.structureRoot = this.curGroup;
				this.curGroup = null;
			}
		} // DoGroupEnd

		// ----------------------------------------------------------------------
		protected override void DoParseEnd()
		{
			if ( openGroupStack.Count > 0 )
			{
				throw new RtfBraceNestingException( Strings.UnclosedGroups );
			}
		} // DoParseEnd

		// ----------------------------------------------------------------------
		// members
		private readonly Stack openGroupStack = new Stack();
		private RtfGroup curGroup;
		private RtfGroup structureRoot;

	} // class RtfParserListenerStructureBuilder

} // namespace Itenso.Rtf.Parser
// -- EOF -------------------------------------------------------------------
