// RecurrenceEdit.h: interface for the CRecurringTaskEdit class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DEPENDENCYEDIT_H__4EE655E3_F4B1_44EA_8AAA_39DD459AD8A8__INCLUDED_)
#define AFX_DEPENDENCYEDIT_H__4EE655E3_F4B1_44EA_8AAA_39DD459AD8A8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ToDoItem.h"
#include "TDLDialog.h"
#include "TDLTaskComboBox.h"

#include "..\shared\enedit.h"
#include "..\shared\inputlistctrl.h"
#include "..\shared\tabbedcombobox.h"

//////////////////////////////////////////////////////////////////////

class CTDLTaskDependencyEdit : public CEnEdit  
{
public:
	CTDLTaskDependencyEdit(const CToDoCtrlData& data);
	virtual ~CTDLTaskDependencyEdit();

	void GetDependencies(CTDCDependencyArray& aDepends) const;
	void SetDependencies(const CTDCDependencyArray& aDepends);
	
	void DDX(CDataExchange* pDX, CTDCDependencyArray& aValues);

protected:
	CTDCDependencyArray m_aDepends;
	BOOL m_bNotifyingParent;

	const CToDoCtrlData& m_data;

protected:
	virtual void OnBtnClick(UINT nID);
	virtual void PreSubclassWindow();

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CTDLTaskDependencyEdit)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	afx_msg BOOL OnChange();

	DECLARE_MESSAGE_MAP()

	int Parse(CTDCDependencyArray& aDepends) const;
	BOOL DoEdit();
};

/////////////////////////////////////////////////////////////////////////////
// CTDLTaskDependencyOptionDlg dialog

class CTDLTaskDependencyListCtrl : public CInputListCtrl
{
public:
	CTDLTaskDependencyListCtrl(const CToDoCtrlData& data);

	void SetDependencies(const CTDCDependencyArray& aDepends);
	int GetDependencies(CTDCDependencyArray& aDepends) const;

protected:
	CTDLTaskComboBox m_cbTasks;

	const CToDoCtrlData& m_data;

// Implementation
protected:
	virtual BOOL CanEditCell(int nRow, int nCol) const;
	virtual void EditCell(int nItem, int nCol, BOOL bBtnClick);
	virtual COLORREF GetItemBackColor(int nItem, int nCol, BOOL bSelected, BOOL bDropHighlighted, BOOL bWndFocus) const;
	virtual void PrepareControl(CWnd& ctrl, int nRow, int nCol);

protected:
	// Generated message map functions
	//{{AFX_MSG(CTDLTaskDependencyListCtrl)
	//}}AFX_MSG
	afx_msg void OnTaskComboCancel();
	afx_msg void OnTaskComboOK();

	DECLARE_MESSAGE_MAP()

protected:
	void FillTaskCombo();
	void FillTaskCombo(const TODOSTRUCTURE* pTDS, int nLevel);
};

// ----------------------------------------------

class CTDLTaskDependencyEditDlg : public CTDLDialog
{
// Construction
public:
	CTDLTaskDependencyEditDlg(const CToDoCtrlData& data, const CTDCDependencyArray& aDepends, CWnd* pParent = NULL);   // standard constructor

	int GetDependencies(CTDCDependencyArray& aDepends) const;

protected:
// Dialog Data
	//{{AFX_DATA(CTDLTaskDependencyOptionDlg)
	//}}AFX_DATA
	CTDLTaskDependencyListCtrl m_lcDependencies;
	CTDCDependencyArray m_aDepends;
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTDLTaskDependencyOptionDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnOK();
	//}}AFX_VIRTUAL
	virtual BOOL OnInitDialog();

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CTDLTaskDependencyOptionDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif // AFX_DEPENDENCYEDIT_H__4EE655E3_F4B1_44EA_8AAA_39DD459AD8A8__INCLUDED_
