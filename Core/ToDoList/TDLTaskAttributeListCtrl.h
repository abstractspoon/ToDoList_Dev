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
#include "..\shared\tooltipctrlex.h"

/////////////////////////////////////////////////////////////////////////////

class CTDLTaskCtrlBase;
class CToDoCtrlData;
class CTDCImageList;

/////////////////////////////////////////////////////////////////////////////
// CTDLTaskAttributeListCtrl window

class CTDLTaskAttributeListCtrl : public CInputListCtrl
{
	// Private helper
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
		virtual DROPEFFECT OnDragScroll(CWnd* pWnd, DWORD dwKeyState, CPoint point);
		
		BOOL CanDropFiles(const CPoint& point, COleDataObject* pDataObject, int& nRow, CStringArray& aFiles) const;
		BOOL CanDropFiles(TDC_ATTRIBUTE nAttribID, const CStringArray& aFiles) const;
	};
	
	friend class CFileDropTarget;

// Construction -------------------------------------------------------------
public:
	CTDLTaskAttributeListCtrl(const CToDoCtrlData& data,
							  const CContentMgr& mgrContent,
							  const CTDCImageList& ilIcons,
							  const TDCCOLEDITVISIBILITY& vis,
							  const CTDCReminderHelper& rems,
							  const CTDCCustomAttribDefinitionArray& aCustAttribDefs);

	virtual ~CTDLTaskAttributeListCtrl();

public:
	BOOL Create(CWnd* pParent, UINT nID);
	void ToggleSortDirection();
	void ToggleGrouping();
	BOOL IsGrouped() const { return m_bGrouped; }

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
	void SetCurrentFolder(const CString& sFolder);
	void SetCompletionStatus(const CString& sStatus);
	void SetPriorityColors(const CDWordArray& aColors);
	void SetPercentDoneIncrement(int nAmount);
	void SetTimeTrackTaskID(DWORD dwTaskID);

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

protected:
	const CToDoCtrlData& m_data;
	const CTDCImageList& m_ilIcons;
	const TDCCOLEDITVISIBILITY& m_vis;
	const CTDCReminderHelper& m_reminders;
	const CTDCCustomAttribDefinitionArray& m_aCustomAttribDefs;

	CTDCTaskFormatter m_formatter;
	CTDCTaskCalculator m_calculator;
	CTDCMultiTasker m_multitasker;

	TDCAUTOLISTDATA m_tldAll, m_tldDefault;

	CDWordArray m_aSelectedTaskIDs;
	CString m_sCompletionStatus;
	CString m_sCurrentFolder;
	CDWordArray m_aPriorityColors;
	CTDCAttributeMap m_mapReadOnlyListData;

	BOOL m_bGrouped;
	BOOL m_bSplitting;
	float m_fAttribColProportion;
	DWORD m_dwTimeTrackingTask;

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
	CToolTipCtrlEx m_tooltip;
	CFileDropTarget m_dropFiles;
	
	mutable CIconCache m_iconCache;

