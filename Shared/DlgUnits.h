// DlgUnits.h: interface for the CDlgUnits class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DLGUNITS_H__18921FC5_A659_4AA7_BDF8_EF630D0D13E6__INCLUDED_)
#define AFX_DLGUNITS_H__18921FC5_A659_4AA7_BDF8_EF630D0D13E6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDlgUnits  
{
public:
	CDlgUnits(HWND hWnd = NULL, BOOL bDlgParent = FALSE);
	CDlgUnits(const CWnd* pWnd, BOOL bDlgParent = FALSE);
	virtual ~CDlgUnits();

	int ToPixelsX(int x) const;
	int ToPixelsY(int y) const;
	int FromPixelsX(int x) const;
	int FromPixelsY(int y) const;

	void ToPixels(long& x, long& y) const;
	void ToPixels(int& x, int& y) const;
	void ToPixels(short& x, short& y) const;
	void ToPixels(POINT& point) const;
	void ToPixels(RECT& rect) const;
	void ToPixels(SIZE& size) const;
	void FromPixels(long& x, long& y) const;
	void FromPixels(int& x, int& y) const;
	void FromPixels(short& x, short& y) const;
	void FromPixels(POINT& point) const;
	void FromPixels(RECT& rect) const;
	void FromPixels(SIZE& size) const;

protected:
	CSize m_dlgBaseUnits;
	HWND m_hWnd;

protected:
	void Initialise(HWND hWnd, BOOL bDlgParent);

};

#endif // !defined(AFX_DLGUNITS_H__18921FC5_A659_4AA7_BDF8_EF630D0D13E6__INCLUDED_)
