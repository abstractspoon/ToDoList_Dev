#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////

using namespace System;

////////////////////////////////////////////////////////////////////////////////////////////////

namespace Abstractspoon
{
	namespace Tdl
	{
		namespace PluginHelpers
		{
			// private class
			ref class OleDataObjectEx
			{
			public:
				OleDataObjectEx(Windows::Forms::IDataObject^ obj);
				OleDataObjectEx(Microsoft::VisualStudio::OLE::Interop::IDataObject^ obj);

				~OleDataObjectEx();

				bool IsValid() { return (m_pData != nullptr); }
				::IDataObject* Data() { return m_pData; }

			private:
				::IUnknown* m_pUnk;
				::IDataObject* m_pData;
			};

			// ----------------------------------------------------------------------

			public ref class ClipboardUtil
			{
			public:
				static bool IsDropFile(Microsoft::VisualStudio::OLE::Interop::IDataObject^ obj);
				static cli::array<String^>^ GetDropFiles(Microsoft::VisualStudio::OLE::Interop::IDataObject^ obj);

				static bool IsRtf(Microsoft::VisualStudio::OLE::Interop::IDataObject^ obj);
				static String^ GetRtf(Microsoft::VisualStudio::OLE::Interop::IDataObject^ obj);

				static bool IsHtml(Microsoft::VisualStudio::OLE::Interop::IDataObject^ obj);

				static String^ GetHtml();
				static String^ GetHtml(Microsoft::VisualStudio::OLE::Interop::IDataObject^ obj);

				static bool GetHtmlFragment(String^% html);
				static bool GetHtmlFragment(String^% html, String^% sourceUrl);

				static bool GetHtmlFragment(Windows::Forms::IDataObject^ obj, String^% html);
				static bool GetHtmlFragment(Windows::Forms::IDataObject^ obj, String^% html, String^% sourceUrl);

			private:
				static String^ GetHtml(OleDataObjectEx^ objEx);
			};
		}
	}
}

