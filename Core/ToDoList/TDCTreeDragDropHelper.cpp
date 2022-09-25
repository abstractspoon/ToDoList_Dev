// TDCTreeDragDropHelper.cpp: implementation of the CTDCTreeDragDropHelper class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "todolist.h"
#include "TDCTreeDragDropHelper.h"
#include "TDLTaskCtrlBase.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////

const int IMAGE_SIZE = GraphicsMisc::ScaleByDPIFactor(16);

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTDCTreeDragDropHelper::CTDCTreeDragDropHelper(const CTDLTaskCtrlBase& ctrl, const CTreeSelectionHelper& selection, CTreeCtrl& tree)
	:
	CTreeDragDropHelper(selection, tree),
	m_Ctrl(ctrl),
	m_bShowIcons(FALSE), 
	m_bShowParentsAsFolders(FALSE)
{
}

CTDCTreeDragDropHelper::~CTDCTreeDragDropHelper()
{
}

void CTDCTreeDragDropHelper::ShowIcons(BOOL bShow, BOOL bShowParentsAsFolders)
{
	m_bShowIcons = bShow;
	m_bShowParentsAsFolders = bShowParentsAsFolders;
}

CSize CTDCTreeDragDropHelper::OnGetDragSize(CDC& dc)
{
	CSize sizeDrag = CTreeDragDropHelper::OnGetDragSize(dc);
	sizeDrag.cx += IMAGE_SIZE;

	return sizeDrag;
}

void CTDCTreeDragDropHelper::OnDrawData(CDC& dc, const CRect& rc, COLORREF& crMask)
{
	CTreeDragDropHelper::OnDrawData(dc, rc, crMask);
}

void CTDCTreeDragDropHelper::OnDrawItem(CDC& dc, const CRect& rItem, HTREEITEM hti)
{
	DWORD dwTaskID = m_tree.GetItemData(hti);
	int nImage = m_Ctrl.GetTaskIconIndex(dwTaskID);

	if (nImage != -1)
		ImageList_Draw(m_Ctrl.GetTaskImageList(), nImage, dc, rItem.left, rItem.top, ILD_TRANSPARENT);

	CRect rRest(rItem);
	rRest.OffsetRect(IMAGE_SIZE, 0);

	CTreeDragDropHelper::OnDrawItem(dc, rRest, hti);
}
