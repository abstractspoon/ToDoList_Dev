// XmlNodeWrapper.cpp: implementation of the CXmlNodeWrapper class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XmlNodeWrapper.h"
#include "comutil.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////

#define STR2BSTR(s) _bstr_t(CXmlNodeWrapper::ConvertStringToBSTR(s), false)

//////////////////////////////////////////////////////////////////////

// fix for bad typedef in <wtypes.h>
#ifdef VARIANT_TRUE
#	undef VARIANT_TRUE
#	define VARIANT_TRUE ((VARIANT_BOOL)(-1))
#endif

//////////////////////////////////////////////////////////////////////

LPCTSTR DEFAULT_HEADER = _T("version=\"1.0\" encoding=\"windows-1252\"");

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CXmlNodeWrapper::CXmlNodeWrapper()
{
}

CXmlNodeWrapper::CXmlNodeWrapper(MSXML2::IXMLDOMNodePtr pNode,BOOL bAutoRelease)
{
	m_xmlnode = pNode;
	m_bAutoRelease = bAutoRelease;
}

CXmlNodeWrapper& CXmlNodeWrapper::operator=(MSXML2::IXMLDOMNodePtr pNode)
{
	if (IsValid())
		m_xmlnode.Release();

	m_xmlnode = pNode;
	return *this;
}

CXmlNodeWrapper::~CXmlNodeWrapper()
{
	if (!m_bAutoRelease)
		m_xmlnode.Detach();
}

CString CXmlNodeWrapper::GetValue(const CString& valueName)
{
	if (!IsValid())
		return _T("");
	
	MSXML2::IXMLDOMNodePtr attribute = m_xmlnode->Getattributes()->getNamedItem(STR2BSTR(valueName));
	if (attribute)
	{
		return (LPCTSTR)attribute->Gettext();
	}
	return _T("");
}

BOOL CXmlNodeWrapper::IsValid()
{
	if (m_xmlnode == NULL)
		return FALSE;
	if (m_xmlnode.GetInterfacePtr() == NULL)
		return FALSE;
   
	return TRUE;
}

MSXML2::IXMLDOMNodePtr CXmlNodeWrapper::GetPrevSibling()
{
	if (!IsValid())
		return NULL;
	return m_xmlnode->GetpreviousSibling();
}

MSXML2::IXMLDOMNodePtr CXmlNodeWrapper::GetNextSibling()
{
	if (!IsValid())
		return NULL;
	return m_xmlnode->GetnextSibling();
}

MSXML2::IXMLDOMNodePtr CXmlNodeWrapper::GetNode(const CString& nodeName)
{
	if (!IsValid())
		return NULL;
	try{
		return m_xmlnode->selectSingleNode(STR2BSTR(nodeName));
	}
	catch (_com_error e)
	{
		CString err = e.ErrorMessage();
	}
	return NULL;
}

MSXML2::IXMLDOMNodePtr CXmlNodeWrapper::GetNode(int nodeIndex)
{
	if (!IsValid())
		return NULL;
	
	return m_xmlnode->GetchildNodes()->Getitem(nodeIndex);
}

MSXML2::IXMLDOMNodePtr CXmlNodeWrapper::GetFirstChildNode()
{
	if (!IsValid())
		return NULL;
	
	return m_xmlnode->GetfirstChild();
}

MSXML2::IXMLDOMNodePtr CXmlNodeWrapper::FindNode(const CString& searchString)
{
	if (!IsValid())
		return NULL;

	try
	{
		return m_xmlnode->selectSingleNode(STR2BSTR(searchString));
	}
	catch (_com_error e)
	{
		CString err = e.ErrorMessage();
	}
	return NULL;
}

MSXML2::IXMLDOMNode* CXmlNodeWrapper::Detach()
{
	if (IsValid())
		return m_xmlnode.Detach();
	else
		return NULL;
}

