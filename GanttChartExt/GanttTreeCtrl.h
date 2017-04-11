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

protected:
	int m_nTitleColumnWidth;
	CToolTipCtrlEx m_tooltip;
	CFontCache m_fonts;
	CTreeCtrlHelper m_tch;

protected:
	virtual void PreSubclassWindow();
	virtual int OnToolHitTest(CPoint point, TOOLINFO* pTI) const;
	
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnShowTooltip(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg LRESULT OnTitleColumnWidthChange(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnSetFont(WPARAM wp, LPARAM lp);

	DECLARE_MESSAGE_MAP()

protected:
	BOOL InitTooltip();
};

/////////////////////////////////////////////////////////////////////////////

