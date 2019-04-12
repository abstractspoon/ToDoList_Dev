// -- FILE ------------------------------------------------------------------
// name       : IRtfInterpreterContext.cs
// project    : RTF Framelet
// created    : Leon Poyyayil - 2008.05.20
// language   : c#
// environment: .NET 2.0
// copyright  : (c) 2004-2009 by Itenso GmbH, Switzerland
// --------------------------------------------------------------------------

namespace Itenso.Rtf
{

	// ------------------------------------------------------------------------
	public interface IRtfInterpreterContext
	{

		// ----------------------------------------------------------------------
		RtfInterpreterState State { get; }

		// ----------------------------------------------------------------------
		int RtfVersion { get; }

		// ----------------------------------------------------------------------
		string DefaultFontId { get; }

		// ----------------------------------------------------------------------
		IRtfFont DefaultFont { get; }

		// ----------------------------------------------------------------------
		IRtfFontCollection FontTable { get; }

		// ----------------------------------------------------------------------
		IRtfColorCollection ColorTable { get; }

		// ----------------------------------------------------------------------
		string Generator { get; }

		// ----------------------------------------------------------------------
		IRtfTextFormatCollection UniqueTextFormats { get; }

		// ----------------------------------------------------------------------
		IRtfTextFormat CurrentTextFormat { get; }

		// ----------------------------------------------------------------------
		IRtfTextFormat GetSafeCurrentTextFormat();

		// ----------------------------------------------------------------------
		IRtfDocumentInfo DocumentInfo { get; }

		// ----------------------------------------------------------------------
		IRtfDocumentPropertyCollection UserProperties { get; }

	} // interface IRtfInterpreterContext

} // namespace Itenso.Rtf
// -- EOF -------------------------------------------------------------------
