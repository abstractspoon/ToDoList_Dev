#if !defined(AFX_PREFERENCESUITASKLISTCOLORSPAGE_H__9612D6FB_2A00_46DA_99A4_1AC6270F060D__INCLUDED_)
#define AFX_PREFERENCESUITASKLISTCOLORSPAGE_H__9612D6FB_2A00_46DA_99A4_1AC6270F060D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PreferencesUITasklistPageColors.h : header file
//

#include "tdcstruct.h"

#include "..\shared\colorbutton.h"
#include "..\shared\colorcombobox.h"
#include "..\shared\autocombobox.h"
#include "..\shared\preferencesbase.h"
#include "..\shared\ColorBrewerCombobox.h"

#include "..\3rdparty\fontcombobox.h"

#include <afxtempl.h>

/////////////////////////////////////////////////////////////////////////////

const UINT WM_PUITCP_POPULATEATTRIB = ::RegisterWindowMessage(_T("WM_PUITCP_POPULATEATTRIB"));

/////////////////////////////////////////////////////////////////////////////

enum { COLOROPT_ATTRIB, COLOROPT_PRIORITY, COLOROPT_DEFAULT, COLOROPT_NONE };

struct ATTRIBCOLOR
{
	CString sAttrib;
	COLORREF color;
};
typedef CArray<ATTRIBCOLOR, ATTRIBCOLOR&> CAttribColorArray;

/////////////////////////////////////////////////////////////////////////////
// CPreferencesUITasklistColorsPage dialog

class CPreferencesUITasklistColorsPage : public CPreferencesPageBase
{
	//DECLARE_DYNCREATE(CPreferencesUITasklistColorsPage)

// Construction
public:
	CPreferencesUITasklistColorsPage();
	~CPreferencesUITasklistColorsPage();

	void SetDefaultListData(const TDCAUTOLISTDATA& defaultListData);

	BOOL GetColorPriority() const { return m_bColorPriority; }
	int GetTextColorOption() const { return m_nTextColorOption; }
	BOOL GetHidePriorityNumber() const { return m_bHidePriorityNumber; }
	int GetPriorityColors(CDWordArray& aColors) const;
	TDC_ATTRIBUTE GetAttributeColors(CTDCColorMap& colors) const;
	TDC_ATTRIBUTE GetColorByAttribute() const { return m_nColorAttribute; }
	BOOL GetTreeFont(CString& sFaceName, int& nPointSize) const;
	BOOL IncrementTreeFontSize(BOOL bLarger, HFONT hFontFallback);
	BOOL CanIncrementTreeFontSize(BOOL bLarger, HFONT hFontFallback) const;
	BOOL RestoreTreeFontSize(HFONT hFontDefault);
	BOOL GetCommentsFont(CString& sFaceName, int& nPointSize) const;
	COLORREF GetGridlineColor() const { return m_bSpecifyGridColor ? m_crGridlines : -1; }
	COLORREF GetDoneTaskColor() const { return m_bSpecifyDoneColor ? m_crDone : -1; }
	COLORREF GetAlternateLineColor() const { return m_bAlternateLineColor ? m_crAltLine : -1; }
	void GetDueTaskColors(COLORREF& crDue, COLORREF& crDueToday) const;
	void GetStartedTaskColors(COLORREF& crStarted, COLORREF& crStartedToday) const;
	BOOL GetColorTaskBackground() const { return m_bColorTaskBackground; }
	BOOL GetCommentsUseTreeFont() const { return m_bSpecifyTreeFont && m_bCommentsUseTreeFont; }
	COLORREF GetFlaggedTaskColor() const { return m_bSpecifyFlaggedColor ? m_crFlagged : -1; }
	COLORREF GetReferenceTaskColor() const { return m_bSpecifyReferenceColor ? m_crReference : -1; }

protected:
// Dialog Data
	//{{AFX_DATA(CPreferencesUITasklistColorsPage)
	enum { IDD = IDD_PREFUITASKLISTCOLORS_PAGE };
	CComboBox	m_cbColorByAttribute;
	BOOL	m_bColorTaskBackground;
	BOOL	m_bCommentsUseTreeFont;
	BOOL	m_bHLSColorGradient;
	BOOL	m_bHidePriorityNumber;
	BOOL	m_bAlternateLineColor;
	int		m_nTextColorOption;
	//}}AFX_DATA
	CColorComboBox m_cbAttributes;
	CString	m_sSelAttribValue;
	CColorComboBox m_cbPriorityColors;
	CColorBrewerComboBox	m_cbPriorityScheme;
	BOOL	m_bSpecifyDueColor;
	BOOL	m_bSpecifyDueTodayColor;
	BOOL	m_bSpecifyStartColor;
	BOOL	m_bSpecifyStartTodayColor;
	BOOL	m_bSpecifyGridColor;
	BOOL	m_bSpecifyDoneColor;
	BOOL	m_bSpecifyFlaggedColor; 
	BOOL	m_bSpecifyReferenceColor;
	CColourButton	m_btFilteredColor;
	CColourButton	m_btAttribColor;
	CColourButton	m_btDoneColor;
	CColourButton	m_btGridlineColor;
	CColourButton	m_btDueColor;
	CColourButton	m_btDueTodayColor;
	CColourButton	m_btStartColor;
	CColourButton	m_btStartTodayColor;
	CColourButton	m_btFlaggedColor;
	CColourButton	m_btReferenceColor;
	CComboBox	m_cbTreeFontSize, m_cbCommentsFontSize;
	CFontNameComboBox	m_cbTreeFonts, m_cbCommentsFonts;
	BOOL	m_bSpecifyTreeFont;
	BOOL	m_bSpecifyCommentsFont;
	CColourButton	m_btSetColor;
	CColourButton	m_btLowColor;
	CColourButton	m_btHighColor;
	BOOL	m_bColorPriority;
	int		m_nPriorityColorOption;
	int		m_nSelPriorityColor;
	BOOL	m_bShowTimeColumn;
	CDWordArray m_aPriorityColors;
	CDWordArray m_aPriorityScheme;
	CAttribColorArray m_aAttribColors;
	COLORREF m_crLow, m_crHigh;
	CString m_sTreeFont;
	int		m_nTreeFontSize;
	CString m_sCommentsFont;
	int		m_nCommentsFontSize;
	COLORREF m_crGridlines, m_crDone;
	CColourButton	m_btAltLineColor;
	COLORREF m_crAltLine;
	COLORREF m_crDue, m_crDueToday;
	COLORREF m_crStart, m_crStartToday;
	COLORREF m_crFlagged;
	COLORREF m_crReference;
	TDC_ATTRIBUTE m_nColorAttribute;

