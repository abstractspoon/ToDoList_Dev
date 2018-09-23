// ColourPopupEx.cpp : implementation file
//

#include "stdafx.h"
#include "ColourPopupEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CColourPopupEx

CColourPopupEx::CColourPopupEx()
{
}

CColourPopupEx::CColourPopupEx(CPoint p, COLORREF crColour, CWnd* pParentWnd, UINT nID, 
	LPCTSTR szDefaultText, LPCTSTR szCustomText, BOOL bIgnoreFirstLBtnUp)
	:
	// Don't call parameterised base class constructor because it calls
	// Create() and we're only partially constructed so our OnCreate()
	// does not get called
	CColourPopup()
{
	// NOTE: CColorPopup() will have called Initialise()

	m_crColour       = m_crInitialColour = crColour;
	m_pParent        = pParentWnd;
	m_strDefaultText = (szDefaultText)? szDefaultText : _T("");
	m_strCustomText  = (szCustomText)?  szCustomText  : _T("");

	Create(p, crColour, pParentWnd, nID, szDefaultText, szCustomText, bIgnoreFirstLBtnUp);
}

CColourPopupEx::~CColourPopupEx()
{
}


BEGIN_MESSAGE_MAP(CColourPopupEx, CColourPopup)
	//{{AFX_MSG_MAP(CColourPopupEx)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
	ON_WM_CREATE()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CColourPopupEx message handlers

int CColourPopupEx::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	// Bypass base class 
	if (CWnd::OnCreate(lpCreateStruct) != 0)
		return -1;

	if (!m_scParent.HookWindow(*m_pParent, this))
		return -1;

	m_pParent->SetFocus();
	ShowWindow(SW_SHOWNOACTIVATE);

	return 0;
}

LRESULT CColourPopupEx::ScWindowProc(HWND hRealWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	if (msg == WM_KEYDOWN)
	{
		switch (wp)
		{
			case VK_DOWN:
			case VK_UP:
			case VK_RIGHT:
			case VK_LEFT:
			case VK_ESCAPE:
			case VK_RETURN:
			case VK_SPACE:
				CColourPopup::OnKeyDown(wp, LOWORD(lp), HIWORD(lp));
				return 0L; // eat
		}
	}

	return CSubclasser::ScDefault(m_scParent);
}

void CColourPopupEx::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// Bypass base-class because we've hooked the parent instead
	CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}
