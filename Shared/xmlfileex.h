// xmlfileex.h: interface for the CXmlFileEx class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XMLFILEEX_H__C7DFA5B5_2B36_4D63_942C_9054EF4240CB__INCLUDED_)
#define AFX_XMLFILEEX_H__C7DFA5B5_2B36_4D63_942C_9054EF4240CB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XmlFile.h"

class IEncryption;

enum
{
	XFL_NOENCRYPTIONDLL	   = XFL_LAST - 1,
	XFL_UNKNOWNENCRYPTION	= XFL_LAST - 2,
};

class CXmlFileEx : public CXmlFile  
{
public:
	CXmlFileEx(const CString& sRootItemName = _T(""), const CString& sPassword = _T(""));
	virtual ~CXmlFileEx();
	
	void SetPassword(LPCWSTR szPassword) { m_sPassword = szPassword; }
	CString GetPassword() { return m_sPassword; }
	
	// allows for selective decrypting
	BOOL Load(const CString& sFilePath, const CString& sRootItemName = _T(""), IXmlParse* pCallback = NULL, BOOL bDecrypt = TRUE);
	BOOL Open(const CString& sFilePath, XF_OPEN nOpenFlags, BOOL bDecrypt = TRUE);
	virtual BOOL LoadEx(const CString& sRootItemName = _T(""), IXmlParse* pCallback = NULL);
	
	// call before standard save
	virtual BOOL Encrypt(LPCWSTR szPassword = NULL, SFE_FORMAT nFormat = SFEF_AUTODETECT); 
	virtual BOOL Decrypt(LPCWSTR szPassword = NULL); 
	static BOOL CanEncrypt(); // false if encryptor dll cannot be loaded
	
	static void SetUIStrings(LPCWSTR szPasswordExplanation, LPCWSTR szDecryptFailed);

	static CString XFE_ENCODEDDATA;
	static CString XFE_ENCODEDDATALEN;

protected:
	IEncryption* m_pEncryptor;
	CString m_sPassword;
	BOOL m_bDecrypt;

	static CString s_sPasswordExplanation, s_sDecryptFailed;

protected:
	BOOL Decrypt(LPCWSTR szInput, CString& sOutput, LPCWSTR szPassword);
	BOOL InitEncryptor();
	BOOL IsEncrypted();
	CXmlItem* GetEncryptedBlock();
	
};

#endif // !defined(AFX_XMLFILEEX_H__C7DFA5B5_2B36_4D63_942C_9054EF4240CB__INCLUDED_)
