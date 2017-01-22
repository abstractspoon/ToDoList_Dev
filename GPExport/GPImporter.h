// GPImporter.h: interface for the CGPImporter class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GPIMPORTER_H__7B3A9934_69F0_46D5_88B4_C84D715772FC__INCLUDED_)
#define AFX_GPIMPORTER_H__7B3A9934_69F0_46D5_88B4_C84D715772FC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

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
	HICON GetIcon() const { return m_hIcon; }

	IIMPORT_RESULT Import(LPCTSTR szSrcFilePath, ITaskList* pDestTaskFile, bool bSilent, IPreferences* pPrefs, LPCTSTR szKey);

protected:
	HICON m_hIcon;
	CMap<int, int, CString, CString&> m_mapResources;

	// Pseudo-const
	CString MILESTONETAG;

protected:
	bool ImportTask(const CXmlItem* pXISrcTask, ITaskList9* pDestTaskFile, HTASKITEM htDestParent, BOOL bAndSiblings);
	void BuildResourceMap(const CXmlItem* pXISrcPrj);
	void FixupDependencies(const CXmlItem* pXISrcTask, ITaskList9* pDestTaskFile, BOOL bAndSiblings);
	void FixupResourceAllocations(const CXmlItem* pXISrcPrj, ITaskList9* pDestTaskFile);
	bool InitConsts(bool bSilent, const IPreferences* pPrefs, LPCTSTR szKey);

	static DWORD GetTDLTaskID(int nGPTaskID);
};

#endif // !defined(AFX_GPIMPORTER_H__7B3A9934_69F0_46D5_88B4_C84D715772FC__INCLUDED_)
