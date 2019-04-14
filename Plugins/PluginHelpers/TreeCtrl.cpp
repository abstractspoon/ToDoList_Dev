// PluginHelpers.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "TreeCtrl.h"



////////////////////////////////////////////////////////////////////////////////////////////////

using namespace Abstractspoon::Tdl::PluginHelpers;

////////////////////////////////////////////////////////////////////////////////////////////////

bool TreeCtrl::SetItemIntegral(System::Windows::Forms::TreeNode^ node, int integral)
{
	TVITEMEX tvi = { 0 };
	tvi.mask = TVIF_HANDLE | TVIF_INTEGRAL;
	tvi.hItem = GetTreeItem(node);
	tvi.iIntegral = integral;

	return (::SendMessage(GetTreeHwnd(node), TVM_SETITEM, 0, (LPARAM)&tvi) != 0);
}

HTREEITEM TreeCtrl::GetTreeItem(System::Windows::Forms::TreeNode^ node)
{
	return static_cast<HTREEITEM>(node->Handle.ToPointer());
}

HWND TreeCtrl::GetTreeHwnd(System::Windows::Forms::TreeNode^ node)
{
	return static_cast<HWND>(node->TreeView->Handle.ToPointer());
}

////////////////////////////////////////////////////////////////////////////////////////////////
