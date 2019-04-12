// Compression.h: interface for the CCompression class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COMPRESSION_H__CEA5EF3E_A3BD_4542_B749_C6AC41F53E53__INCLUDED_)
#define AFX_COMPRESSION_H__CEA5EF3E_A3BD_4542_B749_C6AC41F53E53__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

namespace Compression  
{
	BOOL Compress(const unsigned char* pContentSrc, int nLenSrc, unsigned char*& pContentDest, int& nLenDest);
	BOOL Decompress(const unsigned char* pContentSrc, int nLenSrc, unsigned char*& pContentDest, int& nLenDest);
};

#endif // !defined(AFX_COMPRESSION_H__CEA5EF3E_A3BD_4542_B749_C6AC41F53E53__INCLUDED_)
