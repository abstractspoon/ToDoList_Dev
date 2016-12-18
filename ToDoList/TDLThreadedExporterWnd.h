#if !defined(AFX_TDLTHREADEDEXPORTWND_H__4C046CB3_364C_4221_92DF_DAB8BCCFB3E4__INCLUDED_)
#define AFX_TDLTHREADEDEXPORTWND_H__4C046CB3_364C_4221_92DF_DAB8BCCFB3E4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TDLThreadedExportWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////

struct TDCEXPORTTASKLIST;

/////////////////////////////////////////////////////////////////////////////
// CTDLThreadedExportWnd window

class CTDLThreadedExporterWnd : public CWnd
{
// Construction
public:
	CTDLThreadedExporterWnd();

// Operations
public:
	BOOL IsFinished() const { return (m_nNumThreads == 0); }
	BOOL ExportTasks(TDCEXPORTTASKLIST* pExport);

// Attributes
protected:
	int m_nNumThreads;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTDLThreadedExportWnd)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTDLThreadedExporterWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CTDLThreadedExportWnd)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	afx_msg LRESULT OnExportThreadFinished(WPARAM wp, LPARAM lp);
	afx_msg void OnDestroy();
	DECLARE_MESSAGE_MAP()

private:
	static UINT ExportThreadProc(LPVOID pParam);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TDLTHREADEDEXPORTWND_H__4C046CB3_364C_4221_92DF_DAB8BCCFB3E4__INCLUDED_)
