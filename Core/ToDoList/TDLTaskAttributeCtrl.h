#if !defined(AFX_TDLTASKATTRIBUTEDLGCTRL_H__4173F4AA_2026_4FE1_B669_E6631D12C8F1__INCLUDED_)
#define AFX_TDLTASKATTRIBUTEDLGCTRL_H__4173F4AA_2026_4FE1_B669_E6631D12C8F1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TDLTaskAttributeListCtrl.h : header file
//

#include "TDLTaskAttributeListCtrl.h"

#include "..\shared\EnToolBar.h"
#include "..\shared\ToolBarHelper.h"
#include "..\shared\MenuIconMgr.h"

/////////////////////////////////////////////////////////////////////////////

class CTDLTaskCtrlBase;
class CToDoCtrlData;
class CTDCImageList;
class CContentMgr;
class CUIThemeFile;

/////////////////////////////////////////////////////////////////////////////
// CTDLTaskAttributeDlgCtrl window

class CTDLTaskAttributeCtrl : public CWnd
{
public:
	CTDLTaskAttributeCtrl(const CToDoCtrlData& data,
						  const CContentMgr& mgrContent,
						  const CTDCImageList& ilIcons,
						  const TDCCOLEDITVISIBILITY& vis,
						  const CTDCReminderHelper& rems,
						  const CTDCCustomAttribDefinitionArray& aCustAttribDefs);

	virtual ~CTDLTaskAttributeCtrl();

public:
	BOOL Create(CWnd* pParent, UINT nID, const CRect& rPos = CRect(0, 0, 0, 0));
	void SetUITheme(const CUIThemeFile& theme);
	BOOL HasFocus() const { return m_lcAttributes.HasFocus(); }

	void SetDefaultAutoListData(const TDCAUTOLISTDATA& tldDefault);
	void SetAutoListData(TDC_ATTRIBUTE nAttribID, const TDCAUTOLISTDATA& tld);
	void GetAutoListData(TDC_ATTRIBUTE nAttribID, TDCAUTOLISTDATA& tld) const;
	void SetAutoListDataReadOnly(TDC_ATTRIBUTE nAttribID, BOOL bReadOnly);
	BOOL IsAutoListDataReadOnly(TDC_ATTRIBUTE nAttribID) const;

	TDC_ATTRIBUTE GetSelectedAttributeID() const;
	CString GetSelectedAttributeLabel() const;
	BOOL SetSelectedTaskIDs(const CDWordArray& aTaskIDs);
	BOOL IsAnyTaskSelected(const CDWordArray& aTaskIDs) const;

	void RefreshSelectedTasksValues(const CTDCAttributeMap& mapAttribIDs = TDCA_ALL);
	void RefreshSelectedTasksValue(TDC_ATTRIBUTE nAttribID);
	void RefreshDateTimeFormatting();

	void SetCurrentFolder(const CString& sFolder);
	void SetCompletionStatus(const CString& sStatus);
	void SetPriorityColors(const CDWordArray& aColors);
	void SetPercentDoneIncrement(int nAmount);
	void SetTimeTrackTaskID(DWORD dwTaskID);
	void SetNumPriorityRiskLevels(int nNumLevels);

	void RedrawValue(TDC_ATTRIBUTE nAttribID);
	void SelectValue(TDC_ATTRIBUTE nAttribID);

	// Only attributes 'edited' by this control have getters
	CString GetTaskTitle() const;
	CString GetAllocBy() const;
	CString GetStatus() const;
	CString GetExternalID() const;
	CString GetVersion() const;

	int GetAllocTo(CStringArray& aMatched, CStringArray& aMixed) const;
	int GetCategories(CStringArray& aMatched, CStringArray& aMixed) const;
	int GetTags(CStringArray& aMatched, CStringArray& aMixed) const;

	int GetFileLinks(CStringArray& aFiles) const;
	int GetDependencies(CTDCDependencyArray& aDepends) const;

	int GetPercentDone() const;
	int GetPriority() const;
	int GetRisk() const;

	BOOL GetTimeEstimate(TDCTIMEPERIOD& timeEst) const;
	BOOL GetTimeSpent(TDCTIMEPERIOD& timeSpent) const;
	BOOL GetCost(TDCCOST& cost) const;
	BOOL GetFlag() const;
	BOOL GetLock() const;

	COleDateTime GetStartDate() const;
	COleDateTime GetDueDate() const;
	COleDateTime GetDoneDate() const;
	COleDateTime GetStartTime() const;
	COleDateTime GetDueTime() const;
	COleDateTime GetDoneTime() const;

	BOOL GetCustomAttributeData(const CString& sAttribID, TDCCADATA& data, BOOL bFormatted = FALSE) const;
	int GetCustomAttributeAutoListData(const CString& sAttribID, CStringArray& aItems) const;

	void SaveState(CPreferences& prefs, LPCTSTR szKey) const;
	void LoadState(const CPreferences& prefs, LPCTSTR szKey);

	void OnCustomAttributesChange();
	void OnAttributeVisibilityChange();

	void Flush() { m_lcAttributes.EndEdit(); }

protected:
	CEnToolBar m_toolbar;
	CToolbarHelper m_tbHelper;
	CTDLTaskAttributeListCtrl m_lcAttributes;
	CMenuIconMgr m_mgrMenuIcons;

protected:
	virtual int DoModal() { return IDCANCEL; }

protected:
	//{{AFX_MSG(CTDLTaskAttributeListCtrl)
	//}}AFX_MSG
	afx_msg int OnCreate(LPCREATESTRUCT pCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnItemChanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint pos);

	afx_msg void OnToggleSingleClickEditing();
	afx_msg void OnToggleGrouping();
	afx_msg void OnToggleSorting();
	afx_msg void OnMoveAttributeUp();
	afx_msg void OnMoveAttributeDown();
	afx_msg void OnResetAttributeMoves();
	afx_msg void OnSetLabelBkgndColor();
	afx_msg void OnClearLabelBkgndColor();
	afx_msg void OnCopyAttributeValue();
	afx_msg void OnPasteAttributeValue();
	afx_msg void OnClearAttributeValue();

	afx_msg void OnUpdateMoveAttributeUp(CCmdUI* pCmdUI);
	afx_msg void OnUpdateMoveAttributeDown(CCmdUI* pCmdUI);
	afx_msg void OnUpdateResetAttributeMoves(CCmdUI* pCmdUI);
	afx_msg void OnUpdateSetLabelBkgndColor(CCmdUI* pCmdUI);
	afx_msg void OnUpdateClearLabelBkgndColor(CCmdUI* pCmdUI);
	afx_msg void OnUpdateCopyAttributeValue(CCmdUI* pCmdUI);
	afx_msg void OnUpdatePasteAttributeValue(CCmdUI* pCmdUI);
	afx_msg void OnUpdateClearAttributeValue(CCmdUI* pCmdUI);

	afx_msg LRESULT OnEditTaskAttribute(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnClearTaskAttribute(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnEditTaskReminder(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnClearTaskReminder(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnAttributeEdited(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnAutoItemAddedOrDeleted(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnToggleTimeTracking(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnAddTimeToLogFile(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSelectDependencies(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnGetLinkTooltip(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnGetFileIcon(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnDisplayLink(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()

protected:
	void UpdateToolbarButtons();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TDLTASKATTRIBUTEDLGCTRL_H__4173F4AA_2026_4FE1_B669_E6631D12C8F1__INCLUDED_)
