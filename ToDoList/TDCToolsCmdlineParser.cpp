// ToolsCmdlineParser.cpp: implementation of the CToolsCmdlineParser class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TDCToolsCmdlineParser.h"

#include "..\shared\datehelper.h"
#include "..\shared\misc.h"
#include "..\shared\filemisc.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMap<CString, LPCTSTR, CLA_TYPE, CLA_TYPE&> CTDCToolsCmdlineParser::s_mapTypes;

CTDCToolsCmdlineParser::CTDCToolsCmdlineParser(LPCTSTR szCmdLine)
{
	// init static map first time only
	if (s_mapTypes.GetCount() == 0)
	{
		s_mapTypes[_T("pathname")] = CLAT_PATHNAME;
		s_mapTypes[_T("filetitle")] = CLAT_FILETITLE;
		s_mapTypes[_T("folder")] = CLAT_FOLDER;
		s_mapTypes[_T("filename")] = CLAT_FILENAME;
		s_mapTypes[_T("seltid")] = CLAT_SELTASKID;
		s_mapTypes[_T("selttitle")] = CLAT_SELTASKTITLE;
		s_mapTypes[_T("userfile")] = CLAT_USERFILE;
		s_mapTypes[_T("userfolder")] = CLAT_USERFOLDER;
		s_mapTypes[_T("usertext")] = CLAT_USERTEXT;
		s_mapTypes[_T("userdate")] = CLAT_USERDATE;
		s_mapTypes[_T("todaysdate")] = CLAT_TODAYSDATE;
		s_mapTypes[_T("todolist")] = CLAT_TODOLIST;
		s_mapTypes[_T("seltextid")] = CLAT_SELTASKEXTID;
		s_mapTypes[_T("seltcomments")] = CLAT_SELTASKCOMMENTS;
		s_mapTypes[_T("seltfile")] = CLAT_SELTASKFILELINK;
		s_mapTypes[_T("seltallocby")] = CLAT_SELTASKALLOCBY;
		s_mapTypes[_T("seltallocto")] = CLAT_SELTASKALLOCTO;
		s_mapTypes[_T("seltcustom")] = CLAT_SELTASKCUSTATTRIB;
	}

	SetCmdLine(szCmdLine);
}

CTDCToolsCmdlineParser::~CTDCToolsCmdlineParser()
{

}

CString CTDCToolsCmdlineParser::GetPlaceHolder(CLA_TYPE nType)
{
	CString sPlaceHolder;

	switch (nType)
	{
		case CLAT_PATHNAME:				sPlaceHolder = _T("pathname");		break;
		case CLAT_FILETITLE:			sPlaceHolder = _T("filetitle");		break;
		case CLAT_FOLDER:				sPlaceHolder = _T("folder");		break;
		case CLAT_FILENAME:				sPlaceHolder = _T("filename");		break;
		case CLAT_SELTASKID:			sPlaceHolder = _T("seltid");		break;
		case CLAT_SELTASKTITLE:			sPlaceHolder = _T("selttitle");		break;
		case CLAT_USERFILE:				sPlaceHolder = _T("userfile");		break;
		case CLAT_USERFOLDER:			sPlaceHolder = _T("userfolder");	break;
		case CLAT_USERTEXT:				sPlaceHolder = _T("usertext");		break;
		case CLAT_USERDATE:				sPlaceHolder = _T("userdate");		break;
		case CLAT_TODAYSDATE:			sPlaceHolder = _T("todaysdate");	break;
		case CLAT_TODOLIST:				sPlaceHolder = _T("todolist");		break;
		case CLAT_SELTASKEXTID:			sPlaceHolder = _T("seltextid");		break;
		case CLAT_SELTASKCOMMENTS:		sPlaceHolder = _T("seltcomments");	break;
		case CLAT_SELTASKFILELINK:		sPlaceHolder = _T("seltfile");		break;
		case CLAT_SELTASKALLOCBY:		sPlaceHolder = _T("seltallocby");	break;
		case CLAT_SELTASKALLOCTO:		sPlaceHolder = _T("seltallocto");	break;
		case CLAT_SELTASKCUSTATTRIB:	sPlaceHolder = _T("seltcustom");	break;
	}
	ASSERT(!sPlaceHolder.IsEmpty());

	return sPlaceHolder;
}

void CTDCToolsCmdlineParser::SetCmdLine(LPCTSTR szCmdLine)
{
	m_sCmdLine = szCmdLine;
	m_aArgs.RemoveAll();

	ParseCmdLine();

	// replace 'todaysdate'
	ReplaceArgument(CLAT_TODAYSDATE, CDateHelper::FormatCurrentDate(TRUE));

	// and 'todolist'
	ReplaceArgument(CLAT_TODOLIST, FileMisc::GetAppFilePath());
}

