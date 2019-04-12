#if !defined(AFX_TDLIMPORTOUTLOOKOBJECTSETUPLISTCTRL_H__E379E120_FF91_417F_ADBB_0DD6A98089AA__INCLUDED_)
#define AFX_TDLIMPORTOUTLOOKOBJECTSETUPLISTCTRL_H__E379E120_FF91_417F_ADBB_0DD6A98089AA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "tdcenum.h"
#include "TDLImpExpAttributeMappingListCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CTDLImportOutlookObjectSetupListCtrl window

class CTDLImportOutlookObjectSetupListCtrl : public CTDLImportExportAttributeMappingListCtrl
{
// Construction
public:
	CTDLImportOutlookObjectSetupListCtrl();
	virtual ~CTDLImportOutlookObjectSetupListCtrl();

	// Generated message map functions
protected:
	afx_msg void OnAttribEditOK();

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TDLIMPORTOUTLOOKOBJECTSETUPLISTCTRL_H__E379E120_FF91_417F_ADBB_0DD6A98089AA__INCLUDED_)
