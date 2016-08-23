// ToDoCtrlUndo.h: interface for the CToDoCtrlUndo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TODOCTRLUNDO_H__5299C326_E181_47B2_A971_1563E5F6CEEC__INCLUDED_)
#define AFX_TODOCTRLUNDO_H__5299C326_E181_47B2_A971_1563E5F6CEEC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ToDoitem.h"

#include "..\shared\misc.h"

#include <afxtempl.h>

struct TDCUNDOELEMENT
{
	TDCUNDOELEMENT(TDCUNDOELMOP op = TDCUEO_EDIT, DWORD taskID = 0, DWORD parentID = 0, 
					DWORD prevSiblingID = 0, WORD flags = 0, const TODOITEM* pTDI = NULL) : 
					nOp(op), 
					dwTaskID(taskID), 
					dwParentID(parentID), 
					dwPrevSiblingID(prevSiblingID),
					wFlags(flags)
	{
		if (pTDI)
			tdi = *pTDI;
	}

	TDCUNDOELEMENT(const TDCUNDOELEMENT& elm) 
	{
		*this = elm;
	}

	const TDCUNDOELEMENT& operator=(const TDCUNDOELEMENT& elm) 
	{ 
		nOp = elm.nOp;
		dwTaskID = elm.dwTaskID;
		dwParentID = elm.dwParentID;
		dwPrevSiblingID = elm.dwPrevSiblingID;
		tdi = elm.tdi;
		wFlags = elm.wFlags;

		return *this; 
	}

	BOOL operator==(const TDCUNDOELEMENT& elm) const
	{ 
		return (nOp == elm.nOp && 
				dwTaskID == elm.dwTaskID &&
				dwParentID == elm.dwParentID &&
				dwPrevSiblingID == elm.dwPrevSiblingID &&
				wFlags == elm.wFlags);
	}

	BOOL operator!=(const TDCUNDOELEMENT& elm) const
	{
		return !(*this == elm);
	}

	TDCUNDOELMOP nOp;
	DWORD dwTaskID;
	DWORD dwParentID;
	DWORD dwPrevSiblingID;
	WORD wFlags;

	TODOITEM tdi;
};

typedef CArray<TDCUNDOELEMENT, TDCUNDOELEMENT&> CArrayUndoElements;

struct TDCUNDOACTION
{
	TDCUNDOACTION(TDCUNDOACTIONTYPE type = TDCUAT_NONE) : nType(type) 
	{
	}
	
	TDCUNDOACTION(const TDCUNDOACTION& action) 
	{
		*this = action;
	}

	const TDCUNDOACTION& operator=(const TDCUNDOACTION& action) 
	{ 
		nType = action.nType;
		aElements.Copy(action.aElements);

		return *this; 
	}

	BOOL operator==(const TDCUNDOACTION& action) const
	{ 
		return (nType == action.nType && 
				Misc::MatchAllT(aElements, action.aElements));
	}

	int GetTaskIDs(CDWordArray& aIDs) const
	{
		aIDs.RemoveAll();

		// get around constness
		TDCUNDOACTION* pThis = const_cast<TDCUNDOACTION*>(this);

		for (int nElm = 0; nElm < aElements.GetSize(); nElm++)
		{
			const TDCUNDOELEMENT& elm = pThis->aElements.ElementAt(nElm);
			aIDs.Add(elm.dwTaskID);
		}

		return aIDs.GetSize();
	}

	TDCUNDOACTIONTYPE nType;
	CArrayUndoElements aElements;
};

class CToDoCtrlUndo  
{
public:
	CToDoCtrlUndo();
	virtual ~CToDoCtrlUndo();

	void ResetAll();

	BOOL BeginNewAction(TDCUNDOACTIONTYPE nType);
	BOOL EndCurrentAction();
	BOOL ExtendLastAction(TDCUNDOACTIONTYPE nType = TDCUAT_NONE);

	TDCUNDOACTIONTYPE CurrentAction() const { return m_nActiveAction; }
	BOOL IsActive() const { return (CurrentAction() != TDCUAT_NONE); }

	BOOL SaveElement(TDCUNDOELMOP nOp, DWORD dwTaskID, DWORD dwParentID, DWORD dwPrevSiblingID, 
					 WORD wFlags, const TODOITEM* pTDI);
	BOOL IsValidElementOperation(TDCUNDOELMOP nOp) const;

	int GetLastUndoActionTaskIDs(CDWordArray& aIDs) const;
	int GetLastRedoActionTaskIDs(CDWordArray& aIDs) const;

	BOOL DeleteLastUndoAction(); // only if no redos exist

	TDCUNDOACTIONTYPE GetLastUndoType() const;
	TDCUNDOACTIONTYPE GetLastRedoType() const;

	TDCUNDOACTION* UndoLastAction();
	TDCUNDOACTION* RedoLastAction();

	BOOL CanUndo() const { return m_aUndo.GetSize(); }
	BOOL CanRedo() const { return m_aRedo.GetSize(); }

protected:
	CArray<TDCUNDOACTION, TDCUNDOACTION&> m_aUndo;
	CArray<TDCUNDOACTION, TDCUNDOACTION&> m_aRedo;

	TDCUNDOACTIONTYPE m_nActiveAction;

protected:
	TDCUNDOACTION& LastUndoAction();
	TDCUNDOACTION& LastRedoAction();
	const TDCUNDOACTION& LastUndoAction() const;
	const TDCUNDOACTION& LastRedoAction() const;
	int LastUndoIndex() const { return m_aUndo.GetSize() - 1; }
	int LastRedoIndex() const { return m_aRedo.GetSize() - 1; }
	
};

#endif // !defined(AFX_TODOCTRLUNDO_H__5299C326_E181_47B2_A971_1563E5F6CEEC__INCLUDED_)
