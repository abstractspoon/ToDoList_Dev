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
	if (!CXmlFile::Load(sFilePath))
		return FALSE;

	m_sNamespace = GetItemValue(_T("xmlns:xsl"));

	return TRUE;
}

BOOL CXslFile::Save(const CString& sFilePath, SFE_FORMAT nFormat)
{
	SetXmlHeader(m_sXmlHeader);

	return CXmlFile::Save(sFilePath, nFormat);
}

void CXslFile::CopyFrom(const CXslFile& file)
{
	CXmlFile::CopyFrom(file);
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
	DeleteItem(_T("param"));

	int nNumParam = aParams.GetSize();

	if (nNumParam)
	{
		for (int nParam = 0; nParam < nNumParam; nParam++)
		{
			const XSLPARAM& param = aParams[nParam];
			ASSERT(!param.sName.IsEmpty());
			
			CXmlItem* pXIParam = AddItem(_T("param"));
			ASSERT(pXIParam);

			pXIParam->SetItemValue(_T("name"), param.sName);
			pXIParam->SetItemValue(_T("select"), param.sValue);
			pXIParam->GetItemValue(_T("title"), param.sTitle);
		}
	}

	return TRUE;
}

BOOL CXslFile::UpdateGlobalParams(const CXslParamArray& aParams)
{
	int nNumParam = aParams.GetSize();

	if (!nNumParam)
		return FALSE;

	CXslParamArray aCurParams;

	int nNumCur = GetGlobalParams(aCurParams);

	if (!nNumCur)
		return SetGlobalParams(aParams);

	for (int nParam = 0; nParam < nNumParam; nParam++)
	{
		const XSLPARAM& param = aParams[nParam];
		ASSERT(!param.sName.IsEmpty());

		// Find existing parameter of the same name
		int nCur = nNumCur;

		while (nCur--)
		{
			if (aCurParams[nCur].sName == param.sName)
				break;
		}

		// Add or update
		if (nCur == -1)
		{
			aCurParams.Add(XSLPARAM(param));
		}
		else
		{
			aCurParams[nCur].sValue = param.sValue;
		}
	}

	return SetGlobalParams(aCurParams);
}

MSXML2::IXMLDOMNodePtr CXslFile::InsertNode(CXmlNodeWrapper* pNode, int nNode, const CXmlItem* pXItem) const
{
	ASSERT(!m_sNamespace.IsEmpty());

	if (!m_sNamespace.IsEmpty() && pXItem->IsElement())
	{
		// Hack (for now)
		BOOL bLineBreak = ((pXItem->GetName().CollateNoCase(_T("br")) == 0) && !pXItem->HasValue());

		if (!bLineBreak)
			return pNode->InsertNode(nNode, (_T("xsl:") + pXItem->GetName()), m_sNamespace);
	}

	// all else
	return CXmlFile::InsertNode(pNode, nNode, pXItem);
}
