// XslFile.h: interface for the CXslFile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XSLFILE_H__ADEA8930_FD90_4C38_9275_ECF0D87AFBCD__INCLUDED_)
#define AFX_XSLFILE_H__ADEA8930_FD90_4C38_9275_ECF0D87AFBCD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XmlFile.h"

//////////////////////////////////////////////////////////////////////

struct XSLPARAM
{
	CString sName;
	CString sTitle;
	CString sValue;
};
typedef CArray<XSLPARAM, XSLPARAM&> CXslParamArray;

//////////////////////////////////////////////////////////////////////

class CXslFile : protected CXmlFile  
{
public:
	CXslFile();
	virtual ~CXslFile();

	BOOL Load(const CString& sFilePath);
	BOOL Save(const CString& sFilePath);

	CString GetOutputMediaType() const;
	CString GetOutputFileExtension() const;
	SFE_FORMAT GetOutputFileEncoding() const;

	int GetGlobalParams(CXslParamArray& aParams) const;
	BOOL SetGlobalParams(const CXslParamArray& aParams);

protected:
	const CXmlItem* GetOutputItem() const;
};

/////////////////////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_XSLFILE_H__ADEA8930_FD90_4C38_9275_ECF0D87AFBCD__INCLUDED_)
