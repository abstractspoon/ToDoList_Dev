#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////

using namespace System;

////////////////////////////////////////////////////////////////////////////////////////////////

class ITransText;

////////////////////////////////////////////////////////////////////////////////////////////////

namespace Abstractspoon
{
	namespace Tdl
	{
		namespace PluginHelpers
		{
			public ref class Translator
			{
			public:
				Translator(ITransText* pTransText);

				String^ Translate(String^ sText);

				void Translate(Windows::Forms::Form^ window);
				void Translate(Windows::Forms::Form^ window, Windows::Forms::ToolTip^ tooltips);
				void Translate(Windows::Forms::ComboBox^ combo);
				void Translate(Windows::Forms::Control::ControlCollection^ items);
				void Translate(Windows::Forms::ToolStripItemCollection^ items);

				void AddPreTranslation(String^ sText, String^ sTranslation);

			private:
				ITransText* m_pTransText;
				Collections::Generic::Dictionary<String^, String^>^ m_mapPreTranslate;

			private:
				Translator();

			};
		}
	}
}