protected:
	//{{AFX_MSG(CTDLTaskAttributeListCtrl)
	//}}AFX_MSG
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnCaptureChanged(CWnd* pWnd);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint pos);

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
	afx_msg LRESULT OnSingleFileLinkNotifyBrowse(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()

protected:
	BOOL PreTranslateMessage(MSG* pMsg);
	int OnToolHitTest(CPoint point, TOOLINFO* pTI) const;

protected:
	virtual IL_COLUMNTYPE GetCellType(int nRow, int nCol) const;
	virtual BOOL CanEditCell(int nRow, int nCol) const;
	virtual void PrepareControl(CWnd& ctrl, int nRow, int nCol);
	virtual void EditCell(int nItem, int nCol, BOOL bBtnClick);
	virtual BOOL DeleteSelectedCell();
	virtual void OnCancelEdit();
	virtual BOOL GetButtonRect(int nRow, int nCol, CRect& rBtn) const;

	virtual COLORREF GetItemBackColor(int nItem, int nCol, BOOL bSelected, BOOL bDropHighlighted, BOOL bWndFocus) const;
	virtual COLORREF GetItemTextColor(int nItem, int nCol, BOOL bSelected, BOOL bDropHighlighted, BOOL bWndFocus) const;
	virtual void DrawCellText(CDC* pDC, int nRow, int nCol, const CRect& rText, const CString& sText, COLORREF crText, UINT nDrawTextFlags);
	virtual BOOL DrawButton(CDC* pDC, int nRow, int nCol, const CString& sText, BOOL bSelected, CRect& rBtn);
	virtual UINT GetTextDrawFlags(int nCol) const;

protected:
	CString GetValueText(TDC_ATTRIBUTE nAttribID) const;
	TDC_ATTRIBUTE GetAttributeID(int nRow, BOOL bResolveCustomTimeFields = FALSE) const;
	TDC_ATTRIBUTE MapCustomDateToTime(TDC_ATTRIBUTE nDateAttribID) const;
	TDC_ATTRIBUTE MapCustomTimeToDate(TDC_ATTRIBUTE nTimeAttribID) const;

	void Populate();
	int GetGroupAttributes(TDC_ATTRIBUTEGROUP nGroup, CMap<TDC_ATTRIBUTE, TDC_ATTRIBUTE, CString, LPCTSTR>& mapAttrib) const;
	BOOL WantAddAttribute(TDC_ATTRIBUTE nAttribID) const;
	int CheckAddAttribute(TDC_ATTRIBUTE nAttribID, UINT nAttribResID);
	int GetRow(TDC_ATTRIBUTE nAttribID) const { return FindItemFromData(nAttribID); }
	int GetDateRow(TDC_ATTRIBUTE nTimeAttribID) const;
	void HideAllControls(const CWnd* pWndIgnore = NULL);
	CWnd* GetEditControl(int nRow, BOOL bBtnClick);
	void RefreshSelectedTasksValue(int nRow);
	void NotifyParentEdit(int nRow);
	BOOL DrawIcon(CDC* pDC, const CString& sIcon, const CRect& rText, BOOL bIconIsFile);
 	BOOL GetCellPrompt(int nRow, const CString& sText, CString& sPrompt) const;
	void HandleTimePeriodEdit(int nRow, BOOL bBtnClick);
	CString FormatDate(const COleDateTime& date, BOOL bAndTime) const;
	CString FormatTime(const COleDateTime& date, BOOL bNotSetIsEmpty) const;
	BOOL CheckRecreateCombo(int nRow, CEnCheckComboBox& combo);
	BOOL RowValueVaries(int nRow) const;
	void GetSplitterRect(CRect& rSplitBar) const;
	void RecalcColumnWidths(int nAttribColWidth = -1, int cx = -1);
	BOOL SetValueText(int nRow, const CString& sNewText);

	int HitTestButtonID(int nRow) const;
	int HitTestButtonID(int nRow, const CRect& rBtn) const;
	BOOL CanClickButton(TDC_ATTRIBUTE nAttribID, int nBtnID, const CString& sCellText) const;
	DWORD GetButtonState(TDC_ATTRIBUTE nAttribID, int nBtnID, const CString& sCellText, DWORD dwBaseState) const;
	HICON GetButtonIcon(int nIconID, BOOL bDisabled = FALSE) const;
	HICON GetButtonIcon(TDC_ATTRIBUTE nAttribID, int nBtnID, DWORD dwState) const;
	BOOL DrawIconButton(CDC* pDC, TDC_ATTRIBUTE nAttribID, int nBtnID, const CString& sText, DWORD dwBaseState, CRect& rBtn) const;

	void PrepareMultiSelCombo(int nRow, const CStringArray& aDefValues, const CStringArray& aUserValues, CEnCheckComboBox& combo);
	void PrepareSingleSelCombo(int nRow, const CStringArray& aDefValues, const CStringArray& aUserValues, CEnCheckComboBox& combo);
	void PrepareSingleFileEdit(int nRow, const CString& sValue);
	void PrepareDatePicker(int nRow, TDC_ATTRIBUTE nFallbackDate);
	void PrepareTimeOfDayCombo(int nRow);
	void PrepareTimePeriodEdit(int nRow);

	static int ParseMultiSelValues(const CString& sValues, CStringArray& aMatched, CStringArray& aMixed);
	static CString FormatMultiSelItems(const CStringArray& aMatched, const CStringArray& aMixed);
	static CPoint GetIconPos(const CRect& rText);
	static BOOL IsCustomTime(TDC_ATTRIBUTE nAttribID);
	static int HitTestExtraButton(int nRow, const CRect& rBtn, const CPoint& ptMouse, int nNumExtraBtns);
	static BOOL GetExtraButtonRect(const CRect& rBtn, int nExtraBtn, CRect& rExtraBtn);

private:
	// ---------------------------------------------------------------------

	struct GROUPITEM
	{
		GROUPITEM() : dwItemData(0), nGroupID(0), rItem(0, 0, 0, 0) {}

		DWORD dwItemData;
		int nGroupID;
		CRect rItem;
	};

	class CSortedGroupItemArray : CArray<GROUPITEM, GROUPITEM&>
	{
	public:
		CSortedGroupItemArray(const CEnListCtrl& list) : m_list(list) {}

		void Clear() { RemoveAll(); }
		int GetNextItem(int nKeyPress);

	protected:
		const CEnListCtrl& m_list;

	protected:
		int CheckBuildArray();
		int GetPageSize(int nFrom, BOOL bDown) const;
		int FindItem(DWORD dwItemData) const;

		static int SortProc(const void* item1, const void* item2);
	};

	CSortedGroupItemArray m_aSortedGroupedItems;

	// ---------------------------------------------------------------------

	struct ATTRIBGROUP
	{
		TDC_ATTRIBUTEGROUP nGroup;
		CString sName;
	};

	class CSortedGroupedHeaderArray : public CArray<ATTRIBGROUP, ATTRIBGROUP&>
	{
	public:
		CSortedGroupedHeaderArray(BOOL bSortAscending);

	protected:
		static int AscendingSortProc(const void* item1, const void* item2);
		static int DescendingSortProc(const void* item1, const void* item2);
	};
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TDLTASKATTRIBUTELISTCTRL_H__4173F4AA_2026_4FE1_B669_E6631D12C8F1__INCLUDED_)
