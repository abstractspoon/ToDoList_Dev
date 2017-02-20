// XmlFile.h: interface for the CXmlFile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XMLFILE_H__54045703_1A98_41AB_B1A1_0B165C350AA0__INCLUDED_)
#define AFX_XMLFILE_H__54045703_1A98_41AB_B1A1_0B165C350AA0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////////////

#include <afxtempl.h>

#include "..\3rdparty\stdiofileex.h"
#include "..\3rdparty\xmlnodewrapper.h"

/////////////////////////////////////////////////////////////////////////////////////

// global fns for translating text to/from xml representations eg '&lt;' becomes '<' and vice versa
//CString& XML2TXT(CString& xml);
CString& TXT2XML(CString& txt);
CString& TXT2HTML(CString& txt);

/////////////////////////////////////////////////////////////////////////////////////

class CXmlDocumentWrapper;
class CXmlNodeWrapper;

/////////////////////////////////////////////////////////////////////////////////////

enum XI_TYPE
{
   XIT_ELEMENT,
   XIT_ATTRIB,
   XIT_CDATA,
};

/////////////////////////////////////////////////////////////////////////////////////

class CXmlItem
{
public:
	CXmlItem(CXmlItem* pParent = NULL, const CString& sName = EMPTY_STR, const CString& sValue = EMPTY_STR, XI_TYPE nType = XIT_ELEMENT);
	CXmlItem(const CXmlItem& xi, CXmlItem* pParent = NULL); // copies siblings too
	virtual ~CXmlItem();

	void Copy(const CXmlItem& xi, BOOL bCopySiblings);
	void Reset();

	const CXmlItem* GetItem(const CString& sItemName, const CString& sSubItemName = EMPTY_STR) const;
	CXmlItem* GetItem(const CString& sItemName, const CString& sSubItemName = EMPTY_STR);
	CXmlItem* GetAddItem(const CString& sItemName, XI_TYPE nType = XIT_ELEMENT);
	const CXmlItem* GetItem(const CString& sItemName, int nIndex) const;
	CXmlItem* GetItem(const CString& sItemName, int nIndex);

	BOOL HasItem(const CString& sItemName, const CString& sSubItemName = EMPTY_STR) const;

	const CXmlItem* FindItem(const CString& sItemName, const CString& sItemValue, BOOL bSearchChildren = TRUE) const;
	const CXmlItem* FindItem(const CString& sItemName, int nItemValue, BOOL bSearchChildren = TRUE) const;
	const CXmlItem* FindItem(const CString& sItemName, double dItemValue, BOOL bSearchChildren = TRUE) const;
	CXmlItem* FindItem(const CString& sItemName, const CString& sItemValue, BOOL bSearchChildren = TRUE);
	CXmlItem* FindItem(const CString& sItemName, int nItemValue, BOOL bSearchChildren = TRUE);
	CXmlItem* FindItem(const CString& sItemName, double dItemValue, BOOL bSearchChildren = TRUE);

	BOOL NameIs(const CString& sName) const;
	const CString& GetName() const;
	int GetNameLen() const;
	BOOL HasName() const;

	BOOL HasValue() const;
	int GetValueLen() const;
	const CString& GetValue() const;
	int GetValueI() const;
	double GetValueF() const;

	const CString& GetItemValue(const CString& sItemName, const CString& sSubItemName = EMPTY_STR) const;
	int GetItemValueI(const CString& sItemName, const CString& sSubItemName = EMPTY_STR) const;
	double GetItemValueF(const CString& sItemName, const CString& sSubItemName = EMPTY_STR) const;

	int GetItemCount() const;
	int GetItemCount(const CString& sItemName, const CString& sSubItemName = EMPTY_STR) const;
	int GetSiblingCount() const;

    // returns the new item
	CXmlItem* AddItem(const CString& sName, const CString& sValue = EMPTY_STR, XI_TYPE nType = XIT_ATTRIB);
	CXmlItem* AddItem(const CString& sName, int nValue, XI_TYPE nType = XIT_ATTRIB);
	CXmlItem* AddItem(const CString& sName, double dValue, XI_TYPE nType = XIT_ATTRIB);
	CXmlItem* AddItem(const CXmlItem& xi, BOOL bCopySiblings); // item and all attributes are copied
	CXmlItem* AddItem(CXmlItem* pXI); 

    // returns the new or existing item
	CXmlItem* SetItemValue(const CString& sName, const CString& sValue, XI_TYPE nType = XIT_ATTRIB);
	CXmlItem* SetItemValue(const CString& sName, int nValue, XI_TYPE nType = XIT_ATTRIB);
	CXmlItem* SetItemValue(const CString& sName, double dValue, XI_TYPE nType = XIT_ATTRIB);

