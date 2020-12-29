// TaskListCsvImporter.h: interface for the CTaskListCsvImporter class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TASKLISTCSVIMPORTER_H__ADF211CB_FBD2_42A2_AD51_DFF58E566753__INCLUDED_)
#define AFX_TASKLISTCSVIMPORTER_H__ADF211CB_FBD2_42A2_AD51_DFF58E566753__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "tdcenum.h"
#include "TDLCsvImportExportDlg.h"

#include "..\Interfaces\IImportExport.h"
#include "..\Interfaces\Itasklist.h"

//////////////////////////////////////////////////////////////////////

static const LPCTSTR CSVIMPORT_TYPEID = _T("ADF211CB_FBD2_42A2_AD51_DFF58E566753");

//////////////////////////////////////////////////////////////////////

class CTaskListCsvImporter : public IImportTasklist  
{
public:
	CTaskListCsvImporter();
	virtual ~CTaskListCsvImporter();

    void Release() { delete this; }
	void SetLocalizer(ITransText* /*pTT*/) {}

	LPCTSTR GetMenuText() const { return _T("Spreadsheet"); }
	LPCTSTR GetFileFilter() const { return _T("Spreadsheet Files (*.csv)|*.csv||"); }
	LPCTSTR GetFileExtension() const { return _T("csv"); }
	LPCTSTR GetTypeID() const { return CSVIMPORT_TYPEID; }
	HICON GetIcon() const { return NULL; }

	IIMPORTEXPORT_RESULT Import(LPCTSTR szSrcFilePath, ITaskList* pDestTaskFile, DWORD dwFlags, IPreferences* pPrefs, LPCTSTR szKey);

protected:
	CString DELIM;

	CTDCAttributeMapping m_aColumnMapping;

protected:
	BOOL ImportTask(ITASKLISTBASE* pTasks, const CString& sLine) const;

	bool InitConsts(LPCTSTR szSrcFilePath, ITASKLISTBASE* pTasks, DWORD dwFlags, IPreferences* pPrefs, LPCTSTR szKey);

	void GetTaskAndParentIDs(const CStringArray& aValues, DWORD& dwTaskID, DWORD& dwParentID) const;
	CString GetTaskTitle(const CStringArray& aValues) const;
	CString GetValue(const CStringArray& aValues, int nCol) const;

	void AddAttributeToTask(ITASKLISTBASE* pTasks, HTASKITEM hTask, TDC_ATTRIBUTE nAttrib, const CStringArray& aValues) const;
	void AddCustomAttributesToTask(ITASKLISTBASE* pTasks, HTASKITEM hTask, const CStringArray& aValues) const;
	void AddCustomAttributeDefinitions(ITASKLISTBASE* pTasks) const;

	static CString GetLine(const CStringArray& aLines, int& nLine);
	static TDC_UNITS GetTimeUnits(const CStringArray& aValues, int nCol);
	static BOOL GetCustomAttribIDAndLabel(const TDCATTRIBUTEMAPPING& col, CString& sCustID, CString& sCustLabel);

};

#endif // !defined(AFX_TASKLISTCSVImPORTER_H__ADF211CB_FBD2_42A2_AD51_DFF58E566753__INCLUDED_)
