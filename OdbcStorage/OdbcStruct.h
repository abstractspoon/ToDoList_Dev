// OdbcTasklistStorage.h: interface for the COdbcTasklistStorage class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ODBCSTRUCT_H__1A49027C_CC71_4EB8_8540_B319AB9A045E__INCLUDED_)
#define AFX_ODBCSTRUCT_H__1A49027C_CC71_4EB8_8540_B319AB9A045E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "resource.h"

#include "..\todolist\tdcenum.h"

#include "..\shared\timehelper.h"

#include "..\Interfaces\ITasklist.h"

//////////////////////////////////////////////////////////////////////

typedef __int64 time64_t;

//////////////////////////////////////////////////////////////////////

enum 
{
	OT_DONEDATE		= 0x00000001,
	OT_DUEDATE		= 0x00000002,
	OT_STARTDATE	= 0x00000004,
	OT_PRIORITY		= 0x00000008,
	OT_COLOR		= 0x00000010,
	OT_ALLOCTO		= 0x00000020,
	OT_ALLOCBY		= 0x00000040,
	OT_STATUS		= 0x00000080,
	OT_CATEGORY		= 0x00000100,
	OT_PERCENT		= 0x00000200,
	OT_TIMEEST		= 0x00000400,
	OT_TIMESPENT	= 0x00000800,
	OT_FILEREF		= 0x00001000,
	OT_COMMENTS		= 0x00002000,
	OT_FLAG			= 0x00004000,
	OT_CREATIONDATE = 0x00008000,
	OT_CREATEDBY	= 0x00010000,
	OT_RISK			= 0x00020000,			
	OT_EXTERNALID	= 0x00040000,	
	OT_COST			= 0x00080000,			
	OT_DEPENDENCY	= 0x00100000,	
	OT_RECURRENCE	= 0x00200000,	
	OT_VERSION		= 0x00400000,		
	OT_LASTMOD		= 0x00800000,
	OT_ICON			= 0x01000000,
	OT_TAGS			= 0x02000000,
	OT_TITLE		= 0x04000000,

	OT_ALL			= 0xFFFFFFFF
};


//////////////////////////////////////////////////////////////////////

typedef CMap<CString, LPCTSTR, DWORD, DWORD&> COdbcMapKeyToID;
typedef CMap<DWORD, DWORD, CString, CString&> COdbcMapIDToKey;

//////////////////////////////////////////////////////////////////////

class COdbcTaskArray;

struct ODBCTASK
{
	ODBCTASK();

	CString sTaskKey, sParentKey;

	CString sAllocBy, sComments, sCreatedBy, sVersion, sExternalID, sFileRef, sIcon, sStatus, sTitle;
	CStringArray aAllocTo, aCategories, aTags, aDepends;
	time64_t tCreation, tStart, tDue, tDone, tModified;
	double dCost, dTimeEst, dTimeSpent;
	long nPriority, nRisk, nPercent, color, bFlag;
	DWORD dwID, dwParentID;

	BOOL HasAttribute(DWORD dwAttrib) const;
	void SetHasAttribute(DWORD dwAttrib, BOOL bSet = TRUE);

	BOOL Save(ITaskList12* pTasks, HTASKITEM hTask, const COdbcMapKeyToID& mapTasks) const;
	BOOL Load(const ITaskList12* pTasks, HTASKITEM hTask, COdbcMapIDToKey& mapTasks);

	static CString GetMetaDataKey(LPCTSTR szSubKey);
	static TH_UNITS MapUnitsToTHUnits(TDC_UNITS nUnits);

protected:
	DWORD dwAttribs;

protected:
	void SetAttrib(CString& sVar, LPCTSTR szVal, DWORD dwAttrib);
	void SetAttrib(long& nVar, long nVal, DWORD dwAttrib, long nNullVal = 0);
	void SetAttrib(double& dVar, double dVal, DWORD dwAttrib, double dNullVal = 0.0);

};

//////////////////////////////////////////////////////////////////////

class COdbcTaskArray : public CArray<ODBCTASK*, ODBCTASK*&>
{
public:
	virtual ~COdbcTaskArray();

	void RemoveAll();
	void Sort();
	
protected:
	static int TaskSortProc(const void* pV1, const void* pV2);
};

//////////////////////////////////////////////////////////////////////

class COdbcMapKeyToTask : public CMap<CString, LPCTSTR, ODBCTASK*, ODBCTASK*&>
{
public:
	virtual ~COdbcMapKeyToTask();
};

//////////////////////////////////////////////////////////////////////

struct ODBCTABLESETUP
{
	ODBCTABLESETUP();

	BOOL Verify() const;
	BOOL Verify(const CString& sTableDesc, CStringArray& aErrors) const;

	// attributes
	CString sTableName;
	CString sKeyField;
	CString sContentField;
};

struct ODBCTASKSTABLESETUP
{
	ODBCTASKSTABLESETUP();

	BOOL Verify(BOOL bVerifyTasklistField) const;
	BOOL Verify(const CString& sTableDesc, CStringArray& aErrors, BOOL bVerifyTasklistField) const;

