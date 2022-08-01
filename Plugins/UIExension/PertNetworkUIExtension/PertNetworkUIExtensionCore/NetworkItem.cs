using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Diagnostics;

namespace PertNetworkUIExtension
{
	public class NetworkItem
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

		public HashSet<uint> GetAllDependentIds()
		{
			var dependentIds = new HashSet<uint>();
			
			foreach (var item in Values)
			{
				foreach (var depend in item.DependencyUniqueIds)
					dependentIds.Add(depend);
			}

			return dependentIds;
		}

		public HashSet<uint> GetAllEndIds(HashSet<uint> dependentIds)
		{
			HashSet<uint> endIds = new HashSet<uint>();

			foreach (var item in Values)
			{
				if ((item.DependencyUniqueIds.Count > 0) && !dependentIds.Contains(item.UniqueId))
					endIds.Add(item.UniqueId);
			}

			return endIds;
		}

		public List<NetworkItem> GetItemDependents(NetworkItem item)
		{
			return Values.Where(a => a.DependencyUniqueIds.Contains(item.UniqueId)).ToList();
		}

		public List<NetworkItem> GetItemDependencies(NetworkItem item)
		{
			var dependencies = new List<NetworkItem>();

			foreach (uint dependId in item.DependencyUniqueIds)
				dependencies.Add(GetItem(dependId));

			return dependencies;
		}
	}

	// ------------------------------------------------------------

	public class NetworkPath
	{
		private List<NetworkItem> m_Items { get; set; }

		// ----------------------------

		public NetworkPath(NetworkPath existPath = null)
		{
			m_Items = new List<NetworkItem>();

			// copy references from other paths
			if (existPath != null)
			{
				foreach (var item in existPath.m_Items)
					AddItem(item);
			}
		}

		public IEnumerable<NetworkItem> Items
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

		public void AddItem(NetworkItem item)
		{
			m_Items.Add(item);
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
	}

	// ------------------------------------------------------------

	public class NetworkPaths : List<NetworkPath>
	{
		public int BuildPaths(NetworkGroup group)
		{
			Clear();

			return CreatePath(group.EndItem, group.Items, null);
		}

		// ----------------------------------
		// Internals

		private int CreatePath(NetworkItem item, NetworkItems groupItems, NetworkPath existPath)
		{
			var path = new NetworkPath(existPath);
			Add(path);

			do
			{
				// Add this item
				path.AddItem(item);

				// Process this item's dependencies
				if (item.DependencyUniqueIds.Count > 0)
				{
					var dependItems = groupItems.GetItemDependencies(item);

					// Process first dependency last so that if we need to copy
					// the current path we do so before it gets added to
					for (int i = 1; i < dependItems.Count; i++)
					{
						// else build a new path recursively
						CreatePath(dependItems[i], groupItems, path); // RECURSIVE CALL
					}

					// Add first dependency to the current path
					item = dependItems[0];
					path.AddItem(item);
				}
			}
			while (item.DependencyUniqueIds.Count > 0);

			// Reverse the path so it's forward-facing
			path.Items.Reverse();

			return Count;
		}
	}

	// ------------------------------------------------------------

	public class NetworkGroup
	{
		public NetworkGroup()
		{
			Items = new NetworkItems();
			Paths = new NetworkPaths();
		}

		public Point MaxPos { get; private set; }
		public NetworkItems Items { get; private set; }
		public NetworkPaths Paths { get; private set; }
		public NetworkItem EndItem { get; private set; }

		public IEnumerable<NetworkItem> ItemValues
		{
			get { return Items.Values; }
		}

		public bool Build(NetworkItem endItem, NetworkItems allItems, Point startPos)
		{
			Clear();

			Point maxPos = startPos;

			if (!AddItem(endItem, allItems, ref maxPos))
				return false;

			MaxPos = maxPos;
			EndItem = endItem;

			Paths.BuildPaths(this);

			return true;
		}

		public List<NetworkItem> GetItemDependencies(NetworkItem item)
		{
			return Items.GetItemDependencies(item);
		}

		// ----------------------------------
		// Internals

		private void Clear()
		{
			Items.Clear();
			Paths.Clear();

			MaxPos = Point.Empty;
		}

		private bool AddItem(NetworkItem item, NetworkItems allItems, ref Point maxPos)
		{
			if (item == null)
			{
				Debug.Assert(false);
				return false;
			}

			// Must be unique
			if (Items.ContainsKey(item.UniqueId))
			{
				return false;
			}

			Items.Add(item.UniqueId, item);

			// Don't modify the vertical position of an item already processed in another group
			if (!item.HasPosition)
				item.Position.Y = maxPos.Y;

			// This item's dependencies (can be zero)
			if (item.DependencyUniqueIds.Count > 0)
			{
				bool firstDepend = true;

				foreach (var dependId in item.DependencyUniqueIds)
				{
					NetworkItem dependItem = allItems.GetItem(dependId);

					if (dependItem != null)
					{
						// First dependency shares same VPos as prior item
						if (!firstDepend)
							maxPos.Y++;
						else
							firstDepend = false;

						AddItem(dependItem, allItems, ref maxPos); // RECURSIVE call

						item.Position.X = Math.Max(dependItem.Position.X + 1, item.Position.X);
					}
				}

				maxPos.X = Math.Max(maxPos.X, item.Position.X);
			}
			else // First item in group
			{
				item.Position.X = maxPos.X = 0;
			}

			return true;
		}

	}

	// ------------------------------------------------------------

	public class NetworkGroups : List<NetworkGroup>
	{
		public Point MaxPos { get; private set; }

		public int RebuildGroups(NetworkItems allItems)
		{
			Point maxPos;
			int numGroups = RebuildGroups(allItems, out maxPos);

			MaxPos = maxPos;
			return numGroups;
		}

		// -------------------------------------------
		// internals

		private new void Clear()
		{
			MaxPos = Point.Empty;
			base.Clear();
		}

		private int RebuildGroups(NetworkItems allItems, out Point maxPos)
		{
			// Clear all item positions
			foreach (var item in allItems.Values)
				item.ClearPosition();

			Clear();

			// Get the set of all items on whom other items are dependent
			HashSet<uint> dependentIds = allItems.GetAllDependentIds();

			// Get the set of all items which have dependencies but 
			// on whom NO other items are dependent
			HashSet<uint> endIds = allItems.GetAllEndIds(dependentIds);

			// Build the groups by working backwards from each end item
			maxPos = Point.Empty;

			foreach (var endId in endIds)
			{
				NetworkItem endItem = allItems.GetItem(endId);
				Point groupMaxPos = new Point(0, maxPos.Y);

				if (Count > 0)
					groupMaxPos.Y++;

				if (AddGroup(endItem, allItems, ref groupMaxPos))
				{
					maxPos.Y = Math.Max(maxPos.Y, groupMaxPos.Y);
					maxPos.X = Math.Max(maxPos.X, groupMaxPos.X);
				}
			}

			BalanceVerticalPositions(allItems);

			return Count;
		}

		private bool AddGroup(NetworkItem endItem, NetworkItems allItems, ref Point maxPos)
		{
			var group = new NetworkGroup();

			if (!group.Build(endItem, allItems, maxPos))
				return false;

			Add(group);
			maxPos = group.MaxPos;

			return true;
		}

		private void BalanceVerticalPositions(NetworkItems allItems)
		{
			// Try moving an item towards the centre of its dependencies
			// and stop when we hit a position already taken.
			var allColumns = BuildItemColumns(allItems.Values);

			foreach (var group in this)
			{
				var columns = BuildItemColumns(group.ItemValues);
				List<NetworkItem> column = null;

				for (int hPos = 1; hPos < columns.Count; hPos++)
				{
					if (columns.TryGetValue(hPos, out column))
					{
						foreach (var item in column)
						{
							var dependencies = group.GetItemDependencies(item);
							var midY = GetMidVPos(dependencies);

							if (midY < item.Position.Y) // midY is above
							{
								for (int vPos = midY; vPos < item.Position.Y; vPos++)
								{
									if (!IsPositionTaken(allItems.Values, hPos, vPos))
									{
										item.Position.Y = vPos;
										break;
									}
								}
							}
							else if (midY > item.Position.Y) // midY is below
							{
								for (int vPos = midY; vPos > item.Position.Y; vPos--)
								{
									if (!IsPositionTaken(allItems.Values, hPos, vPos))
									{
										item.Position.Y = vPos;
										break;
									}
								}
							}
							else
							{
								// already where we want
							}
						}
					}
				}

				// For the first column do the same but with its dependents
				if (columns.TryGetValue(0, out column))
				{
					var item = column[0];
					var dependents = allItems.GetItemDependents(item);
					var midY = GetMidVPos(dependents);

					if (!IsPositionTaken(allItems.Values, 0, midY))
						item.Position.Y = midY;
				}
				else
				{
					Debug.Assert(false);
				}
			}


			// Recalculate maximum vertical extent
			int maxY = 0;

			foreach (var item in allItems.Values)
			{
				if (item.HasPosition)
					maxY = Math.Max(maxY, item.Position.Y);
			}

			MaxPos = new Point(MaxPos.X, maxY);
		}

		private static Dictionary<int, List<NetworkItem>> BuildItemColumns(IEnumerable<NetworkItem> items)
		{
			var columns = new Dictionary<int, List<NetworkItem>>();

			foreach (var item in items)
			{
				if (item.HasPosition)
				{
					List<NetworkItem> column = null;

					if (!columns.TryGetValue(item.Position.X, out column))
					{
						column = new List<NetworkItem>();
						columns.Add(item.Position.X, column);
					}

					column.Add(item);
				}
			}

			// Sort the columns top-down
			foreach (var column in columns.Values)
			{
				column.Sort((a, b) => (a.Position.Y - b.Position.Y));
			}

			return columns;
		}

		private static int GetMidVPos(IEnumerable<NetworkItem> items)
		{
			int minY = -1, maxY = -1;

			foreach (var item in items)
			{
				if (minY == -1)
					minY = item.Position.Y;
				else
					minY = Math.Min(minY, item.Position.Y);

				if (maxY == -1)
					maxY = item.Position.Y;
				else
					maxY = Math.Max(maxY, item.Position.Y);
			}

			return ((minY + maxY) / 2);
		}

		private static bool IsPositionTaken(IEnumerable<NetworkItem> items, int x, int y)
		{
			foreach (var item in items)
			{
				if ((item.Position.X == x) && (item.Position.Y == y))
					return true;
			}

			return false;
		}
	}

	// ------------------------------------------------------------

	public class NetworkMatrix
	{
		public NetworkItem[,] Items { get; private set; }

		public void Clear() { Items = null; }
		public bool IsEmpty { get { return (Items == null); } }
		public Size Size { get; private set; }

		public bool Populate(NetworkGroups groups)
		{
			if (groups.MaxPos == Point.Empty)
				return false;

			Size = new Size(groups.MaxPos);
			Items = new NetworkItem[Size.Width + 1, Size.Height + 1];

			foreach (var group in groups)
			{
				foreach (var item in group.ItemValues)
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
		public NetworkData()
		{
			Items = new NetworkItems();
			Groups = new NetworkGroups();
			Matrix = new NetworkMatrix();
		}

		public NetworkItems Items { get; private set; }
		public NetworkGroups Groups { get; private set; }
		public NetworkMatrix Matrix { get; private set; }

		public IEnumerable<NetworkItem> ItemValues { get { return Items.Values; } }

		public void Clear()
		{
			Items.Clear();
			Groups.Clear();
			Matrix.Clear();
		}

		public NetworkItem GetItem(uint uniqueId)
		{
			return Items.GetItem(uniqueId);
		}

		public bool DeleteItem(uint uniqueId)
		{
			return Items.DeleteItem(uniqueId);
		}

		public bool AddItem(NetworkItem item)
		{
			return Items.AddItem(item);
		}

		public Point RebuildGroups()
		{
			Groups.RebuildGroups(Items);
			Matrix.Populate(Groups);

			return Groups.MaxPos;
		}
	}

	// ------------------------------------------------------------

}

