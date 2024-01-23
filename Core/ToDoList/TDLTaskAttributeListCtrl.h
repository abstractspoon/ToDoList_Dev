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
#include "TDLIconComboBox.h"

#include "..\shared\InputListCtrl.h"
#include "..\shared\EnCheckComboBox.h"
#include "..\shared\DateTimeCtrlEx.h"
#include "..\shared\FileComboBox.h"
#include "..\shared\TimeComboBox.h"
#include "..\shared\TimeEdit.h"
#include "..\shared\Icon.h"
#include "..\shared\OleDropTargetEx.h"

/////////////////////////////////////////////////////////////////////////////

class CTDLTaskCtrlBase;
class CToDoCtrlData;
class CTDCImageList;

/////////////////////////////////////////////////////////////////////////////
// CTDLTaskAttributeListCtrl window

class CTDLTaskAttributeListCtrl : public CInputListCtrl
{
	friend class CFileDropTarget;

// Construction
public:
	CTDLTaskAttributeListCtrl(const CToDoCtrlData& data,
							  const CContentMgr& mgrContent,
							  const CTDCImageList& ilIcons,
							  const TDCCOLEDITVISIBILITY& defaultVis);

	virtual ~CTDLTaskAttributeListCtrl();

public:
	void SetAttributeVisibility(const TDCCOLEDITVISIBILITY& vis);
	void SetCustomAttributeDefinitions(const CTDCCustomAttribDefinitionArray& aAttribDefs);
	void SetPercentDoneIncrement(int nAmount);

	void SetDefaultAutoListData(const TDCAUTOLISTDATA& tldDefault);
	void SetAutoListData(TDC_ATTRIBUTE nAttribID, const TDCAUTOLISTDATA& tld);
	void GetAutoListData(TDC_ATTRIBUTE nAttribID, TDCAUTOLISTDATA& tld) const;
	void SetAutoListDataReadOnly(TDC_ATTRIBUTE nAttribID, BOOL bReadOnly);

	void RefreshSelectedTasksValues();
	void RefreshSelectedTasksValue(TDC_ATTRIBUTE nAttribID);
	void RefreshDateTimeFormatting();

	BOOL SetSelectedTaskIDs(const CDWordArray& aTaskIDs);
	void SetCompletionStatus(const CString& sStatus);
	void SetPriorityColors(const CDWordArray& aColors);

	void RedrawValue(TDC_ATTRIBUTE nAttribID);
	void SelectValue(TDC_ATTRIBUTE nAttribID);

	// Only attributes 'edited' by this control have getters
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

protected:
	const CToDoCtrlData& m_data;
	const CTDCImageList& m_ilIcons;

	CTDCTaskFormatter m_formatter;
	CTDCMultiTasker m_multitasker;

	TDCCOLEDITVISIBILITY m_vis;
	CTDCCustomAttribDefinitionArray m_aCustomAttribDefs;
	TDCAUTOLISTDATA m_tldAll, m_tldDefault;

	CDWordArray m_aSelectedTaskIDs;
	CString m_sCompletionStatus;
	CDWordArray m_aPriorityColors;
	CTDCAttributeMap m_mapReadOnlyListData;

	CEnCheckComboBox m_cbTextAndNumbers;
	CDateTimeCtrlEx m_datePicker;
	CTimeComboBox m_cbTimeOfDay;
	CTDLPriorityComboBox m_cbPriority;
	CTDLRiskComboBox m_cbRisk;
	CTDLTaskDependencyEdit m_eDepends;
	CSpinButtonCtrl m_spinPercent;
	CTimeEdit m_eTimePeriod;
	CFileComboBox m_cbMultiFileLink;
	CFileEdit m_eSingleFileLink;
	CTDLIconComboBox m_cbCustomIcons;

	class CFileDropTarget : public COleDropTargetEx
	{
	public:
		CFileDropTarget(CTDLTaskAttributeListCtrl* pAtributeList);

	protected:
		CTDLTaskAttributeListCtrl* m_pAttributeList;
		int m_nDropHighlightedRow;

