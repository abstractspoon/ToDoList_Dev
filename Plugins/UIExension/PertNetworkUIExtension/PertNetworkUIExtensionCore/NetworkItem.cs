using System;
using System.Collections.Generic;
using System.Drawing;

namespace PertNetworkUIExtension
{
	public class NetworkItem
	{
		public NetworkItem(string title, uint uniqueId)
		{
			Title = title;
			UniqueId = uniqueId;

			Position = NullPoint;
			DependencyUniqueIds = new List<uint>();
		}

		public uint UniqueId
		{
			get;
			private set;
		}

		public string Title;
		public Point Position;
		public bool HasPosition { get { return (Position != NullPoint); } }
		public List<uint> DependencyUniqueIds;

		public static Point NullPoint { get { return new Point(-1, -1); } }
	}
	// ------------------------------------------------------------

	public class NetworkData
	{
		private Dictionary<uint, NetworkItem> m_Items;
		private HashSet<NetworkGroup> m_Groups;

		public NetworkData()
		{
			m_Items = new Dictionary<uint, NetworkItem>();
			m_Groups = new HashSet<NetworkGroup>();
		}

		public IEnumerable<NetworkItem> Items
		{
			get { return m_Items.Values; }
		}

		public IEnumerable<NetworkGroup> Groups
		{
			get { return m_Groups; }
		}

		public void Clear()
		{
			m_Items.Clear();
			m_Groups.Clear();
		}

		void ResetPositions()
		{
			foreach (var item in Items)
				item.Position = NetworkItem.NullPoint;
		}

		public NetworkItem GetItem(uint uniqueId)
		{
			return Helper.GetItem(m_Items, uniqueId);
		}

		public bool DeleteItem(uint uniqueId)
		{
			if (!m_Items.ContainsKey(uniqueId))
				return false;

			m_Items.Remove(uniqueId);
			return true;
		}

		public bool AddItem(NetworkItem item)
		{
			if (m_Items.ContainsKey(item.UniqueId))
				return false;

			m_Items.Add(item.UniqueId, item);
			return true;
		}

		public void RebuildGroups(out Point maxPos)
		{
			ResetPositions();
			m_Groups.Clear();

			// Get the set of all tasks on whom other tasks are dependent
			HashSet<uint> dependentIDs = GetAllDependents();

			// Get the set of all tasks which have dependencies but 
			// on whom NO other tasks are dependent
			HashSet<uint> terminatorIDs = GetAllTerminators(dependentIDs);

			// Build the groups by working backwards from each end task
			maxPos = new Point(0, 0);

			foreach (var termId in terminatorIDs)
			{
				NetworkItem termItem = GetItem(termId);

				if (termItem != null)
				{
					Point groupMaxPos = new Point(0, maxPos.Y);

					if (m_Groups.Count > 0)
						groupMaxPos.Y++;

					var group = NewGroup(termItem, ref groupMaxPos);
					m_Groups.Add(group);

					maxPos.Y = groupMaxPos.Y;
					maxPos.X = Math.Max(maxPos.X, groupMaxPos.X);
				}
			}
		}

		HashSet<uint> GetAllDependents()
		{
			var dependentIDs = new HashSet<uint>();
			
			foreach (var item in Items)
			{
				foreach (var depend in item.DependencyUniqueIds)
					dependentIDs.Add(depend);
			}

			return dependentIDs;
		}

		HashSet<uint> GetAllTerminators(HashSet<uint> dependentIDs)
		{
			HashSet<uint> terminatorIds = new HashSet<uint>();

			foreach (var item in Items)
			{
				if ((item.DependencyUniqueIds.Count > 0) && !dependentIDs.Contains(item.UniqueId))
					terminatorIds.Add(item.UniqueId);
			}

			return terminatorIds;
		}

		NetworkGroup NewGroup(NetworkItem termItem, ref Point maxPos)
		{
			var group = new NetworkGroup();

			AddTaskToGroup(termItem, group, ref maxPos);

			group.BalanceVerticalPositions();

			return group;
		}
		
