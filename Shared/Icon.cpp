#include "stdafx.h"
#include "Icon.h"


CIcon::CIcon(HICON hIcon) : m_hIcon(hIcon)
{
}

CIcon::CIcon(UINT nIDIcon, int nSize) : m_hIcon(NULL)
{
	LoadIcon(nIDIcon, nSize);
}

CIcon::~CIcon()
{
	if (m_hIcon)
		::DestroyIcon(m_hIcon);
}

BOOL CIcon::IsValid() const 
{ 
	return (m_hIcon != NULL); 
}

BOOL CIcon::SetIcon(HICON hIcon, BOOL bDeletePrev)
{
	if (hIcon == NULL)
	{
		ASSERT(0);
		return FALSE;
	}

	if (bDeletePrev && (m_hIcon != NULL))
		::DestroyIcon(m_hIcon);

	m_hIcon = hIcon;
	return TRUE;
}

BOOL CIcon::LoadIcon(UINT nIDIcon, int nSize)
{
	HICON hIcon = (HICON)::LoadImage(AfxGetResourceHandle(), MAKEINTRESOURCE(nIDIcon), 
									IMAGE_ICON, nSize, nSize, LR_LOADMAP3DCOLORS);

	return SetIcon(hIcon);
}

BOOL CIcon::Attach(HICON hIcon)
{
	if ((m_hIcon != NULL) || (hIcon == NULL))
	{
		ASSERT(0);
		return FALSE;
	}

	return SetIcon(hIcon);
}

HICON CIcon::Detach()
{
	HICON hIcon = m_hIcon;
	m_hIcon = NULL;

	return hIcon;
}

