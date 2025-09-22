#if !defined(AFX_FINDTASKEXPRESSIONLISTCTRL_H__42272309_6C54_4901_A56D_D6FDA87F1E48__INCLUDED_)
#define AFX_FINDTASKEXPRESSIONLISTCTRL_H__42272309_6C54_4901_A56D_D6FDA87F1E48__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FindTaskExpressionListCtrl.h : header file
//

#include "tdcenum.h"
#include "tdcstruct.h"
#include "tdlfindtaskattributecombobox.h"
#include "tdlprioritycombobox.h"
#include "tdlriskcombobox.h"
#include "tdcimagelist.h"
#include "tdlIconComboBox.h"
#include "TDLRegularityComboBox.h"

#include "..\shared\InputListCtrl.h"
#include "..\shared\timeedit.h"
#include "..\shared\datetimectrlex.h"
#include "..\shared\checkcombobox.h"

#include <afxtempl.h>

/////////////////////////////////////////////////////////////////////////////

class CContentMgr;

/////////////////////////////////////////////////////////////////////////////
// CTDLFindTaskExpressionListCtrl window

class CTDLFindTaskExpressionListCtrl : public CInputListCtrl
{
public:
	CTDLFindTaskExpressionListCtrl(const CContentMgr& mgrContent);
	virtual ~CTDLFindTaskExpressionListCtrl();

	BOOL IsValid() const { return m_aSearchParams.IsValid(); }
	BOOL IsBalanced(int& nNumGroupStarts, int& nNumGroupEnds) const { return m_aSearchParams.IsBalanced(nNumGroupStarts, nNumGroupEnds); }

	void SetSearchParams(const SEARCHPARAM& param);
	void SetSearchParams(const CSearchParamArray& params);
	int GetSearchParams(CSearchParamArray& params) const;

	void SetCustomAttributes(const CTDCCustomAttribDefinitionArray& aAttribDefs);
	void SetAttributeListData(const TDCAUTOLISTDATA& tld, TDC_ATTRIBUTE nAttribID);
	void SetActiveTasklist(const CString& sTasklist, BOOL bWantDefaultIcons);
	void SetNumPriorityRiskLevels(int nNumLevels);
	void SetPriorityColors(const CDWordArray& aColors);
	void SetISODateFormat(BOOL bIso);

	BOOL AddRule();
	BOOL DeleteSelectedRule();
	BOOL CanDeleteSelectedRule() const { return CanDeleteSelectedCell(); }
	BOOL HasRules() const { return m_aSearchParams.GetSize(); }
	BOOL HasRule(TDC_ATTRIBUTE nAttribID) const;

	void MoveSelectedRuleUp();
	BOOL CanMoveSelectedRuleUp() const;
	void MoveSelectedRuleDown();
	BOOL CanMoveSelectedRuleDown() const;

	virtual BOOL IsEditing() const;
	void CancelEdit();
	void ClearSearch();

protected:
	CComboBox						m_cbOperators;
	CTDLFindTaskAttributeComboBox	m_cbAttributes;
	CComboBox						m_cbAndOr;
	CTDLRegularityComboBox			m_cbRecurrence;
	CDateTimeCtrlEx					m_dtcDate;
	CTimeEdit						m_eTimePeriod;
	CCheckComboBox					m_cbListValues;
	CTDLPriorityComboBox			m_cbPriority;
	CTDLRiskComboBox				m_cbRisk;
	CTDLIconComboBox				m_cbCustomIcons;

	CSearchParamArray				m_aSearchParams;
	CTDCCustomAttribDefinitionArray m_aCustAttribDefs;
	TDCAUTOLISTDATA					m_tldListContents;
	CTDCImageList					m_ilIcons;
	BOOL							m_bIsoDateFormat;

	const CContentMgr&				m_mgrContent;

protected:
	virtual void PreSubclassWindow();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

protected:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnAttribEditCancel();
	afx_msg void OnAttribEditOK();
	afx_msg void OnOperatorEditCancel();
	afx_msg void OnOperatorEditOK();
	afx_msg void OnAndOrEditCancel();
	afx_msg void OnAndOrEditOK();
	afx_msg void OnListValuesEditCancel();
	afx_msg void OnListValuesEditOK();
	afx_msg void OnPriorityEditCancel();
	afx_msg void OnPriorityEditOK();
	afx_msg void OnRiskEditCancel();
	afx_msg void OnRiskEditOK();
	afx_msg void OnRecurrenceEditCancel();
	afx_msg void OnRecurrenceEditOK();
	afx_msg void OnCustomIconEditChange();
	afx_msg void OnValueEditOK(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDateChange(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDateCloseUp(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnTimeChange();
	afx_msg LRESULT OnTimeUnitsChange(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnEEBtnClick(WPARAM wp, LPARAM lp);

	DECLARE_MESSAGE_MAP()

	virtual int GetItemIndent(int nItem) const;
	virtual void EditCell(int nItem, int nCol, BOOL bBtnClick);
	virtual BOOL DeleteSelectedCell();
	virtual BOOL CanEditCell(int nRow, int nCol) const;
	virtual void OnCancelEdit();
	virtual void InitState();
	virtual void DrawCellText(CDC* pDC, int nRow, int nCol, const CRect& rText, const CString& sText, COLORREF crText, UINT nDrawTextFlags);
	virtual void HideAllControls(const CWnd* pWndIgnore = NULL);
	virtual COLORREF GetItemTextColor(int nItem, int nCol, BOOL bSelected, BOOL bDropHighlighted, BOOL bWndFocus) const;
	virtual IL_COLUMNTYPE GetCellType(int nRow, int nCol) const;

	void PrepareEdit(int nRow, int nCol);
	void PrepareControl(CWnd& ctrl, int nRow, int nCol);
	void BuildListCtrl();
	int InsertRule(int nRow, const SEARCHPARAM& sp);
	int GetRuleCount() const { return m_aSearchParams.GetSize(); }
	CWnd* GetEditControl(int nRow, int nCol);
	void ValidateListData() const;
	void UpdateValueColumnText(int nRow);
	void AddOperatorToCombo(FIND_OPERATOR op);
	void MoveSelectedRule(BOOL bUp);

	static CString GetOpName(FIND_OPERATOR op);
};

/////////////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_FINDTASKEXPRESSIONLISTCTRL_H__42272309_6C54_4901_A56D_D6FDA87F1E48__INCLUDED_)
