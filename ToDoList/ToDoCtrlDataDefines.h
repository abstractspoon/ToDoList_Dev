// ToDoCtrlData.h: interface for the CToDoCtrlData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TODOCTRLDATADEFINES_H__02C3C360_45AB_45DC_B1BF_BCBEA472F0C7__INCLUDED_)
#define AFX_TODOCTRLDATADEFINES_H__02C3C360_45AB_45DC_B1BF_BCBEA472F0C7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//////////////////////////////////////////////////////////////////////

#define IMPLEMENT_DATA_UNDO(data, type)	\
	CUndoAction ua(data, type, FALSE)

//////////////////////////////////////////////////////////////////////

#define IMPLEMENT_DATA_UNDO_EDIT(data)	\
	CUndoAction ua(data, TDCUAT_EDIT, FALSE)

//////////////////////////////////////////////////////////////////////

#define IMPLEMENT_DATA_UNDO_EXTEND(data, type, extend)	\
	CUndoAction ua(data, type, extend)

//////////////////////////////////////////////////////////////////////

#define GET_DATA_TDI(data, id, tdi, ret)	\
{											\
	if (id == 0)							\
	return ret;								\
	tdi = data.GetTask(id, TRUE);			\
	ASSERT(tdi);							\
	if (tdi == NULL)						\
	return ret;								\
}

//////////////////////////////////////////////////////////////////////

#define GET_DATA_TDS(data, id, tds, ret)	\
{											\
	if (id == 0)							\
	return ret;								\
	tds = data.LocateTask(id);				\
	ASSERT(tds);							\
	if (tds == NULL)						\
	return ret;								\
}

//////////////////////////////////////////////////////////////////////

#define GET_DATA_TDI_TDS(data, id, tdi, tds, ret)	\
{													\
	if (id == 0)									\
	return ret;										\
	VERIFY(data.GetTask(id, tdi, tds, TRUE));		\
	ASSERT(tdi && tds);								\
	if (tdi == NULL || tds == NULL)					\
	return ret;										\
}

//////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_TODOCTRLDATADEFINES_H__02C3C360_45AB_45DC_B1BF_BCBEA472F0C7__INCLUDED_)
