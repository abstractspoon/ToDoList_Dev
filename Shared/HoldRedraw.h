// SetRedraw.h: interface and implementation of the CSetRedraw class.
//
/////////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_HOLDREDRAW_H__7E73ADE2_3848_4ED1_9E8B_8881813B4262__INCLUDED_)
#define AFX_HOLDREDRAW_H__7E73ADE2_3848_4ED1_9E8B_8881813B4262__INCLUDED_

#include <afxtempl.h>

/////////////////////////////////////////////////////////////////////////////////

enum
{
	NCR_UPDATE		= 0x01,
	NCR_PAINT		= 0x02,
	NCR_NCPAINT		= 0x04,
	NCR_ERASEBKGND	= 0x08,
};

/////////////////////////////////////////////////////////////////////////////////

class CRedrawAll
{
public:
	CRedrawAll(HWND hWnd, DWORD dwFlags = NCR_PAINT | NCR_NCPAINT);
	virtual ~CRedrawAll();

protected:
	HWND m_hWnd;
	DWORD m_dwFlags;
};

/////////////////////////////////////////////////////////////////////////////////

class CHoldRedraw : protected CRedrawAll
{
public:
	CHoldRedraw(HWND hWnd, DWORD dwFlags = NCR_PAINT | NCR_NCPAINT);
	virtual ~CHoldRedraw();

	static BOOL IsHeld(HWND hWnd) { return (s_listHwnd.Find(hWnd) != NULL); }

protected:
	static CList<HWND, HWND&> s_listHwnd;
};

/////////////////////////////////////////////////////////////////////////////////

class CNcRedraw // note: there is no inheritance intentionally.
{
public:
	CNcRedraw(HWND hWnd);
	virtual ~CNcRedraw();

protected:
	HWND m_hWnd;
};

/////////////////////////////////////////////////////////////////////////////////

class CLockUpdates : protected CRedrawAll
{
public:
	CLockUpdates(HWND hwnd, DWORD dwFlags = 0);
	virtual ~CLockUpdates();
};
#endif

/////////////////////////////////////////////////////////////////////////////////
