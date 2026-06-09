#pragma once

#include <Shared\InputListCtrl.h>

#include <Interfaces\IEnums.h>

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
		int nType;
	};

	// ----------------------------------------

	struct FILTER
	{
		FILTER();

		BOOL IsValid() const;

		int nXVarIndex, nYVarIndex;
		CString sXCutoff, sYCutoff;
	};

	///////////////////////////////////////////////////////////////////
	
	class CEisenhowerSetupListCtrl : public CInputListCtrl
	{
	public:
		CEisenhowerSetupListCtrl();

		void Initialise(LPCWSTR szXVarColName, 
						LPCWSTR szXCutoffColName, 
						LPCWSTR szYVarColName, 
						LPCWSTR szYCutoffColName,
						LPCWSTR szNewRowPrompt,
						LPCWSTR szCutoffPrompt,
						const CArray<VARIABLE, VARIABLE&>& aVars,
						const CArray<FILTER, FILTER&>& aFilters);

		int GetFilters(CArray<FILTER, FILTER&>& filters) const;

	private:
		CComboBox m_cbVariables;
		CString m_sCutoffPrompt;

		CArray<VARIABLE, VARIABLE&> m_aVariables;
		CArray<FILTER, FILTER&> m_aFilters;

	protected:
		afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
		afx_msg void OnComboSelChange();
		DECLARE_MESSAGE_MAP()

	protected:
		virtual void PrepareControl(CWnd& ctrl, int nRow, int nCol);
		virtual void EditCell(int nItem, int nCol, BOOL bBtnClick);
		virtual void DrawCellText(CDC* pDC, int nItem, int nCol, const CRect& rText, const CString& sText, COLORREF crText, UINT nDrawTextFlags);
		virtual void OnEndEdit(UINT uIDCtrl, int* pResult);
		virtual BOOL CanEditCell(int nRow, int nCol) const;
		virtual BOOL DeleteSelectedCell();
		virtual COLORREF GetItemBackColor(int nItem, int nCol, BOOL bSelected, BOOL bDropHighlighted, BOOL bWndFocus) const;

	protected:
		void PrepareCombo(int nRow, int nCol);
		int GetVarType(int nVar) const;
		BOOL CanEditCutOff(int nVar) const;
	};

	///////////////////////////////////////////////////////////////////

	class HostedEisenhowerSetupListCtrl
	{
	public:
		static HostedEisenhowerSetupListCtrl* Attach(HWND hwndParent, HFONT hFont);

		void Detach();

		void DrawItem(WPARAM wp, LPARAM lp);
		void UpdateSize();

		void Initialise(LPCWSTR szXVarColName,
						LPCWSTR szXCutoffColName,
						LPCWSTR szYVarColName,
						LPCWSTR szYCutoffColName,
						LPCWSTR szNewRowPrompt,
						LPCWSTR szCutoffPrompt,
						const CArray<VARIABLE, VARIABLE&>& vars,
						const CArray<FILTER, FILTER&>& filters);

		int GetFilters(CArray<FILTER, FILTER&>& filters);

	protected:
		HostedEisenhowerSetupListCtrl(HWND hwndParent);

	private:
		CWnd m_WndOfManagedHandle;
		CEisenhowerSetupListCtrl m_ListCtrl;
	};

	///////////////////////////////////////////////////////////////////

	public ref class EisenhowerFilterSetupListCtrl : Windows::Forms::Control
	{
	public:
		EisenhowerFilterSetupListCtrl();

		void Initialise(Translator^ trans,
						EisenhowerVariables^ vars,
						EisenhowerFilters^ filters);

		property EisenhowerFilters^ Filters { EisenhowerFilters^ get(); }

	private:
		IntPtr m_pMFCInfo = IntPtr::Zero;
		Translator^ m_Trans;

		EisenhowerVariables^ m_Vars;
		EisenhowerFilters ^m_Filters, ^m_ModifiedFilters;

	protected:
		void WndProc(Windows::Forms::Message% m) override;
		void OnHandleCreated(EventArgs^ e) override;
		void OnHandleDestroyed(EventArgs^ e) override;
		void OnSizeChanged(EventArgs^ e) override;

	protected:
		void CheckInitListCtrl();
	};

}