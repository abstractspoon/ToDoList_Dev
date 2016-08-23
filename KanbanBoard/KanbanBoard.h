// KanbanBoard.h : main header file for the KANBANBOARD DLL
//

#if !defined(AFX_KANBANBOARD_H__7AA17495_A637_40CB_849D_12DD4A1B9EDB__INCLUDED_)
#define AFX_KANBANBOARD_H__7AA17495_A637_40CB_849D_12DD4A1B9EDB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//////////////////////////////////////////////////////////////////////

#include "..\Interfaces\IUIExtension.h"

//////////////////////////////////////////////////////////////////////

const LPCTSTR KANBAN_TYPEID = _T("B01BF737-BD18-431C-ABCB-31661032EB7C");

//////////////////////////////////////////////////////////////////////

class CKanbanExtApp : public IUIExtension, public CWinApp
{
public:
	CKanbanExtApp();
	virtual ~CKanbanExtApp();
	
    void Release(); // releases the interface
	void SetLocalizer(ITransText* pTT);
	
	LPCTSTR GetMenuText() const { return _T("Kanban");	}
	HICON GetIcon() const		{ return m_hIcon;		}
	LPCTSTR GetTypeID() const	{ return KANBAN_TYPEID; }
	
	IUIExtensionWindow* CreateExtWindow(UINT nCtrlID, DWORD nStyle, 
		long nLeft, long nTop, long nWidth, long nHeight, 
		HWND hwndParent);
	
	void SavePreferences(IPreferences* pPrefs, LPCWSTR szKey) const;
	void LoadPreferences(const IPreferences* pPrefs, LPCWSTR szKey);
	
protected:
	HICON m_hIcon;
	
protected:
	BOOL InitInstance();
};



/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_KANBANBOARD_H__7AA17495_A637_40CB_849D_12DD4A1B9EDB__INCLUDED_)
