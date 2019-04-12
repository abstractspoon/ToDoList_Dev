// -- FILE ------------------------------------------------------------------
// name       : RtfDocumentInfoBuilder.cs
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
	public sealed class RtfDocumentInfoBuilder : RtfElementVisitorBase
	{

		// ----------------------------------------------------------------------
		public RtfDocumentInfoBuilder( RtfDocumentInfo info ) :
			base( RtfElementVisitorOrder.NonRecursive )
		{
			// we iterate over our children ourselves -> hence non-recursive
			if ( info == null )
			{
				throw new ArgumentNullException( "info" );
			}
			this.info = info;
		} // RtfDocumentInfoBuilder

		// ----------------------------------------------------------------------
		public void Reset()
		{
			this.info.Reset();
		} // Reset

		// ----------------------------------------------------------------------
		protected override void DoVisitGroup( IRtfGroup group )
		{
			switch ( group.Destination )
			{
				case RtfSpec.TagInfo:
					VisitGroupChildren( group );
					break;
				case RtfSpec.TagInfoTitle:
					this.info.Title = ExtractGroupText( group );
					break;
				case RtfSpec.TagInfoSubject:
					this.info.Subject = ExtractGroupText( group );
					break;
				case RtfSpec.TagInfoAuthor:
					this.info.Author = ExtractGroupText( group );
					break;
				case RtfSpec.TagInfoManager:
					this.info.Manager = ExtractGroupText( group );
					break;
				case RtfSpec.TagInfoCompany:
					this.info.Company = ExtractGroupText( group );
					break;
				case RtfSpec.TagInfoOperator:
					this.info.Operator = ExtractGroupText( group );
					break;
				case RtfSpec.TagInfoCategory:
					this.info.Category = ExtractGroupText( group );
					break;
				case RtfSpec.TagInfoKeywords:
					this.info.Keywords = ExtractGroupText( group );
					break;
				case RtfSpec.TagInfoComment:
					this.info.Comment = ExtractGroupText( group );
					break;
				case RtfSpec.TagInfoDocumentComment:
					this.info.DocumentComment = ExtractGroupText( group );
					break;
				case RtfSpec.TagInfoHyperLinkBase:
					this.info.HyperLinkbase = ExtractGroupText( group );
					break;
				case RtfSpec.TagInfoCreationTime:
					this.info.CreationTime = ExtractTimestamp( group );
					break;
				case RtfSpec.TagInfoRevisionTime:
					this.info.RevisionTime = ExtractTimestamp( group );
					break;
				case RtfSpec.TagInfoPrintTime:
					this.info.PrintTime = ExtractTimestamp( group );
					break;
				case RtfSpec.TagInfoBackupTime:
					this.info.BackupTime = ExtractTimestamp( group );
					break;
			}
		} // DoVisitGroup

		// ----------------------------------------------------------------------
		protected override void DoVisitTag( IRtfTag tag )
		{
			switch ( tag.Name )
			{
				case RtfSpec.TagInfoVersion:
					this.info.Version = tag.ValueAsNumber;
					break;
				case RtfSpec.TagInfoRevision:
					this.info.Revision = tag.ValueAsNumber;
					break;
				case RtfSpec.TagInfoNumberOfPages:
					this.info.NumberOfPages = tag.ValueAsNumber;
					break;
				case RtfSpec.TagInfoNumberOfWords:
					this.info.NumberOfWords = tag.ValueAsNumber;
					break;
				case RtfSpec.TagInfoNumberOfChars:
					this.info.NumberOfCharacters = tag.ValueAsNumber;
					break;
				case RtfSpec.TagInfoId:
					this.info.Id = tag.ValueAsNumber;
					break;
				case RtfSpec.TagInfoEditingTimeMinutes:
					this.info.EditingTimeInMinutes = tag.ValueAsNumber;
					break;
			}
		} // DoVisitTag

		// ----------------------------------------------------------------------
		private string ExtractGroupText( IRtfGroup group )
		{
			this.textBuilder.Reset();
			this.textBuilder.VisitGroup( group );
			return this.textBuilder.CombinedText;
		} // ExtractGroupText

		// ----------------------------------------------------------------------
		private DateTime ExtractTimestamp( IRtfGroup group )
		{
			this.timestampBuilder.Reset();
			this.timestampBuilder.VisitGroup( group );
			return this.timestampBuilder.CreateTimestamp();
		} // ExtractTimestamp

		// ----------------------------------------------------------------------
		// members
		private readonly RtfDocumentInfo info;
		private readonly RtfTextBuilder textBuilder = new RtfTextBuilder();
		private readonly RtfTimestampBuilder timestampBuilder = new RtfTimestampBuilder();

	} // class RtfDocumentInfoBuilder

} // namespace Itenso.Rtf.Interpreter
// -- EOF -------------------------------------------------------------------
