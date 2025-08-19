#if !defined(AFX_HTMLCHARMAP_H__7E73ADE2_3848_4ED1_9E8B_8881813B4262__INCLUDED_)
#define AFX_HTMLCHARMAP_H__7E73ADE2_3848_4ED1_9E8B_8881813B4262__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// HtmlCharMap.h: interface and implementation
//
// ///////////////////////////////////////////////////////////////////////////////

#include <afxtempl.h>

struct HTMLCHARMAPITEM
{
    int c; // the character required
    LPCTSTR szHtmlRep; // html representation
};

///
/// Standard ASCII set, HTML Entity names, ISO 10646, ISO 8879, ISO 8859-1 Latin alphabet No. 1
/// Browser support: All browsers (http://www.w3.org/TR/html401/sgml/entities.html)
///
static HTMLCHARMAPITEM HTMLCHARARRAY[] = 
{
    // Special Regular ASCII HTML Characters (under 127)
    { '&', _T("&amp;")   }, //     ampersand
    { '<', _T("&lt;")    }, //     less than sign
    { '>', _T("&gt;")    }, //     greater than sign
    { '\"', _T("&quot;") }, //     double quotes
    { '\'', _T("&#39;")  }, //     single quote

    // 128-159 Special Characters not defined in HTML4
    { 150, _T("&#150;")  }, // '�'
    { 151, _T("&#151;")  }, // '�'
    { 153, _T("&#153;")  }, // '�'

    // 160-255 Special Characters
    { 160, _T("&nbsp;")  }, // '�' non-breaking space
    { 161, _T("&iexcl;") }, // '�' inverted exclamation mark
    { 162, _T("&cent;")  }, // '�' cent sign
    { 163, _T("&pound;") }, // '�' point sign
    { 164, _T("&curren;")}, // '�' currency sing
    { 165, _T("&yen;")   }, // '�' yen sign
    { 166, _T("&brvbar;")}, // '�' broken vertical bar
    { 167, _T("&sect;")  }, // '�' section sign
    { 168, _T("&uml;")   }, // '�' spacing diaeresis - umlaut
    { 169, _T("&copy;")  }, // '�' copyright sign
    { 170, _T("&ordf;")  }, // '�' femenine ordinal indicator
    { 171, _T("&laquo;") }, // '�' left double angle quotes
    { 172, _T("&not;")   }, // '�' not sign 
    { 173, _T("&shy;")   }, // '�' soft hyphen
    { 174, _T("&reg;")   }, // '�' registered trade mark sign
    { 175, _T("&macr;")  }, // '�' spacing macron - overline
    { 176, _T("%deg;")   }, // '�' degree sign
    { 177, _T("&plusmn;")}, // '�' plus-or-minus sign
    { 178, _T("&sup2;")  }, // '�' superscript two - squared
    { 179, _T("&sup3;")  }, // '�' superscript three - cubed
    { 180, _T("&acute;") }, // '�' acute accent - spacing acute
    { 181, _T("&micro;") }, // '�' micro sign
    { 182, _T("&para;")  }, // '�' pilcrow sign - paragraph sign
    { 183, _T("&middot;")}, // '�' middle dot
    { 184, _T("&cedil;") }, // '�' spacing cedilla
    { 185, _T("&sup1;")  }, // '�' superscript one
    { 186, _T("&ordm;")  }, // '�' masculine ordinal indicator
    { 187, _T("&raquo;") }, // '�' right double angle quotes
    { 188, _T("&frac14;")}, // '�' fraction one quarter
    { 189, _T("&frac12;")}, // '�' fraction one half
    { 190, _T("&frac34;")}, // '�' fraction three quarters
    { 191, _T("&iquest;")}, // '�' inverted question mark
    { 192, _T("&Agrave;")}, // '?' latin capital letter A with grave
    { 193, _T("&Aacute;")}, // '�' latin capital letter A with acute
    { 194, _T("&Acirc;") }, // '�' latin capital letter A with circumflex
    { 195, _T("&Atilde;")}, // '?' latin capital letter A with tilde
    { 196, _T("&Auml;")  }, // '�' latin capital letter A with diaeresis
    { 197, _T("&Aring;") }, // '�' latin capital letter A with ring above
    { 198, _T("&AElig;") }, // '�' latin capital letter AE
    { 199, _T("&Ccedil;")}, // '�' latin capital letter C with cedilla
    { 200, _T("&Egrave;")}, // '�' latin capital letter E with grave
    { 201, _T("&Eacute;")}, // '�' latin capital letter E with acute
    { 202, _T("&Ecirc;") }, // '�'  latin capital letter E with circumflex
    { 203, _T("&Euml;")  }, // '�' latin capital letter E with diaeresis
    { 204, _T("&Igrave;")}, // '?' latin capital letter I with grave
    { 205, _T("&Iacute;")}, // '�' latin capital letter I with acute
    { 206, _T("&Icirc;") }, // '�' latin capital letter I with circumflex
    { 207, _T("&Iuml;")  }, // '�' latin capital letter I with diaeresis
    { 208, _T("&ETH;")   }, // '?' latin capital letter ETH
    { 209, _T("&Ntilde;")}, // '�' latin capital letter N with tilde
    { 210, _T("&Ograve;")}, // '?' latin capital letter O with grave
    { 211, _T("&Oacute;")}, // '�' latin capital letter O with acute
    { 212, _T("&Ocirc;") }, // '�' latin capital letter O with circumflex
    { 213, _T("&Otilde;")}, // '?' latin capital letter O with tilde
    { 214, _T("&Ouml;")  }, // '�' latin capital letter U with diaeresis
    { 215, _T("&times;") }, // '�' multiplication sign
    { 216, _T("&Oslash;")}, // '�' latin capital letter O with slash
    { 217, _T("&Ugrave;")}, // '�' latin capital letter U with grave
    { 218, _T("&Uacute;")}, // '�' latin capital letter U with acute
    { 219, _T("&Ucirc;") }, // '�' latin capital letter U with circumflex
    { 220, _T("&Uuml;")  }, // '�' latin capital letter U with diaeresis
    { 221, _T("&Yacute;")}, // '?' latin capital letter Y with acute
    { 222, _T("&THORN;") }, // '?' latin capital letter THORN 
    { 223, _T("&szlig;") }, // '�' latin small letter sharp s - ess-zed
    { 224, _T("&agrave;")}, // '�' latin small letter a with grave
    { 225, _T("&aacute;")}, // '�' latin small letter a with acute
    { 226, _T("&acirc;") }, // '�' latin small letter a with circumflex
    { 227, _T("&atilde;")}, // '?' latin small letter a with tilde
    { 228, _T("&auml;")  }, // '�' latin small letter a with diaeresis
    { 229, _T("&aring;") }, // '�' latin small letter a with ring above
    { 230, _T("&aelig;") }, // '�' latin small letter ae
    { 231, _T("&ccedil;")}, // '�' latin small letter c with cedilla
    { 232, _T("&egrave;")}, // '�' latin small letter e with grave
    { 233, _T("&eacute;")}, // '�' latin small letter e with acute
    { 234, _T("&ecirc;") }, // '�' latin small letter e with circumflex
    { 235, _T("&euml;")  }, // '�' latin small letter e with diaeresis
    { 236, _T("&igrave;")}, // '?' latin small letter i with grave 
    { 237, _T("&iacute;")}, // '�' latin small letter i with acute
    { 238, _T("&icirc;") }, // '�' latin small letter i with circumflex
    { 239, _T("&iuml;")  }, // '�' latin small letter i with diaeresis
    { 240, _T("&eth;")   }, // '?' small eth, Icelandic
    { 241, _T("&ntilde;")}, // '�' latin small letter n with tilde
    { 242, _T("&ograve;")}, // '?' latin small letter o with grave
    { 243, _T("&oacute;")}, // '�' latin small letter o with acute
    { 244, _T("&ocirc;") }, // '�' latin small letter o with circumflex
    { 245, _T("&otilde;")}, // '?' latin small letter o with tilde
    { 246, _T("&ouml;")  }, // '�' latin small letter o with diaeresis
    { 247, _T("&divide;")}, // '�' division sign
    { 248, _T("&oslash;")}, // '�' latin small letter o with slash
    { 249, _T("&ugrave;")}, // '�' latin small letter u with acute
    { 250, _T("&uacute;")}, // '�' latin small letter u with acute
    { 251, _T("&ucirc;") }, // '�' latin small letter u with circumflex
    { 252, _T("&uuml;")  }, // '�' latin small letter u with diaeresis
    { 253, _T("&yacute;")}, // '?' latin small letter y with acute
    { 254, _T("&thorn;") }, // '?' latin small letter thorn
    { 255, _T("&yuml;")  }, // '�' latin small letter y with diaeresis

};

