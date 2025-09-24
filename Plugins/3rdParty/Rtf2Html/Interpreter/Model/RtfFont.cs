// -- FILE ------------------------------------------------------------------
// name       : RtfFont.cs
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
	public sealed class RtfFont : IRtfFont
	{

		// ----------------------------------------------------------------------
		public RtfFont( string id, RtfFontKind kind, RtfFontPitch pitch, int charSet, int codePage, string name )
		{
			if ( id == null )
			{
				throw new ArgumentNullException( "id" );
			}
			if ( charSet < 0 )
			{
				throw new ArgumentException( Strings.InvalidCharacterSet( charSet ) );
			}
			if ( codePage < 0 )
			{
				throw new ArgumentException( Strings.InvalidCodePage( codePage ) );
			}
			if ( name == null )
			{
				throw new ArgumentNullException( "name" );
			}
			this.id = id;
			this.kind = kind;
			this.pitch = pitch;
			this.charSet = charSet;
			this.codePage = codePage;
			this.name = name;
		} // RtfFont

		// ----------------------------------------------------------------------
		public string Id
		{
			get { return this.id; }
		} // Id

		// ----------------------------------------------------------------------
		public RtfFontKind Kind
		{
			get { return this.kind; }
		} // Kind

		// ----------------------------------------------------------------------
		public RtfFontPitch Pitch
		{
			get { return this.pitch; }
		} // Pitch

		// ----------------------------------------------------------------------
		public int CharSet
		{
			get { return this.charSet; }
		} // CharSet

		// ----------------------------------------------------------------------
		public int CodePage
		{
			get
			{
				// if a codepage is specified, it overrides the charset setting
				if ( this.codePage == 0 )
				{
					// unspecified codepage: use the one derived from the charset:
					return RtfSpec.GetCodePage( this.charSet );
				}
				return this.codePage;
			}
		} // CodePage

		// ----------------------------------------------------------------------
		public Encoding GetEncoding()
		{
			return Encoding.GetEncoding( CodePage );
		} // GetEncoding

		// ----------------------------------------------------------------------
		public string Name
		{
			get { return this.name; }
		} // Name

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
		public override string ToString()
		{
			return this.id + ":" + this.name;
		} // ToString

		// ----------------------------------------------------------------------
		private bool IsEqual( object obj )
		{
			RtfFont compare = obj as RtfFont; // guaranteed to be non-null
			return
				compare != null &&
				this.id.Equals( compare.id ) &&
				this.kind == compare.kind &&
				this.pitch == compare.pitch &&
				this.charSet == compare.charSet &&
				this.codePage == compare.codePage &&
				this.name.Equals( compare.name );
		} // IsEqual

		// ----------------------------------------------------------------------
		private int ComputeHashCode()
		{
			int hash = this.id.GetHashCode();
			hash = HashTool.AddHashCode( hash, this.kind );
			hash = HashTool.AddHashCode( hash, this.pitch );
			hash = HashTool.AddHashCode( hash, this.charSet );
			hash = HashTool.AddHashCode( hash, this.codePage );
			hash = HashTool.AddHashCode( hash, this.name );
			return hash;
		} // ComputeHashCode

		// ----------------------------------------------------------------------
		// members
		private readonly string id;
		private readonly RtfFontKind kind;
		private readonly RtfFontPitch pitch;
		private readonly int charSet;
		private readonly int codePage;
		private readonly string name;

	} // class RtfFont

} // namespace Itenso.Rtf
// -- EOF -------------------------------------------------------------------
