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

#include "..\shared\enedit.h"

//////////////////////////////////////////////////////////////////////

class CTDLTaskDependencyEdit : public CEnEdit  
{
public:
	CTDLTaskDependencyEdit();
	virtual ~CTDLTaskDependencyEdit();

	void GetDependencies(CTDCDependencyArray& aDepends) const;
	void SetDependencies(const CTDCDependencyArray& aDepends);
	
	void DDX(CDataExchange* pDX, CTDCDependencyArray& aValues);

protected:
	CTDCDependencyArray m_aDepends;

protected:
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
};

#endif 

