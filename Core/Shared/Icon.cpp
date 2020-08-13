#include "stdafx.h"
#include "Icon.h"
#include "GraphicsMisc.h"


CIcon::CIcon() : m_hIcon(NULL)
{
}

CIcon::CIcon(HICON hIcon) : m_hIcon(hIcon)
{
}

CIcon::CIcon(UINT nIDIcon, int nSize, BOOL bScaleByDPI) : m_hIcon(NULL)
{
	Load(nIDIcon, nSize, bScaleByDPI);
}

CIcon::~CIcon()
{
	Destroy();
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
		VERIFY(::DestroyIcon(m_hIcon));

	m_hIcon = hIcon;
	return TRUE;
}

BOOL CIcon::Load(UINT nIDIcon, int nSize, BOOL bScaleByDPI)
{
	if (bScaleByDPI)
		nSize = GraphicsMisc::ScaleByDPIFactor(nSize);

	return SetIcon(GraphicsMisc::LoadIcon(nIDIcon, nSize));
}

void CIcon::Destroy()
{
	if (m_hIcon)
	{
		VERIFY(::DestroyIcon(m_hIcon));
		m_hIcon = NULL;
	}
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