long CXmlNodeWrapper::NumNodes()
{
	if (IsValid())
	{
		return m_xmlnode->GetchildNodes()->Getlength();
	}
	else
		return 0;
}

void CXmlNodeWrapper::SetValue(const CString& valueName,const CString& value)
{
	MSXML2::IXMLDOMDocumentPtr xmlDocument = m_xmlnode->GetownerDocument();

	if (xmlDocument)
	{
		MSXML2::IXMLDOMNamedNodeMapPtr attributes = m_xmlnode->Getattributes();

		if (attributes)
		{
			MSXML2::IXMLDOMAttributePtr attribute = xmlDocument->createAttribute(STR2BSTR(valueName));
			
			if (attribute)
			{
				attribute->Puttext(STR2BSTR(value));
				attributes->setNamedItem(attribute);
			}
		}
	}
}

void CXmlNodeWrapper::SetValue(const CString& valueName,int value)
{
	CString str;
	str.Format(_T("%ld"),value);
	SetValue(valueName,str);
}

void CXmlNodeWrapper::SetValue(const CString& valueName,short value)
{
	CString str;
	str.Format(_T("%hd"),value);
	SetValue(valueName,str);
}

void CXmlNodeWrapper::SetValue(const CString& valueName,double value)
{
	CString str;
	str.Format(_T("%f"),value);
	SetValue(valueName,str);
}

void CXmlNodeWrapper::SetValue(const CString& valueName,float value)
{
	CString str;
	str.Format(_T("%f"),value);
	SetValue(valueName,str);
}

void CXmlNodeWrapper::SetValue(const CString& valueName,bool value)
{
	CString str;
	if (value)
		str = _T("True");
	else
		str = _T("False");
	SetValue(valueName,str);
}

MSXML2::IXMLDOMNodePtr CXmlNodeWrapper::AppendChild(MSXML2::IXMLDOMNodePtr pNode)
{
	return m_xmlnode->appendChild(pNode);
}

MSXML2::IXMLDOMNodePtr CXmlNodeWrapper::InsertNode(int index, const CString& nodeName, const CString& nameSpace)
{
	MSXML2::IXMLDOMDocumentPtr xmlDocument = m_xmlnode->GetownerDocument();

	if (xmlDocument)
	{
		MSXML2::IXMLDOMNodePtr newNode = xmlDocument->createNode(_variant_t((short)MSXML2::NODE_ELEMENT),STR2BSTR(nodeName), STR2BSTR(nameSpace));
		MSXML2::IXMLDOMNodePtr refNode = GetNode(index);

		if (refNode)
			newNode = m_xmlnode->insertBefore(newNode,_variant_t(refNode.GetInterfacePtr()));
		else
			newNode = m_xmlnode->appendChild(newNode);

		return newNode;
	}
	return NULL;
}

MSXML2::IXMLDOMNodePtr CXmlNodeWrapper::InsertNode(int index, MSXML2::IXMLDOMNodePtr pNode)
{
	MSXML2::IXMLDOMNodePtr newNode = pNode->cloneNode(VARIANT_TRUE);

	if (newNode)
	{
		MSXML2::IXMLDOMNodePtr refNode = GetNode(index);

		if (refNode)
			newNode = m_xmlnode->insertBefore(newNode,_variant_t(refNode.GetInterfacePtr()));
		else
			newNode = m_xmlnode->appendChild(newNode);

		return newNode;
	}
	else
		return NULL;
}

CString CXmlNodeWrapper::GetXML()
{
	if (IsValid())
		return (LPCTSTR)m_xmlnode->Getxml();
	else
		return "";
}

MSXML2::IXMLDOMNodePtr CXmlNodeWrapper::RemoveNode(MSXML2::IXMLDOMNodePtr pNode)
{
	if (!IsValid())
		return NULL;
	return m_xmlnode->removeChild(pNode);
}

/* ********************************************************************************************************* */

