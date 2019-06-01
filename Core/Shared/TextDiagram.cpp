// TextDiagram.cpp: implementation of the CTextDiagram class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TextDiagram.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// rects
#define CHAR_TOP '-'
#define CHAR_RIGHT '|'
#define CHAR_BOTTOM '-'
#define CHAR_LEFT '|'

// for output only
#define CHAR_TOPLEFT '.'
#define CHAR_TOPRIGHT '.'
#define CHAR_BOTTOMLEFT '·'
#define CHAR_BOTTOMRIGHT '·'

// connections
#define CHAR_HORZ '-'
#define CHAR_VERT '|'
#define CHAR_END '*'
#define CHAR_ENDUP '^'
#define CHAR_ENDDOWN 'v'
#define CHAR_ENDLEFT '<'
#define CHAR_ENDRIGHT '>'
#define CHAR_CROSSOVER '+'
#define CHAR_START '#' 
#define CHAR_STARTUP '^'
#define CHAR_STARTDOWN 'v'
#define CHAR_STARTLEFT '<'
#define CHAR_STARTRIGHT '>'

// misc
#define ENDL '\n'
#define SPACE ' '

const int MAXITERATIONS = 1000;

//{{DIAGRAM_START
//
//: AKFILE processing elements
//
//     .-----------.                                   .--------.
//     |RJA124RCONV>-----                       -------<ASAKUP  |
//     |           >----+-------      ----------+------<        |
//     ·--vv-------·    |      |      |         |      ·--------·
//        ||            |      |      |         |
//        ||            |      |      |         |      .--------.
//        |v---------.  |      |      |        -+------<RJA124R |
//        ||AKCHGCONV|  |      |     -+--------++------<        |
//        ||         |  |      |     ||        ||      ·v-v-----·
//        |·---------·  v----. v-----vv  .-----vv       | |
//        |             |ENRL| |AKFILE|  |AKCHGS|       | |
//        |             |    | |      |  |      |       | |
//        |             ·----· ^v-----·  ·v-----·       | |
//        |                    ||         |             | |
//        |                    ||         |             | |
//        v----------<---------++---------+-------------- v----.
//        |CWIDEMLSEQ|         ||         |               |ENRL|
//       -<          |         ||         |               |    |
//       |·----------·         ||         |               ·v---·
//       |                     ||         |                |
//       | .--------<----------+-         |                |
//       | |RJA124E <----------+-----------                |
//       | |        >----------+------------->------.     .v---.
//       | ·--------·          |             |STEVTF|     |ftp |
//  .----v            ---------+-------------<      |     |    |
//  |ftp |            |        |             ·------·     ·----·
//  |    |            |        |
//  ·----· .----------v        |                      .----------.
//         |eml_client>---------                      |eml_server|
//         |          >------------------------------->          |
//         ·----------·                               ·----------·
//}}DIAGRAM_END


//////////////////////////////////////////////////////////////////////

CTDRect::CTDRect(const CRect& rect, LPCTSTR szText) : CRect(rect), m_sText(szText)
{
}

CTDRect::CTDRect()
{ 
	SetRectEmpty(); 
}

CTDRect::~CTDRect()
{
}

const CTDRect& CTDRect::operator=(const CTDRect& rect)
{
	CopyRect(rect);
	SetText(rect.GetText());

	return *this;
}

int CTDRect::GetSide(CPoint point)
{
	if (point.x == left)
		return RECT_LEFT;

	else if (point.x == right)
		return RECT_RIGHT;

	else if (point.y == top)
		return RECT_TOP;

	else if (point.y == bottom)
		return RECT_BOTTOM;

	else
		return NONE;
}

//////////////////////////////////////////////////////////////////////

CTDConnection::CTDConnection(int nRectFrom, int nRectTo, int nSideFrom) :
	m_nRectFrom(nRectFrom), m_nRectTo(nRectTo), m_nSideFrom(nSideFrom), m_ptFrom(0, 0)
{
}

CTDConnection::CTDConnection() : m_nRectFrom(0), m_nRectTo(0), m_ptFrom(0, 0)
{
}

CTDConnection::~CTDConnection()
{
}

const CTDConnection& CTDConnection::operator=(const CTDConnection& conn)
{
	m_nRectFrom = conn.m_nRectFrom;
	m_nRectTo = conn.m_nRectTo;
	m_nSideFrom = conn.m_nSideFrom;
	m_ptFrom = conn.m_ptFrom;
	m_aSegments.Copy(conn.m_aSegments);

	return *this;
}

CPoint CTDConnection::GetEndPos(int nNextSegment) const
{
	CPoint ptEnd = GetSegmentPos(NumSegments() - 1);
	int nDir = GetEndDirection(nNextSegment);

	// nNextSegment
	switch (nDir)
	{
	case CONN_UP:
	case CONN_DOWN:
		ptEnd.y += nNextSegment;
		break;
		
	case CONN_RIGHT:
	case CONN_LEFT:
		ptEnd.x += nNextSegment;
		break;
	}

	return ptEnd;
}

CPoint CTDConnection::GetSegmentPos(int nSegment) const
{
	if (nSegment < 0)
		return GetStartPos();

	int nDir = GetStartDirection();
	CPoint ptEnd = GetStartPos();

	int nLastSegment = min(nSegment, NumSegments() - 1);

	for (nSegment = 0; nSegment <= nLastSegment; nSegment++)
	{
		switch (nDir)
		{
		case CONN_UP:
		case CONN_DOWN:
			ptEnd.y += m_aSegments[nSegment];
			nDir = CONN_LEFT; // same effect as CONN_RIGHT
			break;

		case CONN_RIGHT:
		case CONN_LEFT:
			ptEnd.x += m_aSegments[nSegment];
			nDir = CONN_UP; // same effect as CONN_DOWN
			break;
		}
	}

	return ptEnd;
}

int CTDConnection::GetLength() const
{
	int nLength = 0;
	int nSegment = NumSegments();

	while (nSegment--)
		nLength += abs(Segment(nSegment));

	return nLength;
}

int CTDConnection::GetEndDirection(int nNextSegment) const
{
	int nDir = GetStartDirection();

	for (int nSegment = 1; nSegment < NumSegments(); nSegment++)
	{
		switch (nDir)
		{
		case CONN_UP:
		case CONN_DOWN:
			nDir = m_aSegments[nSegment] < 0 ? CONN_LEFT : CONN_RIGHT;
			break;

		case CONN_RIGHT:
		case CONN_LEFT:
			nDir = m_aSegments[nSegment] < 0 ? CONN_UP : CONN_DOWN;
			break;
		}
	}

	// nNextSegment
	if (nNextSegment && NumSegments())
	{
		switch (nDir)
		{
		case CONN_UP:
		case CONN_DOWN:
			nDir = nNextSegment < 0 ? CONN_LEFT : CONN_RIGHT;
			break;

		case CONN_RIGHT:
		case CONN_LEFT:
			nDir = nNextSegment < 0 ? CONN_UP : CONN_DOWN;
			break;
		}
	}

	return nDir;
}

CRect CTDConnection::GetBoundingRect() const
{
	CRect rBounds(m_ptFrom, CSize(0, 0));

	for (int nSegment = 1; nSegment < NumSegments(); nSegment++)
	{
		CPoint ptEnd = GetSegmentPos(nSegment);

		if (rBounds.left > ptEnd.x)
			rBounds.left = ptEnd.x;

		if (rBounds.right < ptEnd.x)
			rBounds.right = ptEnd.x;

		if (rBounds.top > ptEnd.y)
			rBounds.top = ptEnd.y;

		if (rBounds.bottom < ptEnd.y)
			rBounds.bottom = ptEnd.y;
	}

	return rBounds;
}

