// FMindImporter.h: interface for the CFMindImporter class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FMINDIMPORTER_H__48AE3CCE_E042_432D_B5DB_D7E310CF99CE__INCLUDED_)
#define AFX_FMINDIMPORTER_H__48AE3CCE_E042_432D_B5DB_D7E310CF99CE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\SHARED\xmlfile.h"

#include "..\Interfaces\IImportExport.h"
#include "..\Interfaces\ITasklist.h"

class ITransText;

class CFMindImporter : public IImportTasklist  
{
public:
	CFMindImporter();
	virtual ~CFMindImporter();

	// interface implementation
	void Release() { delete this; }
	void SetLocalizer(ITransText* pTT);
	
	// caller must copy only
	LPCTSTR GetMenuText() const { return _T("FreeMind"); }
	LPCTSTR GetFileFilter() const { return _T("FreeMind Files (*.mm)|*.mm||"); }
	LPCTSTR GetFileExtension() const { return _T("mm"); }
	HICON GetIcon() const { return m_hIcon; }

	IIMPORT_RESULT Import(LPCTSTR szSrcFilePath, ITaskList* pDestTaskFile, bool bSilent, IPreferences* pPrefs, LPCTSTR szKey);

protected:
	HICON m_hIcon;
	
protected:
	static bool ImportTask(const CXmlItem* pFMTask, ITaskList10* pDestTaskFile, HTASKITEM hParent, BOOL bAndSiblings);
	static COLORREF GetFMColor(const CXmlItem* pFMTask, LPCTSTR szColorField);
	static time_t GetFMDate(const CXmlItem* pFMTask, LPCTSTR szDateField);
	static CString GetAttribValueS(const CXmlItem* pFMTask, LPCTSTR szAttribName);
	static int GetAttribValueI(const CXmlItem* pFMTask, LPCTSTR szAttribName);
	static bool GetAttribValueB(const CXmlItem* pFMTask, LPCTSTR szAttribName);
	static double GetAttribValueD(const CXmlItem* pFMTask, LPCTSTR szAttribName);
	static time_t GetAttribValueT(const CXmlItem* pFMTask, LPCTSTR szAttribName);
	static CString GetTaskRichContent(const CXmlItem* pFMTask, LPCTSTR szRichType, LPCTSTR szFallback = NULL);
	static int GetTaskArrayItems(const CXmlItem* pFMTask, LPCTSTR szAttribName, CStringArray& aItems);
	static CString GetTaskComments(const CXmlItem* pFMTask);

	static CString ExtractContentFromItem(const CXmlItem* pFMItem, BOOL bAndSiblings);
};

#endif // !defined(AFX_FMINDIMPORTER_H__48AE3CCE_E042_432D_B5DB_D7E310CF99CE__INCLUDED_)