	BOOL SetName(const CString& sName); // can't have any siblings
	void SetValue(const CString& sValue);
	void SetValue(int nValue);
	void SetValue(double dValue);
	void ClearValue();

	BOOL RemoveItem(CXmlItem* pXIChild); // does not remove siblings or delete item
	BOOL DeleteItem(CXmlItem* pXIChild); // does not remove siblings
	BOOL DeleteItem(const CString& sItemName, const CString& sSubItemName = EMPTY_STR); // Removes siblings
	void DeleteAllItems();

	const CXmlItem* GetParent() const;
	const CXmlItem* GetSibling() const;
	CXmlItem* GetParent();
	CXmlItem* GetSibling();

	BOOL IsAttribute(int nMaxAttribLen = 8192) const; 
	BOOL IsCDATA() const;
	BOOL SetType(XI_TYPE nType);
	XI_TYPE GetType() const { return m_nType; }

	POSITION GetFirstItemPos() const;
	const CXmlItem* GetNextItem(POSITION& pos) const;
	CXmlItem* GetNextItem(POSITION& pos);

	// matching helpers
	BOOL NameMatches(const CString& sName) const;
	BOOL NameMatches(const CXmlItem* pXITest) const;
	BOOL ValueMatches(const CString& sValue, BOOL bIgnoreCase = TRUE) const;
	BOOL ValueMatches(const CXmlItem* pXITest, BOOL bIgnoreCase = TRUE) const;
	BOOL ItemValueMatches(const CXmlItem* pXITest, const CString& sItemName, BOOL bIgnoreCase = TRUE) const;
	
	// sorting
	void SortItems(const CString& sItemName, const CString& sKeyName, BOOL bAscending = TRUE); // sort by string value
	void SortItemsI(const CString& sItemName, const CString& sKeyName, BOOL bAscending = TRUE); // sort by int value
	void SortItemsF(const CString& sItemName, const CString& sKeyName, BOOL bAscending = TRUE); // sort by float value
	
	static void ValidateString(CString& sText, TCHAR cReplace = ' ');

protected:
	CString m_sName;
	CString m_sValue;
    XI_TYPE m_nType;

	CXmlItem* m_pParent;
	CXmlItem* m_pSibling;

	CList<CXmlItem*, CXmlItem*> m_lstItems; // children

protected:
	static const CString EMPTY_STR;

protected:
	BOOL AddSibling(CXmlItem* pXI); // must share the same name and parent
	const CXmlItem* GetItemEx(const CString& sItemName, const CString& sSubItemName = EMPTY_STR) const;
	const CXmlItem* FindItemEx(const CString& sItemName, const CString& sItemValue, BOOL bSearchChildren = TRUE) const;

	enum XI_SORTKEY { XISK_STRING, XISK_INT, XISK_FLOAT };
	void SortItems(const CString& sItemName, const CString& sKeyName, XI_SORTKEY nKey, BOOL bAscending);

	static int CompareItems(const CXmlItem* pXIItem1, const CXmlItem* pXIItem2, const CString& sKeyName, XI_SORTKEY nKey);
	
	static CString ToString(int nValue);
	static CString ToString(double dValue);
};

/////////////////////////////////////////////////////////////////////////////////////

class IXmlParse
{
public:
	// return TRUE to continue parsing
	virtual BOOL Continue(LPCTSTR szItem, LPCTSTR szValue) const = 0;
};

/////////////////////////////////////////////////////////////////////////////////////

enum XF_OPEN 
{
	XF_READ,
	XF_WRITE,
	XF_READWRITE,
};

/////////////////////////////////////////////////////////////////////////////////////

enum // load errors. if >0 then == GetLastFileError
{
	XFL_NONE		= 0,
	XFL_CANCELLED	= -1,
	XFL_MISSINGROOT = -2,
	XFL_BADMSXML	= -3,
    XFL_LAST        = -99
};

/////////////////////////////////////////////////////////////////////////////////////

class CXmlFile : protected CStdioFileEx 
{
public:
	CXmlFile(const CString& sRootItemName = EMPTY_STR);
	virtual ~CXmlFile();

	BOOL Load(const CString& sFilePath, const CString& sRootItemName = EMPTY_STR, IXmlParse* pCallback = NULL);
	BOOL Save(const CString& sFilePath, SFE_FORMAT nFormat);

	BOOL LoadContent(const CString& sContent, const CString& sRootItemName = EMPTY_STR);

	void CopyFrom(const CXmlFile& file);
	void Reset();
	int GetItemCount() const;

	// extended interface
	BOOL Open(const CString& sFilePath, XF_OPEN nOpenFlags, SFE_FORMAT nFormat = SFEF_AUTODETECT);
	virtual BOOL SaveEx();
	virtual BOOL LoadEx(const CString& sRootItemName = EMPTY_STR, IXmlParse* pCallback = NULL);
	void Close();
	int GetLastFileError() const;

