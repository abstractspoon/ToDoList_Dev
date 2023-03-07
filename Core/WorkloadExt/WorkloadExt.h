// WorkloadExt.h: interface for the CWorkloadExtApp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WORKLOADEXT_H__DEE73DE1_C6EC_4648_9151_0FC2C75A806D__INCLUDED_)
#define AFX_WORKLOADEXT_H__DEE73DE1_C6EC_4648_9151_0FC2C75A806D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//////////////////////////////////////////////////////////////////////

#include "..\shared\Icon.h"

#include "..\Interfaces\IUIExtension.h"

//////////////////////////////////////////////////////////////////////

const LPCTSTR WORKLOAD_TYPEID = _T("WORKLOAD_4320FC3D-C6A5-4A7E-A159-3BA69CEE6E9D");

//////////////////////////////////////////////////////////////////////

class CWorkloadExtApp : public IUIExtension, public CWinApp
{
public:
	CWorkloadExtApp();
	virtual ~CWorkloadExtApp();

    void Release(); // releases the interface
	void SetLocalizer(ITransText* pTT);

	LPCTSTR GetMenuText() const { return _T("Workload"); }
	HICON GetIcon() const { return m_icon; }
	LPCTSTR GetTypeID() const { return WORKLOAD_TYPEID; }

	IUIExtensionWindow* CreateExtWindow(UINT nCtrlID, DWORD nStyle, 
										long nLeft, long nTop, long nWidth, long nHeight, 
										HWND hwndParent);
	
	void SavePreferences(IPreferences* pPrefs, LPCWSTR szKey) const;
	void LoadPreferences(const IPreferences* pPrefs, LPCWSTR szKey);

	bool SupportsTaskSelection() const { return true; }

protected:
	CIcon m_icon;

protected:
	BOOL InitInstance();
};

#endif // !defined(AFX_WORKLOADEXT_H__DEE73DE1_C6EC_4648_9151_0FC2C75A806D__INCLUDED_)
