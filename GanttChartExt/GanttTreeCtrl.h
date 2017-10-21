#pragma once

#include "..\shared\tooltipctrlex.h"
#include "..\shared\fontcache.h"
#include "..\shared\treectrlhelper.h"

/////////////////////////////////////////////////////////////////////////////
// CGanttTreeCtrl

class CGanttTreeCtrl : public CTreeCtrl
{
	DECLARE_DYNAMIC(CGanttTreeCtrl)

public:
	CGanttTreeCtrl();
	virtual ~CGanttTreeCtrl();

	CTreeCtrlHelper& TCH() { return m_tch; }
	const CTreeCtrlHelper& TCH() const { return m_tch; }

	CFontCache& Fonts() { return m_fonts; }
	const CFontCache& Fonts() const { return m_fonts; }
	
	bool ProcessMessage(MSG* pMsg);
	void FilterToolTipMessage(MSG* pMsg);

	void ShowCheckboxes(BOOL bShow = TRUE);
	void ShowTaskIcons(BOOL bShow = TRUE);

	HIMAGELIST GetTaskIcon(DWORD dwTaskID, int& iImageIndex) const;
	
protected:
	int m_nTitleColumnWidth;
	CToolTipCtrlEx m_tooltip;
	CFontCache m_fonts;
	CTreeCtrlHelper m_tch;
	CImageList m_ilCheckboxes, m_ilTaskIcons;

protected:
	virtual void PreSubclassWindow();
	
protected:
	afx_msg void OnDestroy();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnShowTooltip(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg LRESULT OnTitleColumnWidthChange(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnSetFont(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnToolHitTest(WPARAM wp, LPARAM lp);

	DECLARE_MESSAGE_MAP()

protected:
	BOOL InitTooltip();
};

/////////////////////////////////////////////////////////////////////////////