int CTDConnection::GetStartDirection() const
{
	switch (m_nSideFrom)
	{
	case RECT_TOP:
		return CONN_UP;

	case RECT_RIGHT:
		return CONN_RIGHT;

	case RECT_BOTTOM:
		return CONN_DOWN;

	case RECT_LEFT:
		return CONN_LEFT;
	}

	return -1;
}

void CTDConnection::Increment(int& nSegment)
{
	ASSERT (nSegment);
	nSegment += (nSegment < 0) ? -1 : 1;
}

void CTDConnection::Decrement(int& nSegment)
{
	ASSERT (nSegment);
	nSegment -= (nSegment < 0) ? -1 : 1;
}

void CTDConnection::AddSegment(int nSegment)
{
	m_aSegments.Add(nSegment);
}

void CTDConnection::IncrementLastSegment()
{
	ASSERT (NumSegments());

	if (NumSegments())
		Increment(m_aSegments[NumSegments() - 1]);
}

BOOL CTDConnection::PtInConn(CPoint point, BOOL bHorz) const
{
	// iterate all the segments testing each one for an overlap in the requested orientation
	int nDir = GetStartDirection();

	CPoint ptStart, ptEnd = GetStartPos();

	for (int nSegment = 0; nSegment < NumSegments(); nSegment++)
	{
		ptStart = ptEnd;

		switch (nDir)
		{
		case CONN_UP:
		case CONN_DOWN:
			ptEnd.y += m_aSegments[nSegment];

			// test
			if (!bHorz && point.x == ptEnd.x)
			{
				if ((point.y >= ptStart.y && point.y <= ptEnd.y) ||
					(point.y >= ptEnd.y && point.y <= ptStart.y))
				{
					return TRUE;
				}
			}
			nDir = CONN_LEFT; // same effect as CONN_RIGHT
			break;

		case CONN_RIGHT:
		case CONN_LEFT:
			ptEnd.x += m_aSegments[nSegment];

			// test
			if (bHorz && point.y == ptEnd.y)
			{
				if ((point.x >= ptStart.x && point.x <= ptEnd.x) ||
					(point.x >= ptEnd.x && point.x <= ptStart.x))
				{
					return TRUE;
				}
			}
			nDir = CONN_UP; // same effect as CONN_DOWN
			break;
		}
	}

	return FALSE;
}

//////////////////////////////////////////////////////////////////////

CTextDiagram::CTextDiagram(LPCTSTR szDiagram) : m_size(0, 0), m_bModified(FALSE)
{
	SetDiagram(szDiagram);
}

CTextDiagram::~CTextDiagram()
{

}

void CTextDiagram::ResetDiagram()
{
	m_sTitle.Empty();
	m_size.cx = m_size.cy = 0;

	m_aRects.RemoveAll();
	m_aConns.RemoveAll();
}

void CTextDiagram::SetDiagram(const CTextDiagram& diagram)
{
	*this = diagram;
}

void CTextDiagram::SetDiagram(LPCTSTR szDiagram)
{
	CStringArray diagram;

	ConvertDiagram(szDiagram, diagram);
	SetDiagram(diagram);
}

void CTextDiagram::SetDiagram(const CStringArray& diagram)
{
	ResetDiagram();
	ProcessDiagram(diagram);

	SetModified(FALSE);
}

int CTextDiagram::ConvertDiagram(LPCTSTR szDiagram, CStringArray& diagram)
{
	// convert text into array of lines
	CString sDiagram(szDiagram);
	diagram.RemoveAll(); 

	while (!sDiagram.IsEmpty())
	{
		int nFind = sDiagram.Find(ENDL);
		CString sLine;

		if (nFind != -1)
		{
			sLine = sDiagram.Left(nFind);
			sDiagram = sDiagram.Mid(nFind + 1);
		}
		else
		{
			sLine = sDiagram;
			sDiagram.Empty();
		}

		sLine.TrimRight();
		ExpandTabs(sLine);

		diagram.Add(sLine);
	}

	return diagram.GetSize();
}

BOOL CTextDiagram::GetDiagram(CStringArray& diagram, BOOL bVerify) const
{
	if (bVerify && !VerifyDiagram())
	return FALSE;

	BuildDiagram(diagram);
	return TRUE;
}

BOOL CTextDiagram::GetDiagram(CString& sDiagram, BOOL bVerify) const
{
	CStringArray diagram;

	if (!GetDiagram(diagram, bVerify))
		return FALSE;

	sDiagram.Empty();

	for (int nLine = 0; nLine < diagram.GetSize(); nLine++)
	{
		sDiagram += diagram.GetAt(nLine);
		sDiagram += "\r\n";
	}

	return TRUE;
}

void CTextDiagram::BuildDiagram(CStringArray& diagram) const
{
	diagram.RemoveAll();

	if (m_size.cx && m_size.cy)
	{
		// initialize the diagram to match the current size
		for (int nY = 0; nY < m_size.cy + 1; nY++)
			diagram.Add(CString(SPACE, m_size.cx + 1));

		// draw each rect
		CTDRect rect;
		int nRect = 0;

		while (GetRect(nRect, rect))
		{
			DrawRect(diagram, rect);
			nRect++;
		}

		// draw each conn
		CTDConnection conn;
		int nConn = 0;

		while (GetConnection(nConn, conn))
		{
			DrawConn(diagram, conn);
			nConn++;
		}

		// add title at the end because this alters the y-coordinates
		AddTitle(diagram);

		// finally remove excess whitespace
		int nLine = diagram.GetSize();

		while (nLine--)
			diagram[nLine].TrimRight();
	}
}

void CTextDiagram::AddTitle(CStringArray& diagram) const
{
	CStringArray aTitle;
	CString sTitle(m_sTitle);

	sTitle.TrimRight(); // remove trailing carriage returns

	if (!sTitle.IsEmpty())
	{
		CString sLine(": ");
		int nChar = 0, nLineChar = 0;

		// walk the characters till we hit a new line character
		while (nChar < sTitle.GetLength())
		{
			char c = sTitle[nChar];
			
			// new line?
			if (c == '\r')
			{
				sLine.TrimRight();

				if (aTitle.GetSize() || sLine.GetLength() > 2)
					aTitle.Add(sLine);

				sLine = ": ";
				nLineChar = 0;
				nChar++; // jump '\n' too
			}
			else
			{
				sLine += c;
				nLineChar++;
			}

			nChar++;
		}

		// add whatever's left
		sLine.TrimRight();

		if (sLine.GetLength() > 2)
			aTitle.Add(sLine);
	}

	// insert a blank line above if we have a none empty title
	if (aTitle.GetSize())
		aTitle.InsertAt(0, "");

	diagram.InsertAt(0, &aTitle);
}

void CTextDiagram::DrawRect(CStringArray& diagram, const CTDRect& rect) const
{
	// right
	DrawLine(diagram, CPoint(rect.right, rect.top), CPoint(rect.right, rect.bottom));

	// left
	DrawLine(diagram, CPoint(rect.left, rect.top), CPoint(rect.left, rect.bottom));

	// top
	DrawLine(diagram, CPoint(rect.left, rect.top), CPoint(rect.right, rect.top));

	// bottom
	DrawLine(diagram, CPoint(rect.left, rect.bottom), CPoint(rect.right, rect.bottom));

	// corners
	SetChar(diagram, rect.left, rect.top, CHAR_TOPLEFT);
	SetChar(diagram, rect.right, rect.top, CHAR_TOPRIGHT);
	SetChar(diagram, rect.right, rect.bottom, CHAR_BOTTOMRIGHT);
	SetChar(diagram, rect.left, rect.bottom, CHAR_BOTTOMLEFT);

	// text
	int nLen = rect.GetTextLen(), nX = rect.left + 1, nY = rect.top + 1;
	LPCTSTR szText = rect.GetText();

	for (int nChar = 0; nChar < nLen && nY < rect.bottom; nChar++)
	{
		char c = szText[nChar];

		// new line?
		if (c == '\r')
		{
			nX = rect.left + 1;
			nY++;
			nChar++; // jump '\n'
		}
		else if (nX >= rect.right)
		{
			nX = rect.left + 1;
			nY++;
			nChar--;
		}
		else
		{
			SetChar(diagram, nX, nY, c);
			nX++;
		}
	}
}

