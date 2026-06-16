#pragma once

#include <Shared\CheckComboBox.h>

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
			class HostedCheckComboBox
			{
			public:
				static HostedCheckComboBox* Attach(HWND hwndParent, HFONT hFont);

				int AddItem(LPCWSTR szItem, int nItemData, bool checked);
				bool IsItemChecked(int nItemData);
				bool SetItemChecked(int nItemData, bool checked);

				void RemoveAllItems();
				int GetItemCount();

				void DrawItem(WPARAM wp, LPARAM lp);
				void SetEnabled(bool enabled);
				void Detach();

			protected:
				HostedCheckComboBox(HWND hwndParent);

			private:
				CWnd m_WndOfManagedHandle;
				CCheckComboBox m_Combo;
			};

			// -------------------------------------------------------------------

			public interface class ICheckComboBoxItem
			{
			public:
				virtual property String^ Label { String^ get(); }
				virtual property int ItemData { int get(); }
			};

			// -------------------------------------------------------------------

			public ref class CheckComboBox : Windows::Forms::Control
			{
			public:
				CheckComboBox();

				int AddItem(ICheckComboBoxItem^ item, bool checked);
				bool IsItemChecked(ICheckComboBoxItem^ item);
				bool SetItemChecked(ICheckComboBoxItem^ item, bool checked);
				void RemoveAllItems();

				property IEnumerable<ICheckComboBoxItem^>^ Items { IEnumerable<ICheckComboBoxItem^>^ get() { return m_Items; } }
				property IEnumerable<ICheckComboBoxItem^>^ CheckedItems { IEnumerable<ICheckComboBoxItem^>^ get() { return m_CheckedItems; } }

				property bool Enabled
				{
					bool get() { return Windows::Forms::Control::Enabled; }
					void set(bool enabled) { SetEnabled(enabled); }
				}

			private:
				IntPtr m_pMFCInfo = IntPtr::Zero;
				List<ICheckComboBoxItem^>^ m_Items;
				List<ICheckComboBoxItem^>^ m_CheckedItems;

			protected:
				void WndProc(Windows::Forms::Message% m) override;
				void OnHandleCreated(EventArgs^ e) override;
				void OnHandleDestroyed(EventArgs^ e) override;

				void CheckPopulateCombo();
				void UpdateCheckStates();
				void SetEnabled(bool enabled);
			};

		}
	}
}