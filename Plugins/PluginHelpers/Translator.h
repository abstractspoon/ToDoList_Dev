#pragma once

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
				void Translate(System::Windows::Forms::Form^ window);
				void Translate(System::Windows::Forms::ComboBox^ combo);
				void Translate(System::Windows::Forms::Control::ControlCollection^ items);
				void Translate(System::Windows::Forms::ToolStripItemCollection^ items);

				void AddPreTranslation(String^ sText, String^ sTranslation);

			private:
				ITransText* m_pTransText;
				System::Collections::Generic::Dictionary<String^, String^>^ m_mapPreTranslate;

			private:
				Translator();

				void RecalcDropWidth(System::Windows::Forms::ComboBox^ combo);
			};
		}
	}
}