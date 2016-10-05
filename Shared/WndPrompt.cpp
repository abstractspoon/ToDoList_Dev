// WndPrompt.cpp: implementation of the CWndPrompt class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wndPrompt.h"
#include "winclasses.h"
#include "wclassdefines.h"
#include "enstring.h"
#include "misc.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWndPrompt::CWndPrompt() : m_nCheckMsg(0), m_lCheckResult(0), m_bCentred(0)
{

}

CWndPrompt::~CWndPrompt()
{

}

BOOL CWndPrompt::Initialize(HWND hWnd, LPCTSTR szPrompt, UINT nCheckMsg, LRESULT lCheckRes, BOOL bCentred)
{
	ASSERT (hWnd);
	ASSERT (!IsHooked());
	ASSERT (nCheckMsg);

	if (!IsHooked() && hWnd && !Misc::IsEmpty(szPrompt) && nCheckMsg)
	{
		if (HookWindow(hWnd))
		{
			m_sPrompt = szPrompt;
			m_nCheckMsg = nCheckMsg;
			m_lCheckResult = lCheckRes;
			m_sClass = CWinClasses::GetClass(hWnd);
			m_bCentred = bCentred;
			
			if (WantPrompt())
				Invalidate();

			return TRUE;
		}
	}

	return FALSE;
}

void CWndPrompt::SetPrompt(LPCTSTR szPrompt, BOOL bCentred)
{
	if (IsHooked() && !Misc::IsEmpty(szPrompt))
	{
		m_sPrompt = szPrompt;

		if (bCentred >= 0)
			m_bCentred = bCentred;

		if (WantPrompt())
			Invalidate();
	}
	else
	{
		ASSERT(0);
	}
}

LRESULT CWndPrompt::WindowProc(HWND hRealWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)
	{
	case WM_KILLFOCUS:
	case WM_SETFOCUS:
		if (CWinClasses::IsEditControl(hRealWnd) && WantPrompt(FALSE))
			Invalidate();
		break;

	case WM_PRINT:
		{
			LRESULT lr = Default();
			DrawPrompt((HDC)wp);
			return lr;
		}
		break;

	case WM_PAINT:
		{
			LRESULT lr = Default();
			DrawPrompt(NULL);
			return lr;
		}
		break;
	}

	return CSubclassWnd::Default();
}

BOOL CWndPrompt::WantPrompt(BOOL bCheckFocus) 
{ 
	BOOL bWantPrompt = (SendMessage(m_nCheckMsg) == m_lCheckResult); 
	HWND hWnd = GetHwnd();

	if (bWantPrompt)
	{
		if (CWinClasses::IsEditControl(m_sClass))
		{
			if (bCheckFocus)
				bWantPrompt = (GetFocus() != hWnd);

			if (bWantPrompt)
				bWantPrompt = (IsWindowEnabled() && !(GetStyle() & ES_READONLY));
		}
		else if (CWinClasses::IsComboBox(m_sClass))
		{
			if (bCheckFocus)
				bWantPrompt = (GetFocus() != hWnd);
			
			if (bWantPrompt)
				bWantPrompt = IsWindowEnabled();
		}
	}

	return bWantPrompt;
}

void CWndPrompt::DrawPrompt(HDC hdc)
{
	if (WantPrompt())
		DrawPrompt(GetHwnd(), m_sPrompt, hdc, m_bCentred, m_sClass);
}

