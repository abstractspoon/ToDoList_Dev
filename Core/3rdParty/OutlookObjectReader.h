#if !defined(AFX_OUTLOOKMSGREADER_H__56B41CA4_32AC_485E_8436_49958724F2C9__INCLUDED_)
#define AFX_OUTLOOKMSGREADER_H__56B41CA4_32AC_485E_8436_49958724F2C9__INCLUDED_
//-----------------------------------------------------------------------------------
// SSFileDLL A open dll to extract information from outlook msg file without outlook
//
//Copyright (C) 2007 Calvin Streeting
//
//This program is free software; you can redistribute it and/or
//modify it under the terms of the GNU General Public License
//as published by the Free Software Foundation; either version 2
//of the License, or (at your option) any later version.
//
//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.
//
//You should have received a copy of the GNU General Public License
//along with this program; if not, write to the Free Software
//Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
//-----------------------------------------------------------------------------------
// SSFileDLL.h
//-----------------------------------------------------------------------------------

#include "SSFile.h"
#include <afxtempl.h>

class COutlookObjectReader;

typedef CArray<COutlookObjectReader, COutlookObjectReader&> COutlookObjectReaderArray;
typedef CMap<ULONG, ULONG, CString, CString&> COutlookObjectDataMap;

class COutlookObjectReader : public CSSFile
{
public:
	
	COutlookObjectReader();
	virtual ~COutlookObjectReader();
	
	bool OpenObject(LPCTSTR szFilePath);
	void CloseObject();
	
	bool AttachObject(COleDataObject* pDataObj);
	int AttachObject(COleDataObject* pDataObj, COutlookObjectReaderArray& aObjects);
	void DetachObject();
	
	CString GetObjectClass();

	CString GetObjectData(ULONG ulPropTag);
	BOOL GetObjectData(ULONG ulPropTag, CString& sValue);
	BOOL GetObjectData(ULONG ulPropTag, LONG& lValue);
	BOOL GetObjectData(ULONG ulPropTag, BYTE*& pValue, int& nLen);
	int GetObjectData(COutlookObjectDataMap& mapData);
	
	CString Report();
	CString Report(ULONG ulPropTag);
	
	static bool IsValidObject(LPCTSTR szName);
	static bool IsValidObject(COleDataObject* pDataObj);
	static int ExtractObjects(CStringArray& aFileNames, COutlookObjectReaderArray& aObjects);
	
protected:
	bool m_bOpen;
	STGMEDIUM m_storage;
	DWORD m_dwStorageType;
	
	bool AttachObject(COleDataObject* pDataObj, COutlookObjectReaderArray* pMsgs, COutlookObjectReader* pMsg);

	static CString FormatDataName(WORD wID, WORD wType);
	static CString FormatDataName(ULONG ulPropTag);
	static CString Report(ULONG ulPropTag, const CString& sData);
	
};

#endif // !defined(AFX_OUTLOOKMSGREADER_H__56B41CA4_32AC_485E_8436_49958724F2C9__INCLUDED_)
