// TDLBrowserDlg.h: interface for the CTDLBrowserDlg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TDLBROWSERDLG_H__D921F585_9D28_4C49_BBED_98AAF718314E__INCLUDED_)
#define AFX_TDLBROWSERDLG_H__D921F585_9D28_4C49_BBED_98AAF718314E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\Shared\browserdlg.h"

class CTDLBrowserDlg : public CBrowserDlg  
{
public:
	CTDLBrowserDlg(BOOL bBrowser);
	virtual ~CTDLBrowserDlg();

protected:
	afx_msg void BeforeNavigate2(LPDISPATCH pDisp, VARIANT* pURL, VARIANT* pFlags,
								 VARIANT* pTargetFrameName, VARIANT* pPostData,
								 VARIANT* pHeaders, VARIANT_BOOL* pCancel);
	DECLARE_EVENTSINK_MAP()
};

#endif // !defined(AFX_TDLBROWSERDLG_H__D921F585_9D28_4C49_BBED_98AAF718314E__INCLUDED_)
