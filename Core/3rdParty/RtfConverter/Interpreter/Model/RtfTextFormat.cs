// -- FILE ------------------------------------------------------------------
// name       : IRtfTextFormat.cs
// project    : RTF Framelet
// created    : Leon Poyyayil - 2008.05.20
// language   : c#
// environment: .NET 2.0
// copyright  : (c) 2004-2009 by Itenso GmbH, Switzerland
// --------------------------------------------------------------------------
using System;
using System.Text;
using Itenso.Sys;

namespace Itenso.Rtf.Model
{

	// ------------------------------------------------------------------------
	public sealed class RtfTextFormat : IRtfTextFormat
	{

		// ----------------------------------------------------------------------
		public RtfTextFormat( IRtfFont font, int fontSize )
		{
			if ( font == null )
			{
				throw new ArgumentNullException( "font" );
			}
			if ( fontSize <= 0 || fontSize > 0xFFFF )
			{
				throw new ArgumentException( Strings.FontSizeOutOfRange( fontSize ) );
			}
			this.font = font;
			this.fontSize = fontSize;
		} // RtfTextFormat

		// ----------------------------------------------------------------------
		public RtfTextFormat( IRtfTextFormat copy )
		{
			if ( copy == null )
			{
				throw new ArgumentNullException( "copy" );
			}
			this.font = copy.Font; // enough because immutable
			this.fontSize = copy.FontSize;
			this.superScript = copy.SuperScript;
			this.bold = copy.IsBold;
			this.italic = copy.IsItalic;
			this.underline = copy.IsUnderline;
			this.strikeThrough = copy.IsStrikeThrough;
			this.hidden = copy.IsHidden;
			this.backgroundColor = copy.BackgroundColor; // enough because immutable
			this.foregroundColor = copy.ForegroundColor; // enough because immutable
			this.alignment = copy.Alignment;
		} // RtfTextFormat

		// ----------------------------------------------------------------------
		public RtfTextFormat( RtfTextFormat copy )
		{
			if ( copy == null )
			{
				throw new ArgumentNullException( "copy" );
			}
			this.font = copy.font; // enough because immutable
			this.fontSize = copy.fontSize;
			this.superScript = copy.superScript;
			this.bold = copy.bold;
			this.italic = copy.italic;
			this.underline = copy.underline;
			this.strikeThrough = copy.strikeThrough;
			this.hidden = copy.hidden;
			this.backgroundColor = copy.backgroundColor; // enough because immutable
			this.foregroundColor = copy.foregroundColor; // enough because immutable
			this.alignment = copy.alignment;
		} // RtfTextFormat

		// ----------------------------------------------------------------------
		public string FontDescriptionDebug
		{
			get
			{
				StringBuilder buf = new StringBuilder( this.font.Name );
				buf.Append( ", " );
				buf.Append( this.fontSize );
				buf.Append( this.superScript >= 0 ? "+" : "" );
				buf.Append( this.superScript );
				buf.Append( ", " );
				if ( this.bold || this.italic || this.underline || this.strikeThrough )
				{
					bool combined = false;
					if ( this.bold )
					{
						buf.Append( "bold" );
						combined = true;
					}
					if ( this.italic )
					{
						buf.Append( combined ? "+italic" : "italic" );
						combined = true;
					}
					if ( this.underline )
					{
						buf.Append( combined ? "+underline" : "underline" );
						combined = true;
					}
					if ( this.strikeThrough )
					{
						buf.Append( combined ? "+strikethrough" : "strikethrough" );
					}
				}
				else
				{
					buf.Append( "plain" );
				}
				if ( this.hidden )
				{
					buf.Append( ", hidden" );
				}
				return buf.ToString();
			}
		} // FontDescriptionDebug

		// ----------------------------------------------------------------------
		public IRtfFont Font
		{
			get { return this.font; }
		} // Font

		// ----------------------------------------------------------------------
		public RtfTextFormat DeriveWithFont( IRtfFont rtfFont )
		{
			if ( rtfFont == null )
			{
				throw new ArgumentNullException( "rtfFont" );
			}
			if ( this.font.Equals( rtfFont ) )
			{
				return this;
			}

			RtfTextFormat copy = new RtfTextFormat( this );
			copy.font = rtfFont;
			return copy;
		} // DeriveWithFont

		// ----------------------------------------------------------------------
		public int FontSize
		{
			get { return this.fontSize; }
		} // FontSize

		// ----------------------------------------------------------------------
		public RtfTextFormat DeriveWithFontSize( int derivedFontSize )
		{
			if ( derivedFontSize <= 0 || derivedFontSize > 0xFFFF )
			{
				throw new ArgumentException( Strings.FontSizeOutOfRange( derivedFontSize ) );
			}
			if ( this.fontSize == derivedFontSize )
			{
				return this;
			}

			RtfTextFormat copy = new RtfTextFormat( this );
			copy.fontSize = derivedFontSize;
			return copy;
		} // DeriveWithFontSize

