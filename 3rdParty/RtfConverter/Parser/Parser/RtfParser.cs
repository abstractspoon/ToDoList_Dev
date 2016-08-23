// -- FILE ------------------------------------------------------------------
// name       : RtfParser.cs
// project    : RTF Framelet
// created    : Leon Poyyayil - 2008.05.20
// language   : c#
// environment: .NET 2.0
// copyright  : (c) 2004-2009 by Itenso GmbH, Switzerland
// --------------------------------------------------------------------------
using System;
using System.Collections;
using System.Globalization;
using System.IO;
using System.Text;
using Itenso.Rtf.Model;

namespace Itenso.Rtf.Parser
{

	// ------------------------------------------------------------------------
	public sealed class RtfParser : RtfParserBase
	{

		// ----------------------------------------------------------------------
		public RtfParser()
		{
		} // RtfParser

		// ----------------------------------------------------------------------
		public RtfParser( params IRtfParserListener[] listeners )
			: base( listeners )
		{
		} // RtfParser

		// ----------------------------------------------------------------------
		protected override void DoParse( IRtfSource rtfTextSource )
		{
			NotifyParseBegin();
			try
			{
				ParseRtf( rtfTextSource.Reader );
				NotifyParseSuccess();
			}
			catch ( RtfException e )
			{
				NotifyParseFail( e );
				throw;
			}
			finally
			{
				NotifyParseEnd();
			}
		} // DoParse

		// ----------------------------------------------------------------------
		private void ParseRtf( TextReader reader )
		{
			this.curText = new StringBuilder();

			this.unicodeSkipCountStack.Clear();
			this.codePageStack.Clear();
			this.unicodeSkipCount = 1;
			this.level = 0;
			this.tagCountAtLastGroupStart = 0;
			this.tagCount = 0;
			this.fontTableStartLevel = -1;
			this.targetFont = null;
			this.expectingThemeFont = false;
			this.fontToCodePageMapping.Clear();
			this.hexDecodingBuffer.SetLength( 0 );
			UpdateEncoding( RtfSpec.AnsiCodePage );
			int groupCount = 0;
			const int eof = -1;
			int nextChar = PeekNextChar( reader, false );
			bool backslashAlreadyConsumed = false;
			while ( nextChar != eof )
			{
				int peekChar = 0;
				bool peekCharValid = false;
				switch ( nextChar )
				{
					case '\\':
						if ( !backslashAlreadyConsumed )
						{
							reader.Read(); // must still consume the 'peek'ed char
						}
						int secondChar = PeekNextChar( reader, true );
						switch ( secondChar )
						{
							case '\\':
							case '{':
							case '}':
								this.curText.Append( ReadOneChar( reader ) ); // must still consume the 'peek'ed char
								break;

							case '\n':
							case '\r':
								reader.Read(); // must still consume the 'peek'ed char
								// must be treated as a 'par' tag if preceded by a backslash
								// (see RTF spec page 144)
								HandleTag( reader, new RtfTag( RtfSpec.TagParagraph ) );
								break;

							case '\'':
								reader.Read(); // must still consume the 'peek'ed char
								char hex1 = (char)ReadOneByte( reader );
								char hex2 = (char)ReadOneByte( reader );
								if ( !IsHexDigit( hex1 ) )
								{
									throw new RtfHexEncodingException( Strings.InvalidFirstHexDigit( hex1 ) );
								}
								if ( !IsHexDigit( hex2 ) )
								{
									throw new RtfHexEncodingException( Strings.InvalidSecondHexDigit( hex2 ) );
								}
								int decodedByte = Int32.Parse( "" + hex1 + hex2, NumberStyles.HexNumber );
								this.hexDecodingBuffer.WriteByte( (byte)decodedByte );
								peekChar = PeekNextChar( reader, false );
								peekCharValid = true;
								bool mustFlushHexContent = true;
								if ( peekChar == '\\' )
								{
									reader.Read();
									backslashAlreadyConsumed = true;
									int continuationChar = PeekNextChar( reader, false );
									if ( continuationChar == '\'' )
									{
										mustFlushHexContent = false;
									}
								}
								if ( mustFlushHexContent )
								{
									// we may _NOT_ handle hex content in a character-by-character way as
									// this results in invalid text for japanese/chinese content ...
									// -> we wait until the following content is non-hex and then flush the
									//    pending data. ugly but necessary with our decoding model.
									DecodeCurrentHexBuffer();
								}
								break;

							case '|':
							case '~':
							case '-':
							case '_':
							case ':':
							case '*':
								HandleTag( reader, new RtfTag( "" + ReadOneChar( reader ) ) ); // must still consume the 'peek'ed char
								break;

							default:
								ParseTag( reader );
								break;
						}
						break;

					case '\n':
					case '\r':
						reader.Read(); // must still consume the 'peek'ed char
						break;

					case '\t':
						reader.Read(); // must still consume the 'peek'ed char
						// should be treated as a 'tab' tag (see RTF spec page 144)
						HandleTag( reader, new RtfTag( RtfSpec.TagTabulator ) );
						break;

					case '{':
						reader.Read(); // must still consume the 'peek'ed char
						FlushText();
						NotifyGroupBegin();
						this.tagCountAtLastGroupStart = this.tagCount;
						this.unicodeSkipCountStack.Push( this.unicodeSkipCount );
						this.codePageStack.Push( this.encoding == null ? 0 : this.encoding.CodePage );
						this.level++;
						break;

					case '}':
						reader.Read(); // must still consume the 'peek'ed char
						FlushText();
						if ( this.level > 0 )
						{
							this.unicodeSkipCount = (int)this.unicodeSkipCountStack.Pop();
							if ( this.fontTableStartLevel == this.level )
							{
								this.fontTableStartLevel = -1;
								this.targetFont = null;
								this.expectingThemeFont = false;
							}
							UpdateEncoding( (int)this.codePageStack.Pop() );
							this.level--;
							NotifyGroupEnd();
							groupCount++;
						}
						else
						{
							throw new RtfBraceNestingException( Strings.ToManyBraces );
						}
						break;

					default:
						this.curText.Append( ReadOneChar( reader ) ); // must still consume the 'peek'ed char
						break;
				}
				if ( this.level == 0 && IgnoreContentAfterRootGroup )
				{
					break;
				}
				if ( peekCharValid )
				{
					nextChar = peekChar;
				}
				else
				{
					nextChar = PeekNextChar( reader, false );
					backslashAlreadyConsumed = false;
				}
			}
			FlushText();
			reader.Close();

			if ( this.level > 0 )
			{
				throw new RtfBraceNestingException( Strings.ToFewBraces );
			}
			if ( groupCount == 0 )
			{
				throw new RtfEmptyDocumentException( Strings.NoRtfContent );
			}
			this.curText = null;
		} // ParseRtf