void CTextDiagram::DrawConn(CStringArray& diagram, const CTDConnection& conn) const
{
	CPoint ptEnd, ptStart = conn.GetStartPos();

	for (int nSegment = 0; nSegment < conn.NumSegments(); nSegment++)
	{
		ptEnd = conn.GetSegmentPos(nSegment);

		DrawLine(diagram, ptStart, ptEnd, &conn);
		ptStart = ptEnd;
	}

	// replace start with '+'
//	ptStart = conn.GetStartPos();
//	SetChar(diagram, ptStart.x, ptStart.y, CHAR_START);

	// replace end with '*'
//	ptEnd = conn.GetEndPos();
//	SetChar(diagram, ptEnd.x, ptEnd.y, CHAR_END);

	// replace start and end with arrows
	ptStart = conn.GetStartPos();
	switch (conn.GetStartDirection())
	{
	case CONN_UP:
		SetChar(diagram, ptStart.x, ptStart.y, CHAR_STARTUP);
		break;

	case CONN_DOWN:
		SetChar(diagram, ptStart.x, ptStart.y, CHAR_STARTDOWN);
		break;

	case CONN_LEFT:
		SetChar(diagram, ptStart.x, ptStart.y, CHAR_STARTLEFT);
		break;

	case CONN_RIGHT:
		SetChar(diagram, ptStart.x, ptStart.y, CHAR_STARTRIGHT);
		break;
	}

	ptEnd = conn.GetEndPos();
	switch (conn.GetEndDirection())
	{
	case CONN_UP:
		SetChar(diagram, ptEnd.x, ptEnd.y, CHAR_ENDUP);
		break;

	case CONN_DOWN:
		SetChar(diagram, ptEnd.x, ptEnd.y, CHAR_ENDDOWN);
		break;

	case CONN_LEFT:
		SetChar(diagram, ptEnd.x, ptEnd.y, CHAR_ENDLEFT);
		break;

	case CONN_RIGHT:
		SetChar(diagram, ptEnd.x, ptEnd.y, CHAR_ENDRIGHT);
		break;
	}
}

void CTextDiagram::DrawLine(CStringArray& diagram, CPoint ptFrom, CPoint ptTo, const CTDConnection* pConn) const
{
	// must be vert or horz
	ASSERT (ptFrom.x == ptTo.x || ptFrom.y == ptTo.y);

	int nXLen = abs(ptFrom.x - ptTo.x);
	int nYLen = abs(ptFrom.y - ptTo.y);

	if (nYLen) // vert
	{
		int nFrom = (ptFrom.y > ptTo.y) ? ptTo.y : ptFrom.y;
		int nTo = (ptFrom.y > ptTo.y) ? ptFrom.y : ptTo.y;

		if (pConn && nYLen > 0)
		{
			nFrom++;

			// make sure the line is at least one char long
			// with the exception of the start and end points
			if (nTo - nFrom > 0 || ptFrom == pConn->GetStartPos() || ptTo == pConn->GetEndPos())
				nTo--;
		}

		for (int nY = nFrom; nY <= nTo; nY++)
		{
			char c = CHAR_VERT;

			if (pConn && PtInConn(ptTo.x, nY, TRUE) && !pConn->PtInConn(CPoint(ptTo.x, nY), TRUE))
				c = CHAR_CROSSOVER;

			SetChar(diagram, ptTo.x, nY, c);
		}
	}
	else if (nXLen) // horz
	{
		int nFrom = (ptFrom.x > ptTo.x) ? ptTo.x : ptFrom.x;
		int nTo = (ptFrom.x > ptTo.x) ? ptFrom.x : ptTo.x;

		for (int nX = nFrom; nX <= nTo; nX++)
		{
			char c = CHAR_HORZ;

			if (pConn && PtInConn(nX, ptTo.y, FALSE) && !pConn->PtInConn(CPoint(nX, ptTo.y), FALSE))
				c = CHAR_CROSSOVER;

			SetChar(diagram, nX, ptTo.y, c);
		}
	}
}

void CTextDiagram::ExpandTabs(CString& sLine, int nTabSize) 
{
	CString sExpanded;

	for (int nPos = 0; nPos < sLine.GetLength(); nPos++)
	{
		char c = sLine[nPos];

		if (c == '\t')
		{
			int nTabs = nTabSize - (sExpanded.GetLength() % nTabSize);

			while (nTabs--)
				sExpanded += ' ';
		}
		else
			sExpanded += c;
	}

	sLine = sExpanded;
}

char CTextDiagram::GetChar(const CStringArray& diagram, int nX, int nY)
{
	if (nY < 0 || nY >= diagram.GetSize())
		return 0;

	CString& sLine = diagram[nY];

	if (nX < 0 || nX >= sLine.GetLength())
		return 0;

	return sLine[nX];
}

BOOL CTextDiagram::SetChar(CStringArray& diagram, int nX, int nY, char c)
{
	if (!c)
		return FALSE;

	if (nY < 0 || nY >= diagram.GetSize())
	{
		ASSERT(0);
		return FALSE;
	}

	CString& sLine = diagram[nY];

	if (nX < 0 || nX >= sLine.GetLength())
	{
		ASSERT(0);
		return FALSE;
	}

	sLine.SetAt(nX, c);
	return TRUE;
}

int CTextDiagram::LineLength(const CStringArray& diagram, int nY)
{
	return (nY < 0 || nY >= diagram.GetSize()) ? 0 : diagram[nY].GetLength();
}

BOOL CTextDiagram::TestChar(const CStringArray& diagram, int nX, int nY, char cTest1, char cTest2, char cTest3, char cTest4)
{
	char c = GetChar(diagram, nX, nY);

	return ((cTest1 && cTest1 == c) || (cTest2 && cTest2 == c) || 
			(cTest3 && cTest3 == c) || (cTest4 && cTest4 == c));
}

void CTextDiagram::ProcessDiagram(const CStringArray& diagram)
{
	if (!diagram.GetSize())
		return;

	CStringArray diagramTemp;
	diagramTemp.Copy(diagram);

	// 1. title
	// find the first non-empty line
	int nLine = 0;
	BOOL bContinue = TRUE;
	
	while (bContinue && nLine < diagramTemp.GetSize())
	{
		CString sLine = diagramTemp[nLine];
		sLine.TrimLeft();
		
		if (bContinue = sLine.IsEmpty())
			nLine++;
	}
	
	// title lines must begin with a ':'
	if (TestChar(diagramTemp, 0, nLine, ':'))
	{
		while (TestChar(diagramTemp, 0, nLine, ':'))
		{
			CString sLine = diagramTemp[nLine].Mid(1);
			sLine.TrimLeft();
			sLine.TrimRight();
			
			m_sTitle += sLine;
			m_sTitle += "\r\n";
			
			nLine++;
		}
		
		// remove the title lines
		while (nLine--)
			diagramTemp.RemoveAt(0);
	}

	if (!diagramTemp.GetSize())
		return;

	// 2. check boundary conditions.
	// specifically rects either of whose left or top is zero
	BOOL bFoundHorz = FALSE, bFoundVert = FALSE;

	for (int nX = 0; nX < LineLength(diagramTemp, 0) && !bFoundHorz; nX++)
		bFoundHorz = (RECT_TOP == RectStartChar(diagramTemp, nX, 0));

	for (int nY = 0; nY < diagramTemp.GetSize() && !bFoundVert; nY++)
		bFoundVert = (RECT_TOP == RectStartChar(diagramTemp, 1, nY));

	// if we find a rect at y==0 then simply add an empty line at the start
	if (bFoundHorz)
		diagramTemp.InsertAt(0, "");

	// if we find a rect at x==0 then prepeand a space to every line
	if (bFoundVert)
	{
		for (int nY = 0; nY < diagramTemp.GetSize(); nY++)
			diagramTemp[nY] = " " + diagramTemp[nY];
	}

	// 3. process rects
	for (nY = 0; nY < diagramTemp.GetSize(); nY++)
	{
		int nLineLen = LineLength(diagramTemp, nY);

		for (int nX = 0; nX < nLineLen; nX++)
		{
			AddRect(diagramTemp, nX, nY);
		}
	}

	// 4. process connections
	if (GetRectCount() > 1)
	{
		for (nY = 0; nY < diagramTemp.GetSize(); nY++)
		{
			int nLineLen = LineLength(diagramTemp, nY);

			for (int nX = 0; nX < nLineLen; nX++)
			{
				AddConn(diagramTemp, nX, nY);
			}
		}
	}
}

