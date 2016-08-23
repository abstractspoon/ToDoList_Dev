// FMindExporter.h: interface for the CFMindExporter class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FMINDEXPORTER_H__49174123_010C_4E73_B1BF_5440AE766D46__INCLUDED_)
#define AFX_FMINDEXPORTER_H__49174123_010C_4E73_B1BF_5440AE766D46__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\Interfaces\IImportExport.h"
#include "..\Interfaces\ITasklist.h"

class CXmlItem;
class CXmlFile;
class ITransText;

class CFMindExporter : public IExportTasklist  
{
public:
	CFMindExporter();
	virtual ~CFMindExporter();
	
	// interface implementation
	void Release() { delete this; }
	void SetLocalizer(ITransText* pTT);

	// caller must copy only
	LPCTSTR GetMenuText() const { return _T("FreeMind"); }
	LPCTSTR GetFileFilter() const { return _T("FreeMind Files (*.mm)|*.mm||"); }
	LPCTSTR GetFileExtension() const { return _T("mm"); }
	HICON GetIcon() const { return m_hIcon; }
	
	bool Export(const ITaskList* pSrcTaskFile, LPCTSTR szDestFilePath, BOOL bSilent, IPreferences* pPrefs, LPCTSTR szKey);
	bool Export(const IMultiTaskList* pSrcTaskFile, LPCTSTR szDestFilePath, BOOL bSilent, IPreferences* pPrefs, LPCTSTR szKey);

protected:
	HICON m_hIcon;
	
protected:
	void ExportTask(const ITaskList10* pSrcTaskFile, HTASKITEM hTask, 
					CXmlItem* pXIDestParent, int LEVEL, BOOL bAndSiblings);
	CString FormatDate(time_t tDate);

	static CString Translate(LPCTSTR szText);
	static CString Export(const CXmlFile& file);

};

#endif // !defined(AFX_FMINDEXPORTER_H__49174123_010C_4E73_B1BF_5440AE766D46__INCLUDED_)
