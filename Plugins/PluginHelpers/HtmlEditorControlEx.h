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
			ref class UIThemeToolbarRenderer;
			ref class Translator;

			public ref class HtmlEditorControlEx : MSDN::Html::Editor::HtmlEditorControl
			{
			public:
				HtmlEditorControlEx(Drawing::Font^ controlFont, Translator^ trans, bool fixupToolbarButtonSizes);

				void SetUITheme(UITheme^ theme);
				void SetBodyFont(String^ fontName, int pointSize);

				void TextCut() override;
				void TextCopy() override;
				void TextPaste() override;

				property String^ LastBrowsedImageFolder;
				property String^ LastBrowsedFileFolder;
				property bool IncludeSourceUrlWhenPasting;

				static property Drawing::Size SizeEditHtmlForm
				{
					Drawing::Size get() { return s_SizeEditHtmlForm; }
					void set(Drawing::Size size) { s_SizeEditHtmlForm = size; }
				}

			protected:
				void SetControlFont(Drawing::Font^ font);
				void SetTranslator(Translator^ trans);

			protected:
				UIThemeToolbarRenderer^ m_ToolbarRenderer;
				Drawing::Font^ m_ControlsFont;
				Translator^ m_Trans;
				bool m_AutoFixupToolbarButtonsSize;

				static Drawing::Size s_SizeEditHtmlForm = Drawing::Size(-1, -1);
				static BOOL s_ClipboardEnabled = -1;

			protected:
				virtual void OnLoad(EventArgs^ args) override;

			protected:
				void InitializeComponentEx();
				void InitialiseFeatures(bool hideDocumentCommands);

				Windows::Forms::DialogResult ShowDialog(Windows::Forms::Form^ dialog, Drawing::Icon^ icon);
				void PreShowDialog(Windows::Forms::Form^ dialog, Drawing::Icon^ icon);
				Windows::Forms::DialogResult PostShowDialog(Windows::Forms::Form^ dialog, Windows::Forms::DialogResult res);

				void Translate();
				bool InitialiseClipboardSupport();
			};
		}
	}
}
