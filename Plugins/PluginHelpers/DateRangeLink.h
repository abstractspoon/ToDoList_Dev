#pragma once

#include <3rdParty\StatLink.h>

////////////////////////////////////////////////////////////////////////////////////////////////

using namespace System;

////////////////////////////////////////////////////////////////////////////////////////////////

namespace Abstractspoon
{
	namespace Tdl
	{
		namespace PluginHelpers
		{
			class HostedDateRangeLink
			{
			public:
				static HostedDateRangeLink* Attach(HWND hwndParent, HFONT hFont);
				
				void Detach();

				void SetText(LPCTSTR szText);
				HBRUSH CtlColor(HDC hDC);
				void SetBackColor(COLORREF crBack);

			protected:
				HostedDateRangeLink(HWND hwndParent);

			private:
				CWnd m_WndOfManagedHandle;
				CStaticLink m_LinkLabel;
				CBrush m_brBack;
			};

			// -------------------------------------------------------------------

			public ref class DateRangeLink : Windows::Forms::Control
			{
			public:
				DateRangeLink();

				bool SetRange(DateTime start, DateTime end);
				void ClearRange();
				void SetISOFormat(bool iso);

				event System::EventHandler^ ClickEvent;

			private:
				IntPtr m_pMFCInfo = IntPtr::Zero;
				DateTime m_Start, m_End;
				bool m_IsoFormat;

			protected:
				void WndProc(Windows::Forms::Message% m) override;
				void OnHandleCreated(EventArgs^ e) override;
				void OnHandleDestroyed(EventArgs^ e) override;
				void OnBackColorChanged(EventArgs^ e) override;

				bool CheckSetRange();
			};
		}
	}
}