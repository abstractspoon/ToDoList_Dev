// BinaryData.h: interface for the CBinaryData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BINARYDATA_H__4BDC3FF9_9A50_4386_99EF_449CDCE47CBB__INCLUDED_)
#define AFX_BINARYDATA_H__4BDC3FF9_9A50_4386_99EF_449CDCE47CBB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CBinaryData : protected CString
{
public:
  CBinaryData(LPCTSTR szData = NULL); 
  CBinaryData(const unsigned char* pData, int nByteLength);
  CBinaryData(const CString& sContent);
  CBinaryData(const CBinaryData& data);

  void operator = (const CBinaryData& data);

  ~CBinaryData() { Empty(); }

  unsigned char* GetBuffer(int nByteLength);
  void ReleaseBuffer(int nByteLength);

  const unsigned char* Get(int& nByteLength) const;
  const unsigned char* Get() const;

  void Empty();
  BOOL IsEmpty() const;

  bool operator == (const CBinaryData& data) const;
  bool operator != (const CBinaryData& data) const;

  int GetLength() const { return GetByteLength(); }

  BOOL Base64Encode(CString& sEncoded) const;
  BOOL Base64Decode(const CString& sEncoded);

  CString AsString() const;
  static CString AsString(const unsigned char* pData, int nByteLength);

protected:
  int GetByteLength() const;
  void Set(const unsigned char* pData, int nByteLength);

  static int GetEquivalentStringLength(int nByteLength);

  operator const unsigned char*() const;

};

#endif // !defined(AFX_BINARYDATA_H__4BDC3FF9_9A50_4386_99EF_449CDCE47CBB__INCLUDED_)
