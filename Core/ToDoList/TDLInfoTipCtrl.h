#if !defined(AFX_TDLINFOTIPCTRL_H__4BBE69FD_312A_4C7E_A8BE_A435C740EB7A__INCLUDED_)
#define AFX_TDLINFOTIPCTRL_H__4BBE69FD_312A_4C7E_A8BE_A435C740EB7A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TDLInfoTipCtrl.h : header file
//

#include "ToDoCtrlDataUtils.h"

#include "..\Shared\ToolTipCtrlEx.h"

#include "..\3rdParty\GdiPlus.h"

/////////////////////////////////////////////////////////////////////////////

class CTDCAttributeMap;
class CToDoCtrlData;
class CTDCCustomAttribDefinitionArray;

/////////////////////////////////////////////////////////////////////////////
// CTDLInfoTipCtrl window

class CTDLInfoTipCtrl : public CToolTipCtrlEx
{
// Construction
public:
	CTDLInfoTipCtrl(const CToDoCtrlData& data, 
					const CTDCCustomAttribDefinitionArray& aCustAttribs, 
					const CContentMgr& mgrContent);
	virtual ~CTDLInfoTipCtrl();

	CString FormatTip(DWORD dwTaskID, 
					  const CTDCAttributeMap& mapAttrib, 
					  int nMaxCommentsLen,
					  const CString& sCompletionStatus) const;

protected:
	const CToDoCtrlData& m_data;
	const CTDCCustomAttribDefinitionArray& m_aCustomAttribDefs;

	CTDCTaskFormatter m_formatter;
	CTDCTaskCalculator m_calculator;
	
	CGdiPlusBitmap m_bmpImageTip;

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg BOOL OnNotifyShow(NMHDR* pNMHDR, LRESULT* pResult);

	DECLARE_MESSAGE_MAP()

protected:
	virtual void OnPaintTip(CDC* pDC);

protected:
	int BuildSortedAttributeArray(DWORD dwTaskID, 
								  const CTDCAttributeMap& mapAttrib, 
								  int nMaxCommentsLen,
								  const CString& sCompletionStatus,
								  CTDCInfoTipItemArray& aItems) const;

	static int InfoTipSortProc(const void* pV1, const void* pV2);
};

/////////////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_TDLINFOTIPCTRL_H__4BBE69FD_312A_4C7E_A8BE_A435C740EB7A__INCLUDED_)
