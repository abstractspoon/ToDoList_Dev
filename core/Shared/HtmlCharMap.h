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
    { 150, _T("&#150;")  }, // '–'
    { 151, _T("&#151;")  }, // '—'
    { 153, _T("&#153;")  }, // '˜'

    // 160-255 Special Characters
    { 160, _T("&nbsp;")  }, // ' ' non-breaking space
    { 161, _T("&iexcl;") }, // '¡' inverted exclamation mark
    { 162, _T("&cent;")  }, // '¢' cent sign
    { 163, _T("&pound;") }, // '£' point sign
    { 164, _T("&curren;")}, // '¤' currency sing
    { 165, _T("&yen;")   }, // '¥' yen sign
    { 166, _T("&brvbar;")}, // '¦' broken vertical bar
    { 167, _T("&sect;")  }, // '§' section sign
    { 168, _T("&uml;")   }, // '¨' spacing diaeresis - umlaut
    { 169, _T("&copy;")  }, // '©' copyright sign
    { 170, _T("&ordf;")  }, // 'ª' femenine ordinal indicator
    { 171, _T("&laquo;") }, // '«' left double angle quotes
    { 172, _T("&not;")   }, // '¬' not sign 
    { 173, _T("&shy;")   }, // '­' soft hyphen
    { 174, _T("&reg;")   }, // '®' registered trade mark sign
    { 175, _T("&macr;")  }, // '¯' spacing macron - overline
    { 176, _T("%deg;")   }, // '°' degree sign
    { 177, _T("&plusmn;")}, // '±' plus-or-minus sign
    { 178, _T("&sup2;")  }, // '²' superscript two - squared
    { 179, _T("&sup3;")  }, // '³' superscript three - cubed
    { 180, _T("&acute;") }, // '´' acute accent - spacing acute
    { 181, _T("&micro;") }, // 'µ' micro sign
    { 182, _T("&para;")  }, // '¶' pilcrow sign - paragraph sign
    { 183, _T("&middot;")}, // '·' middle dot
    { 184, _T("&cedil;") }, // '¸' spacing cedilla
    { 185, _T("&sup1;")  }, // '¹' superscript one
    { 186, _T("&ordm;")  }, // 'º' masculine ordinal indicator
    { 187, _T("&raquo;") }, // '»' right double angle quotes
    { 188, _T("&frac14;")}, // '¼' fraction one quarter
    { 189, _T("&frac12;")}, // '½' fraction one half
    { 190, _T("&frac34;")}, // '¾' fraction three quarters
    { 191, _T("&iquest;")}, // '¿' inverted question mark
    { 192, _T("&Agrave;")}, // '?' latin capital letter A with grave
    { 193, _T("&Aacute;")}, // 'Á' latin capital letter A with acute
    { 194, _T("&Acirc;") }, // 'Â' latin capital letter A with circumflex
    { 195, _T("&Atilde;")}, // '?' latin capital letter A with tilde
    { 196, _T("&Auml;")  }, // 'Ä' latin capital letter A with diaeresis
    { 197, _T("&Aring;") }, // 'Å' latin capital letter A with ring above
    { 198, _T("&AElig;") }, // 'Æ' latin capital letter AE
    { 199, _T("&Ccedil;")}, // 'Ç' latin capital letter C with cedilla
    { 200, _T("&Egrave;")}, // 'È' latin capital letter E with grave
    { 201, _T("&Eacute;")}, // 'É' latin capital letter E with acute
    { 202, _T("&Ecirc;") }, // 'Ê'  latin capital letter E with circumflex
    { 203, _T("&Euml;")  }, // 'Ë' latin capital letter E with diaeresis
    { 204, _T("&Igrave;")}, // '?' latin capital letter I with grave
    { 205, _T("&Iacute;")}, // 'Í' latin capital letter I with acute
    { 206, _T("&Icirc;") }, // 'Î' latin capital letter I with circumflex
    { 207, _T("&Iuml;")  }, // 'Ï' latin capital letter I with diaeresis
    { 208, _T("&ETH;")   }, // '?' latin capital letter ETH
    { 209, _T("&Ntilde;")}, // 'Ñ' latin capital letter N with tilde
    { 210, _T("&Ograve;")}, // '?' latin capital letter O with grave
    { 211, _T("&Oacute;")}, // 'Ó' latin capital letter O with acute
    { 212, _T("&Ocirc;") }, // 'Ô' latin capital letter O with circumflex
    { 213, _T("&Otilde;")}, // '?' latin capital letter O with tilde
    { 214, _T("&Ouml;")  }, // 'Ö' latin capital letter U with diaeresis
    { 215, _T("&times;") }, // '×' multiplication sign
    { 216, _T("&Oslash;")}, // 'Ø' latin capital letter O with slash
    { 217, _T("&Ugrave;")}, // 'Ù' latin capital letter U with grave
    { 218, _T("&Uacute;")}, // 'Ú' latin capital letter U with acute
    { 219, _T("&Ucirc;") }, // 'Û' latin capital letter U with circumflex
    { 220, _T("&Uuml;")  }, // 'Ü' latin capital letter U with diaeresis
    { 221, _T("&Yacute;")}, // '?' latin capital letter Y with acute
    { 222, _T("&THORN;") }, // '?' latin capital letter THORN 
    { 223, _T("&szlig;") }, // 'ß' latin small letter sharp s - ess-zed
    { 224, _T("&agrave;")}, // 'à' latin small letter a with grave
    { 225, _T("&aacute;")}, // 'á' latin small letter a with acute
    { 226, _T("&acirc;") }, // 'â' latin small letter a with circumflex
    { 227, _T("&atilde;")}, // '?' latin small letter a with tilde
    { 228, _T("&auml;")  }, // 'ä' latin small letter a with diaeresis
    { 229, _T("&aring;") }, // 'å' latin small letter a with ring above
    { 230, _T("&aelig;") }, // 'æ' latin small letter ae
    { 231, _T("&ccedil;")}, // 'ç' latin small letter c with cedilla
    { 232, _T("&egrave;")}, // 'è' latin small letter e with grave
    { 233, _T("&eacute;")}, // 'é' latin small letter e with acute
    { 234, _T("&ecirc;") }, // 'ê' latin small letter e with circumflex
    { 235, _T("&euml;")  }, // 'ë' latin small letter e with diaeresis
    { 236, _T("&igrave;")}, // '?' latin small letter i with grave 
    { 237, _T("&iacute;")}, // 'í' latin small letter i with acute
    { 238, _T("&icirc;") }, // 'î' latin small letter i with circumflex
    { 239, _T("&iuml;")  }, // 'ï' latin small letter i with diaeresis
    { 240, _T("&eth;")   }, // '?' small eth, Icelandic
    { 241, _T("&ntilde;")}, // 'ñ' latin small letter n with tilde
    { 242, _T("&ograve;")}, // '?' latin small letter o with grave
    { 243, _T("&oacute;")}, // 'ó' latin small letter o with acute
    { 244, _T("&ocirc;") }, // 'ô' latin small letter o with circumflex
    { 245, _T("&otilde;")}, // '?' latin small letter o with tilde
    { 246, _T("&ouml;")  }, // 'ö' latin small letter o with diaeresis
    { 247, _T("&divide;")}, // '÷' division sign
    { 248, _T("&oslash;")}, // 'ø' latin small letter o with slash
    { 249, _T("&ugrave;")}, // 'ú' latin small letter u with acute
    { 250, _T("&uacute;")}, // 'ú' latin small letter u with acute
    { 251, _T("&ucirc;") }, // 'û' latin small letter u with circumflex
    { 252, _T("&uuml;")  }, // 'ü' latin small letter u with diaeresis
    { 253, _T("&yacute;")}, // '?' latin small letter y with acute
    { 254, _T("&thorn;") }, // '?' latin small letter thorn
    { 255, _T("&yuml;")  }, // 'ÿ' latin small letter y with diaeresis

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
			TCHAR c = sText[nChar];

#ifndef _UNICODE
			// don't translate multibyte chars
			if (IsDBCSLeadByte(c))
			{
				// add 2 chars to result because they're really
				// a single unicode char
				sResult += sText.Mid(nChar, 2);
				nChar++; // bump pos
			}
			else
#endif
				Translate(c, sResult);
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
