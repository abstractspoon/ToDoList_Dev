// MLOExporter.h: interface for the CMLOExporter class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MLOEXPORTER_H__F588E6B1_3646_4994_99A2_4223FDDA1A31__INCLUDED_)
#define AFX_MLOEXPORTER_H__F588E6B1_3646_4994_99A2_4223FDDA1A31__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//////////////////////////////////////////////////////////////////////

#ifndef SHORT_MAX
#	define SHORT_MAX SHRT_MAX
#endif

//////////////////////////////////////////////////////////////////////

#include "..\shared\Icon.h"
#include "..\Shared\TimeHelper.h"

#include "..\Interfaces\IImportExport.h"
#include "..\Interfaces\ITasklist.h"

#include <afxtempl.h>

//////////////////////////////////////////////////////////////////////

class CXmlItem;
class ITransText;

//////////////////////////////////////////////////////////////////////

class CMLOExporter : public IExportTasklist  
{
public:
	CMLOExporter();
	virtual ~CMLOExporter();

	// interface implementation
	void Release() { delete this; }
	void SetLocalizer(ITransText* pTT);
	bool SupportsHtmlComments() const { return true; }
	HICON GetIcon() const { return m_icon; }

	// caller must copy only
    LPCTSTR GetMenuText() const { return _T("My Life Organized"); }
	LPCTSTR GetFileFilter() const { return _T("MLO Task Files (*.ml)|*.ml||"); }
	LPCTSTR GetFileExtension() const { return _T("ml"); }
	LPCWSTR GetTypeID() const { return _T("F588E6B1_3646_4994_99A2_4223FDDA1A31"); }

	IIMPORTEXPORT_RESULT Export(const ITaskList* pSrcTaskFile, LPCTSTR szDestFilePath, DWORD dwFlags, IPreferences* pPrefs, LPCTSTR szKey);
	IIMPORTEXPORT_RESULT Export(const IMultiTaskList* pSrcTaskFile, LPCTSTR szDestFilePath, DWORD dwFlags, IPreferences* pPrefs, LPCTSTR szKey);

protected:
	CIcon m_icon;

protected:
	typedef CArray<const ITASKLISTBASE*, const ITASKLISTBASE*> CITaskListArray;
	IIMPORTEXPORT_RESULT ExportTasklists(const CITaskListArray& aTasklists, LPCTSTR szDestFilePath) const;

	bool ExportTask(const ITASKLISTBASE* pSrcTaskFile, HTASKITEM hTask, CXmlItem* pXIDestParent, BOOL bAndSiblings) const;

	void BuildPlacesMap(const ITASKLISTBASE* pSrcTaskFile, HTASKITEM hTask, CMapStringToString& mapPlaces, BOOL bAndSiblings) const;
	void ExportPlaces(const ITASKLISTBASE* pSrcTaskFile, CXmlItem* pDestPrj) const;

	CString FormatFileLinks(const ITASKLISTBASE* pSrcTaskFile, HTASKITEM hTask) const;
	CString FormatDependencies(const ITASKLISTBASE* pSrcTaskFile, HTASKITEM hTask, CXmlItem* pDestItem) const;
	CString FormatComments(const ITASKLISTBASE* pSrcTaskFile, HTASKITEM hTask) const;

	CXmlItem* CreateTaskNode(LPCTSTR szTitle, DWORD dwID, CXmlItem* pXIDestParent) const;

	static TH_UNITS MapUnitsToTHUnits(TDC_UNITS nUnits);
	static CString FormatDate(time64_t tDate);
	static void AddSpacedContent(const CString& sSrc, CString& sDest);
	static CString FormatDestID(const CString& sTitle, DWORD dwID);
	static CString FormatTitle(LPCTSTR szReportTitle, LPCTSTR szReportDate);
};

#endif // !defined(AFX_MLOEXPORTER_H__F588E6B1_3646_4994_99A2_4223FDDA1A31__INCLUDED_)