TDPOS CTextDiagram::RectGetNextChar(const CStringArray& diagram, TDPOS nCurPos, int& nX, int& nY) const
{
	if (PtInRect(nX, nY))
		return NONE;

	switch (nCurPos)
	{
	case RECT_TOP:
		// test change of direction first
		if (TestChar(diagram, nX + 1, nY + 1, CHAR_RIGHT, CHAR_START, CHAR_END) ||
			TestChar(diagram, nX + 1, nY + 1, CHAR_STARTRIGHT, CHAR_ENDLEFT))
		{
			nX++;
			nY++;
			return RECT_RIGHT;
		}
		else if (TestChar(diagram, nX + 1, nY, CHAR_TOP, CHAR_START, CHAR_END) ||
				TestChar(diagram, nX + 1, nY, CHAR_STARTUP, CHAR_ENDDOWN))
		{
			nX++;
			return RECT_TOP;
		}
		break;

	case RECT_RIGHT:
		// test change of direction first
		if (TestChar(diagram, nX - 1, nY + 1, CHAR_BOTTOM, CHAR_START, CHAR_END) ||
			TestChar(diagram, nX - 1, nY + 1, CHAR_STARTDOWN, CHAR_ENDUP))
		{
			nX--;
			nY++;
			return RECT_BOTTOM;
		}
		else if (TestChar(diagram, nX, nY + 1, CHAR_RIGHT, CHAR_START, CHAR_END) ||
				TestChar(diagram, nX, nY + 1, CHAR_STARTRIGHT, CHAR_ENDLEFT))	
		{
			nY++;
			return RECT_RIGHT;
		}
		break;

	case RECT_BOTTOM:
		// test change of direction first
		if (TestChar(diagram, nX - 1, nY - 1, CHAR_LEFT, CHAR_START, CHAR_END) ||
			TestChar(diagram, nX - 1, nY - 1, CHAR_STARTLEFT, CHAR_ENDRIGHT))
		{
			nX--;
			nY--;
			return RECT_LEFT;
		}
		else if (TestChar(diagram, nX - 1, nY, CHAR_BOTTOM, CHAR_START, CHAR_END) ||
				TestChar(diagram, nX - 1, nY, CHAR_STARTDOWN, CHAR_ENDUP))
		{
			nX--;
			return RECT_BOTTOM;
		}
		break;

	case RECT_LEFT:
		// test change of direction first
		if (TestChar(diagram, nX + 1, nY - 1, CHAR_TOP, CHAR_START, CHAR_END) ||
			TestChar(diagram, nX + 1, nY - 1, CHAR_STARTUP, CHAR_ENDDOWN))
		{
			nX++;
			nY--;
			return RECT_TOP;
		}
		else if (TestChar(diagram, nX, nY - 1, CHAR_LEFT, CHAR_START, CHAR_END) ||
				TestChar(diagram, nX, nY - 1, CHAR_STARTLEFT, CHAR_ENDRIGHT))	
		{
			nY--;
			return RECT_LEFT;
		}
		break;
	}

	return NONE;
}

TDPOS CTextDiagram::RectStartChar(const CStringArray& diagram, int nX, int nY) const
{
	if (PtInRect(nX, nY))
		return NONE;

	char c = GetChar(diagram, nX, nY);

	// test for top char
	if (c == CHAR_TOP || c == CHAR_START || c == CHAR_END || c == CHAR_STARTUP || c == CHAR_ENDDOWN)
	{
		// test for left char
		c = GetChar(diagram, nX - 1, nY + 1);
		
		if (c == CHAR_LEFT || c == CHAR_START || c == CHAR_END || c == CHAR_STARTLEFT || c == CHAR_ENDRIGHT)
		{
			// finally test for corner char
			c = GetChar(diagram, nX - 1, nY);

			if (c == SPACE || c == CHAR_TOPLEFT || c == CHAR_START || c == CHAR_END ||
				c == CHAR_STARTLEFT || c == CHAR_ENDRIGHT || c == CHAR_STARTUP || c == CHAR_ENDDOWN)
				return RECT_TOP;
		}
	}

	return NONE;
}

TDPOS CTextDiagram::ConnStartChar(const CStringArray& diagram, int nX, int nY) const
{
	if (!PtInRect(nX, nY))
		return NONE;

	char c = GetChar(diagram, nX, nY);

	if (c == CHAR_START || c == CHAR_STARTUP || c == CHAR_STARTDOWN || 
		c == CHAR_STARTLEFT || c == CHAR_STARTRIGHT)
	{
		// check we are on a rect border
		int nRect = IntersectRect(nX, nY);
		ASSERT (nRect != -1);

		CTDRect rect;
		
		if (GetRect(nRect, rect) && rect.GetSide(CPoint(nX, nY)) != NONE)
			return CONN_START;
	}

	// all else
	return NONE;
}

