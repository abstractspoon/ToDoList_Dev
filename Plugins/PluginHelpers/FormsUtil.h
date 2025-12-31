#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////

using namespace System;

////////////////////////////////////////////////////////////////////////////////////////////////

namespace Abstractspoon
{
	namespace Tdl
	{
		namespace PluginHelpers
		{
			public ref class FormsUtil
			{
			public:
				static void SetFont(Windows::Forms::Form^ form, Drawing::Font^ font);
				static void SetFont(Windows::Forms::Control^ ctrl, Drawing::Font^ font);
				static void SetFont(Windows::Forms::Control::ControlCollection^ ctrls, Drawing::Font^ font);

				static bool SetEditCue(Windows::Forms::Control^ ctrl, String^ sCueText);
				static bool SetEditCue(Windows::Forms::Control^ parent, String^ childName, String^ sCueText, bool recursiveSearch);

				static bool SetComboBoxCue(Windows::Forms::ComboBox^ combo, String^ sCueText);
				static bool SetComboBoxCue(Windows::Forms::Control^ parent, String^ childName, String^ sCueText, bool recursiveSearch);

				static Windows::Forms::Control^ Find(Windows::Forms::Control^ parent, String^ childName, bool recursive);

				static void RecalcDropWidth(Windows::Forms::ComboBox^ combo);
				static int CalcWidestItem(Windows::Forms::ComboBox^ combo);

				static void FixThumbScrolling(Windows::Forms::Message% msg);
			};

			// ---------------------------------------------------------

			public ref class PanelEx : public Windows::Forms::Panel
			{
			protected:
				// Calls FormsUtil::FixThumbScrolling automatically
				void WndProc(Windows::Forms::Message% m) override;
			};
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////

