// Compression.cpp: implementation of the CCompression class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Compression.h"

#include "..\3rdparty\zlib\zlib.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

BOOL Compression::Compress(const unsigned char* pContentSrc, int nLenSrc, 
								  unsigned char*& pContentDest, int& nLenDest)
{
	ULONG lDest = (int)(nLenSrc * 1.001) + 12; // zlib requirements
	pContentDest = new unsigned char[lDest + 4]; // extra DWORD for storing the source length

	// leave the first DWORD free
	int nRet = compress(pContentDest + 4, &lDest, pContentSrc, nLenSrc);

	if (nRet != Z_OK)
	{
		// cleanup
		delete [] pContentDest;
		nLenDest = 0;
	}
	else
	{
		nLenDest = lDest + 4;

		// encode the source length into the first 4 bytes
		((DWORD*)pContentDest)[0] = nLenSrc;
	}

	return (nRet == Z_OK);
}

BOOL Compression::Decompress(const unsigned char* pContentSrc, int nLenSrc, 
									unsigned char*& pContentDest, int& nLenDest)
{
	// get the source length from the first 4 bytes
	ULONG lDest = ((DWORD*)pContentSrc)[0];

	// sanity check
	if (lDest < (ULONG)nLenSrc || lDest > (ULONG)(1000 * nLenSrc))
		return FALSE;

	pContentDest = new unsigned char[lDest];

	int nRet = uncompress(pContentDest, &lDest, pContentSrc + 4, nLenSrc - 4);

	if (nRet != Z_OK)
	{
		delete [] pContentDest;
		nLenDest = 0;
	}
	else
		nLenDest = lDest;

	return (nRet == Z_OK);
}
