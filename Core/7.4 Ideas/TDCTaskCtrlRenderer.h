// TDCTreeListCtrl.h: interface for the CTDCTaskCtrlBase class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TDCTASKCTRLRENDERER_H__155791A3_22AC_4083_B933_F39E9EBDADEF__INCLUDED_)
#define AFX_TDCTASKCTRLRENDERER_H__155791A3_22AC_4083_B933_F39E9EBDADEF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////

#include "tdcmsg.h"
#include "tdcstruct.h"
#include "todoctrlfind.h"
#include "todoctrldata.h"

/////////////////////////////////////////////////////////////////////////////

#include "..\Shared\EnHeaderCtrl.h"
#include "..\Shared\TreeListSyncer.h"
#include "..\Shared\Treeselectionhelper.h"
#include "..\Shared\runtimedlg.h"
#include "..\shared\sysimagelist.h"
#include "..\shared\iconcache.h"
#include "..\shared\fontcache.h"
#include "..\shared\graphicsmisc.h"
#include "..\shared\tooltipctrlex.h"
#include "..\Shared\enimagelist.h"

/////////////////////////////////////////////////////////////////////////////

class CTreeCtrlHelper;
class CToDoCtrlData;
class CPreferences;
class CTDCImageList;

/////////////////////////////////////////////////////////////////////////////

class CTDCTaskCtrlRenderer
{
public:
	CTDCTaskCtrlRenderer(const CTDCImageList& ilIcons,
						 const CToDoCtrlData& data,
						 const CTDCColumnMap& mapColumns,
						 const CTDCColumnIDMap& mapVisibleCols,
						 const CTDCCustomAttribDefinitionArray& aCustAttribDefs);

	virtual ~CTDCTaskCtrlRenderer();

	// setter responsible for deleting these shared resources
	const CImageList& GetCheckImageList() const { return m_ilCheckboxes; }

	BOOL GetTaskTextColors(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, COLORREF& crText, 
							COLORREF& crBack, BOOL bRef = -1) const;
	BOOL GetTaskTextColors(DWORD dwTaskID, COLORREF& crText, COLORREF& crBack, BOOL bRef = -1) const;

	int GetTaskIconIndex(DWORD dwTaskID) const;
	int GetTaskIconIndex(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS) const;

	BOOL SetAlternateLineColor(COLORREF crAltLine);
	BOOL SetGridlineColor(COLORREF crGridLine);
	BOOL SetPriorityColors(const CDWordArray& aColors); // must have 12 elements
	void GetPriorityColors(CDWordArray& aColors) const { aColors.Copy(m_aPriorityColors); }
	COLORREF GetPriorityColor(int nPriority) const;
	BOOL SetCompletedTaskColor(COLORREF color);
	COLORREF GetCompletedTaskColor() const { return m_crDone; }
	BOOL SetFlaggedTaskColor(COLORREF color);
	BOOL SetReferenceTaskColor(COLORREF color);
	BOOL SetAttributeColors(TDC_ATTRIBUTE nAttrib, const CTDCColorMap& colors);
	TDC_ATTRIBUTE GetColorByAttribute() const  { return m_nColorByAttrib; }
	BOOL SetStartedTaskColors(COLORREF crStarted, COLORREF crStartedToday);
	void GetStartedTaskColors(COLORREF& crStarted, COLORREF& crStartedToday) { crStarted = m_crStarted; crStartedToday = m_crStartedToday; }
	BOOL SetDueTaskColors(COLORREF crDue, COLORREF crDueToday);
	void GetDueTaskColors(COLORREF& crDue, COLORREF& crDueToday) const { crDue = m_crDue; crDueToday = m_crDueToday; }
	BOOL HasDueTodayColor() const { return (m_crDueToday != CLR_NONE); }

	void SetSavingToImage(BOOL bSavingToImage) { m_bSavingToImage = bSavingToImage; }
	void SetReadOnly(bool bReadOnly) { m_bReadOnly = bReadOnly; }

	void SetTimeTrackTaskID(DWORD dwTaskID);
	BOOL SetCompletionStatus(const CString& sStatus);

protected:
	const CToDoCtrlData& m_data;
	const CTDCImageList& m_ilTaskIcons;
	const CTDCColumnMap& m_mapColumns;
	const CTDCColumnIDMap& m_mapVisibleCols;
	const CTDCCustomAttribDefinitionArray& m_aCustomAttribDefs;

	BOOL m_bReadOnly;
	BOOL m_bSavingToImage;

	CString m_sCompletionStatus;
	DWORD m_dwTimeTrackTaskID;
	float m_fAveHeaderCharWidth;
	CString m_sTasklistFolder;
	TDCDATETIMEWIDTHS m_dateTimeWidths;

	CTDCTaskComparer m_comparer;
	CTDCTaskCalculator m_calculator;
	CTDCTaskFormatter m_formatter;