		// ----------------------------------------------------------------------
		private void ParseTag( TextReader reader )
		{
			StringBuilder tagName = new StringBuilder();
			StringBuilder tagValue = null;
			bool readingName = true;
			bool delimReached = false;

			int nextChar = PeekNextChar( reader, true );
			while ( !delimReached )
			{
				if ( readingName && IsASCIILetter( nextChar ) )
				{
					tagName.Append( ReadOneChar( reader ) ); // must still consume the 'peek'ed char
				}
				else if ( IsDigit( nextChar ) || (nextChar == '-' && tagValue == null) )
				{
					readingName = false;
					if ( tagValue == null )
					{
						tagValue = new StringBuilder();
					}
					tagValue.Append( ReadOneChar( reader ) ); // must still consume the 'peek'ed char
				}
				else
				{
					delimReached = true;
					IRtfTag newTag;
					if ( tagValue != null && tagValue.Length > 0 )
					{
						newTag = new RtfTag( tagName.ToString(), tagValue.ToString() );
					}
					else
					{
						newTag = new RtfTag( tagName.ToString() );
					}
					bool skippedContent = HandleTag( reader, newTag );
					if ( nextChar == ' ' && !skippedContent )
					{
						reader.Read(); // must still consume the 'peek'ed char
					}
				}
				if ( !delimReached )
				{
					nextChar = PeekNextChar( reader, true );
				}
			}
		} // ParseTag

