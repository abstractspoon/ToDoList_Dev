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

				BOOL SetMinMax(double min, double max);		// < Set Interval [Min, Max] of RangeSlider
				BOOL SetRange(double left, double right);	// < Set Selected Range in [Min, Max] 
				BOOL SetStep(double step = -1);				// -1 == no step

				void SetParentBackColor(COLORREF color);
				BOOL SetMinMaxRangeWidths(double dMinWidth = 0, double dMaxWidth = -1);
				BOOL SetMinTickSpacing(int nPixels);

			protected:
				HostedRangeSliderCtrl();

			private:
				CWnd m_WndOfManagedHandle;
				CRangeSliderCtrl m_Slider;
			};

			// -------------------------------------------------------------------

			public ref class RangeSliderCtrl : Windows::Forms::Control
			{
			public:
				RangeSliderCtrl();
				
				bool SetMinMax(double min, double max);
				bool SetRange(double left, double right);
				bool SetStep(double step);

				void SetParentBackColor(System::Drawing::Color color);
				bool SetMinMaxRangeWidths(double dMinWidth, double dMaxWidth);
				bool SetMinTickSpacing(int nPixels);

				static int GetRequiredHeight();

			private:
				IntPtr m_pMFCInfo = IntPtr::Zero;

			protected:
				void OnHandleCreated(EventArgs^ e) override;
				void OnHandleDestroyed(EventArgs^ e) override;

			};

			// -------------------------------------------------------------------

			public ref class MonthRangeSliderCtrl : RangeSliderCtrl
			{
			public:
				MonthRangeSliderCtrl();

				bool SetMinMax(DateTime^ dtFrom, DateTime^ dtTo);

				bool HasSelectedRange();
				bool GetSelectedRange(DateTime^% dtFrom, DateTime^% dtTo);
				bool SetSelectedRange(DateTime^ dtFrom, DateTime^ dtTo);
				void ClearSelectedRange();

				String^ FormatRange(char cDelim);

			protected:

			protected:
				void OnHandleCreated(EventArgs^ e) override;

			protected:
				int DateToMonths(DateTime^ date);
				DateTime^ MonthsToDate(int nMonths);
			};
		}
	}
}