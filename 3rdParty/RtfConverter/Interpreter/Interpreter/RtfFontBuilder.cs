// -- FILE ------------------------------------------------------------------
// name       : RtfFontBuilder.cs
// project    : RTF Framelet
// created    : Leon Poyyayil - 2008.05.21
// language   : c#
// environment: .NET 2.0
// copyright  : (c) 2004-2009 by Itenso GmbH, Switzerland
// --------------------------------------------------------------------------
using System.Text;
using Itenso.Rtf.Model;
using Itenso.Rtf.Support;

namespace Itenso.Rtf.Interpreter
{

	// ------------------------------------------------------------------------
	public sealed class RtfFontBuilder : RtfElementVisitorBase
	{

		// ----------------------------------------------------------------------
		public RtfFontBuilder()
			: base( RtfElementVisitorOrder.NonRecursive )
		{
			// we iterate over our children ourselves -> hence non-recursive
			Reset();
		} // RtfFontBuilder

		// ----------------------------------------------------------------------
		public string FontId
		{
			get { return this.fontId; }
		} // FontId

		// ----------------------------------------------------------------------
		public int FontIndex
		{
			get { return this.fontIndex; }
		} // FontIndex

		// ----------------------------------------------------------------------
		public int FontCharset
		{
			get { return this.fontCharset; }
		} // FontCharset

		// ----------------------------------------------------------------------
		public int FontCodePage
		{
			get { return this.fontCodePage; }
		} // FontCodePage

		// ----------------------------------------------------------------------
		public RtfFontKind FontKind
		{
			get { return this.fontKind; }
		} // FontKind

		// ----------------------------------------------------------------------
		public RtfFontPitch FontPitch
		{
			get { return this.fontPitch; }
		} // FontPitch

		// ----------------------------------------------------------------------
		public string FontName
		{
			get
			{
				string fontName = null;
				int len = this.fontNameBuffer.Length;
				if ( len > 0 && this.fontNameBuffer[ len - 1 ] == ';' )
				{
					fontName = this.fontNameBuffer.ToString().Substring( 0, len - 1 ).Trim();
					if ( fontName.Length == 0 )
					{
						fontName = null;
					}
				}
				return fontName;
			}
		} // FontName

		// ----------------------------------------------------------------------
		public IRtfFont CreateFont()
		{
			return new RtfFont( this.fontId, this.fontKind, this.fontPitch,
				this.fontCharset, this.fontCodePage, this.FontName );
		} // CreateFont

		// ----------------------------------------------------------------------
		public void Reset()
		{
			this.fontIndex = 0;
			this.fontCharset = 0;
			this.fontCodePage = 0;
			this.fontKind = RtfFontKind.Nil;
			this.fontPitch = RtfFontPitch.Default;
			this.fontNameBuffer.Remove( 0, this.fontNameBuffer.Length );
		} // Reset

		// ----------------------------------------------------------------------
		protected override void DoVisitGroup( IRtfGroup group )
		{
			switch ( group.Destination )
			{
				case RtfSpec.TagFont:
				case RtfSpec.TagThemeFontLoMajor:
				case RtfSpec.TagThemeFontHiMajor:
				case RtfSpec.TagThemeFontDbMajor:
				case RtfSpec.TagThemeFontBiMajor:
				case RtfSpec.TagThemeFontLoMinor:
				case RtfSpec.TagThemeFontHiMinor:
				case RtfSpec.TagThemeFontDbMinor:
				case RtfSpec.TagThemeFontBiMinor:
					VisitGroupChildren( group );
					break;
			}
		} // DoVisitGroup

		// ----------------------------------------------------------------------
		protected override void DoVisitTag( IRtfTag tag )
		{
			switch ( tag.Name )
			{
				case RtfSpec.TagThemeFontLoMajor:
				case RtfSpec.TagThemeFontHiMajor:
				case RtfSpec.TagThemeFontDbMajor:
				case RtfSpec.TagThemeFontBiMajor:
				case RtfSpec.TagThemeFontLoMinor:
				case RtfSpec.TagThemeFontHiMinor:
				case RtfSpec.TagThemeFontDbMinor:
				case RtfSpec.TagThemeFontBiMinor:
					// skip and ignore for the moment
					break;
				case RtfSpec.TagFont:
					this.fontId = tag.FullName;
					this.fontIndex = tag.ValueAsNumber;
					break;
				case RtfSpec.TagFontKindNil:
					this.fontKind = RtfFontKind.Nil;
					break;
				case RtfSpec.TagFontKindRoman:
					this.fontKind = RtfFontKind.Roman;
					break;
				case RtfSpec.TagFontKindSwiss:
					this.fontKind = RtfFontKind.Swiss;
					break;
				case RtfSpec.TagFontKindModern:
					this.fontKind = RtfFontKind.Modern;
					break;
				case RtfSpec.TagFontKindScript:
					this.fontKind = RtfFontKind.Script;
					break;
				case RtfSpec.TagFontKindDecor:
					this.fontKind = RtfFontKind.Decor;
					break;
				case RtfSpec.TagFontKindTech:
					this.fontKind = RtfFontKind.Tech;
					break;
				case RtfSpec.TagFontKindBidi:
					this.fontKind = RtfFontKind.Bidi;
					break;
				case RtfSpec.TagFontCharset:
					this.fontCharset = tag.ValueAsNumber;
					break;
				case RtfSpec.TagCodePage:
					this.fontCodePage = tag.ValueAsNumber;
					break;
				case RtfSpec.TagFontPitch:
					switch ( tag.ValueAsNumber )
					{
						case 0:
							this.fontPitch = RtfFontPitch.Default;
							break;
						case 1:
							this.fontPitch = RtfFontPitch.Fixed;
							break;
						case 2:
							this.fontPitch = RtfFontPitch.Variable;
							break;
					}
					break;
			}
		} // DoVisitTag

		// ----------------------------------------------------------------------
		protected override void DoVisitText( IRtfText text )
		{
			this.fontNameBuffer.Append( text.Text );
		} // DoVisitText

		// ----------------------------------------------------------------------
		// members
		private string fontId;
		private int fontIndex;
		private int fontCharset;
		private int fontCodePage;
		private RtfFontKind fontKind;
		private RtfFontPitch fontPitch;
		private readonly StringBuilder fontNameBuffer = new StringBuilder();

	} // class RtfFontBuilder

} // namespace Itenso.Rtf.Interpreter
// -- EOF -------------------------------------------------------------------
