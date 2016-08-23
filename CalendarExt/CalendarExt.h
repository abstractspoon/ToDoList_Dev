// CalendarExtApp.h: interface for the CCalendarExtApp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CALENDAREXTAPP_H__8FDB207D_40DB_405D_8D76_A9898B6DAE21__INCLUDED_)
#define AFX_CALENDAREXTAPP_H__8FDB207D_40DB_405D_8D76_A9898B6DAE21__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//////////////////////////////////////////////////////////////////////

#include "..\Interfaces\IUIExtension.h"

//////////////////////////////////////////////////////////////////////

const LPCTSTR CAL_TYPEID = _T("4A5678B7-6583-43D7-BB00-B1CA5C16D541");

//////////////////////////////////////////////////////////////////////

class CCalendarExtApp : public IUIExtension, public CWinApp  
{
public:
	CCalendarExtApp();
	virtual ~CCalendarExtApp();

    void Release(); // releases the interface
	void SetLocalizer(ITransText* pTT);

	LPCTSTR GetMenuText() const { return _T("Calendar"); }
	HICON GetIcon() const { return m_hIcon; }
	LPCTSTR GetTypeID() const { return CAL_TYPEID; }

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

#endif // !defined(AFX_CALENDAREXTAPP_H__8FDB207D_40DB_405D_8D76_A9898B6DAE21__INCLUDED_)
