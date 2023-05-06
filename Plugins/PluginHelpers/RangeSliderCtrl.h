#pragma once

#include <Shared\RangeSliderCtrl.h>

////////////////////////////////////////////////////////////////////////////////////////////////

using namespace System;

////////////////////////////////////////////////////////////////////////////////////////////////

namespace Abstractspoon
{
	namespace Tdl
	{
		namespace PluginHelpers
		{
			class HostedRangeSliderCtrl
			{
			public:
				static HostedRangeSliderCtrl* Attach(IntPtr handleManaged);
				
				void Detach();
				void SetParentBackColor(COLORREF color);

			private:
				CWnd m_WndOfManagedHandle;
				CRangeSliderCtrl m_Slider;
			};

			// -------------------------------------------------------------------

			public ref class RangeSliderCtrl : System::Windows::Forms::Control
			{
			public:
				RangeSliderCtrl();

				void SetBackColor(System::Drawing::Color color);

			private:
				IntPtr m_pMFCInfo = IntPtr::Zero;

			protected:
				void OnHandleCreated(EventArgs^ e) override;
				void OnHandleDestroyed(EventArgs^ e) override;

			};
		}
	}
}