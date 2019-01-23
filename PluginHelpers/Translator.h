#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////

class ITransText;

////////////////////////////////////////////////////////////////////////////////////////////////

using namespace System;

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
				Translator(ITransText* pTaskList);

				String^ Translate(String^ sText);
				void Translate(System::Windows::Forms::Form^ window);
				void Translate(System::Windows::Forms::Control::ControlCollection^ items);
				void Translate(System::Windows::Forms::ToolStripItemCollection^ items);

			private:
				ITransText* m_pTransText;

			private:
				Translator();
			};
		}
	}
}