		// ----------------------------------------------------------------------
		private bool HandleTag( TextReader reader, IRtfTag tag )
		{
			if ( this.level == 0 )
			{
				throw new RtfStructureException( Strings.TagOnRootLevel( tag.ToString() ) );
			}

			if ( this.tagCount < 4 )
			{
				// this only handles the initial encoding tag in the header section
				UpdateEncoding( tag );
			}

			string tagName = tag.Name;
			// enable the font name detection in case the last tag was introducing
			// a theme font
			bool detectFontName = this.expectingThemeFont;
			if ( this.tagCountAtLastGroupStart == this.tagCount )
			{
				// first tag in a group
				switch ( tagName )
				{
					case RtfSpec.TagThemeFontLoMajor:
					case RtfSpec.TagThemeFontHiMajor:
					case RtfSpec.TagThemeFontDbMajor:
					case RtfSpec.TagThemeFontBiMajor:
					case RtfSpec.TagThemeFontLoMinor:
					case RtfSpec.TagThemeFontHiMinor:
					case RtfSpec.TagThemeFontDbMinor:
					case RtfSpec.TagThemeFontBiMinor:
						// these introduce a new font, but the actual font tag will be
						// the second tag in the group, so we must remember this condition ...
						this.expectingThemeFont = true;
						break;
				}
				// always enable the font name detection also for the first tag in a group
				detectFontName = true;
			}
			if ( detectFontName )
			{
				// first tag in a group:
				switch ( tagName )
				{
					case RtfSpec.TagFont:
						if ( this.fontTableStartLevel > 0 )
						{
							// in the font-table definition:
							// -> remember the target font for charset mapping
							this.targetFont = tag.FullName;
							this.expectingThemeFont = false; // reset that state now
						}
						break;
					case RtfSpec.TagFontTable:
						// -> remember we're in the font-table definition
						this.fontTableStartLevel = this.level;
						break;
				}
			}
			if ( this.targetFont != null )
			{
				// within a font-tables font definition: perform charset mapping
				if ( RtfSpec.TagFontCharset.Equals( tagName ) )
				{
					int charSet = tag.ValueAsNumber;
					int codePage = RtfSpec.GetCodePage( charSet );
					this.fontToCodePageMapping[ this.targetFont ] = codePage;
					UpdateEncoding( codePage );
				}
			}
			if ( this.fontToCodePageMapping.Count > 0 && RtfSpec.TagFont.Equals( tagName ) )
			{
				int? codePage = (int?)this.fontToCodePageMapping[ tag.FullName ];
				if ( codePage != null )
				{
					UpdateEncoding( codePage.Value );
				}
			}

			bool skippedContent = false;
			switch ( tagName )
			{
				case RtfSpec.TagUnicodeCode:
					int unicodeValue = tag.ValueAsNumber;
					char unicodeChar = (char)unicodeValue;
					this.curText.Append( unicodeChar );
					// skip over the indicated number of 'alternative representation' text
					for ( int i = 0; i < this.unicodeSkipCount; i++ )
					{
						int nextChar = PeekNextChar( reader, true );
						switch ( nextChar )
						{
							case ' ':
							case '\r':
							case '\n':
								reader.Read(); // consume peeked char
								skippedContent = true;
								if ( i == 0 )
								{
									// the first whitespace after the tag
									// -> only a delimiter, doesn't count for skipping ...
									i--;
								}
								break;
							case '\\':
								reader.Read(); // consume peeked char
								skippedContent = true;
								int secondChar = ReadOneByte( reader ); // mandatory
								switch ( secondChar )
								{
									case '\'':
										// ok, this is a hex-encoded 'byte' -> need to consume both
										// hex digits too
										ReadOneByte( reader ); // high nibble
										ReadOneByte( reader ); // low nibble
										break;
								}
								break;
							case '{':
							case '}':
								// don't consume peeked char and abort skipping
								i = this.unicodeSkipCount;
								break;
							default:
								reader.Read(); // consume peeked char
								skippedContent = true;
								break;
						}
					}
					break;

				case RtfSpec.TagUnicodeSkipCount:
					int newSkipCount = tag.ValueAsNumber;
					if ( newSkipCount < 0 || newSkipCount > 10 )
					{
						throw new RtfUnicodeEncodingException( Strings.InvalidUnicodeSkipCount( tag.ToString() ) );
					}
					this.unicodeSkipCount = newSkipCount;
					break;

				default:
					FlushText();
					NotifyTagFound( tag );
					break;
			}

			this.tagCount++;

			return skippedContent;
		} // HandleTag

		// ----------------------------------------------------------------------
		private void UpdateEncoding( IRtfTag tag )
		{
			switch ( tag.Name )
			{
				case RtfSpec.TagEncodingAnsi:
					UpdateEncoding( RtfSpec.AnsiCodePage );
					break;
				case RtfSpec.TagEncodingMac:
					UpdateEncoding( 10000 );
					break;
				case RtfSpec.TagEncodingPc:
					UpdateEncoding( 437 );
					break;
				case RtfSpec.TagEncodingPca:
					UpdateEncoding( 850 );
					break;
				case RtfSpec.TagEncodingAnsiCodePage:
					UpdateEncoding( tag.ValueAsNumber );
					break;
			}
		} // UpdateEncoding

		// ----------------------------------------------------------------------
		private void UpdateEncoding( int codePage )
		{
			if ( this.encoding == null || codePage != this.encoding.CodePage )
			{
				switch ( codePage )
				{
					case RtfSpec.AnsiCodePage:
					case RtfSpec.SymbolFakeCodePage: // hack to handle a windows legacy ...
						this.encoding = RtfSpec.AnsiEncoding;
						break;
					default:
						this.encoding = Encoding.GetEncoding( codePage );
						break;
				}
				this.byteToCharDecoder = null;
			}
			if ( this.byteToCharDecoder == null )
			{
				this.byteToCharDecoder = this.encoding.GetDecoder();
			}
		} // UpdateEncoding