BOOL CXmlDocumentWrapper::s_bVer3orGreater = -1;

CXmlDocumentWrapper::CXmlDocumentWrapper(BOOL bInitialise)
	: m_bHeaderSet(FALSE)
{
	if (bInitialise)
		VERIFY(Initialise());
}

CXmlDocumentWrapper::CXmlDocumentWrapper(const CString& header)
	: m_bHeaderSet(FALSE)
{
	VERIFY(Initialise(header));
}

CXmlDocumentWrapper::CXmlDocumentWrapper(const CString& sHeader, const CString& sRootItem) 
	: m_bHeaderSet(FALSE), m_sRootItemName(sRootItem)
{
	VERIFY(Initialise(sHeader));
}

BOOL CXmlDocumentWrapper::Initialise()
{
	if (IsValid())
	{
		ASSERT(0);
		return FALSE;
	}

	try
	{
		m_xmldoc.CreateInstance(MSXML2::CLSID_DOMDocument);
		m_xmldoc->put_validateOnParse(VARIANT_FALSE);
		m_xmldoc->put_async(VARIANT_FALSE);
		
		Reset();
		return TRUE;
	}
	catch (...)
	{
	}

	return FALSE;
}

BOOL CXmlDocumentWrapper::Initialise(const CString& header, const CString& sRootItem)
{
	CString sPrevRootItem = m_sRootItemName;
	m_sRootItemName = sRootItem; // because Initialise wants it to be set

	if (!Initialise())
	{
		m_sRootItemName = sPrevRootItem;
		return FALSE;
	}

	if (!header.IsEmpty())
		SetXmlHeader(header);

	return TRUE;
}

CXmlDocumentWrapper::CXmlDocumentWrapper(MSXML2::IXMLDOMDocumentPtr pDoc)
{
	m_xmldoc = pDoc;
}

CXmlDocumentWrapper::~CXmlDocumentWrapper()
{
	Release();
}

void CXmlDocumentWrapper::Release()
{
	if (m_xmldoc != NULL)
		m_xmldoc.Release();
}

void CXmlDocumentWrapper::Reset()
{
	if (IsValid())
	{
		if (!m_sRootItemName.IsEmpty())
		{
			CString sRoot;
			sRoot.Format(_T("<%s/>"), m_sRootItemName); 
			LoadXML(sRoot);
		}
		else
		{
			m_xmldoc->childNodes->reset();
		}
		
		m_bHeaderSet = FALSE;
	}
}

BOOL CXmlDocumentWrapper::IsValid() const
{
	if (m_xmldoc == NULL)
		return FALSE;

	if (m_xmldoc.GetInterfacePtr() == NULL)
		return FALSE;

	return TRUE;
}

CString CXmlDocumentWrapper::GetXmlHeader(BOOL bAsXml) const
{
	return GetHeader(_T(""), bAsXml);
}

CString CXmlDocumentWrapper::GetXslHeader(BOOL bAsXml) const
{
	return GetHeader(_T("stylesheet"), bAsXml);
}

CString CXmlDocumentWrapper::GetHeader(const CString& sName, BOOL bAsXml) const
{
	CString sHeader;
	
	if (IsValid())
	{
		BOOL bRootItem = sName.IsEmpty();
		long nItem = FindHeaderItem(sName);

		if (nItem != -1)
		{
			CXmlNodeWrapper nodeHdr(m_xmldoc->childNodes->item[nItem]);
		
			if (nodeHdr.IsValid())
			{
				sHeader = nodeHdr.GetXML();

				// remove the xml start/end 
				sHeader.TrimLeft(_T("<? "));
				sHeader.TrimRight(_T(">? "));
				sHeader = sHeader.Mid(4); // 'xml'

				// remove name
				if (!bRootItem)
					sHeader = sHeader.Mid(sName.GetLength());

				sHeader.TrimLeft();
				sHeader.TrimRight();
			}
		
			// Fallback for root header item
			if (sHeader.IsEmpty() && bRootItem)
			{
				sHeader = DEFAULT_HEADER;
			
				// get active code page
				CString sCodePage;
			
				GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_IDEFAULTANSICODEPAGE, sCodePage.GetBuffer(7), 6);
				sCodePage.ReleaseBuffer();
			
				// and replace in header
				if (_ttoi(sCodePage) > 0)
					sHeader.Replace(_T("1252"), sCodePage);
			}
		
			if (bAsXml && !sHeader.IsEmpty())
			{
				if (bRootItem)
					sHeader = _T("<?xml ") + sHeader;
				else
					sHeader = _T("<?xml-") + sHeader;

				sHeader += _T("?>");
			}

		}
	}
	
	return sHeader;
}