void CWndPrompt::DrawPrompt(HWND hWnd, LPCTSTR szPrompt, HDC hdc, BOOL bCentred, LPCTSTR szClass)
{
	ASSERT (!Misc::IsEmpty(szPrompt));
	
	// set up DC
	HDC hdcOrg = hdc;
	int nSaveDC = 0;
	
	if (!hdc)
	{
		hdc = ::GetDC(hWnd);
		nSaveDC = ::SaveDC(hdc);
	}
	
	// font
	HFONT hFont = (HFONT)::SendMessage(hWnd, WM_GETFONT, 0, 0);
	
	if (!hFont)
		hFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
	
	::SelectObject(hdc, hFont);

	CRect rClient;
	::GetClientRect(hWnd, rClient);
	rClient.DeflateRect(2, 1, 2, 0);

	UINT nFlags = (DT_TOP | DT_NOPREFIX | (bCentred ? DT_CENTER : DT_LEFT));

	if (CWinClasses::IsComboBox(szClass))
	{
		nFlags |= (DT_VCENTER | DT_SINGLELINE);
		rClient.DeflateRect(2, 0);
	}
	else if (CWinClasses::IsEditControl(hWnd))
	{
		HBRUSH hbrBkgnd = NULL;

		if (!::IsWindowEnabled(hWnd) || (::GetWindowLong(hWnd, GWL_STYLE) & ES_READONLY))
			hbrBkgnd = GetSysColorBrush(COLOR_3DFACE);
		else
			hbrBkgnd = (HBRUSH)::SendMessage(::GetParent(hWnd), WM_CTLCOLOREDIT, (WPARAM)hdc, (LPARAM)hWnd);
		
		::FillRect(hdc, rClient, hbrBkgnd);
	}

	// background colour
	::SetBkMode(hdc, TRANSPARENT);
	::SetTextColor(hdc, GetSysColor(COLOR_3DSHADOW));
	
	// draw prompt
	::DrawText(hdc, szPrompt, -1, rClient, nFlags);
	
	// cleanup
	if (!hdcOrg)
	{
		::RestoreDC(hdc, nSaveDC);
		::ReleaseDC(hWnd, hdc);
	}
}

//////////////////////////////////////////////////////////////////////

CWndPromptManager::CWndPromptManager()
{
}

CWndPromptManager::~CWndPromptManager()
{
	// cleanup
	HWND hwnd;
	CWndPrompt* pWnd;

	POSITION pos = m_mapWnds.GetStartPosition();

	while (pos)
	{
		m_mapWnds.GetNextAssoc(pos, hwnd, pWnd);

		if (pWnd->IsValid())
			pWnd->HookWindow(NULL);

		delete pWnd;
	}
}

BOOL CWndPromptManager::SetPrompt(HWND hWnd, LPCTSTR szPrompt, 
								  UINT nCheckMsg, LRESULT lCheckRes, BOOL bCentred)
{
	// have we already got it?
	CWndPrompt* pWnd = NULL;

	if (m_mapWnds.Lookup(hWnd, pWnd))
	{
		if (pWnd->IsValid())
		{
			// empty prompt => remove
			if (Misc::IsEmpty(szPrompt))
			{
				pWnd->HookWindow(NULL);
				delete pWnd;
				m_mapWnds.RemoveKey(hWnd);
			}
			else // update prompt
			{
				pWnd->SetPrompt(szPrompt, bCentred);
			}

			return TRUE;
		}
		else // control has been destroyed and hwnd is being reused
		{
			delete pWnd;
			m_mapWnds.RemoveKey(hWnd);
		}

		pWnd = NULL;
	}

	// create new wnd prompt
	pWnd = new CWndPrompt;

	if (pWnd && pWnd->Initialize(hWnd, szPrompt, nCheckMsg, lCheckRes, bCentred))
	{
		m_mapWnds[hWnd] = pWnd;
		return TRUE;
	}
	
	// else
	delete pWnd;
	return FALSE;
}

BOOL CWndPromptManager::SetPrompt(HWND hWnd, UINT nIDPrompt, 
								  UINT nCheckMsg, LRESULT lCheckRes, BOOL bCentred)
{
	return SetPrompt(hWnd, CEnString(nIDPrompt), nCheckMsg, lCheckRes, bCentred);
}

BOOL CWndPromptManager::SetPrompt(UINT nIDCtrl, HWND hwndParent, LPCTSTR szPrompt, 
								  UINT nCheckMsg, LRESULT lCheckRes, BOOL bCentred)
{
	return SetPrompt(GetDlgItem(hwndParent, nIDCtrl), szPrompt, nCheckMsg, lCheckRes, bCentred);
}

