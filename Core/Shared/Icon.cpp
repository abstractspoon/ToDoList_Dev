#include "stdafx.h"
#include "Icon.h"
#include "GraphicsMisc.h"


CIcon::CIcon() 
	: 
	m_hIcon(NULL), 
	m_size(0, 0)
{
}

CIcon::CIcon(HICON hIcon) 
	: 
	m_hIcon(hIcon), 
	m_size(GraphicsMisc::GetIconSize(hIcon))
{
}

CIcon::CIcon(UINT nIDIcon, int nSize, BOOL bScaleByDPI) 
	: 
	m_hIcon(NULL), 
	m_size(0, 0)
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
	m_size = GraphicsMisc::GetIconSize(m_hIcon);

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
		m_size.SetSize(0, 0);
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
	m_size.SetSize(0, 0);

	return hIcon;
}

BOOL CIcon::Draw(CDC* pDC, const CPoint& ptTopLeft) const
{
	if (!m_hIcon)
		return FALSE;

	return ::DrawIconEx(*pDC, ptTopLeft.x, ptTopLeft.y, m_hIcon, m_size.cx, m_size.cy, 0, NULL, DI_NORMAL);
}