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

				BOOL SetMinMax(double min, double max);
				BOOL SetStep(double step = -1);

				BOOL HasSelectedRange();
				BOOL GetSelectedRange(double& dFrom, double& dTo);
				BOOL SetSelectedRange(double dFrom, double dTo);
				void ClearSelectedRange();

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
				bool SetStep(double step);
				
				bool HasSelectedRange();
				bool GetSelectedRange(double% dFrom, double% dTo);
				bool SetSelectedRange(double dFrom, double dTo);
				void ClearSelectedRange();

				void SetParentBackColor(System::Drawing::Color color);
				bool SetMinMaxRangeWidths(double dMinWidth, double dMaxWidth);
				bool SetMinTickSpacing(int nPixels);

				static int GetRequiredHeight();

				event System::EventHandler^ ChangeEvent;

			private:
				IntPtr m_pMFCInfo = IntPtr::Zero;

			protected:
				void WndProc(Windows::Forms::Message% m) override;
				void OnHandleCreated(EventArgs^ e) override;
				void OnHandleDestroyed(EventArgs^ e) override;

			};

			// -------------------------------------------------------------------

			public ref class MonthRangeSliderCtrl : RangeSliderCtrl
			{
			public:
				MonthRangeSliderCtrl();

				bool SetMinMax(DateTime^ dtFrom, DateTime^ dtTo);

				bool GetSelectedRange(DateTime% dtFrom, DateTime% dtTo);
				bool SetSelectedRange(DateTime dtFrom, DateTime dtTo);

				String^ FormatRange(char cDelim);

			protected:

			protected:
				void OnHandleCreated(EventArgs^ e) override;

			protected:
				int DateToMonths(DateTime^ date);
				DateTime MonthsToDate(int nMonths);
			};
		}
	}
}