// MSWordHelper.cpp: implementation of the CMSWordHelper class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MSWordHelper.h"
#include "filemisc.h"
#include "fileregister.h"
#include "regkey.h"
#include "misc.h"

#include "..\3rdparty\msword.h"

#include <shlwapi.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////

using namespace WordAPI;

//////////////////////////////////////////////////////////////////////

WordAPI::_Application* CMSWordHelper::s_pWord = NULL;
int CMSWordHelper::s_nRefCount = 0;

//////////////////////////////////////////////////////////////////////

// some handy const variants
static COleVariant varNull(_T("")), varZero((short)0), varOne((short)1);
static COleVariant varFalse((short)FALSE), varTrue((short)TRUE);

static LPCTSTR DISABLED_REGKEY(_T("Software\\Microsoft\\Office\\%d.0\\Word\\Resiliency\\DisabledItems"));

//////////////////////////////////////////////////////////////////////

#define RPC_NOT_AVAIL 0x800706BA

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMSWordHelper::CMSWordHelper(int nMinVersion)
{
	if (s_pWord == NULL && IsWordInstalled(nMinVersion))
	{
		ASSERT(s_nRefCount == 0);
		RestartWord();
	}

	// ref count even on failure
	s_nRefCount++;
}

CMSWordHelper::~CMSWordHelper()
{
	s_nRefCount--;

	if (s_nRefCount == 0 && s_pWord)
	{
		try
		{
			s_pWord->Quit(varZero,   // SaveChanges
						  varZero,   // OriginalFormat
						  varFalse); // RouteDocument
		}
		catch(...)
		{
			// do nothing
		}

		delete s_pWord;
		s_pWord = NULL;
	}
}

BOOL CMSWordHelper::IsWordInstalled(int nMinVersion)
{
	if (CFileRegister::IsRegisteredApp(_T("docx"), _T("WINWORD.EXE"), TRUE))
	{
		return (GetInstalledWordVersion() >= nMinVersion);
	}

	// else
	return FALSE;
}

int CMSWordHelper::GetInstalledWordVersion()
{
	CRegKey2 reg;
	
	if (reg.Open(HKEY_CLASSES_ROOT, _T("Word.Application\\CurVer"), TRUE) == ERROR_SUCCESS)
	{
		CString sCurVer;
		reg.Read(_T(""), sCurVer);
		
		int nLastDot = sCurVer.ReverseFind('.');
		
		if (nLastDot != -1)
		{
			CString sVersion = sCurVer.Mid(nLastDot + 1);
			
			if (Misc::IsNumber(sVersion))
				return _ttoi(sVersion);
		}
	}

	// all else
	return -1;
}

BOOL CMSWordHelper::RestartWord()
{
	if (s_pWord == NULL)
	{
		// first time
		s_pWord = new WordAPI::_Application;
	}
	else
	{
		// detach the interface without Releasing since we are
		// assuming that the interface is cactus
		s_pWord->DetachDispatch();
	}

	// try to restart word
	if (!s_pWord->CreateDispatch(_T("Word.Application")))
	{
		delete s_pWord;
		s_pWord = NULL;

		return FALSE;
	}

	// else all's well
	return TRUE;
}

BOOL CMSWordHelper::ConvertContent(const CString& sInput, WdOpenFormat nInputFormat, 
								   CString& sOutput, WdSaveFormat nOutputFormat, 
									WordAPI::MsoEncoding nEncoding)
{
	if (sInput.IsEmpty())
	{
		sOutput.Empty();
		return TRUE;
	}

	if (!IsValid())
		return FALSE;

	CString sInputFilename = FileMisc::GetTempFilePath(_T("temp"));
	CString sOutputFilename = FileMisc::GetTempFilePath(_T("temp"));
	
	// rtf must be saved as multibyte
	if (FileMisc::SaveFile(sInputFilename, sInput, SFEF_UTF8WITHOUTBOM))
	{
		::DeleteFile(sOutputFilename);

		if (ConvertFile(sInputFilename, nInputFormat, sOutputFilename, nOutputFormat, nEncoding))
		{
			FileMisc::LoadFile(sOutputFilename, sOutput);
		}
	}

	// cleanup
	::DeleteFile(sInputFilename);
	::DeleteFile(sOutputFilename);

	return !sOutput.IsEmpty();
}

