// TDCTreeDragDropHelper.cpp: implementation of the CTDCTreeDragDropRenderer class.
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

CTDCTreeDragDropRenderer::CTDCTreeDragDropRenderer(const CTDLTaskCtrlBase& ctrl, const CTreeSelectionHelper& selection, CTreeCtrl& tree)
	:
	CTreeDragDropRenderer(selection, tree),
	m_Ctrl(ctrl)
{
}

CTDCTreeDragDropRenderer::~CTDCTreeDragDropRenderer()
{
}

void CTDCTreeDragDropRenderer::OnGetDragItemRect(CDC& dc, HTREEITEM hti, CRect& rItem)
{
	CTreeDragDropRenderer::OnGetDragItemRect(dc, hti, rItem);

	rItem.left -= IMAGE_SIZE;
}

void CTDCTreeDragDropRenderer::OnDrawDragItem(CDC& dc, HTREEITEM hti, const CRect& rItem)
{
	DWORD dwTaskID = m_dragTree.GetItemData(hti);
	int nImage = m_Ctrl.GetTaskIconIndex(dwTaskID);

	if (nImage != -1)
		ImageList_Draw(m_Ctrl.GetTaskImageList(), nImage, dc, rItem.left, rItem.top, ILD_TRANSPARENT);

	CRect rText(rItem);
	rText.OffsetRect(IMAGE_SIZE, 0);

	CTreeDragDropRenderer::OnDrawDragItem(dc, hti, rText);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTDCTreeDragDropHelper::CTDCTreeDragDropHelper(const CTDLTaskCtrlBase& ctrl, const CTreeSelectionHelper& selection, CTreeCtrl& tree)
	:
	CTreeDragDropHelper(selection, tree, &m_renderer),
	m_renderer(ctrl, selection, tree)
{
}

