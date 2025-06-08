
#include "stdafx.h"
#include "UITheme.h"
#include "Win32.h"
#include "DPIScaling.h"
#include "Translator.h"
#include "FormsUtil.h"
#include "PluginHelpers.h"
#include "HtmlEditorControlEx.h"

#include <shared\Clipboard.h>
#include <shared\Misc.h>
#include <shared\Rtf2HtmlConverter.h>
#include <shared\DarkMode.h>

#include <3rdParty\ClipboardBackup.h>

////////////////////////////////////////////////////////////////////////////////////////////////

using namespace System::Drawing;
using namespace System::Windows::Forms;
using namespace System::Collections::Generic;

using namespace MSDN::Html::Editor;
using namespace Command::Handling;

using namespace Abstractspoon::Tdl::PluginHelpers;

////////////////////////////////////////////////////////////////////////////////////////////////

HtmlEditorControlEx::HtmlEditorControlEx(Drawing::Font^ font, Translator^ trans, bool fixupToolbarButtonSizes) 
	: 
	m_AutoFixupToolbarButtonsSize(fixupToolbarButtonSizes)
{
	m_ControlsFont = font;

	m_ToolbarRenderer = gcnew UIThemeToolbarRenderer();
	m_ToolbarRenderer->SetUITheme(gcnew UITheme());

	if (trans != nullptr)
		m_Trans = trans;
	else
		m_Trans = gcnew Translator(nullptr); // for pre-translation

	IncludeSourceUrlWhenPasting = true;
}

void HtmlEditorControlEx::SetControlFont(Drawing::Font^ font)
{
	m_ControlsFont = font;

	if (m_ControlsFont != nullptr)
	{
		FormsUtil::SetFont(this, font);

		ToolBar->Items["toolstripFontComboBox"]->Font = m_ControlsFont;
		Toolbars::FixupButtonSizes(ToolBar);
	}
}

void HtmlEditorControlEx::SetTranslator(Translator^ trans)
{
	m_Trans = trans;

	Translate();
}

void HtmlEditorControlEx::SetUITheme(UITheme^ theme)
{
	m_ToolbarRenderer->SetUITheme(theme);

	// This sets toolbar back colour too
	BackColor = theme->GetAppDrawingColor(UITheme::AppColor::AppBackLight);
}

void HtmlEditorControlEx::InitializeComponentEx()
{
	ToolBar->Renderer = m_ToolbarRenderer;
	ContextMenu->Renderer = m_ToolbarRenderer;

	if (m_ControlsFont != nullptr)
	{
		ToolBar->Font = m_ControlsFont;
		ContextMenu->Font = m_ControlsFont;

		ToolBar->Items["toolstripFontComboBox"]->Font = m_ControlsFont;
	}

	if (DPIScaling::WantScaling())
	{
		int imageSize = DPIScaling::Scale(16);

		ToolBar->ImageScalingSize = Drawing::Size(imageSize, imageSize);
		ContextMenu->ImageScalingSize = Drawing::Size(imageSize, imageSize);
	}

	Translate();
}

void HtmlEditorControlEx::OnLoad(System::EventArgs^ args)
{
	HtmlEditorControl::OnLoad(args);

	if (m_AutoFixupToolbarButtonsSize)
		Toolbars::FixupButtonSizes(ToolBar);
}

void HtmlEditorControlEx::Translate()
{
	if (m_Trans != nullptr)
	{
		// Prepare pre-translation of enum comboboxes 
		m_Trans->AddPreTranslation(gcnew String("Center"), gcnew String("Centre"));

		m_Trans->Translate(ToolBar->Items, false);
		m_Trans->Translate(ContextMenu->Items, true);
	}
}

