using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Diagnostics;

namespace DetectiveUIExtension
{
	public class Node : IComparable
	{
		public Node(string title, uint uniqueId, uint parentId, List<uint> linkIds)
		{
			Title = title;
			UniqueId = uniqueId;
			ParentId = parentId;

			Position = NullPoint;
			LinkIds = linkIds;
		}

		public uint UniqueId
		{
			get;
			private set;
		}

		public uint ParentId
		{
			get;
			private set;
		}

		public string Title;
		public Point Position;
		public bool HasPosition { get { return (Position != NullPoint); } }

		public List<uint> LinkIds;
		public bool HasLinks { get { return LinkIds.Count > 0; } }

		public bool LinksTo(Node item)
		{
			return (LinkIds.Contains(item.UniqueId) || item.LinkIds.Contains(UniqueId));
		}

		public double Distance(Node item)
		{
			return Distance(Position, item.Position);
		}

		// Statics 
		public static Point NullPoint { get { return new Point(-1, -1); } }

		public static double Distance(Point pos1, Point pos2)
		{
			int diffX = (pos1.X - pos2.X);
			int diffY = (pos1.Y - pos2.Y);

			return Math.Sqrt((diffX * diffX) + (diffY * diffY));
		}

		public virtual int CompareTo(object other)
		{
			return ((int)UniqueId - (int)((Node)other).UniqueId);
		}
	}

	// ------------------------------------------------------------

	public class Nodes : Dictionary<uint, Node>
	{
		public Node GetNode(uint uniqueId)
		{
			Node item = null;
			TryGetValue(uniqueId, out item);

			return item;
		}

		public bool DeleteNode(uint uniqueId)
		{
			if (!ContainsKey(uniqueId))
				return false;

			Remove(uniqueId);
			return true;
		}

		public bool AddNode(Node node)
		{
			if (ContainsKey(node.UniqueId))
				return false;

			Add(node.UniqueId, node);
			return true;
		}
	}


}

