#if !defined(AFX_TDLFINDTASKSDLG_H__9118493D_32FD_434D_B549_8947D00277CD__INCLUDED_)
#define AFX_TDLFINDTASKSDLG_H__9118493D_32FD_434D_B549_8947D00277CD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TDLFindTasksDlg.h : header file
//
#include "TDLfindtaskexpressionlistctrl.h"
#include "tdlfindresultslistctrl.h"
#include "tdlfilteroptioncombobox.h"

#include "..\shared\entoolbar.h"
#include "..\shared\toolbarhelper.h"
#include "..\shared\runtimedlg.h"
#include "..\shared\wndprompt.h"
#include "..\shared\icon.h"
#include "..\shared\SizeGrip.h"

#include "..\Interfaces\uithemefile.h"

/////////////////////////////////////////////////////////////////////////////

class CPreferences;

//////////////////////////////////////////////////////////////////////

enum DM_POS
{
	DMP_UNDOCKED = -1,
	DMP_LEFT,
	DMP_RIGHT,
	DMP_BELOW,
};

/////////////////////////////////////////////////////////////////////////////
// CTDLFindTasksDlg dialog

class CTDLFindTasksDlg : public CRuntimeDlg
{
// Construction
public:
	CTDLFindTasksDlg();   // standard constructor
	~CTDLFindTasksDlg();

	BOOL Create();
	BOOL Show(BOOL bShow = TRUE);
	void RefreshSearch();

	BOOL GetSearchAllTasklists();
	BOOL GetSearchIncludesCompletedTasks(LPCTSTR szName) const;
	BOOL SetSearchIncludesCompletedTasks(LPCTSTR szName, BOOL bIncDone);

	int GetSearchParams(SEARCHPARAMS& params);
	int GetSearchParams(TDCADVANCEDFILTER& filter);
	int GetSearchParams(LPCTSTR szName, TDCADVANCEDFILTER& filter) const;

	CString GetActiveSearch() const { return m_sActiveSearch; }
	int GetSavedSearches(CStringArray& aNames) const;

	void AddResults(const CFilteredToDoCtrl* pTDC, const CResultArray& aResults, BOOL bShowValueOnly, LPCTSTR szHeaderText = NULL);

	int GetResultCount() const; // all tasklists
	int GetResultCount(const CFilteredToDoCtrl* pTDC) const;
	int GetAllResults(CFTDResultsArray& aResults) const;
	int GetResults(const CFilteredToDoCtrl* pTDC, CFTDResultsArray& aResults) const;
	int GetResultIDs(const CFilteredToDoCtrl* pTDC, CDWordArray& aTaskIDs) const;

	void DeleteResults(const CFilteredToDoCtrl* pTDC);
	void DeleteAllResults();

	void SetCustomAttributes(const CTDCCustomAttribDefinitionArray& aActiveTasklistAttribDefs,
							const CTDCCustomAttribDefinitionArray& aAllTasklistsAttribDefs);
	void SetAttributeListData(const TDCAUTOLISTDATA& tldActive, const TDCAUTOLISTDATA& tldAll, TDC_ATTRIBUTE nAttribID);
	void SetActiveTasklist(const CString& sTasklist, BOOL bWantDefaultIcons);
	
	void RefreshUserPreferences() { m_lcResults.RefreshUserPreferences(); }
	void SetUITheme(const CUIThemeFile& theme);

	BOOL IsDocked() const { return IsDocked(m_nDockPos); }
	DM_POS GetDockPosition() const { return m_nDockPos; }
	int GetDockedDimension(BOOL bMaximized) const;
	int GetDockedDimension(DM_POS nPos, BOOL bMaximized) const;

protected:
// Dialog Data
	//{{AFX_DATA(CTDLFindTasksDlg)
	//}}AFX_DATA
	CCheckComboBox m_cbInclude;
	CComboBox m_cbSearches, m_cbTasklists;
	CTDLFindTaskExpressionListCtrl m_lcFindSetup;
	CTDLFindResultsListCtrl m_lcResults;
	CEnToolBar m_toolbar;
	CSizeGrip m_sbGrip;

	CWndPromptManager m_mgrPrompts;
	CToolbarHelper m_tbHelper;

	BOOL m_bInitializing;
	BOOL m_bSplitting;
	int m_nCurSel;
	int	m_bAllTasklists;
	DM_POS m_nDockPos;

