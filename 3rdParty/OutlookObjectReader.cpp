
//-----------------------------------------------------------------------------------
// SSFileDLL A open dll to extract information from outlook obj file without outlook
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
// SSFileDLL.cpp
//-----------------------------------------------------------------------------------

#include "stdafx.h"
#include "outlookobjectreader.h"

#include <mapidefs.h>
#include <mapitags.h>

static const WORD WTYPES[] = 
{ 
	PT_I2,          /* Signed 16-bit value */
	PT_LONG,        /* Signed 32-bit value */
	PT_R4,          /* 4-byte floating point */
	PT_DOUBLE,      /* Floating point double */
	PT_CURRENCY,    /* Signed 64-bit int (decimal w/    4 digits right of decimal pt) */
	PT_APPTIME,     /* Application time */
	PT_ERROR,       /* 32-bit error value */
	PT_BOOLEAN,     /* 16-bit boolean (non-zero true) */
	PT_OBJECT,      /* Embedded object in a property */
	PT_I8,          /* 8-byte signed integer */
	PT_STRING8,     /* Null terminated 8-bit character string */
	PT_UNICODE,     /* Null terminated Unicode string */
	PT_SYSTIME,     /* FILETIME 64-bit int w/ number of 100ns periods since Jan 1,1601 */
	PT_CLSID,       /* OLE GUID */
	PT_BINARY,      /* Uninterpreted (counted byte array) */
};

const UINT NUMTYPES = sizeof(WTYPES) / sizeof(WORD);

static const DWORD32 DWRANGES[] = 
{
	MAKELONG(0x0001, 0x007F),
	MAKELONG(0x1000, 0x1012),
	MAKELONG(0x0C00, 0x0C1F),
	MAKELONG(0x0E00, 0x0E26),
	MAKELONG(0x0FF4, 0x0FFF),
	MAKELONG(0x3000, 0x300D),
	MAKELONG(0x3301, 0x330A),
	MAKELONG(0x3400, 0x3415),
	MAKELONG(0x35DF, 0x35E7),
	MAKELONG(0x3600, 0x3617),
	MAKELONG(0x3700, 0x370F),
	MAKELONG(0x3900, 0x3904),
	MAKELONG(0x39FF, 0x3A64),
	MAKELONG(0x3D00, 0x3D12),
	MAKELONG(0x3E00, 0x3E0D),
	MAKELONG(0x3F00, 0x3F08),
};

const UINT NUMRANGES = sizeof(DWRANGES) / sizeof(DWORD32);

// Register the private clipboard formats of Outlook

#define CFSTR_RENPRIVATEMESSAGES  _T("RenPrivateMessages") 
#define CFSTR_RENPRIVATESRCFOLDER _T("RenPrivateSourceFolder")

static CLIPFORMAT CF_RENPRIVATEMESSAGES = (CLIPFORMAT)::RegisterClipboardFormat(CFSTR_RENPRIVATEMESSAGES);
static CLIPFORMAT CF_RENPRIVATESRCFOLDER = (CLIPFORMAT)::RegisterClipboardFormat(CFSTR_RENPRIVATESRCFOLDER);	
static CLIPFORMAT CF_FILECONTENTS = (CLIPFORMAT)::RegisterClipboardFormat(CFSTR_FILECONTENTS);
static CLIPFORMAT CF_FILEDESCRIPTOR = (CLIPFORMAT)::RegisterClipboardFormat(CFSTR_FILEDESCRIPTORW);	

COutlookObjectReader::COutlookObjectReader() : CSSFile(), m_bOpen(false)//, m_bAutoDetach(false)
{
	ZeroMemory(&m_storage, sizeof(m_storage));
}

COutlookObjectReader::~COutlookObjectReader() 
{
	DetachObject();
	CloseObject();
}