TDPOS CTextDiagram::ConnGetNextChar(const CStringArray& diagram, TDPOS nCurPos, int& nX, int& nY) const
{
	switch (nCurPos)
	{
	case CONN_START:
		// test normal chars first
		if (IntersectRect(nX + 1, nY) == -1 && TestChar(diagram, nX + 1, nY, CHAR_HORZ, CHAR_CROSSOVER))
		{
			nX++;
			return CONN_RIGHT;
		}
		else if (IntersectRect(nX - 1, nY) == -1 && TestChar(diagram, nX - 1, nY, CHAR_HORZ, CHAR_CROSSOVER))
		{
			nX--;
			return CONN_LEFT;
		}
		else if (IntersectRect(nX, nY + 1) == -1 && TestChar(diagram, nX, nY + 1, CHAR_VERT, CHAR_CROSSOVER))
		{
			nY++;
			return CONN_DOWN;
		}
		else if (IntersectRect(nX, nY - 1) == -1 && TestChar(diagram, nX, nY - 1, CHAR_VERT, CHAR_CROSSOVER))
		{
			nY--;
			return CONN_UP;
		}
		// then, if vertical, test for immediate change of direction
		else if (IntersectRect(nX, nY + 1) == -1 && TestChar(diagram, nX, nY + 1, CHAR_HORZ))
		{
			nY++;

			// which way does it go?
			if (TestChar(diagram, nX + 1, nY, CHAR_HORZ, CHAR_CROSSOVER))
				return CONN_RIGHT;
			else
				return CONN_LEFT;
		}
		else if (IntersectRect(nX, nY - 1) == -1 && TestChar(diagram, nX, nY - 1, CHAR_HORZ))
		{
			nY--;

			// which way does it go?
			if (TestChar(diagram, nX + 1, nY, CHAR_HORZ, CHAR_CROSSOVER))
				return CONN_RIGHT;
			else
				return CONN_LEFT;
		}
		break;
		
	case CONN_RIGHT:
		// 1. test for continuation
		if (TestChar(diagram, nX + 1, nY, CHAR_HORZ, CHAR_CROSSOVER))
		{
			nX++;
			return CONN_RIGHT;
		}
		// 2. test for termination
		else if (TestChar(diagram, nX + 1, nY, CHAR_END, CHAR_ENDRIGHT))
		{
			nX++;
			return CONN_END;
		}
		// 3. test for change of direction (down)
		else if (TestChar(diagram, nX, nY + 1, CHAR_VERT, CHAR_CROSSOVER))
		{
			nY++;
			return CONN_DOWN;
		}
		// 4. test for change of direction (up)
		else if (TestChar(diagram, nX, nY - 1, CHAR_VERT, CHAR_CROSSOVER))
		{
			nY--;
			return CONN_UP;
		}
		// 5. test for termination (down)
		else if (TestChar(diagram, nX, nY + 1, CHAR_END, CHAR_ENDDOWN))
		{
			nY++;
			return CONN_END;
		}
		// 6. test for termination (up)
		else if (TestChar(diagram, nX, nY - 1, CHAR_END, CHAR_ENDUP))
		{
			nY--;
			return CONN_END;
		}
		break;
		
	case CONN_LEFT:
		// 1. test for continuation
		if (TestChar(diagram, nX - 1, nY, CHAR_HORZ, CHAR_CROSSOVER))
		{
			nX--;
			return CONN_LEFT;
		}
		// 2. test for termination
		else if (TestChar(diagram, nX - 1, nY, CHAR_END, CHAR_ENDLEFT))
		{
			nX--;
			return CONN_END;
		}
		// 3. test for change of direction (down)
		else if (TestChar(diagram, nX, nY + 1, CHAR_VERT, CHAR_CROSSOVER))
		{
			nY++;
			return CONN_DOWN;
		}
		// 4. test for change of direction (up)
		else if (TestChar(diagram, nX, nY - 1, CHAR_VERT, CHAR_CROSSOVER))
		{
			nY--;
			return CONN_UP;
		}
		// 5. test for termination (down)
		else if (TestChar(diagram, nX, nY + 1, CHAR_END, CHAR_ENDDOWN))
		{
			nY++;
			return CONN_END;
		}
		// 6. test for termination (up)
		else if (TestChar(diagram, nX, nY - 1, CHAR_END, CHAR_ENDUP))
		{
			nY--;
			return CONN_END;
		}
		break;
		
	case CONN_UP:
		if (TestChar(diagram, nX, nY - 1, CHAR_HORZ))
		{
			nY--;

			// which way does it go?
			if (TestChar(diagram, nX + 1, nY, CHAR_HORZ, CHAR_CROSSOVER, CHAR_END))
				return CONN_RIGHT;
			else
				return CONN_LEFT;
		}
		else if (TestChar(diagram, nX, nY - 1, CHAR_CROSSOVER))
		{
			nY--;
			return CONN_UP;
		}
		else if (TestChar(diagram, nX, nY - 1, CHAR_END, CHAR_ENDUP))
		{
			nY--;
			return CONN_END;
		}
		else if (TestChar(diagram, nX, nY - 1, CHAR_VERT))
		{
			nY--;
			return CONN_UP;
		}
		break;
		
	case CONN_DOWN:
		if (TestChar(diagram, nX, nY + 1, CHAR_HORZ))
		{
			nY++;

			// which way does it go?
			if (TestChar(diagram, nX + 1, nY, CHAR_HORZ, CHAR_CROSSOVER, CHAR_END))
				return CONN_RIGHT;
			else
				return CONN_LEFT;
		}
		else if (TestChar(diagram, nX, nY + 1, CHAR_CROSSOVER))
		{
			nY++;
			return CONN_DOWN;
		}
		else if (TestChar(diagram, nX, nY + 1, CHAR_END, CHAR_ENDDOWN))
		{
			nY++;
			return CONN_END;
		}
		else if (TestChar(diagram, nX, nY + 1, CHAR_VERT))
		{
			nY++;
			return CONN_DOWN;
		}
		break;
	}
	
	return NONE;
}

void CTextDiagram::AddConn(const CStringArray& diagram, int nX, int nY)
{
	TDPOS nPos = ConnStartChar(diagram, nX, nY);

	if (nPos == CONN_START)
	{
		CPoint ptStart(nX, nY);
		int nFromRect = IntersectRect(nX, nY);

		BOOL bDone = FALSE;
		TDPOS nPosOrg = nPos;

		int nIteration = 0; // for catching infinite loops

		while (nPos != NONE && !bDone && nIteration < MAXITERATIONS)
		{
			nIteration++;
			nPos = ConnGetNextChar(diagram, nPos, nX, nY);

			// are we done yet?
			bDone = (nPos == CONN_END);
		}

		if (bDone)
		{
			int nToRect = IntersectRect(nX, nY);

			if (nToRect != -1 && nToRect != nFromRect)
			{
				CTDRect rect;

				GetRect(nFromRect, rect);
				int nSideFrom = rect.GetSide(ptStart);

				ASSERT (nSideFrom != NONE);

				AddConnection(nFromRect, nToRect, nSideFrom);
			}
		}
	}
}

BOOL CTextDiagram::PtInRect(int nX, int nY) const
{
	return (IntersectRect(nX, nY) != -1);
}

int CTextDiagram::IntersectRect(int nX, int nY) const
{
	if (!m_aRects.GetSize())
		return -1;

	// else
	for (int nRect = 0; nRect < m_aRects.GetSize(); nRect++)
	{
		CRect rect = m_aRects[nRect];

		rect.right++; // inclusive
		rect.bottom++; // inclusive

		if (rect.PtInRect(CPoint(nX, nY)))
			return nRect;
	}

	return -1;
}

BOOL CTextDiagram::PtInConn(int nX, int nY, BOOL bHorz) const
{
	return (IntersectConn(CPoint(nX, nY), bHorz) != -1);
}

int CTextDiagram::IntersectConn(CPoint point, BOOL bHorz) const
{
	int nConn = m_aConns.GetSize();

	while (nConn--)
	{
		if (m_aConns[nConn].PtInConn(point, bHorz))
			return nConn;
	}

	return -1; // not found
}

BOOL CTextDiagram::IntersectRect(const CRect& rect, int nIgnoreRect) const
{
	if (!m_aRects.GetSize())
		return FALSE;

	// else
	CRect rTest(rect);
	rTest.right++; // inclusive
	rTest.bottom++; // inclusive

	for (int nRect = 0; nRect < m_aRects.GetSize(); nRect++)
	{
		CRect r = m_aRects[nRect];

		r.right++; // inclusive
		r.bottom++; // inclusive

		if (CRect().IntersectRect(r, rTest) && nRect != nIgnoreRect)
			return TRUE;
	}

	return FALSE;
}

void CTextDiagram::AddRect(const CStringArray& diagram, int nX, int nY)
{
	TDPOS nPos = RectStartChar(diagram, nX, nY);

	if (nPos != NONE)
	{
		CPoint ptOrg(nX, nY);
		CRect rect(nX, nY, nX, nY);

		BOOL bDone = FALSE;
		TDPOS nPosOrg = nPos;

		int nIteration = 0; // for catching infinite loops

		while (nPos != NONE && !bDone && nIteration < MAXITERATIONS)
		{
			nIteration++;
			nPos = RectGetNextChar(diagram, nPos, nX, nY);

			if (nPos != NONE)
			{
				rect.left = min(rect.left, nX);
				rect.top = min(rect.top, nY);
				rect.right = max(rect.right, nX);
				rect.bottom = max(rect.bottom, nY);

				// are we done yet?
				bDone = (nPos == nPosOrg && nX == ptOrg.x && nY == ptOrg.y);
			}
		}

		// get rect text
		if (bDone)
		{
			CString sText;

			for (nY = rect.top + 1; nY < rect.bottom; nY++)
			{
				for (nX = rect.left + 1; nX < rect.right; nX++)
				{
					sText += GetChar(diagram, nX, nY);
				}
				
				// if the last char is a space then insert a carriage return
				if (sText.Right(1) == " ")
				{
					sText.TrimRight();
					sText += "\r\n";
				}
			}

			sText.TrimRight();
			AddRect(rect, sText);
		}
	}
}

