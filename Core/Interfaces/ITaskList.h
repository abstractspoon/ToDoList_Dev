#if !defined(_ITASKLIST_H__5951FDE6_508A_4A9D_A55D_D16EB026AEF7__INCLUDED_)
#define _ITASKLIST_H__5951FDE6_508A_4A9D_A55D_D16EB026AEF7__INCLUDED_

////////////////////////////////////////////////////////////////////////////////

#include "IEnums.h"

////////////////////////////////////////////////////////////////////////////////

#pragma warning(disable:4505)

/////////////////////////////////////////////////////////////////////////////////
//
// ITaskList.h: interface and implementation of the ITaskList class.
//
/////////////////////////////////////////////////////////////////////////////////

// latest interface
#define ITASKLISTBASE	 ITaskList17 
#define IID_TASKLISTBASE IID_TASKLIST17

/////////////////////////////////////////////////////////////////////////////////

// extended interface IDs
static const GUID IID_TASKLIST2  = { 0x41d9fd9e, 0xaa1f, 0x4ee0, { 0x86, 0x05, 0xeb, 0x3f, 0x64, 0x7e, 0x70, 0xf6 } };
static const GUID IID_TASKLIST3  = { 0xb75b2120, 0x267d, 0x4a82, { 0xab, 0x58, 0x1a, 0x45, 0x40, 0x50, 0x0a, 0x1a } };
static const GUID IID_TASKLIST4  = { 0xd063e3de, 0x83d1, 0x40d7, { 0xbc, 0x2d, 0xfa, 0x24, 0x04, 0x85, 0x57, 0xed } };
static const GUID IID_TASKLIST5  = { 0x5a1ac54b, 0x084f, 0x4299, { 0xb5, 0xaa, 0x2b, 0x93, 0x22, 0xf1, 0x3d, 0xc0 } };
static const GUID IID_TASKLIST6  = { 0xb782136e, 0x546f, 0x4184, { 0xab, 0xaf, 0xe9, 0x07, 0xdd, 0xf6, 0x91, 0x81 } };
static const GUID IID_TASKLIST7  = { 0x65781a9b, 0xced2, 0x490a, { 0xa9, 0x39, 0x71, 0x6d, 0xd9, 0x18, 0x53, 0x33 } };
static const GUID IID_TASKLIST8  = { 0xdd98646c, 0xe608, 0x4109, { 0x9a, 0xf5, 0x9e, 0xbd, 0x93, 0x6d, 0x74, 0x0b } };
static const GUID IID_TASKLIST9  = { 0x3fe8c9ff, 0x91d2, 0x431b, { 0x9d, 0xeb, 0xb5, 0xc6, 0xa6, 0x83, 0x5b, 0x71 } };
static const GUID IID_TASKLIST10 = { 0xe925ad5f, 0x0958, 0x45b0, { 0x96, 0x15, 0x11, 0x82, 0xd7, 0x7a, 0xc7, 0xb2 } };
static const GUID IID_TASKLIST11 = { 0xaadd0c6d, 0x5fd8, 0x4995, { 0xa1, 0x3c, 0xce, 0x4c, 0x9f, 0x2b, 0xc0, 0x7b } };
static const GUID IID_TASKLIST12 = { 0x6c65c122, 0x6406, 0x4d53, { 0x97, 0x3a, 0x4c, 0x7b, 0x00, 0x49, 0xff, 0x1e } };
static const GUID IID_TASKLIST13 = { 0x5951fde6, 0x508a, 0x4a9d, { 0xa5, 0x5d, 0xd1, 0x6e, 0xb0, 0x26, 0xae, 0xf7 } };
static const GUID IID_TASKLIST14 = { 0x0e95dc97, 0x41f7, 0x4d9c, { 0xb4, 0x25, 0xf5, 0xd4, 0xc2, 0xb1, 0xba, 0x4c } };
static const GUID IID_TASKLIST15 = { 0x2a8fd5f0, 0x63ae, 0x485f, { 0xbc, 0x5e, 0xa2, 0xa2, 0xc2, 0xc2, 0x51, 0x5e } };
static const GUID IID_TASKLIST16 = { 0x66fc35d2, 0xc471, 0x4af6, { 0xbe, 0xfc, 0xf3, 0xba, 0x5f, 0xfd, 0x90, 0xd1 } };
static const GUID IID_TASKLIST17 = { 0xd44e1030, 0x3c18, 0x41df, { 0xb7, 0x5e, 0x70, 0xcf, 0xd8, 0x62, 0x15, 0x47 } };

