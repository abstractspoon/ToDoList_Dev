// DockManager.h: interface for the CDockManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DOCKMANAGER_H__E0312E72_217B_4520_88F2_6708F4148F9D__INCLUDED_)
#define AFX_DOCKMANAGER_H__E0312E72_217B_4520_88F2_6708F4148F9D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Subclass.h"

enum DM_POS
{
	DMP_UNDOCKED = -1,
	DMP_LEFT,
	DMP_RIGHT,
	DMP_BELOW,
};

class CDockManager : public CSubclassWnd, public CSubclasser  
{
public:
	CDockManager();
	virtual ~CDockManager();

	BOOL Initialize(CWnd* pMainWnd, CWnd* pDockWnd, 
					DM_POS nPos = DMP_UNDOCKED,	DM_POS nLastPos = DMP_RIGHT,
					int nWidthDocked = -1, int nWidthDockedMax = -1, 
					int nHeightDocked = -1, int nHeightDockedMax = -1);
	BOOL Initialized() { return IsHooked(); }
	BOOL Dock(DM_POS nPos);
	BOOL UnDock();

	inline DM_POS GetDockPos() const { return m_nDockPos; }
	inline DM_POS GetLastDockPos() const { return IsDocked() ? m_nDockPos : m_nLastDockPos; }
	inline BOOL IsDocked() const { return m_nDockPos != DMP_UNDOCKED; }
	CRect GetUnDockedRect() const;
	int GetDockedWidth(BOOL bMaximized) const { return bMaximized ? m_nWidthDockedMax : m_nWidthDocked; }
	int GetDockedHeight(BOOL bMaximized) const { return bMaximized ? m_nHeightDockedMax : m_nHeightDocked; }

	inline BOOL IsMaximized() const { return ::IsZoomed(GetHwnd()); }

protected:
	DM_POS m_nDockPos, m_nLastDockPos;
	CRect m_rUndocked;
	int m_nWidthDocked, m_nWidthDockedMax;
	int m_nHeightDocked, m_nHeightDockedMax;
	CSize m_sizeMainMin, m_sizeDockMin;
	BOOL m_bResizeUpdate;
	BOOL m_bSizeUpdate;
	CSubclassWnd m_scDockWnd;
	CRect m_rOSBorders; // Windows 10 really

protected:
	virtual LRESULT WindowProc(HWND hRealWnd, UINT msg, WPARAM wp, LPARAM lp); // main wnd
	virtual LRESULT ScWindowProc(HWND hRealWnd, UINT msg, WPARAM wp, LPARAM lp); // dock wnd
	virtual CSubclasser* GetTopSubclasser() { return this; }

	void UpdateDockWindowPos();
	void UpdateMainWindowPos();
	
	CRect GetWorkArea(BOOL bMaximized = TRUE);
	void MoveWindow(CWnd* pWnd, const CRect& rect);
	CSize GetMinMaximizedSize();
	void FitDockWindowToWorkArea();
	void InitializeOSBorders();
	
	// pseudo message handlers
	void OnMaximize();
	void OnRestore();
	void OnMinMaxInfo(LPMINMAXINFO pMMI, BOOL bMain);

	static BOOL IsValidDock(DM_POS nPos);
};

#endif // !defined(AFX_DOCKMANAGER_H__E0312E72_217B_4520_88F2_6708F4148F9D__INCLUDED_)
