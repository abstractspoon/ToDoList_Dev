// ImageCache.cpp: implementation of the CImageCache class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IconCache.h"
#include "Icon.h"
#include "enbitmap.h"
#include "GraphicsMisc.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CIconCache::CIconCache(int cx, int cy) : m_sizeIcon(max(cx, 16), max(cy, 16))
{
	GraphicsMisc::ScaleByDPIFactor(&m_sizeIcon);
}

CIconCache::~CIconCache()
{
	Clear();
}

BOOL CIconCache::Add(const CString& sName, HBITMAP hbm, COLORREF crMask)
{
	if (IsValidName(sName) && hbm)
	{
		CIcon icon(CEnBitmap::ExtractIcon(hbm, crMask, m_sizeIcon.cx, m_sizeIcon.cy));
		ASSERT(icon.IsValid());

		return Add(sName, icon);
	}
	
	ASSERT(0);
	return FALSE;
}

BOOL CIconCache::Add(const CString& sName, CBitmap& bmp, COLORREF crMask)
{
	return Add(sName, (HBITMAP)bmp.GetSafeHandle());
}

BOOL CIconCache::Add(const CString& sName, HICON hIcon)
{
	if (IsValidName(sName) && hIcon)
	{
		// create image list first time only
		if (m_ilImages.GetSafeHandle() ||
			m_ilImages.Create(m_sizeIcon.cx, m_sizeIcon.cy, ILC_COLOR32 | ILC_MASK, 0, 1))
		{
			int nImage = m_ilImages.Add(hIcon);

			if (nImage >= 0)
			{
				m_mapIndices[sName] = nImage;
				return TRUE;
			}
		}
	}

	ASSERT(0);
	return FALSE;
}

BOOL CIconCache::Add(const CString& sName, const CString& sImagePath, COLORREF crBack)
{
	if (IsValidName(sName) && !sImagePath.IsEmpty())
	{
		CIcon icon(CEnBitmap::LoadImageFileAsIcon(sImagePath, crBack, m_sizeIcon.cx, m_sizeIcon.cy));
		ASSERT(icon.IsValid());

		return Add(sName, icon);
	}
	
	ASSERT(0);
	return FALSE;
}

BOOL CIconCache::Remove(const CString& sName)
{
	int nImage = -1;
	
	if (m_mapIndices.Lookup(sName, nImage))
	{
		VERIFY(m_ilImages.Remove(nImage));
		VERIFY(m_mapIndices.RemoveKey(sName));

		// Decrement the indices of all images coming after 'nImage'
		POSITION pos = m_mapIndices.GetStartPosition();
		
		while (pos)
		{
			CString sName;
			int nOther = -1;
			
			m_mapIndices.GetNextAssoc(pos, sName, nOther);

			if (nOther > nImage)
				m_mapIndices[sName] = --nOther;
		}

		return TRUE;
	}
	
	ASSERT(0);
	return FALSE;
}

BOOL CIconCache::HasIcon(const CString& sName) const
{
	int nImage = -1;
	
	return m_mapIndices.Lookup(sName, nImage);
}

BOOL CIconCache::IsValidName(const CString& sName) const
{
	return (!sName.IsEmpty() && !HasIcon(sName));
}

void CIconCache::Clear()
{
	if (m_ilImages.GetSafeHandle())
		while (m_ilImages.Remove(0));

	m_mapIndices.RemoveAll();
}

BOOL CIconCache::Draw(CDC* pDC, const CString& sName, POINT pt, UINT nStyle)
{
	int nImage = -1;

	if (m_mapIndices.Lookup(sName, nImage) && (nImage != -1))
	{
		return m_ilImages.Draw(pDC, nImage, pt, nStyle);
	}
	
	return FALSE;
}

