// EditPrompt.cpp: implementation of the CEditPrompt class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EditPrompt.h"
#include "winclasses.h"
#include "wclassdefines.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEditPrompt::CEditPrompt()
{

}

CEditPrompt::~CEditPrompt()
{

}

BOOL CEditPrompt::Initialize(HWND hwndEdit, LPCTSTR szPrompt)
{
	ASSERT (hwndEdit);
	ASSERT (!IsHooked());
	ASSERT (szPrompt && *szPrompt);
//	ASSERT (CWinClasses::IsEditControl(hwndEdit));
//	ASSERT (!(GetWindowLong(hwndEdit, GWL_STYLE) & ES_MULTILINE));

	if (!IsHooked() && hwndEdit && szPrompt && *szPrompt)
	{
		// check that it is a single line edit control
		if (CWinClasses::IsEditControl(hwndEdit)/* && 
			!(GetWindowLong(hwndEdit, GWL_STYLE) & ES_MULTILINE)*/)
		{
			if (HookWindow(hwndEdit))
			{
				m_sPrompt = szPrompt;

				// check whether we should invalidate
				if (!GetWindowTextLength(hwndEdit) && GetFocus() != hwndEdit)
					Invalidate();

				return TRUE;
			}
		}
	}

	return FALSE;
}

void CEditPrompt::SetPrompt(LPCTSTR szPrompt)
{
	ASSERT (IsHooked());
	ASSERT (szPrompt && *szPrompt);

	if (IsHooked() && szPrompt && *szPrompt)
	{
		m_sPrompt = szPrompt;

		// check whether we should invalidate
		HWND hwnd = GetHwnd();

		if (!GetWindowTextLength(hwnd) && GetFocus() != hwnd)
			Invalidate();
	}
}

LRESULT CEditPrompt::WindowProc(HWND hRealWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)
	{
	case WM_KILLFOCUS:
	case WM_SETFOCUS:
		if (!GetWindowTextLength(hRealWnd))
			Invalidate();
		break;

	case WM_PAINT:
		if (!GetWindowTextLength(hRealWnd) && GetFocus() != hRealWnd &&
			IsWindowEnabled() && !(GetStyle() & ES_READONLY))
		{
			ASSERT (m_sPrompt.GetLength());

			// pass to default handler 
			Default();

			// now paint prompt over the top
			HDC hdc = GetDC(hRealWnd);
			int nSaveDC = ::SaveDC(hdc);
			CRect rClient;
			GetClientRect(rClient);
			rClient.DeflateRect(2, 1);

			HFONT hFont = (HFONT)SendMessage(WM_GETFONT);

			if (!hFont)
				hFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);

			::SelectObject(hdc, hFont);
			::SetTextColor(hdc, GetSysColor(COLOR_3DSHADOW));
			::SetBkMode(hdc, TRANSPARENT);
			::DrawText(hdc, m_sPrompt, m_sPrompt.GetLength(), rClient, DT_TOP | DT_LEFT);

			::RestoreDC(hdc, nSaveDC);
			::ReleaseDC(hRealWnd, hdc);

			return 0;
		}
		break;
	}

	return CSubclassWnd::WindowProc(hRealWnd, msg, wp, lp);
}

//////////////////////////////////////////////////////////////////////

CEditPromptManager::CEditPromptManager()
{
}

CEditPromptManager::~CEditPromptManager()
{
	// cleanup
	HWND hwnd;
	CEditPrompt* pEdit;

	POSITION pos = m_mapEdits.GetStartPosition();

	while (pos)
	{
		m_mapEdits.GetNextAssoc(pos, hwnd, pEdit);

		if (pEdit->IsValid())
			pEdit->HookWindow(NULL);

		delete pEdit;
	}
}

BOOL CEditPromptManager::SetEditPrompt(HWND hwndEdit, LPCTSTR szPrompt)
{
	// have we already got it?
	CEditPrompt* pEdit = NULL;

	if (m_mapEdits.Lookup(hwndEdit, pEdit))
	{
		if (pEdit->IsValid())
		{
			// empty prompt => remove
			if (!szPrompt || !*szPrompt)
			{
				pEdit->HookWindow(NULL);
				delete pEdit;
				m_mapEdits.RemoveKey(hwndEdit);
			}
			else // update prompt
				pEdit->SetPrompt(szPrompt);

			return TRUE;
		}
		else // edit control has been destroyed and hwnd is being reused
		{
			delete pEdit;
			m_mapEdits.RemoveKey(hwndEdit);
		}

		pEdit = NULL;
	}

	// create new editprompt
	pEdit = new CEditPrompt;

	if (pEdit && pEdit->Initialize(hwndEdit, szPrompt))
	{
		m_mapEdits[hwndEdit] = pEdit;
		return TRUE;
	}
	
	// else
	delete pEdit;
	return FALSE;
}

BOOL CEditPromptManager::SetEditPrompt(UINT nIDEdit, HWND hwndParent, LPCTSTR szPrompt)
{
	return SetEditPrompt(GetDlgItem(hwndParent, nIDEdit), szPrompt);
}

BOOL CEditPromptManager::SetComboEditPrompt(HWND hwndCombo, LPCTSTR szPrompt)
{
	return SetEditPrompt(1001, hwndCombo, szPrompt);
}
