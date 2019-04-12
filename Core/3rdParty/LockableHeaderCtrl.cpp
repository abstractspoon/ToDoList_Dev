////////////////////////////////////////////////////////////////
// MSDN Magazine -- June 2003
// If this code works, it was written by Paul DiLascia.
// If not, I don't know who wrote it.
// Compiles with Visual Studio .NET on Windows XP. Tab size=3.
//
#include "stdafx.h"
#include "lockableHeaderctrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(CLockableHeaderCtrl, CHeaderCtrl)
	ON_WM_SETCURSOR()
END_MESSAGE_MAP()

//////////////////

void CLockableHeaderCtrl::LockColumn(int nColumn, BOOL bLock)
{
	if (nColumn >= 0 && nColumn < GetItemCount())
	{
		if (bLock)
			m_mapLocked[nColumn] = 1;
		else
			m_mapLocked.RemoveKey(nColumn);
	}
}

BOOL CLockableHeaderCtrl::IsColumnLocked(int nColumn)
{
	if (nColumn >= 0 && nColumn < GetItemCount())
	{
		int nDummy;

		return m_mapLocked.Lookup(nColumn, nDummy);
	}

	// else
	return FALSE;
}

// If columns are locked, don't display size cursor.
//
BOOL CLockableHeaderCtrl::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT msg)
{
	// find what item we're over
	CPoint point(GetMessagePos());
	ScreenToClient(&point);
	
	HDHITTESTINFO hdht = { { point.x, point.y }, 0, 0 };

	int nItem = SendMessage(HDM_HITTEST, 0, (LPARAM)&hdht);

	if (IsColumnLocked(nItem) && (hdht.flags & (HHT_ONDIVIDER | HHT_ONDIVOPEN)))
		return TRUE;
	
	// else
	return CHeaderCtrl::OnSetCursor(pWnd, nHitTest, msg);
}

//////////////////
// The header will send either HDN_BEGINTRACKA or HDN_BEGINTRACKW
// depending on how your parent list control responds to
// WM_NOTIFYFORMAT (default=HDN_BEGINTRACKW, Unicode). But if you're
// implementing a control to be used in any app, you should handle both
// notifications if you want it to work in either situation.
//
// Since OnChildNotify is virtual, all you have to do is override
// it--no need for message map entries to handle HDN_BEGINTRACKA/W.
//
BOOL CLockableHeaderCtrl::OnChildNotify(UINT msg, WPARAM wp, LPARAM lp, LRESULT* pRes)
{
	LPNMHDR pNMHDR = (LPNMHDR)lp;

	if (pNMHDR->code == HDN_BEGINTRACKA || pNMHDR->code == HDN_BEGINTRACKW) 
	{
		HD_NOTIFY *phdn = (HD_NOTIFY *) pNMHDR;

		TRACE (_T("HDN_BEGINTRACK(column = %d)\n"), phdn->iItem);

		// the search field column is zero width then prevent it being resized
		if (IsColumnLocked(phdn->iItem))
			return (*pRes = TRUE); // eat message to disallow sizing
	}
	
	// otherwise, pass to header control for default processing
	return CHeaderCtrl::OnChildNotify(msg, wp, lp, pRes);
}
