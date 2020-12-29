// MLOImporter.h: interface for the CMLOImporter class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MLOIMPORTER_H__E1C1DB38_D45E_481E_8D91_7D8455C5155E__INCLUDED_)
#define AFX_MLOIMPORTER_H__E1C1DB38_D45E_481E_8D91_7D8455C5155E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\shared\Icon.h"

#include "..\Interfaces\IImportExport.h"
#include "..\Interfaces\iTasklist.h"

class CXmlItem;
class ITransText;

class CMLOImporter : public IImportTasklist  
{
public:
	CMLOImporter();
	virtual ~CMLOImporter();
	
	void Release() { delete this; }
	void SetLocalizer(ITransText* pTT);

	LPCTSTR GetMenuText() const { return _T("My Life Organized"); }
	LPCTSTR GetFileFilter() const { return _T("MLO Task Files (*.ml, *.xml)|*.ml;*.xml||"); }
	LPCTSTR GetFileExtension() const { return _T("ml"); }
	LPCWSTR GetTypeID() const { return _T("E1C1DB38_D45E_481E_8D91_7D8455C5155E"); }
	HICON GetIcon() const { return m_icon; }
	
	IIMPORTEXPORT_RESULT Import(LPCTSTR szSrcFilePath, ITaskList* pDestTaskFile, DWORD dwFlags, IPreferences* pPrefs, LPCTSTR szKey);

protected:
	CIcon m_icon;

protected:
	bool ImportTask(const CXmlItem* pXIMLOTask, ITASKLISTBASE* pDestTaskFile, HTASKITEM hParent, BOOL bAndSiblings) const;
	time64_t GetDate(const CXmlItem* pXIMLOTask, LPCTSTR szDateField) const;

};

#endif // !defined(AFX_MLOIMPORTER_H__E1C1DB38_D45E_481E_8D91_7D8455C5155E__INCLUDED_)
