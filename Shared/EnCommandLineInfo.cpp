// EnCommandLineInfo.cpp: implementation of the CEnCommandLineInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EnCommandLineInfo.h"

#include "misc.h"
#include "filemisc.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEnCommandLineInfo::CEnCommandLineInfo(const CString& sFileExts)
{
	m_nLastParameter = -1;	
	
	if (!sFileExts.IsEmpty())
			Misc::Split(sFileExts, m_aFileExt, ';');
}

CEnCommandLineInfo::~CEnCommandLineInfo()
{

}

void CEnCommandLineInfo::ParseParam(LPCTSTR lpszParam, BOOL bFlag, BOOL /*bLast*/)
{
	CString sLookup, sParam(lpszParam);

	// Handling negative arguments to flags is a bit tricky
	// because our base class will strip off the leading
	// dash and tell us that it is in fact a flag itself.
	// So if we already have a 'current' flag AND the value
	// is a number AND our base class is telling us that it
	// is a flag then we treat it as a negative number
	if (!m_sCurFlag.IsEmpty() && bFlag && Misc::IsNumber(sParam)) 
	{
		// sanity check
		ASSERT(sParam[0] != '-');

		bFlag = FALSE;
		sParam.Insert(0, '-');
	}

	// then normal processing
	if (bFlag)
	{
		m_sCurFlag = lpszParam; 	   // save in case other value specified
		m_sCurFlag.MakeUpper();

		// this is a "flag" (begins with / or -)
		m_mapCommandLine[m_sCurFlag] = lpszParam; // default value is "TRUE"
		m_nLastParameter = -1;		

		return;
	} 

	// else must be an argument to a flag or possibly a matching filename 
	if (!m_sCurFlag.IsEmpty())
	{
		m_nLastParameter++;

		sLookup.Format(_T("%s_PARAMETER_%d"), m_sCurFlag, m_nLastParameter);
		m_mapCommandLine[sLookup] = sParam;
	}

	// set m_strFilename to the first parameter having a (matching) file extension 
	if (m_strFileName.IsEmpty())
	{
		CString sExt = FileMisc::GetExtension(sParam);

		if (!sExt.IsEmpty()) // found something
		{
			if (!m_aFileExt.GetSize() || (Misc::Find(m_aFileExt, sExt, FALSE, FALSE) >= 0))
			{
				m_strFileName = sParam;
			}
		}
	}
}

BOOL CEnCommandLineInfo::GetOptions(LPCTSTR szFlag, CStringArray& aParams) const
{
	CString sFlag(szFlag), sLookup, sParameter;
	sFlag.MakeUpper();

	if (!m_mapCommandLine.Lookup(sFlag, sParameter))
		return FALSE;

	aParams.RemoveAll();

	int nParam = 0;
	sLookup.Format(_T("%s_PARAMETER_0"), sFlag);

	while (m_mapCommandLine.Lookup(sLookup, sParameter))
	{
		aParams.Add(sParameter);

		nParam++;
		sLookup.Format(_T("%s_PARAMETER_%d"), sFlag, nParam);
	}

	return TRUE;
}

void CEnCommandLineInfo::SetOption(LPCTSTR szFlag)
{
	m_mapCommandLine[Misc::ToUpper(szFlag)] = szFlag;
}

BOOL CEnCommandLineInfo::SetOption(LPCTSTR szFlag, LPCTSTR szParam, BOOL bFailIfExists)
{
	CString sFlag(Misc::ToUpper(szFlag));

	// option cannot already exist
	if (bFailIfExists)
	{
		CString sUnused;

		if (m_mapCommandLine.Lookup(sFlag, sUnused))
			return FALSE;
	}

	// create flag
	m_mapCommandLine[sFlag] = szFlag;

	// set szParam as the one and only option parameter
	CString sParam;
	sParam.Format(_T("%s_PARAMETER_0"), sFlag);

	m_mapCommandLine[sParam] = szParam;

	return TRUE;
}

BOOL CEnCommandLineInfo::SetOption(LPCTSTR szFlag, DWORD dwParam, BOOL bFailIfExists)
{
	return SetOption(szFlag, Misc::Format(dwParam), bFailIfExists);
}

BOOL CEnCommandLineInfo::GetOption(LPCTSTR szFlag, CString& sParam) const
{
	sParam.Empty();
	CStringArray aParams;

	if (GetOptions(szFlag, aParams))
	{
		if (aParams.GetSize())
			sParam = aParams[0];

		return TRUE;
	}

	return FALSE;
}

CString CEnCommandLineInfo::GetOption(LPCTSTR szFlag) const
{
	CString sOption;
	GetOption(szFlag, sOption);

	return sOption;
}

BOOL CEnCommandLineInfo::HasOption(LPCTSTR szFlag) const
{
	CString sOption;
	
	return GetOption(szFlag, sOption);
}

void CEnCommandLineInfo::DeleteOption(LPCTSTR szFlag)
{
	CString sFlag(szFlag);
	sFlag.MakeUpper();

	m_mapCommandLine.RemoveKey(sFlag);
}

CString CEnCommandLineInfo::GetCommandLine(TCHAR cDelim) const
{
	CString sCmdLine(FormatValue(m_strFileName));
	POSITION pos = m_mapCommandLine.GetStartPosition();

	while (pos)
	{
		CString sKey, sFlag;
		m_mapCommandLine.GetNextAssoc(pos, sKey, sFlag);

		if (sKey.CompareNoCase(sFlag) == 0) // actual switch value
		{
			CString sParamKey, sParamVal;

			sParamKey.Format(_T("%s_PARAMETER_0"), sKey);

			if (m_mapCommandLine.Lookup(sParamKey, sParamVal))
			{
				// first param
				sCmdLine += FormatSwitch(sFlag, sParamVal, cDelim);

				// rest of params
				int nParam = 1;

				while (true)
				{
					sParamKey.Format(_T("%s_PARAMETER_%d"), sKey, nParam);

					if (!m_mapCommandLine.Lookup(sParamKey, sParamVal))
						break;

					// else
					sCmdLine += FormatValue(sParamVal);
				}
			}
			else // param-less
			{
				sCmdLine += FormatSwitch(sFlag, NULL, cDelim);
			}
		}
	}

	Misc::Trim(sCmdLine);

	return sCmdLine;
}

CString CEnCommandLineInfo::FormatSwitch(LPCTSTR szSwitch, LPCTSTR szValue, TCHAR cDelim)
{
	CString sSwitch;

	// note the leading space
	sSwitch.Format(_T(" %c%s%s"), cDelim, szSwitch, FormatValue(szValue));

	return sSwitch;
}

CString CEnCommandLineInfo::FormatSwitch(TCHAR cSwitch, LPCTSTR szValue, TCHAR cDelim)
{
	TCHAR szSwitch[2] = { cSwitch, 0 };
	
	return FormatSwitch(szSwitch, szValue, cDelim);
}

CString CEnCommandLineInfo::FormatValue(LPCTSTR szValue)
{
	CString sValue(szValue);

	if (!sValue.IsEmpty())
	{
		// note the leading space
		// quote values with spaces
		if (sValue.Find(' ') != -1)
			sValue.Format(_T(" \"%s\""), szValue);
		else
			sValue = (' ' + sValue);
	}

	return sValue;
}
