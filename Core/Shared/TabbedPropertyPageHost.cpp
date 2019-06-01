// TabbedPropertyPageHost.cpp: implementation of the CTabbedPropertyPageHost class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TabbedPropertyPageHost.h"
#include "misc.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

const UINT IDC_TABCTRL = 1001;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTabbedPropertyPageHost::CTabbedPropertyPageHost(DWORD dwFlags, ETabOrientation orientation) : 
	m_tabCtrl(dwFlags, orientation), 
	m_pImages(NULL)
{

}

CTabbedPropertyPageHost::~CTabbedPropertyPageHost()
{

}

BEGIN_MESSAGE_MAP(CTabbedPropertyPageHost, CPropertyPageHost)
	//{{AFX_MSG_MAP(CPropertyPageHost)
	//}}AFX_MSG_MAP
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TABCTRL, OnSelchangeTabcontrol)
END_MESSAGE_MAP()

int CTabbedPropertyPageHost::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CPropertyPageHost::OnCreate(lpCreateStruct) == -1)
		return -1;

	// construct tab control if there's more than one page
	if (!ConstructTabControl())
		return -1;
	
	return 0;
}

void CTabbedPropertyPageHost::SetImageList(CImageList* pImages)
{
	if (m_tabCtrl.GetSafeHwnd())
		m_tabCtrl.SetImageList(pImages);
	else
		m_pImages = pImages;
}

void CTabbedPropertyPageHost::SetBackgroundColor(COLORREF color)
{
	m_tabCtrl.SetBackgroundColor(color);
}

BOOL CTabbedPropertyPageHost::ConstructTabControl()
{
	ASSERT(GetSafeHwnd());

	if (m_tabCtrl.GetSafeHwnd() || GetPageCount() <= 1)
		return TRUE;

	DWORD dwStyles = WS_CHILD | WS_VISIBLE;

	switch (m_tabCtrl.GetOrientation())
	{
		case e_tabTop:	  
			break; // do nothing

		case e_tabBottom: 
			dwStyles |= TCS_BOTTOM;
			break;

		case e_tabLeft:	  
			dwStyles |= TCS_VERTICAL;
			break;

		case e_tabRight:  
			dwStyles |= TCS_RIGHT | TCS_VERTICAL;
			break;

		default:
			ASSERT(0);
	}


	if (!m_tabCtrl.Create(dwStyles, CRect(0, 0, 0, 0), this, IDC_TABCTRL))
		return FALSE;

	if (m_pImages)
	{
		m_tabCtrl.SetImageList(m_pImages);
		m_pImages = NULL;
	}

	for (int nPage = 0; nPage < GetPageCount(); nPage++)
	{
		if (-1 == m_tabCtrl.InsertItem(TCIF_TEXT | TCIF_PARAM, nPage, GetPageTitle(nPage), -1, GetPageItemData(nPage), 0, 0))
		{
			ASSERT(0);
			m_tabCtrl.DestroyWindow();
			return FALSE;
		}
	}

	return TRUE;
}

BOOL CTabbedPropertyPageHost::AddPage(CPropertyPage* pPage, LPCTSTR szTitle, DWORD dwItemData)
{
	if (Misc::IsEmpty(szTitle))
	{
		ASSERT(0);
		return FALSE;
	}

	if (GetSafeHwnd() && !ConstructTabControl())
		return FALSE;

	if (!CPropertyPageHost::AddPage(pPage, szTitle, dwItemData))
		return FALSE;

	if (m_tabCtrl.GetSafeHwnd())
		return m_tabCtrl.InsertItem(TCIF_TEXT | TCIF_PARAM, GetPageCount(), szTitle, -1, dwItemData, 0, 0);

	// else
	return TRUE;
}

BOOL CTabbedPropertyPageHost::SetActivePage(int nIndex, BOOL bAndFocus)
{
	if (CPropertyPageHost::SetActivePage(nIndex, bAndFocus))
	{
		// resize new page
		CRect rTabs, rPages;

		if (CalcTabPageRects(0, 0, rTabs, rPages))
			GetActivePage()->MoveWindow(rPages);

		if (nIndex != m_tabCtrl.GetCurSel())
			m_tabCtrl.SetCurSel(nIndex);

		return TRUE;
	}

	// else
	return FALSE;
}

BOOL CTabbedPropertyPageHost::SetActivePage(CPropertyPage* pPage, BOOL bAndFocus)
{
	return SetActivePage(FindPage(pPage), bAndFocus);
}

void CTabbedPropertyPageHost::OnSelchangeTabcontrol(NMHDR* /*pNMHDR*/, LRESULT* pResult) 
{
	SetActivePage(m_tabCtrl.GetCurSel());

	*pResult = 0;
}

void CTabbedPropertyPageHost::OnSize(UINT nType, int cx, int cy)
{
	CRect rTabs, rPages;

	if (!CalcTabPageRects(cx, cy, rTabs, rPages))
	{
		CPropertyPageHost::OnSize(nType, cx, cy);
		return;
	}

	m_tabCtrl.MoveWindow(rTabs);

	if (GetActivePage())
		GetActivePage()->MoveWindow(rPages);
}

BOOL CTabbedPropertyPageHost::CalcTabPageRects(int nWidth, int nHeight, CRect& rTabs, CRect& rPages) const
{
	if (!m_tabCtrl.GetSafeHwnd() || GetPageCount() <= 1)
		return FALSE;

	if (nWidth == 0 && nHeight == 0)
	{
		CRect rClient;
		GetClientRect(rClient);

		nWidth = rClient.Width();
		nHeight = rClient.Height();
	}

	rTabs = rPages = CRect(0, 0, nWidth, 0);

	// to get round AdjustRect not being const
	(const_cast<CTabbedPropertyPageHost*>(this))->m_tabCtrl.AdjustRect(TRUE, rTabs);

	int nTabHeight = rTabs.Height();
	rTabs.SetRect(0, 0, nWidth, nHeight);

	switch (m_tabCtrl.GetOrientation())
	{
		case e_tabTop:	  
			rTabs.bottom = rTabs.top + nTabHeight - 5;
			rTabs.right += 2;
			rPages.top = rTabs.bottom;
			break;

		case e_tabBottom: 
			rTabs.top = rTabs.bottom - nTabHeight + 5;
			rTabs.right += 2;
			rPages.bottom = rTabs.top;
			break;

		case e_tabLeft:	  
			rTabs.right = rTabs.left + nTabHeight; 
			rPages.left = rTabs.right;
			break;

		case e_tabRight:  
			rTabs.left = rTabs.right - nTabHeight;
			rPages.right = rTabs.left;
			break;

		default:
			return FALSE;
	}

	return TRUE;
}