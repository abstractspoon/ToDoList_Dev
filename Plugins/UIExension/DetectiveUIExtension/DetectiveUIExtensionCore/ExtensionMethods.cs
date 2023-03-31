using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace DetectiveUIExtension
{
	public static class ExtensionMethods
	{
		public static System.Drawing.Point GetPosition(this RadialTree.Point point)
		{
			return new System.Drawing.Point((int)point.X, (int)point.Y);
		}

		public static System.Drawing.Point GetPosition(this RadialTree.Point point, System.Drawing.Size offset)
		{
			return new System.Drawing.Point((int)point.X + offset.Width, (int)point.Y + offset.Height);
		}

		public static System.Drawing.Rectangle GetRectangle(this RadialTree.Point point, System.Drawing.Size size)
		{
			return point.GetRectangle(size, System.Drawing.Size.Empty);
		}

		public static System.Drawing.Rectangle GetRectangle(this RadialTree.Point point, System.Drawing.Size size, System.Drawing.Size offset)
		{
			var pos = GetPosition(point, offset);

			return new System.Drawing.Rectangle(pos.X - (size.Width / 2),
												pos.Y - (size.Height / 2),
												size.Width,
												size.Height);
		}

		public static System.Drawing.Point GetPosition<T>(this RadialTree.TreeNode<T> node)
		{
			return node.Point.GetPosition();
		}
		
		public static System.Drawing.Point GetPosition<T>(this RadialTree.TreeNode<T> node, System.Drawing.Size offset)
		{
			return node.Point.GetPosition(offset);
		}

		public static System.Drawing.Rectangle GetRectangle<T>(this RadialTree.TreeNode<T> node, System.Drawing.Size size)
		{
			return node.Point.GetRectangle(size);
		}

		public static System.Drawing.Rectangle GetRectangle<T>(this RadialTree.TreeNode<T> node, System.Drawing.Size size, System.Drawing.Size offset)
		{
			return node.Point.GetRectangle(size, offset);
		}
	}
}
