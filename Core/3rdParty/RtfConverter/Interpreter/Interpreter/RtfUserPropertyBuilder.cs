// -- FILE ------------------------------------------------------------------
// name       : RtfUserPropertyBuilder.cs
// project    : RTF Framelet
// created    : Leon Poyyayil - 2008.05.23
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
	public sealed class RtfUserPropertyBuilder : RtfElementVisitorBase
	{

		// ----------------------------------------------------------------------
		public RtfUserPropertyBuilder( RtfDocumentPropertyCollection collectedProperties ) :
			base( RtfElementVisitorOrder.NonRecursive )
		{
			// we iterate over our children ourselves -> hence non-recursive
			if ( collectedProperties == null )
			{
				throw new ArgumentNullException( "collectedProperties" );
			}
			this.collectedProperties = collectedProperties;
		} // RtfUserPropertyBuilder

		// ----------------------------------------------------------------------
		public IRtfDocumentProperty CreateProperty()
		{
			return new RtfDocumentProperty( this.propertyTypeCode, this.propertyName, this.staticValue, this.linkValue );
		} // CreateProperty

		// ----------------------------------------------------------------------
		public void Reset()
		{
			this.propertyTypeCode = 0;
			this.propertyName = null;
			this.staticValue = null;
			this.linkValue = null;
		} // Reset

		// ----------------------------------------------------------------------
		protected override void DoVisitGroup( IRtfGroup group )
		{
			switch ( group.Destination )
			{
				case RtfSpec.TagUserProperties:
					VisitGroupChildren( group );
					break;
				case null:
					Reset();
					VisitGroupChildren( group );
					this.collectedProperties.Add( CreateProperty() );
					break;
				case RtfSpec.TagUserPropertyName:
					this.textBuilder.Reset();
					this.textBuilder.VisitGroup( group );
					this.propertyName = textBuilder.CombinedText;
					break;
				case RtfSpec.TagUserPropertyValue:
					this.textBuilder.Reset();
					this.textBuilder.VisitGroup( group );
					this.staticValue = textBuilder.CombinedText;
					break;
				case RtfSpec.TagUserPropertyLink:
					this.textBuilder.Reset();
					this.textBuilder.VisitGroup( group );
					this.linkValue = textBuilder.CombinedText;
					break;
			}
		} // DoVisitGroup

		// ----------------------------------------------------------------------
		protected override void DoVisitTag( IRtfTag tag )
		{
			switch ( tag.Name )
			{
				case RtfSpec.TagUserPropertyType:
					this.propertyTypeCode = tag.ValueAsNumber;
					break;
			}
		} // DoVisitTag

		// ----------------------------------------------------------------------
		// members
		private readonly RtfDocumentPropertyCollection collectedProperties;
		private readonly RtfTextBuilder textBuilder = new RtfTextBuilder();
		private int propertyTypeCode;
		private string propertyName;
		private string staticValue;
		private string linkValue;

	} // class RtfUserPropertyBuilder

} // namespace Itenso.Rtf.Interpreter
// -- EOF -------------------------------------------------------------------