/////////////////////////////////////////////////////////////////////////////////

typedef void* HTASKITEM;
typedef __int64 time64_t;

/////////////////////////////////////////////////////////////////////////////////////////////

class ITaskList;

/////////////////////////////////////////////////////////////////////////////////

// handy template functions for extracting interfaces
template <class TLInterface> 
const TLInterface* GetITLInterface(const ITaskList* pTasks, const GUID& IID)
{
	TLInterface* pInterface = 0;
	ITaskList* pNCTasks = const_cast<ITaskList*>(pTasks);

	if (pNCTasks)
		pNCTasks->QueryInterface(IID, reinterpret_cast<void**>(&pInterface));

	return reinterpret_cast<TLInterface*>(pInterface);
}

template <class TLInterface> 
TLInterface* GetITLInterface(ITaskList* pTasks, const GUID& IID)
{
	TLInterface* pInterface = 0;

	if (pTasks)
		pTasks->QueryInterface(IID, reinterpret_cast<void**>(&pInterface));

	return reinterpret_cast<TLInterface*>(pInterface);
}

/////////////////////////////////////////////////////////////////////////////////

class IMultiTaskList : public IUnknown
{
public:
	virtual int GetTaskListCount() const = 0;
	virtual const ITaskList* GetTaskList(int nTaskList = 0) const = 0;
};

/////////////////////////////////////////////////////////////////////////////////

class ITaskList : public IUnknown 
{
public:
	// file level methods
	virtual bool IsArchive() const = 0;
	virtual bool IsCheckedOut() const = 0; // deprecated. Will always return false
	virtual bool IsSourceControlled() const = 0;
	
	virtual LPCWSTR GetProjectName() const = 0;
	virtual LPCWSTR GetCheckOutTo() const = 0;
	
	virtual unsigned long GetFileFormat() const = 0;
	virtual unsigned long GetFileVersion() const = 0;
	
	virtual bool SetProjectName(LPCWSTR szName) = 0;
	virtual bool SetFileVersion(unsigned long nVersion) = 0;

	// task level methods
	virtual HTASKITEM NewTask(LPCWSTR szTitle, HTASKITEM hParent = NULL) = 0;

	virtual HTASKITEM GetFirstTask(HTASKITEM hParent = NULL) const = 0;
	virtual HTASKITEM GetNextTask(HTASKITEM hTask) const = 0;

	// get methods
	virtual LPCWSTR GetTaskTitle(HTASKITEM hTask) const = 0;
	virtual LPCWSTR GetTaskComments(HTASKITEM hTask) const = 0;
	virtual LPCWSTR GetTaskAllocatedTo(HTASKITEM hTask) const = 0;
	virtual LPCWSTR GetTaskAllocatedBy(HTASKITEM hTask) const = 0;
	virtual LPCWSTR GetTaskCategory(HTASKITEM hTask) const = 0;
	virtual LPCWSTR GetTaskStatus(HTASKITEM hTask) const = 0;
	virtual LPCWSTR GetTaskFileLinkPath(HTASKITEM hTask) const = 0;
	virtual LPCWSTR GetTaskWebColor(HTASKITEM hTask) const = 0;
	virtual LPCWSTR GetTaskPriorityWebColor(HTASKITEM hTask) const = 0;

	virtual unsigned long GetTaskID(HTASKITEM hTask) const = 0;
	virtual unsigned long GetTaskColor(HTASKITEM hTask) const = 0;
	virtual unsigned long GetTaskPriorityColor(HTASKITEM hTask) const = 0;

	virtual int GetTaskPriority(HTASKITEM hTask, bool bHighest) const = 0;
	virtual unsigned char GetTaskPercentDone(HTASKITEM hTask, bool bCalc) const = 0;

	virtual double GetTaskTimeEstimate(HTASKITEM hTask, TDC_UNITS& cUnits, bool bCalc) const = 0;
	virtual double GetTaskTimeSpent(HTASKITEM hTask, TDC_UNITS& cUnits, bool bCalc) const = 0;

