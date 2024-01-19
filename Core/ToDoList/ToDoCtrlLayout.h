// ToDoCtrlSplitting.h: interface for the CToDoCtrlSplitting class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TODOCTRLSPLITTING_H__806C32DA_659C_4BF3_9C59_C0114FD43C37__INCLUDED_)
#define AFX_TODOCTRLSPLITTING_H__806C32DA_659C_4BF3_9C59_C0114FD43C37__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//////////////////////////////////////////////////////////////////////

#include "tdcenum.h"

#include "..\3rdParty\SimpleSplitter.h"

//////////////////////////////////////////////////////////////////////

struct UITHEME;

//////////////////////////////////////////////////////////////////////

class CToDoCtrlLayout  
{
public:
	CToDoCtrlLayout(CWnd* pParent, CWnd* pAttributes, CWnd* pComments);
	virtual ~CToDoCtrlLayout();

	BOOL ModifyLayout(TDC_UILOCATION nAttribsPos, TDC_UILOCATION nCommentsPos);
	BOOL ModifyLayout(BOOL bAllowStacking, BOOL bStackCommentAbove);

	BOOL SetMaximiseState(TDC_MAXSTATE nState, BOOL bShowCommentsAlways);
	void Resize(const CRect& rect);
	void SetUITheme(const UITHEME& theme);

	TDC_MAXSTATE GetMaximiseState() const { return m_nMaxState; }
	TDC_UILOCATION GetAttributesPosition() const { return m_nAttribsPos; }
	TDC_UILOCATION GetCommentsPos() const { return m_nCommentsPos; }

	BOOL HasMaximiseState(TDC_MAXSTATE nState) const { return (m_nMaxState == nState); }
	BOOL IsCommentsVisible(/*BOOL bActually = FALSE*/) const;
	BOOL IsRebuildingLayout() const { return m_bRebuildingLayout; }

protected:
	CSimpleSplitter m_splitterHorz;
	CSimpleSplitter m_splitterVert;

	CWnd* m_pParent;
	CWnd* m_pAttributes;
	CWnd* m_pComments;

	TDC_MAXSTATE m_nMaxState;

	TDC_UILOCATION m_nAttribsPos;
	TDC_UILOCATION m_nCommentsPos;

	BOOL m_bAllowStacking;
	BOOL m_bStackCommentsAbove;
	BOOL m_bShowCommentsAlways;
	BOOL m_bRebuildingLayout;

protected:
	void RebuildLayout();
	BOOL ResizeIfRoot(CSimpleSplitter& splitter, const CRect& rect) const;
};

#endif // !defined(AFX_TODOCTRLSPLITTING_H__806C32DA_659C_4BF3_9C59_C0114FD43C37__INCLUDED_)