int CTDCToolsCmdlineParser::GetArguments(CCLArgArray& aArgs) const
{
	aArgs.Copy(m_aArgs);
	return aArgs.GetSize();
}

int CTDCToolsCmdlineParser::GetUserArguments(CCLArgArray& aArgs) const
{
	int nArg = m_aArgs.GetSize();

	// add to start to maintain order
	while (nArg--)
	{
		switch (m_aArgs[nArg].nType)
		{
		case CLAT_USERFILE:
		case CLAT_USERFOLDER:
		case CLAT_USERTEXT:
		case CLAT_USERDATE:
			{
				CMDLINEARG cla = m_aArgs[nArg];
				aArgs.InsertAt(0, cla);
			}
			break;
		}
	}

	return aArgs.GetSize();
}

int CTDCToolsCmdlineParser::GetCustomAttributeArguments(CCLArgArray& aArgs) const
{
	int nArg = m_aArgs.GetSize();

	// add to start to maintain order
	while (nArg--)
	{
		switch (m_aArgs[nArg].nType)
		{
		case CLAT_SELTASKCUSTATTRIB:
			{
				CMDLINEARG cla = m_aArgs[nArg];
				cla.sName.MakeLower();

				aArgs.InsertAt(0, cla);
			}
			break;
		}
	}

	return aArgs.GetSize();
}

BOOL CTDCToolsCmdlineParser::ReplaceArgument(CLA_TYPE nType, LPCTSTR szValue)
{
	// see if we have that type
	int nArg = m_aArgs.GetSize();
	BOOL bFound = FALSE;

	// and replace all of them
	while (nArg--)
	{
		if (m_aArgs[nArg].nType == nType)
			bFound |= ReplaceArgument(nArg, szValue);
	}

	// not found
	return bFound;
}

BOOL CTDCToolsCmdlineParser::ReplaceArgument(CLA_TYPE nType, DWORD dwValue)
{
	CString sValue;
	sValue.Format(_T("%lu"), dwValue);

	return ReplaceArgument(nType, sValue);
}

BOOL CTDCToolsCmdlineParser::ReplaceArgument(LPCTSTR szName, LPCTSTR szValue)
{
	// see if we have a user item with that name
	CString sName(szName);
	sName.MakeLower();

	int nArg = m_aArgs.GetSize();

	while (nArg--)
	{
		switch (m_aArgs[nArg].nType)
		{
		case CLAT_USERFILE:
		case CLAT_USERFOLDER:
		case CLAT_USERTEXT:
		case CLAT_USERDATE:
			if (sName.CompareNoCase(m_aArgs[nArg].sName) == 0)
				return ReplaceArgument(nArg, szValue);
			break;

		case CLAT_SELTASKCUSTATTRIB:
			if (sName.CompareNoCase(m_aArgs[nArg].sPlaceHolder) == 0)
				return ReplaceArgument(nArg, szValue);
			break;
		}
	}

	// not found
	return FALSE;

}

BOOL CTDCToolsCmdlineParser::IsUserInputRequired() const
{
	// see if we have any 'USER' types
	int nArg = m_aArgs.GetSize();

	while (nArg--)
	{
		switch (m_aArgs[nArg].nType)
		{
		case CLAT_USERFILE:
		case CLAT_USERFOLDER:
		case CLAT_USERTEXT:
		case CLAT_USERDATE:
			return TRUE;
		}
	}

	// not found
	return FALSE;
}

BOOL CTDCToolsCmdlineParser::HasTasklistArgument() const
{
	// see if we have any tasklist related types
	int nArg = m_aArgs.GetSize();

	while (nArg--)
	{
		switch (m_aArgs[nArg].nType)
		{
		case CLAT_PATHNAME:
		case CLAT_FILETITLE:   
		case CLAT_FOLDER:        
		case CLAT_FILENAME:
			return TRUE;
		}
	}

	// not found
	return FALSE;
}

BOOL CTDCToolsCmdlineParser::IsUserInputType(CLA_TYPE nType) const
{
	switch (nType)
	{
	case CLAT_USERFILE:
	case CLAT_USERFOLDER:
	case CLAT_USERTEXT:
	case CLAT_USERDATE:
		return TRUE;
	}
	
	return FALSE;
}

