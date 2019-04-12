// TextDiagram.h: interface for the CTextDiagram class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TEXTDIAGRAM_H__F278ECE0_2D7D_49BC_8177_332A79991F34__INCLUDED_)
#define AFX_TEXTDIAGRAM_H__F278ECE0_2D7D_49BC_8177_332A79991F34__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>

class CTDRect : public CRect
{
public:
	CTDRect(const CRect& rect, LPCTSTR szText = NULL);
	CTDRect(const CTDRect& rect) { *this = rect; }
	CTDRect();
	virtual ~CTDRect();

	const CTDRect& operator=(const CTDRect& rect);

	int GetSide(CPoint point);

	inline void SetText(LPCTSTR szText) { m_sText = szText; }
	inline const CString& GetText() const { return m_sText; }
	inline int GetTextLen() const { return m_sText.GetLength(); }

protected:
	CString m_sText;
};

enum { TDCS_LEFT, TDCS_TOP, TDCS_RIGHT, TDCS_BOTTOM };

class CTDConnection
{
public:
	CTDConnection(int nRectFrom, int nRectTo, int nSideFrom);
	CTDConnection(const CTDConnection& conn) { *this = conn; }
	CTDConnection();
	virtual ~CTDConnection();

	const CTDConnection& operator=(const CTDConnection& conn);

	inline int RectFrom() const { return m_nRectFrom; }
	inline int RectTo() const { return m_nRectTo; }
	inline int SideFrom() const { return m_nSideFrom; }

	void SetRectFrom(int nRect) { m_nRectFrom = nRect; }
	void SetRectTo(int nRect) { m_nRectTo = nRect; }

	void SetStartPos(CPoint ptStart) { m_ptFrom = ptStart; }
	inline CPoint GetStartPos()  const { return m_ptFrom; }
	CPoint GetEndPos(int nNextSegment = 0) const;
	int GetEndDirection(int nNextSegment = 0) const;
	int GetStartDirection() const;
	BOOL PtInConn(CPoint point, BOOL bHorz) const;

	CRect GetBoundingRect() const;
	int GetLength() const;

	void AddSegment(int nSegment);
	void IncrementLastSegment();
	void ResetSegments() { m_aSegments.RemoveAll(); }
	inline int NumSegments() const { return m_aSegments.GetSize(); }
	inline int Segment(int nSegment) const { return (nSegment >= 0 && nSegment < NumSegments()) ? m_aSegments[nSegment] : 0; }
	CPoint GetSegmentPos(int nSegment) const;

	static void Increment(int& nSegment);
	static void Decrement(int& nSegment);

protected:
	int m_nRectFrom, m_nRectTo;
	int m_nSideFrom;
	CPoint m_ptFrom;
	CArray<int, int&> m_aSegments;

};

typedef enum // pos
{
	NONE = -1,

	RECT_LEFT,
	RECT_TOP,
	RECT_RIGHT,
	RECT_BOTTOM,

	CONN_START,
	CONN_END,
	CONN_UP,
	CONN_DOWN,
	CONN_LEFT,
	CONN_RIGHT,

} TDPOS;

class CTextDiagram  
{
public:
	CTextDiagram(LPCTSTR szDiagram = NULL);
	CTextDiagram(const CTextDiagram& diagram) { SetDiagram(diagram); }
	virtual ~CTextDiagram();

	virtual void SetDiagram(const CTextDiagram& diagram);
	void SetDiagram(LPCTSTR szDiagram);
	void SetDiagram(const CStringArray& diagram);
	BOOL GetDiagram(CString& sDiagram, BOOL bVerify = TRUE) const;
	BOOL GetDiagram(CStringArray& diagram, BOOL bVerify = TRUE) const;

	void ResetDiagram();
	BOOL IsEmpty() { return (GetRectCount() == 0); } 

	BOOL IsModified() { return m_bModified; }
	void SetModified(BOOL bModified = TRUE) { m_bModified = bModified; }

	const CString& GetTitle() { return m_sTitle; }
	void SetTitle(LPCTSTR szTitle) { m_sTitle = szTitle; SetModified(); }

	inline int GetRectCount() const { return m_aRects.GetSize(); }
	virtual int AddRect(LPCRECT pRect = NULL, LPCTSTR szText = NULL);
	virtual BOOL DeleteRect(int nRect);
	virtual BOOL SetRect(int nRect, const CTDRect& rect);
	BOOL GetRect(int nRect, CTDRect& rect) const;
	inline BOOL IsValidRect(int nRect) const { return nRect >= 0 && nRect < GetRectCount(); }

