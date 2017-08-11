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

	BOOL CheckOutTasklist(CString& sCheckedOutTo) const;
	BOOL CheckInTasklist() const;

	BOOL CheckOutTask(DWORD dwTaskID, CString& sCheckedOutTo) const;
	BOOL CheckInTask(DWORD dwTaskID, TODOITEM& tdi) const;

	BOOL LoadCheckedOutTask(DWORD dwTaskID, TODOITEM& tdi) const;
	BOOL LoadCheckedOutTask(DWORD& dwTaskID, TODOITEM& tdi) const;

	BOOL IsTasklistCheckedOut();
	BOOL IsTasklistCheckedOut(CString& sCheckedOutTo);
	BOOL IsTasklistCheckedOutToUs();

	CString GetTasklistSourceControlPath() const;
	CString GetTaskSourceControlFolder() const;
	CString GetTaskSourceControlPath(DWORD dwTaskID) const;

	CString GetSourceControlID(BOOL bAlternate) const;
	BOOL MatchesOurSourceControlID(const CString& sID) const;

	BOOL IsTaskCheckedOut(DWORD dwTaskID);
	BOOL IsTaskCheckedOut(DWORD dwTaskID, CString& sCheckedOutTo);
	BOOL IsTaskCheckedOutToUs(DWORD dwTaskID);

protected:
	const CToDoCtrl& m_tdc;

protected:
	CString GetTasklistPath() const;

	static BOOL CreateSentinelFile(LPCTSTR szPath);
	static BOOL DeleteSentinelFile(LPCTSTR szPath);
	static BOOL SourceControlIDsMatch(const CString& sID1, const CString& sID2);
	static CString GetCheckedOutTo(LPCTSTR szPath);
};

//////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_TDCSOURCECONTROLHELPER_H__4044B3B7_1EA0_4279_9620_F2035DAE87DF__INCLUDED_)
