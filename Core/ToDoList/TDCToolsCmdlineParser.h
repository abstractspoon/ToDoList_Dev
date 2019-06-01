// ToolsCmdlineParser.h: interface for the CToolsCmdlineParser class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOOLSCMDLINEPARSER_H__D0CE619C_1C78_4BF1_9B60_CBCBD89C0B32__INCLUDED_)
#define AFX_TOOLSCMDLINEPARSER_H__D0CE619C_1C78_4BF1_9B60_CBCBD89C0B32__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>

enum CLA_TYPE
{
	CLAT_NONE,
	CLAT_PATHNAME,      
	CLAT_FILETITLE,    
	CLAT_FOLDER,        
	CLAT_FILENAME,      
	CLAT_USERFILE,
	CLAT_USERFOLDER,
	CLAT_USERTEXT,
	CLAT_USERDATE,
	CLAT_TODAYSDATE, // this gets handled internally
	CLAT_TODOLIST, 
	CLAT_SELTASKID,      
	CLAT_SELTASKTITLE,      
	CLAT_SELTASKEXTID,      
	CLAT_SELTASKCOMMENTS,      
	CLAT_SELTASKFILELINK,      
	CLAT_SELTASKALLOCBY,      
	CLAT_SELTASKALLOCTO,
	CLAT_SELTASKCUSTATTRIB,
};

struct CMDLINEARG
{
	CLA_TYPE nType;
	CString sName;
	CString sLabel;
	CString sDefValue;

	CString sPlaceHolder; // for when we want to replace it
};

typedef CArray<CMDLINEARG, CMDLINEARG&> CCLArgArray;

class CTDCToolsCmdlineParser  
{
public:
	CTDCToolsCmdlineParser(LPCTSTR szCmdLine = NULL);
	virtual ~CTDCToolsCmdlineParser();

	void SetCmdLine(LPCTSTR szCmdLine); // will clear previous results
	CString GetCmdLine() const { return m_sCmdLine; }

	int GetArguments(CCLArgArray& aArgs) const; // all
	int GetUserArguments(CCLArgArray& aArgs) const; // may include CUSTOM attribs
	int GetCustomAttributeArguments(CCLArgArray& aArgs) const; // may include USER attribs

	BOOL ReplaceArgument(CLA_TYPE nType, LPCTSTR szValue);
	BOOL ReplaceArgument(LPCTSTR szName, LPCTSTR szValue); // for USER/CUSTOM types sharing the same type

	BOOL HasArgument(CLA_TYPE nType) const;
	BOOL HasTasklistArgument() const;
	BOOL IsUserInputRequired() const;

	static BOOL PrepareToolPath(CString& sToolPath);
	static CString GetPlaceHolder(CLA_TYPE nType);

protected:
	CCLArgArray m_aArgs, m_aUserArgs; // user args are arguments which reference CMDLINEARG::sName
	CString m_sCmdLine;

	static CMap<CString, LPCTSTR, CLA_TYPE, CLA_TYPE&> s_mapTypes;

	void ParseCmdLine();
	CLA_TYPE GetType(LPCTSTR szVarType) const;
	BOOL IsUserInputType(CLA_TYPE nType) const;
	BOOL ReplaceArgument(int nArg, LPCTSTR szValue);
};

#endif // !defined(AFX_TOOLSCMDLINEPARSER_H__D0CE619C_1C78_4BF1_9B60_CBCBD89C0B32__INCLUDED_)
