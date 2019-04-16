// IAttribute.h: attribute enum to serve all interfaces
//
/////////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_IATTRIBUTE_H__7741547B_BA15_4851_A41B_2B4EC1DC12D5__INCLUDED_)
#define AFX_IATTRIBUTE_H__7741547B_BA15_4851_A41B_2B4EC1DC12D5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////////

enum I_ATTRIBUTE
{
	// NEVER CHANGE THE ORDER OF THIS LIST
	IA_NONE		= 0,
	IA_TASKNAME,			
	IA_DONEDATE,			
	IA_DUEDATE	,			
	IA_STARTDATE,			
	IA_PRIORITY,		
	IA_COLOR,		
	IA_ALLOCTO,		
	IA_ALLOCBY,		
	IA_STATUS,		
	IA_CATEGORY,		
	IA_PERCENT,		
	IA_TIMEEST,		
	IA_TIMESPENT,		
	IA_FILEREF,		
	IA_COMMENTS,		
	IA_FLAG,		
	IA_CREATIONDATE,		
	IA_CREATEDBY,		
	IA_RISK,				
	IA_EXTERNALID,		
	IA_COST,				
	IA_DEPENDENCY,		
	IA_RECURRENCE,		
	IA_VERSION,			
	IA_POSITION,		
	IA_ID,		
	IA_LASTMOD,		
	IA_ICON,		
	IA_TAGS,		
	IA_CUSTOMATTRIB,		
	IA_OFFSETTASK,		
	IA_LOCK,
	IA_SUBTASKDONE,
	IA_TASKNAMEORCOMMENTS,
	IA_ANYTEXTATTRIBUTE,
	IA_METADATA,
	IA_PROJECTNAME,

	// new values here
	// IA_

	// ALWAYS THE LAST VALUE
	IA_NUMATTRIBUTES
};

//////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_IATTRIBUTE_H__7741547B_BA15_4851_A41B_2B4EC1DC12D5__INCLUDED_)
