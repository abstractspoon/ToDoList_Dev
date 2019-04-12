////////////////////////////////////////////////////////////////
// MSDN Magazine -- June 2003
// If this code works, it was written by Paul DiLascia.
// If not, I don't know who wrote it.
// Compiles with Visual Studio .NET on Windows XP. Tab size=3.
//
// modified to give per column control
#pragma once

//////////////////
// Header control with lockable columns.
// Call Lock to lock/unlock them.

#include <afxtempl.h>
//
class CLockableHeaderCtrl : public CHeaderCtrl
{
public:
	CLockableHeaderCtrl() { }

	void LockColumn(int nColumn, BOOL bLock = TRUE);
	BOOL IsColumnLocked(int nColumn);

protected:
	CMap<int, int, int, int&> m_mapLocked; // columns are locked

protected:
	virtual BOOL OnChildNotify(UINT msg, WPARAM wp, LPARAM lp, LRESULT* pRes);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	DECLARE_MESSAGE_MAP();
};
