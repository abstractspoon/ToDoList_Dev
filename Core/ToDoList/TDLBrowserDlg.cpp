// TDLBrowserDlg.cpp: implementation of the CTDLBrowserDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TDLBrowserDlg.h"
#include "TDCMsg.h"
#include "ToDoItem.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTDLBrowserDlg::CTDLBrowserDlg(BOOL bBrowser) : CBrowserDlg(bBrowser)
{

}

CTDLBrowserDlg::~CTDLBrowserDlg()
{

}

BEGIN_EVENTSINK_MAP(CTDLBrowserDlg, CBrowserDlg)
	//{{AFX_EVENTSINK_MAP(CBrowserDlg)
	//}}AFX_EVENTSINK_MAP
	ON_EVENT(CTDLBrowserDlg, BROWSER_CTRLID, 250 /*DISPID_BEFORENAVIGATE2*/, BeforeNavigate2, VTS_DISPATCH VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PBOOL)
END_EVENTSINK_MAP()


void CTDLBrowserDlg::BeforeNavigate2(LPDISPATCH /*pDisp*/, VARIANT* pURL, VARIANT* /*pFlags*/,
									VARIANT* /*pTargetFrameName*/, VARIANT* /*pPostData*/,
									VARIANT* /*pHeaders*/, VARIANT_BOOL* pCancel)
{
	// Call virtual handler
	ASSERT(V_VT(pURL) == VT_BSTR);
	ASSERT(pCancel != NULL);

	CString sURL(V_BSTR(pURL)), sTaskFile;
	DWORD dwTaskID = 0;

	if (TDCTASKLINK::Parse(sURL, TRUE, _T(""), dwTaskID, sTaskFile))
	{
		AfxGetMainWnd()->SendMessage(WM_TDCM_SELECTTASK, dwTaskID, (LPARAM)(LPCTSTR)sTaskFile);
		*pCancel = AFX_OLE_TRUE;
	}
	else
	{
		*pCancel = AFX_OLE_FALSE;
	}
}