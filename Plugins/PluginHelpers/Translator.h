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
			ref class Translator; // predec

			public interface class ITranslatable
			{
			public:
				virtual void Translate(Translator^ trans);
			};

			public ref class Translator
			{
			public:
				enum class CtrlType
				{
					Button,
					CheckBox,
					ComboBox,
					Dialog,
					GroupBox,
					Header,
					Label,
					ListBox,
					Menu,
					RadioButton,
					Tab,
					Text,
					ToolTip,
				};

			public:
				Translator(ITransText* pTransText);

				String^ Translate(String^ sText, CtrlType type);

				void Translate(Windows::Forms::Form^ window);
				void Translate(Windows::Forms::Form^ window, Windows::Forms::ToolTip^ tooltips);
				void Translate(Windows::Forms::Control^ ctrl);
				void Translate(Windows::Forms::ComboBox^ combo);
				void Translate(Windows::Forms::Control::ControlCollection^ items);
				void Translate(Windows::Forms::ToolStripItemCollection^ items, bool isMenu);
				void Translate(Windows::Forms::ListView::ColumnHeaderCollection^ items);

				void Translate(ITranslatable^ ctrl);

				void AddPreTranslation(String^ sText, String^ sTranslation);

			private:
				ITransText* m_pTransText;
				Collections::Generic::Dictionary<String^, String^>^ m_mapPreTranslate;

			private:
				Translator();

				String^ Translate(String^ sText, LPCWSTR sClassName);
				LPCWSTR GetClassName(CtrlType type);
			};
		}
	}
}