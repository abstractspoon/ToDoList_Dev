#pragma once

//////////////////////////////////////////////////////////////////////////

#include "autocombobox.h"
#include "fileedit.h"
#include "iconcache.h"

// CFileComboBox /////////////////////////////////////////////////////////

class CFileComboBox : public CAutoComboBox
{
	DECLARE_DYNAMIC(CFileComboBox)
		
public:
	CFileComboBox(int nEditStyle = FES_COMBOSTYLEBTN);
	virtual ~CFileComboBox();

	void SetCurrentFolder(LPCTSTR szFolder) { m_fileEdit.SetCurrentFolder(szFolder); }
	CString GetCurrentFolder() const { return m_fileEdit.GetCurrentFolder(); }
	void SetReadOnly(BOOL bReadOnly = TRUE);

	int GetFileList(CStringArray& aFiles);
	int SetFileList(const CStringArray& aFiles);
	int AddFile(const CString& sFile) { return AddUniqueItem(sFile); }
	int AddFiles(const CStringArray& aFiles);

	void DDX(CDataExchange* pDX, CStringArray& value);

protected:
	class CMultiFileEdit : public CFileEdit
	{
	public:
		CMultiFileEdit(int nEditStyle);

	protected:
		void HandleBrowseForFile(CEnFileDialog& dlg);
	};
	
protected:
	CMultiFileEdit m_fileEdit;
	CIconCache m_imageIcons;
	BOOL m_bReadOnly;

protected:
	BOOL PreCreateWindow(CREATESTRUCT& cs);
	int OnToolHitTest(CPoint point, TOOLINFO* pTI) const;

protected:
	afx_msg LRESULT OnFileEditBrowseChange(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnFileEditGetFileIcon(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnFileEditGetFileTooltip(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnFileEditDisplayFile(WPARAM wp, LPARAM lp);
	afx_msg BOOL OnSelChange();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()

	// pseudo message handler
	LRESULT OnEditboxMessage(UINT msg, WPARAM wp, LPARAM lp);
	LRESULT OnListboxMessage(UINT msg, WPARAM wp, LPARAM lp);

protected:
	BOOL InitFileEdit();
	void DrawItemText(CDC& dc, const CRect& rect, int nItem, UINT nItemState,
						DWORD dwItemData, const CString& sItem, BOOL bList, COLORREF crText);	
	BOOL HasIcon() const { return TRUE; }
	int GetMaxDropWidth() const { return 400; }
	UINT GetDrawEllipsis() const { return DT_PATH_ELLIPSIS; }
	BOOL DeleteLBItem(int nItem);
	void HandleReturnKey();

};


