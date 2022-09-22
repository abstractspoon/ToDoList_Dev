
#include "stdafx.h"
#include "PluginHelpers.h"
#include "DragDropUtil.h"
#include "Win32.h"

////////////////////////////////////////////////////////////////////////////////////////////////

using namespace Windows::Forms;
using namespace Drawing;
using namespace Abstractspoon::Tdl::PluginHelpers;

////////////////////////////////////////////////////////////////////////////////////////////////

bool DragDropUtil::ObjectHasFormat(Microsoft::VisualStudio::OLE::Interop::IDataObject^ obj, CLIPFORMAT cf)
{
	if (obj == nullptr)
		return false;

	// From _AfxFillFormatEtc()
	auto formatEtc = gcnew cli::array<Microsoft::VisualStudio::OLE::Interop::FORMATETC>(1);

	formatEtc[0].cfFormat = cf;
	formatEtc[0].ptd = IntPtr::Zero;
	formatEtc[0].dwAspect = DVASPECT_CONTENT;
	formatEtc[0].lindex = -1;
	formatEtc[0].tymed = (DWORD)-1;
	
	// attempt to get the data
	return (obj->QueryGetData(formatEtc) == S_OK);
}

// ----------------------------------------------------------------------------

DragImage::DragImage()
	:
	m_hImageList(NULL),
	m_hwndLock(NULL)
{ 
}

DragImage::~DragImage() 
{ 
	End(); 
}

bool DragImage::Begin(IntPtr wnd, Font^ font, String^ text, int width, int height, int hotX, int hotY)
{
	Bitmap^ bm = gcnew Bitmap(width, height);
	Graphics^ g = Graphics::FromImage(bm);

	g->FillRectangle(SystemBrushes::Highlight, 0, 0, width, height);
	g->DrawString(text, font, SystemBrushes::HighlightText, RectangleF(0, 0, (float)width, (float)height));

	return Begin(wnd, bm, width, height, hotX, hotY);
}

bool DragImage::Begin(IntPtr wnd, IDragRenderer^ renderer, Object^ object, int width, int height, int hotX, int hotY)
{
	if (m_hImageList != NULL)
		return false;

	Bitmap^ bm = gcnew Bitmap(width, height);
	Graphics^ g = Graphics::FromImage(bm);

	renderer->DrawDragImage(g, object, width, height);

	return Begin(wnd, bm, width, height, hotX, hotY);
}

bool DragImage::Begin(IntPtr wnd, Bitmap^ bm, int width, int height, int hotX, int hotY)
{
	if (m_hImageList != NULL)
		return false;

	m_hwndLock = Win32::GetHwnd(wnd);
	m_hImageList = ImageList_Create(width, height, ILC_COLOR32, 0, 1);

	if (m_hImageList != NULL)
	{
		HBITMAP hbm = Win32::GetHBitmap(bm->GetHbitmap());

		ImageList_Add(m_hImageList, hbm, NULL);
		ImageList_BeginDrag(m_hImageList, 0, hotX, hotY);

		CRect rWindow;
		GetWindowRect(m_hwndLock, rWindow);

		CPoint ptCursor(::GetMessagePos());
		ImageList_DragEnter(m_hwndLock, ptCursor.x - rWindow.left, ptCursor.y - rWindow.top);

		DeleteObject(hbm);
	}

	return true;
}

bool DragImage::Move(int xScreen, int yScreen)
{
	if (m_hImageList == NULL)
		return false;

	CRect rWindow;
	GetWindowRect(m_hwndLock, rWindow);

	return (ImageList_DragMove(xScreen - rWindow.left, yScreen - rWindow.top) != FALSE);
}

bool DragImage::End()
{
	if (m_hImageList == NULL)
		return false;

	ImageList_EndDrag();
	ImageList_DragLeave(m_hwndLock);
	ImageList_Destroy(m_hImageList);

	m_hImageList = NULL;
	m_hwndLock = NULL;

	return true;
}

bool DragImage::ShowNoLock(bool show)
{
	if (m_hImageList == NULL)
		return false;

	ImageList_DragShowNolock(show);
	return true;
}