BOOL CTextDiagram::GetRect(int nRect, CTDRect& rect) const
{
	if (nRect < 0 || nRect >= GetRectCount())
		return FALSE;

	// else
	rect = m_aRects[nRect];
	return TRUE;
}

BOOL CTextDiagram::GetRect(int nRect, CRect& rect) const
{
	CTDRect tdRect;

	if (GetRect(nRect, tdRect))
	{
		rect = tdRect;
		return TRUE;
	}

	return FALSE;
}


BOOL CTextDiagram::DeleteRect(int nRect)
{
	if (nRect < 0 || nRect >= GetRectCount())
		return FALSE;

	// modify all connections referencing rects after this one
	// to take account of the removal
	// and remove all connections which reference this rect
	int nConn = m_aConns.GetSize();

	while (nConn--)
	{
		CTDConnection& conn = m_aConns[nConn];

		int nRectFrom = conn.RectFrom();
		int nRectTo = conn.RectTo();

		if (nRectFrom == nRect || nRectTo == nRect)
			m_aConns.RemoveAt(nConn);
		else
		{
			if (nRectFrom > nRect)
				conn.SetRectFrom(nRectFrom - 1);

			if (nRectTo > nRect)
				conn.SetRectTo(nRectTo - 1);
		}
	}

	m_aRects.RemoveAt(nRect);
	RebuildConnections();
	SetModified();

	return TRUE;
}

BOOL CTextDiagram::DeleteConnection(int nConn)
{
	if (nConn < 0 || nConn >= GetConnectionCount())
		return FALSE;

	m_aConns.RemoveAt(nConn);
	SetModified();

	return TRUE;
}

BOOL CTextDiagram::GetConnection(int nConn, CTDConnection& conn) const
{
	if (nConn < 0 || nConn >= GetConnectionCount())
		return FALSE;

	// else
	conn = m_aConns[nConn];
	return TRUE;
}

BOOL CTextDiagram::SetRect(int nRect, const CTDRect& rect)
{
	if (nRect < 0 || nRect >= GetRectCount())
		return FALSE;

	// validate the rect
	if (rect.left <= 0 || rect.top <= 0 || rect.Width() < 2 || rect.Height() < 2)
		return FALSE;

	// make sure it doesn't overlap any other rect
	// don't let it come too close either
	CRect r(rect);
	r.InflateRect(1, 1);

	if (IntersectRect(r, nRect))
		return FALSE;

	// save current state in case this fails
	CTextDiagram diagramBackup(*this);

	m_aRects[nRect] = rect;

	// rebuild all connections to ensure no overlaps
	if (RebuildConnections())
	{
		// update the diagram size
		RecalcSize();
		SetModified();

		return TRUE;
	}

	// else restore the backup
	SetDiagram(diagramBackup);
	return FALSE;
}

BOOL CTextDiagram::SetText(int nRect, LPCTSTR szText)
{
	if (nRect < 0 || nRect >= GetRectCount())
		return FALSE;

	m_aRects[nRect].SetText(szText);
	SetModified();

	return TRUE;
}

BOOL CTextDiagram::GetText(int nRect, CString& sText)
{
	if (nRect < 0 || nRect >= GetRectCount())
		return FALSE;

	sText = m_aRects[nRect].GetText();
	return TRUE;
}

BOOL CTextDiagram::RebuildConnections()
{
	// save a copy of existing connections
	CArray<CTDConnection, CTDConnection&> aConns;
	aConns.Copy(m_aConns);

	// delete all connections and re-add
	m_aConns.RemoveAll();

	for (int nConn = 0; nConn < aConns.GetSize(); nConn++)
	{
		// defensive programming
		CTDConnection conn = aConns[nConn];

		if (AddConnection(conn.RectFrom(), conn.RectTo(), conn.SideFrom()) == -1)
			return FALSE;
	}

	return TRUE;
}

int CTextDiagram::AddRect(LPCRECT pRect, LPCTSTR szText)
{
	// validate the rect
	CRect rect(pRect ? pRect : NewRect());

	if (rect.left <= 0 || rect.top <= 0 || rect.Width() < 2 || rect.Height() < 2)
		return -1;

	// make sure it doesn't overlap any other rect
	// don't let it come too close either
	CRect r(rect);
	r.InflateRect(1, 1);

	if (IntersectRect(r))
		return -1;

	// don't let it overlap or touch any connections
	if (IntersectConn(rect))
		return -1;

	// save current state in case this fails
	CTextDiagram diagramBackup(*this);

	int nRect = m_aRects.Add(CTDRect(rect, szText));

	// rebuild all connections to ensure no overlaps
	if (nRect >= 0 && RebuildConnections())
	{
		// update the diagram size
		RecalcSize();
		SetModified();

		return nRect;
	}

	// else restore backup
	SetDiagram(diagramBackup);
	return -1;
}

int CTextDiagram::FindConnection(int nRectFrom, int nRectTo) const
{
	int nConn = m_aConns.GetSize();

	while (nConn--)
	{
		if (m_aConns[nConn].RectFrom() == nRectFrom && m_aConns[nConn].RectTo() == nRectTo)
			return nConn;
	}

	return -1; // not found
}

int CTextDiagram::AddConnection(int nRectFrom, int nRectTo, int nSideFrom)
{
	if (FindConnection(nRectFrom, nRectTo) != -1)
		return -1;

	CTDConnection conn(nRectFrom, nRectTo, nSideFrom);

	if (BuildPath(conn, -1))
	{
		int nConn = m_aConns.Add(conn);

		// update the diagram size
		RecalcSize();
		SetModified();

		return nConn;
	}

	return -1;
}

BOOL CTextDiagram::SetConnection(int nConn, const CTDConnection& conn)
{
	CTDConnection connExist;

	// retrieve existing connection
	if (!GetConnection(nConn, connExist))
		return FALSE;

	// ensure requested connection is not already taken
	int nCurConn = FindConnection(conn.RectFrom(), conn.RectTo());

	if (nCurConn != -1 && nCurConn != nConn)
		return FALSE;

	// save current state in case this fails
	CTextDiagram diagramBackup(*this);

	// update existing connection
	m_aConns.SetAt(nConn, CTDConnection(conn));

	if (RebuildConnections())
	{
		// update the diagram size
		RecalcSize();
		SetModified();

		return TRUE;
	}

	// restore backup
	SetDiagram(diagramBackup);
	return FALSE;
}

void CTextDiagram::GetRelationship(const CRect& rFrom, const CRect& rTo, int& nHorz, int& nVert)
{
	GetRelationship(rFrom.CenterPoint(), rTo.CenterPoint(), nHorz, nVert);
}

void CTextDiagram::GetRelationship(CPoint ptFrom, const CRect& rTo, int& nHorz, int& nVert)
{
	// horz
	if (ptFrom.x >= rTo.left && ptFrom.x <= rTo.right)
		nHorz = 0; // 'to' is aligned with 'from'

	else if (ptFrom.x < rTo.left)
		nHorz = 1; // 'to' is to the right of 'from'

	else
		nHorz = -1; // 'to' is to the left of 'from'

	// vert
	if (ptFrom.y >= rTo.top && ptFrom.y <= rTo.bottom)
		nVert = 0; // 'to' is aligned with 'from'

	else if (ptFrom.y < rTo.top)
		nVert = 1; // 'to' is below 'from'

	else
		nVert = -1; // 'to' is above 'from'
}