	virtual BOOL SetText(int nRect, LPCTSTR szText);
	virtual BOOL GetText(int nRect, CString& sText);

	inline int GetConnectionCount() const { return m_aConns.GetSize(); }
	virtual int AddConnection(int nRectFrom, int nRectTo, int nSideFrom);
	virtual BOOL DeleteConnection(int nConn);
	virtual BOOL SetConnection(int nConn, const CTDConnection& conn);
	BOOL GetConnection(int nConn, CTDConnection& conn) const;
	int FindConnection(int nRectFrom, int nRectTo) const;
	inline BOOL IsValidConnection(int nConn) const { return nConn >= 0 && nConn < GetConnectionCount(); }

	BOOL VerifyDiagram() const;

	static int ConvertDiagram(LPCTSTR szDiagram, CStringArray& aDiagram); // returns number of lines

protected:
	CString m_sTitle;
	CArray<CTDRect, CTDRect&> m_aRects;
	CArray<CTDConnection, CTDConnection&> m_aConns;
	CSize m_size;
	BOOL m_bModified;

protected:
	BOOL operator==(const CTextDiagram& diagram) const;
	const CTextDiagram& operator=(const CTextDiagram& diagram);

	void ProcessDiagram(const CStringArray& diagram);
	void BuildDiagram(CStringArray& diagram) const;

	void AddRect(const CStringArray& diagram, int nX, int nY);
	TDPOS RectGetNextChar(const CStringArray& diagram, TDPOS nCurPos, int& nX, int& nY) const;
	TDPOS RectStartChar(const CStringArray& diagram, int nX, int nY) const;
	void DrawRect(CStringArray& diagram, const CTDRect& rect) const;
	BOOL GetRect(int nRect, CRect& rect) const;
	CRect NewRect(); 

	void AddConn(const CStringArray& diagram, int nX, int nY);
	TDPOS ConnStartChar(const CStringArray& diagram, int nX, int nY) const;
	TDPOS ConnGetNextChar(const CStringArray& diagram, TDPOS nCurPos, int& nX, int& nY) const;
	void DrawConn(CStringArray& diagram, const CTDConnection& conn) const;

	static void GetRelationship(const CRect& rFrom, const CRect& rTo, int& nHorz, int& nVert);
	static void GetRelationship(CPoint ptFrom, const CRect& rTo, int& nHorz, int& nVert);
	static void GetRelationship(CPoint ptFrom, CPoint ptTo, int& nHorz, int& nVert);

	BOOL PtInRect(int nX, int nY) const;
	BOOL PtInConn(int nX, int nY, BOOL bHorz) const;
	BOOL IntersectRect(const CRect& rect, int nIgnoreRect = -1) const;
	BOOL IntersectConn(const CRect& rect) const;
	int IntersectConn(CPoint point, BOOL bHorz) const;
	int IntersectRect(int nX, int nY) const;
	int IntersectRect(CPoint point) const { return IntersectRect(point.x, point.y); }

	void DrawLine(CStringArray& diagram, CPoint ptFrom, CPoint ptTo, const CTDConnection* pConn = NULL) const;
	int PickStartPoint(const CTDConnection& conn, int nIncrement, CPoint& ptStart, int nIgnoreConn = -1) const;
	void RecalcSize();
	BOOL CanAddConnection(const CTDRect& rect, int nSide);
	virtual BOOL RebuildConnections();
	virtual int BuildPath(CTDConnection& conn, int nIgnoreConn);
	void AddTitle(CStringArray& diagram) const;
	void CleanupBacktracks(CTDConnection& conn);

	static int LineLength(const CStringArray& diagram, int nY);
	static BOOL TestChar(const CStringArray& diagram, int nX, int nY, char cTest1, char cTest2 = 0, char cTest3 = 0, char cTest4 = 0);
	static void ExpandTabs(CString& sLine, int nTabSize = 4);
	static char GetChar(const CStringArray& diagram, int nX, int nY);
	static BOOL SetChar(CStringArray& diagram, int nX, int nY, char c);

	// debugging aids
	void TraceDiagram() const;
	static void TraceDiagram(const CStringArray& diagram);

	// to help verification
	int FindRect(LPCRECT pRect) const;
	int FindConn(LPCRECT pFrom, LPCRECT pTo) const;

};

#endif // !defined(AFX_TEXTDIAGRAM_H__F278ECE0_2D7D_49BC_8177_332A79991F34__INCLUDED_)
