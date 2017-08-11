// TDCSourceControlHelper.h: interface for the CTDCCustomAttributeHelper class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TDCSOURCECONTROLHELPER_H__4044B3B7_1EA0_4279_9620_F2035DAE87DF__INCLUDED_)
#define AFX_TDCSOURCECONTROLHELPER_H__4044B3B7_1EA0_4279_9620_F2035DAE87DF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//////////////////////////////////////////////////////////////////////

class CToDoCtrl;
class CToDoCtrlData;

struct TODOITEM;

//////////////////////////////////////////////////////////////////////

/*

class CToDoCtrl
{
	friend class CTDCSourceControlHelper;

	...
};

*/

class CTDCSourceControlHelper  
{
public:
	CTDCSourceControlHelper(const CToDoCtrl& tdc);
	virtual ~CTDCSourceControlHelper();

	CString GetTasklistSourceControlPath() const;
	CString GetTaskSourceControlFolder() const;
	CString GetTaskSourceControlPath(DWORD dwTaskID) const;

	CString GetSourceControlID(BOOL bAlternate) const;
	BOOL MatchesSourceControlID(const CString& sID) const;

	BOOL CheckOutTasklist() const;
	BOOL CheckInTasklist() const;

	BOOL CheckOutTask(DWORD dwTaskID) const;
	BOOL CheckInTask(DWORD dwTaskID, TODOITEM& tdi) const;

	BOOL LoadCheckedOutTask(DWORD dwTaskID, TODOITEM& tdi) const;
	BOOL LoadCheckedOutTask(DWORD& dwTaskID, TODOITEM& tdi) const;

protected:
	const CToDoCtrl& m_tdc;

protected:
	CString GetTasklistPath() const;
};

//////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_TDCSOURCECONTROLHELPER_H__4044B3B7_1EA0_4279_9620_F2035DAE87DF__INCLUDED_)
