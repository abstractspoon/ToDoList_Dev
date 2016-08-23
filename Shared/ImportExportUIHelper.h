// ImportExportUIHelper.h: interface for the CImportExportUIHelper class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IMPORTEXPORTUIHELPER_H__AC709E3B_C876_43D8_A965_CAD2D2E7AD34__INCLUDED_)
#define AFX_IMPORTEXPORTUIHELPER_H__AC709E3B_C876_43D8_A965_CAD2D2E7AD34__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CImportExportMgr;
class CMenuIconMgr;

class CImportExportUIHelper  
{
public:
	CImportExportUIHelper(const CImportExportMgr& mgrImpExp, CMenuIconMgr& mgrIcon);
	virtual ~CImportExportUIHelper();

	void UpdateExportMenu(CCmdUI* pCmdUI, int nMaxCount, BOOL bEnabled = TRUE) const;
	void UpdateImportMenu(CCmdUI* pCmdUI, int nMaxCount, BOOL bEnabled = TRUE) const;

protected:
	const CImportExportMgr& m_mgrImpExp;
	CMenuIconMgr& m_mgrIcon;

};

#endif // !defined(AFX_IMPORTEXPORTUIHELPER_H__AC709E3B_C876_43D8_A965_CAD2D2E7AD34__INCLUDED_)
