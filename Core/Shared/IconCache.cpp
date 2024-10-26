// ImageCache.cpp: implementation of the CImageCache class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IconCache.h"
#include "Icon.h"
#include "enbitmapex.h"
#include "GraphicsMisc.h"
#include "FileMisc.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CIconCache::CIconCache(BOOL bLargeIcons) 
	: 
	m_nIconSize(GraphicsMisc::ScaleByDPIFactor(bLargeIcons ? 32 : 16))
{
}

CIconCache::~CIconCache()
{
	Clear();
}

BOOL CIconCache::Add(const CString& sName, HBITMAP hbm, COLORREF crMask)
{
	if (IsValidName(sName) && hbm)
	{
		HICON hIcon = CEnBitmap::ExtractIcon(hbm, crMask, m_nIconSize, m_nIconSize);
		ASSERT(hIcon);

		return Add(sName, hIcon, FALSE);
	}
	
	return FALSE;
}

BOOL CIconCache::Add(const CString& sName, UINT nIconID)
{
	if (IsValidName(sName) && nIconID)
	{
		HICON hIcon = GraphicsMisc::LoadIcon(nIconID, m_nIconSize);
		ASSERT(hIcon);

		return Add(sName, hIcon, FALSE);
	}

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

	return FALSE;
}

BOOL CIconCache::Add(const CString& sName, const CString& sImagePath, COLORREF crBack)
{
	if (IsValidName(sName) && !sImagePath.IsEmpty())
	{
		HICON hIcon = CEnBitmap::LoadImageFileAsIcon(sImagePath, crBack, m_nIconSize, m_nIconSize);
		ASSERT(hIcon || !FileMisc::PathExists(sImagePath));

		return Add(sName, hIcon, FALSE);
	}
	
	return FALSE;
}

BOOL CIconCache::Remove(const CString& sName)
{
	HICON hIcon = GetIcon(sName);
	
	if (hIcon)
	{
		VERIFY(m_mapIcons.RemoveKey(sName));
		::DestroyIcon(hIcon);

		return TRUE;
	}
	
	return FALSE;
}

BOOL CIconCache::HasIcon(const CString& sName) const
{
	return (GetIcon(sName) != NULL);
}

HICON CIconCache::GetIcon(const CString& sName) const
{
	HICON hIcon = NULL;
	m_mapIcons.Lookup(sName, hIcon);

	return hIcon;
}

HICON CIconCache::GetIcon(const CString& sName, BOOL bDisabled)
{
	HICON hIcon = GetIcon(sName);

	if (!bDisabled || !hIcon)
		return hIcon;

	CString sDisName = (sName + _T("__disabled__"));
	HICON hDisIcon = GetIcon(sDisName);

	if (!hDisIcon)
	{
		hDisIcon = CEnBitmapEx::CreateDisabledIcon(hIcon);
		VERIFY(Add(sDisName, hDisIcon, FALSE));
	}

	return GetIcon(sDisName);
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
		return ::DrawIconEx(*pDC, pt.x, pt.y, hIcon, m_nIconSize, m_nIconSize, 0, NULL, DI_NORMAL);

	return FALSE;
}

