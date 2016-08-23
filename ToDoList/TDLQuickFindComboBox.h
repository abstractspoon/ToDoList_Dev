#pragma once

#include "..\shared\autocombobox.h"

// CTDLQuickFindComboBox

class CTDLQuickFindComboBox : public CAutoComboBox
{
	DECLARE_DYNAMIC(CTDLQuickFindComboBox)

public:
	CTDLQuickFindComboBox(DWORD dwFlags = 0);
	virtual ~CTDLQuickFindComboBox();

	void ClearEdit() { m_bEditChange = FALSE; }

protected:
	DECLARE_MESSAGE_MAP()
};


