// MultiTaskFile.h: interface for the CMultiTaskFile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MULTITASKFILE_H__3896047E_FE76_4EDC_9F97_191CABB982C0__INCLUDED_)
#define AFX_MULTITASKFILE_H__3896047E_FE76_4EDC_9F97_191CABB982C0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\Interfaces\ITaskList.h"

#include <afxtempl.h>

class CTaskFile;

class CMultiTaskFile : public IMultiTaskList  
{
public:
	CMultiTaskFile(LPCTSTR szReportTitle = NULL, const COleDateTime& dtReport = 0.0);
	virtual ~CMultiTaskFile();

	CTaskFile& GetTaskFile(int nTaskFile = 0);

	/////////////////////////////////////////////////////
	// IMultiTaskList implementation
	virtual int GetTaskListCount() const;
	virtual const ITaskList* GetTaskList(int nTaskList = 0) const;

	virtual LPCWSTR GetReportTitle() const { return m_sReportTitle; }
	virtual LPCWSTR GetReportDate() const { return m_sReportDate; }

	/////////////////////////////////////////////////////
	// IUnknown implementation
	HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void __RPC_FAR *__RPC_FAR *ppvObject);
    ULONG STDMETHODCALLTYPE AddRef(void) { return 1; } // do nothing
    ULONG STDMETHODCALLTYPE Release( void) { return 1; } // do nothing

protected:
	CArray<CTaskFile*, CTaskFile*&> m_aTaskFiles;

	CString m_sReportDate, m_sReportTitle;
};

#endif // !defined(AFX_MULTITASKFILE_H__3896047E_FE76_4EDC_9F97_191CABB982C0__INCLUDED_)
