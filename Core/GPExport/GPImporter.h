// GPImporter.h: interface for the CGPImporter class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GPIMPORTER_H__7B3A9934_69F0_46D5_88B4_C84D715772FC__INCLUDED_)
#define AFX_GPIMPORTER_H__7B3A9934_69F0_46D5_88B4_C84D715772FC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\shared\Icon.h"

#include "..\Interfaces\IImportExport.h"
#include "..\Interfaces\ITasklist.h"

#include <afxtempl.h>

class CXmlItem;

class CGPImporter : public IImportTasklist    
{
public:
	CGPImporter();
	virtual ~CGPImporter();

	// interface implementation
	void Release() { delete this; }
	void SetLocalizer(ITransText* pTT);

	// caller must copy only
	LPCTSTR GetMenuText() const;
	LPCTSTR GetFileFilter() const;
	LPCTSTR GetFileExtension() const;
	LPCWSTR GetTypeID() const { return _T("7B3A9934_69F0_46D5_88B4_C84D715772FC"); }
	HICON GetIcon() const { return m_icon; }

	IIMPORTEXPORT_RESULT Import(LPCTSTR szSrcFilePath, ITaskList* pDestTaskFile, DWORD dwFlags, IPreferences* pPrefs, LPCTSTR szKey);

protected:
	CIcon m_icon;
	CMap<int, int, CString, CString&> m_mapResources;

	// Pseudo-const
	CString MILESTONETAG;

protected:
	bool ImportTask(const CXmlItem* pXISrcTask, ITASKLISTBASE* pDestTaskFile, HTASKITEM htDestParent, BOOL bAndSiblings);
	void BuildResourceMap(const CXmlItem* pXISrcPrj);
	void FixupDependencies(const CXmlItem* pXISrcTask, ITASKLISTBASE* pDestTaskFile, BOOL bAndSiblings);
	void FixupResourceAllocations(const CXmlItem* pXISrcPrj, ITASKLISTBASE* pDestTaskFile);
	bool InitConsts(DWORD dwFlags, const IPreferences* pPrefs, LPCTSTR szKey);
	void InitWeekends(const CXmlItem* pXISrcPrj);

	static DWORD GetTDLTaskID(int nGPTaskID);
};

#endif // !defined(AFX_GPIMPORTER_H__7B3A9934_69F0_46D5_88B4_C84D715772FC__INCLUDED_)
