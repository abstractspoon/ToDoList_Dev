#pragma once

namespace Abstractspoon
{
	namespace Tdl
	{
		namespace PluginHelpers
		{
			ref class UIThemeToolbarRenderer;
			ref class Translator;

			public ref class HtmlEditorControlEx : MSDN::Html::Editor::HtmlEditorControl
			{
			public:
				HtmlEditorControlEx(System::Drawing::Font^ controlFont, Translator^ trans);

				void SetUITheme(UITheme^ theme);
				void SetBodyFont(String^ fontName, int pointSize);

				property String^ LastBrowsedImageFolder;
				property String^ LastBrowsedFileFolder;

			protected:
				void SetControlFont(System::Drawing::Font^ font);
				void SetTranslator(Translator^ trans);

			protected:
				UIThemeToolbarRenderer^ m_toolbarRenderer;
				System::Drawing::Font^ m_ControlsFont;
				Translator^ m_Trans;

			protected:
				virtual void OnLoad(System::EventArgs^ args) override;

			protected:
				void InitializeComponentEx();
				void InitialiseFeatures(bool hideDocumentCommands);

				void PreShowDialog(System::Windows::Forms::Form^ dialog, System::Drawing::Icon^ icon);
				virtual void PostShowDialog(System::Windows::Forms::Form^ dialog) override;

				void Translate();
			};
		}
	}
}
