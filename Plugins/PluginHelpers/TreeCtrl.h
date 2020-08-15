#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////

#include <CommCtrl.h>

////////////////////////////////////////////////////////////////////////////////////////////////

using namespace System;

////////////////////////////////////////////////////////////////////////////////////////////////

namespace Abstractspoon
{
	namespace Tdl
	{
		namespace PluginHelpers
		{
			public ref class TreeCtrl
			{
			public:
				static bool SetItemIntegral(Windows::Forms::TreeNode^ node, int integral);

				static Windows::Forms::TreeNode^ GetNextItem(Windows::Forms::TreeNode^ node);
				static Windows::Forms::TreeNode^ GetPrevItem(Windows::Forms::TreeNode^ node);
				static Windows::Forms::TreeNode^ GetLastItem(Windows::Forms::TreeNode^ node);

			protected:
				static HTREEITEM GetTreeItem(Windows::Forms::TreeNode^ node);
				static HWND GetTreeHwnd(Windows::Forms::TreeNode^ node);
			};

		}
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////

