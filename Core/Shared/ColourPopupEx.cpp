// ColourPopupEx.cpp : implementation file
//

#include "stdafx.h"
#include "ColourPopupEx.h"
#include "EnColorDialog.h"

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
	LPCTSTR szDefaultText, LPCTSTR szCustomText)
	:
	// Don't call parameterised base class constructor because it doesn't 
	// set m_pParent before calling Create() and we need it set so that
	// we can hook the parent in OnCreate()
	CColourPopup()
{
	// NOTE: CColorPopup() will have called Initialise()

	m_crColour       = m_crInitialColour = crColour;
	m_pParent        = pParentWnd;
	m_strDefaultText = (szDefaultText)? szDefaultText : _T("");
	m_strCustomText  = (szCustomText)?  szCustomText  : _T("");

	Create(p, crColour, pParentWnd, nID, szDefaultText, szCustomText);
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
	m_pParent->SetCapture();

	ShowWindow(SW_SHOWNOACTIVATE);

	return 0;
}

LRESULT CColourPopupEx::ScWindowProc(HWND hRealWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	switch (msg)
	{
	case WM_KEYDOWN:
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
		break;

	case WM_LBUTTONDOWN:
		{
			CPoint ptMsg = CPoint(GetMessagePos());

			if (m_WindowRect.PtInRect(ptMsg))
			{
				ScreenToClient(&ptMsg);
				return ::SendMessage(hRealWnd, msg, wp, MAKELPARAM(ptMsg.x, ptMsg.y));
			}

			EndSelection(CPN_SELENDCANCEL);
			return 0L;
		}
		break;

	case WM_LBUTTONUP:
	case WM_MOUSEMOVE:
		{
			CPoint ptMsg = CPoint(GetMessagePos());

			if (m_WindowRect.PtInRect(ptMsg))
			{
				ScreenToClient(&ptMsg);
				return ::SendMessage(hRealWnd, msg, wp, MAKELPARAM(ptMsg.x, ptMsg.y));
			}
		}
		break;
	}

	return CSubclasser::ScDefault(m_scParent);
}

void CColourPopupEx::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// Bypass base-class because we've hooked the parent instead
	CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}

BOOL CColourPopupEx::DoColorDialog(COLORREF crIn, COLORREF& crOut)
{
	CEnColorDialog dlg(crIn, CC_FULLOPEN | CC_ANYCOLOR, this);

	if (dlg.DoModal() != IDOK)
		return FALSE;

	crOut = dlg.GetColor();
	return TRUE;
}
