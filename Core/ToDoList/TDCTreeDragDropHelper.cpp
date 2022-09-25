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

CSize CTDCTreeDragDropRenderer::OnGetDragSize(CDC& dc)
{
	CSize sizeDrag = CTreeDragDropRenderer::OnGetDragSize(dc);
	sizeDrag.cx += IMAGE_SIZE;

	return sizeDrag;
}

void CTDCTreeDragDropRenderer::OnDrawData(CDC& dc, const CRect& rc, COLORREF& crMask)
{
	CTreeDragDropRenderer::OnDrawData(dc, rc, crMask);
}

void CTDCTreeDragDropRenderer::OnDrawItem(CDC& dc, const CRect& rItem, HTREEITEM hti)
{
	DWORD dwTaskID = m_tree.GetItemData(hti);
	int nImage = m_Ctrl.GetTaskIconIndex(dwTaskID);

	if (nImage != -1)
		ImageList_Draw(m_Ctrl.GetTaskImageList(), nImage, dc, rItem.left, rItem.top, ILD_TRANSPARENT);

	CRect rRest(rItem);
	rRest.OffsetRect(IMAGE_SIZE, 0);

	CTreeDragDropRenderer::OnDrawItem(dc, rRest, hti);
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

