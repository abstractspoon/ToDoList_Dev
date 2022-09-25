// TDCTreeDragDropHelper.h: interface for the CTDCTreeDragDropHelper class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TDCTREEDRAGDROPHELPER_H__2961480D_2B17_42F9_A870_F9C3F5F443AC__INCLUDED_)
#define AFX_TDCTREEDRAGDROPHELPER_H__2961480D_2B17_42F9_A870_F9C3F5F443AC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//////////////////////////////////////////////////////////////////////

#include "..\Shared\TreeDragDropHelper.h"

//////////////////////////////////////////////////////////////////////

class CTDLTaskCtrlBase;

//////////////////////////////////////////////////////////////////////

class CTDCTreeDragDropRenderer : public CTreeDragDropRenderer  
{
public:
	CTDCTreeDragDropRenderer(const CTDLTaskCtrlBase& ctrl, const CTreeSelectionHelper& selection, CTreeCtrl& tree);
	virtual ~CTDCTreeDragDropRenderer();

protected:
	const CTDLTaskCtrlBase& m_Ctrl;

protected:
	virtual CSize OnGetDragSize(CDC& dc);
	virtual void OnDrawData(CDC& dc, const CRect& rc, COLORREF& crMask);
	virtual void OnDrawItem(CDC& dc, const CRect& rItem, HTREEITEM hti);
};

//////////////////////////////////////////////////////////////////////

class CTDCTreeDragDropHelper : public CTreeDragDropHelper  
{
public:
	CTDCTreeDragDropHelper(const CTDLTaskCtrlBase& ctrl, const CTreeSelectionHelper& selection, CTreeCtrl& tree);

protected:
	CTDCTreeDragDropRenderer m_renderer;
};

#endif // !defined(AFX_TDCTREEDRAGDROPHELPER_H__2961480D_2B17_42F9_A870_F9C3F5F443AC__INCLUDED_)
