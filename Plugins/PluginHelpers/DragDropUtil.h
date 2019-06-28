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
				static UInt32 DRAGDROP_NONE		= 0;
				static UInt32 DRAGDROP_COPY		= 1;
				static UInt32 DRAGDROP_MOVE		= 2;
				static UInt32 DRAGDROP_LINK		= 4;

				static UInt32 DRAGDROP_LBUTTON	= 0x01;
				static UInt32 DRAGDROP_RBUTTON	= 0x02;
				static UInt32 DRAGDROP_SHIFT	= 0x04;
				static UInt32 DRAGDROP_CONTROL	= 0x08;
				static UInt32 DRAGDROP_MBUTTON	= 0x10;
				static UInt32 DRAGDROP_XBUTTON1	= 0x20;
				static UInt32 DRAGDROP_XBUTTON2	= 0x40;

			};
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////