		// ----------------------------------------------------------------------
		public int SuperScript
		{
			get { return this.superScript; }
		} // SuperScript

		// ----------------------------------------------------------------------
		public RtfTextFormat DeriveWithSuperScript( int deviation )
		{
			if ( this.superScript == deviation )
			{
				return this;
			}

			RtfTextFormat copy = new RtfTextFormat( this );
			copy.superScript = deviation;
			return copy;
		} // DeriveWithSuperScript

		// ----------------------------------------------------------------------
		public RtfTextFormat DeriveWithSuperScript( bool super )
		{
			RtfTextFormat copy = new RtfTextFormat( this );
			copy.fontSize = Math.Max( 1, ( this.fontSize * 2 ) / 3 );
			copy.superScript = ( super ? 1 : -1 ) * Math.Max( 1, this.fontSize / 2 );
			return copy;
		} // DeriveWithSuperScript

		// ----------------------------------------------------------------------
		public bool IsNormal
		{
			get
			{
				return
					!this.bold && !this.italic && !this.underline && !this.strikeThrough &&
					!this.hidden &&
					this.fontSize == RtfSpec.DefaultFontSize &&
					this.superScript == 0 &&
					RtfColor.Black.Equals( this.foregroundColor ) &&
					RtfColor.White.Equals( this.backgroundColor );
			}
		} // IsNormal

		// ----------------------------------------------------------------------
		public RtfTextFormat DeriveNormal()
		{
			if ( IsNormal )
			{
				return this;
			}

			RtfTextFormat copy = new RtfTextFormat( this.font, RtfSpec.DefaultFontSize );
			copy.alignment = this.alignment; // this is a paragraph property, keep it
			return copy;
		} // DeriveNormal

		// ----------------------------------------------------------------------
		public bool IsBold
		{
			get { return this.bold; }
		} // IsBold

		// ----------------------------------------------------------------------
		public RtfTextFormat DeriveWithBold( bool derivedBold )
		{
			if ( this.bold == derivedBold )
			{
				return this;
			}

			RtfTextFormat copy = new RtfTextFormat( this );
			copy.bold = derivedBold;
			return copy;
		} // DeriveWithBold

		// ----------------------------------------------------------------------
		public bool IsItalic
		{
			get { return this.italic; }
		} // IsItalic

		// ----------------------------------------------------------------------
		public RtfTextFormat DeriveWithItalic( bool derivedItalic )
		{
			if ( this.italic == derivedItalic )
			{
				return this;
			}

			RtfTextFormat copy = new RtfTextFormat( this );
			copy.italic = derivedItalic;
			return copy;
		} // DeriveWithItalic

		// ----------------------------------------------------------------------
		public bool IsUnderline
		{
			get { return this.underline; }
		} // IsUnderline

		// ----------------------------------------------------------------------
		public RtfTextFormat DeriveWithUnderline( bool derivedUnderline )
		{
			if ( this.underline == derivedUnderline )
			{
				return this;
			}

			RtfTextFormat copy = new RtfTextFormat( this );
			copy.underline = derivedUnderline;
			return copy;
		} // DeriveWithUnderline

		// ----------------------------------------------------------------------
		public bool IsStrikeThrough
		{
			get { return this.strikeThrough; }
		} // IsStrikeThrough

		// ----------------------------------------------------------------------
		public RtfTextFormat DeriveWithStrikeThrough( bool derivedStrikeThrough )
		{
			if ( this.strikeThrough == derivedStrikeThrough )
			{
				return this;
			}

			RtfTextFormat copy = new RtfTextFormat( this );
			copy.strikeThrough = derivedStrikeThrough;
			return copy;
		} // DeriveWithStrikeThrough

		// ----------------------------------------------------------------------
		public bool IsHidden
		{
			get { return this.hidden; }
		} // IsHidden

		// ----------------------------------------------------------------------
		public RtfTextFormat DeriveWithHidden( bool derivedHidden )
		{
			if ( this.hidden == derivedHidden )
			{
				return this;
			}

			RtfTextFormat copy = new RtfTextFormat( this );
			copy.hidden = derivedHidden;
			return copy;
		} // DeriveWithHidden

		// ----------------------------------------------------------------------
		public IRtfColor BackgroundColor
		{
			get { return this.backgroundColor; }
		} // BackgroundColor

		// ----------------------------------------------------------------------
		public RtfTextFormat DeriveWithBackgroundColor( IRtfColor derivedBackgroundColor )
		{
			if ( derivedBackgroundColor == null )
			{
				throw new ArgumentNullException( "derivedBackgroundColor" );
			}
			if ( this.backgroundColor.Equals( derivedBackgroundColor ) )
			{
				return this;
			}

			RtfTextFormat copy = new RtfTextFormat( this );
			copy.backgroundColor = derivedBackgroundColor;
			return copy;
		} // DeriveWithBackgroundColor

