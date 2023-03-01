#if !defined(AFX_TDCFINDFILTERHELPER_H__5951FDE6_508A_4A9D_A55D_D16EB026AEF7__INCLUDED_)
#define AFX_TDCFINDFILTERHELPER_H__5951FDE6_508A_4A9D_A55D_D16EB026AEF7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ToDoCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////

class CTDLFindTasksDlg;
class CTDLFilterBar;

/////////////////////////////////////////////////////////////////////////////
// 

class CTDCFindFilterHelper
{
// Construction
public:
	CTDCFindFilterHelper(CTDLFindTasksDlg& findDlg, CTDLFilterBar& m_filterBar);
	virtual ~CTDCFindFilterHelper();

	void RefreshFilterBarAdvancedFilters();
	BOOL UpdateFilterBarAdvancedFilter(LPCTSTR szFilter);

	BOOL UpdateFindDlgAdvancedFilter(LPCTSTR szFilter, DWORD dwFilterFlags);

protected:
	CTDLFindTasksDlg& m_findDlg;
	CTDLFilterBar& m_filterBar;

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TDCFINDFILTERHELPER_H__5951FDE6_508A_4A9D_A55D_D16EB026AEF7__INCLUDED_)
