// GPExporter.h: interface for the CGPExporter class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GPEXPORTER_H__F588E6B1_3646_4994_99A2_4223FDDA1A31__INCLUDED_)
#define AFX_GPEXPORTER_H__F588E6B1_3646_4994_99A2_4223FDDA1A31__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

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

	// caller must copy only
	LPCTSTR GetMenuText() const;
	LPCTSTR GetFileFilter() const;
	LPCTSTR GetFileExtension() const;
	HICON GetIcon() const { return m_hIcon; }

	bool Export(const ITaskList* pSrcTaskFile, LPCTSTR szDestFilePath, bool bSilent, IPreferences* pPrefs, LPCTSTR szKey);
	bool Export(const IMultiTaskList* pSrcTaskFile, LPCTSTR szDestFilePath, bool bSilent, IPreferences* pPrefs, LPCTSTR szKey);

protected:
	HICON m_hIcon;
	CMap<CString, LPCTSTR, int, int&> m_mapResources;
	CMap<int, int, CXmlItem*, CXmlItem*&> m_mapTasks;

	// Pseudo-const
	CString MILESTONETAG;

protected:
	bool ExportTask(const ITaskList9* pSrcTaskFile, HTASKITEM hTask, CXmlItem* pXIDestParent, CXmlItem* pXIAllocations, BOOL bAndSiblings);
	void BuildResourceMap(const ITaskList9* pSrcTaskFile, HTASKITEM hTask, CXmlItem* pDestPrj, BOOL bAndSiblings);
	void ExportResources(const ITaskList9* pSrcTaskFile, CXmlItem* pDestPrj);
	void SetupDisplay(CXmlItem* pDestPrj);
	void SetupCalendar(CXmlItem* pDestPrj);
	void ExportDependencies(const ITaskList9* pSrcTaskFile, HTASKITEM hTask, CXmlItem* pDestPrj, BOOL bAndSiblings);
	bool InitConsts(const ITaskList9* pTaskFile, bool bSilent, const IPreferences* pPrefs, LPCTSTR szKey);

	static void GetTaskDates(const ITaskList9* pSrcTaskFile, HTASKITEM hTask, time_t& tEarliestStart, time_t& tLatestDue, time_t& tLatestDone);
	static int GetGPTaskID(DWORD dwTDLTaskID);
};

#endif // !defined(AFX_GPEXPORTER_H__F588E6B1_3646_4994_99A2_4223FDDA1A31__INCLUDED_)
