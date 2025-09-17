// FMindExporter.h: interface for the CFMindExporter class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FMINDEXPORTER_H__49174123_010C_4E73_B1BF_5440AE766D46__INCLUDED_)
#define AFX_FMINDEXPORTER_H__49174123_010C_4E73_B1BF_5440AE766D46__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//////////////////////////////////////////////////////////////////////

#include "..\shared\Icon.h"

#include "..\Interfaces\IImportExport.h"
#include "..\Interfaces\ITasklist.h"

#include <afxtempl.h>

//////////////////////////////////////////////////////////////////////

class CXmlItem;
class CXmlFile;
class ITransText;

//////////////////////////////////////////////////////////////////////

class CFMindExporter : public IExportTasklist  
{
public:
	CFMindExporter();
	virtual ~CFMindExporter();
	
	// interface implementation
	void Release() { delete this; }
	void SetLocalizer(ITransText* pTT);
	bool SupportsHtmlComments() const { return false; }
	HICON GetIcon() const { return m_icon; }

	// caller must copy only
	LPCTSTR GetMenuText() const { return _T("FreeMind"); }
	LPCTSTR GetFileFilter() const { return _T("FreeMind Files (*.mm)|*.mm||"); }
	LPCTSTR GetFileExtension() const { return _T("mm"); }
	LPCWSTR GetTypeID() const { return _T("49174123_010C_4E73_B1BF_5440AE766D46"); }

	IIMPORTEXPORT_RESULT Export(const ITaskList* pSrcTaskFile, LPCTSTR szDestFilePath, DWORD dwFlags, IPreferences* pPrefs, LPCTSTR szKey);
	IIMPORTEXPORT_RESULT Export(const IMultiTaskList* pSrcTaskFile, LPCTSTR szDestFilePath, DWORD dwFlags, IPreferences* pPrefs, LPCTSTR szKey);

protected:
	CIcon m_icon;
	
protected:
	typedef CArray<const ITASKLISTBASE*, const ITASKLISTBASE*> CITaskListArray;
	IIMPORTEXPORT_RESULT ExportTasklists(const CITaskListArray& aTasklists, LPCTSTR szReportTitle, LPCTSTR szReportDate, LPCTSTR szDestFilePath) const;

	void ExportTask(const ITASKLISTBASE* pSrcTaskFile, HTASKITEM hTask, 
					CXmlItem* pXIDestParent, int LEVEL, BOOL bAndSiblings) const;

	static CString Encode(LPCTSTR szText);
	static CString ExportContent(const CXmlFile& file);
	static CString FormatTitle(LPCTSTR szReportTitle, LPCTSTR szReportDate);
};

#endif // !defined(AFX_FMINDEXPORTER_H__49174123_010C_4E73_B1BF_5440AE766D46__INCLUDED_)
