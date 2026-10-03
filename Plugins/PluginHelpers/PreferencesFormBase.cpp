
#include "stdafx.h"
#include "PreferencesFormBase.h"

////////////////////////////////////////////////////////////////////////////////////////////////

using namespace System::Diagnostics;
using namespace System::Drawing;
using namespace System::Windows::Forms;
using namespace System::IO;

using namespace Abstractspoon::Tdl::PluginHelpers;

////////////////////////////////////////////////////////////////////////////////////////////////

PreferencesFormBase::PreferencesFormBase() : m_OK(nullptr), m_Cancel(nullptr), m_Panel(nullptr)
{
	InitializeComponent();
}

void PreferencesFormBase::InitializeComponent()
{
	m_Cancel = gcnew Button();
	m_OK	 = gcnew Button();
	m_Error  = gcnew Label();

	m_Panel  = gcnew PanelEx();

	SuspendLayout();
	// 
	// m_Panel
	// 
	m_Panel->Anchor = ((AnchorStyles)((((AnchorStyles::Top | AnchorStyles::Bottom)
																 | AnchorStyles::Left)
																| AnchorStyles::Right)));
	m_Panel->AutoScroll = true;
	m_Panel->BackColor = System::Drawing::SystemColors::Window;
	m_Panel->Location = System::Drawing::Point(12, 13);
	m_Panel->Name = "m_Panel";
	m_Panel->Size = System::Drawing::Size(337, 207);
	m_Panel->TabIndex = 0;
	// 
	// m_OK
	// 
	m_OK->Anchor = ((AnchorStyles)((AnchorStyles::Bottom | AnchorStyles::Right)));
	m_OK->DialogResult = Windows::Forms::DialogResult::OK;
	m_OK->Location = System::Drawing::Point(195, 231);
	m_OK->Name = "m_OK";
	m_OK->Size = System::Drawing::Size(75, 23);
	m_OK->TabIndex = 50;
	m_OK->Text = "OK";
	m_OK->UseVisualStyleBackColor = true;
	// 
	// m_Cancel
	// 
	m_Cancel->Anchor = ((AnchorStyles)((AnchorStyles::Bottom | AnchorStyles::Right)));
	m_Cancel->DialogResult = Windows::Forms::DialogResult::Cancel;
	m_Cancel->Location = System::Drawing::Point(276, 231);
	m_Cancel->Name = "m_Cancel";
	m_Cancel->Size = System::Drawing::Size(75, 23);
	m_Cancel->TabIndex = 51;
	m_Cancel->Text = "Cancel";
	m_Cancel->UseVisualStyleBackColor = true;
	// 
	// m_Error
	// 
	m_Error->Anchor = ((AnchorStyles)(((AnchorStyles::Bottom | AnchorStyles::Left) 
    | AnchorStyles::Right)));
	m_Error->ForeColor = System::Drawing::Color::Red;
	m_Error->Location = System::Drawing::Point(9, 221);
	m_Error->Name = "m_Error";
	m_Error->Size = System::Drawing::Size(180, 38);
	m_Error->TabIndex = 52;
	m_Error->Text = "";
	m_Error->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
	m_Error->Visible = false;
	// 
	// PreferencesFormBase
	// 
	AcceptButton = m_OK;
	AutoScaleDimensions = System::Drawing::SizeF(6.f, 13.f);
	AutoScaleMode = Windows::Forms::AutoScaleMode::Font;
	CancelButton = m_Cancel;
	ClientSize = System::Drawing::Size(361, 264);
	Controls->Add(m_OK);
	Controls->Add(m_Cancel);
	Controls->Add(m_Error);
	Controls->Add(m_Panel);
	FormBorderStyle = Windows::Forms::FormBorderStyle::FixedDialog;
	MaximizeBox = false;
	MinimizeBox = false;
	Name = "PreferencesFormBase";
	ShowInTaskbar = false;
	StartPosition = FormStartPosition::CenterParent;
	Text = "PreferencesFormBase";

	ResumeLayout(false);
}

void PreferencesFormBase::WndProc(Message% m)
{
	// TODO

	Form::WndProc(m);
}


void PreferencesFormBase::OnPaint(PaintEventArgs^ e)
{
	Form::OnPaint(e);

	auto pen = gcnew Pen(Color::FromArgb(192, 192, 192));
	auto rect = m_Panel->Bounds;

	rect.X--;
	rect.Width++;
	rect.Y--;
	rect.Height++;
	
	e->Graphics->DrawRectangle(pen, rect);
}

void PreferencesFormBase::OnSizeChanged(EventArgs^ e)
{
	Form::OnSizeChanged(e);

	Invalidate(false);
}
