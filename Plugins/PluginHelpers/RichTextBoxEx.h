#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////

#include "LabelTip.h"
#include "ContentControl.h"

////////////////////////////////////////////////////////////////////////////////////////////////

using namespace System;

////////////////////////////////////////////////////////////////////////////////////////////////

namespace Abstractspoon
{
	namespace Tdl
	{
		namespace PluginHelpers
		{
			public ref class RichTextBoxEx : Windows::Forms::RichTextBox, ILabelTipHandler
			{
			public:
				bool SelectionContainsPos(Drawing::Point ptClient);
				bool SelectionContainsMessagePos();

				void Indent();
				void Outdent();
				void SetPrompt(String^ szPrompt, Drawing::Font^ font);

				virtual LabelTipInfo^ ToolHitTest(Drawing::Point ptScreen);
				virtual Windows::Forms::Control^ GetOwner() { return this; }

				NeedLinkTooltipEventHandler^ NeedLinkTooltip;

			protected:
				String^ m_CurrentLink;
				String^ m_Prompt;
				Drawing::Font^ m_PromptFont = nullptr;
				LabelTip^ m_LinkTip = nullptr;
				int m_LinkTipVOffset = 0;

			protected:
				virtual property Windows::Forms::CreateParams^ CreateParams
				{
					Windows::Forms::CreateParams^ get() override;
				}

				virtual void WndProc(Windows::Forms::Message% m) override;
				virtual void OnLinkClicked(Windows::Forms::LinkClickedEventArgs^ e) override;

				String^ GetTextRange(const CHARRANGE& cr);
			};
		}
	}
}
