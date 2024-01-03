#if !defined(AFX_TDLTASKATTRIBUTELISTCTRL_H__4173F4AA_2026_4FE1_B669_E6631D12C8F1__INCLUDED_)
#define AFX_TDLTASKATTRIBUTELISTCTRL_H__4173F4AA_2026_4FE1_B669_E6631D12C8F1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TDLTaskAttributeListCtrl.h : header file
//

#include "ToDoCtrlDataUtils.h"
#include "TDLPriorityComboBox.h"
#include "TDLRiskComboBox.h"
#include "tdltaskdependencyedit.h"

#include "..\shared\InputListCtrl.h"
#include "..\shared\AutoComboBox.h"
#include "..\shared\CheckComboBox.h"
#include "..\shared\DateTimeCtrlEx.h"
#include "..\shared\FileComboBox.h"
#include "..\shared\TimeComboBox.h"
#include "..\shared\TimeEdit.h"
#include "..\shared\Icon.h"

/////////////////////////////////////////////////////////////////////////////

class CTDLTaskCtrlBase;
class CToDoCtrlData;
class CTDCImageList;

/////////////////////////////////////////////////////////////////////////////
// CTDLTaskAttributeListCtrl window

class CTDLTaskAttributeListCtrl : public CInputListCtrl
{
// Construction
public:
	CTDLTaskAttributeListCtrl(const CTDLTaskCtrlBase& taskCtrl,
							  const CToDoCtrlData& data,
							  const CTDCImageList& ilIcons,
							  const TDCCOLEDITVISIBILITY& defaultVis);

	virtual ~CTDLTaskAttributeListCtrl();

public:
	void SetAttributeVisibility(const TDCCOLEDITVISIBILITY& vis);
	void SetCustomAttributeDefinitions(const CTDCCustomAttribDefinitionArray& aAttribDefs);
	void SetPercentDoneIncrement(int nAmount);

	void SetDefaultAutoListData(const TDCAUTOLISTDATA& tldDefault);
	void SetAutoListData(const TDCAUTOLISTDATA& tld, TDC_ATTRIBUTE nAttribID);
	void GetAutoListData(TDCAUTOLISTDATA& tld, TDC_ATTRIBUTE nAttribID) const;

	void RefreshSelectedTaskValues(BOOL bForceClear = FALSE);
	void RefreshSelectedTaskValue(TDC_ATTRIBUTE nAttribID);
	void RefreshDateTimeFormatting();
	void RefreshCompletionStatus();

	void RedrawValue(TDC_ATTRIBUTE nAttribID);

	// Note: 'Color', 'Icon' and 'Recurrence' are handled by parent
	BOOL GetTimeEstimate(TDCTIMEPERIOD& timeEst) const;
	BOOL GetTimeSpent(TDCTIMEPERIOD& timeSpent) const;
	int GetAllocTo(CStringArray& aMatched, CStringArray& aMixed) const;
	CString GetAllocBy() const;
	CString GetStatus() const;
	int GetCategories(CStringArray& aMatched, CStringArray& aMixed) const;
	int GetDependencies(CTDCDependencyArray& aDepends) const;
	int GetTags(CStringArray& aMatched, CStringArray& aMixed) const;
	int GetFileLinks(CStringArray& aFiles) const;
	CString GetExternalID() const;
	int GetPercent() const;
	int GetPriority() const;
	int GetRisk() const;
	BOOL GetCost(TDCCOST& cost) const;
	BOOL GetFlag() const;
	BOOL GetLock() const;
	CString GetVersion() const;
	COleDateTime GetStartDate() const;
	COleDateTime GetDueDate() const;
	COleDateTime GetDoneDate() const;
	COleDateTime GetStartTime() const;
	COleDateTime GetDueTime() const;
	COleDateTime GetDoneTime() const;
	BOOL GetCustomAttributeData(const CString& sAttribID, TDCCADATA& data) const;

protected:
	const CTDLTaskCtrlBase& m_taskCtrl;
	const CToDoCtrlData& m_data;
	const CTDCImageList& m_ilIcons;