	protected:
		virtual DROPEFFECT OnDragEnter(CWnd* pWnd, COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
		virtual DROPEFFECT OnDragOver(CWnd* pWnd, COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
		virtual BOOL OnDrop(CWnd* pWnd, COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point);
		virtual void OnDragLeave(CWnd* pWnd);

		BOOL CanDropFiles(const CPoint& point, COleDataObject* pDataObject, int& nRow, CStringArray& aFiles) const;
		BOOL CanDropFiles(TDC_ATTRIBUTE nAttribID, const CStringArray& aFiles) const;

	};
	CFileDropTarget m_dropFiles;

	static CIcon s_iconTrackTime, s_iconAddTime, s_iconLink, s_iconBrowse, s_iconApp;

protected:
	//{{AFX_MSG(CTDLTaskAttributeListCtrl)
	//}}AFX_MSG
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);

#ifdef _DEBUG
	afx_msg void OnPaint();
#endif

	afx_msg void OnTextEditOK(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDateCloseUp(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDateChange(NMHDR* pNMHDR, LRESULT* pResult);

	afx_msg void OnDependsChange();
	afx_msg void OnTimePeriodChange();
	afx_msg void OnSingleFileLinkChange();

	afx_msg void OnComboKillFocus(UINT nCtrlID);
	afx_msg void OnComboCloseUp(UINT nCtrlID);
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
	virtual BOOL GetButtonRect(int nRow, int nCol, CRect& rButton) const;

	virtual COLORREF GetItemBackColor(int nItem, int nCol, BOOL bSelected, BOOL bDropHighlighted, BOOL bWndFocus) const;
	virtual COLORREF GetItemTextColor(int nItem, int nCol, BOOL bSelected, BOOL bDropHighlighted, BOOL bWndFocus) const;
	virtual void DrawCellText(CDC* pDC, int nRow, int nCol, const CRect& rText, const CString& sText, COLORREF crText, UINT nDrawTextFlags);
	virtual BOOL DrawButton(CDC* pDC, int nRow, int nCol, const CString& sText, BOOL bSelected, CRect& rButton);

protected:
	CString GetValueText(TDC_ATTRIBUTE nAttribID) const;
	TDC_ATTRIBUTE GetAttributeID(int nRow, BOOL bResolveCustomTimeFields = FALSE) const;
	TDC_ATTRIBUTE MapCustomDateToTime(TDC_ATTRIBUTE nDateAttribID) const;
	TDC_ATTRIBUTE MapCustomTimeToDate(TDC_ATTRIBUTE nTimeAttribID) const;

	void Populate();
	void CheckAddAttribute(TDC_ATTRIBUTE nAttribID, UINT nAttribResID);
	int GetRow(TDC_ATTRIBUTE nAttribID) const { return FindItemFromData(nAttribID); }
	void HideAllControls(const CWnd* pWndIgnore = NULL);
	CWnd* GetEditControl(int nRow, BOOL bBtnClick);
	void RefreshSelectedTasksValue(int nRow);
	void NotifyParentEdit(int nRow);
	BOOL DrawIcon(CDC* pDC, const CString& sIcon, const CRect& rText, BOOL bIconIsFile);
 	BOOL GetCellPrompt(int nRow, const CString& sText, CString& sPrompt) const;
	void HandleSingleFileLinkEdit(int nRow, const CString& sFile, BOOL bBtnClick);
	CString FormatDate(const COleDateTime& date, BOOL bAndTime) const;
	CString FormatTime(const COleDateTime& date, BOOL bNotSetIsEmpty) const;
	BOOL CheckRecreateCombo(int nRow, CEnCheckComboBox& combo);

	void PrepareMultiSelCombo(int nRow, const CStringArray& aDefValues, const CStringArray& aUserValues, CEnCheckComboBox& combo);
	void PrepareSingleSelCombo(int nRow, const CStringArray& aDefValues, const CStringArray& aUserValues, CEnCheckComboBox& combo);
	void PrepareDatePicker(int nRow, TDC_ATTRIBUTE nFallbackDate);
	void PrepareTimeOfDayCombo(int nRow);
	void PrepareTimePeriodEdit(int nRow);

	static int ParseMultiSelValues(const CString& sValues, CStringArray& aMatched, CStringArray& aMixed);
	static CString FormatMultiSelItems(const CStringArray& aMatched, const CStringArray& aMixed);
	static CPoint GetIconPos(const CRect& rText);
	static BOOL IsCustomTime(TDC_ATTRIBUTE nAttribID);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TDLTASKATTRIBUTELISTCTRL_H__4173F4AA_2026_4FE1_B669_E6631D12C8F1__INCLUDED_)
