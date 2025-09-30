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
			// The purpose of this class is to properly handle the 
			// conversion of UTF8 Html (as it is stored on the clipboard)
			// to Unicode. 
			public ref class DataObjectEx : Windows::Forms::IDataObject
			{
			public:
				// For clipboard operations 
				// ie. where one would normally use Clipboard::GetDataObject()
				DataObjectEx(); 

				// For drag and drop operations
				DataObjectEx(Windows::Forms::IDataObject^ obj); 

				virtual Object^ GetData(String^ format);
				virtual Object^ GetData(Type^ format);
				virtual Object^ GetData(String^ format, bool autoConvert);
				virtual bool GetDataPresent(String^ format);
				virtual bool GetDataPresent(Type^ format);
				virtual bool GetDataPresent(String^ format, bool autoConvert);
				virtual cli::array<String^>^ GetFormats();
				virtual cli::array<String^>^ GetFormats(bool autoConvert);
				virtual void SetData(Object^ data);
				virtual void SetData(String^ format, Object^ data);
				virtual void SetData(Type^ format, Object^ data);
				virtual void SetData(String^ format, bool autoConvert, Object^ data);

			private:
				Windows::Forms::IDataObject^ m_Obj;
			};

			// ----------------------------------------------------------------------

			public ref class OleDataObjectEx
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
				static bool GetHtmlFragment(String^% html); 
				static bool GetHtmlFragment(String^% html, String^% sourceUrl);

				static bool GetHtmlFragment(Windows::Forms::IDataObject^ obj, String^% html);
				static bool GetHtmlFragment(Windows::Forms::IDataObject^ obj, String^% html, String^% sourceUrl);

				static bool IsDropFile(Microsoft::VisualStudio::OLE::Interop::IDataObject^ obj);
				static cli::array<String^>^ GetDropFiles(Microsoft::VisualStudio::OLE::Interop::IDataObject^ obj);

			private:
				static Windows::Forms::IDataObject^ GetDataObject(Windows::Forms::IDataObject^ obj);
			};
		}
	}
}

