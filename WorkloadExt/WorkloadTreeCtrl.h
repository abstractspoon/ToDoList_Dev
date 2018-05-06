#if !defined(AFX_WORKLOADTREECTRL_H__1571B442_7ED5_45D8_A040_C359EAE9FDE1__INCLUDED_)
#define AFX_WORKLOADTREECTRL_H__1571B442_7ED5_45D8_A040_C359EAE9FDE1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\shared\tooltipctrlex.h"
#include "..\shared\fontcache.h"
#include "..\shared\treectrlhelper.h"

/////////////////////////////////////////////////////////////////////////////
// CWorkloadTreeCtrl

class CWorkloadTreeCtrl : public CTreeCtrl
{
	DECLARE_DYNAMIC(CWorkloadTreeCtrl)

public:
	CWorkloadTreeCtrl();
	virtual ~CWorkloadTreeCtrl();

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
#endif // AFX_WORKLOADTREECTRL_H__1571B442_7ED5_45D8_A040_C359EAE9FDE1__INCLUDED_

