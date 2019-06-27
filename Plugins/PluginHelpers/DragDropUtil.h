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
			public ref class DragDropUtil
			{
				static bool IsOutlookObject(Microsoft::VisualStudio::OLE::Interop::IDataObject^ obj);
				static bool FormatAsOutlookUrl(Microsoft::VisualStudio::OLE::Interop::IDataObject^ obj, String^% url);

			protected:
				static bool ObjectHasFormat(Microsoft::VisualStudio::OLE::Interop::IDataObject^ obj, CLIPFORMAT cf);

			};
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////
