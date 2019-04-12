// EditPrompt.h: interface for the CEditPrompt class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EDITPROMPT_H__485A738F_5BCB_4D7E_9B3E_6E9382AC62D8__INCLUDED_)
#define AFX_EDITPROMPT_H__485A738F_5BCB_4D7E_9B3E_6E9382AC62D8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "WndPrompt.h"

class CEditPromptManager : public CWndPromptManager
{
public:
	CEditPromptManager();
	virtual ~CEditPromptManager();

	BOOL SetEditPrompt(UINT nIDEdit, HWND hwndParent, LPCTSTR szPrompt);
	BOOL SetEditPrompt(HWND hwndEdit, LPCTSTR szPrompt);
	BOOL SetComboEditPrompt(HWND hwndCombo, LPCTSTR szPrompt);

protected:
	virtual BOOL WantPrompt(HWND hWnd);
};

#endif // !defined(AFX_EDITPROMPT_H__485A738F_5BCB_4D7E_9B3E_6E9382AC62D8__INCLUDED_)