void HtmlEditorControlEx::InitialiseFeatures(bool hideDocumentCommands)
{
	if (hideDocumentCommands)
	{
		CommandHandling::RemoveCommand("contextDocument", ContextMenu->Items);

		CommandHandling::RemoveCommand("toolstripDocumentNew", ToolBar->Items);
		CommandHandling::RemoveCommand("toolstripDocumentOpen", ToolBar->Items);
		CommandHandling::RemoveCommand("toolstripDocumentSave", ToolBar->Items);
		CommandHandling::RemoveCommand("toolstripDocumentPrint", ToolBar->Items);
		CommandHandling::RemoveCommand("toolstripDocumentHelp", ToolBar->Items);
		// 
		// CommandHandling::SetMenuShortcut("contextFormatStrikeout", Keys::Control | Keys::Subtract, ContextMenu::Items);
		// CommandHandling::SetMenuShortcut("contextFormatIncrease", Keys::Control | Keys::Decimal, ContextMenu::Items);
		// CommandHandling::SetMenuShortcut("contextFormatDecrease", Keys::Control | Keys::Oemcomma, ContextMenu::Items);
		// TODO
	}
}

void HtmlEditorControlEx::SetBodyFont(String^ fontName, int pointSize)
{
	// Convert size to ems because it gives us greater granularity
	float ems = Win32::PointsToEms(pointSize);

	BodyFont = HtmlFontProperty(fontName, ems);
}

DialogResult HtmlEditorControlEx::ShowDialog(Form^ dialog, Icon^ icon)
{
	PreShowDialog(dialog, icon);

	DialogResult res = HtmlEditorControl::ShowDialog(dialog);

	return PostShowDialog(dialog, res);
}

void HtmlEditorControlEx::PreShowDialog(Form^ dialog, Icon^ icon)
{
	FormsUtil::SetFont(dialog, m_ControlsFont);

	// Add icon for identification
	dialog->Icon = icon;
	dialog->ShowIcon = (icon != nullptr);

	// Per dialog customisations
	if (ISTYPE(dialog, EnterHrefForm))
	{
		auto urlDialog = ASTYPE(dialog, EnterHrefForm);

		urlDialog->EnforceHrefTarget(NavigateActionOption::Default);
		urlDialog->LastBrowsedFolder = LastBrowsedFileFolder;
	
		m_Trans->Translate(urlDialog, urlDialog->Tooltip);

		urlDialog->BrowseTitle = m_Trans->Translate(urlDialog->BrowseTitle, Translator::Type::Dialog);
		urlDialog->BrowseFilter = m_Trans->Translate(urlDialog->BrowseFilter, Translator::Type::FileFilter);
	}
	else if (ISTYPE(dialog, EnterImageForm))
	{
		auto imageDialog = ASTYPE(dialog, EnterImageForm);

		imageDialog->LastBrowsedFolder = LastBrowsedImageFolder;
		FormsUtil::SetEditCue(dialog, gcnew String("hrefText"), m_Trans->Translate(gcnew String("Optional"), Translator::Type::Text), false);

		m_Trans->Translate(imageDialog, imageDialog->Tooltip);

		imageDialog->BrowseTitle = m_Trans->Translate(imageDialog->BrowseTitle, Translator::Type::Dialog);
		imageDialog->BrowseFilter = m_Trans->Translate(imageDialog->BrowseFilter, Translator::Type::FileFilter);
	}
	else if (ISTYPE(dialog, EditHtmlForm))
	{
		auto htmlDialog = ASTYPE(dialog, EditHtmlForm);

		htmlDialog->FontName = gcnew String("Lucida Console");
		m_Trans->Translate(dialog);

		if ((s_SizeEditHtmlForm.Width > 0) && (s_SizeEditHtmlForm.Height > 0))
			htmlDialog->Size = s_SizeEditHtmlForm;
	}
	else // all others
	{
		m_Trans->Translate(dialog);
	}

	Win32::ActivateApp(Handle);
}

