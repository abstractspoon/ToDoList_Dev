#pragma once

#include <Shared\CheckComboBox.h>
#include <Shared\WndPrompt.h>

////////////////////////////////////////////////////////////////////////////////////////////////

using namespace System;
using namespace System::Collections::Generic;

////////////////////////////////////////////////////////////////////////////////////////////////

namespace Abstractspoon
{
	namespace Tdl
	{
		namespace PluginHelpers
		{	
			class CCheckComboBox_ : public CCheckComboBox
			{
			public:
				// Because MFC message reflection appears not to work
				void OnEditchange() { CCheckComboBox::OnEditchange(); }
				void OnDropdown() { CCheckComboBox::OnDropdown(); }
				void OnCloseUp() { CCheckComboBox::OnCloseUp(); }
			};

			// -------------------------------------------------------------------

			class HostedCheckComboBox
			{
			public:
				static HostedCheckComboBox* Attach(HWND hwndParent, HFONT hFont, BOOL bSorted = TRUE);

				int AddItem(LPCWSTR szItem, int nUniqueId, bool bChecked);
				bool IsItemChecked(int nUniqueId);
				bool SetItemChecked(int nUniqueId, bool bChecked);

				void RemoveAllItems();
				int GetItemCount();

				void DrawItem(WPARAM wp, LPARAM lp);
				void Detach();
				bool IsDropped() { return (m_Combo.GetDroppedState() != FALSE); }
				void FilterTooltipMessage(MSG* pMsg);

				void SetEnabled(bool bEnabled);
				void SetPrompt(LPCWSTR szPrompt);

				// Because MFC message reflection appears not to work
				void OnEditchange();
				void OnDropdown();
				void OnCloseUp();

			protected:
				HostedCheckComboBox(HWND hwndParent);

			private:
				CWnd m_WndOfManagedHandle;
				CCheckComboBox_ m_Combo;
				CWndPrompt m_ComboPrompt;
			};

			// -------------------------------------------------------------------

			public interface class ICheckComboBoxItem
			{
			public:
				virtual property String^ Label { String^ get(); }
				virtual property int UniqueId { int get(); }
			};

			// -------------------------------------------------------------------
			// Minimal implementation

			ref class Translator;

			public ref class CheckComboBoxItem : public ICheckComboBoxItem
			{
			public:
				CheckComboBoxItem(String^ label, int UniqueId);
				CheckComboBoxItem(String^ label, int UniqueId, Translator^ trans);
;
			public:
				virtual property String^ Label { String^ get(); }
				virtual property int UniqueId { int get(); }

			private:
				String^ m_Label;
				int m_UniqueId;
			};

			// --------------------------------------------------------

			public ref class CheckComboBox : Windows::Forms::Control
			{
			public:
				CheckComboBox();

				int AddItem(ICheckComboBoxItem^ item, bool checked);
				bool IsItemChecked(ICheckComboBoxItem^ item);
				bool SetItemChecked(ICheckComboBoxItem^ item, bool checked);
				void RemoveAllItems();
				void FilterTooltipMessage(Windows::Forms::Message m);

				property IEnumerable<ICheckComboBoxItem^>^ Items 
				{ 
					IEnumerable<ICheckComboBoxItem^>^ get() { return m_Items; } 
				}
				
				property IEnumerable<ICheckComboBoxItem^>^ CheckedItems 
				{ 
					IEnumerable<ICheckComboBoxItem^>^ get() { return m_CheckedItems; } 
				}

				property bool Sorted	{ bool get(); void set(bool sorted); }
				property bool Enabled	{ bool get(); void set(bool enabled); }

				property String^ Prompt	{ void set(String^ prompt); }

				event EventHandler^ DropDownClosed; // == edit completion

			private:
				IntPtr m_pMFCInfo = IntPtr::Zero;

				bool m_Sorted = false;
				String^ m_Prompt = L"<none>";

				List<ICheckComboBoxItem^>^ m_Items;
				List<ICheckComboBoxItem^>^ m_CheckedItems;

			protected:
				void WndProc(Windows::Forms::Message% m) override;
				void OnHandleCreated(EventArgs^ e) override;
				void OnHandleDestroyed(EventArgs^ e) override;

				void CheckPopulateCombo();
				void SetEnabled(bool enabled);
			};

		}
	}
}