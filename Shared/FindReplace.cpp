// FindReplace.cpp : implementation file
//

#include "stdafx.h"
#include "FindReplace.h"
#include "GraphicsMisc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

CFindReplaceDialog* IFindReplaceCmdHandler::NewFindReplaceDlg()
{
	return new CFindReplaceDialog;
}

/////////////////////////////////////////////////////////////////////////////

FIND_STATE::FIND_STATE() : pFindReplaceDlg(NULL), bFindOnly(FALSE), bCaseSensitive(FALSE), bFindNext(TRUE), bWholeWord(FALSE) 
{
}

void FIND_STATE::UpdateState(const CString& sFind, BOOL bNext, BOOL bCase, BOOL bWord)
{
	strFind = sFind;
	bFindNext = bNext;
	bCaseSensitive = bCase;
	bWholeWord = bWord;
}

void FIND_STATE::UpdateState(const CString& sFind, const CString& sReplace, BOOL bNext, BOOL bCase, BOOL bWord)
{
	UpdateState(sFind, bNext, bCase, bWord);

	strReplace = sReplace;
}

BOOL FIND_STATE::Initialise(HWND hwndParent, 
							IFindReplaceCmdHandler* pCmdHandler, 
							BOOL bFind, 
							LPCTSTR szTitle,
							LPCTSTR szFind)
{
	CWnd* pParent = CWnd::FromHandle(hwndParent);
	ASSERT_VALID(pParent);
	ASSERT(pCmdHandler);

	if (pFindReplaceDlg != NULL)
	{
		if (bFindOnly == bFind)
		{
			pFindReplaceDlg->SetActiveWindow();
			pFindReplaceDlg->ShowWindow(SW_SHOW);

			return TRUE;
		}

		// else
		DestroyDialog();
	}

	CString strPrev(strFind);
	strFind = szFind;

	// if selection is empty or spans multiple lines use old find text
	if (strFind.IsEmpty() || (strFind.FindOneOf(_T("\n\r")) != -1))
		strFind = strPrev;

	pFindReplaceDlg = pCmdHandler->NewFindReplaceDlg();
	ASSERT(pFindReplaceDlg != NULL);

	DWORD dwFlags = 0;

	if (bFindNext)
		dwFlags |= FR_DOWN;

	if (bCaseSensitive)
		dwFlags |= FR_MATCHCASE;

	if (bWholeWord)
		dwFlags |= FR_WHOLEWORD;

	if (!pFindReplaceDlg->Create(bFind, strFind, strReplace, dwFlags, pParent))
	{
		delete pFindReplaceDlg;
		pFindReplaceDlg = NULL;

		return FALSE;
	}

	// set the title
	if (szTitle && *szTitle)
		pFindReplaceDlg->SetWindowText(szTitle);

	bFindOnly = bFind;

	pFindReplaceDlg->CenterWindow(pParent);
	pFindReplaceDlg->SetActiveWindow();
	pFindReplaceDlg->ShowWindow(SW_SHOW);

	return TRUE;
}

void FIND_STATE::DestroyDialog()
{
	if (pFindReplaceDlg)
	{
		pFindReplaceDlg->DestroyWindow();
		pFindReplaceDlg = NULL;
	}
}

void FIND_STATE::HandleCmd(IFindReplaceCmdHandler* pCmdHandler, WPARAM /*wParam*/, LPARAM lParam)
{
	ASSERT(lParam);
	ASSERT(pCmdHandler);

	CFindReplaceDialog* pDialog = CFindReplaceDialog::GetNotifier(lParam);

	ASSERT(pDialog != NULL);
	ASSERT(pDialog == pFindReplaceDlg);

	if (pDialog->IsTerminating())
	{
		::SetFocus(pDialog->m_fr.hwndOwner);
		pFindReplaceDlg = NULL;
	}
	else if (pDialog->FindNext())
	{
		pCmdHandler->OnFindNext(pDialog->GetFindString(), 
									pDialog->SearchDown(),
									pDialog->MatchCase(), 
									pDialog->MatchWholeWord());
	}
	else if (pDialog->ReplaceCurrent())
	{
		ASSERT(!bFindOnly);

		pCmdHandler->OnReplaceSel(pDialog->GetFindString(),
									pDialog->GetReplaceString(),
									pDialog->SearchDown(), 
									pDialog->MatchCase(), 
									pDialog->MatchWholeWord());
	}
	else if (pDialog->ReplaceAll())
	{
		ASSERT(!bFindOnly);

		pCmdHandler->OnReplaceAll(pDialog->GetFindString(), 
									pDialog->GetReplaceString(),
									pDialog->MatchCase(), 
									pDialog->MatchWholeWord());
	}
}

void FIND_STATE::AdjustDialogPosition(const CPoint& ptScreen, BOOL bUpDown)
{
	ASSERT(pFindReplaceDlg != NULL);

	CRect rExclude(ptScreen.x - 1, ptScreen.y - 1, ptScreen.x + 1, ptScreen.y + 1);
	AdjustDialogPosition(rExclude);
}

void FIND_STATE::AdjustDialogPosition(const CRect& rExcludeScreen, BOOL bUpDown)
{
	CRect rDlg;
	pFindReplaceDlg->GetWindowRect(&rDlg);

	if (CRect().IntersectRect(rDlg, rExcludeScreen))
	{
		const int PADDING = GraphicsMisc::ScaleByDPIFactor(20);
		CRect rScreen(0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));

		if (bUpDown)
		{
			// Move find dialog above or below the exclusion rect
			// depending on where there is most space. Ideally stay
			// on the same side if possible
			BOOL bMoveAbove = (rExcludeScreen.CenterPoint().y > rDlg.CenterPoint().y);

			// Check for space
			if (bMoveAbove)
				bMoveAbove = (rDlg.top > rDlg.Height());
			else
				bMoveAbove = ((rScreen.bottom - rDlg.bottom) < rDlg.Height());

			// Adjust the pos
			if (bMoveAbove)
				rDlg.OffsetRect(0, rExcludeScreen.top - rDlg.bottom - PADDING);
			else
				rDlg.OffsetRect(0, rExcludeScreen.bottom - rDlg.top + PADDING);
		}
		else
		{
			// Move find dialog left or right of the exclusion rect
			// depending on where there is most space. Ideally stay
			// on the same side if possible
			BOOL bMoveLeft = (rExcludeScreen.CenterPoint().x > rDlg.CenterPoint().x);

			// Check for space
			if (bMoveLeft)
				bMoveLeft = (rDlg.left > rDlg.Width());
			else
				bMoveLeft = ((rScreen.right - rDlg.right) < rDlg.Width());

			// Adjust the pos
			if (bMoveLeft)
				rDlg.OffsetRect(rExcludeScreen.left - rDlg.right - PADDING, 0);
			else
				rDlg.OffsetRect(rExcludeScreen.right - rDlg.left + PADDING, 0);
		}

		pFindReplaceDlg->MoveWindow(&rDlg);
	}
}

