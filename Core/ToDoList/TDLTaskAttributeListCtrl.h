#if !defined(AFX_TDLTASKATTRIBUTELISTCTRL_H__4173F4AA_2026_4FE1_B669_E6631D12C8F1__INCLUDED_)
#define AFX_TDLTASKATTRIBUTELISTCTRL_H__4173F4AA_2026_4FE1_B669_E6631D12C8F1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TDLTaskAttributeListCtrl.h : header file
//

#include "ToDoCtrlDataUtils.h"

#include "..\shared\InputListCtrl.h"
#include "..\shared\AutoComboBox.h"
#include "..\shared\CheckComboBox.h"

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
	void RefreshSelectedTaskAttributeValues(BOOL bForceClear = FALSE);
	void SetAttributeVisibility(const TDCCOLEDITVISIBILITY& vis);
	void SetCustomAttributeDefinitions(const CTDCCustomAttribDefinitionArray& aAttribDefs);

	COLORREF GetColor() const;
	CString GetIcon() const;
	BOOL GetTimeEstimate(TDCTIMEPERIOD& timeEst) const;
	BOOL GetTimeSpent(TDCTIMEPERIOD& timeSpent) const;
	int GetAllocTo(CStringArray& aAllocTo) const;
	int GetAllocTo(CStringArray& aMatched, CStringArray& aMixed) const;
	CString GetAllocBy() const;
	CString GetStatus() const;
	int GetCategories(CStringArray& aCats) const;
	int GetCategories(CStringArray& aMatched, CStringArray& aMixed) const;
	int GetDependencies(CTDCDependencyArray& aDepends) const;
	int GetTags(CStringArray& aTags) const;
	int GetTags(CStringArray& aMatched, CStringArray& aMixed) const;
	int GetFileLinks(CStringArray& aFiles) const;
	CString GetExternalID() const;
	int GetPercent() const;
	int GetPriority() const;
	int GetRisk() const;
	BOOL GetCost(TDCCOST& cost) const;
	BOOL GetFlag() const;
	BOOL GetLock() const;
	BOOL GetRecurrence(TDCRECURRENCE& tr) const;
	CString GetVersion() const;
	COleDateTime GetStartDate() const;
	COleDateTime GetDueDate() const;
	COleDateTime GetDoneDate() const;
	BOOL GetCustomAttributeData(const CString& sAttribID, TDCCADATA& data) const;

protected:
	const CTDLTaskCtrlBase& m_taskCtrl;
	const CToDoCtrlData& m_data;
	const CTDCImageList& m_ilIcons;

	CTDCTaskFormatter m_formatter;
	TDCCOLEDITVISIBILITY m_vis;
	CTDCCustomAttribDefinitionArray m_aCustomAttribDefs;

	CAutoComboBox m_cbSingleSelection;
	CCheckComboBox m_cbMultiSelection;

protected:
	//{{AFX_MSG(CTDLTaskAttributeListCtrl)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	//}}AFX_MSG
	afx_msg void OnTextEditOK(NMHDR* pNMHDR, LRESULT* pResult);

	DECLARE_MESSAGE_MAP()

protected:
	virtual IL_COLUMNTYPE GetCellType(int nRow, int nCol) const;
	virtual BOOL CanEditCell(int nRow, int nCol) const;
	virtual void PrepareControl(CWnd& ctrl, int nRow, int nCol);
	virtual void EditCell(int nItem, int nCol, BOOL bBtnClick);

	virtual COLORREF GetItemBackColor(int nItem, int nCol, BOOL bSelected, BOOL bDropHighlighted, BOOL bWndFocus) const;
	virtual COLORREF GetItemTextColor(int nItem, int nCol, BOOL bSelected, BOOL bDropHighlighted, BOOL bWndFocus) const;
	virtual void DrawCellText(CDC* pDC, int nRow, int nCol, const CRect& rText, const CString& sText, COLORREF crText, UINT nDrawTextFlags);

protected:
	void Populate();
	void CheckAddAttribute(TDC_ATTRIBUTE nAttribID, UINT nAttribResID);
	TDC_ATTRIBUTE GetAttributeID(int nRow, BOOL bResolveCustomTimeFields = FALSE) const;
	COleDateTime GetDate(TDC_ATTRIBUTE nDate, TDC_ATTRIBUTE nTime) const;
	CWnd* GetEditControl(int nRow, int nCol);

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TDLTASKATTRIBUTELISTCTRL_H__4173F4AA_2026_4FE1_B669_E6631D12C8F1__INCLUDED_)