long CXmlDocumentWrapper::FindHeaderItem(const CString& sName) const
{
	if (IsValid())
	{
		long nNumNodes = m_xmldoc->childNodes->Getlength();

		for (long nNode = 0; nNode < nNumNodes; nNode++)
		{
			CXmlNodeWrapper node(m_xmldoc->childNodes->item[nNode]);
			
			if (node.IsValid())
			{
				CString sNodeXml(node.GetXML());
				CString sHeaderItem(_T("?xml"));

				if (!sName.IsEmpty())
				{
					sHeaderItem += '-';
					sHeaderItem += sName;
				}
				
				if (sNodeXml.Find(sHeaderItem) == 1)
				{
					return nNode;
				}
			}
		}
	}

	// all else
	return -1;
}

BOOL CXmlDocumentWrapper::SetXmlHeader(const CString& sHeader)
{
	ASSERT(IsValid());
	ASSERT(!sHeader.IsEmpty());
	ASSERT(!m_bHeaderSet);

	if (IsValid() && !sHeader.IsEmpty() && !m_bHeaderSet)
	{
		// Delete any existing 'xml' processing instructions
		MSXML2::IXMLDOMNodePtr pNode = m_xmldoc->childNodes->item[0];

		while (pNode)
		{
			if ((int)pNode->GetnodeType() != MSXML2::NODE_PROCESSING_INSTRUCTION)
				break;

			CString sName = (LPCTSTR)pNode->GetnodeName();

			if (sName != _T("xml"))
				break;

			pNode->GetparentNode()->removeChild(pNode);
			pNode = m_xmldoc->childNodes->item[0];
		}

		// always insert header right at the start
		MSXML2::IXMLDOMProcessingInstructionPtr pHdr = m_xmldoc->createProcessingInstruction(STR2BSTR(_T("xml")), STR2BSTR(sHeader));
		
		if (pNode)
		{
			_variant_t varFirst(pNode.GetInterfacePtr());
			m_xmldoc->insertBefore(pHdr, varFirst);
		}
		else
		{
			m_xmldoc->appendChild(pHdr);
		}

		m_bHeaderSet = TRUE;
	}

	return m_bHeaderSet;
}

BOOL CXmlDocumentWrapper::SetXslHeader(const CString& sHeader)
{
	ASSERT(IsValid());

	if (IsValid())
	{
		if (sHeader.IsEmpty())
		{
			// delete the xsl header
			long nItem = FindHeaderItem(_T("stylesheet"));

			if (nItem != -1)
			{
				// TODO
			}
		}
		else
		{
			_bstr_t name(STR2BSTR(_T("xml-stylesheet")));
			_bstr_t bstr(STR2BSTR(sHeader));
		
			MSXML2::IXMLDOMProcessingInstructionPtr pHdr = m_xmldoc->createProcessingInstruction(name, bstr);
		
			// always insert header right at the start but after root header
			long nItem = (FindHeaderItem() + 1);
			MSXML2::IXMLDOMNodePtr pNode = m_xmldoc->childNodes->item[nItem];
		
			if (pNode)
			{
				CString sXml = (LPCTSTR)pNode->Getxml();
				_variant_t varFirst(pNode.GetInterfacePtr());
			
				m_xmldoc->insertBefore(pHdr, varFirst);
			}
			else
			{
				m_xmldoc->appendChild(pHdr);
			}
		}

		return TRUE;
	}

	return FALSE;
}

