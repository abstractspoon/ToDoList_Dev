// LightBox.h: interface for the CLightBox class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LIGHTBOX_H__B86BA77E_2353_44E0_9F47_1EB2F02009A1__INCLUDED_)
#define AFX_LIGHTBOX_H__B86BA77E_2353_44E0_9F47_1EB2F02009A1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Subclass.h"
#include "runtimedlg.h"
#include "hookmgr.h"

class CLightBox;

#ifndef WS_EX_LAYERED
#  define WS_EX_LAYERED 0x00080000
#  define LWA_COLORKEY 1 
#  define LWA_ALPHA 2 
#endif

class CLightBoxMgr : public CHookMgr<CLightBoxMgr>
{
	friend class CHookMgr<CLightBoxMgr>;
	
public:
	CLightBoxMgr();
	~CLightBoxMgr();

	static BOOL Initialize(CWnd* pMainWnd, COLORREF crBkgnd = GetSysColor(COLOR_3DLIGHT), int nOpaquePercent = 50);
	static void Release();
	
protected:
	CMap<HWND, HWND, CLightBox*, CLightBox*&> m_mapCtrls;
	COLORREF m_crBkgnd;
	int m_nOpaquePercent;
	
protected:
	static CLightBoxMgr& Instance() { return CHookMgr<CLightBoxMgr>::GetInstance(); }
	BOOL AttachLightBox(CWnd* pWnd);
	void CleanupLightBoxes();
	
	virtual BOOL OnCallWndProc(const MSG& msg);
};

class CLightBox : public CRuntimeDlg, public CSubclassWnd  
{
	DECLARE_DYNAMIC(CLightBox)
		
public:
	CLightBox();
	virtual ~CLightBox();
	
	BOOL Initialize(CWnd* pWnd, COLORREF crBkgnd = GetSysColor(COLOR_3DHILIGHT), int nOpaquePercent = 50);
	
protected:
	COLORREF m_crBkgnd;
	int m_nOpaquePercent;
	
	typedef DWORD (WINAPI *PSLWA)(HWND, DWORD, BYTE, DWORD);
	PSLWA m_pSetLayeredWindowAttributes;
	
	// Generated message map functions
protected:
	//{{AFX_MSG(CLightBox)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
		
protected:
	virtual LRESULT WindowProc(HWND hRealWnd, UINT msg, WPARAM wp, LPARAM lp);
	
	BOOL ShowTransparentWnd(BOOL bShow = TRUE);
};

#endif // !defined(AFX_LIGHTBOX_H__B86BA77E_2353_44E0_9F47_1EB2F02009A1__INCLUDED_)