	virtual time_t GetTaskLastModified(HTASKITEM hTask) const = 0;
	virtual time_t GetTaskDoneDate(HTASKITEM hTask) const = 0;
	virtual time_t GetTaskDueDate(HTASKITEM hTask) const = 0;
	virtual time_t GetTaskStartDate(HTASKITEM hTask) const = 0;

	virtual LPCWSTR GetTaskDoneDateString(HTASKITEM hTask) const = 0;
	virtual LPCWSTR GetTaskDueDateString(HTASKITEM hTask) const = 0;
	virtual LPCWSTR GetTaskStartDateString(HTASKITEM hTask) const = 0;

	virtual bool IsTaskDone(HTASKITEM hTask) const = 0;
	virtual bool IsTaskDue(HTASKITEM hTask) const = 0;

	virtual unsigned long GetTaskPosition(HTASKITEM hTask) const = 0;

	virtual bool TaskHasAttribute(HTASKITEM hTask, LPCWSTR szAttrib) const = 0;
	virtual LPCWSTR GetTaskAttribute(HTASKITEM hTask, LPCWSTR szAttrib) const = 0;
	virtual HTASKITEM GetTaskParent(HTASKITEM hTask) const = 0;
		
	// set methods
	virtual bool SetTaskTitle(HTASKITEM hTask, LPCWSTR szTitle) = 0;
	virtual bool SetTaskComments(HTASKITEM hTask, LPCWSTR szComments) = 0;
	virtual bool SetTaskAllocatedTo(HTASKITEM hTask, LPCWSTR szAllocTo) = 0;
	virtual bool SetTaskAllocatedBy(HTASKITEM hTask, LPCWSTR szAllocBy) = 0;
	virtual bool SetTaskCategory(HTASKITEM hTask, LPCWSTR szCategory) = 0;
	virtual bool SetTaskStatus(HTASKITEM hTask, LPCWSTR szStatus) = 0;
	virtual bool SetTaskFileLinkPath(HTASKITEM hTask, LPCWSTR szFileLink) = 0;

	virtual bool SetTaskColor(HTASKITEM hTask, unsigned long nColor) = 0;

	virtual bool SetTaskPriority(HTASKITEM hTask, int nPriority) = 0;
	virtual bool SetTaskPercentDone(HTASKITEM hTask, unsigned char nPercent) = 0;

	virtual bool SetTaskTimeEstimate(HTASKITEM hTask, double dTimeEst, TDC_UNITS cUnits) = 0;
	virtual bool SetTaskTimeSpent(HTASKITEM hTask, double dTimeSpent, TDC_UNITS cUnits) = 0;

	virtual bool SetTaskLastModified(HTASKITEM hTask, time_t tLastMod) = 0;
	virtual bool SetTaskDoneDate(HTASKITEM hTask, time_t tDoneDate) = 0;
	virtual bool SetTaskDueDate(HTASKITEM hTask, time_t tDueDate) = 0;
	virtual bool SetTaskStartDate(HTASKITEM hTask, time_t tStartDate) = 0;

	virtual bool SetTaskPosition(HTASKITEM hTask, unsigned long nPos) = 0;

	// new methods
	virtual bool SetTaskFlag(HTASKITEM hTask, bool bFlag) = 0;
	virtual bool IsTaskFlagged(HTASKITEM hTask) const = 0;

};

class ITaskList2 : public ITaskList
{
	// new methods
public:
	virtual LPCWSTR GetTaskCreatedBy(HTASKITEM hTask) const = 0;
	virtual time_t GetTaskCreationDate(HTASKITEM hTask) const = 0;
	virtual LPCWSTR GetTaskCreationDateString(HTASKITEM hTask) const = 0;
	
	virtual bool SetTaskCreatedBy(HTASKITEM hTask, LPCWSTR szCreatedBy) = 0;
	virtual bool SetTaskCreationDate(HTASKITEM hTask, time_t tCreationDate) = 0;
};

class ITaskList3 : public ITaskList2
{
	// new methods
public:
	virtual time_t GetTaskDueDate(HTASKITEM hTask, bool bCalc) const = 0;
	virtual LPCWSTR GetTaskDueDateString(HTASKITEM hTask, bool bCalc) const = 0;
	virtual COLORREF GetTaskTextColor(HTASKITEM hTask) const = 0;
	virtual int GetTaskRisk(HTASKITEM hTask, bool bHighest) const = 0;
	virtual LPCWSTR GetTaskExternalID(HTASKITEM hTask) const = 0;
	