	// font/color related
	COLORREF m_crAltLine, m_crGridLine, m_crDone;
	COLORREF m_crStarted, m_crStartedToday, m_crDue; 
	COLORREF m_crDueToday, m_crFlagged, m_crReference;
	CDWordArray m_aPriorityColors;
	CTDCColorMap m_mapAttribColors;
	TDC_ATTRIBUTE m_nColorByAttrib;
	CSysImageList m_ilFileRef;
	CBrush m_brDue, m_brDueToday;
	CFontCache m_fonts;
	CEnImageList m_ilCheckboxes, m_ilColSymbols;
	CIconCache m_imageIcons;

	static double s_dRecentModPeriod;

protected:
	//BOOL IsReadOnly() const { return m_data.HasStyle(TDCS_READONLY); }
	BOOL GetAttributeColor(const CString& sAttrib, COLORREF& color) const;
//	BOOL HasThemedState(GM_ITEMSTATE nState) const;
	CPoint CalcColumnIconTopLeft(const CRect& rSubItem, int nImageSize = 16, int nImage = 0, int nCount = 1) const;
	BOOL CalcFileIconRect(const CRect& rSubItem, CRect& rIcon, int nImage = 0, int nCount = 1) const;

	CString GetTaskColumnText(DWORD dwTaskID, const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, TDC_COLUMN nColID, BOOL bDrawing) const;
	CString FormatTaskDate(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, TDC_DATE nDate) const;
	CString FormatDate(const COleDateTime& date, TDC_DATE nDate) const;
	BOOL FormatDate(const COleDateTime& date, TDC_DATE nDate, CString& sDate, CString& sTime, CString& sDow, BOOL bCustomWantsTime = FALSE) const;

protected:
	BOOL WantDrawColumnTime(TDC_DATE nDate, BOOL bCustomWantsTime = FALSE) const;
	void DrawColumn(CDC* pDC, DWORD dwTaskID, const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS,
					const TDCCOLUMN& col, const CRect& rect, COLORREF crText);
	void DrawColumnText(CDC* pDC, const CString& sText, const CRect& rect, int nAlign,
						COLORREF crText, BOOL bTaskTitle = FALSE, int nTextLen = -1);
	void DrawColumnFileLinks(CDC* pDC, const CStringArray& aFileLinks, const CRect& rect, COLORREF crText);
	void DrawColumnImage(CDC* pDC, TDC_COLUMN nColID, const CRect& rect, BOOL bAlternate = FALSE);
	void DrawGridlines(CDC* pDC, const CRect& rect, BOOL bSelected, BOOL bHorz, BOOL bVert);
	void DrawTasksRowBackground(CDC* pDC, const CRect& rRow, const CRect& rLabel, GM_ITEMSTATE nState, COLORREF crBack = CLR_NONE);
	void DrawCommentsText(CDC* pDC, const CRect& rRow, const CRect& rLabel, const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS);
	BOOL DrawItemCustomColumn(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, TDC_COLUMN nColID, 
							  CDC* pDC, const CRect& rSubItem, COLORREF crText);

	enum TTCB_CHECK { TTCNC_UNCHECKED, TTCBC_CHECKED, TTCBC_MIXED };
	void DrawColumnCheckBox(CDC* pDC, const CRect& rSubItem, TTCB_CHECK nCheck);

	void DrawColumnDate(CDC* pDC, const COleDateTime& date, TDC_DATE nDate, const CRect& rect, COLORREF crText, 
						BOOL bCalculated = FALSE, BOOL bCustomWantsTime = FALSE, int nAlign = DT_RIGHT);

	BOOL IsColumnShowing(TDC_COLUMN nColID) const;
	BOOL HasStyle(TDC_STYLE nStyle) const { return m_data.HasStyle(nStyle); }
	const TDCCOLUMN* GetColumn(TDC_COLUMN nColID) const;

	// internal version
	BOOL GetTaskTextColors(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, COLORREF& crText,
							COLORREF& crBack, BOOL bRef, BOOL bSelected) const;
	COLORREF GetTaskCommentsTextColor(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS) const;

	static BOOL IsDateWithin7DaysOfToday(const COleDateTime& date, TDC_DATE nDate);
	static int CalcRequiredIconColumnWidth(int nNumImage);
	static inline BOOL HasColor(COLORREF color) { return (color != CLR_NONE) ? TRUE : FALSE; }
	static BOOL SetColor(COLORREF& color, COLORREF crNew);
	static BOOL CheckUpdateDueBrushColor(COLORREF crNew, COLORREF& crCur, CBrush& brCur);

};

#endif // !defined(AFX_TDCTASKCTRLRENDERER_H__155791A3_22AC_4083_B933_F39E9EBDADEF__INCLUDED_)
