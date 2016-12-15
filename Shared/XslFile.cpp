// XslFile.cpp: implementation of the CXslFile class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XslFile.h"

#include "misc.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CXslFile::CXslFile()
{

}

CXslFile::~CXslFile()
{

}

BOOL CXslFile::Load(const CString& sFilePath)
{
	return CXmlFile::Load(sFilePath);
}

CString CXslFile::GetOutputMediaType() const
{
	const CXmlItem* pXIOutput = GetOutputItem();

	if (pXIOutput)
		return pXIOutput->GetItemValue(_T("media-type"));
		
	// else
	return EMPTY_STR;
}

CString CXslFile::GetOutputFileExtension() const
{
	CString sMediaType = GetOutputMediaType(), sExt;
	
	if (Misc::Split(sMediaType, sExt, '/') && !sExt.IsEmpty())
		return sExt;

	// else
	return _T("html");
}

SFE_FORMAT CXslFile::GetOutputFileEncoding() const
{
	const CXmlItem* pXIOutput = GetOutputItem();

	if (pXIOutput)
	{
		if (pXIOutput->GetItemValue(_T("encoding")).CompareNoCase(_T("utf-16")) == 0)
			return SFEF_UTF16;
	}
	
	// all else
	return SFEF_UTF8WITHOUTBOM;
}

const CXmlItem* CXslFile::GetOutputItem() const
{
	return GetItem(_T("output"));
}