BOOL CMSWordHelper::ConvertFile(const CString& sInputFilename, WdOpenFormat nInputFormat, 
								const CString& sOutputFilename, WdSaveFormat nOutputFormat,
								MsoEncoding nEncoding)
{
	if (!IsValid())
		return FALSE;

	if (!FileMisc::FileExists(sInputFilename))
		return FALSE;

	// caller is responsible for ensuring output file does not exist
	if (FileMisc::FileExists(sOutputFilename))
		return FALSE;

	// get document object first. This is a great way to validate
	// that the Word object is still operating.
	Documents docs;

	try
	{
		docs.AttachDispatch(s_pWord->GetDocuments());
	}
	catch (COleException *e) 
	{ 
		SCODE sc = e->m_sc;
		e->Delete(); 

		// assume someone has forcibly closed word from under us
		if (sc == RPC_NOT_AVAIL)
		{
			// try to restart word
			if (!RestartWord())
				return FALSE;
			else
				docs.AttachDispatch(s_pWord->GetDocuments());
		}
	}
	catch(...)
	{
		return FALSE;
	}

	LPDISPATCH lpd = NULL;

	try 
	{
		lpd = docs.Open(COleVariant(sInputFilename),		// FileName
						varFalse,							// ConfirmConversions
						varFalse,							// ReadOnly
						varFalse,							// AddToRecentFiles
						varNull,							// PasswordDocument
						varNull,							// PasswordTemplate
						varTrue,							// Revert
						varNull,							// WritePasswordDocument
						varNull,							// WritePasswordTemplate
						COleVariant((long)nInputFormat),	// Format
						varZero,							// Encoding
						varFalse,							// Visible
						varFalse,							// OpenAndRepair
						varZero,							// DocumentDirection
						varTrue,							// NoEncodingDialog
						varNull);							// XMLTransform
		
	}
	catch (...)
	{
		// do nothing
	}

	if (lpd == NULL)
		return FALSE;

	_Document doc(lpd);
	BOOL bSuccess = TRUE;

	try 
	{
		doc.SetSaveEncoding(nEncoding);

		doc.SaveAs(COleVariant(sOutputFilename),		// FileName
					COleVariant((short)nOutputFormat),	// FileFormat
					varFalse,							// LockComments
					varNull,							// Password
					varFalse,							// AddToRecentFiles
					varNull,							// WritePassword
					varFalse,							// ReadOnlyRecommended
					varFalse,							// EmbedTrueTypeFonts
					varFalse,							// SaveNativePictureFormat
					varFalse,							// SaveFormsData
					varFalse,							// SaveAsAOCELetter
					varZero,							// Encoding
					varFalse,							// InsertLineBreaks
					varFalse,							// AllowSubstitutions
					varOne,								// LineEnding
					varFalse);							// AddBiDiMarks
	}
	catch (...)
	{
		bSuccess = FALSE;
	}

	// cleanup
	doc.Close(varZero,   // SaveChanges
			  varZero,   // OriginalFormat
			  varFalse); // RouteDocument

	return bSuccess;
}

BOOL CMSWordHelper::EnableDisabledDocument(LPCTSTR szFilePath)
{
	return EnableDisabledDocument(HKEY_CURRENT_USER, szFilePath);
}

BOOL CMSWordHelper::EnableDisabledDocument(HKEY hKey, LPCTSTR szFilePath)
{
	CString sValueName;
				
	if (IsDocumentDisabled(hKey, szFilePath, sValueName))
	{
		CString sKey;
		sKey.Format(DISABLED_REGKEY, GetInstalledWordVersion());

		CRegKey2 reg;

		if ((reg.Open(hKey, sKey, FALSE) == ERROR_SUCCESS) &&
			(reg.DeleteValue(sValueName) == ERROR_SUCCESS))
		{
			return TRUE;
		}

		ASSERT(0);
		return FALSE;
	}

	// File was not found in disabled list
	return TRUE;
}

