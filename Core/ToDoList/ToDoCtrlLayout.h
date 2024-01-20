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

class CPreferences;

//////////////////////////////////////////////////////////////////////

class CToDoCtrlLayout  
{
public:
	CToDoCtrlLayout(CWnd* pParent, CWnd* pAttributes, CWnd* pComments);
	virtual ~CToDoCtrlLayout();

	BOOL ModifyLayout(TDC_UILOCATION nAttribsPos, 
					  TDC_UILOCATION nCommentsPos,
					  BOOL bAllowStacking, 
					  BOOL bStackCommentAbove);

	BOOL ModifyLayout(TDC_MAXSTATE nState, 
					  BOOL bShowCommentsAlways);

	void Resize(int cx, int cy);
	void SetSplitBarColor(COLORREF color);

	TDC_MAXSTATE GetMaximiseState() const { return m_nMaxState; }
	TDC_UILOCATION GetAttributesPosition() const { return m_nAttribsPos; }
	TDC_UILOCATION GetCommentsPos() const { return m_nCommentsPos; }

	BOOL HasMaximiseState(TDC_MAXSTATE nState) const { return (m_nMaxState == nState); }
	BOOL HasSplitters() const { return (m_splitterHorz.GetPaneCount() || m_splitterVert.GetPaneCount()); }

	BOOL IsCommentsVisible() const;
	BOOL IsRebuildingLayout() const { return m_bRebuildingLayout; }

	void SaveState(CPreferences& prefs, LPCTSTR szKey) const;
	void LoadState(const CPreferences& prefs, LPCTSTR szKey);

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
	
	static void SaveState(CPreferences& prefs, LPCTSTR szKey, LPCTSTR szEntry, const CSimpleSplitter& splitter);
	static void LoadState(const CPreferences& prefs, LPCTSTR szKey, LPCTSTR szEntry, CSimpleSplitter& splitter);

};

#endif // !defined(AFX_TODOCTRLSPLITTING_H__806C32DA_659C_4BF3_9C59_C0114FD43C37__INCLUDED_)
