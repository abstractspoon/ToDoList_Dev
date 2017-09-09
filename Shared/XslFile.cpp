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

BOOL CXslFile::Save(const CString& sFilePath)
{
	return CXmlFile::Save(sFilePath, SFEF_AUTODETECT);
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

int CXslFile::GetGlobalParams(CXslParamArray& aParams) const
{
	aParams.RemoveAll();

	const CXmlItem* pXIParam = GetItem(_T("param"));
	XSLPARAM param;

	while (pXIParam)
	{
		param.sName = pXIParam->GetItemValue(_T("name"));
		param.sValue = pXIParam->GetItemValue(_T("select"));
		param.sTitle = pXIParam->GetItemValue(_T("title"));

		if (param.sTitle.IsEmpty())
			param.sTitle = pXIParam->GetValue();

		if (param.sTitle.IsEmpty())
			param.sTitle = param.sName;

		aParams.Add(param);

		pXIParam = pXIParam->GetSibling();
	}
	
	return aParams.GetSize();
}

BOOL CXslFile::SetGlobalParams(const CXslParamArray& aParams)
{
	// TODO 
	return FALSE;
}
