// SaveFocus.h: interface for the CSaveFocus class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SAVEFOCUS_H__9FCD89DF_6CFF_40CC_A54A_F6744399CDF0__INCLUDED_)
#define AFX_SAVEFOCUS_H__9FCD89DF_6CFF_40CC_A54A_F6744399CDF0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CSaveFocus  
{
public:
	CSaveFocus();
	virtual ~CSaveFocus();

protected:
	HWND m_hwnd;
	CHARRANGE m_crSel;
	BOOL m_bEdit;
};

#endif // !defined(AFX_SAVEFOCUS_H__9FCD89DF_6CFF_40CC_A54A_F6744399CDF0__INCLUDED_)