	// attributes
	CString sTableName;
	CString sTaskKeyField;
	CString sParentKeyField;
	CString sTasklistKeyField;
};

//////////////////////////////////////////////////////////////////////

struct ODBCTABLESSETUP
{
	ODBCTABLESSETUP();

	BOOL Verify() const;
	BOOL Verify(CStringArray& aErrors) const;

	// attributes
	ODBCTASKSTABLESETUP tableTasks;
	ODBCTABLESETUP tableTasklists;
	ODBCTABLESETUP tableAllocTo;
};

//////////////////////////////////////////////////////////////////////

struct ODBCATTRIBUTESETUP
{
	ODBCATTRIBUTESETUP();

	BOOL Verify(BOOL bVerifyLinkTasklistKey) const;
	BOOL Verify(CStringArray& aErrors, BOOL bVerifyLinkTasklistKey) const;

	CString GetTaskAttributeName() const;
	static CString GetTaskAttributeName(TDC_ATTRIBUTE nAttrib);

	// attributes
	TDC_ATTRIBUTE nAttrib;
	CString sTaskField;

	CString sAttribTable;
	CString sAttribKeyField;
	CString sAttribContentField;

	CString sLinkTable;
	CString sLinkTaskKeyField;
	CString sLinkTaskListKeyField;
	CString sLinkAttribKeyField;
};
typedef CArray<ODBCATTRIBUTESETUP, ODBCATTRIBUTESETUP&> COdbcAttribSetupArray;

//////////////////////////////////////////////////////////////////////

struct ODBCSETUP
{
	ODBCSETUP();
	ODBCSETUP(const ODBCSETUP& other);

	ODBCSETUP& operator=(const ODBCSETUP& other);

	BOOL Verify() const;
	BOOL Verify(CStringArray& aErrors) const;

	// attributes
	ODBCTABLESSETUP tables;
	COdbcAttribSetupArray aAttrib;
};

//////////////////////////////////////////////////////////////////////

struct ODBCDATABASESETUP
{
	ODBCDATABASESETUP();

	BOOL Verify() const;
	BOOL Verify(CStringArray& aErrors) const;

	// attributes
	CString sDbName;
	CString sConnect;
	CString sTasklistID, sTasklist;
	CString sAllocToID, sAllocTo;

	ODBCSETUP setup;
};
typedef CArray<ODBCDATABASESETUP, ODBCDATABASESETUP&> COdbcDatabaseSetupArray;

//////////////////////////////////////////////////////////////////////

struct ATTRIBNAME
{
	TDC_ATTRIBUTE nAttrib;
	UINT nIDName;
};

const ATTRIBNAME TASKATTRIB_NAMES[] = 
{
	{ TDCA_TASKNAME,		IDS_TA_TITLE },
	{ TDCA_DONEDATE,		IDS_TA_DONEDATE },
	{ TDCA_DUEDATE,			IDS_TA_DUEDATE },
	{ TDCA_STARTDATE,		IDS_TA_STARTDATE },
	{ TDCA_PRIORITY,		IDS_TA_PRIORITY },
	{ TDCA_COLOR,			IDS_TA_COLOR },
	{ TDCA_ALLOCTO,			IDS_TA_ALLOCTO },
	{ TDCA_ALLOCBY,			IDS_TA_ALLOCBY },
	{ TDCA_STATUS,			IDS_TA_STATUS },
	{ TDCA_CATEGORY,		IDS_TA_CATEGORY },
	{ TDCA_PERCENT,			IDS_TA_PERCENT },
	{ TDCA_TIMEEST,			IDS_TA_TIMEEST },
	{ TDCA_TIMESPENT,		IDS_TA_TIMESPENT },
	{ TDCA_FILEREF,			IDS_TA_FILEREF },
	{ TDCA_COMMENTS,		IDS_TA_COMMENTS },
	{ TDCA_FLAG,			IDS_TA_FLAG },
	{ TDCA_CREATIONDATE,	IDS_TA_CREATEDATE },
	{ TDCA_CREATEDBY,		IDS_TA_CREATEDBY },
	{ TDCA_RISK,			IDS_TA_RISK },			
	{ TDCA_EXTERNALID,		IDS_TA_EXTERNALID },	
	{ TDCA_COST,			IDS_TA_COST },			
	{ TDCA_DEPENDENCY,		IDS_TA_DEPENDS },	
	{ TDCA_RECURRENCE,		IDS_TA_RECURRENCE },	
	{ TDCA_VERSION,			IDS_TA_VERSION },		
	{ TDCA_POSITION,		IDS_TA_POS },
	{ TDCA_LASTMOD,			IDS_TA_MODIFYDATE },
	{ TDCA_ICON,			IDS_TA_ICON },
	{ TDCA_TAGS,			IDS_TA_TAGS },
};
const int NUM_ATTRIB = (sizeof(TASKATTRIB_NAMES) / sizeof(ATTRIBNAME));

//////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_ODBCSTRUCT_H__1A49027C_CC71_4EB8_8540_B319AB9A045E__INCLUDED_)