DialogResult HtmlEditorControlEx::PostShowDialog(Form^ dialog, DialogResult res)
{
	if (res == DialogResult::OK)
	{
		// Per dialog customisations
		if (ISTYPE(dialog, EnterHrefForm))
		{
			auto urlDialog = ASTYPE(dialog, EnterHrefForm);

			LastBrowsedFileFolder = urlDialog->LastBrowsedFolder;
		}
		else if (ISTYPE(dialog, EnterImageForm))
		{
			auto imageDialog = ASTYPE(dialog, EnterImageForm);

			LastBrowsedImageFolder = imageDialog->LastBrowsedFolder;
		}
		else if (ISTYPE(dialog, EditHtmlForm))
		{
			auto htmlDialog = ASTYPE(dialog, EditHtmlForm);

			// Save the unmaximised size
			if (htmlDialog->WindowState == FormWindowState::Maximized)
				s_SizeEditHtmlForm = htmlDialog->RestoreBounds.Size;
			else
				s_SizeEditHtmlForm = htmlDialog->Size;
		}
	}

	return res;
}

void HtmlEditorControlEx::TextCut()
{
	if (!InitialiseClipboardSupport())
		return;

	if (s_ClipboardEnabled)
	{
		HtmlEditorControl::TextCut();
	}
	else
	{
		TextCopy();
		HtmlEditorControl::SelectedText = L"";
	}
}

void HtmlEditorControlEx::TextCopy()
{
	if (!InitialiseClipboardSupport())
		return;

	if (s_ClipboardEnabled)
	{
		HtmlEditorControl::TextCopy();
	}
	else if (!String::IsNullOrEmpty(SelectedHtml))
	{
		CClipboard clipboard(Win32::GetHwnd(Handle));

		if (clipboard.IsOpen())
		{
			clipboard.SetText((LPCWSTR)MS(SelectedText));

			CString sHtml((LPCWSTR)MS(SelectedHtml));
			CClipboard::PackageHTMLFragment(sHtml);

			Misc::EncodeAsMultiByte(sHtml, CP_UTF8);
			clipboard.SetText(sHtml, CBF_HTML);
		}
	}
}

void HtmlEditorControlEx::TextPaste()
{
	if (DoPasteUrlOrFiles())
		return;

	if (!InitialiseClipboardSupport())
	{
		if (Clipboard::ContainsText(TextDataFormat::Html) || Clipboard::ContainsText())
		{
			auto innerHtml = InnerHtml;
			auto oldHtml = (innerHtml ? innerHtml : String::Empty);

			HtmlEditorControl::TextPaste();

			innerHtml = InnerHtml;
			auto newHtml = (innerHtml ? innerHtml : String::Empty);

			if ((newHtml->Length != oldHtml->Length) || !newHtml->Equals(oldHtml))
			{
				s_ClipboardEnabled = TRUE;
				return; // paste is already done
			}
			else
			{
				s_ClipboardEnabled = FALSE;
			}
		}
	}

	CString sSourceUrl;

	if (s_ClipboardEnabled)
	{
		HtmlEditorControl::TextPaste();
	}
	else // Fallback
	{
		BOOL bHandled = FALSE;

		if (Clipboard::ContainsText(TextDataFormat::Html))
		{
			CString sHtml = CClipboard().GetText(CBF_HTML);
			Misc::EncodeAsUnicode(sHtml, CP_UTF8);

			CClipboard::UnpackageHTMLFragment(sHtml, sSourceUrl);

			if (!sHtml.IsEmpty())
			{
				SelectedHtml = gcnew String(sHtml);
				bHandled = TRUE;
			}
		}

		if (!bHandled && Clipboard::ContainsText(TextDataFormat::Rtf))
		{
			CString sRtf, sHtml;
			
			if (CClipboard().GetText(sRtf, CBF_RTF) && 
				CRtfHtmlConverter(FALSE).ConvertRtfToHtml((LPCSTR)(LPCWSTR)sRtf, NULL, sHtml, L"C:\\Temp"))
			{
				SelectedHtml = gcnew String(sHtml);
				bHandled = TRUE;
			}
		}

		if (!bHandled && Clipboard::ContainsText())
		{
			SelectedText = Clipboard::GetText();
		}
	}
	
	if (IncludeSourceUrlWhenPasting && Clipboard::ContainsText(TextDataFormat::Html))
	{
		if (sSourceUrl.IsEmpty() && !CClipboard().GetHTMLSourceLink(sSourceUrl))
			return;

		SelectedHtml = String::Format(L"<a href=\"{0}\">{1}</a>", gcnew String(sSourceUrl), m_Trans->Translate(L"Source", Translator::Type::Text));
	}
}

