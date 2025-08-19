#include "stdafx.h"
#include "Icon.h"
#include "GraphicsMisc.h"


CIcon::CIcon() 
	: 
	m_hIcon(NULL), 
	m_size(0, 0),
	m_bOwned(FALSE)
{
}

CIcon::CIcon(HICON hIcon, BOOL bOwned)
	: 
	m_hIcon(hIcon), 
	m_size(GraphicsMisc::GetIconSize(hIcon)),
	m_bOwned(bOwned)
{
}

CIcon::CIcon(UINT nIDIcon, int nSize, BOOL bScaleByDPI) 
	: 
	m_hIcon(NULL), 
	m_size(0, 0),
	m_bOwned(FALSE)
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

void CIcon::Destroy()
{
	if (m_hIcon && m_bOwned)
		VERIFY(::DestroyIcon(m_hIcon));

	m_hIcon = NULL;
	m_size.cx = m_size.cy = 0;
	m_bOwned = FALSE;
}

BOOL CIcon::SetIcon(HICON hIcon, BOOL bOwned)
{
	Destroy();

	if (hIcon)
	{
		m_hIcon = hIcon;
		m_bOwned = bOwned;
		m_size = GraphicsMisc::GetIconSize(m_hIcon);
	}

	return TRUE;
}

BOOL CIcon::Load(UINT nIDIcon, int nSize, BOOL bScaleByDPI)
{
	if (bScaleByDPI)
		nSize = GraphicsMisc::ScaleByDPIFactor(nSize);

	HICON hIcon = GraphicsMisc::LoadIcon(nIDIcon, nSize);

	if (!hIcon)
	{
		ASSERT(0);
		return FALSE;
	}

	return SetIcon(hIcon, TRUE); // owned
}

BOOL CIcon::Attach(HICON hIcon, BOOL bOwned)
{
	if ((m_hIcon != NULL) || (hIcon == NULL))
	{
		ASSERT(0);
		return FALSE;
	}

	return SetIcon(hIcon, bOwned);
}

HICON CIcon::Detach()
{
	HICON hIcon = m_hIcon;

	m_hIcon = NULL;
	m_size.cx = m_size.cy = 0;
	m_bOwned = FALSE;

	return hIcon;
}

BOOL CIcon::Draw(CDC* pDC, const CPoint& ptTopLeft) const
{
	if (!m_hIcon)
		return FALSE;

	return ::DrawIconEx(*pDC, ptTopLeft.x, ptTopLeft.y, m_hIcon, m_size.cx, m_size.cy, 0, NULL, DI_NORMAL);
}