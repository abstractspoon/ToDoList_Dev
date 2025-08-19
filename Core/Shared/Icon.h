#if !defined(AFX_ICON_H__02B03EF2_F6B2_4A6B_A254_591BD1283FCE__INCLUDED_)
#define AFX_ICON_H__02B03EF2_F6B2_4A6B_A254_591BD1283FCE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CIcon
{
public:
	CIcon();
	CIcon(HICON hIcon, BOOL bOwned = TRUE);
	CIcon(UINT nIDIcon, int nSize = 16, BOOL bScaleByDPI = TRUE);
	virtual ~CIcon();

	BOOL Load(UINT nIDIcon, int nSize = 16, BOOL bScaleByDPI = TRUE);
	void Destroy();

	BOOL IsValid() const;
	BOOL SetIcon(HICON hIcon, BOOL bOwned = TRUE);

	BOOL Attach(HICON hIcon, BOOL bOwned = TRUE);
	HICON Detach();

	BOOL Draw(CDC* pDC, const CPoint& ptTopLeft) const;
	CSize GetSize() const { return m_size; }

	operator HICON() const { return m_hIcon; }
	operator HICON() { return m_hIcon; }

protected:
	HICON m_hIcon;
	CSize m_size;
	BOOL m_bOwned;
};


#endif // AFX_ICON_H__02B03EF2_F6B2_4A6B_A254_591BD1283FCE__INCLUDED_