	virtual bool SetTaskRisk(HTASKITEM hTask, int nRisk) = 0;
	virtual bool SetTaskExternalID(HTASKITEM hTask, LPCWSTR szID) = 0;
};

class ITaskList4 : public ITaskList3
{
	// new methods
public:
	virtual LPCWSTR GetAttribute(LPCWSTR szAttrib) const = 0;

	virtual LPCWSTR GetHtmlCharSet() const = 0;
	virtual LPCWSTR GetReportTitle() const = 0;
	virtual LPCWSTR GetReportDate() const = 0;
	virtual double GetTaskCost(HTASKITEM hTask, bool bCalc) const = 0;
	virtual int GetTaskCategoryCount(HTASKITEM hTask) const = 0;
	virtual LPCWSTR GetTaskCategory(HTASKITEM hTask, int nIndex) const = 0;
	virtual LPCWSTR GetTaskDependency(HTASKITEM hTask) const = 0;

	virtual bool SetTaskCost(HTASKITEM hTask, double dCost) = 0;
	virtual bool SetTaskDependency(HTASKITEM hTask, LPCWSTR szDepends) = 0;
};

class ITaskList5 : public ITaskList4
{
	// new methods
public:
	virtual bool AddTaskCategory(HTASKITEM hTask, LPCWSTR szCategory) = 0;
};

class ITaskList6 : public ITaskList5
{
	// new methods
public:
	virtual bool SetTaskRecurrence(HTASKITEM hTask, TDC_REGULARITY nRegularity, DWORD dwSpecific1, 
									DWORD dwSpecific2, bool bRecalcFromDue, TDC_RECURREUSEOPTION nReuse) = 0;
	virtual bool GetTaskRecurrence(HTASKITEM hTask, TDC_REGULARITY& nRegularity, DWORD& dwSpecific1, 
									DWORD& dwSpecific2, bool& bRecalcFromDue, TDC_RECURREUSEOPTION& nReuse) const = 0;

	virtual bool SetTaskVersion(HTASKITEM hTask, LPCWSTR szVersion) = 0;
	virtual LPCWSTR GetTaskVersion(HTASKITEM hTask) const = 0;
};

class ITaskList7 : public ITaskList6
{
	// new methods
public:
	virtual int GetTaskDependencyCount(HTASKITEM hTask) const = 0;
	virtual bool AddTaskDependency(HTASKITEM hTask, LPCWSTR szDepends) = 0;
	virtual bool AddTaskDependency(HTASKITEM hTask, unsigned long dwID) = 0;
	virtual LPCWSTR GetTaskDependency(HTASKITEM hTask, int nIndex) const = 0;

	virtual int GetTaskAllocatedToCount(HTASKITEM hTask) const = 0;
	virtual bool AddTaskAllocatedTo(HTASKITEM hTask, LPCWSTR szAllocTo) = 0;
	virtual LPCWSTR GetTaskAllocatedTo(HTASKITEM hTask, int nIndex) const = 0;
};

class ITaskList8 : public ITaskList7
{
	// new methods
public:
	virtual HTASKITEM NewTask(LPCWSTR szTitle, HTASKITEM hParent, unsigned long dwID) = 0;
	virtual unsigned long GetTaskParentID(HTASKITEM hTask) const = 0;
	virtual HTASKITEM FindTask(unsigned long dwTaskID) const = 0;
	virtual int GetTaskCount() const = 0;
	virtual void Reset() = 0;

 	virtual bool SetTaskAttribute(HTASKITEM hTask, LPCWSTR szAttrib, LPCWSTR szValue) = 0;
};

class ITaskList9 : public ITaskList8
{
	// new methods
public:
	virtual int GetTaskTagCount(HTASKITEM hTask) const = 0;
	virtual LPCWSTR GetTaskTag(HTASKITEM hTask, int nIndex) const = 0;
	virtual bool AddTaskTag(HTASKITEM hTask, LPCWSTR szTag) = 0;

	virtual LPCWSTR GetTaskPositionString(HTASKITEM hTask) const = 0;
	virtual bool SetTaskPosition(HTASKITEM hTask, LPCWSTR szPos) = 0;
};

