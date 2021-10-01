// EnImageList.cpp: implementation of the CEnImageList class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EnImageList.h"
#include "GraphicsMisc.h"
#include "EnBitmapEx.h"
#include "icon.h"
#include "misc.h"

//////////////////////////////////////////////////////////////////////

// Special value to ensure that images containing
// the real magenta are not affected
const COLORREF MAGENTA = RGB(255, 1, 255);

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEnImageList::CEnImageList()
{

}

CEnImageList::~CEnImageList()
{

}

int CEnImageList::GetImageSize() const
{
	return GetImageSize(*this);
}

BOOL CEnImageList::GetImageSize(int& nCx, int& nCy) const
{
	return GetImageSize(*this, nCx, nCy);
}

BOOL CEnImageList::GetImageSize(CSize& size) const
{
	return GetImageSize(*this, size);
}

int CEnImageList::GetImageWidth() const
{
	CSize size;

	if (!GetImageSize(size))
		return -1;

	return size.cx;
}

int CEnImageList::GetImageHeight() const
{
	CSize size;

	if (!GetImageSize(size))
		return -1;

	return size.cy;
}

BOOL CEnImageList::ScaleByDPIFactor(COLORREF crBkgnd)
{
	return ScaleByDPIFactor(*this, crBkgnd);
}

int CEnImageList::GetImageSize(HIMAGELIST hil)
{
	int nCx, nCy;
	
	if (!GetImageSize(hil, nCx, nCy))
		return -1;
	
	return max(nCx, nCy);
}

BOOL CEnImageList::GetImageSize(HIMAGELIST hil, int& nCx, int& nCy)
{
	return ImageList_GetIconSize(hil, &nCx, &nCy);
}

BOOL CEnImageList::GetImageSize(HIMAGELIST hil, CSize& size)
{
	return GetImageSize(hil, (int&)size.cx, (int&)size.cy);
}

BOOL CEnImageList::ScaleByDPIFactor(CImageList& il, COLORREF crBkgnd)
{
	if (!GraphicsMisc::WantDPIScaling())
		return TRUE;

	int nOldCx = 0, nOldCy = 0;

	if (!GetImageSize(il, nOldCx, nOldCy))
		return FALSE;
	
	int nNewCx = GraphicsMisc::ScaleByDPIFactor(nOldCx);
	int nNewCy = GraphicsMisc::ScaleByDPIFactor(nOldCy);
	
	if (nOldCx == nNewCx)
		return TRUE;
	
	int nCount = il.GetImageCount();
	
	CEnImageList ilTemp;
	
	if (!ilTemp.Create(nNewCx, nNewCy, ILC_COLOR32 | ILC_MASK, nCount, 1)) 
		return FALSE;
	
	for (int nImage = 0; nImage < nCount; nImage++)
	{
		ilTemp.Add(CIcon(il.ExtractIcon(nImage)), crBkgnd);
	}
	ASSERT(ilTemp.GetImageCount() == il.GetImageCount());
	
	il.DeleteImageList();
	il.Attach(ilTemp.Detach());
	
	return TRUE;
}

int CEnImageList::Add(HICON hIcon, COLORREF crBkgnd)
{
	return AddReplace(hIcon, crBkgnd, -1);
}

int CEnImageList::Replace(int nImage, HICON hIcon, COLORREF crBkgnd)
{
	return AddReplace(hIcon, crBkgnd, nImage);
}

int CEnImageList::AddReplace(HICON hIcon, COLORREF crBkgnd, int nImage)
{
	BOOL bDoDefault = ((crBkgnd == CLR_NONE) ||
					   !GraphicsMisc::WantDPIScaling() ||
					   (GraphicsMisc::GetIconSize(hIcon).cx >= GetImageSize()));

	// Our custom scaling below produces nicer images than 
	// the default imagelist scaling but introduces some
	// artifacts than look terrible on high contract schemes
	bDoDefault |= Misc::IsHighContrastActive();

	if (bDoDefault)
	{
		if (nImage == -1)
			return CImageList::Add(hIcon);

		// else
		return CImageList::Replace(nImage, hIcon);
	}
	
	CEnBitmapEx bmp;
	
	bmp.CopyImage(hIcon, crBkgnd);
	bmp.ResizeImage(GraphicsMisc::GetDPIScaleFactor());
	bmp.ReplaceColor(crBkgnd, MAGENTA);

	if (nImage == -1)
		return Add(&bmp, MAGENTA);

	// else
	return CImageList::Replace(nImage, CIcon(bmp.ExtractIcon(MAGENTA)));
}
