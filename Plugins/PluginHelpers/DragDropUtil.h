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
			public:
				static bool ObjectHasFormat(Microsoft::VisualStudio::OLE::Interop::IDataObject^ obj, CLIPFORMAT cf);

			public:
				static UInt32 DRAGDROP_NONE = 0;
				static UInt32 DRAGDROP_COPY = 1;
				static UInt32 DRAGDROP_MOVE = 2;
				static UInt32 DRAGDROP_LINK = 4;
			};
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////