	CTDCTaskFormatter m_formatter;
	TDCCOLEDITVISIBILITY m_vis;
	CTDCCustomAttribDefinitionArray m_aCustomAttribDefs;
	TDCAUTOLISTDATA m_tldAll, m_tldDefault;

	CAutoComboBox m_cbSingleSelection;
	CCheckComboBox m_cbMultiSelection;
	CDateTimeCtrlEx m_datePicker;
	CTimeComboBox m_cbTimeOfDay;
	CTDLPriorityComboBox m_cbPriority;
	CTDLRiskComboBox m_cbRisk;
	CTDLTaskDependencyEdit m_eDepends;
	CSpinButtonCtrl m_spinPercent;
	CTimeEdit m_eTimePeriod;
	CFileComboBox m_cbFileLinks;

	CIcon m_iconTrackTime, m_iconAddTime, m_iconLink;
	
	static CIcon s_iconApp;

protected:
	//{{AFX_MSG(CTDLTaskAttributeListCtrl)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	//}}AFX_MSG
	afx_msg void OnTextEditOK(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDateCloseUp(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDateChange(NMHDR* pNMHDR, LRESULT* pResult);

	afx_msg void OnDependsChange();
	afx_msg void OnTimePeriodChange();

	afx_msg void OnComboCloseUp(UINT nCtrlID);
	afx_msg void OnComboEditCancel(UINT nCtrlID);
	afx_msg void OnComboEditChange(UINT nCtrlID);

	afx_msg LRESULT OnAutoComboAddDelete(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnEnEditButtonClick(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnFileLinkWantIcon(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnFileLinkWantTooltip(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnFileLinkDisplay(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()

protected:
	virtual IL_COLUMNTYPE GetCellType(int nRow, int nCol) const;
	virtual BOOL CanEditCell(int nRow, int nCol) const;
	virtual void PrepareControl(CWnd& ctrl, int nRow, int nCol);
	virtual void EditCell(int nItem, int nCol, BOOL bBtnClick);
	virtual BOOL DeleteSelectedCell();
	virtual void OnCancelEdit();

	virtual COLORREF GetItemBackColor(int nItem, int nCol, BOOL bSelected, BOOL bDropHighlighted, BOOL bWndFocus) const;
	virtual COLORREF GetItemTextColor(int nItem, int nCol, BOOL bSelected, BOOL bDropHighlighted, BOOL bWndFocus) const;
	virtual void DrawCellText(CDC* pDC, int nRow, int nCol, const CRect& rText, const CString& sText, COLORREF crText, UINT nDrawTextFlags);

protected:
	CString GetValueText(TDC_ATTRIBUTE nAttribID) const;
	TDC_ATTRIBUTE GetAttributeID(int nRow, BOOL bResolveCustomTimeFields = FALSE) const;

	void Populate();
	void CheckAddAttribute(TDC_ATTRIBUTE nAttribID, UINT nAttribResID);
	void HideAllControls(const CWnd* pWndIgnore = NULL);
	CWnd* GetEditControl(int nRow, BOOL bBtnClick);
	void RefreshSelectedTaskValue(int nRow);
	LRESULT NotifyParentEdit(TDC_ATTRIBUTE nAttribID);
	BOOL DrawIcon(CDC* pDC, const CString& sIcon, const CRect& rText, BOOL bIconIsFile);

	void PrepareMultiSelCombo(int nRow, const CStringArray& aDefValues, const CStringArray& aUserValues);
	void PrepareSingleSelCombo(int nRow, const CStringArray& aDefValues, const CStringArray& aUserValues);
	void PrepareDatePicker(int nRow, TDC_ATTRIBUTE nFallbackDate);
	void PrepareTimeCombo(int nRow);
	void PrepareTimePeriodEdit(int nRow);

	static int ParseMultiSelValues(const CString& sValues, CStringArray& aMatched, CStringArray& aMixed);
	static CString FormatMultiSelItems(const CStringArray& aMatched, const CStringArray& aMixed);
	static CPoint GetIconPos(const CRect& rText);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TDLTASKATTRIBUTELISTCTRL_H__4173F4AA_2026_4FE1_B669_E6631D12C8F1__INCLUDED_)
