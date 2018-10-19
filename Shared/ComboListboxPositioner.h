// ComboListboxPositioner.h: interface for the CComboListboxPositioner class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COMBOLISTBOXPOSITIONER_H__8AE280AF_E103_447C_AF22_8AA12C03E9BD__INCLUDED_)
#define AFX_COMBOLISTBOXPOSITIONER_H__8AE280AF_E103_447C_AF22_8AA12C03E9BD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "hookmgr.h"

class CComboListboxPositioner : public CHookMgr<CComboListboxPositioner>  
{
	friend class CHookMgr<CComboListboxPositioner>;

public:
	CComboListboxPositioner();
	virtual ~CComboListboxPositioner();

	static BOOL Initialize();
	static void Release();

protected:		
	BOOL m_bMovingListBox;

protected:		
	static CComboListboxPositioner& Instance() { return CHookMgr<CComboListboxPositioner>::GetInstance(); }
	static void FixupListBoxPosition(HWND hwndListbox, const WINDOWPOS& wpos);

	virtual BOOL OnCallWndRetProc(const MSG& msg, LRESULT lr);
};

#endif // !defined(AFX_ComboListboxPositioner_H__8AE280AF_E103_447C_AF22_8AA12C03E9BD__INCLUDED_)
