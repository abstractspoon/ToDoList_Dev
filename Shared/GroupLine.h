// EditPrompt.h: interface for the CGroupLine class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GROUPLINE_H__485A738F_5BCB_4D7E_9B3E_6E9382AC62D8__INCLUDED_)
#define AFX_GROUPLINE_H__485A738F_5BCB_4D7E_9B3E_6E9382AC62D8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Subclass.h"
#include <afxtempl.h>

class CGroupLine : public CSubclassWnd  
{
public:
	CGroupLine();
	virtual ~CGroupLine();

	BOOL Initialize(HWND hwndStatic);

protected:
	CFont m_fontLabel;
	
protected:
	virtual LRESULT WindowProc(HWND hRealWnd, UINT msg, WPARAM wp, LPARAM lp);

};

class CGroupLineManager
{
public:
	CGroupLineManager();
	virtual ~CGroupLineManager();

	BOOL AddGroupLine(UINT nIDStatic, HWND hwndParent);

protected:
	CMap<HWND, HWND, CGroupLine*, CGroupLine*&> m_mapGroupLines;
};

#endif // !defined(AFX_GROUPLINE_H__485A738F_5BCB_4D7E_9B3E_6E9382AC62D8__INCLUDED_)