class ITaskList10 : public ITaskList9
{
	// new methods
public:
	// tasklist meta data
	virtual LPCWSTR GetMetaData(LPCWSTR szKey) const = 0;
	virtual bool SetMetaData(LPCWSTR szKey, LPCWSTR szMetaData) = 0;
	virtual bool ClearMetaData(LPCWSTR szKey) = 0;

	// task meta data
	virtual LPCWSTR GetTaskMetaData(HTASKITEM hTask, LPCWSTR szKey) const = 0;
	virtual bool SetTaskMetaData(HTASKITEM hTask, LPCWSTR szKey, LPCWSTR szMetaData) = 0;
	virtual bool ClearTaskMetaData(HTASKITEM hTask, LPCWSTR szKey) = 0;

	// user-defined data definitions
	virtual int GetCustomAttributeCount() const = 0;
	virtual LPCWSTR GetCustomAttributeLabel(int nIndex) const = 0;
	virtual LPCWSTR GetCustomAttributeID(int nIndex) const = 0;
	virtual LPCWSTR GetCustomAttributeValue(int nIndex, LPCWSTR szItem) const = 0;

	// user-defined data
	virtual LPCWSTR GetTaskCustomAttributeData(HTASKITEM hTask, LPCWSTR szID) const = 0;
	virtual bool SetTaskCustomAttributeData(HTASKITEM hTask, LPCWSTR szID, LPCWSTR szData) = 0;
	virtual bool ClearTaskCustomAttributeData(HTASKITEM hTask, LPCWSTR szID) = 0;
};

class ITaskList11 : public ITaskList10
{
	// new methods
public:
	virtual time_t GetTaskStartDate(HTASKITEM hTask, bool bCalc) const = 0;
	virtual LPCWSTR GetTaskStartDateString(HTASKITEM hTask, bool bCalc) const = 0;

	virtual bool SetTaskIcon(HTASKITEM hTask, LPCWSTR szIcon) = 0;
	virtual LPCWSTR GetTaskIcon(HTASKITEM hTask) const = 0;
};

class ITaskList12 : public ITaskList11
{
	// new methods
public:
	virtual bool GetTaskCreationDate64(HTASKITEM hTask, time64_t& timeT) const = 0;
	virtual bool GetTaskStartDate64(HTASKITEM hTask, bool bCalc, time64_t& timeT) const = 0;
	virtual bool GetTaskDueDate64(HTASKITEM hTask, bool bCalc, time64_t& timeT) const = 0;
	virtual bool GetTaskDoneDate64(HTASKITEM hTask, time64_t& timeT) const = 0;
	virtual bool GetTaskLastModified64(HTASKITEM hTask, time64_t& timeT) const = 0;

	virtual bool SetTaskCreationDate64(HTASKITEM hTask, time64_t tCreationdate) = 0;
	virtual bool SetTaskLastModified64(HTASKITEM hTask, time64_t tLastMod) = 0;
	virtual bool SetTaskDoneDate64(HTASKITEM hTask, time64_t tDoneDate) = 0;
	virtual bool SetTaskDueDate64(HTASKITEM hTask, time64_t tDueDate) = 0;
	virtual bool SetTaskStartDate64(HTASKITEM hTask, time64_t tStartDate) = 0;

	virtual COLORREF GetTaskBkgndColor(HTASKITEM hTask) const = 0;

	virtual bool DeleteTask(HTASKITEM hTask) = 0;

};

class ITaskList13 : public ITaskList12
{
	// new methods
public:
	virtual bool SetTaskRecurrence(HTASKITEM hTask, TDC_REGULARITY nRegularity, DWORD dwSpecific1, 
									DWORD dwSpecific2, TDC_RECURFROMOPTION nRecalcFrom, 
									TDC_RECURREUSEOPTION nReuse, int nNumOccur) = 0;
	virtual bool GetTaskRecurrence(HTASKITEM hTask, TDC_REGULARITY& nRegularity, DWORD& dwSpecific1, 
									DWORD& dwSpecific2, TDC_RECURFROMOPTION& nRecalcFrom, 
									TDC_RECURREUSEOPTION& nReuse, int& nNumOccur) const = 0;