void CTextDiagram::GetRelationship(CPoint ptFrom, CPoint ptTo, int& nHorz, int& nVert)
{
	// horz
	if (ptFrom.x == ptTo.x)
		nHorz = 0; // 'to' is aligned with 'from'

	else if (ptFrom.x < ptTo.x)
		nHorz = 1; // 'to' is to the right of 'from'

	else
		nHorz = -1; // 'to' is to the left of 'from'

	// vert
	if (ptFrom.y == ptTo.y)
		nVert = 0; // 'to' is aligned with 'from'

	else if (ptFrom.y < ptTo.y)
		nVert = 1; // 'to' is below 'from'

	else
		nVert = -1; // 'to' is above 'from'
}

BOOL CTextDiagram::PickStartPoint(const CTDConnection& conn, int nIncrement, CPoint& ptStart, int nIgnoreConn) const
{
	CRect rFrom;
	
	if (!GetRect(conn.RectFrom(), rFrom))
		return FALSE;

	BOOL bHorz = FALSE;

	// determine start point
	int nSideFrom = conn.SideFrom();

	switch (nSideFrom)
	{
	case RECT_LEFT:
	case RECT_RIGHT:
		{
			ptStart.x = (nSideFrom == RECT_LEFT) ? rFrom.left : rFrom.right;
			ptStart.y = rFrom.top + 1;

			while(nIncrement--)
				ptStart.y++;

			if (ptStart.y >= rFrom.bottom)
				return FALSE;

			int nIntersect = IntersectConn(ptStart, TRUE);

			return (nIntersect == -1 || nIntersect == nIgnoreConn) ? 1 : -1;
		}
		break;

	case RECT_TOP:
	case RECT_BOTTOM:
		{
			ptStart.y = (nSideFrom == RECT_TOP) ? rFrom.top : rFrom.bottom;
			ptStart.x = rFrom.left + 1;

			while(nIncrement--)
				ptStart.x++;

			if (ptStart.x >= rFrom.right)
				return FALSE;

			int nIntersect = IntersectConn(ptStart, FALSE);

			return (nIntersect == -1 || nIntersect == nIgnoreConn) ? 1 : -1;
		}
		break;
	}

	return 0;
}

int CTextDiagram::BuildPath(CTDConnection& conn, int nIgnoreConn)
{
	if (conn.RectFrom() == conn.RectTo())
		return FALSE;
	
	CRect rFrom, rTo;
	
	if (!GetRect(conn.RectFrom(), rFrom) || !GetRect(conn.RectTo(), rTo))
		return FALSE;

	// reset
	conn.ResetSegments();

	// to get the best results we try every point on the start side
	// and take to shortest route as our solution
	CTDConnection connMin;
	int nIncrement = 0;
	CPoint ptFrom;
	int nPick = 0;

	while (nPick = PickStartPoint(conn, nIncrement, ptFrom, nIgnoreConn))
	{
		if (nPick == -1) // already taken
		{
			nIncrement++;
			continue;
		}

		CTDConnection connTemp(conn);
		
		connTemp.SetStartPos(ptFrom);
		BOOL bCanBacktrack = FALSE; // always false for first segment
		BOOL bBlueYonder = FALSE; // indicates whether the last change of dir was due to 'blue yonder'

		int nIteration = 0; // for catching infinite loops

		int nHorzRect, nVertRect;
		GetRelationship(rFrom, rTo, nHorzRect, nVertRect);

		// build the path
		// note: we can stop any path as soon as we exceed the current shortest path
		while (IntersectRect(connTemp.GetEndPos()) != connTemp.RectTo() && 
				nIteration < MAXITERATIONS &&
				(!connMin.GetLength() || connTemp.GetLength() < connMin.GetLength()))
		{
			// start of a new segment
			int nCurSegment = 0;
			BOOL bSegmentDone = FALSE;

			// cache our current relationship
			int nHorzOrg, nVertOrg;
			GetRelationship(connTemp.GetEndPos(), rTo, nHorzOrg, nVertOrg);

			// refresh current relationship
			while (!bSegmentDone && nIteration < MAXITERATIONS)
			{
				nIteration++;

				// refresh current relationship
				CPoint ptEnd = connTemp.GetEndPos(nCurSegment);

				int nHorz, nVert;
				GetRelationship(ptEnd, rTo, nHorz, nVert);

				// special case: start of segment
				if (!nCurSegment)
				{
					// start of first segment
					if (connTemp.GetStartPos() == ptEnd)
					{
						// always move at least one char in the direction we are pointing
						switch (connTemp.SideFrom())
						{
						case RECT_LEFT:
						case RECT_TOP:
							nCurSegment--;
							break;

						case RECT_RIGHT:
						case RECT_BOTTOM:
							nCurSegment++;
							break;

						default:
							ASSERT(0);
						}
					}
					else // start of subsequent segment
					{
						// select new direction
						int nPrevDir = connTemp.GetEndDirection();

						switch (nPrevDir)
						{
						case CONN_UP:
						case CONN_DOWN:
							if (nHorz)
								nCurSegment = nHorz;
							else
							{
								// look more closely at the rect relationship
								nCurSegment = nHorzRect ? nHorzRect : 1;
							}
							break;
							
						case CONN_RIGHT:
						case CONN_LEFT:
							if (nVert)
								nCurSegment = nVert;
							else
							{
								// look more closely at the rect relationship
								nCurSegment = nVertRect ? nVertRect : 1;
							}
							break;
						}
					}

					continue;
				}
				// special case: make sure we're *not* pointing off into the wide blue yonder
				else
				{
					int nDir = connTemp.GetEndDirection(nCurSegment);
					
					if ((nDir == CONN_LEFT && nHorz == 1) || 
						(nDir == CONN_UP && nVert == 1) ||
						(nDir == CONN_RIGHT && nHorz == -1) || 
						(nDir == CONN_DOWN && nVert == -1))
					{
						bBlueYonder = TRUE;
						bSegmentDone = TRUE;
						connTemp.AddSegment(nCurSegment);
						continue;
					}
				}

				// else continue to move in same direction until we hit something
				// or we intersect vertically or horizontally with nRectTo
				int nDir = connTemp.GetEndDirection(nCurSegment);
				
				// 1. check if we've arrived
				if (IntersectRect(ptEnd) == connTemp.RectTo())
				{
					bBlueYonder = FALSE;
					bSegmentDone = TRUE;
					connTemp.AddSegment(nCurSegment);
					continue;
				}
				
				// 2. hit another rect or run off the page?
				if (ptEnd.x < 0 || ptEnd.y < 0 || PtInRect(ptEnd.x, ptEnd.y))
				{
					bSegmentDone = TRUE;
					
					if (bCanBacktrack) // backtrack to last segment, move on and try again
					{
						connTemp.IncrementLastSegment();
					}
					else
					{
						CTDConnection::Decrement(nCurSegment);
						connTemp.AddSegment(nCurSegment);
					}
					
					bBlueYonder = FALSE;
					continue;
				}
				
				// 3. hit another conn?
				// note: although the code following is the same as for hitting a rect
				// we do it separately as its more efficient to do the rect check first
				int nConn = IntersectConn(ptEnd, nDir == CONN_LEFT || nDir == CONN_RIGHT);
				
				if (nConn != -1 && nConn != nIgnoreConn)
				{
					bSegmentDone = TRUE;
					
					if (bCanBacktrack || bBlueYonder) // backtrack to last segment, move on and try again
					{
						connTemp.IncrementLastSegment();
					}
					else
					{
						bBlueYonder = FALSE;

						CTDConnection::Decrement(nCurSegment);
						connTemp.AddSegment(nCurSegment);
					}
				}
				// 4. horz intersect with target rect?
				else if (!nHorz && (nHorz != nHorzOrg || nDir == CONN_LEFT || nDir == CONN_RIGHT))
				{
					ASSERT (nHorz != nVert); // can't be both
					
					bCanBacktrack = TRUE;
					bSegmentDone = TRUE;
					bBlueYonder = FALSE;
					
					connTemp.AddSegment(nCurSegment);
				}
				// 5. vert intersect with target rect?
				else if (!nVert && (nVert != nVertOrg || nDir == CONN_UP || nDir == CONN_DOWN)) 
				{
					ASSERT (nHorz != nVert); // can't be both
					
					bCanBacktrack = TRUE;
					bSegmentDone = TRUE;
					bBlueYonder = FALSE;
					
					connTemp.AddSegment(nCurSegment);
				}
				// 6. just keep going
				else
				{
					CTDConnection::Increment(nCurSegment);
				}
			}
		}

		// update min length if the connection is valid
		if (nIteration < MAXITERATIONS)
		{
			// cleanup any unexpected backtracks
			CleanupBacktracks(connTemp);

			// make sure we still end up at the target rect
			if (IntersectRect(connTemp.GetEndPos()) == connTemp.RectTo())
			{
				// test for shortest
				if (connTemp.GetLength() && (!connMin.GetLength() || connTemp.GetLength() < connMin.GetLength()))
					connMin = connTemp;
			}
		}

		// next try
		nIncrement++;
	}

	// pick the shortest route
	conn = connMin;

	return conn.GetLength();
}

