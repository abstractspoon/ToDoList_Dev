#if !defined(AFX_ICON_H__02B03EF2_F6B2_4A6B_A254_591BD1283FCE__INCLUDED_)
#define AFX_ICON_H__02B03EF2_F6B2_4A6B_A254_591BD1283FCE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// fileedit.h : header file
//

class CIcon
{
public:
	CIcon(HICON hIcon = NULL);
	virtual ~CIcon();

	BOOL LoadIcon(UINT nIDIcon, int nSize = 16);

	BOOL IsValid() const;
	BOOL SetIcon(HICON hIcon, BOOL bDeletePrev = TRUE);

	BOOL Attach(HICON hIcon);
	HICON Detach();

	operator HICON() const { return m_hIcon; }
	operator HICON() { return m_hIcon; }

protected:
	HICON m_hIcon;
};


#endif // AFX_ICON_H__02B03EF2_F6B2_4A6B_A254_591BD1283FCE__INCLUDED_