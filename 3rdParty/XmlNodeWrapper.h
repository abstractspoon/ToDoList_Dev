// XmlNodeWrapper.h: interface for the CXmlNodeWrapper class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XMLNODEWRAPPER_H__43622334_FDEB_4175_9E6D_19BBAA3992A5__INCLUDED_)
#define AFX_XMLNODEWRAPPER_H__43622334_FDEB_4175_9E6D_19BBAA3992A5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//////////////////////////////////////////////////////////////////////////////

#import "MSXML3.dll" named_guids 
using namespace MSXML2;

//////////////////////////////////////////////////////////////////////////////

class CXmlNodeWrapper  
{
public:
	CXmlNodeWrapper();
	CXmlNodeWrapper(MSXML2::IXMLDOMNodePtr pNode,BOOL bAutoRelease = TRUE);
	CXmlNodeWrapper& operator=(MSXML2::IXMLDOMNodePtr pNode);
	virtual ~CXmlNodeWrapper();

	CString NodeType();
	int NodeTypeVal();
	CString GetAttribVal(int index);
	CString GetAttribName(int index);
	int GetAttributes(CStringArray& aNames, CStringArray& aValues);
	int NumAttributes();
	void ReplaceNode(MSXML2::IXMLDOMNodePtr pOldNode,MSXML2::IXMLDOMNodePtr pNewNode);
	CString GetText();
	void SetText(const CString& text);
	MSXML2::IXMLDOMNodePtr InsertAfter(MSXML2::IXMLDOMNodePtr refNode, MSXML2::IXMLDOMNodePtr pNode);
	CString Name();
	MSXML2::IXMLDOMNodeListPtr FindNodes(const CString& searchStr);
	MSXML2::IXMLDOMNodePtr Parent();
	void RemoveNodes(const CString& searchStr);
	void Reset();
	MSXML2::IXMLDOMNodePtr InsertAfter(MSXML2::IXMLDOMNodePtr refNode, const CString& nodeName);
	MSXML2::IXMLDOMNodePtr InsertBefore(MSXML2::IXMLDOMNodePtr refNode, const CString& nodeName);
	MSXML2::IXMLDOMNode* Interface();
	MSXML2::IXMLDOMDocumentPtr ParentDocument();
	CString GetXML();
	MSXML2::IXMLDOMNodePtr RemoveNode(MSXML2::IXMLDOMNodePtr pNode);
	MSXML2::IXMLDOMNodePtr InsertNode(int index, const CString& nodeName);
	MSXML2::IXMLDOMNodePtr InsertNode(int index,MSXML2::IXMLDOMNodePtr pNode);
	MSXML2::IXMLDOMNodePtr AppendChild(MSXML2::IXMLDOMNodePtr pNode);
	long NumNodes();
	MSXML2::IXMLDOMNode* Detach();
	MSXML2::IXMLDOMNodePtr GetNode(const CString& nodeName);
	MSXML2::IXMLDOMNodePtr GetNode(int nodeIndex);
	MSXML2::IXMLDOMNodePtr FindNode(const CString& searchString);
	MSXML2::IXMLDOMNodePtr GetPrevSibling();
	MSXML2::IXMLDOMNodePtr GetNextSibling();
	MSXML2::IXMLDOMNodePtr GetFirstChildNode();

	void SetValue(const CString& valueName,const CString& value);
	void SetValue(const CString& valueName,int value);
	void SetValue(const CString& valueName,short value);
	void SetValue(const CString& valueName,double value);
	void SetValue(const CString& valueName,float value);
	void SetValue(const CString& valueName,bool value);
	BOOL IsValid();
	CString GetValue(const CString& valueName);
	BOOL IsPreservingWhiteSpace() const;

	// fixed version
	static BSTR ConvertStringToBSTR(const CString& pSrc);

private:
	BOOL m_bAutoRelease;
	MSXML2::IXMLDOMNodePtr m_xmlnode;
};

//////////////////////////////////////////////////////////////////////////////

class CXmlDocumentWrapper
{
public:
	CXmlDocumentWrapper();
	CXmlDocumentWrapper(const CString& header);
	CXmlDocumentWrapper(const CString& header, const CString& sRootItem);
	CXmlDocumentWrapper(MSXML2::IXMLDOMDocumentPtr pDoc);
	virtual ~CXmlDocumentWrapper();

	CString GetUrl() const;
	CString GetXML(BOOL bPreserveWhiteSpace = TRUE) const;
	BOOL IsValid() const;
	BOOL Load(const CString& path, BOOL bPreserveWhiteSpace = TRUE);
	BOOL Transform(CXmlDocumentWrapper& xsl, CString& sOutput) const;
	BOOL LoadXML(const CString& xml, BOOL bPreserveWhiteSpace = TRUE);
	BOOL Save(const CString& path = _T(""), BOOL bPreserveWhiteSpace = TRUE);
	void Reset();

	CString GetXmlHeader(BOOL bAsXml = FALSE) const;
	CString GetXslHeader(BOOL bAsXml = FALSE) const;
	BOOL SetXmlHeader(const CString& szHeader);
	BOOL SetXslHeader(const CString& szHeader);

	static BOOL IsVersion3orGreater();

	MSXML2::IXMLDOMDocument* Detach();

	MSXML2::IXMLDOMDocumentPtr Clone();
	MSXML2::IXMLDOMDocument* Interface();
	void operator=(MSXML2::IXMLDOMDocumentPtr pDoc);
	MSXML2::IXMLDOMNodePtr AsNode();

protected:
	long FindHeaderItem(const CString& sName = _T("")) const;
	CString GetHeader(const CString& sName, BOOL bAsXml = FALSE) const;

private:
	MSXML2::IXMLDOMDocumentPtr m_xmldoc;
	BOOL m_bHeaderSet;
	CString m_sRootItemName;

	static BOOL s_bVer3orGreater;

private:
	BOOL Initialise();
	BOOL Initialise(const CString& header);

//////////////////////////////////////////////////////////////////////////////
};

class CXmlNodeListWrapper
{
public:
	CXmlNodeListWrapper();
	CXmlNodeListWrapper(MSXML2::IXMLDOMNodeListPtr pList);
	virtual ~CXmlNodeListWrapper();

	MSXML2::IXMLDOMDocumentPtr AsDocument();
	MSXML2::IXMLDOMNodePtr Node(int index);
	void Start();
	MSXML2::IXMLDOMNodePtr Next();
	BOOL IsValid();
	int Count();
	void operator=(MSXML2::IXMLDOMNodeListPtr pList);

private:
	MSXML2::IXMLDOMNodeListPtr m_xmlnodelist;

	
};

//////////////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_XMLNODEWRAPPER_H__43622334_FDEB_4175_9E6D_19BBAA3992A5__INCLUDED_)
