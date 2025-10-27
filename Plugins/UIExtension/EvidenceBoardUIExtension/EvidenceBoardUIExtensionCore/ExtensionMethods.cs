using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace EvidenceBoardUIExtension
{
	public static class ExtensionMethods
	{
		public static string EncodeBase64(this string value)
		{
			try
			{
				var valueBytes = Encoding.Unicode.GetBytes(value);
				return Convert.ToBase64String(valueBytes);
			}
			catch
			{
			}

			return string.Empty;
		}

		public static string DecodeBase64(this string value)
		{
			try
			{
				var valueBytes = Convert.FromBase64String(value);
				return Encoding.Unicode.GetString(valueBytes);
			}
			catch
			{
			}

			return string.Empty;
		}

		public static System.Drawing.Point GetPosition(this RadialTree.Point point)
		{
			return new System.Drawing.Point((int)point.X, (int)point.Y);
		}

		public static void SetPosition(this RadialTree.Point point, System.Drawing.Point dPoint)
		{
			point.X = dPoint.X;
			point.Y = dPoint.Y;
		}

		public static System.Drawing.Point GetPosition(this RadialTree.Point point, System.Drawing.Size offset)
		{
			return new System.Drawing.Point((int)point.X + offset.Width, (int)point.Y + offset.Height);
		}

		public static System.Drawing.Point GetPosition<T>(this RadialTree.TreeNode<T> node)
		{
			return node.Point.GetPosition();
		}
		
		public static System.Drawing.Point GetPosition<T>(this RadialTree.TreeNode<T> node, System.Drawing.Size offset)
		{
			return node.Point.GetPosition(offset);
		}

		public static System.Drawing.Rectangle Multiply(this System.Drawing.Rectangle rect, float mult)
		{
			return new System.Drawing.Rectangle(rect.Location.Multiply(mult), rect.Size.Multiply(mult));
		}

		public static System.Drawing.Size Multiply(this System.Drawing.Size size, float mult)
		{
			return new System.Drawing.Size((int)(size.Width * mult), (int)(size.Height * mult));
		}

		public static System.Drawing.Point Multiply(this System.Drawing.Point pt, float mult)
		{
			return new System.Drawing.Point((int)(pt.X * mult), (int)(pt.Y * mult));
		}

		public static System.Drawing.Point Divide(this System.Drawing.Point pt, float div)
		{
			return new System.Drawing.Point((int)(pt.X / div), (int)(pt.Y / div));
		}

		public static System.Drawing.Size Divide(this System.Drawing.Size size, float div)
		{
			return new System.Drawing.Size((int)(size.Width / div), (int)(size.Height / div));
		}

		public static void MoveToHead<T>(this IList<T> list, T item)
		{
			list.Remove(item);
			list.Insert(0, item);
		}

		public static void Add<T>(this IList<T> list, IList<T> other)
		{
			foreach (T val in other)
				list.Add(val);
		}

		public static void Remove<T>(this IList<T> list, IList<T> other)
		{
			foreach (T val in other)
				list.Remove(val);
		}
	}
}
