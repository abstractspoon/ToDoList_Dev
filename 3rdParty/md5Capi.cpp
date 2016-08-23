
//////////////////////////////////////////////////////////////////////
// md5Capi.cpp: implementation of the Cmd5Capi class.
// Calcule MD5 Digest using the WIN Crypto API.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "md5Capi.h"

#include <wincrypt.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Cmd5Capi::Cmd5Capi()
{
	m_sDigest.Empty();
}

Cmd5Capi::Cmd5Capi(CString & csBuffer)
{
	Digest(csBuffer);
}



Cmd5Capi::~Cmd5Capi()
{

}

CString &Cmd5Capi::GetDigest(void)
{
	return m_sDigest; 

}


CString &Cmd5Capi::Digest(CString & csBuffer)
{
    HCRYPTPROV hCryptProv; 
    HCRYPTHASH hHash; 
    BYTE bHash[0x7f]; 
    DWORD dwHashLen= 16; // The MD5 algorithm always returns 16 bytes. 
    DWORD cbContent= csBuffer.GetLength(); 
    BYTE* pbContent= (BYTE*)csBuffer.GetBuffer(cbContent); 


    if(CryptAcquireContext(&hCryptProv, 
		NULL, NULL, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT | CRYPT_MACHINE_KEYSET)) 
	{

		if(CryptCreateHash(hCryptProv, 
			CALG_MD5,	// algorithm identifier definitions see: wincrypt.h
			0, 0, &hHash)) 
		{
			if(CryptHashData(hHash, pbContent, cbContent, 0))
			{

				if(CryptGetHashParam(hHash, HP_HASHVAL, bHash, &dwHashLen, 0)) 
				{
					// Make a string version of the numeric digest value
					m_sDigest.Empty();
					CString tmp;
					  for (int i = 0; i<16; i++)
					  {
						tmp.Format("%02x", bHash[i]);
						m_sDigest+=tmp;
					  }

				}
				else m_sDigest=_T("Error getting hash param"); 

			}
			else m_sDigest=_T("Error hashing data"); 
		}
		else m_sDigest=_T("Error creating hash"); 

    }
    else m_sDigest=_T("Error acquiring context"); 


    CryptDestroyHash(hHash); 
    CryptReleaseContext(hCryptProv, 0); 
	csBuffer.ReleaseBuffer();
    return m_sDigest; 


}
