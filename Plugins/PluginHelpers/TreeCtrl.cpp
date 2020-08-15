// PluginHelpers.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "TreeCtrl.h"

////////////////////////////////////////////////////////////////////////////////////////////////

using namespace System::Windows::Forms;

using namespace Abstractspoon::Tdl::PluginHelpers;

////////////////////////////////////////////////////////////////////////////////////////////////

bool TreeCtrl::SetItemIntegral(TreeNode^ node, int integral)
{
	TVITEMEX tvi = { 0 };
	tvi.mask = TVIF_HANDLE | TVIF_INTEGRAL;
	tvi.hItem = GetTreeItem(node);
	tvi.iIntegral = integral;

	return (::SendMessage(GetTreeHwnd(node), TVM_SETITEM, 0, (LPARAM)&tvi) != 0);
}

HTREEITEM TreeCtrl::GetTreeItem(TreeNode^ node)
{
	return static_cast<HTREEITEM>(node->Handle.ToPointer());
}

HWND TreeCtrl::GetTreeHwnd(TreeNode^ node)
{
	return static_cast<HWND>(node->TreeView->Handle.ToPointer());
}

TreeNode^ TreeCtrl::GetNextItem(TreeNode^ node)
{
	if (node == nullptr)
		return nullptr;

	// try our first child if we have one
	if (node->Nodes->Count > 0)
		return node->Nodes[0];

	// else try next sibling
	TreeNode^ next = node->NextNode;

	// finally look up the parent chain as far as we can
	if (next == nullptr)
	{
		TreeNode^ parent = node;

		while ((parent != nullptr) && (next == nullptr))
		{
			parent = parent->Parent;

			if (parent != nullptr)
				next = parent->NextNode;
		}
	}

	return next;
}

TreeNode^ TreeCtrl::GetPrevItem(TreeNode^ node)
{
	if (node == nullptr)
		return nullptr;

	// try our prior sibling
	TreeNode^ prev = node->PrevNode;

	// if we have one then first try its last child
	if (prev != nullptr)
	{
		prev = GetLastItem(prev);
	}
	else // get parent
	{
		prev = node->Parent;
	}

	return prev;
}

TreeNode^ TreeCtrl::GetLastItem(TreeNode^ node)
{
	if (node == nullptr)
		return nullptr;

	if (node->Nodes->Count == 0)
		return node;

	return GetLastItem(node->LastNode); // RECURSIVE CALL
}

////////////////////////////////////////////////////////////////////////////////////////////////
