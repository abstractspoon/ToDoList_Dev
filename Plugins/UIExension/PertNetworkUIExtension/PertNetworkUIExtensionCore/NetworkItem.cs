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

		public static Point NullPoint { get { return new Point(-1, -1); } }
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

	public class NetworkGroups : HashSet<NetworkGroup>
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
					maxPos.Y = groupMaxPos.Y;
					maxPos.X = Math.Max(maxPos.X, groupMaxPos.X);
				}
			}

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

	}

	// ------------------------------------------------------------

	public class NetworkData
	{
		private NetworkItems m_Items;
		private NetworkGroups m_Groups;
		private NetworkMatrix m_Matrix;

		// -------------------------------

		public NetworkData()
		{
			m_Items = new NetworkItems();
			m_Groups = new NetworkGroups();
			m_Matrix = new NetworkMatrix();
		}

		public NetworkItems Items
		{
			get { return m_Items; }
		}

		public IEnumerable<NetworkItem> ItemValues
		{
			get { return m_Items.Values; }
		}

		public NetworkGroups Groups
		{
			get { return m_Groups; }
		}

		public NetworkMatrix Matrix
		{
			get
			{
				if (m_Matrix.IsEmpty)
					m_Matrix.Populate(m_Groups);

				return m_Matrix;
			}
		}

		public void Clear()
		{
			m_Items.Clear();
			m_Groups.Clear();
			m_Matrix.Clear();
		}

		public NetworkItem GetItem(uint uniqueId)
		{
			return m_Items.GetItem(uniqueId);
		}

		public bool DeleteItem(uint uniqueId)
		{
			return m_Items.DeleteItem(uniqueId);
		}

		public bool AddItem(NetworkItem item)
		{
			return m_Items.AddItem(item);
		}

		public Point RebuildGroups()
		{
			m_Groups.RebuildGroups(Items);
			m_Matrix.Clear(); // populated on demand

			return m_Groups.MaxPos;
		}
	}

	// ------------------------------------------------------------
	
	public class NetworkGroup
	{
		private NetworkItems m_Items;

		// ----------------

		public NetworkGroup()
		{
			m_Items = new NetworkItems();
		}

		public Point MaxPos { get; private set; }

		public NetworkItems Items
		{
			get { return m_Items; }
		}

		public IEnumerable<NetworkItem> ItemValues
		{
			get { return m_Items.Values; }
		}

		public bool Build(NetworkItem endItem, NetworkItems allItems, Point startPos)
		{
			Clear();

			Point maxPos = startPos;

			if (!AddItem(endItem, allItems, ref maxPos))
				return false;

			MaxPos = maxPos;

			BalanceVerticalPositions();
			
			return true;
		}

		public List<NetworkItem> GetItemDependencies(NetworkItem item)
		{
			return m_Items.GetItemDependencies(item);
		}

		// ----------------------------------
		// Internals

		private void Clear()
		{
			m_Items.Clear();
			MaxPos = Point.Empty;
		}

		private bool AddItem(NetworkItem item, NetworkItems allItems, ref Point maxPos)
		{
			// Must be unique
			if (m_Items.ContainsKey(item.UniqueId))
				return false;

			m_Items.Add(item.UniqueId, item);

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

		private void BalanceVerticalPositions()
		{
			// Try moving an item towards the centre of its dependencies
			// and stop when we hit a position already taken.
			var subGroups = BuildHorizontalSubGroups();
			List<NetworkItem> subGroup = null;

			for (int hPos = 1; hPos < subGroups.Count; hPos++)
			{
				if (subGroups.TryGetValue(hPos, out subGroup))
				{
					foreach (var item in subGroup)
					{
						var dependencies = GetItemDependencies(item);
						var midY = GetMidVPos(dependencies);

						for (int vPos = item.Position.Y; vPos <= midY; vPos++)
						{
							if (IsPositionTaken(subGroup, hPos, vPos))
								break;

							// else
							item.Position.Y = vPos;
						}
					}
				}
			}

			// For the first group do the same but with its dependents
			if (subGroups.TryGetValue(0, out subGroup))
			{
				var item = subGroup[0];
				var dependents = m_Items.GetItemDependents(item);
				var midY = GetMidVPos(dependents);

				if (!IsPositionTaken(subGroup, 0, midY))
					item.Position.Y = midY;
			}

			// Recalculate maximum vertical extent because it might have REDUCED
			int maxY = 0;

			foreach (var item in ItemValues)
				maxY = Math.Max(maxY, item.Position.Y);

			MaxPos = new Point(MaxPos.X, maxY);
		}

		private Dictionary<int, List<NetworkItem>> BuildHorizontalSubGroups()
		{
			var subGroups = new Dictionary<int, List<NetworkItem>>();

			foreach (var item in ItemValues)
			{
				List<NetworkItem> subGroup = null;

				if (!subGroups.TryGetValue(item.Position.X, out subGroup))
				{
					subGroup = new List<NetworkItem>();
					subGroups.Add(item.Position.X, subGroup);
				}

				subGroup.Add(item);
			}

			// Sort the subgroups top-down
			foreach (var subGroup in subGroups.Values)
			{
				subGroup.Sort((a, b) => (a.Position.Y - b.Position.Y));
			}

			return subGroups;
		}

		private int GetMidVPos(IEnumerable<NetworkItem> items)
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

		private bool IsPositionTaken(IEnumerable<NetworkItem> items, int x, int y)
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
		private NetworkItem[,] m_Items;
		private Point m_MaxPos;

		// -------------

		public NetworkItem[,] Items { get; private set; }

		public void Clear() { m_Items = null; }
		public bool IsEmpty { get { return (m_Items == null); } }
		public Size Size { get { return new Size(m_MaxPos); } }

		public bool Populate(NetworkGroups groups)
		{
			if (groups.MaxPos == Point.Empty)
				return false;

			m_MaxPos = groups.MaxPos;
			m_Items = new NetworkItem[m_MaxPos.X + 1, m_MaxPos.Y + 1];

			foreach (var group in groups)
			{
				foreach (var item in group.ItemValues)
				{
					if (item.HasPosition)
					{
						m_Items[item.Position.X, item.Position.Y] = item;
					}
				}
			}

			return true;
		}

		public NetworkItem GetItemAt(Point pos)
		{
			if ((pos.X < 0) || (pos.X > m_MaxPos.X))
				return null;

			if ((pos.Y < 0) || (pos.Y > m_MaxPos.Y))
				return null;

			return m_Items[pos.X, pos.Y];
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
					if (Distance(startPos, parallelItem1.Position) <= Distance(startPos, parallelItem2.Position))
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
				pos.X = Math.Min(pos.X + increment, m_MaxPos.X);
				break;

			case Direction.Down:
				pos.Y = Math.Min(pos.Y + increment, m_MaxPos.Y);
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
					return (pos.X < m_MaxPos.X);

				case Direction.Down:
					return (pos.Y < m_MaxPos.Y);
				}
			}

			// all else
			return false;
		}

		private double Distance(Point pos1, Point pos2)
		{
			int diffX = (pos1.X - pos2.X);
			int diffY = (pos1.Y - pos2.Y);

			return Math.Sqrt((diffX * diffX) + (diffY * diffY));
		}
	}

	// ------------------------------------------------------------

}

