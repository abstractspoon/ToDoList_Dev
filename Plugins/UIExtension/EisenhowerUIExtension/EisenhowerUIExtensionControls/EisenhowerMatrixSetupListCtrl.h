#pragma once

#include <Shared\InputListCtrl.h>
#include <Shared\DateTimeCtrlEx.h>
#include <Shared\TimeEdit.h>

#include <Interfaces\IEnums.h>
#include <Interfaces\ITransText.h>

#include <afxtempl.h>

////////////////////////////////////////////////////////////////////////////////////////////////

using namespace System;
using namespace Abstractspoon::Tdl::PluginHelpers;

////////////////////////////////////////////////////////////////////////////////////////////////

namespace EisenhowerUIExtension
{
	///////////////////////////////////////////////////////////////////

	struct VARIABLE
	{
		CString sAttribID;
		CString sLabel;
		CString sListData;
		int nType;
	};

	///////////////////////////////////////////////////////////////////

	struct MATRIX
	{
		MATRIX();

		BOOL IsValid() const;

		int nXVarIndex, nYVarIndex;
		CString sXCutoff, sYCutoff;
	};

	///////////////////////////////////////////////////////////////////
	
	class CEisenhowerSetupListCtrl : public CInputListCtrl
	{
	public:
		CEisenhowerSetupListCtrl();

		void Initialise(const CArray<VARIABLE, VARIABLE&>& aVars,
						const CArray<MATRIX, MATRIX&>& aMatrices);

		int GetMatrices(CArray<MATRIX, MATRIX&>& aMatrices) const;

		BOOL PreTranslateMessage(MSG* pMsg);

	private:
		CComboBox m_cbVariables;
		CComboBox m_cbCutoffs;
		CDateTimeCtrlEx m_dtcCutoffs;
		CTimeEdit m_tpCutoffs;

		CArray<VARIABLE, VARIABLE&> m_aVariables;
		CArray<MATRIX, MATRIX&> m_aMatrices;

	protected:
		afx_msg void OnVariableComboCloseUp();
		afx_msg void OnCutoffComboCloseUp();
		afx_msg void OnHeaderCustomDraw(NMHDR* pNMHDR, LPARAM* lResult);
		afx_msg void OnDateCutoffChange(NMHDR* pNMHDR, LPARAM* lResult);
		afx_msg void OnTimeCutoffKillFocus();

		DECLARE_MESSAGE_MAP()

	protected:
		virtual void InitState();
		virtual void PrepareControl(CWnd& ctrl, int nRow, int nCol);
		virtual void EditCell(int nItem, int nCol, BOOL bBtnClick);
		virtual void OnEndEdit(UINT uIDCtrl, int* pResult);
		virtual BOOL CanEditCell(int nRow, int nCol) const;
		virtual BOOL DeleteSelectedCell();
		virtual void HideAllControls(const CWnd* pWndIgnore = NULL);
		virtual void OnCancelEdit();

		virtual COLORREF GetItemTextColor(int nItem, int nCol, BOOL bSelected, BOOL bDropHighlighted, BOOL bWndFocus) const;
		virtual COLORREF GetItemBackColor(int nItem, int nCol, BOOL bSelected, BOOL bDropHighlighted, BOOL bWndFocus) const;
		virtual IL_COLUMNTYPE GetCellType(int nRow, int nCol) const;

	protected:
		void PrepareVariableCombo(int nRow, int nCol);
		int GetVarType(int nVar) const;
		CString GetVarLabel(int nVar) const;
		int GetVarIndex(int nRow, int nCol) const;
		BOOL CanEditCutOff(int nVar) const;
		void NotifyEditChange();
		CString GetCellPrompt(int nItem, int nCol, const CString& sText) const;
		CString GetCellPrompt(int nItem, int nCol, const CString& sText, int nVar) const;
		void DrawHeaderRect(CDC* pDC, const CRect& rItem, const CString& sItem, BOOL bBold);
		void PrepareControl(int nRow, int nCol);
		void PrepareCutoffControl(int nVar, const CString& sCutoff);
		void EditCutoffCell(int nItem, int nCol, int nVar, BOOL bBtnClick);
		IL_COLUMNTYPE GetCutoffCellType(int nRow, int nCol, int nVar) const;
		BOOL UpdateCellText(int nRow, int nCol);
		CString FormatCellText(int nRow, int nCol, int nVar, const CString& sCutoff) const;
		int AddRow();
		BOOL SetSelectedMatrixCutoff(const CString& sCutoff);
	};

	///////////////////////////////////////////////////////////////////

	class HostedEisenhowerSetupListCtrl
	{
	public:
		static HostedEisenhowerSetupListCtrl* Attach(HWND hwndParent, HFONT hFont);

		void Initialise(ITransText* pTrans,
						const CArray<VARIABLE, VARIABLE&>& aVars,
						const CArray<MATRIX, MATRIX&>& aMatrices);

		int GetMatrices(CArray<MATRIX, MATRIX&>& aMatrices);

		void Detach();
		void SetFocus();
		void UpdateSize();
		void DrawItem(WPARAM wp, LPARAM lp);
		bool PreTranslateMessage(MSG* pMsg);

	protected:
		HostedEisenhowerSetupListCtrl(HWND hwndParent);

	private:
		CWnd m_WndOfManagedHandle;
		CEisenhowerSetupListCtrl m_ListCtrl;
	};

	///////////////////////////////////////////////////////////////////

	public ref class EisenhowerMatrixSetupListCtrl : Windows::Forms::Control
	{
	public:
		EisenhowerMatrixSetupListCtrl();

		void Initialise(Translator^ trans,
						EisenhowerVariables^ vars,
						EisenhowerMatrices^ matrices);

		property EisenhowerMatrices^ Matrices { EisenhowerMatrices^ get(); }

		event System::EventHandler^ ChangeEvent;

		bool PreProcessMessage(Windows::Forms::Message% msg) override;

	private:
		IntPtr m_pMFCInfo = IntPtr::Zero;
		Translator^ m_Trans;

		EisenhowerVariables^ m_Vars;
		EisenhowerMatrices^ m_Matrices;

	protected:
		void WndProc(Windows::Forms::Message% m) override;

		void OnHandleCreated(EventArgs^ e) override;
		void OnHandleDestroyed(EventArgs^ e) override;
		void OnSizeChanged(EventArgs^ e) override;

	protected:
		void CheckInitListCtrl();
		void CheckUpdateMatrices();
	};

}