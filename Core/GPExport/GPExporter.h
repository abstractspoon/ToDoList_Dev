// GPExporter.h: interface for the CGPExporter class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GPEXPORTER_H__BD28EDEE_0F1F_4AC9_B083_9A033E473181__INCLUDED_)
#define AFX_GPEXPORTER_H__BD28EDEE_0F1F_4AC9_B083_9A033E473181__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\shared\Icon.h"

#include "..\Interfaces\IImportExport.h"
#include "..\Interfaces\ITasklist.h"

#include <afxtempl.h>

class CXmlItem;

class CGPExporter : public IExportTasklist  
{
public:
	CGPExporter();
	virtual ~CGPExporter();

	// interface implementation
	void Release() { delete this; }
	void SetLocalizer(ITransText* pTT);
	bool SupportsHtmlComments() const { return false; }
	HICON GetIcon() const { return m_icon; }

	// caller must copy only
	LPCTSTR GetMenuText() const;
	LPCTSTR GetFileFilter() const;
	LPCTSTR GetFileExtension() const;
	LPCWSTR GetTypeID() const { return _T("BD28EDEE-0F1F-4AC9-B083-9A033E473181"); }

	IIMPORTEXPORT_RESULT Export(const ITaskList* pSrcTaskFile, LPCTSTR szDestFilePath, DWORD dwFlags, IPreferences* pPrefs, LPCTSTR szKey);
	IIMPORTEXPORT_RESULT Export(const IMultiTaskList* pSrcTaskFile, LPCTSTR szDestFilePath, DWORD dwFlags, IPreferences* pPrefs, LPCTSTR szKey);

protected:
	CIcon m_icon;
	CMap<CString, LPCTSTR, int, int&> m_mapResources;
	CMap<int, int, CXmlItem*, CXmlItem*&> m_mapTasks;

	// Pseudo-const
	CString MILESTONETAG;

protected:
	bool ExportTask(const ITASKLISTBASE* pSrcTaskFile, HTASKITEM hTask, CXmlItem* pXIDestParent, CXmlItem* pXIAllocations, BOOL bAndSiblings);
	void BuildResourceMap(const ITASKLISTBASE* pSrcTaskFile, HTASKITEM hTask, CXmlItem* pDestPrj, BOOL bAndSiblings);
	void ExportResources(const ITASKLISTBASE* pSrcTaskFile, CXmlItem* pDestPrj);
	void SetupDisplay(CXmlItem* pDestPrj);
	void SetupCalendar(CXmlItem* pDestPrj);
	void ExportDependencies(const ITASKLISTBASE* pSrcTaskFile, HTASKITEM hTask, CXmlItem* pDestPrj, BOOL bAndSiblings);
	bool InitConsts(const ITASKLISTBASE* pTaskFile, DWORD dwFlags, const IPreferences* pPrefs, LPCTSTR szKey);

	static void GetTaskDates(const ITASKLISTBASE* pSrcTaskFile, HTASKITEM hTask, time64_t& tEarliestStart, time64_t& tLatestDue, time64_t& tLatestDone);
	static int GetGPTaskID(DWORD dwTDLTaskID);
};

#endif // !defined(AFX_GPEXPORTER_H__BD28EDEE_0F1F_4AC9_B083_9A033E473181__INCLUDED_)
