// TransTextMgr.h: interface for the CTransTextMgr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TRANSWND_H__1F1C6D57_D2C6_4C06_9E7C_1E0C54B65D30__INCLUDED_)
#define AFX_TRANSWND_H__1F1C6D57_D2C6_4C06_9E7C_1E0C54B65D30__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\shared\subclass.h"

#include <afxtempl.h>

//////////////////////////////////////////////////////////////////////

enum
{
	TWS_HANDLENONE		= 0x00,
	TWS_HANDLETOOLTIPS	= 0x02,
	TWS_HANDLEMENUPOPUP	= 0x04,

	// more here

	TWS_HANDLEALL		= 0xff,
};

//////////////////////////////////////////////////////////////////////

class CTransWnd : public CSubclassWnd
{
public:
	static CTransWnd* NewTransWnd(const CString& sClass, DWORD dwStyle);

	CTransWnd(DWORD dwOptions = TWS_HANDLEALL);
	virtual ~CTransWnd();
	
	void UpdateMenu() { TranslateMenu(::GetMenu(GetHwnd()), FALSE); }
	void AllowTranslation(BOOL bAllow) { m_bAllowTranslate = bAllow; }
	void PostHookWindow();
	
protected:
	DWORD m_dwOptions;
	BOOL m_bInit;
	BOOL m_bAllowTranslate;

protected:
	virtual LRESULT WindowProc(HWND hRealWnd, UINT msg, WPARAM wp, LPARAM lp);
	virtual BOOL HookWindow(HWND hRealWnd, CSubclasser* pSubclasser);
	virtual void Initialize();
	
	BOOL TranslateText(CString& sText);
	void TranslateMenu(HMENU hMenu, BOOL bToplevelOnly);
	BOOL HasFlag(DWORD dwFlag);
};

//////////////////////////////////////////////////////////////////////

class CTransComboBox : public CTransWnd
{
public:
	CTransComboBox() : CTransWnd(TWS_HANDLENONE) {}

protected:
	virtual void Initialize();
	virtual LRESULT WindowProc(HWND hRealWnd, UINT msg, WPARAM wp, LPARAM lp);

	// needed by Initialize()
	struct CBITEMDATA
	{
		CString sText;
		DWORD dwData;
	};
	typedef CArray<CBITEMDATA, CBITEMDATA&> CCbItemArray;
};

//////////////////////////////////////////////////////////////////////

class CTransComboBoxEx : public CTransWnd
{
public:
	CTransComboBoxEx() : CTransWnd(TWS_HANDLENONE) {}

protected:
	virtual void Initialize();
	virtual LRESULT WindowProc(HWND hRealWnd, UINT msg, WPARAM wp, LPARAM lp);
};

//////////////////////////////////////////////////////////////////////

class CTransListBox : public CTransWnd
{
public:
	CTransListBox(BOOL bCheckListBox = FALSE) : CTransWnd(TWS_HANDLENONE), m_bCheckLB(bCheckListBox) { }

protected:
	virtual void Initialize();
	virtual LRESULT WindowProc(HWND hRealWnd, UINT msg, WPARAM wp, LPARAM lp);

protected:
	BOOL m_bCheckLB;

	// needed by Initialize()
	struct LBITEMDATA
	{
		CString sText;
		DWORD dwData;
		int nCheck;
	};
	typedef CArray<LBITEMDATA, LBITEMDATA&> CLbItemArray;
};

//////////////////////////////////////////////////////////////////////

class CTransTabCtrl : public CTransWnd
{
public:
	CTransTabCtrl() : CTransWnd(TWS_HANDLENONE) {}

protected:
	virtual void Initialize();
	virtual LRESULT WindowProc(HWND hRealWnd, UINT msg, WPARAM wp, LPARAM lp);
};

//////////////////////////////////////////////////////////////////////

class CTransHeaderCtrl : public CTransWnd
{
public:
	CTransHeaderCtrl() : CTransWnd(TWS_HANDLENONE) {}

protected:
	virtual void Initialize();
	virtual LRESULT WindowProc(HWND hRealWnd, UINT msg, WPARAM wp, LPARAM lp);
};

//////////////////////////////////////////////////////////////////////

class CTransListCtrl : public CTransWnd
{
public:
	CTransListCtrl() : CTransWnd(TWS_HANDLENONE) {}

protected:
	virtual void Initialize();
	virtual LRESULT WindowProc(HWND hRealWnd, UINT msg, WPARAM wp, LPARAM lp);
};

//////////////////////////////////////////////////////////////////////

class CTransTooltips : public CTransWnd
{
public:
	CTransTooltips() : CTransWnd(TWS_HANDLENONE) {}

protected:
	virtual void Initialize();
	virtual LRESULT WindowProc(HWND hRealWnd, UINT msg, WPARAM wp, LPARAM lp);
};

//////////////////////////////////////////////////////////////////////

class CTransToolBar : public CTransWnd
{
public:
	CTransToolBar() : CTransWnd(TWS_HANDLETOOLTIPS) {}

protected:
	virtual void Initialize();
	virtual LRESULT WindowProc(HWND hRealWnd, UINT msg, WPARAM wp, LPARAM lp);
};

//////////////////////////////////////////////////////////////////////

class CTransStatusBar : public CTransWnd
{
public:
	CTransStatusBar() : CTransWnd(TWS_HANDLETOOLTIPS) {}

protected:
	virtual void Initialize();
	virtual LRESULT WindowProc(HWND hRealWnd, UINT msg, WPARAM wp, LPARAM lp);
};

//////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_TRANSWND_H__1F1C6D57_D2C6_4C06_9E7C_1E0C54B65D30__INCLUDED_)
