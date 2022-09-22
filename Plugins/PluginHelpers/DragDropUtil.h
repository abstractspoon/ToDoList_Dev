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

			// ----------------------------------------------------------------------------

			public interface class IDragRenderer
			{
			public:
				virtual void DrawDragImage(Drawing::Graphics^ graphics, Object^ object, int width, int height) = 0;
			};

			// ----------------------------------------------------------------------------

			public ref class DragImage
			{
			public:
				DragImage();
				~DragImage();

				bool Begin(IntPtr wnd, Drawing::Font^ font, String^ text, int width, int height, int hotX, int hotY);
				bool Begin(IntPtr wnd, IDragRenderer^ renderer, Object^ object, int width, int height, int hotX, int hotY);
				bool Move(int x, int y);
				bool End();
				bool ShowNoLock(bool show);

			protected:
				HIMAGELIST m_hImageList;
				HWND m_hwndLock;

			protected:
				bool Begin(IntPtr wnd, Drawing::Bitmap^ bm, int width, int height, int hotX, int hotY);
			};
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////
