using System;
using System.Linq;
using System.Windows.Forms;

using BaseNode = RadialTree.TreeNode<uint>;

namespace EvidenceBoardUIExtension
{
	class NodeNavigation
	{
		static public BaseNode GetNextNode(BaseNode node, Keys keypress, ref bool extentsChange)
		{
			if (Control.ModifierKeys != Keys.None)
				return null;

			switch (keypress)
			{
			case Keys.Up:		return NextNodeUp(node);
			case Keys.Down:		return NextNodeDown(node, node.IsExpanded);
			case Keys.Left:		return NextNodeLeft(node, ref extentsChange);
			case Keys.Right:	return NextNodeRight(node, ref extentsChange);
			}

			return null;
		}

		static BaseNode LastVisibleChild(BaseNode node)
		{
			if (node.IsLeaf || node.IsCollapsed)
				return null;

			var lastChild = LastVisibleChild(node.Children.Last());

			if (lastChild != null)
				return lastChild;

			// else
			return node.Children.Last();
		}

		static BaseNode NextNodeLeft(BaseNode node, ref bool extentsChange)
		{
			if (node.Expand(false))
			{
				extentsChange = true;
				return node;
			}
			
			// else
			return node.Parent;
		}

		static BaseNode NextNodeRight(BaseNode node, ref bool extentsChange)
		{
			if (node.Expand(true))
			{
				extentsChange = true;
				return node;
			}

			// else
			return node.FirstChild;
		}

		static BaseNode NextNodeUp(BaseNode node)
		{
			if (node.Parent == null)
				return null;

			int pos = node.Parent.Children.IndexOf(node);

			if (pos == 0)
				return node.Parent;

			var prevNode = LastVisibleChild(node.Parent.Children[pos - 1]);

			if (prevNode != null)
				return prevNode;

			return node.Parent.Children[pos - 1];
		}

		static BaseNode NextNodeDown(BaseNode node, bool incChildren)
		{
			// 1. If expanded, retrieve first child
			if (incChildren && node.IsExpanded)
				return node.FirstChild;

			// 2. look for next sibling
			var parent = node.Parent;

			if (parent == null)
				return null;

			int pos = parent.Children.IndexOf(node);

			if (pos < (parent.Children.Count - 1))
				return parent.Children[pos + 1];

			// 3. Pass on to our parent but excluding
			// its children which would otherwise include us
			return NextNodeDown(parent, false);
		}
	}

}