BOOL CWndPromptManager::SetPrompt(UINT nIDCtrl, HWND hwndParent, UINT nIDPrompt, 
								  UINT nCheckMsg, LRESULT lCheckRes, BOOL bCentred)
{
	return SetPrompt(nIDCtrl, hwndParent, CEnString(nIDPrompt), nCheckMsg, lCheckRes, bCentred);
}

// -------------------------------------------------------------------------------

BOOL CWndPromptManager::SetEditPrompt(UINT nIDEdit, HWND hwndParent, LPCTSTR szPrompt)
{
	return SetEditPrompt(GetDlgItem(hwndParent, nIDEdit), szPrompt);
}

BOOL CWndPromptManager::SetEditPrompt(UINT nIDEdit, HWND hwndParent, UINT nIDPrompt)
{
	return SetEditPrompt(nIDEdit, hwndParent, CEnString(nIDPrompt));
}

BOOL CWndPromptManager::SetEditPrompt(HWND hwndEdit, LPCTSTR szPrompt)
{
	if (!CWinClasses::IsEditControl(hwndEdit))
		return FALSE;

	return SetPrompt(hwndEdit, szPrompt, WM_GETTEXTLENGTH);
}

BOOL CWndPromptManager::SetEditPrompt(HWND hwndEdit, UINT nIDPrompt)
{
	return SetEditPrompt(hwndEdit, CEnString(nIDPrompt));
}

// -------------------------------------------------------------------------------

BOOL CWndPromptManager::SetComboEditPrompt(UINT nIDCombo, HWND hwndParent, LPCTSTR szPrompt)
{
	return SetComboEditPrompt(GetDlgItem(hwndParent, nIDCombo), szPrompt);
}

BOOL CWndPromptManager::SetComboEditPrompt(UINT nIDCombo, HWND hwndParent, UINT nIDPrompt)
{
	return SetComboEditPrompt(nIDCombo, hwndParent, CEnString(nIDPrompt));
}

BOOL CWndPromptManager::SetComboEditPrompt(HWND hwndCombo, LPCTSTR szPrompt)
{
	CString sClass = CWinClasses::GetClass(hwndCombo);

	if (!CWinClasses::IsClass(sClass, WC_COMBOBOX) && 
		!CWinClasses::IsClass(sClass, WC_COMBOBOXEX))
		return FALSE;

	return SetPrompt(1001, hwndCombo, szPrompt, WM_GETTEXTLENGTH);
}

BOOL CWndPromptManager::SetComboEditPrompt(HWND hwndCombo, UINT nIDPrompt)
{
	return SetComboEditPrompt(hwndCombo, CEnString(nIDPrompt));
}

// -------------------------------------------------------------------------------

BOOL CWndPromptManager::SetComboPrompt(UINT nIDCombo, HWND hwndParent, LPCTSTR szPrompt)
{
	return SetComboPrompt(GetDlgItem(hwndParent, nIDCombo), szPrompt);
}

BOOL CWndPromptManager::SetComboPrompt(HWND hwndCombo, LPCTSTR szPrompt)
{
	CString sClass = CWinClasses::GetClass(hwndCombo);

	if (!CWinClasses::IsClass(sClass, WC_COMBOBOX) && 
		!CWinClasses::IsClass(sClass, WC_COMBOBOXEX))
		return FALSE;

	// if the combo has an edit field then this is where the 
	// prompt must be set
	UINT nStyle = GetWindowLong(hwndCombo, GWL_STYLE);

	if ((nStyle & 0xf) != CBS_DROPDOWNLIST)
		return SetComboEditPrompt(hwndCombo, szPrompt);
	
	// else
	return SetPrompt(hwndCombo, szPrompt, WM_GETTEXTLENGTH);
}

BOOL CWndPromptManager::SetComboPrompt(UINT nIDCombo, HWND hwndParent, UINT nIDPrompt)
{
	return SetComboPrompt(GetDlgItem(hwndParent, nIDCombo), CEnString(nIDPrompt));
}

BOOL CWndPromptManager::SetComboPrompt(HWND hwndCombo, UINT nIDPrompt)
{
	return SetComboPrompt(hwndCombo, CEnString(nIDPrompt));
}

// -------------------------------------------------------------------------------