	virtual LPCWSTR GetTaskSubtaskCompletion(HTASKITEM hTask) const = 0;
	virtual bool AddCustomAttribute(LPCWSTR szID, LPCWSTR szLabel, LPCWSTR szColumn, bool bList = false) = 0;
};

class ITaskList14 : public ITaskList13
{
	// new methods
public:
	virtual int GetTaskFileLinkCount(HTASKITEM hTask) const = 0;
	virtual bool AddTaskFileLink(HTASKITEM hTask, LPCWSTR szFileLink) = 0;
	virtual LPCWSTR GetTaskFileLink(HTASKITEM hTask, int nIndex) const = 0;
	virtual bool IsTaskGoodAsDone(HTASKITEM hTask) const = 0;
	virtual LPCWSTR GetTaskCustomDateString(HTASKITEM hTask, LPCWSTR szID) const = 0;
};

class ITaskList15 : public ITaskList14
{
	// new methods
public:
	virtual bool IsTaskParent(HTASKITEM hTask) const = 0;

	virtual int GetAttributeCount(LPCWSTR szAttrib) const = 0;
	virtual LPCWSTR GetAttributeItem(LPCWSTR szAttrib, int nIndex) const = 0;

	virtual unsigned long GetCustomAttributeType(int nIndex) const = 0;
	virtual LPCWSTR GetCustomAttributeListData(int nIndex) const = 0;
	virtual bool IsCustomAttributeEnabled(int nIndex) const = 0;
	
	virtual DWORD GetTaskReferenceID(HTASKITEM hTask) const = 0;
	virtual bool IsTaskReference(HTASKITEM hTask) const = 0;

};

class ITaskList16 : public ITaskList15
{
	// new methods
public:
	virtual bool IsTaskLocked(HTASKITEM hTask, bool bCalc) const = 0;
	virtual bool SetTaskLock(HTASKITEM hTask, bool bLocked) = 0;
	virtual bool IsTaskFlagged(HTASKITEM hTask, bool bCalc) const = 0;
	
	virtual LPCWSTR GetTaskCustomAttributeData(HTASKITEM hTask, LPCWSTR szID, bool bDisplay) const = 0;
	virtual LPCWSTR GetTaskLastModifiedBy(HTASKITEM hTask) const = 0;
	virtual bool SetTaskLastModifiedBy(HTASKITEM hTask, LPCWSTR szModifiedBy) = 0;

	virtual unsigned long GetCustomAttributeTypeByID(LPCWSTR szID) const = 0;

	virtual bool SetTaskRecurrence(HTASKITEM hTask, TDC_REGULARITY nRegularity, 
									DWORD dwSpecific1, DWORD dwSpecific2, 
									TDC_RECURFROMOPTION nRecalcFrom, TDC_RECURREUSEOPTION nReuse, 
									int nNumOccur, int nRemainingOccur, bool bPreserveComments) = 0;
	virtual bool GetTaskRecurrence(HTASKITEM hTask, TDC_REGULARITY& nRegularity, 
									DWORD& dwSpecific1, DWORD& dwSpecific2, 
									TDC_RECURFROMOPTION& nRecalcFrom, TDC_RECURREUSEOPTION& nReuse, 
									int& nNumOccur, int& nRemainingOccur, bool& bPreserveComments) const = 0;
};

class ITaskList17 : public ITaskList16
{
	// new methods
public:
	virtual LPCWSTR GetFileName(bool bFullPath) const = 0;

	virtual bool IsTaskRecurring(HTASKITEM hTask) const = 0;
	virtual bool IsAttributeAvailable(TDC_ATTRIBUTE nAttrib) const = 0;
	virtual bool TaskHasAttribute(HTASKITEM hTask, TDC_ATTRIBUTE nAttrib, bool bCalc = false, bool bDisplay = false) const = 0;

	virtual LPCWSTR GetTaskAttribute(HTASKITEM hTask, TDC_ATTRIBUTE nAttrib, bool bCalc = false, bool bDisplay = false) const = 0;
	
	virtual bool SetTaskCost(HTASKITEM hTask, double dCost, bool bIsRate) = 0;
	virtual double GetTaskCost(HTASKITEM hTask, bool bCalc, bool& bIsRate) const = 0;
};

#endif // _ITASKLIST_H__5951FDE6_508A_4A9D_A55D_D16EB026AEF7__INCLUDED_