	CEnString m_sResultsLabel;
	CString m_sActiveSearch;

	CStringArray m_aSavedSearches;
	CTDCCustomAttribDefinitionArray m_aActiveTDCAttribDefs, m_aAllTDCAttribDefs;
	TDCAUTOLISTDATA m_tldActive, m_tldAll;

	CUIThemeFile m_theme;
	CBrush m_brBkgnd;
	CIcon m_icon;
	CSize m_sizeDocked, m_sizeDockedMax;
	CRect m_rUndocked;

// Overrides
protected:
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTDLFindTasksDlg)
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	virtual void OnCancel();
	virtual void OnOK();
	virtual BOOL OnInitDialog();

	//}}AFX_VIRTUAL

// Implementation
protected:
	int DoModal() { return -1; } // not for public use

	// Generated message map functions
	//{{AFX_MSG(CTDLFindTasksDlg)
	afx_msg void OnFindHelp();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnAddRule();
	afx_msg void OnApplyasfilter();
	afx_msg void OnClose();
	afx_msg void OnDblClkResults(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeleteRule();
	afx_msg void OnDeleteSearch();
	afx_msg void OnDestroy();
	afx_msg void OnDockbelow();
	afx_msg void OnDockleft();
	afx_msg void OnDockright();
	afx_msg void OnEditchangeSearchlist();
	afx_msg void OnFind();
	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	afx_msg void OnItemActivated(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemchangedRulelist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemchangingResults(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnMove(int x, int y);
	afx_msg void OnMoveRuleDown();
	afx_msg void OnMoveRuleUp();
	afx_msg void OnNewSearch();
	afx_msg void OnSaveSearch();
	afx_msg void OnSelchangeSearchlist();
	afx_msg void OnSelchangeTasklistoptions();
	afx_msg void OnSelchangeInclude();
	afx_msg void OnSelectall();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnUndock();
	afx_msg void OnUpdateDeleteRule(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDeleteSearch(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDockbelow(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDockleft(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDockright(CCmdUI* pCmdUI);
	afx_msg void OnUpdateMoveRuleDown(CCmdUI* pCmdUI);
	afx_msg void OnUpdateMoveRuleUp(CCmdUI* pCmdUI);
	afx_msg void OnUpdateSaveSearch(CCmdUI* pCmdUI);
	afx_msg void OnUpdateUndock(CCmdUI* pCmdUI);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnCaptureChanged(CWnd* pWnd);

protected:
	// pseudo-handler
	void OnSaveSearch(BOOL bNotifyParent); 
	void OnChangeDock(DM_POS nNewPos);

	BOOL Create(DM_POS nPos);
	void SaveSettings();
	void ResizeDlg(BOOL bOrientationChange, int cx = 0, int cy = 0);
	void LoadSettings();
	CSize GetMinSize(DM_POS nPos);
	int GetNextResult(int nItem, BOOL bDown);
	void SelectItem(int nItem);
	int GetSelectedItem();
	CString GetCurrentSearch();
	BOOL InitializeToolbar();
	void EnableApplyAsFilterButton();
	void BuildOptionCombos();

	enum FIND_INCLUDE
	{ 
		FI_COMPLETED,
		FI_PARENT,
		FI_FILTEREDOUT,
	};
	BOOL IncludeOptionIsChecked(FIND_INCLUDE nOption) const;
	void CheckIncludeOption(FIND_INCLUDE nOption, BOOL bCheck);

	BOOL LoadSearch(LPCTSTR szName, CSearchParamArray& params, BOOL& bDone, BOOL& bParents, BOOL& bFilteredOut) const;
	BOOL LoadSearch(LPCTSTR szName);
	BOOL SaveSearch(LPCTSTR szName);
	int LoadSearches();
	int SaveSearches();
	int GetSearchParams(LPCTSTR szName, SEARCHPARAMS& params) const;

	CRect GetSplitterRect() const;
	BOOL GetSplitterRect(CRect& rSplitter, int nSplitPos) const;
	BOOL IsSplitterVertical() const;
	BOOL SetSplitterPos(int nSplitPos);

	static BOOL IsDocked(DM_POS nPos) { return (nPos != DMP_UNDOCKED); }

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TDLFINDTASKSDLG_H__9118493D_32FD_434D_B549_8947D00277CD__INCLUDED_)