const INT SIZEOFHTMLCHARARRAY = sizeof(HTMLCHARARRAY) / sizeof(HTMLCHARMAPITEM);

class CHtmlCharMap
{
public:
    static CString& ConvertFromRep(CString& sText)
    {
        CString sResult;

        // look for '&...;' pairs
        int nStart = sText.Find('&', 0);

        while (nStart != -1)
        {
            int nEnd = sText.Find(';', nStart);

            if (nEnd != -1)
            {
                sResult += sText.Left(nStart);

                CString sRep = sText.Mid(nStart, nEnd - nStart + 1);
                Translate(sRep, sResult);

                sText = sText.Mid(nEnd + 1);
                nStart = sText.Find('&', 0);
            }
            else
                break;
        }

        // add whatevers left
        sResult += sText;

        sText = sResult;
        return sText;
    }

    static CString& ConvertToRep(CString& sText)
    {
        CString sResult;

        for (int nChar = 0; nChar < sText.GetLength(); nChar++)
		{
			Translate(sText[nChar], sResult);
		}

        sText = sResult;
        return sText;
    }

protected:
    static void Translate(LPCTSTR szHtmlRep, CString& sAppendTo)
    {
        static CMap<CString, LPCTSTR, int, int&> mapHtmlRep;

        // init map once only
        if (!mapHtmlRep.GetCount())
        {
            int nItem = SIZEOFHTMLCHARARRAY;

            while (nItem--)
                mapHtmlRep.SetAt(HTMLCHARARRAY[nItem].szHtmlRep, HTMLCHARARRAY[nItem].c);
        }

        int c = 0;

        if (mapHtmlRep.Lookup(szHtmlRep, c))
            sAppendTo += (TCHAR)c;
        else
            sAppendTo += szHtmlRep;
    }


    static void Translate(int c, CString& sAppendTo)
    {
        static CMap<int, int, CString, LPCTSTR> mapChar;

        // init map once only
        if (!mapChar.GetCount())
        {
            int nItem = SIZEOFHTMLCHARARRAY;

            while (nItem--)
                mapChar.SetAt(HTMLCHARARRAY[nItem].c, HTMLCHARARRAY[nItem].szHtmlRep);
        }

        CString sHtmlRep;

        if (mapChar.Lookup(c, sHtmlRep))
            sAppendTo += sHtmlRep;
        else
            sAppendTo += (TCHAR)c;
    }
};


#endif // AFX_HTMLCHARMAP_H__7E73ADE2_3848_4ED1_9E8B_8881813B4262__INCLUDED_
