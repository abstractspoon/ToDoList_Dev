#pragma once

#include <Shared\InputListCtrl.h>

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

		void Initialise(const CArray<VARIABLE, VARIABLE&>& aVars,
						const CArray<FILTER, FILTER&>& aFilters);

		int GetFilters(CArray<FILTER, FILTER&>& filters) const;

	private:
		CComboBox m_cbVariables;

		CArray<VARIABLE, VARIABLE&> m_aVariables;
		CArray<FILTER, FILTER&> m_aFilters;

	protected:
		afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
		afx_msg void OnVariableComboCloseUp();
		afx_msg void OnHeaderCustomDraw(NMHDR* pNMHDR, LPARAM* lResult);
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
		CString GetVarLabel(int nVar) const;
		BOOL CanEditCutOff(int nVar) const;
		void NotifyEditChange();
		CString GetCellPrompt(int nItem, int nCol, const CString& sText) const;
		CString GetCellPrompt(int nItem, int nCol, const CString& sText, int nVar) const;
		void DrawHeaderRect(CDC* pDC, const CRect& rItem, const CString& sItem);
	};

	///////////////////////////////////////////////////////////////////

	class HostedEisenhowerSetupListCtrl
	{
	public:
		static HostedEisenhowerSetupListCtrl* Attach(HWND hwndParent, HFONT hFont);

		void Initialise(ITransText* pTrans,
						const CArray<VARIABLE, VARIABLE&>& vars,
						const CArray<FILTER, FILTER&>& filters);

		int GetFilters(CArray<FILTER, FILTER&>& filters);

		void Detach();
		void DrawItem(WPARAM wp, LPARAM lp);
		void UpdateSize();

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

		event System::EventHandler^ ChangeEvent;

	private:
		IntPtr m_pMFCInfo = IntPtr::Zero;
		Translator^ m_Trans;

		EisenhowerVariables^ m_Vars;
		EisenhowerFilters^ m_Filters;

	protected:
		void WndProc(Windows::Forms::Message% m) override;
		void OnHandleCreated(EventArgs^ e) override;
		void OnHandleDestroyed(EventArgs^ e) override;
		void OnSizeChanged(EventArgs^ e) override;

	protected:
		void CheckInitListCtrl();
		void CheckUpdateFilters();
	};

}