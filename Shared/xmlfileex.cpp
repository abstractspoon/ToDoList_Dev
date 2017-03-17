// xmlfileex.cpp: implementation of the CXmlFileEx class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "xmlfileex.h"
#include "stringres.h"
#include "enstring.h"
#include "misc.h"
#include "passworddialog.h"

#include "..\Interfaces\iencryption.h"

#include "..\3rdparty\base64coder.h"
#include "..\3rdparty\xmlnodewrapper.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CString CXmlFileEx::s_sPasswordExplanation(ENCRYPT_ENTERPWD);
CString CXmlFileEx::s_sDecryptFailed(ENCRYPT_DECRYPTFAILED);

CString CXmlFileEx::XFE_ENCODEDDATA = _T("ENCODEDDATA");
CString CXmlFileEx::XFE_ENCODEDDATALEN = _T("DATALEN");

CXmlFileEx::CXmlFileEx(const CString& sRootItemName, const CString& sPassword)
: CXmlFile(sRootItemName), m_pEncryptor(NULL), m_sPassword(sPassword)
{
	
}

CXmlFileEx::~CXmlFileEx()
{
	
}

void CXmlFileEx::SetUIStrings(LPCWSTR szPasswordExplanation, LPCWSTR szDecryptFailed)
{
	s_sPasswordExplanation = szPasswordExplanation;
	s_sDecryptFailed = szDecryptFailed;
}


BOOL CXmlFileEx::Encrypt(LPCWSTR szPassword, SFE_FORMAT nFormat)
{
	if (!szPassword)
		szPassword = m_sPassword;
	
	if (!(*szPassword) || !InitEncryptor())
		return FALSE;
	
	// 1. export everything below the root to a string
	CXmlDocumentWrapper doc;
	doc.LoadXML(_T("<a></a>"));

	CXmlNodeWrapper nodeDoc(doc.AsNode());

	CString sXml;
	POSITION pos = m_xiRoot.GetFirstItemPos();
	int nNode = 0;
	
	while (pos)
	{
		const CXmlItem* pXI = m_xiRoot.GetNextItem(pos);
		ASSERT (pXI);

		while (pXI)
		{
			CXmlNodeWrapper nodeChild(nodeDoc.InsertNode(nNode++, pXI->GetName()));
			ASSERT (nodeChild.IsValid());

			Export(pXI, &nodeChild);
			sXml += nodeChild.GetXML();

			// siblings if there are any
			pXI = pXI->GetSibling();
		}
	}
	
	// 2. encrypt it
	if (nFormat == SFEF_AUTODETECT)
		nFormat = m_nFormat;

	// password must be ANSI to match non-unicode build
	int nLen = lstrlen(szPassword);
	LPSTR szMultiPassword = Misc::WideToMultiByte(szPassword, nLen);
	szPassword = (LPCWSTR)szMultiPassword;

	// encrypt the xml as a binary byte array
	unsigned char* pEncrypted = NULL;
	int nLenEncrypted = 0;
	BOOL bResult = FALSE;
	
	if (nFormat == SFEF_UTF16)
	{
		bResult = m_pEncryptor->Encrypt((const unsigned char*)(LPCWSTR)sXml, (sXml.GetLength() + 1) * sizeof(TCHAR), 
										(const char*)szPassword, pEncrypted, nLenEncrypted);
	}
	else
	{
		int nLenXml = sXml.GetLength();
		LPSTR szMultiXml = Misc::WideToMultiByte(sXml, nLenXml);

		bResult = m_pEncryptor->Encrypt((const unsigned char*)szMultiXml, nLenXml + 1, 
										(const char*)szPassword, pEncrypted, nLenEncrypted);

		// cleanup
		delete [] szMultiXml;
	}

	// 3. convert the binary byte array to a string
	if (bResult)
	{
		Base64Coder b64;
		
		b64.Encode(pEncrypted, nLenEncrypted);
		CString sEncodedDataBuffer = b64.EncodedMessage();
		
		// 4. replace file contents with a single CDATA item
		m_xiRoot.DeleteAllItems();
		m_xiRoot.AddItem(XFE_ENCODEDDATA, sEncodedDataBuffer, XIT_CDATA);
		m_xiRoot.AddItem(XFE_ENCODEDDATALEN, nLenEncrypted);
	}
	
	// 5. cleanup
	delete [] szMultiPassword;

	m_pEncryptor->FreeBuffer(pEncrypted);
	
	return TRUE;
}

BOOL CXmlFileEx::IsEncrypted()
{
	return (GetItemValueI(XFE_ENCODEDDATALEN) > 0);
}

