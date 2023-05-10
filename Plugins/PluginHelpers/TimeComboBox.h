#pragma once

#include <Shared\TimeComboBox.h>

////////////////////////////////////////////////////////////////////////////////////////////////

using namespace System;

////////////////////////////////////////////////////////////////////////////////////////////////

namespace Abstractspoon
{
	namespace Tdl
	{
		namespace PluginHelpers
		{
			class HostedTimeComboBox
			{
			public:
				static HostedTimeComboBox* Attach(HWND hwndParent, HFONT hFont);
				
				void Detach();

				double Get24HourTime();
				BOOL Set24HourTime(double dTime); // -1 for 'no time'

				void DrawItem(WPARAM wp, LPARAM lp);

			protected:
				HostedTimeComboBox(HWND hwndParent);

			private:
				CWnd m_WndOfManagedHandle;
				CTimeComboBox m_Combo;
			};

			// -------------------------------------------------------------------

			public ref class TimeComboBox : Windows::Forms::Control
			{
			public:
				TimeComboBox();

				TimeSpan GetTime();
				bool SetTime(TimeSpan time);
				bool SetTime(DateTime date);
				
				event System::EventHandler^ ChangeEvent;

			private:
				IntPtr m_pMFCInfo = IntPtr::Zero;

			protected:
				void WndProc(Windows::Forms::Message% m) override;
				void OnHandleCreated(EventArgs^ e) override;
				void OnHandleDestroyed(EventArgs^ e) override;

			};

		}
	}
}