void CTextDiagram::CleanupBacktracks(CTDConnection& conn)
{
	CTDConnection temp(conn);
	conn.ResetSegments();
	
	for (int nSeg = 0; nSeg < temp.NumSegments(); nSeg++)
	{
		int nSegment = temp.Segment(nSeg);
		
		// if the next (nSeg + 1) segment is zero
		// then aggregate this with the one after that
		// and omit the zero item
		if (nSeg < temp.NumSegments() - 1 && temp.Segment(nSeg + 1) == 0)
		{
			if (nSeg < temp.NumSegments() - 2)
			{
				nSegment += temp.Segment(nSeg + 2);
				nSeg++;
			}
			
			nSeg++;
		}
		
		conn.AddSegment(nSegment);
	}
}

void CTextDiagram::TraceDiagram(const CStringArray& diagram)
{
#ifdef _DEBUG
	TRACE ("\n");

	for (int nLine = 0; nLine < diagram.GetSize(); nLine++)
		TRACE("%s\n", diagram[nLine]);

	TRACE ("\n");
#endif
}

BOOL CTextDiagram::VerifyDiagram() const
{
	// to verify:
	// 1. get the current diagram as text
	// 2. load it into a temp diagram
	// 3. compare the diagram ro ourselves
	CString sDiagram;
	GetDiagram(sDiagram, FALSE);

	CTextDiagram dgmTemp(sDiagram);

	if (!(*this == dgmTemp))
	{
		TRACE ("Current Diagram\n---------------\n");
		TraceDiagram();
		TRACE ("%d rects, %d connections\n", GetRectCount(), GetConnectionCount());

		TRACE ("\n\nVerified Diagram\n----------------\n");
		dgmTemp.TraceDiagram();
		TRACE ("%d rects, %d connections\n", dgmTemp.GetRectCount(), dgmTemp.GetConnectionCount());

		return FALSE;
	}

	return TRUE;
}

void CTextDiagram::TraceDiagram() const
{
#ifdef _DEBUG
	CStringArray diagram;

	BuildDiagram(diagram);
	TraceDiagram(diagram);
#endif
}

BOOL CTextDiagram::operator==(const CTextDiagram& diagram) const
{
	if (m_aRects.GetSize() != diagram.m_aRects.GetSize())
		return FALSE;

	if (m_aConns.GetSize() != diagram.m_aConns.GetSize())
		return FALSE;

	// rects can be in any order
	for (int nRect = 0; nRect < m_aRects.GetSize(); nRect++)
	{
		if (diagram.FindRect(m_aRects[nRect]) == -1)
			return FALSE;
	}

	// conns can be in any order
	for (int nConn = 0; nConn < m_aConns.GetSize(); nConn++)
	{
		CRect rFrom, rTo;
		const CTDConnection& conn = m_aConns[nConn];

		GetRect(conn.RectFrom(), rFrom);
		GetRect(conn.RectTo(), rTo);

		if (diagram.FindConn(rFrom, rTo) == -1)
			return FALSE;
	}

	return TRUE;
}

int CTextDiagram::FindRect(LPCRECT pRect) const
{
	ASSERT (pRect);

	if (!pRect)
		return -1;

	for (int nRect = 0; nRect < m_aRects.GetSize(); nRect++)
	{
		if (m_aRects[nRect].EqualRect(pRect))
			return nRect;
	}

	return -1;
}

int CTextDiagram::FindConn(LPCRECT pFrom, LPCRECT pTo) const
{
	ASSERT (pFrom && pTo);

	if (!(pFrom && pTo))
		return -1;

	for (int nConn = 0; nConn < m_aConns.GetSize(); nConn++)
	{
		CRect rFrom, rTo;
		const CTDConnection& conn = m_aConns[nConn];

		GetRect(conn.RectFrom(), rFrom);
		GetRect(conn.RectTo(), rTo);

		if (rFrom.EqualRect(pFrom) && rTo.EqualRect(pTo))
			return nConn;
	}

	return -1;
}

const CTextDiagram& CTextDiagram::operator=(const CTextDiagram& diagram)
{
	m_aRects.Copy(diagram.m_aRects);
	m_aConns.Copy(diagram.m_aConns);
	m_size = diagram.m_size;

	SetModified(FALSE);

	return *this;
}

void CTextDiagram::RecalcSize()
{
	CRect rSize(0, 0, 0, 0);

	for (int nRect = 0; nRect < m_aRects.GetSize(); nRect++)
	{
		rSize |= m_aRects[nRect];
	}

	for (int nConn = 0; nConn < m_aConns.GetSize(); nConn++)
	{
		rSize |= m_aConns[nConn].GetBoundingRect();
	}

	m_size.cx = rSize.right;
	m_size.cy = rSize.bottom;
}

BOOL CTextDiagram::CanAddConnection(const CTDRect& rect, int nSide)
{
	int nX, nY;

	switch (nSide)
	{
	case RECT_LEFT:
		for (nY = rect.top; nY <= rect.bottom; nY++)
		{
			if (!PtInConn(rect.left, nY, TRUE))
				return TRUE;
		}
		break;

	case RECT_RIGHT:
		for (nY = rect.top; nY <= rect.bottom; nY++)
		{
			if (!PtInConn(rect.right, nY, TRUE))
				return TRUE;
		}
		break;

	case RECT_TOP:
		for (nX = rect.left; nX <= rect.right; nX++)
		{
			if (!PtInConn(nX, rect.top, FALSE))
				return TRUE;
		}
		break;

	case RECT_BOTTOM:
		for (nX = rect.left; nX <= rect.right; nX++)
		{
			if (!PtInConn(nX, rect.bottom, FALSE))
				return TRUE;
		}
		break;
	}

	return FALSE;
}

CRect CTextDiagram::NewRect()
{
	// work our way out from the origin in rings until we find a spot
	int nRadius = 1;

	while (1)
	{
		for (int nY = 1; nY <= nRadius; nY++)
		{
			for (int nX = 1; nX <= nRadius; nX++)
			{
				CRect rNew(nX, nY, nX + 5, nY + 3);
				rNew.InflateRect(1, 1);

				if (!IntersectRect(rNew))
				{
					rNew.DeflateRect(1, 1);
					return rNew;
				}
			}
		}

		nRadius++;
	}

	return CRect(0, 0, 0, 0);
}

BOOL CTextDiagram::IntersectConn(const CRect& rect) const
{
	// the simplest though probably not the most efficient approach
	// simply to check every point in the rect
	for (int nX = rect.left; nX <= rect.right; nX++)
	{
		for (int nY = rect.top; nY <= rect.bottom; nY++)
		{
			if (PtInConn(nX, nY, TRUE) || PtInConn(nX, nY, FALSE))
				return TRUE;
		}
	}

	return FALSE;
}

