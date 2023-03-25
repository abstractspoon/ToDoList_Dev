using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Diagnostics;

namespace DetectiveUIExtension
{
	public class Node
	{
		public Node(string title, uint uniqueId, List<uint> linkIds)
		{
			Title = title;
			UniqueId = uniqueId;

			LinkIds = linkIds;
		}

		public uint UniqueId
		{
			get;
			private set;
		}

		public string Title;
		public List<uint> LinkIds;
		public bool HasLinks { get { return LinkIds.Count > 0; } }

		public bool LinksTo(Node item)
		{
			return (LinkIds.Contains(item.UniqueId) || item.LinkIds.Contains(UniqueId));
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

