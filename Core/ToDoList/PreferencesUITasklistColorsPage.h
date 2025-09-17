// PreferencesUITasklistPageColors.h : header file
//

#if !defined(AFX_PREFERENCESUITASKLISTCOLORSPAGE_H__9612D6FB_2A00_46DA_99A4_1AC6270F060D__INCLUDED_)
#define AFX_PREFERENCESUITASKLISTCOLORSPAGE_H__9612D6FB_2A00_46DA_99A4_1AC6270F060D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////

#include "tdcstruct.h"
#include "TDLPriorityComboBox.h"

#include "..\shared\colorbutton.h"
#include "..\shared\colorcombobox.h"
#include "..\shared\autocombobox.h"
#include "..\shared\preferencesbase.h"
#include "..\shared\ColorBrewerCombobox.h"

#include "..\3rdparty\fontcombobox.h"

#include <afxtempl.h>

/////////////////////////////////////////////////////////////////////////////

const UINT WM_PUITCP_TEXTCOLOROPTION = ::RegisterWindowMessage(_T("WM_PUITCP_TEXTCOLOROPTION"));

/////////////////////////////////////////////////////////////////////////////
// default colors

const COLORREF DEF_ALTERNATELINECOLOR	= RGB(235, 235, 255);
const COLORREF DEF_TASKDONECOLOR		= RGB(128, 128, 128);
const COLORREF DEF_GRIDLINECOLOR		= RGB(192, 192, 192);
const COLORREF DEF_TASKDUECOLOR			= RGB(255, 0, 0);
const COLORREF DEF_TASKDUETODAYCOLOR	= RGB(255, 128, 0);
const COLORREF DEF_TASKSTARTCOLOR		= RGB(0, 255, 0);
const COLORREF DEF_FLAGGEDCOLOR			= RGB(128, 64, 0);
const COLORREF DEF_REFERENCECOLOR		= RGB(128, 0, 64);
const COLORREF DEF_GROUPHEADERBKCOLOR	= RGB(63, 118, 179);
const COLORREF DEF_PRIORITYLOWCOLOR		= RGB(30, 225, 0);
const COLORREF DEF_PRIORITYHIGHCOLOR	= RGB(255, 0, 0);

/////////////////////////////////////////////////////////////////////////////

enum PUITCP_TEXTCOLOROPTION
{
	TEXTOPT_ATTRIB,
	TEXTOPT_PRIORITY,
	TEXTOPT_DEFAULT,
	TEXTOPT_NONE
};

enum PUITCP_PRIORITYCOLOROPTION
{
	PRIORITYOPT_INDIVIDUAL,
	PRIORITYOPT_GRADIENT,
	PRIORITYOPT_SCHEME,
};

/////////////////////////////////////////////////////////////////////////////
// CPreferencesUITasklistColorsPage dialog

class CPreferencesUITasklistColorsPage : public CPreferencesPageBase
{
// Construction
public:
	CPreferencesUITasklistColorsPage();
	~CPreferencesUITasklistColorsPage();

	void SetDefaultListData(const TDCAUTOLISTDATA& defaultListData);
	void SetNumPriorityRiskLevels(int nNumLevels);

	BOOL GetColorPriority() const { return m_bColorPriority; }
	PUITCP_TEXTCOLOROPTION GetTextColorOption() const { return m_nTextColorOption; }
	int GetPriorityColors(CDWordArray& aColors) const;
	TDC_ATTRIBUTE GetAttributeColors(CTDCColorMap& colors) const;
	TDC_ATTRIBUTE GetColorByAttribute() const { return m_nColorAttribute; }
	BOOL GetTreeFont(CString& sFaceName, int& nPointSize) const;
	BOOL IncrementTreeFontSize(BOOL bLarger, HFONT hFontFallback);
	BOOL CanIncrementTreeFontSize(BOOL bLarger, HFONT hFontFallback) const;
	BOOL RestoreTreeFontSize(HFONT hFontDefault);
	BOOL GetCommentsFont(CString& sFaceName, int& nPointSize) const;
	COLORREF GetGridlineColor() const { return m_bSpecifyGridColor ? m_crGridlines : CLR_NONE; }
	COLORREF GetDoneTaskColor() const { return m_bSpecifyDoneColor ? m_crDone : CLR_NONE; }
	COLORREF GetAlternateLineColor() const { return m_bSpecifyAlternateLineColor ? m_crAltLine : CLR_NONE; }
	void GetDueTaskColors(COLORREF& crDue, COLORREF& crDueToday) const;
	void GetStartedTaskColors(COLORREF& crStarted, COLORREF& crStartedToday) const;
	BOOL GetColorTaskBackground() const { return m_bColorTaskBackground; }
	BOOL GetCommentsUseTreeFont() const { return m_bSpecifyTreeFont && m_bCommentsUseTreeFont; }
	BOOL GetRemindersUseTreeFont() const { return m_bSpecifyTreeFont && m_bRemindersUseTreeFont; }
	BOOL GetFindTasksUseTreeFont() const { return m_bSpecifyTreeFont && m_bFindTasksUseTreeFont; }
	COLORREF GetFlaggedTaskColor() const { return m_bSpecifyFlaggedColor ? m_crFlagged : CLR_NONE; }
	COLORREF GetReferenceTaskColor() const { return m_bSpecifyReferenceColor ? m_crReference : CLR_NONE; }
	COLORREF GetGroupHeaderBackgroundColor() const { return m_bSpecifyGroupHeaderBkgndColor ? m_crGroupHeaderBkgnd : CLR_NONE; }

protected:
// Dialog Data
	//{{AFX_DATA(CPreferencesUITasklistColorsPage)
	//}}AFX_DATA