		// ----------------------------------------------------------------------
		private static bool IsASCIILetter( int character )
		{
			return (character >= 'a' && character <= 'z') || (character >= 'A' && character <= 'Z');
		} // IsASCIILetter

		// ----------------------------------------------------------------------
		private static bool IsHexDigit( int character )
		{
			return (character >= '0' && character <= '9') ||
						 (character >= 'a' && character <= 'f') ||
						 (character >= 'A' && character <= 'F');
		} // IsHexDigit

		// ----------------------------------------------------------------------
		private static bool IsDigit( int character )
		{
			return character >= '0' && character <= '9';
		} // IsDigit

		// ----------------------------------------------------------------------
		private static int ReadOneByte( TextReader reader )
		{
			int byteValue = reader.Read();
			if ( byteValue == -1 )
			{
				throw new RtfUnicodeEncodingException( Strings.UnexpectedEndOfFile );
			}
			return byteValue;
		} // ReadOneByte

		// ----------------------------------------------------------------------
		private char ReadOneChar( TextReader reader )
		{
			// NOTE: the handling of multi-byte encodings is probably not the most
			// efficient here ...

			bool completed = false;
			int byteIndex = 0;
			while ( !completed )
			{
				this.byteDecodingBuffer[ byteIndex ] = (byte)ReadOneByte( reader );
				byteIndex++;
				int usedBytes;
				int usedChars;
				this.byteToCharDecoder.Convert(
					this.byteDecodingBuffer, 0, byteIndex,
					this.charDecodingBuffer, 0, 1,
					true,
					out usedBytes,
					out usedChars,
					out completed );
				if ( completed && ( usedBytes != byteIndex || usedChars != 1 ) )
				{
					throw new RtfMultiByteEncodingException( Strings.InvalidMultiByteEncoding( 
					this.byteDecodingBuffer, byteIndex, this.encoding ) );
				}
			}
			char character = this.charDecodingBuffer[ 0 ];
			return character;
		} // ReadOneChar

		// ----------------------------------------------------------------------
		private void DecodeCurrentHexBuffer()
		{
			long pendingByteCount = this.hexDecodingBuffer.Length;
			if ( pendingByteCount > 0 )
			{
				byte[] pendingBytes = this.hexDecodingBuffer.ToArray();
				char[] convertedChars = new char[ pendingByteCount ]; // should be enough

				int startIndex = 0;
				bool completed = false;
				while ( !completed && startIndex < pendingBytes.Length )
				{
					int usedBytes;
					int usedChars;
					this.byteToCharDecoder.Convert(
						pendingBytes, startIndex, pendingBytes.Length - startIndex,
						convertedChars, 0, convertedChars.Length,
						true,
						out usedBytes,
						out usedChars,
						out completed );
					this.curText.Append( convertedChars, 0, usedChars );
					startIndex += usedChars;
				}

				this.hexDecodingBuffer.SetLength( 0 );
			}
		} // DecodeCurrentHexBuffer

		// ----------------------------------------------------------------------
		private static int PeekNextChar( TextReader reader, bool mandatory )
		{
			int character = reader.Peek();
			if ( mandatory && character == -1 )
			{
				throw new RtfMultiByteEncodingException( Strings.EndOfFileInvalidCharacter );
			}
			return character;
		} // PeekNextChar

		// ----------------------------------------------------------------------
		private void FlushText()
		{
			if ( this.curText.Length > 0 )
			{
				if ( this.level == 0 )
				{
					throw new RtfStructureException( Strings.TextOnRootLevel( this.curText.ToString() ) );
				}
				NotifyTextFound( new RtfText( this.curText.ToString() ) );
				this.curText.Remove( 0, this.curText.Length );
			}
		} // FlushText

		// ----------------------------------------------------------------------
		// members
		private StringBuilder curText;
		private readonly Stack unicodeSkipCountStack = new Stack();
		private int unicodeSkipCount;
		private readonly Stack codePageStack = new Stack();
		private int level;
		private int tagCountAtLastGroupStart;
		private int tagCount;
		private int fontTableStartLevel;
		private string targetFont;
		private bool expectingThemeFont;
		private readonly Hashtable fontToCodePageMapping = new Hashtable();
		private Encoding encoding;
		private Decoder byteToCharDecoder;
		private readonly MemoryStream hexDecodingBuffer = new MemoryStream();
		private readonly byte[] byteDecodingBuffer = new byte[ 8 ]; // >0 for multi-byte encodings
		private readonly char[] charDecodingBuffer = new char[ 1 ];

	} // class RtfParser

} // namespace Itenso.Rtf.Parser
// -- EOF -------------------------------------------------------------------