BOOL CMSWordHelper::IsDocumentDisabled(HKEY hKey, LPCTSTR szFilePath, CString& sValueName)
{
	CString sKey;
	sKey.Format(DISABLED_REGKEY, GetInstalledWordVersion());

	if (!CRegKey2::KeyExists(hKey, sKey))
		return FALSE; // nothing disabled

	// Get the list of disabled keys
	CRegKey2 reg;

	if (reg.Open(hKey, sKey, TRUE) != ERROR_SUCCESS) // read-only
		return FALSE;

	CStringArray aValueNames;
	int nValue = reg.GetValueNames(aValueNames);

	if (!nValue)
		return TRUE; // empty key

	// Enumerate the values of the keys until we find the one we want
	BYTE buffer[1024] = { 0 };

	while (nValue--)
	{
		// Inside the loop because Read() will change it
		DWORD dwBufLen = 1024;

		if ((reg.Read(aValueNames[nValue], buffer, dwBufLen) == ERROR_SUCCESS) && dwBufLen)
		{
			// Each disabled item is stored as a randomly-named key of type REG_BINARY. 
			// The format of the byte array is:

			// bytes 0-3 : ??? (perhaps a 32-bit type code, 1 = COM Addin)
			// bytes 4-7 : 32-bit length (in bytes) for the first string (path)
			// bytes 8-11 : 32-bit length (in bytes) for the second string (description)
			// bytes 12-end : two strings of unicode characters, the byte length for 
			//                each of which is stored in the lengths above

			DWORD dwType		= *((DWORD*)(&buffer[0]));
			DWORD dwPathByteLen = *((DWORD*)(&buffer[4]));
			DWORD dwDescByteLen = *((DWORD*)(&buffer[8]));

			ASSERT(dwBufLen == (4 + 4 + 4 + dwPathByteLen + dwDescByteLen));

			CString sFilePath((WCHAR*)(&buffer[12]));
			CString sDescription((WCHAR*)(&buffer[12] + dwPathByteLen));

			//ASSERT(sFilePath.GetLength() == ((dwPathByteLen / sizeof(WCHAR)) - 1));
			//ASSERT(sDescription.GetLength() == ((dwDescByteLen / sizeof(WCHAR)) - 1));

			if (sFilePath.CompareNoCase(szFilePath) == 0)
			{
				sValueName = aValueNames[nValue];
				return TRUE;
			}
		}
	}

	// not found
	return FALSE;
}

BOOL CMSWordHelper::IsDocumentDisabled(LPCTSTR szFilePath)
{
	CString sDummy;

	return IsDocumentDisabled(HKEY_CURRENT_USER, szFilePath, sDummy) ||
			IsDocumentDisabled(HKEY_LOCAL_MACHINE, szFilePath, sDummy);
}

BOOL CMSWordHelper::DisableDocument(LPCTSTR szFilePath, LPCTSTR szDescription)
{
	if (IsDocumentDisabled(szFilePath))
		return TRUE; // already done

	// We always write only to HKEY_CURRENT_USER
	const HKEY hKey = HKEY_CURRENT_USER;

	CString sKey;
	sKey.Format(DISABLED_REGKEY, GetInstalledWordVersion());

	CRegKey2 reg;

	if (reg.Open(hKey, sKey, FALSE) != ERROR_SUCCESS) // writable
		return FALSE;

	// Build the binary data blob
	BYTE buffer[1024] = { 0 };
	DWORD dwBufLen = 0;

	// Each disabled item is stored as a randomly-named key of type REG_BINARY. 
	// The format of the byte array is:

	// bytes 0-3 : ??? (perhaps a 32-bit type code, 1 = COM Addin)
	// bytes 4-7 : 32-bit length (in bytes) for the first string (path)
	// bytes 8-11 : 32-bit length (in bytes) for the second string (description)
	// bytes 12-end : two strings of unicode characters, the byte length for 
	//                each of which is stored in the lengths above
	*((DWORD*)(&buffer[0])) = 1;

	DWORD dwPathByteLen = ((lstrlen(szFilePath) + 1) * sizeof(WCHAR)), dwDescByteLen = 0;
	*((DWORD*)(&buffer[4])) = dwPathByteLen;

	memcpy(&buffer[12], szFilePath, dwPathByteLen);

	if (szDescription)
	{
		dwDescByteLen = ((lstrlen(szDescription) + 1) * sizeof(WCHAR));
		*((DWORD*)(&buffer[8])) = dwDescByteLen;

		memcpy(&buffer[12 + dwPathByteLen], szDescription, dwDescByteLen);
	}

	dwBufLen = (4 + 4 + 4 + dwPathByteLen + dwDescByteLen);

	// Create random value name
	// TODO 
	CString sValueName(_T("123456"));

	return (reg.Write(sValueName, buffer, dwBufLen) == ERROR_SUCCESS);
}
