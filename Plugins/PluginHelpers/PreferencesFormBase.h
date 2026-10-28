#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////

#include "FormsUtil.h"

////////////////////////////////////////////////////////////////////////////////////////////////

using namespace System;

////////////////////////////////////////////////////////////////////////////////////////////////

namespace Abstractspoon
{
	namespace Tdl
	{
		namespace PluginHelpers
		{
			public ref class PreferencesFormBase : Windows::Forms::Form
			{
			public:
				PreferencesFormBase();

			protected:
				System::Windows::Forms::Button^ m_Cancel;
				System::Windows::Forms::Button^ m_OK;
				System::Windows::Forms::Label^  m_Error;

				PanelEx^  m_Panel;

			protected:
				virtual void OnPaint(Windows::Forms::PaintEventArgs^ e) override;
				virtual void OnSizeChanged(EventArgs^ e) override;

			protected:
				void InitializeComponent();
				void EnableOK(bool enable);

			};
		}
	}
}
