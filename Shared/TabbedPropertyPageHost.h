// TabbedPropertyPageHost.h: interface for the CTabbedPropertyPageHost class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TABBEDPROPERTYPAGEHOST_H__4BC7E883_D652_462C_BB3A_6102BA978FBC__INCLUDED_)
#define AFX_TABBEDPROPERTYPAGEHOST_H__4BC7E883_D652_462C_BB3A_6102BA978FBC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PropertyPageHost.h"
#include "tabctrlex.h"

class CTabbedPropertyPageHost : public CPropertyPageHost  
{
public:
	CTabbedPropertyPageHost(DWORD dwFlags = 0, ETabOrientation orientation = e_tabTop);
	virtual ~CTabbedPropertyPageHost();

	BOOL AddPage(CPropertyPage* pPage, LPCTSTR szTitle, DWORD dwItemData = 0);
	BOOL SetActivePage(int nIndex, BOOL bAndFocus = TRUE);
	BOOL SetActivePage(CPropertyPage* pPage, BOOL bAndFocus = TRUE);

	void SetImageList(CImageList* pImages);
	void SetBackgroundColor(COLORREF color);

protected:
	CTabCtrlEx m_tabCtrl;
	CImageList* m_pImages;

	// Generated message map functions
protected:
	//{{AFX_MSG(CTabbedPropertyPageHost)
	//}}AFX_MSG
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSelchangeTabcontrol(NMHDR* pNMHDR, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()

protected:
	BOOL ConstructTabControl();
	BOOL CalcTabPageRects(int nWidth, int nHeight, CRect& rTabs, CRect& rPages) const;
};

#endif // !defined(AFX_TABBEDPROPERTYPAGEHOST_H__4BC7E883_D652_462C_BB3A_6102BA978FBC__INCLUDED_)