bool HtmlEditorControlEx::InitialiseClipboardSupport()
{
	if ((s_ClipboardEnabled == -1) && !String::IsNullOrEmpty(SelectedHtml))
	{
		// Backup the clipboard
		CClipboardBackup clipboard(Win32::GetHwnd(Handle));
		clipboard.Backup();

		// Do a test copy
		Clipboard::Clear();

		HtmlEditorControl::TextCopy();

		s_ClipboardEnabled = (Clipboard::ContainsText() ||
							  Clipboard::ContainsText(TextDataFormat::Html)) ? TRUE : FALSE;

		// Restore clipboard
		clipboard.Restore();
	}

	return (s_ClipboardEnabled != -1);
}

bool HtmlEditorControlEx::ExecuteCommandRange(MSHTML::IHTMLTxtRange^ range, String^ command, Object^ data)
{
	if (command->Equals(HtmlEditorControl::HTML_COMMAND_FORE_COLOR))
	{
		Color color = Color::FromName(ASTYPE(data, String));

		if (OnSetForeColor(range, color))
			return true;
	}

	return HtmlEditorControl::ExecuteCommandRange(range, command, data);
}

bool HtmlEditorControlEx::OnSetForeColor(MSHTML::IHTMLTxtRange^ range, Drawing::Color color)
{
	// Intercept setting white text in Dark Mode, and black text in non Dark Mode, 
	// and instead remove any colour definition by removing all attributes and 
	// then adding back everything that was there before except for the colour
	bool darkMode = (CDarkMode::IsEnabled() != FALSE);
	bool isWhite = (color == Color::White), isBlack = (color == Color::Black);

	if ((darkMode && isWhite) || (!darkMode && isBlack))
	{
		cli::array<String^>^ AttribCmds =
		{
			HTML_COMMAND_BOLD,
			HTML_COMMAND_UNDERLINE,
			HTML_COMMAND_ITALIC,
			HTML_COMMAND_SUBSCRIPT,
			HTML_COMMAND_SUPERSCRIPT,
			HTML_COMMAND_STRIKE_THROUGH,
			HTML_COMMAND_FONT_NAME,
			HTML_COMMAND_FONT_SIZE,
		};

		cli::array<Object^>^ AttribVals = gcnew cli::array<Object ^>(AttribCmds->Length);

		for (int attrib = 0; attrib < AttribCmds->Length; attrib++)
		{
			AttribVals[attrib] = QueryCommandRange(range, AttribCmds[attrib]);
		}

		if (HtmlEditorControl::ExecuteCommandRange(range, HtmlEditorControl::HTML_COMMAND_REMOVE_FORMAT, nullptr))
		{
			// restore all the other removed attributes
			for (int attrib = 0; attrib < AttribCmds->Length; attrib++)
			{
				// Only re-apply boolean attributes if they are true
				if (ISTYPE(AttribVals[attrib], bool))
				{
					bool^ val = ASTYPE(AttribVals[attrib], bool);

					if (!(*val))
						continue;
				}

				ExecuteCommandRange(range, AttribCmds[attrib], AttribVals[attrib]);
			}

			return true;
		}
	}

	return false; // not handled
}
