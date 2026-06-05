#pragma once

#include <Shared\InputListCtrl.h>

////////////////////////////////////////////////////////////////////////////////////////////////

using namespace System;

////////////////////////////////////////////////////////////////////////////////////////////////

namespace EisenhowerUIExtension
{
		namespace EisenhowerUIExtensionControls
		{
			class CEisenhowerSetupListCtrl : public CInputListCtrl
			{
			public:

			protected:
				afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

				DECLARE_MESSAGE_MAP()

			private:
			};

			// -------------------------------------------------------------------

			class HostedEisenhowerSetupListCtrl
			{
			public:
				static HostedEisenhowerSetupListCtrl* Attach(HWND hwndParent, HFONT hFont);
				
				void Detach();

				void DrawItem(WPARAM wp, LPARAM lp);
				void UpdateSize();

			protected:
				HostedEisenhowerSetupListCtrl(HWND hwndParent);

			private:
				CWnd m_WndOfManagedHandle;
				CEisenhowerSetupListCtrl m_ListCtrl;
			};

			// -------------------------------------------------------------------

// 			public ref class EisenhowerFilterSetup
// 			{
// 			public:
// 				String XVar, YVar;
// 				double XCutoff, YCutoff;
// 			};

			// -------------------------------------------------------------------

			public ref class EisenhowerFilterSetupListCtrl : Windows::Forms::Control
			{
			public:
				EisenhowerFilterSetupListCtrl();

//  				Collections::Generic::List<EisenhowerFilterSetup^>^ GetFilters();
// 				void SetFilters(Collections::Generic::List<EisenhowerFilterSetup^>^ filters);

			private:
				IntPtr m_pMFCInfo = IntPtr::Zero;

			protected:
				void WndProc(Windows::Forms::Message% m) override;
				void OnHandleCreated(EventArgs^ e) override;
				void OnHandleDestroyed(EventArgs^ e) override;
				void OnSizeChanged(EventArgs^ e) override;
			};

		}
}