MSXML2::IXMLDOMDocument* CXmlDocumentWrapper::Detach()
{
	if (!IsValid())
		return NULL;

	return m_xmldoc.Detach();
}

MSXML2::IXMLDOMDocumentPtr CXmlDocumentWrapper::Clone()
{
	if (!IsValid())
		return NULL;

	MSXML2::IXMLDOMDocumentPtr xmldoc;
	xmldoc.CreateInstance(MSXML2::CLSID_DOMDocument);

	_variant_t v(xmldoc.GetInterfacePtr());
	m_xmldoc->save(v);

	return xmldoc;
}

MSXML2::IXMLDOMDocument* CXmlDocumentWrapper::Interface()
{
	if (IsValid())
		return m_xmldoc;

	return NULL;
}

BOOL CXmlDocumentWrapper::Load(const CString& path, BOOL bPreserveWhiteSpace)
{
	if (!IsValid())
		return FALSE;
	
	m_xmldoc->put_preserveWhiteSpace(bPreserveWhiteSpace ? VARIANT_TRUE : VARIANT_FALSE);
	m_xmldoc->put_async(VARIANT_FALSE);
	
	return (VARIANT_TRUE == m_xmldoc->load(STR2BSTR(path)));
}

BOOL CXmlDocumentWrapper::LoadXML(const CString& xml, BOOL bPreserveWhiteSpace)
{
	if (!IsValid())
		return FALSE;
	
	m_xmldoc->put_preserveWhiteSpace(bPreserveWhiteSpace ? VARIANT_TRUE : VARIANT_FALSE);
	m_xmldoc->put_async(VARIANT_FALSE);

	return (VARIANT_TRUE == m_xmldoc->loadXML(STR2BSTR(xml)));
}

BOOL CXmlDocumentWrapper::Save(const CString& path, BOOL bPreserveWhiteSpace)
{
	if (!IsValid())
		return FALSE;
	
	try
	{
		_bstr_t bPath(STR2BSTR(path));
		
		if (bPath.length() == 0)
			bPath = m_xmldoc->Geturl();
		
		m_xmldoc->put_preserveWhiteSpace(bPreserveWhiteSpace ? VARIANT_TRUE : VARIANT_FALSE);
		m_xmldoc->put_async(VARIANT_FALSE);

		if (!m_bHeaderSet)
		{
			ASSERT(0);
			SetXmlHeader(DEFAULT_HEADER);
		}
		
		return (VARIANT_TRUE == m_xmldoc->save(bPath));
	}
	catch(...)
	{
		return FALSE;
	}
}

BOOL CXmlDocumentWrapper::Transform(CXmlDocumentWrapper& xsl, CString& sOutput) const
{
	sOutput.Empty();

    try
    {
        sOutput = (LPTSTR)m_xmldoc->transformNode(xsl.AsNode());
    }
    catch( const _com_error & err)
    {
        AfxMessageBox(err.ErrorMessage(), MB_OK | MB_ICONERROR);
    }

	return !sOutput.IsEmpty();
}

MSXML2::IXMLDOMNodePtr CXmlDocumentWrapper::AsNode()
{
	if (!IsValid())
		return NULL;

	return m_xmldoc->GetdocumentElement();
}

