// MultiTaskFile.cpp: implementation of the CMultiTaskFile class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "todolist.h"
#include "MultiTaskFile.h"
#include "TaskFile.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMultiTaskFile::CMultiTaskFile()
{

}

CMultiTaskFile::~CMultiTaskFile()
{
	// cleanup
	int nTaskFile = GetTaskListCount();

	while (nTaskFile--)
		delete m_aTaskFiles[nTaskFile];
}

CTaskFile& CMultiTaskFile::GetTaskFile(int nTaskFile) 
{
	while (m_aTaskFiles.GetSize() <= nTaskFile)
	{
		CTaskFile* pTF = new CTaskFile; 
		m_aTaskFiles.Add(pTF);
	}

	return *(m_aTaskFiles[nTaskFile]);
}

HRESULT CMultiTaskFile::QueryInterface(REFIID /*riid*/, void __RPC_FAR *__RPC_FAR *ppvObject)
{
	*ppvObject = reinterpret_cast<IMultiTaskList*>(this);

	// compare riid against our supported version numbers
/*
	if (IsEqualIID(riid, IID_MULTITASKLIST))
	{
		*ppvObject = reinterpret_cast<IMultiTaskList*>(this);
		AddRef();
	}
	else if (IsEqualIID(riid, IID_MULTITASKLIST2))
	{
		*ppvObject = reinterpret_cast<IMultiTaskList2*>(this);
		AddRef();
	}
	else if (IsEqualIID(riid, IID_MULTITASKLIST3))
	{
		*ppvObject = reinterpret_cast<IMultiTaskList3*>(this);
		AddRef();
	}
	else if (IsEqualIID(riid, IID_MULTITASKLIST4))
	{
		*ppvObject = reinterpret_cast<IMultiTaskList4*>(this);
		AddRef();
	}
	else if (IsEqualIID(riid, IID_MULTITASKLIST5))
	{
		*ppvObject = reinterpret_cast<IMultiTaskList5*>(this);
		AddRef();
	}
	else if (IsEqualIID(riid, IID_MULTITASKLIST6))
	{
		*ppvObject = reinterpret_cast<IMultiTaskList6*>(this);
		AddRef();
	}
	else if (IsEqualIID(riid, IID_MULTITASKLIST7))
	{
		*ppvObject = reinterpret_cast<IMultiTaskList7*>(this);
		AddRef();
	}
*/
	
	return (*ppvObject ? S_OK : E_NOTIMPL);
}

int CMultiTaskFile::GetTaskListCount() const
{
	return m_aTaskFiles.GetSize();
}

const ITaskList* CMultiTaskFile::GetTaskList(int nTaskList) const
{
	ASSERT(m_aTaskFiles.GetSize() > nTaskList);

	if (m_aTaskFiles.GetSize() <= nTaskList)
		return NULL;

	// else
	return m_aTaskFiles[nTaskList];
}
