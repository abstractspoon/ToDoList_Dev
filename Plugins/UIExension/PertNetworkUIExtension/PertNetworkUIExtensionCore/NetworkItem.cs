using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Diagnostics;

namespace PertNetworkUIExtension
{
	public class NetworkItem : IComparable
	{
		public NetworkItem(string title, uint uniqueId, List<uint> dependencyIds)
		{
			Title = title;
			UniqueId = uniqueId;

			Position = NullPoint;
			DependencyUniqueIds = dependencyIds;
		}

		public uint UniqueId
		{
			get;
			private set;
		}


		public void ClearPosition()
		{
			Position = NullPoint;
		}

		public string Title;
		public Point Position;
		public bool HasPosition { get { return (Position != NullPoint); } }

		public List<uint> DependencyUniqueIds;
		public bool HasDependencies { get { return DependencyUniqueIds.Count > 0; } }

		public bool IsDependency(NetworkItem item)
		{
			return DependencyUniqueIds.Contains(item.UniqueId);
		}

		public double Distance(NetworkItem item)
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
			return ((int)UniqueId - (int)((NetworkItem)other).UniqueId);
		}
	}

	// ------------------------------------------------------------

	public class NetworkItems : Dictionary<uint, NetworkItem>
	{
		public NetworkItem GetItem(uint uniqueId)
		{
			NetworkItem item = null;
			TryGetValue(uniqueId, out item);

			return item;
		}

		public bool DeleteItem(uint uniqueId)
		{
			if (!ContainsKey(uniqueId))
				return false;

			Remove(uniqueId);
			return true;
		}

		public bool AddItem(NetworkItem item)
		{
			if (ContainsKey(item.UniqueId))
				return false;

			Add(item.UniqueId, item);
			return true;
		}
	}

	// ------------------------------------------------------------

	public class NetworkDependents : Dictionary<uint, HashSet<uint>>
	{
		public int Build(NetworkItems allItems)
		{
			Clear();

			foreach (var item in allItems.Values)
			{
				foreach (uint dependId in item.DependencyUniqueIds)
				{
					HashSet<uint> dependents = null;

					if (!TryGetValue(dependId, out dependents))
					{
						dependents = new HashSet<uint>();
						Add(dependId, dependents);
					}

					dependents.Add(item.UniqueId);
				}
			}

			return Count;
		}

		public List<uint> GetDependents(uint id)
		{
			HashSet<uint> dependents = null;

			if (!TryGetValue(id, out dependents) || (dependents == null))
				return null;

			return dependents.ToList();
		}

		public bool HasDependents(uint id)
		{
			return ContainsKey(id);
		}

	};

	// ------------------------------------------------------------

	public class NetworkPath : IComparable
	{
		private List<NetworkItem> m_Items { get; set; }

		// ----------------------------

		public Point MaxPos { get; private set; }

		public NetworkPath(NetworkPath existPath = null)
		{
			m_Items = new List<NetworkItem>();

			// copy references from other paths
			if (existPath != null)
			{
				foreach (var item in existPath.m_Items)
					AddItem(item);
			}

			MaxPos = NetworkItem.NullPoint;
		}

		public IList<NetworkItem> Items
		{
			get
			{
				return m_Items;
			}
		}

		public bool Contains(NetworkItem item)
		{
			return m_Items.Contains(item);
		}

		public bool AddItem(NetworkItem item)
		{
			// prevent duplicates/circular paths
			if (Contains(item))
				return false;

			m_Items.Add(item);
			return true;
		}

		public double Length
		{
			get
			{
				double length = 0;

				for (int i = 1; i < m_Items.Count; i++)
					length += m_Items[i - 1].Distance(m_Items[i]);

				return length;
			}
		}

		public int Count
		{
			get
			{
				return m_Items.Count;
			}
		}

		public Point LayoutPath(int nextAvailYPos)
		{
			int nextAvailXPos = 0; // always

			for (int i = 0; i < Count; i++)
			{
				var item = m_Items[i];

				if (item.HasPosition)
				{
					// Stay ahead of previously positioned dependencies
					nextAvailXPos = Math.Max(nextAvailXPos, item.Position.X);
				}
				else
				{
					item.Position = new Point(nextAvailXPos, nextAvailYPos);
				}

				if (i < (Count - 1))
					nextAvailXPos++;
			}

			MaxPos = new Point(nextAvailXPos, nextAvailYPos);
			return MaxPos;
		}

		virtual public int CompareTo(object other)
		{
			NetworkPath otherPath = (other as NetworkPath);

			if (otherPath == null)
				return 0;

			int numItems = Math.Min(m_Items.Count, otherPath.m_Items.Count);

			for (int i = 0; i < numItems; i++)
			{
				NetworkItem item = m_Items[i];
				NetworkItem otherItem = otherPath.m_Items[i];

				int compare = item.CompareTo(otherItem);

				if (compare != 0)
					return compare;
			}

			return (m_Items.Count - otherPath.m_Items.Count);
		}
	}

	// ------------------------------------------------------------

	public class NetworkPaths : List<NetworkPath>
	{
		public Point MaxPos { get; private set; }

		public int RebuildPaths(NetworkItems allItems, NetworkDependents dependents)
		{
			Clear(allItems);

			var startIds = GetPathStartIds(allItems, dependents);

			foreach (uint startId in startIds)
			{
				CreatePath(startId, allItems, dependents, null);
			}

			MaxPos = LayoutPaths(allItems);

			return Count;
		}

		// ----------------------------------
		// Internals

		private void Clear(NetworkItems allItems)
		{
			base.Clear();

			MaxPos = NetworkItem.NullPoint;

			foreach (var item in allItems.Values)
				item.Position = NetworkItem.NullPoint;
		}

		private int CreatePath(uint id, NetworkItems allItems, NetworkDependents allDependents, NetworkPath existPath)
		{
			var item = allItems.GetItem(id);

			if (item != null)
			{
				var path = new NetworkPath(existPath);
				Add(path);

				do
				{
					if (!path.AddItem(item))
					{
						// Circular path
						break;
					}

					// Recursively process this item's dependents
					var dependents = allDependents.GetDependents(item.UniqueId);

					// Last item in path won't have any dependents
					if (dependents == null)
					{
						break;
					}

					// Process first dependency last so that if we need to copy
					// the current path we do so before it gets added to
					for (int i = 1; i < dependents.Count; i++)
					{
						// else build a new path recursively
						CreatePath(dependents[i], allItems, allDependents, path); // RECURSIVE CALL
					}

					// Next item in this path
					item = allItems.GetItem(dependents[0]);
				}
				while (true);
			}

			return Count;
		}

		private HashSet<uint> GetPathStartIds(NetworkItems allItems, NetworkDependents dependents)
		{
			var startIds = new HashSet<uint>();

			// All items having no dependencies of their own
			// but on whom other tasks are dependent
			foreach (var item in allItems.Values)
			{
				if ((item.DependencyUniqueIds.Count == 0) && dependents.ContainsKey(item.UniqueId))
					startIds.Add(item.UniqueId);
			}

			return startIds;
		}

		private Point LayoutPaths(NetworkItems allItems)
		{
			Sort();

			// First pass just sets a basic position
			int maxXPos = 0, maxYPos = 0;

			foreach (var path in this)
			{
				var maxPathPos = path.LayoutPath(maxYPos);

				maxXPos = Math.Max(maxXPos, maxPathPos.X);
				maxYPos++;
			}
			maxYPos--;

			// Second pass checks to ensure that tasks in the earlier
			// paths come after the tasks on which they are dependent
			foreach (var path in this)
			{
				for (int i = 0; i < path.Count; i++)
				{
					NetworkItem item = path.Items[i];

					foreach (uint dependencyId in item.DependencyUniqueIds)
					{
						var dependency = allItems.GetItem(dependencyId);

						if (item.Position.X <= dependency.Position.X)
						{
							// Bump item and all items after it in the path
							int offset = (dependency.Position.X - item.Position.X) + 1;

							for (int j = i; j < path.Count; j++)
							{
								path.Items[j].Position.X += offset;
							}
						}
					}
				}

				maxXPos = Math.Max(maxXPos, path.Items[path.Count - 1].Position.X);
			}

			MaxPos = new Point(maxXPos, maxYPos);

			return MaxPos;
		}

	}

	// ------------------------------------------------------------

	public class NetworkMatrix
	{
		public NetworkItem[,] Items { get; private set; }

		public void Clear() { Items = null; }
		public bool IsEmpty { get { return (Items == null); } }
		public Size Size { get; private set; }

		public bool Populate(NetworkPaths paths)
		{
			if (paths.MaxPos == Point.Empty)
				return false;

			Size = new Size(paths.MaxPos);
			Items = new NetworkItem[Size.Width + 1, Size.Height + 1];

			foreach (var path in paths)
			{
				foreach (var item in path.Items)
				{
					if (item.HasPosition)
					{
						Items[item.Position.X, item.Position.Y] = item;
					}
				}
			}

			return true;
		}

		public NetworkItem GetItemAt(Point pos)
		{
			if ((pos.X < 0) || (pos.X > Size.Width))
				return null;

			if ((pos.Y < 0) || (pos.Y > Size.Height))
				return null;

			return Items[pos.X, pos.Y];
		}

		public enum Direction
		{
			Left,
			Up,
			Right,
			Down,
		}

		public NetworkItem GetNextNearestItem(Point startPos, Direction dir, int increment = 1)
		{
			// 1. Look along the same row or column until we find an item
			NetworkItem nextItem = GetNextNearestItemOnSameRowOrCol(startPos, dir, increment);

			if (nextItem != null)
				return nextItem;

			// 2. Progressively search parallel rows or columns until we find an item
			Point nextPos = startPos;

			while (Increment(ref nextPos, dir, increment))
			{
				NetworkItem parallelItem1 = null, parallelItem2 = null;

				switch (dir)
				{
				case Direction.Left:
				case Direction.Right:
					parallelItem1 = GetNextNearestItemOnSameRowOrCol(nextPos, Direction.Up, 1);
					parallelItem2 = GetNextNearestItemOnSameRowOrCol(nextPos, Direction.Down, 1);
					break;

				case Direction.Up:
				case Direction.Down:
					parallelItem1 = GetNextNearestItemOnSameRowOrCol(nextPos, Direction.Left, 1);
					parallelItem2 = GetNextNearestItemOnSameRowOrCol(nextPos, Direction.Right, 1);
					break;
				}

				if ((parallelItem1 != null) && (parallelItem2 != null))
				{
					double dist1 = NetworkItem.Distance(startPos, parallelItem1.Position);
					double dist2 = NetworkItem.Distance(startPos, parallelItem2.Position);

					if (dist1 <= dist2)
						return parallelItem1;

					// else
					return parallelItem2;
				}

				if (parallelItem1 != null)
					return parallelItem1;

				if (parallelItem2 != null)
					return parallelItem2;

				// keep going
			}

			return null;
		}

		// ---------------------------------------
		// Internals

		private NetworkItem GetNextItem(Point startPos, Direction dir, int increment)
		{
			var nextPos = startPos;
			Increment(ref nextPos, dir, increment);

			return GetItemAt(nextPos);
		}

		private NetworkItem GetNextNearestItemOnSameRowOrCol(Point startPos, Direction dir, int increment = 1)
		{
			NetworkItem nextItem = null;
			var nextPos = startPos;

			while ((nextItem == null) && Increment(ref nextPos, dir, increment))
			{
				nextItem = GetItemAt(nextPos);
			}

			return nextItem;
		}

		private bool Increment(ref Point pos, Direction dir, int increment)
		{
			if (!CanIncrement(pos, dir, increment))
				return false;

			switch (dir)
			{
			case Direction.Left:
				pos.X = Math.Max(pos.X - increment, 0);
				break;

			case Direction.Up:
				pos.Y = Math.Max(pos.Y - increment, 0);
				break;

			case Direction.Right:
				pos.X = Math.Min(pos.X + increment, Size.Width);
				break;

			case Direction.Down:
				pos.Y = Math.Min(pos.Y + increment, Size.Height);
				break;
			}

			return true;
		}

		private bool CanIncrement(Point pos, Direction dir, int increment)
		{
			if (increment > 0)
			{
				switch (dir)
				{
				case Direction.Left:
					return (pos.X > 0);

				case Direction.Up:
					return (pos.Y > 0);

				case Direction.Right:
					return (pos.X < Size.Width);

				case Direction.Down:
					return (pos.Y < Size.Height);
				}
			}

			// all else
			return false;
		}
	}

	// ------------------------------------------------------------

	public class NetworkData
	{
		private NetworkDependents m_Dependents = null;

		// -------------------------

		public NetworkData()
		{
			AllItems = new NetworkItems();
			AllPaths = new NetworkPaths();
			ItemMatrix = new NetworkMatrix();
		}

		public NetworkItems AllItems { get; private set; }
		public NetworkPaths AllPaths { get; private set; }
		public NetworkMatrix ItemMatrix { get; private set; }

		public NetworkDependents AllDependents
		{
			get
			{
				if (m_Dependents == null)
				{
					m_Dependents = new NetworkDependents();
					m_Dependents.Build(AllItems);
				}

				return m_Dependents;
			}
		}

		public IEnumerable<NetworkItem> ItemValues { get { return AllItems.Values; } }

		public void Clear()
		{
			AllItems.Clear();
			AllPaths.Clear();
			ItemMatrix.Clear();
		}

		public NetworkItem GetItem(uint uniqueId)
		{
			return AllItems.GetItem(uniqueId);
		}

		public bool DeleteItem(uint uniqueId)
		{
			if (!AllItems.DeleteItem(uniqueId))
				return false;

			m_Dependents = null;
			return true;
		}

		public bool AddItem(NetworkItem item)
		{
			if (!AllItems.AddItem(item))
				return false;

			m_Dependents = null;
			return true;
		}

		public Point RebuildPaths()
		{
			AllPaths.RebuildPaths(AllItems, AllDependents);
			ItemMatrix.Populate(AllPaths);

			return AllPaths.MaxPos;
		}
	}

	// ------------------------------------------------------------

}

