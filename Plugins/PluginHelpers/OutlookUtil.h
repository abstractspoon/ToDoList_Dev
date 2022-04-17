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
			public ref class OutlookUtil
			{
			public:
				static bool IsOutlookItem(Microsoft::VisualStudio::OLE::Interop::IDataObject^ obj);
				static bool HandleUrl(String^ url, IntPtr hwnd);
				static bool IsOutlookUrl(String^ url);
				static bool IsOutlookInstalled();

 				static String^ FormatItemAsUrl(String^ id);
			};
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////