	TDCAUTOLISTDATA m_defaultListData;

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPreferencesUITasklistColorsPage)
	//}}AFX_VIRTUAL

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	virtual void OnFirstShow();

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPreferencesUITasklistColorsPage)
	afx_msg void OnAltlinecolor();
	afx_msg void OnSpecifyAlternatelinecolor();
	afx_msg void OnChangeTextColorOption();
	afx_msg void OnCommentsusetreefont();
	afx_msg void OnSpecifycommentsfont();
	afx_msg void OnPopulateattriblist();
	afx_msg void OnSelchangeAttributetocolorby();
	//}}AFX_MSG
	afx_msg void OnSetAttribValuecolor();
	afx_msg void OnEditAttribValue();
	afx_msg void OnSelchangeAttribValue();
	afx_msg void OnSelchangeCommentsFont();
	afx_msg void OnSelchangeTreeFont();
	afx_msg void OnStarttaskcolor();
	afx_msg void OnSetStarttaskcolor();
	afx_msg void OnStarttodaytaskcolor();
	afx_msg void OnSetStarttodaytaskcolor();
	afx_msg void OnDuetaskcolor();
	afx_msg void OnSetduetaskcolor();
	afx_msg void OnDuetodaytaskcolor();
	afx_msg void OnSetduetodaytaskcolor();
	afx_msg void OnSpecifytreefont();
	afx_msg void OnSetgridlinecolor();
	afx_msg void OnSpecifygridlinecolor();
	afx_msg void OnSetdonecolor();
	afx_msg void OnSpecifydonecolor();
	afx_msg void OnLowprioritycolor();
	afx_msg void OnHighprioritycolor();
	afx_msg void OnSetprioritycolor();
	afx_msg void OnChangePriorityColorOption();
	afx_msg void OnColorPriority();
	afx_msg void OnSelchangePrioritycolors();
	afx_msg LRESULT OnAttribValueAdded(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnAttribValueDeleted(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSpecifyflaggedcolor();
	afx_msg void OnSetflaggedcolor();
	afx_msg void OnSpecifyReferencecolor();
	afx_msg void OnSetReferencecolor();

	DECLARE_MESSAGE_MAP()

protected:
	int FindAttribValue(LPCTSTR szAttrib);
	void AddDefaultListItemsToAttributeColors();
	int GetNextTreeFontIncrement(int nPointSize, BOOL bLarger) const;
	BOOL GetTreeFont(CString& sFaceName, int& nPointSize, HFONT hFontFallback) const;
		
	virtual void LoadPreferences(const IPreferences* pPrefs, LPCTSTR szKey);
	virtual void SavePreferences(IPreferences* pPrefs, LPCTSTR szKey) const;

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PREFERENCESUITASKLISTPAGE_H__9612D6FB_2A00_46DA_99A4_1AC6270F060D__INCLUDED_)
