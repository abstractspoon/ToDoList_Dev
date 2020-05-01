
#include "stdafx.h"
#include "UITheme.h"
#include "Win32.h"
#include "DPIScaling.h"
#include "Translator.h"
#include "FormsUtil.h"
#include "PluginHelpers.h"
#include "HtmlEditorControlEx.h"

////////////////////////////////////////////////////////////////////////////////////////////////

using namespace System::Drawing;
using namespace System::Windows::Forms;

using namespace MSDN::Html::Editor;
using namespace Abstractspoon::Tdl::PluginHelpers;
using namespace Command::Handling;

////////////////////////////////////////////////////////////////////////////////////////////////

HtmlEditorControlEx::HtmlEditorControlEx(Drawing::Font^ font, Translator^ trans)
{
	m_ControlsFont = font;
	m_toolbarRenderer = gcnew UIThemeToolbarRenderer();

	if (trans != nullptr)
		m_Trans = trans;
	else
		m_Trans = gcnew Translator(nullptr); // for pre-translation
}

void HtmlEditorControlEx::SetControlFont(Drawing::Font^ font)
{
	m_ControlsFont = font;

	if (m_ControlsFont != nullptr)
	{
		FormsUtil::SetFont(this, font);
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
	m_toolbarRenderer->SetUITheme(theme);

	// This sets toolbar back colour too
	BackColor = theme->GetAppDrawingColor(UITheme::AppColor::AppBackLight);
}

void HtmlEditorControlEx::InitializeComponentEx()
{
	ToolBar->Renderer = m_toolbarRenderer;

	if (m_ControlsFont != nullptr)
	{
		ToolBar->Font = m_ControlsFont;
		ContextMenu->Font = m_ControlsFont;
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

	Toolbars::FixupButtonSizes(ToolBar);
}

void HtmlEditorControlEx::Translate()
{
	if (m_Trans != nullptr)
	{
		// Prepare pre-translation of enum comboboxes 
		m_Trans->AddPreTranslation(gcnew String("Center"), gcnew String("Centre"));

		m_Trans->Translate(ToolBar->Items);
		m_Trans->Translate(ContextMenu->Items);
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

void HtmlEditorControlEx::PreShowDialog(Form^ dialog, Icon^ icon)
{
	HtmlEditorControl::PreShowDialog(dialog);

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

		urlDialog->BrowseTitle = m_Trans->Translate(urlDialog->BrowseTitle);
		urlDialog->BrowseFilter = m_Trans->Translate(urlDialog->BrowseFilter);
	}
	else if (ISTYPE(dialog, EnterImageForm))
	{
		auto imageDialog = ASTYPE(dialog, EnterImageForm);

		imageDialog->LastBrowsedFolder = LastBrowsedImageFolder;
		FormsUtil::SetEditCue(dialog, gcnew String("hrefText"), m_Trans->Translate(gcnew String("Optional")), false);

		m_Trans->Translate(imageDialog, imageDialog->Tooltip);

		imageDialog->BrowseTitle = m_Trans->Translate(imageDialog->BrowseTitle);
		imageDialog->BrowseFilter = m_Trans->Translate(imageDialog->BrowseFilter);
	}
	else if (ISTYPE(dialog, EditHtmlForm))
	{
		auto htmlDialog = ASTYPE(dialog, EditHtmlForm);

		htmlDialog->FontName = gcnew String("Lucida Console");
		m_Trans->Translate(dialog);

		if ((m_SizeEditHtmlForm.Width > 0) && (m_SizeEditHtmlForm.Height > 0))
			htmlDialog->Size = m_SizeEditHtmlForm;
	}
	else // all others
	{
		m_Trans->Translate(dialog);
	}

	Win32::ActivateApp(Handle);
}

void HtmlEditorControlEx::PostShowDialog(Form^ dialog)
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
			m_SizeEditHtmlForm = htmlDialog->RestoreBounds.Size;
		else
			m_SizeEditHtmlForm = htmlDialog->Size;
	}
}

