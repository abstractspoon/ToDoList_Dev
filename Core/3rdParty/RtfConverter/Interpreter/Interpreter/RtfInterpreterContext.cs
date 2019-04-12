// -- FILE ------------------------------------------------------------------
// name       : RtfInterpreterContext.cs
// project    : RTF Framelet
// created    : Leon Poyyayil - 2008.05.21
// language   : c#
// environment: .NET 2.0
// copyright  : (c) 2004-2009 by Itenso GmbH, Switzerland
// --------------------------------------------------------------------------
using System;
using System.Collections;
using Itenso.Rtf.Model;

namespace Itenso.Rtf.Interpreter
{

	// ------------------------------------------------------------------------
	public sealed class RtfInterpreterContext : IRtfInterpreterContext
	{

		// ----------------------------------------------------------------------
		public RtfInterpreterContext()
		{
		} // RtfInterpreterContext

		// ----------------------------------------------------------------------
		public RtfInterpreterState State
		{
			get { return this.state; }
			set { this.state = value; }
		} // State

		// ----------------------------------------------------------------------
		public int RtfVersion
		{
			get { return this.rtfVersion; }
			set { this.rtfVersion = value; }
		} // RtfVersion

		// ----------------------------------------------------------------------
		public string DefaultFontId
		{
			get { return this.defaultFontId; }
			set { this.defaultFontId = value; }
		} // DefaultFontIndex

		// ----------------------------------------------------------------------
		public IRtfFont DefaultFont
		{
			get
			{
				IRtfFont defaultFont = this.fontTable[ this.defaultFontId ];
				if ( defaultFont != null )
				{
					return defaultFont;
				}
				throw new RtfUndefinedFontException( Strings.InvalidDefaultFont( 
					this.defaultFontId, this.fontTable.ToString() ) );
			}
		} // DefaultFont

		// ----------------------------------------------------------------------
		public IRtfFontCollection FontTable
		{
			get { return this.fontTable; }
		} // FontTable

		// ----------------------------------------------------------------------
		public RtfFontCollection WritableFontTable
		{
			get { return this.fontTable; }
		} // WritableFontTable

		// ----------------------------------------------------------------------
		public IRtfColorCollection ColorTable
		{
			get { return this.colorTable; }
		} // ColorTable

		// ----------------------------------------------------------------------
		public RtfColorCollection WritableColorTable
		{
			get { return this.colorTable; }
		} // WritableColorTable

		// ----------------------------------------------------------------------
		public string Generator
		{
			get { return this.generator; }
			set { this.generator = value; }
		} // Generator

		// ----------------------------------------------------------------------
		public IRtfTextFormatCollection UniqueTextFormats
		{
			get { return this.uniqueTextFormats; }
		} // UniqueTextFormats

		// ----------------------------------------------------------------------
		public IRtfTextFormat CurrentTextFormat
		{
			get { return this.currentTextFormat; }
		} // CurrentTextFormat

		// ----------------------------------------------------------------------
		public IRtfTextFormat GetSafeCurrentTextFormat()
		{
			return this.currentTextFormat != null ? this.currentTextFormat : WritableCurrentTextFormat;
		} // GetSafeCurrentTextFormat

		// ----------------------------------------------------------------------
		public RtfTextFormat WritableCurrentTextFormat
		{
			get
			{
				if ( this.currentTextFormat == null )
				{
					// set via property to ensure it will get added to the unique map
					WritableCurrentTextFormat = new RtfTextFormat( DefaultFont, RtfSpec.DefaultFontSize );
				}
				return this.currentTextFormat;
			}
			set
			{
				if ( value == null )
				{
					throw new ArgumentNullException( "value" );
				}
				int existingEquivalentPos = this.uniqueTextFormats.IndexOf( value );
				if ( existingEquivalentPos >= 0 )
				{
					// we already know an equivalent format -> reference that one for future use
					this.currentTextFormat = (RtfTextFormat)uniqueTextFormats[ existingEquivalentPos ];
				}
				else
				{
					// this is a yet unknown format -> add it to the known formats and use it
					this.uniqueTextFormats.Add( value );
					this.currentTextFormat = value;
				}
			}
		} // WritableCurrentTextFormat

		// ----------------------------------------------------------------------
		public IRtfDocumentInfo DocumentInfo
		{
			get { return this.documentInfo; }
		} // DocumentInfo

		// ----------------------------------------------------------------------
		public RtfDocumentInfo WritableDocumentInfo
		{
			get { return this.documentInfo; }
		} // WritableDocumentInfo

		// ----------------------------------------------------------------------
		public IRtfDocumentPropertyCollection UserProperties
		{
			get { return this.userProperties; }
		} // UserProperties

		// ----------------------------------------------------------------------
		public RtfDocumentPropertyCollection WritableUserProperties
		{
			get { return this.userProperties; }
		} // WritableUserProperties

		// ----------------------------------------------------------------------
		public void PushCurrentTextFormat()
		{
			this.textFormatStack.Push( WritableCurrentTextFormat );
		} // PushCurrentTextFormat

		// ----------------------------------------------------------------------
		public void PopCurrentTextFormat()
		{
			if ( this.textFormatStack.Count == 0 )
			{
				throw new RtfStructureException( Strings.InvalidTextContextState );
			}
			currentTextFormat = (RtfTextFormat)this.textFormatStack.Pop();
		} // PopCurrentTextFormat

		// ----------------------------------------------------------------------
		public void Reset()
		{
			this.state = RtfInterpreterState.Init;
			this.rtfVersion = RtfSpec.RtfVersion1;
			this.defaultFontId = "f0";
			this.fontTable.Clear();
			this.colorTable.Clear();
			this.generator = null;
			this.uniqueTextFormats.Clear();
			this.textFormatStack.Clear();
			this.currentTextFormat = null;
			this.documentInfo.Reset();
			this.userProperties.Clear();
		} // Reset

		// ----------------------------------------------------------------------
		// members
		private RtfInterpreterState state;
		private int rtfVersion;
		private string defaultFontId;
		private readonly RtfFontCollection fontTable = new RtfFontCollection();
		private readonly RtfColorCollection colorTable = new RtfColorCollection();
		private string generator;
		private readonly RtfTextFormatCollection uniqueTextFormats = new RtfTextFormatCollection();
		private readonly Stack textFormatStack = new Stack();
		private RtfTextFormat currentTextFormat;
		private readonly RtfDocumentInfo documentInfo = new RtfDocumentInfo();
		private readonly RtfDocumentPropertyCollection userProperties = new RtfDocumentPropertyCollection();

	} // class RtfInterpreterContext

} // namespace Itenso.Rtf.Interpreter
// -- EOF -------------------------------------------------------------------
