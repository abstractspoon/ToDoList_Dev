#pragma once

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

	mutable HTREEITEM m_htiTooltip;

protected:
	virtual void PreSubclassWindow();
	virtual INT_PTR OnToolHitTest(CPoint point, TOOLINFO* pTI) const;
	
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnShowTooltip(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg LRESULT OnTitleColumnWidthChange(WPARAM wp, LPARAM lp);

	DECLARE_MESSAGE_MAP()

protected:
	void InitTooltipFont() const;
};

/////////////////////////////////////////////////////////////////////////////