bool COutlookObjectReader::OpenObject(LPCTSTR szFilePath)
{
	//m_bAutoDetach = false;
	DetachObject();
	CloseObject();
	
	if (m_bOpen == false)
	{
		if (OpenSSFile(szFilePath) == true)
		{	
			m_bOpen  = true;
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}

bool COutlookObjectReader::IsValidObject(LPCTSTR szFilePath) 
{
	CString sFile(szFilePath);
	sFile.MakeLower();
	
	return (sFile.Find(_T(".msg")) == (sFile.GetLength() - 4));
}

bool COutlookObjectReader::IsValidObject(COleDataObject* pDataObj) 
{
	return (pDataObj &&
			pDataObj->IsDataAvailable(CF_RENPRIVATEMESSAGES) && 
			pDataObj->IsDataAvailable(CF_RENPRIVATESRCFOLDER) &&
			pDataObj->IsDataAvailable(CF_FILECONTENTS) && 
			pDataObj->IsDataAvailable(CF_FILEDESCRIPTOR));
}

bool COutlookObjectReader::AttachObject(COleDataObject* pDataObj)
{
	COutlookObjectReaderArray aObjects;
	
	// extract all messages and use the details of the first one
	if (AttachObject(pDataObj, aObjects))
	{
		// swap storage from first msg to 'this'
		COutlookObjectReader& obj = aObjects[0];
		IStorage* pMsgStore = obj.m_pRootStg;
		obj.m_pRootStg = NULL; // so it doesn't get freed
		
		Attach(pMsgStore);
		return true;
	}
	
	return false;
}

int COutlookObjectReader::AttachObject(COleDataObject* pDataObj, COutlookObjectReaderArray& aObjects)
{
	if (!IsValidObject(pDataObj))
		return false;
	
	DetachObject();
	CloseObject();
	
	// We need to use Non unicode version of FILEGROUPDESCRIPTOR and FILEDESCRIPTOR even in unicode build
	FILEGROUPDESCRIPTORW *file_group_descriptor;	
	FILEDESCRIPTORW      file_descriptor;
	
	// Important: these strings need to be non-Unicode (don't compile UNICODE)
	
	// Set up format structure for the descriptor and contents
	FORMATETC descriptor_format = {CF_FILEDESCRIPTOR, NULL, DVASPECT_CONTENT, -1, TYMED_HGLOBAL};
	FORMATETC contents_format = {CF_FILECONTENTS,   NULL, DVASPECT_CONTENT, -1, (DWORD)-1 };//TYMED_ISTREAM}; //TYMED_ISTREAM means use stream as a tempoary meory instead of RAM's global memory
	
	// Get the descriptor information
	if (!pDataObj->GetData(CF_FILEDESCRIPTOR,&m_storage, &descriptor_format ))
		return false;
	
	file_group_descriptor = (FILEGROUPDESCRIPTORW *) GlobalLock(m_storage.hGlobal);
	
	// For each file, get the name and copy the stream to a file
	CString file_list;
	unsigned int nFileCount = file_group_descriptor->cItems;
	
	for (unsigned int file_index = 0; file_index < nFileCount; file_index++) 
	{
		file_descriptor = file_group_descriptor->fgd[file_index];
		contents_format.lindex = file_index;
		
		if (pDataObj->GetData(CF_FILECONTENTS, &m_storage, &contents_format ))
		{
			if (m_storage.tymed == TYMED_ISTREAM)
			{ 
				// Dump stream to a file
				CString sTempFile = _T("C:\\Temp\\temp.msg");
				
				if (CSSFile::StreamToFile(m_storage.pstm, sTempFile) == S_OK)
				{
					int nIndex = aObjects.Add(COutlookObjectReader());
					
					if (nIndex >= 0)
						aObjects[nIndex].OpenObject(sTempFile);
					
					// cleanup
					::DeleteFile(sTempFile);
				}
			}
			else if (m_storage.tymed == TYMED_ISTORAGE)
			{
				int nIndex = aObjects.Add(COutlookObjectReader());
				
				if (nIndex >= 0)
					aObjects[nIndex].Attach(m_storage.pstg);
			}
		}
	}

	return aObjects.GetSize();
}

int COutlookObjectReader::ExtractObjects(CStringArray& aFileNames, COutlookObjectReaderArray& aObjects)
{
	int nFile = aFileNames.GetSize();
	aObjects.RemoveAll();

	while (nFile--) // so we can delete as we go
	{
		const CString& sFile = aFileNames[nFile];

		if (IsValidObject(sFile))
		{
			int nIndex = aObjects.Add(COutlookObjectReader());
						
			if (nIndex >= 0)
			{
				aObjects[nIndex].OpenObject(sFile);
				aFileNames.RemoveAt(nFile);
			}
		}
	}

	return aObjects.GetSize();
}

void COutlookObjectReader::DetachObject()
{
	if (m_storage.hGlobal)
	{
		if (m_storage.tymed == TYMED_ISTORAGE)
			Detach();
		
		GlobalUnlock(m_storage.hGlobal);
		GlobalFree(m_storage.hGlobal);
		
		ZeroMemory(&m_storage, sizeof(m_storage));
	}
}

void COutlookObjectReader::CloseObject()
{
	if (m_bOpen  == true)
	{
		Close();
		m_bOpen = false;
	}
}

CString COutlookObjectReader::Report()
{
	COutlookObjectDataMap mapData;
	CString sReport;

	if (GetObjectData(mapData))
	{
		POSITION pos = mapData.GetStartPosition();

		while (pos)
		{
			ULONG ulPropTag;
			CString sData;

			mapData.GetNextAssoc(pos, ulPropTag, sData);
			sReport += Report(ulPropTag, sData);
		}
	}

	return sReport;
}

CString COutlookObjectReader::Report(ULONG ulPropTag)
{
	CString sData = GetObjectData(ulPropTag);
	return Report(ulPropTag, sData);
}

CString COutlookObjectReader::Report(ULONG ulPropTag, const CString& sData)
{
	if (sData.IsEmpty())
		return _T("");

	CString sReport, sName = FormatDataName(ulPropTag);

	switch (LOWORD(ulPropTag))
	{
	case PT_I2: 
		sReport.Format(_T("%s = PT_I2: %s\n"), sName, sData);
		break;
		
	case PT_LONG:
		sReport.Format(_T("%s = PT_LONG: %ld\n"), sName, sData);
		break;
		
	case PT_R4:
		sReport.Format(_T("%s = PT_R4: %f\n"), sName, sData);
		break;
		
	case PT_DOUBLE:
		sReport.Format(_T("%s = PT_DOUBLE: %lf\n"), sName, sData);
		break;
		
	case PT_CURRENCY:
		sReport.Format(_T("%s = PT_CURRENCY: %s\n"), sName, sData);
		break;
		
	case PT_APPTIME: 
		sReport.Format(_T("%s = PT_APPTIME: %s\n"), sName, sData);
		break;
		
	case PT_ERROR:
		sReport.Format(_T("%s = PT_ERROR: %ld\n"), sName, sData);
		break;
		
	case PT_BOOLEAN:
		sReport.Format(_T("%s = PT_BOOLEAN: %d\n"), sName, sData);
		break;
		
	case PT_OBJECT: 
		sReport.Format(_T("%s = PT_OBJECT: %s\n"), sName, sData);
		break;
		
	case PT_I8: 
		sReport.Format(_T("%s = PT_I8: %d\n"), sName, sData);
		break;
		
	case PT_STRING8:
		sReport.Format(_T("%s = PT_STRING8: %s\n"), sName, sData);
		break;
		
	case PT_UNICODE:
		sReport.Format(_T("%s = PT_UNICODE: %s\n"), sName, sData);
		break;
		
	case PT_SYSTIME:
		sReport.Format(_T("%s = PT_SYSTIME: %ld\n"), sName, sData);
		break;
		
	case PT_CLSID:  
		sReport.Format(_T("%s = PT_CLSID: %s\n"), sName, sData);
		break;
		
	case PT_BINARY: 
		sReport.Format(_T("%s = PT_BINARY: %s\n"), sName, sData);
		break;
	}
	
	return sReport;
}

int COutlookObjectReader::GetObjectData(COutlookObjectDataMap& mapData)
{
	mapData.RemoveAll();

	for (UINT nType = 0; nType < NUMTYPES; nType++)
	{
		for (int nRange = 0; nRange < NUMRANGES; nRange++)
		{
			DWORD dwRange = DWRANGES[nRange];
			WORD wStart = LOWORD(dwRange), wEnd = HIWORD(dwRange);

			for (WORD nAttrib = wStart; nAttrib <= wEnd; nAttrib++)
			{
				ULONG ulPropTag = MAKELPARAM(WTYPES[nType], nAttrib);
				CString sAttrib = GetObjectData(ulPropTag);

				if (!sAttrib.IsEmpty())
					mapData[ulPropTag] = sAttrib;
			}
		}
	}

	return mapData.GetCount();
}

CString COutlookObjectReader::GetObjectData(ULONG ulPropTag)
{
	CString sData;
	BYTE* pValue = NULL;
	int nLen = 0;

	if (GetObjectData(ulPropTag, pValue, nLen))
	{
		switch (LOWORD(ulPropTag))
		{
		case PT_I2:          /* Signed 16-bit value */
			sData.Format(_T("%d"), (WORD&)(*pValue));
			break;

		case PT_LONG:        /* Signed 32-bit value */
			sData.Format(_T("%ld"), (LONG&)(*pValue));
			break;

		case PT_R4:          /* 4-byte floating point */
			sData.Format(_T("%f"), (float&)(*pValue));
			break;

		case PT_DOUBLE:      /* Floating point double */
			sData.Format(_T("%lf"), (double&)(*pValue));
			break;

		case PT_CURRENCY:    /* Signed 64-bit int (decimal w/    4 digits right of decimal pt) */
			//sData.Format(_T("%s"), _T("Currency")/*(WORD&)(*pValue)*/);
			break;

		case PT_APPTIME:     /* Application time */
			//sData.Format(_T("%s"), _T("AppTime")/*(WORD&)(*pValue)*/);
			break;

		case PT_ERROR:       /* 32-bit error value */
			sData.Format(_T("%ld"), (DWORD&)(*pValue));
			break;

		case PT_BOOLEAN:     /* 16-bit boolean (non-zero true) */
			sData.Format(_T("%d"), (WORD&)(*pValue));
			break;

		case PT_OBJECT:      /* Embedded object in a property */
			//sData.Format(_T("%s"), _T("Embedded Object")/*(WORD&)(*pValue)*/);
			break;

		case PT_I8:          /* 8-byte signed integer */
			sData.Format(_T("%d"), (short&)(*pValue));
			break;

		case PT_STRING8:     /* Null terminated 8-bit character string */
			sData.Format(_T("%s"), CString((LPCSTR)pValue));
			break;

		case PT_UNICODE:     /* Null terminated Unicode string */
			sData.Format(_T("%s"), CString((LPCWSTR)pValue));
			break;

		case PT_SYSTIME:     /* FILETIME 64-bit int w/ number of 100ns periods since Jan 1:1601 */
			sData.Format(_T("%ld"), (FILETIME&)(*pValue));
			break;

		case PT_CLSID:       /* OLE GUID */
			//sData.Format(_T("%s"), _T("Guid")/*(WORD&)(*pValue)*/);
			break;

		case PT_BINARY:      /* Uninterpreted (counted byte array) */
			//sData.Format(_T("%s"), CString((LPCSTR)pValue));
			//sData.Format(_T("%s"), _T("Binary")/*(WORD&)(*pValue)*/);
			break;
		}

		// cleanup
		delete [] pValue;
	}

	return sData;
}

BOOL COutlookObjectReader::GetObjectData(ULONG ulPropTag, BYTE*& pValue, int& nLen)
{
	pValue = NULL;
	nLen = 0;

	COleStreamFile osf;	
	CString sName = FormatDataName(ulPropTag);
	
	if (OpenStream(sName, osf))
	{
		nLen = osf.GetLength();
		pValue = new BYTE[nLen+2];

		osf.Read(pValue, nLen);
		pValue[nLen] = 0;
		pValue[nLen+1] = 0;
		
		osf.Close();
	}
	
	return (nLen > 0);
}

CString COutlookObjectReader::FormatDataName(WORD wID, WORD wType)
{
	CString sName;
	sName.Format(_T("__substg1.0_%04X%04X"), wID, wType);

	return sName;
}

BOOL COutlookObjectReader::GetObjectData(ULONG ulPropTag, CString& sValue)
{
	sValue.Empty();

	BYTE* pValue = NULL;
	int nLen = 0;

	if (GetObjectData(ulPropTag, pValue, nLen))
	{
		sValue = (TCHAR*)pValue;

		// cleanup
		delete [] pValue;
	}

	return !sValue.IsEmpty();
}

BOOL COutlookObjectReader::GetObjectData(ULONG ulPropTag, LONG& lValue)
{
	lValue = 0;

	BYTE* pValue = NULL;
	int nLen = 0;

	if (GetObjectData(ulPropTag, pValue, nLen))
	{
		lValue = (LONG&)*pValue;

		// cleanup
		delete [] pValue;
		return TRUE;
	}

	return FALSE;
}

CString COutlookObjectReader::FormatDataName(ULONG ulPropTag)
{
	return FormatDataName((WORD)HIWORD(ulPropTag), (WORD)LOWORD(ulPropTag));
}

CString COutlookObjectReader::GetObjectClass()
{
	return GetObjectData(PR_MESSAGE_CLASS);
}

