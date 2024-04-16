#if !defined(AFX_TDLTASKATTRIBUTEDLGCTRL_H__4173F4AA_2026_4FE1_B669_E6631D12C8F1__INCLUDED_)
#define AFX_TDLTASKATTRIBUTEDLGCTRL_H__4173F4AA_2026_4FE1_B669_E6631D12C8F1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TDLTaskAttributeListCtrl.h : header file
//

#include "TDLTaskAttributeListCtrl.h"

#include "..\shared\EnToolBar.h"
#include "..\shared\RuntimeDlg.h"

/////////////////////////////////////////////////////////////////////////////

class CTDLTaskCtrlBase;
class CToDoCtrlData;
class CTDCImageList;
class CContentMgr;

/////////////////////////////////////////////////////////////////////////////
// CTDLTaskAttributeDlgCtrl window

class CTDLTaskAttributeCtrl : public CRuntimeDlg
{
public:
	CTDLTaskAttributeCtrl(const CToDoCtrlData& data,
							 const CContentMgr& mgrContent,
							 const CTDCImageList& ilIcons,
							 const TDCCOLEDITVISIBILITY& vis,
							 const CTDCCustomAttribDefinitionArray& aCustAttribDefs);

	virtual ~CTDLTaskAttributeCtrl();

public:
	BOOL Create(CWnd* pParent, UINT nID, const CRect& rPos = CRect(0, 0, 0, 0));

	void SetDefaultAutoListData(const TDCAUTOLISTDATA& tldDefault);
	void SetAutoListData(TDC_ATTRIBUTE nAttribID, const TDCAUTOLISTDATA& tld);
	void GetAutoListData(TDC_ATTRIBUTE nAttribID, TDCAUTOLISTDATA& tld) const;
	void SetAutoListDataReadOnly(TDC_ATTRIBUTE nAttribID, BOOL bReadOnly);

	TDC_ATTRIBUTE GetSelectedAttributeID() const;
	CString GetSelectedAttributeLabel() const;

	void RefreshSelectedTasksValues();
	void RefreshSelectedTasksValues(const CTDCAttributeMap& mapAttribIDs);
	void RefreshSelectedTasksValue(TDC_ATTRIBUTE nAttribID);
	void RefreshDateTimeFormatting();

	BOOL SetSelectedTaskIDs(const CDWordArray& aTaskIDs);
	void SetCompletionStatus(const CString& sStatus);
	void SetPriorityColors(const CDWordArray& aColors);
	void SetPercentDoneIncrement(int nAmount);

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
	CTDLTaskAttributeListCtrl m_lcAttributes;

protected:
	virtual BOOL OnInitDialog();
	virtual int DoModal() { return IDCANCEL; }

protected:
	//{{AFX_MSG(CTDLTaskAttributeListCtrl)
	//}}AFX_MSG
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSetFocus(CWnd* pOldWnd);

	afx_msg LRESULT OnFileLinkWantTooltip(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnFileLinkDisplay(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TDLTASKATTRIBUTEDLGCTRL_H__4173F4AA_2026_4FE1_B669_E6631D12C8F1__INCLUDED_)
