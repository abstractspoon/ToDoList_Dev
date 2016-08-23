// InterSpy.h: interface for the CInterSpy class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INTERSPY_H__BB79D08D_4C2C_4092_816E_B2CB6B61F388__INCLUDED_)
#define AFX_INTERSPY_H__BB79D08D_4C2C_4092_816E_B2CB6B61F388__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "hookmgr.h"

#include <afxtempl.h>

enum // command ids
{
	IS_CMD_RESET,
	IS_CMD_ADDMSG,

};

enum // init flags
{
	IS_NOKICKIDLE		= 0x0001,
	IS_NODUPLICATES		= 0x0002,
	IS_RESETONSTART		= 0x0004,
	IS_AUTOSTARTOUTPUT	= 0x0008,
};


struct IS_COPYDATA
{
	UINT uCmd;
	WPARAM wParam;
	LPARAM lParam;
	char sMsg[1024]; // first item is message then params delimetered by '|'
};

/////////////////////////////////////////////////////////////////////////////

class CInterSpy : protected CHookMgr<CInterSpy>
{
	friend class CHookMgr<CInterSpy>;

public:
	static BOOL Initialize(DWORD dwFlags = IS_NOKICKIDLE | IS_NODUPLICATES | IS_RESETONSTART | IS_AUTOSTARTOUTPUT);
  static void Release();
	static void IncludeMsg(UINT uMsg);
	static void ExcludeMsg(UINT uMsg);

	virtual ~CInterSpy();

protected:
	HWND m_hwndOutput;
	DWORD m_dwFlags;
	CMap<UINT, UINT, UINT, UINT&> m_mapExclusions;
	CMap<HWND, HWND, UINT, UINT&> m_mapLastMsgs;
	CString m_sOutputWndPath;

protected:
	CInterSpy();

	virtual BOOL OnCallWndProc(const MSG& msg);
	virtual BOOL OnGetMessage(const MSG& msg);

protected:
	HWND GetOutputWnd(BOOL bStartup = FALSE);
	BOOL ProcessMessage(const MSG& msg);
	void IncludeMsg(UINT uMsg, BOOL bInclude);
	void SetFlags(DWORD dwFlags);

};

#endif // !defined(AFX_INTERSPY_H__BB79D08D_4C2C_4092_816E_B2CB6B61F388__INCLUDED_)