BOOL CXmlFileEx::Decrypt(LPCWSTR szPassword)
{
	if (!IsEncrypted())
		return TRUE; // nothing to do
    
	// we don't try to decrypt if no encryption capabilities
	if (!CanEncrypt())
	{
		m_nFileError = XFL_NOENCRYPTIONDLL;
		return FALSE;
	}
	
	// use existing password if required
	if (!szPassword)
		szPassword = m_sPassword;

	CXmlItem* pXI = GetEncryptedBlock();
    
	if (pXI && !pXI->GetSibling())
	{
		// else keep getting password till success or user cancels
		CString sPassword(szPassword);
		
		while (TRUE)
		{
			if (sPassword.IsEmpty())
			{
				CString sExplanation(s_sPasswordExplanation);

				if (sExplanation.Find(_T("%s")) != -1)
					sExplanation.Format(s_sPasswordExplanation, GetFileName());
				
				if (!CPasswordDialog::RetrievePassword(FALSE, sPassword, sExplanation))
				{
					// RB - Set m_nFileError to avoid "The selected task list could not be opened..." message when cancelling
					m_nFileError = XFL_CANCELLED;
					return FALSE;
				}
			}
			
			CString sFile;
			
			if (Decrypt(pXI->GetValue(), sFile, sPassword))
			{
				m_sPassword = sPassword;
				
				Misc::Trim(sFile);
				sFile = _T("<ROOT>") + sFile + _T("</ROOT>");
				
				// delete the cdata item and datalen
				m_xiRoot.DeleteItem(pXI);
				m_xiRoot.DeleteItem(XFE_ENCODEDDATALEN);
				
				try
				{
					CXmlDocumentWrapper doc;
					
					// reparse decrypted xml
					if (doc.LoadXML(sFile))
					{
						CXmlNodeWrapper node(doc.AsNode());
						
						return ParseItem(m_xiRoot, &node);
					}
				}
				catch (...)
				{
					m_nFileError = XFL_BADMSXML;
				}
				
				return FALSE;
			}
			// RB - Added code to format the error message before calling AfxMessage
			else
			{
				CEnString sMessage(s_sDecryptFailed, GetFileName());

				if (IDNO == AfxMessageBox(sMessage, MB_YESNO))
				{
					m_nFileError = XFL_CANCELLED;
					return FALSE;
				}
				// else user will try again
				sPassword.Empty();
			}
		}
	}
    
	// else
	m_nFileError = XFL_UNKNOWNENCRYPTION;
	return FALSE;
}

CXmlItem* CXmlFileEx::GetEncryptedBlock()
{
	CXmlItem* pXI = NULL;
	int nDataLen = GetItemValueI(XFE_ENCODEDDATALEN);
	
	if (nDataLen)
	{
		pXI = GetItem(XFE_ENCODEDDATA);
		
		if (!pXI)
		{
			// backwards compatibility
			pXI = GetItem(_T("CDATA"));
			
			if (!pXI)
			{
				// missing tags (last ditched effort)
				pXI = &m_xiRoot;
			}
		}
	}
	
	return pXI;
}

BOOL CXmlFileEx::Load(const CString& sFilePath, const CString& sRootItemName, IXmlParse* pCallback, BOOL bDecrypt)
{
	m_bDecrypt = bDecrypt;
	
	return CXmlFile::Load(sFilePath, sRootItemName, pCallback);
}

BOOL CXmlFileEx::Open(const CString& sFilePath, XF_OPEN nOpenFlags, BOOL bDecrypt)
{
	m_bDecrypt = bDecrypt;
	
	return CXmlFile::Open(sFilePath, nOpenFlags);
}

BOOL CXmlFileEx::LoadEx(const CString& sRootItemName, IXmlParse* pCallback)
{
	if (!CXmlFile::LoadEx(sRootItemName, pCallback))
		return FALSE;
	
	// we assume the file is encrypted if it contains a single CDATA element
	if (m_bDecrypt)
		return Decrypt();
	
	return TRUE;
}

BOOL CXmlFileEx::Decrypt(LPCWSTR szInput, CString& sOutput, LPCWSTR szPassword)
{
	if (!InitEncryptor())
		return FALSE;
	
	Base64Coder b64;

	// 0. if necessary convert from wide to multibyte

	// content
	int nLen = lstrlen(szInput);
	LPSTR szMultiInput = Misc::WideToMultiByte(szInput, nLen);
	b64.Decode((const PBYTE)szMultiInput, nLen);
	delete [] szMultiInput;

	// password must be ANSI to match older (non-unicode) file formats
	nLen = lstrlen(szPassword);
	LPSTR szMultiPassword = Misc::WideToMultiByte(szPassword, nLen);
	szPassword = (LPCWSTR)szMultiPassword;

	// 1. convert the input string back to binary
	DWORD nReqBufLen = 0;
	unsigned char* pDecodedDataBuffer = b64.DecodedMessage(nReqBufLen);
	
	nReqBufLen = GetItemValueI(_T("DATALEN"));
	
	// 2. decrypt it
	unsigned char* pDecrypted = NULL;
	int nLenDecrypted = 0;
	
	BOOL bResult = (m_pEncryptor->Decrypt(pDecodedDataBuffer, nReqBufLen, 
											(const char*)szPassword, pDecrypted, nLenDecrypted));

	delete [] szMultiPassword;
	
	// 3. result should be a null-terminated string
	if (bResult)
	{
		// if the file format is unicode we can assume that the encrypted content
		// was also unicode (when it was written)
		if (m_nFormat == SFEF_UTF16)
			sOutput = (LPCWSTR)pDecrypted;
		else
			sOutput = (LPCSTR)pDecrypted;
	}
	
	// 4. cleanup
	m_pEncryptor->FreeBuffer(pDecrypted);
	
	return bResult;
}

BOOL CXmlFileEx::InitEncryptor()
{
	if (m_pEncryptor)
		return TRUE;
	
	m_pEncryptor = CreateEncryptionInterface(_T("EncryptDecrypt.dll"));
	
	return (m_pEncryptor != NULL);
}

BOOL CXmlFileEx::CanEncrypt()
{
	return CXmlFileEx().InitEncryptor();
}
