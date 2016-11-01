#pragma once

/////////////////////////////////////////////////////////////////////////////

const UINT WM_GTCN_TITLECOLUMNWIDTHCHANGE = ;

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
	mutable HTREEITEM m_htiTooltip;

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnShowTooltip(NMHDR* pNMHDR, LRESULT* pResult);

	DECLARE_MESSAGE_MAP()

	virtual void PreSubclassWindow();
	virtual INT_PTR OnToolHitTest(CPoint point, TOOLINFO* pTI) const;

};

/////////////////////////////////////////////////////////////////////////////

