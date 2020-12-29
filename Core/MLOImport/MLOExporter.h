// MLOExporter.h: interface for the CMLOExporter class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MLOEXPORTER_H__F588E6B1_3646_4994_99A2_4223FDDA1A31__INCLUDED_)
#define AFX_MLOEXPORTER_H__F588E6B1_3646_4994_99A2_4223FDDA1A31__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\shared\Icon.h"
#include "..\Shared\TimeHelper.h"

#include "..\Interfaces\IImportExport.h"
#include "..\Interfaces\ITasklist.h"

#include <afxtempl.h>

class CXmlItem;
class ITransText;

class CMLOExporter : public IExportTasklist  
{
public:
	CMLOExporter();
	virtual ~CMLOExporter();

	// interface implementation
	void Release() { delete this; }
	void SetLocalizer(ITransText* pTT);
	bool SupportsHtmlComments() const { return false; }
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
	bool ExportTask(const ITASKLISTBASE* pSrcTaskFile, HTASKITEM hTask, CXmlItem* pXIDestParent, BOOL bAndSiblings);

	void BuildPlacesMap(const ITASKLISTBASE* pSrcTaskFile, HTASKITEM hTask, CMapStringToString& mapPlaces, BOOL bAndSiblings);
	void ExportPlaces(const ITASKLISTBASE* pSrcTaskFile, CXmlItem* pDestPrj);

	static TH_UNITS MapUnitsToTHUnits(TDC_UNITS nUnits);
	static CString FormatDate(time64_t tDate);
};

#endif // !defined(AFX_MLOEXPORTER_H__F588E6B1_3646_4994_99A2_4223FDDA1A31__INCLUDED_)
