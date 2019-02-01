#if !defined(AFX_FINDREPLACE_H__E7F84BEA_24A6_42D4_BE92_4B8891484048__INCLUDED_)
#define AFX_FINDREPLACE_H__E7F84BEA_24A6_42D4_BE92_4B8891484048__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

////////////////////////////////////////////////////////////////////////////////

const UINT WM_FINDREPLACE = ::RegisterWindowMessage(FINDMSGSTRING);

////////////////////////////////////////////////////////////////////////////////

class IFindReplaceCmdHandler
{
public:
	virtual void OnFindNext(const CString& sFind, 
							BOOL bNext, 
							BOOL bCase, 
							BOOL bWord) = 0;

	virtual void OnReplaceSel(const CString& sFind, 
								const CString& sReplace, 
								BOOL bNext, 
								BOOL bCase,
								BOOL bWord) = 0;

	virtual void OnReplaceAll(const CString& sFind, 
								const CString& sReplace,
								BOOL bCase, 
								BOOL bWord) = 0;

	virtual CFindReplaceDialog* NewFindReplaceDlg();
};

////////////////////////////////////////////////////////////////////////////////

struct FIND_STATE
{
	FIND_STATE();

	void UpdateState(const CString& sFind, BOOL bNext, BOOL bCase, BOOL bWord);
	void UpdateState(const CString& sFind, const CString& sReplace, BOOL bNext, BOOL bCase, BOOL bWord);

	BOOL Initialise(HWND hwndParent, IFindReplaceCmdHandler* pCmdHandler, BOOL bFindOnly, LPCTSTR szTitle, LPCTSTR szFind = NULL);
	void DestroyDialog();

	void AdjustDialogPosition(const CPoint& ptScreen, BOOL bUpDown = TRUE);
	void AdjustDialogPosition(const CRect& rScreen, BOOL bUpDown = TRUE);

	void HandleCmd(IFindReplaceCmdHandler* pCmdHandler, WPARAM wParam, LPARAM lParam);

	CFindReplaceDialog* pFindReplaceDlg;	// find or replace dialog
	BOOL bFindOnly;							// Is pFindReplaceDlg the find or replace?
	CString strFind;						// last find string
	CString strReplace;						// last replace string
	BOOL bFindNext;							// TRUE==search down, FALSE== search up
	BOOL bCaseSensitive;					// TRUE==case sensitive, FALSE==not
	BOOL bWholeWord;						// TRUE==match whole word, FALSE==not
};

////////////////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_RICHEDITBASECTRL_H__E7F84BEA_24A6_42D4_BE92_4B8891484048__INCLUDED_)
