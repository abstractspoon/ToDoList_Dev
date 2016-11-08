// TrayIcon.h : header file for CTrayIcon
//
//////////////////////////////////////////////////////////////////////////

#if !defined ( TRAYICON_H )
#define TRAYICON_H

#include "subclass.h"
#include <afxtempl.h>

#ifndef NIIF_NONE
#	define NIIF_NONE	0x00
#	define NIIF_INFO	0x01
#	define NIIF_WARNING 0x02
#	define NIIF_ERROR	0x03
#endif

struct NM_TRAYICON
{ 
    NMHDR hdr; 
    POINT ptAction; 
};

class CTrayIcon : public CWnd, protected CSubclasser
{
// Construction
public:
	CTrayIcon();
	virtual ~CTrayIcon();

	BOOL Create(CWnd* pParentWnd, UINT uID, UINT uIDIcon, UINT uIDTip = 0);
	BOOL Create(CWnd* pParentWnd, UINT uID, UINT uIDIcon, LPCTSTR sTip);
	void ShowTrayIcon(BOOL bShow = TRUE);
	void StartAnimation();
	void StopAnimation();
	void SetAnimationIcons(UINT pIconIDs[], int nNumIcons);
	void SetAnimationDelay(int nDelay);
	BOOL SetTip(UINT uIDNewTip);
	BOOL SetTip(LPCTSTR sNewTip);
	BOOL SetIcon(UINT uIDNewIcon);
	BOOL SetIcon(HICON hNewIcon, BOOL bAutoDelete);

	BOOL ShowBalloon(LPCTSTR szText, LPCTSTR szTitle = NULL,
                     DWORD dwIcon = NIIF_NONE, UINT uTimeout = 5);

// Attributes
private:
	CString m_sTip;
	HICON m_hIcon;
	BOOL m_bVisible;
	NM_TRAYICON m_nm;
	UINT m_nPrevMsg;
	CUIntArray m_aAnimationIconIDs;
	BOOL m_bAnimationOn;
	int m_nAnimationDelay;
	int m_nCurIcon;
	BOOL m_bTemporaryIcon;
	BOOL m_bIconOwner;

	CSubclassWnd m_scParent;

// Generated message map functions
protected:
	//{{AFX_MSG(CTrayIcon)
    afx_msg LRESULT OnTrayIconNotify(WPARAM wParam, LPARAM lParam);
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	virtual LRESULT ScWindowProc(HWND hRealWnd, UINT msg, WPARAM wp, LPARAM lp);

private:
	BOOL AddToTray();
	BOOL DeleteFromTray();
	BOOL ModifyTip(LPCTSTR sNewTip);
	BOOL ModifyIcon(HICON hNewIcon);

};

#endif
