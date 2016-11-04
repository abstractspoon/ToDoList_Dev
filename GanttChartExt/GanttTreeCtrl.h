#pragma once

#include "..\shared\tooltipctrlex.h"

/////////////////////////////////////////////////////////////////////////////
// CGanttTreeCtrl

class CGanttTreeCtrl : public CTreeCtrl
{
	DECLARE_DYNAMIC(CGanttTreeCtrl)

public:
	CGanttTreeCtrl();
	virtual ~CGanttTreeCtrl();

	bool ProcessMessage(MSG* pMsg);

protected:
	int m_nTitleColumnWidth;
	CToolTipCtrlEx m_tooltip;

protected:
	virtual void PreSubclassWindow();
	virtual int OnToolHitTest(CPoint point, TOOLINFO* pTI) const;
	
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnShowTooltip(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg LRESULT OnTitleColumnWidthChange(WPARAM wp, LPARAM lp);

	DECLARE_MESSAGE_MAP()

protected:
	BOOL InitTooltip();
};

/////////////////////////////////////////////////////////////////////////////

