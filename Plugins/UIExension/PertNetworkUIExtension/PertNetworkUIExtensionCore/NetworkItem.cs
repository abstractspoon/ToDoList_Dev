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

		public HashSet<uint> GetAllDependents()
		{
			var dependentIds = new HashSet<uint>();
			
			foreach (var item in Values)
			{
				foreach (var depend in item.DependencyUniqueIds)
					dependentIds.Add(depend);
			}

			return dependentIds;
		}

		public HashSet<uint> GetAllTerminators(HashSet<uint> dependentIds)
		{
			HashSet<uint> terminatorIds = new HashSet<uint>();

			foreach (var item in Values)
			{
				if ((item.DependencyUniqueIds.Count > 0) && !dependentIds.Contains(item.UniqueId))
					terminatorIds.Add(item.UniqueId);
			}

			return terminatorIds;
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

		public List<NetworkItem> GetHorizontalItems(int yPos, int fromXPos = 0, int toXPos = -1)
		{
			return Values.Where(a => ((a.Position.Y == yPos) &&
										(a.Position.X >= fromXPos) &&
										((toXPos == -1) || (a.Position.X <= toXPos))))
						.OrderBy(a => a.Position.X).ToList();
		}

		public List<NetworkItem> GetVerticalItems(int xPos, int fromYPos = 0, int toYPos = -1)
		{
			return Values.Where(a => ((a.Position.X == xPos) &&
										(a.Position.Y >= fromYPos) &&
										((toYPos == -1) || (a.Position.Y <= toYPos))))
						.OrderBy(a => a.Position.Y).ToList();
		}

		public NetworkItem GetItemAtPosition(int x, int y)
		{
			return Values.Where(a => ((a.Position.X == x) && (a.Position.Y >= y))).FirstOrDefault();
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

			// Get the set of all tasks on whom other tasks are dependent
			HashSet<uint> dependentIds = allItems.GetAllDependents();

			// Get the set of all tasks which have dependencies but 
			// on whom NO other tasks are dependent
			HashSet<uint> terminatorIds = allItems.GetAllTerminators(dependentIds);

			// Build the groups by working backwards from each end task
			maxPos = Point.Empty;

			foreach (var termId in terminatorIds)
			{
				NetworkItem termItem = allItems.GetItem(termId);
				Point groupMaxPos = new Point(0, maxPos.Y);

				if (Count > 0)
					groupMaxPos.Y++;

				if (AddGroup(termItem, allItems, ref groupMaxPos))
				{
					maxPos.Y = groupMaxPos.Y;
					maxPos.X = Math.Max(maxPos.X, groupMaxPos.X);
				}
			}

			return Count;
		}

		private bool AddGroup(NetworkItem termItem, NetworkItems allItems, ref Point maxPos)
		{
			var group = new NetworkGroup();

			if (!group.Build(termItem, allItems, maxPos))
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

		public bool Build(NetworkItem termItem, NetworkItems allItems, Point startPos)
		{
			Clear();

			Point maxPos = startPos;

			if (!AddTask(termItem, allItems, ref maxPos))
				return false;

			MaxPos = maxPos;

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

		private bool AddTask(NetworkItem item, NetworkItems allItems, ref Point maxPos)
		{
			bool doBalance = (m_Items.Count == 0);

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

						AddTask(dependItem, allItems, ref maxPos); // RECURSIVE call

						item.Position.X = Math.Max(dependItem.Position.X + 1, item.Position.X);
					}
				}

				maxPos.X = Math.Max(maxPos.X, item.Position.X);
			}
			else // First item in group
			{
				item.Position.X = maxPos.X = 0;
			}

			if (doBalance)
				maxPos = BalanceVerticalPositions();

			return true;
		}

		private Point BalanceVerticalPositions()
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
			var maxPos = new Point(MaxPos.X, 0);

			foreach (var item in ItemValues)
				maxPos.Y = Math.Max(maxPos.Y, item.Position.Y);

			return maxPos;
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
			if ((pos.X < 0) || (pos.X >= m_MaxPos.X))
				return null;

			if ((pos.Y < 0) || (pos.Y >= m_MaxPos.Y))
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

		public NetworkItem GetNextItem(Point startPos, Direction dir, int increment = 1)
		{
			var nextPos = Increment(startPos, dir, increment);
			var nextItem = GetItemAt(nextPos);

			// TODO

			return null;
		}

		// ---------------------------------------
		// Internals

		private Point Increment(Point pos, Direction dir, int increment)
		{
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
				pos.Y = Math.Max(pos.Y + increment, m_MaxPos.Y);
				break;
			}

			return pos;
		}
	}

	// ------------------------------------------------------------

}