CString CXmlDocumentWrapper::GetXML(BOOL bPreserveWhiteSpace) const
{
	CString sXml;
	
	if (IsValid())
	{
		m_xmldoc->put_preserveWhiteSpace(bPreserveWhiteSpace ? VARIANT_TRUE : VARIANT_FALSE);
		m_xmldoc->put_async(VARIANT_FALSE);
		
		sXml = (LPCTSTR)m_xmldoc->Getxml();
		
		// Getxml doesn't return entire header so we need to fix it up here
		CString sHeader = GetXmlHeader(TRUE);
		int nStart = sXml.Find(_T("?xml"));
		
		if (nStart > 0)
		{
			// find start brace
			while (sXml[nStart] != '<')
				nStart--;
			
			// find end brace
			int nEnd = sXml.Find('>') + 1; // closing brace
			
			sXml = sXml.Left(nStart) + sHeader + sXml.Mid(nEnd);
		}
		else // just add header
			sXml = sHeader + sXml;
	}
	
	return sXml;
}

CString CXmlDocumentWrapper::GetUrl() const
{
	return (LPTSTR)m_xmldoc->Geturl();
}

BOOL CXmlDocumentWrapper::IsVersion3orGreater()
{
	if (s_bVer3orGreater == -1)
	{
		try
		{
			// try to load some dummy xml
			CXmlDocumentWrapper doc;
			CString sXml;

			sXml.Format(_T("<?xml %s ?><NodeList/>"), DEFAULT_HEADER);
			s_bVer3orGreater = doc.LoadXML(sXml);
		}
		catch (CException* /*e*/)
		{
			s_bVer3orGreater = FALSE;
		}
	}

	return s_bVer3orGreater;
}

//////////////////////////////////////////////////////////////////////////

//------------------------//
// Convert char * to BSTR //
//------------------------//
BSTR CXmlNodeWrapper::ConvertStringToBSTR(const CString& src)
{
	return ::SysAllocStringLen((LPCWSTR)src, src.GetLength());
};

//////////////////////////////////////////////////////////////////////////////

MSXML2::IXMLDOMDocumentPtr CXmlNodeWrapper::ParentDocument()
{
	return m_xmlnode->GetownerDocument();
}

BOOL CXmlNodeWrapper::IsPreservingWhiteSpace() const
{
	VARIANT_BOOL bPreserving = VARIANT_FALSE;
	m_xmlnode->GetownerDocument()->get_preserveWhiteSpace(&bPreserving);
	
	return (bPreserving == VARIANT_TRUE);
}

MSXML2::IXMLDOMNode* CXmlNodeWrapper::Interface()
{
	if (IsValid())
		return m_xmlnode;

	return NULL;
}

MSXML2::IXMLDOMNodePtr CXmlNodeWrapper::InsertBefore(MSXML2::IXMLDOMNodePtr refNode, const CString& nodeName)
{
	MSXML2::IXMLDOMDocumentPtr xmlDocument = m_xmlnode->GetownerDocument();
	if (xmlDocument)
	{
		MSXML2::IXMLDOMNodePtr newNode = xmlDocument->createNode(_variant_t((short)MSXML2::NODE_ELEMENT),STR2BSTR(nodeName),"");
		newNode = m_xmlnode->insertBefore(newNode,_variant_t(refNode.GetInterfacePtr()));

		return newNode;
	}
	return NULL;
}

MSXML2::IXMLDOMNodePtr CXmlNodeWrapper::InsertAfter(MSXML2::IXMLDOMNodePtr refNode, const CString& nodeName)
{
	MSXML2::IXMLDOMDocumentPtr xmlDocument = m_xmlnode->GetownerDocument();
	if (xmlDocument)
	{
		MSXML2::IXMLDOMNodePtr newNode = xmlDocument->createNode(_variant_t((short)MSXML2::NODE_ELEMENT),STR2BSTR(nodeName),"");
		MSXML2::IXMLDOMNodePtr nextNode = refNode->GetnextSibling();

		if (nextNode.GetInterfacePtr() != NULL)
			newNode = m_xmlnode->insertBefore(newNode,_variant_t(nextNode.GetInterfacePtr()));
		else
			newNode = m_xmlnode->appendChild(newNode);

		return newNode;
	}
	return NULL;
}