	CComboBox m_cbColorByAttribute;
	CComboBox m_cbTreeFontSize, m_cbCommentsFontSize;
	CColorComboBox m_cbAttributes;
	CTDLPriorityComboBox m_cbPriorityColors;
	CColorBrewerComboBox m_cbPriorityScheme;
	CFontNameComboBox m_cbTreeFonts, m_cbCommentsFonts;

	BOOL m_bColorTaskBackground;
	BOOL m_bHLSColorGradient;
	BOOL m_bSpecifyAlternateLineColor;
	BOOL m_bSpecifyDueColor;
	BOOL m_bSpecifyDueTodayColor;
	BOOL m_bSpecifyStartColor;
	BOOL m_bSpecifyStartTodayColor;
	BOOL m_bSpecifyGridColor;
	BOOL m_bSpecifyDoneColor;
	BOOL m_bSpecifyFlaggedColor;
	BOOL m_bSpecifyReferenceColor;
	BOOL m_bSpecifyGroupHeaderBkgndColor;
	BOOL m_bCommentsUseTreeFont;
	BOOL m_bRemindersUseTreeFont;
	BOOL m_bFindTasksUseTreeFont;
	BOOL m_bSpecifyTreeFont;
	BOOL m_bSpecifyCommentsFont;
	BOOL m_bColorPriority;

	CColourButton m_btFilteredColor;
	CColourButton m_btAttribColor;
	CColourButton m_btDoneColor;
	CColourButton m_btGridlineColor;
	CColourButton m_btDueColor;
	CColourButton m_btDueTodayColor;
	CColourButton m_btStartColor;
	CColourButton m_btStartTodayColor;
	CColourButton m_btFlaggedColor;
	CColourButton m_btReferenceColor;
	CColourButton m_btGroupHeaderBkgndColor;
	CColourButton m_btPriorityColor;
	CColourButton m_btPriorityLowColor;
	CColourButton m_btPriorityHighColor;
	CColourButton m_btAltLineColor;

	CDWordArray m_aPriorityColors;
	CDWordArray m_aPriorityScheme;

	int m_nSelPriorityColor;
	int m_nTreeFontSize;
	int m_nCommentsFontSize;
	int m_nNumPriorityRiskLevels;

	CString	m_sSelAttribValue;
	CString m_sTreeFont;
	CString m_sCommentsFont;

	COLORREF m_crPriorityLow, m_crPriorityHigh;
	COLORREF m_crGridlines, m_crDone;
	COLORREF m_crAltLine;
	COLORREF m_crDue, m_crDueToday;
	COLORREF m_crStart, m_crStartToday;
	COLORREF m_crFlagged;
	COLORREF m_crReference;
	COLORREF m_crGroupHeaderBkgnd;

	TDC_ATTRIBUTE m_nColorAttribute;
	PUITCP_TEXTCOLOROPTION m_nTextColorOption;
	PUITCP_PRIORITYCOLOROPTION m_nPriorityColorOption;

	TDCAUTOLISTDATA m_defaultListData;

	struct ATTRIBCOLOR
	{
		CString sAttrib;
		COLORREF color;
	};

	CArray<ATTRIBCOLOR, ATTRIBCOLOR&> m_aAttribColors;

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
	afx_msg void OnSpecifyGroupHeaderBkgndcolor();
	afx_msg void OnSetGroupHeaderBkgndcolor();

	DECLARE_MESSAGE_MAP()

protected:
	int FindAttribValue(LPCTSTR szAttrib);
	void AddDefaultListItemsToAttributeColors();
	int GetNextTreeFontIncrement(int nPointSize, BOOL bLarger) const;
	BOOL GetTreeFont(CString& sFaceName, int& nPointSize, HFONT hFontFallback) const;
		
	virtual void LoadPreferences(const IPreferences* pPrefs, LPCTSTR szKey);
	virtual void SavePreferences(IPreferences* pPrefs, LPCTSTR szKey) const;

	static void GetDefaultPriorityColors(CDWordArray& aColors);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PREFERENCESUITASKLISTPAGE_H__9612D6FB_2A00_46DA_99A4_1AC6270F060D__INCLUDED_)