		// ----------------------------------------------------------------------
		public IRtfColor ForegroundColor
		{
			get { return this.foregroundColor; }
		} // ForegroundColor

		// ----------------------------------------------------------------------
		public RtfTextFormat DeriveWithForegroundColor( IRtfColor derivedForegroundColor )
		{
			if ( derivedForegroundColor == null )
			{
				throw new ArgumentNullException( "derivedForegroundColor" );
			}
			if ( this.foregroundColor.Equals( derivedForegroundColor ) )
			{
				return this;
			}

			RtfTextFormat copy = new RtfTextFormat( this );
			copy.foregroundColor = derivedForegroundColor;
			return copy;
		} // DeriveWithForegroundColor

		// ----------------------------------------------------------------------
		public RtfTextAlignment Alignment
		{
			get { return this.alignment; }
		} // Alignment

		// ----------------------------------------------------------------------
		public RtfTextFormat DeriveWithAlignment( RtfTextAlignment derivedAlignment )
		{
			if ( this.alignment == derivedAlignment )
			{
				return this;
			}

			RtfTextFormat copy = new RtfTextFormat( this );
			copy.alignment = derivedAlignment;
			return copy;
		} // DeriveWithForegroundColor

		// ----------------------------------------------------------------------
		IRtfTextFormat IRtfTextFormat.Duplicate()
		{
			return new RtfTextFormat( this );
		} // IRtfTextFormat.Duplicate

		// ----------------------------------------------------------------------
		public RtfTextFormat Duplicate()
		{
			return new RtfTextFormat( this );
		} // Duplicate

		// ----------------------------------------------------------------------
		public override bool Equals( object obj )
		{
			if ( obj == this )
			{
				return true;
			}
			
			if ( obj == null || GetType() != obj.GetType() )
			{
				return false;
			}

			return IsEqual( obj );
		} // Equals

		// ----------------------------------------------------------------------
		public override int GetHashCode()
		{
			return HashTool.AddHashCode( GetType().GetHashCode(), ComputeHashCode() );
		} // GetHashCode

		// ----------------------------------------------------------------------
		private bool IsEqual( object obj )
		{
			RtfTextFormat compare = obj as RtfTextFormat; // guaranteed to be non-null
			return
				compare != null &&
				this.font.Equals( compare.font ) &&
				this.fontSize == compare.fontSize &&
				this.superScript == compare.superScript &&
				this.bold == compare.bold &&
				this.italic == compare.italic &&
				this.underline == compare.underline &&
				this.strikeThrough == compare.strikeThrough &&
				this.hidden == compare.hidden &&
				this.backgroundColor.Equals( compare.backgroundColor ) &&
				this.foregroundColor.Equals( compare.foregroundColor ) &&
				this.alignment == compare.alignment;
		} // IsEqual

		// ----------------------------------------------------------------------
		private int ComputeHashCode()
		{
			int hash = this.font.GetHashCode();
			hash = HashTool.AddHashCode( hash, this.fontSize );
			hash = HashTool.AddHashCode( hash, this.superScript );
			hash = HashTool.AddHashCode( hash, this.bold );
			hash = HashTool.AddHashCode( hash, this.italic );
			hash = HashTool.AddHashCode( hash, this.underline );
			hash = HashTool.AddHashCode( hash, this.strikeThrough );
			hash = HashTool.AddHashCode( hash, this.hidden );
			hash = HashTool.AddHashCode( hash, this.backgroundColor );
			hash = HashTool.AddHashCode( hash, this.foregroundColor );
			hash = HashTool.AddHashCode( hash, this.alignment );
			return hash;
		} // ComputeHashCode

		// ----------------------------------------------------------------------
		public override string ToString()
		{
			StringBuilder buf = new StringBuilder( "Font " );
			buf.Append( FontDescriptionDebug );
			buf.Append( ", " );
			buf.Append( this.alignment );
			buf.Append( ", " );
			buf.Append( this.foregroundColor.ToString() );
			buf.Append( " on " );
			buf.Append( this.backgroundColor.ToString() );
			return buf.ToString();
		} // ToString

		// ----------------------------------------------------------------------
		// members
		private IRtfFont font;
		private int fontSize;
		private int superScript;
		private bool bold;
		private bool italic;
		private bool underline;
		private bool strikeThrough;
		private bool hidden;
		private IRtfColor backgroundColor = RtfColor.White;
		private IRtfColor foregroundColor = RtfColor.Black;
		private RtfTextAlignment alignment = RtfTextAlignment.Left;

	} // class RtfTextFormat

} // namespace Itenso.Rtf.Model
// -- EOF -------------------------------------------------------------------