		void AddTaskToGroup(NetworkItem item, NetworkGroup group, ref Point maxPos)
		{
			if (!group.AddItem(item))
				return;

			// Don't modify the vertical position of an item already processed in another group
			if (!item.HasPosition)
				item.Position.Y = maxPos.Y;
			
			// This item's dependencies (can be zero)
			if (item.DependencyUniqueIds.Count > 0)
			{
				bool firstDepend = true;

				foreach (var dependId in item.DependencyUniqueIds)
				{
					NetworkItem dependItem = GetItem(dependId);

					if (dependItem != null)
					{
						// First dependency shares same VPos as prior item
						if (!firstDepend)
							maxPos.Y++;
						else
							firstDepend = false;

						AddTaskToGroup(dependItem, group, ref maxPos); // RECURSIVE call

						item.Position.X = Math.Max(dependItem.Position.X + 1, item.Position.X);
					}
				}

				maxPos.X = Math.Max(maxPos.X, item.Position.X);
			}
			else // First item in group
			{
				item.Position.X = maxPos.X = 0;
			}
		}
	}

	// ------------------------------------------------------------

	class Helper
	{
		public static List<NetworkItem> GetItemDependents(IEnumerable<NetworkItem> items, NetworkItem item)
		{
			var dependents = new List<NetworkItem>();

			foreach (var dependItem in items)
			{
				foreach (uint dependID in dependItem.DependencyUniqueIds)
				{
					if (dependID == item.UniqueId)
						dependents.Add(dependItem);
				}
			}

			return dependents;
		}

		public static List<NetworkItem> GetItemDependencies(IDictionary<uint, NetworkItem> items, NetworkItem item)
		{
			var dependencies = new List<NetworkItem>();

			foreach (uint dependID in item.DependencyUniqueIds)
				dependencies.Add(GetItem(items, dependID));

			return dependencies;
		}
		
		public static NetworkItem GetItem(IDictionary<uint, NetworkItem> items, uint uniqueId)
		{
			NetworkItem item = null;
			items.TryGetValue(uniqueId, out item);

			return item;
		}
	}

	// ------------------------------------------------------------

	public class NetworkGroup
	{
		private Dictionary<uint, NetworkItem> m_Items;

		public NetworkGroup()
		{
			m_Items = new Dictionary<uint, NetworkItem>();
		}

		public IEnumerable<NetworkItem> Items
		{
			get { return m_Items.Values; }
		}

		public bool AddItem(NetworkItem item)
		{
			// Must be unique
			if (m_Items.ContainsKey(item.UniqueId))
				return false;

			m_Items.Add(item.UniqueId, item);
			return true;
		}

		Dictionary<int, List<NetworkItem>> BuildHorizontalSubGroups()
		{
			var subGroups = new Dictionary<int, List<NetworkItem>>();

			foreach (var item in Items)
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

		void GetVerticalRange(IEnumerable<NetworkItem> items, out int minY, out int maxY)
		{
			minY = -1;
			maxY = -1;

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
		}

		public void BalanceVerticalPositions()
		{
			// Try moving an item towards the centre of its dependencies
			// and stop when we hit a position already taken.
			// Don't allow items to move outside the maximum range
			int groupMinY, groupMaxY;
			GetVerticalRange(Items, out groupMinY, out groupMaxY);

			var subGroups = BuildHorizontalSubGroups();
			List<NetworkItem> subGroup = null;

			for (int iHPos = 1; iHPos < subGroups.Count; iHPos++)
			{
				if (subGroups.TryGetValue(iHPos, out subGroup))
				{
					foreach (var item in subGroup)
					{
						var dependencies = GetItemDependencies(item);

						int minY, maxY;
						GetVerticalRange(dependencies, out minY, out maxY);

						int iVPos = ((maxY + minY) / 2);

						for (int vPos = item.Position.Y; vPos <= iVPos; vPos++)
						{
							if (IsPositionTaken(subGroup, iHPos, vPos))
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
				var dependents = GetItemDependents(item);

				int minY, maxY;
				GetVerticalRange(dependents, out minY, out maxY);

				int newPos = ((maxY + minY) / 2);

				if (!IsPositionTaken(subGroup, 0, newPos))
					item.Position.Y = newPos;
			}
		}

		NetworkItem GetItem(uint uniqueId)
		{
			return Helper.GetItem(m_Items, uniqueId);
		}

		List<NetworkItem> GetItemDependents(NetworkItem item)
		{
			return Helper.GetItemDependents(m_Items.Values, item);
		}

		public List<NetworkItem> GetItemDependencies(NetworkItem item)
		{
			return Helper.GetItemDependencies(m_Items, item);
			var dependencies = new List<NetworkItem>();

			foreach (uint dependID in item.DependencyUniqueIds)
				dependencies.Add(GetItem(dependID));

			return dependencies;
		}

		bool IsPositionTaken(IEnumerable<NetworkItem> items, int x, int y)
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

}

