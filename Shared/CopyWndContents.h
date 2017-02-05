#pragma once

class CCopyWndContents
{
public:
	virtual ~CCopyWndContents();

	BOOL DoCopy(CBitmap& bmp);

protected:
	CCopyWndContents(CWnd& wnd);

protected:
	CWnd& m_wnd;

protected:
	virtual int PageDown(int nCurVertPos);
	virtual int PageRight(int nCurHorzPos);
	virtual void DoPrint(CDC& dc);
	virtual CSize CalcContentsSize() const;
	virtual CSize CalcPageSize() const;
	virtual void DoPageDown();
	virtual void DoPageRight();

	CSize GetContentsSize() const { return m_sizeContent; }
	CSize GetPageSize() const { return m_sizePage; }

	static void DoPrint(HWND hwnd, HDC hdc, DWORD dwFlags = PRF_CLIENT);

private:
	CSize m_sizeContent, m_sizePage;

private:
};

class CCopyTreeCtrlContents : public CCopyWndContents
{
public:
	CCopyTreeCtrlContents(CTreeCtrl& tree);
	virtual ~CCopyTreeCtrlContents();

protected:
	int m_nItemHeight;
	CTreeCtrl& m_tree;

protected:
	virtual void DoPageDown();
	virtual CSize CalcContentsSize() const;
	virtual CSize CalcPageSize() const;
};

class CCopyListCtrlContents : public CCopyWndContents
{
public:
	CCopyListCtrlContents(CListCtrl& list);
	virtual ~CCopyListCtrlContents();

protected:
	int m_nItemHeight;
	CListCtrl& m_list;

protected:
	virtual int PageDown(int nCurVertPos);
	virtual void DoPageDown();
	virtual void DoPrint(CDC& dc);
	virtual CSize CalcContentsSize() const;
	virtual CSize CalcPageSize() const;

	// helpers
	int CalcHeaderHeight() const;
};

class CCopyHeaderCtrlContents : public CCopyWndContents
{
public:
	CCopyHeaderCtrlContents(CHeaderCtrl& hdr);
	virtual ~CCopyHeaderCtrlContents();
};

class CCopyEditContents : public CCopyWndContents
{
public:
	CCopyEditContents(CEdit& edit);
	virtual ~CCopyEditContents();

protected:
	int m_nLineHeight, m_nLineCount;

protected:
	virtual void DoPageDown();
	virtual CSize CalcContentsSize() const;
	virtual CSize CalcPageSize() const;
};