BOOL CTDCToolsCmdlineParser::ReplaceArgument(int nArg, LPCTSTR szValue)
{
	if (nArg < 0 || nArg >= m_aArgs.GetSize())
		return FALSE;

	CMDLINEARG& cla = m_aArgs[nArg];

	CString sValue(szValue);
	Misc::Trim(sValue);

	if (m_sCmdLine.Replace(cla.sPlaceHolder, sValue))
	{
		// also check if there are any user references to this variable name
		// and replace those too
		if (IsUserInputType(cla.nType))
		{
			int nUserArg = m_aUserArgs.GetSize();

			while (nUserArg--)
			{
				CMDLINEARG& claUser = m_aUserArgs[nUserArg];

				if (claUser.sName.CompareNoCase(cla.sName) == 0)
				{
					m_sCmdLine.Replace(claUser.sPlaceHolder, sValue);

					// and remove
					m_aUserArgs.RemoveAt(nUserArg);
				}
			}
		}

		return TRUE;
	}

	// else
	return FALSE;
}

BOOL CTDCToolsCmdlineParser::HasArgument(CLA_TYPE nType) const
{
	int nArg = m_aArgs.GetSize();

	while (nArg--)
	{
		if (m_aArgs[nArg].nType == nType)
			return TRUE;
	}

	return FALSE;
}

void CTDCToolsCmdlineParser::ParseCmdLine()
{
	CString sCmdLine(m_sCmdLine); // preserve original

	int nDollar = sCmdLine.Find('$');

	while (-1 != nDollar)
	{
		// find opening bracket
		int nOpenFind = sCmdLine.Find('(', nDollar);

		if (nOpenFind == -1)
			break;

		// find closing bracket
		int nCloseFind = sCmdLine.Find(')', nOpenFind);

		if (nCloseFind == -1)
			break;

		// parse variable in the form of (vartype, varname, varlabel, vardefvalue)
		CString sVarArgs = sCmdLine.Mid(nOpenFind + 1, nCloseFind - nOpenFind - 1);
		CStringArray aVars;

		if (Misc::Split(sVarArgs, aVars, ',', TRUE))
		{
			CMDLINEARG cla;
	
			cla.nType = GetType(aVars[0]);
			cla.sPlaceHolder = sCmdLine.Mid(nDollar, nCloseFind - nDollar + 1);
			
			switch (cla.nType)
			{
			case CLAT_USERFILE:
			case CLAT_USERFOLDER:
			case CLAT_USERTEXT:
			case CLAT_USERDATE:
			case CLAT_SELTASKCUSTATTRIB: 
				{
					CString sName, sLabel, sDefValue;

					switch (aVars.GetSize())
					{
						// cascade from item to item
						case 4:	sDefValue	= aVars[3];	Misc::Trim(sDefValue);
						case 3:	sLabel		= aVars[2];	Misc::Trim(sLabel);
						case 2:	sName		= aVars[1];	Misc::Trim(sName);
					}

					// user input types must have a valid name that is not a 'keyword'
					if (sName.IsEmpty() || (GetType(sName) != CLAT_NONE))
					{
						ASSERT(0);
					}
					else
					{
						cla.sName = Misc::ToLower(sName);
						cla.sLabel = sLabel;
						cla.sDefValue = sDefValue;

						m_aArgs.Add(cla);
					}
				}
				break;

			case CLAT_TODOLIST:      
			case CLAT_PATHNAME:      
			case CLAT_FILETITLE:    
			case CLAT_FOLDER:        
			case CLAT_FILENAME:      
			case CLAT_TODAYSDATE:
			case CLAT_SELTASKID:
			case CLAT_SELTASKTITLE:
			case CLAT_SELTASKEXTID:
			case CLAT_SELTASKCOMMENTS:
			case CLAT_SELTASKFILELINK:
			case CLAT_SELTASKALLOCBY:
			case CLAT_SELTASKALLOCTO:      
				m_aArgs.Add(cla);
				break;

			default: // assume it is USER attribute
				ASSERT (cla.nType == CLAT_NONE); // what else?
	
				cla.sName = Misc::ToLower(aVars[0]);
				m_aUserArgs.Add(cla);
				break;
			}
		}

		// next arg
		nDollar = sCmdLine.Find('$', nCloseFind);
	}
}

CLA_TYPE CTDCToolsCmdlineParser::GetType(LPCTSTR szVarType) const
{
	CString sType(szVarType);

	Misc::Trim(sType);
	sType.MakeLower();

	CLA_TYPE nType = CLAT_NONE;
	s_mapTypes.Lookup(sType, nType);

	return nType;
}

BOOL CTDCToolsCmdlineParser::PrepareToolPath(CString& sToolPath)
{
	return sToolPath.Replace(_T("$(todolist)"), FileMisc::GetAppFilePath());
}