void CXmlNodeWrapper::Reset()
{
	if (!IsValid())
		return;

	MSXML2::IXMLDOMNodeListPtr nodeList = m_xmlnode->GetchildNodes();
//	ASSERT(nodeList->);

	VERIFY(SUCCEEDED(nodeList->reset()));
}

void CXmlNodeWrapper::RemoveNodes(const CString& searchStr)
{
	if (!IsValid())
		return;

	MSXML2::IXMLDOMNodeListPtr nodeList = m_xmlnode->selectNodes(STR2BSTR(searchStr));
	for (int i = 0; i < nodeList->Getlength(); i++)
	{
		try
		{
			MSXML2::IXMLDOMNodePtr pNode = nodeList->Getitem(i);
			pNode->GetparentNode()->removeChild(pNode);
		}
		catch (_com_error er)
		{
			AfxMessageBox(er.ErrorMessage());
		}
	}
}

MSXML2::IXMLDOMNodePtr CXmlNodeWrapper::Parent()
{
	if (IsValid())
		return m_xmlnode->GetparentNode();

	return NULL;
}

CXmlNodeListWrapper::CXmlNodeListWrapper()
{
}
CXmlNodeListWrapper::CXmlNodeListWrapper(MSXML2::IXMLDOMNodeListPtr pList)
{
	m_xmlnodelist = pList;
}

void CXmlNodeListWrapper::operator=(MSXML2::IXMLDOMNodeListPtr pList)
{
	if (IsValid())
		m_xmlnodelist.Release();
	
	m_xmlnodelist = pList;
}

CXmlNodeListWrapper::~CXmlNodeListWrapper()
{
	
}

int CXmlNodeListWrapper::Count()
{
	if (IsValid())
		return m_xmlnodelist->Getlength();
	else
		return 0;
}

BOOL CXmlNodeListWrapper::IsValid()
{
	if (m_xmlnodelist == NULL)
		return FALSE;

	if (m_xmlnodelist.GetInterfacePtr() == NULL)

		return FALSE;
	return TRUE;
}

MSXML2::IXMLDOMNodePtr CXmlNodeListWrapper::Next()
{
	if (IsValid())
		return m_xmlnodelist->nextNode();
	else
		return NULL;
}

void CXmlNodeListWrapper::Start()
{
	if (IsValid())
		m_xmlnodelist->reset();
}

MSXML2::IXMLDOMNodePtr CXmlNodeListWrapper::Node(int index)
{
	if (IsValid())
		return m_xmlnodelist->Getitem(index);
	else
		return NULL;
}

MSXML2::IXMLDOMDocumentPtr CXmlNodeListWrapper::AsDocument()
{
	if (IsValid())
	{
		CXmlDocumentWrapper doc;
		doc.LoadXML(_T("<NodeList></NodeList>"));
		
		CXmlNodeWrapper root(doc.AsNode());
		
		for (int i = 0; i < m_xmlnodelist->Getlength(); i++)
		{
			root.InsertNode(root.NumNodes(),m_xmlnodelist->Getitem(i)->cloneNode(VARIANT_TRUE));
		}
		return doc.Interface();
	}
	else 
		return NULL;
}

MSXML2::IXMLDOMNodeListPtr CXmlNodeWrapper::FindNodes(const CString& searchStr)
{
	if(IsValid())
	{
		try
		{
			return m_xmlnode->selectNodes(STR2BSTR(searchStr));
		}
		catch (_com_error e)
		{
			CString err = e.ErrorMessage();
			return NULL;
		}
	}
	else
		return NULL;
}

CString CXmlNodeWrapper::Name()
{
	if (IsValid())
		return (LPCTSTR)m_xmlnode->GetbaseName();
	return "";
}

