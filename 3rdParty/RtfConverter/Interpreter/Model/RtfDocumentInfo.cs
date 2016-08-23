// -- FILE ------------------------------------------------------------------
// name       : RtfDocumentInfo.cs
// project    : RTF Framelet
// created    : Leon Poyyayil - 2008.05.23
// language   : c#
// environment: .NET 2.0
// copyright  : (c) 2004-2009 by Itenso GmbH, Switzerland
// --------------------------------------------------------------------------
using System;

namespace Itenso.Rtf.Model
{

	// ------------------------------------------------------------------------
	public sealed class RtfDocumentInfo : IRtfDocumentInfo
	{

		// ----------------------------------------------------------------------
		public RtfDocumentInfo()
		{
		} // RtfDocumentInfo

		// ----------------------------------------------------------------------
		public int? Id
		{
			get { return this.id; }
			set { this.id = value; }
		} // Id

		// ----------------------------------------------------------------------
		public int? Version
		{
			get { return this.version; }
			set { this.version = value; }
		} // Version

		// ----------------------------------------------------------------------
		public int? Revision
		{
			get { return this.revision; }
			set { this.revision = value; }
		} // Revision

		// ----------------------------------------------------------------------
		public string Title
		{
			get { return this.title; }
			set { this.title = value; }
		} // Title

		// ----------------------------------------------------------------------
		public string Subject
		{
			get { return this.subject; }
			set { this.subject = value; }
		} // Subject

		// ----------------------------------------------------------------------
		public string Author
		{
			get { return this.author; }
			set { this.author = value; }
		} // Author

		// ----------------------------------------------------------------------
		public string Manager
		{
			get { return this.manager; }
			set { this.manager = value; }
		} // Manager

		// ----------------------------------------------------------------------
		public string Company
		{
			get { return this.company; }
			set { this.company = value; }
		} // Company

		// ----------------------------------------------------------------------
		public string Operator
		{
			get { return this.operatorName; }
			set { this.operatorName = value; }
		} // Operator

		// ----------------------------------------------------------------------
		public string Category
		{
			get { return this.category; }
			set { this.category = value; }
		} // Category

		// ----------------------------------------------------------------------
		public string Keywords
		{
			get { return this.keywords; }
			set { this.keywords = value; }
		} // Keywords

		// ----------------------------------------------------------------------
		public string Comment
		{
			get { return this.comment; }
			set { this.comment = value; }
		} // Comment

		// ----------------------------------------------------------------------
		public string DocumentComment
		{
			get { return this.documentComment; }
			set { this.documentComment = value; }
		} // DocumentComment

		// ----------------------------------------------------------------------
		public string HyperLinkbase
		{
			get { return this.hyperLinkbase; }
			set { this.hyperLinkbase = value; }
		} // HyperLinkbase

		// ----------------------------------------------------------------------
		public DateTime? CreationTime
		{
			get { return this.creationTime; }
			set { this.creationTime = value; }
		} // CreationTime

		// ----------------------------------------------------------------------
		public DateTime? RevisionTime
		{
			get { return this.revisionTime; }
			set { this.revisionTime = value; }
		} // RevisionTime

		// ----------------------------------------------------------------------
		public DateTime? PrintTime
		{
			get { return this.printTime; }
			set { this.printTime = value; }
		} // PrintTime

		// ----------------------------------------------------------------------
		public DateTime? BackupTime
		{
			get { return this.backupTime; }
			set { this.backupTime = value; }
		} // BackupTime

		// ----------------------------------------------------------------------
		public int? NumberOfPages
		{
			get { return this.numberOfPages; }
			set { this.numberOfPages = value; }
		} // NumberOfPages

		// ----------------------------------------------------------------------
		public int? NumberOfWords
		{
			get { return this.numberOfWords; }
			set { this.numberOfWords = value; }
		} // NumberOfWords

		// ----------------------------------------------------------------------
		public int? NumberOfCharacters
		{
			get { return this.numberOfCharacters; }
			set { this.numberOfCharacters = value; }
		} // NumberOfCharacters

		// ----------------------------------------------------------------------
		public int? EditingTimeInMinutes
		{
			get { return this.editingTimeInMinutes; }
			set { this.editingTimeInMinutes = value; }
		} // EditingTimeInMinutes

		// ----------------------------------------------------------------------
		public void Reset()
		{
			this.id = null;
			this.version = null;
			this.revision = null;
			this.title = null;
			this.subject = null;
			this.author = null;
			this.manager = null;
			this.company = null;
			this.operatorName = null;
			this.category = null;
			this.keywords = null;
			this.comment = null;
			this.documentComment = null;
			this.hyperLinkbase = null;
			this.creationTime = null;
			this.revisionTime = null;
			this.printTime = null;
			this.backupTime = null;
			this.numberOfPages = null;
			this.numberOfWords = null;
			this.numberOfCharacters = null;
			this.editingTimeInMinutes = null;
		} // Reset

		// ----------------------------------------------------------------------
		public override string ToString()
		{
			return "RTFDocInfo";
		} // ToString

		// ----------------------------------------------------------------------
		// members
		private int? id;
		private int? version;
		private int? revision;
		private string title;
		private string subject;
		private string author;
		private string manager;
		private string company;
		private string operatorName;
		private string category;
		private string keywords;
		private string comment;
		private string documentComment;
		private string hyperLinkbase;
		private DateTime? creationTime;
		private DateTime? revisionTime;
		private DateTime? printTime;
		private DateTime? backupTime;
		private int? numberOfPages;
		private int? numberOfWords;
		private int? numberOfCharacters;
		private int? editingTimeInMinutes;

	} // interface IRtfDocumentInfo

} // namespace Itenso.Rtf.Model
// -- EOF -------------------------------------------------------------------
