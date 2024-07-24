// ImageCache.cpp: implementation of the CImageCache class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IconCache.h"
#include "Icon.h"
#include "enbitmap.h"
#include "GraphicsMisc.h"
#include "FileMisc.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CIconCache::CIconCache(BOOL bLargeIcons) : m_bLargeIcons(bLargeIcons)
{
}

CIconCache::~CIconCache()
{
	Clear();
}

int CIconCache::GetIconSize(BOOL bScaledByDPI) const
{
	int nSize = (m_bLargeIcons ? 32 : 16);

	if (bScaledByDPI)
		nSize = GraphicsMisc::ScaleByDPIFactor(nSize);

	return nSize;
}

BOOL CIconCache::Add(const CString& sName, HBITMAP hbm, COLORREF crMask)
{
	if (IsValidName(sName) && hbm)
	{
		int nReqSize = GetIconSize();

		HICON hIcon = CEnBitmap::ExtractIcon(hbm, crMask, nReqSize, nReqSize);
		ASSERT(hIcon);

		return Add(sName, hIcon, FALSE);
	}
	
	ASSERT(0);
	return FALSE;
}

BOOL CIconCache::Add(const CString& sName, UINT nIconID)
{
	if (IsValidName(sName) && nIconID)
	{
		HICON hIcon = GraphicsMisc::LoadIcon(nIconID, GetIconSize(FALSE));
		ASSERT(hIcon);

		return Add(sName, hIcon, FALSE);
	}

	ASSERT(0);
	return FALSE;
}

BOOL CIconCache::Add(const CString& sName, HICON hIcon)
{
	return Add(sName, hIcon, TRUE);
}

BOOL CIconCache::Add(const CString& sName, HICON hIcon, BOOL bCopy)
{
	if (IsValidName(sName) && hIcon)
	{
		if (bCopy)
			hIcon = ::CopyIcon(hIcon);

		Remove(sName);
		m_mapIcons[sName] = hIcon;

		return TRUE;
	}

	ASSERT(0);
	return FALSE;
}

BOOL CIconCache::Add(const CString& sName, const CString& sImagePath, COLORREF crBack)
{
	if (IsValidName(sName) && !sImagePath.IsEmpty())
	{
		int nReqSize = GetIconSize();

		HICON hIcon = CEnBitmap::LoadImageFileAsIcon(sImagePath, crBack, nReqSize, nReqSize);
		ASSERT(hIcon || !FileMisc::PathExists(sImagePath));

		return Add(sName, hIcon, FALSE);
	}
	
	ASSERT(0);
	return FALSE;
}

BOOL CIconCache::Remove(const CString& sName)
{
	HICON hIcon = NULL;
	
	if (m_mapIcons.Lookup(sName, hIcon))
	{
		VERIFY(m_mapIcons.RemoveKey(sName));
		::DestroyIcon(hIcon);

		return TRUE;
	}
	
	return FALSE;
}

BOOL CIconCache::HasIcon(const CString& sName) const
{
	HICON hUnused = NULL;
	
	return m_mapIcons.Lookup(sName, hUnused);
}

BOOL CIconCache::IsValidName(const CString& sName) const
{
	return (!sName.IsEmpty() && !HasIcon(sName));
}

void CIconCache::Clear()
{
	POSITION pos = m_mapIcons.GetStartPosition();
	CString sName;
	HICON hIcon = NULL;

	while (pos)
	{
		m_mapIcons.GetNextAssoc(pos, sName, hIcon);
		::DestroyIcon(hIcon);
	}

	m_mapIcons.RemoveAll();
}

BOOL CIconCache::Draw(CDC* pDC, const CString& sName, POINT pt, UINT nStyle)
{
	HICON hIcon = NULL;

	if (m_mapIcons.Lookup(sName, hIcon))
	{
		int nReqSize = GetIconSize();
		return ::DrawIconEx(*pDC, pt.x, pt.y, hIcon, nReqSize, nReqSize, 0, NULL, DI_NORMAL);
	}

	return FALSE;
}

