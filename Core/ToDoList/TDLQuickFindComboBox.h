#pragma once

//////////////////////////////////////////////////////////////////////////

#include "..\shared\autocombobox.h"
#include "..\shared\enedit.h"
#include "..\shared\icon.h"

// CTDLQuickFindComboBox /////////////////////////////////////////////////

enum TDLQF_BUTTONS
{
	TDLQFB_PREV = 1,
	TDLQFB_NEXT
};

//////////////////////////////////////////////////////////////////////////

class CTDLQuickFindComboBox : public CAutoComboBox
{
	DECLARE_DYNAMIC(CTDLQuickFindComboBox)

public:
	CTDLQuickFindComboBox(DWORD dwFlags = 0);
	virtual ~CTDLQuickFindComboBox();

	void ClearEdit() { m_bEditChange = FALSE; }

protected:
	class CTDLQuickFindEdit : public CEnEdit
	{
	public:
		int OnToolHitTest(CPoint point, TOOLINFO* pTI) const
		{
			return CEnEdit::OnToolHitTest(point, pTI);
		}
	};

	CTDLQuickFindEdit m_quickEdit;
	CIcon m_iconNext, m_iconPrev;

protected:
	virtual int OnToolHitTest(CPoint point, TOOLINFO* pTI) const;
	virtual void OnSubclassChild(HWND hwndChild);

protected:
	afx_msg LRESULT OnEEBtnClick(WPARAM wp, LPARAM lp);

	DECLARE_MESSAGE_MAP()
};


