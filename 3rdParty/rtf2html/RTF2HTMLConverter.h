// RTF2HTMLConverter.h: interface for the CRTF_HTMLConverter class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RTF2HTMLCONVERTER_H__136CB6EA_55FC_4DBE_865E_753A7503A0DC__INCLUDED_)
#define AFX_RTF2HTMLCONVERTER_H__136CB6EA_55FC_4DBE_865E_753A7503A0DC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RTF2HTMLTree.h"

#include <afxtempl.h>

//! Performs RTF<->HTML Conversion
/*! 
At the moment, only RTF->HTML conversion is supported. 
Supported RTF options are poor and simple. Refer to the additional feature & bug list to see which
RTF options are implemented
*/
class CRTF_HTMLConverter : public CObject  
{
public:
   //! Conversion direction (RTF<->HTML)
   typedef enum {
      c_modRTF2HTML, c_modHTML2RTF
   } TMode;
   
   //! Construction
   /*! See TMode (Conversion Direction) */
	CRTF_HTMLConverter(TMode enMode);
   //! Destruction
	virtual ~CRTF_HTMLConverter();

   //! Streaming out into string (Data has already been converted)
   friend CString& operator<< ( CString& os, CRTF_HTMLConverter& dt );
   //! Streaming in of string and auto-perform conversion
   /*! Depending on conversion direction */
   friend CString& operator>> ( CString& is, CRTF_HTMLConverter& dt );
   
   static bool Convert(const CString& sRtf, CString& sHtml, BOOL bWantHeaderFooter = TRUE);
   static int GetCodePage(const CString& sRtf);
   static BOOL HasMultiByteChars(const CString& sRtf);

   //! Set target document title
   /*! This must be performed BEFORE streaming in regular file (and therefore converting it)*/
   void SetTitle(const CString& strTitle);
protected:
   //! One RTF element ini {} braces
   class CRTFNodeA {
   public:
      CString m_strName; //!< Name of node (primary attribute/RTF Tag)
      CString m_strCode; //!< The node's whole code (with sub-nodes)
      CString m_strThisCode; //!< the nodes code (without sub-nodes)
      CString m_strPlain; //!< The nodes plain text (without sub-nodes and attributes/RTF Tags)
   };

   //! A html element
   class CHTMLElement {
   public:
      //! Type if a html node
      typedef enum {
         c_nodHTMLBegin,  //!< e.g. <b>
         c_nodHTMLEnd,    //!< e.g. </b>
         c_nodText,       //!< e.g. "this is a test>
         c_nodInvalid     //!< temporary (initialized) state, no such nodes are written
      } THTMLNodeType;

      CHTMLElement(THTMLNodeType type = c_nodInvalid, LPCTSTR szText = NULL);
	  virtual ~CHTMLElement() {}
      THTMLNodeType m_enNodeType;
      CString       m_strNodeText;
      CMapStringToString m_mapParams; //!< Only relevant for m_enNodeType==c_nodHTMLBegin;
   };

   typedef Tree<CRTFNodeA> CRTFTree;
   typedef CRTFTree::Node CRTFNode;

   //! Conversion RTF->HTML main routine
   bool ConvertRTF2HTML(BOOL bWantHeaderFooter);
   //! Helper, builds up RTF tree (see m_RTFTree)
   CRTFNode R2H_BuildTree(const CString& strSource, CRTFNode* pNodeParent=NULL);
   //! Converts the HTML text (main node source) to HTML
   void R2H_CreateHTMLElements(const CString& strRTFSource);
   //! Gets the created HTML elements as HTML text
   void R2H_GetHTMLElements(CString& strHTML);
   //! In a given RTF string, this methods delivers the complete RTF tag starting at a distinct
   //! position
   CString R2H_GetRTFTag(const CString& strRTFSource, long lPos);
   //! Gets all RTF tags in a string as an array
   void R2H_GetRTFTags(const CString& strRTFSource, CStringArray& arrTgt);
   //! Resets the internal meta data structure
   /*! Meta data is data like charset etc.*/
   void ResetMetaData();   
   //! Fills up internal meta data structure
   void R2H_SetMetaData(CRTFNode& NodeRoot);
   //! Interpretes an RTF tag and writes the coresponding HTML Tag(s)
   void R2H_InterpretTag(const CString& strRTFTag);
   //! Gets the HTML Header
   CString R2H_GetHTMLHeader();
   //! Gets the HTML Footer
   CString R2H_GetHTMLFooter();
   
   //! Conversion direction (RTF<->HTML)
   TMode   m_enMode;
   //! RTF Code
   CString m_strRTF;
   //! HTML Code
   CString m_strHTML;
   //! The title of the document
   CString m_strTitle;
   //! Charset/Codepage of the document
   CString m_strCharset;
private:
   //! Internal RTF Tree
   /*! Each matching {} is a tree node. See CRTFNodeA for details*/
   CRTFTree* m_RTFTree;
   //! Parser actual font size
   CString m_strActFontSize;
   //! Parser actual font name
   CString m_strActFontName;
   //! Parser actual color
   CString m_strActFontColor;
   //! Font map table
   /*! Key: String(Font-Designator), e.g. "f0", "f1", ... -> Value: Font Name, e.g. "Arial", "Courier" */
   CMapStringToString m_mapFontNames;
   //! Array of colors (palette)
   /*! Colors do not have name (like fonts: "f0"), they are just sequentially ordered in a color table*/
   CArray<COLORREF, COLORREF&> m_arrColors;
   //! Array of created html nodes
   CArray<CHTMLElement*, CHTMLElement*> m_arrHTMLElements;
};    

#endif // !defined(AFX_RTF2HTMLCONVERTER_H__136CB6EA_55FC_4DBE_865E_753A7503A0DC__INCLUDED_)