MSXML2::IXMLDOMNodePtr CXmlNodeWrapper::InsertAfter(MSXML2::IXMLDOMNodePtr refNode, MSXML2::IXMLDOMNodePtr pNode)
{
	MSXML2::IXMLDOMNodePtr nextNode = refNode->GetnextSibling();
	MSXML2::IXMLDOMNodePtr newNode;

	if (nextNode.GetInterfacePtr() != NULL)
		newNode = m_xmlnode->insertBefore(pNode,_variant_t(nextNode.GetInterfacePtr()));
	else
		newNode = m_xmlnode->appendChild(pNode);

	return newNode;
}

void CXmlNodeWrapper::SetText(const CString& text)
{
	if (IsValid())
	{
		m_xmlnode->Puttext(STR2BSTR(text));
	}
}

CString CXmlNodeWrapper::GetText()
{
	if (IsValid())
		return (LPCTSTR)m_xmlnode->Gettext();
	else
		return "";
}

void CXmlNodeWrapper::ReplaceNode(MSXML2::IXMLDOMNodePtr pOldNode, MSXML2::IXMLDOMNodePtr pNewNode)
{
	if (IsValid())
	{
		m_xmlnode->replaceChild(pNewNode,pOldNode);
	}
}

int CXmlNodeWrapper::NumAttributes()
{
	if (IsValid())
	{
		MSXML2::IXMLDOMNamedNodeMapPtr attribs = m_xmlnode->Getattributes();
		
		if (attribs)
			return attribs->Getlength();
	}
	
	return 0;
}

CString CXmlNodeWrapper::GetAttribName(int index)
{
	CString sName;
	
	if (IsValid())
	{
		MSXML2::IXMLDOMNamedNodeMapPtr attribs = m_xmlnode->Getattributes();

		if (attribs)
		{
			MSXML2::IXMLDOMAttributePtr attrib = attribs->Getitem(index);

			if (attrib)
			{
				sName = (LPCTSTR)attrib->Getname();
				attrib.Release();
			}
			
			attribs.Release();
		}
	}
	
	return sName;
}

CString CXmlNodeWrapper::GetAttribVal(int index)
{
	CString sVal;
	
	if (IsValid())
	{
		MSXML2::IXMLDOMNamedNodeMapPtr attribs = m_xmlnode->Getattributes();

		if (attribs)
		{
			MSXML2::IXMLDOMAttributePtr attrib = attribs->Getitem(index);

			if (attrib)
			{
				sVal = (LPCTSTR)attrib->Gettext();
				attrib.Release();
			}
			
			attribs.Release();
		}
	}
	
	
	return sVal;
}

int CXmlNodeWrapper::GetAttributes(CStringArray& aNames, CStringArray& aValues)
{
	aNames.RemoveAll();
	aValues.RemoveAll();

	if (IsValid())
	{
		MSXML2::IXMLDOMNamedNodeMapPtr attribs = m_xmlnode->Getattributes();

		if (attribs)
		{
			int nNumAttrib = attribs->Getlength();

			for (int nAtt = 0; nAtt < nNumAttrib; nAtt++)
			{
				MSXML2::IXMLDOMAttributePtr attrib = attribs->Getitem(nAtt);

				if (attrib)
				{
					aNames.Add((LPCTSTR)attrib->Getname());
					aValues.Add((LPCTSTR)attrib->Gettext());
					attrib.Release();
				}
			}
		
			attribs.Release();
		}
	}

	ASSERT(aNames.GetSize() == aValues.GetSize());

	return aNames.GetSize();
}

CString CXmlNodeWrapper::NodeType()
{
	if (IsValid())
		return (LPCTSTR)m_xmlnode->GetnodeTypeString();

	return _T("");
}

int CXmlNodeWrapper::NodeTypeVal()
{
	if (IsValid())
		return (int)m_xmlnode->GetnodeType();

	// Valik - Fully qualify the name of NODE_INVALID to prevent an ambiguous symbol error (C2872) in VC 7.1
	return MSXML2::NODE_INVALID;
}

