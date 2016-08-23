



// RoundCorner.h: interface for the CRoundCorner class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ROUNDCORNER_H__5CCBE3A0_97AD_11D5_95AD_91F9BAF50F32__INCLUDED_)
#define AFX_ROUNDCORNER_H__5CCBE3A0_97AD_11D5_95AD_91F9BAF50F32__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

enum 
{ 
	RC_TOPLEFT 		= 0x0001, 
	RC_TOPRIGHT		= 0x0002, 
	RC_BOTTOMLEFT	= 0x0004, 
	RC_BOTTOMRIGHT	= 0x0008,

	RC_LEFT			= RC_TOPLEFT | RC_BOTTOMLEFT, 
	RC_TOP			= RC_TOPLEFT | RC_TOPRIGHT, 
	RC_RIGHT		= RC_TOPRIGHT | RC_BOTTOMRIGHT,
	RC_BOTTOM		= RC_BOTTOMLEFT | RC_BOTTOMRIGHT, 
	RC_ALL			= RC_LEFT | RC_RIGHT,
};

enum
{
	RC_NORMAL		= 0x0000, // standard windows 2-pixel wide 
	RC_THIN			= 0x0010, // single pixel wide
	RC_RENDER		= 0x0020, // full 3d style. requires RC_EDGES too.
	RC_EDGES		= 0x0040, // draw lines between corners if there is any space
	RC_DOWN			= 0x0100, // reverses the colors
	RC_UP			= 0x0200,
//	RC_DEFBTN		= 0x0400,
};

struct Corner;

class CRoundCorner
{
public:
	CRoundCorner(CWnd* pWnd, int nRadius, COLORREF crBkgnd = -1);
	virtual ~CRoundCorner();

	void Draw(int nCorners = RC_ALL, DWORD dwStyle = RC_EDGES | RC_THIN | RC_UP);
	static void Draw(CDC* pDC, LPRECT pRect, int nRadius, int nCorners = RC_ALL, 
						DWORD dwStyle = RC_EDGES | RC_THIN | RC_UP, COLORREF crBkgnd = -1);

	// purpose of crBkgnd varies depending on dwStyle:
	//
	// if (dwStyle & (RC_UP | RC_DOWN)) > 0 then its the equivalent buttons color
	// from which the highlight and shadow colors are derived
	//
	// else its the parent background color for drawing the button border hidden

	// some public helpers
	static COLORREF GetColor(int nColor, COLORREF crBase); // nColor = COLOR_3Dxxxx
	static COLORREF Lighter(COLORREF crColor, float fFactor = 0.25f); // 0.0f < fFactor < 1.0f
	static COLORREF Darker(COLORREF crColor, float fFactor = 0.5f); // 0.0f < fFactor < 1.0f

protected:
	CWnd* m_pWnd;
	int m_nRadius;
	COLORREF m_crBkgnd;

protected:
	// draws a segment of the curve (circular or elliptical) in any direction
	// angles defined as follows:
	//
	//        360°/0°
	//          __
	//   270°  /  \  
	//         \__/  90° 
	//
	//         180°
	//
	static COLORREF GetColor(int nAngle, int nDegreesFrom, int nDegreesTo, COLORREF crFrom, COLORREF crTo);

	static void DrawEdges(CDC* pDC, LPRECT pRect, int nRadius, int nCorners, DWORD dwStyle, COLORREF crBkgnd);
	static void DrawTopLeft(CDC* pDC, LPRECT pRect, int nRadius, DWORD dwStyle, COLORREF crBkgnd);
	static void DrawTopRight(CDC* pDC, LPRECT pRect, int nRadius, DWORD dwStyle, COLORREF crBkgnd);
	static void DrawBottomLeft(CDC* pDC, LPRECT pRect, int nRadius, DWORD dwStyle, COLORREF crBkgnd);
	static void DrawBottomRight(CDC* pDC, LPRECT pRect, int nRadius, DWORD dwStyle, COLORREF crBkgnd);

	static void DrawCorner(CDC* pDC, int nRadius, Corner* pCorner);
	static void DrawDefBtnBorder(CDC* pDC, LPRECT pRect, int nRadius, int nCorners);

};

#endif // !defined(AFX_ROUNDCORNER_H__5CCBE3A0_97AD_11D5_95AD_91F9BAF50F32__INCLUDED_)

