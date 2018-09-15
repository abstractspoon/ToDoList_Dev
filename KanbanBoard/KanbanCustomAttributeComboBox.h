#pragma once

#include "KanbanStruct.h"

// CKanbanCustomAttributeComboBox

class CKanbanCustomAttributeComboBox : public CComboBox
{
	DECLARE_DYNAMIC(CKanbanCustomAttributeComboBox)

public:
	CKanbanCustomAttributeComboBox();
	virtual ~CKanbanCustomAttributeComboBox();

	void SetAttributeDefinitions(const CKanbanCustomAttributeDefinitionArray& aAttribDefs);

	CString GetSelectedAttributeID() const;
	BOOL SetSelectedAttributeID(const CString& sAttribID);

protected:
	CKanbanCustomAttributeDefinitionArray m_aCustAttribDefs;
	
protected:
	DECLARE_MESSAGE_MAP()

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual void PreSubclassWindow();

protected:
	void BuildCombo();
};