	BOOL Export(CString& sOutput) const;
	BOOL Transform(const CString& sTransformPath, CString& sOutput) const;
	BOOL TransformToFile(const CString& sTransformPath, const CString& sOutputPath, SFE_FORMAT nFormat) const;

	const CXmlItem* Root() const;
	CXmlItem* Root();

	CString GetRootValue() const;
	int GetRootValueI() const;
	double GetRootValueF() const;

	const CXmlItem* GetItem(const CString& sItemName) const;
	CXmlItem* GetItem(const CString& sItemName);
	CXmlItem* GetAddItem(const CString& sItemName, XI_TYPE nType = XIT_ELEMENT);
	const CXmlItem* GetItem(const CString& sItemName, int nIndex) const;
	CXmlItem* GetItem(const CString& sItemName, int nIndex);

	const CXmlItem* FindItem(const CString& sItemName, const CString& sItemValue, BOOL bSearchChildren = TRUE) const;
	const CXmlItem* FindItem(const CString& sItemName, int nItemValue, BOOL bSearchChildren = TRUE) const;
	const CXmlItem* FindItem(const CString& sItemName, double dItemValue, BOOL bSearchChildren = TRUE) const;

	CXmlItem* FindItem(const CString& sItemName, const CString& sItemValue, BOOL bSearchChildren = TRUE);
	CXmlItem* FindItem(const CString& sItemName, int nItemValue, BOOL bSearchChildren = TRUE);
	CXmlItem* FindItem(const CString& sItemName, double dItemValue, BOOL bSearchChildren = TRUE);

	CXmlItem* AddItem(const CString& sName, const CString& sValue = EMPTY_STR, XI_TYPE nType = XIT_ATTRIB);
	CXmlItem* AddItem(const CString& sName, int nValue, XI_TYPE nType = XIT_ATTRIB);
	CXmlItem* AddItem(const CString& sName, double dValue, XI_TYPE nType = XIT_ATTRIB);

	BOOL DeleteItem(CXmlItem* pXI);
	BOOL DeleteItem(const CString& sItemName, const CString& sSubItemName = EMPTY_STR); 

	CXmlItem* SetItemValue(const CString& sName, const CString& sValue, XI_TYPE nType = XIT_ATTRIB);
	CXmlItem* SetItemValue(const CString& sName, int nValue, XI_TYPE nType = XIT_ATTRIB);
	CXmlItem* SetItemValue(const CString& sName, double dValue, XI_TYPE nType = XIT_ATTRIB);

	CString GetItemValue(const CString& sItemName, const CString& sSubItemName = EMPTY_STR) const;
	int GetItemValueI(const CString& sItemName, const CString& sSubItemName = EMPTY_STR) const;
	double GetItemValueF(const CString& sItemName, const CString& sSubItemName = EMPTY_STR) const;

	// returns the number of subitems having this name ie inc. siblings
	int GetItemCount(const CString& sItemName) const;

	CString GetFilePath() const;
	CString GetFileName() const;
	const HANDLE GetFileHandle() const;

	CString GetXmlHeader() const;
	CString GetXslHeader() const;
	BOOL SetXmlHeader(const CString& sHeader);
	BOOL SetXslHeader(const CString& sHeader);

	void Trace() const;

	// sorting
	void SortItems(const CString& sItemName, const CString& sKeyName, BOOL bAscending = TRUE);
	void SortItemsI(const CString& sItemName, const CString& sKeyName, BOOL bAscending = TRUE);
	void SortItemsF(const CString& sItemName, const CString& sKeyName, BOOL bAscending = TRUE);
	
protected:
	CXmlItem m_xiRoot;
	CString m_sXmlHeader, m_sXslHeader;
	IXmlParse* m_pCallback;

	mutable CXmlDocumentWrapper m_xmlDoc, m_xslDoc;
	mutable int m_nFileError;

protected:
	static const CString EMPTY_STR;
	
protected:
	BOOL ParseItem(CXmlItem& xi, CXmlNodeWrapper* pNode); // returns false if the callback stopped it
	BOOL ParseRootItem(CXmlDocumentWrapper* pDoc, const CString& sRootItemName);
	BOOL ContinueParsing(const CString& sItem, const CString& sValue); 
	BOOL BuildDOM() const;

	static int Export(const CXmlItem* pItem, CXmlNodeWrapper* pNode);
	static void FixInputString(CString& sXml, const CString& sRootItem);
};

/////////////////////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_XMLFILE_H__54045703_1A98_41AB_B1A1_0B165C350AA0__INCLUDED_)
