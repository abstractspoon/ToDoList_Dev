using System;
using System.Collections.Generic;
using System.Drawing;

namespace PertNetworkUIExtension
{
	public class PertNetworkItem
	{
		public PertNetworkItem(string title, uint uniqueId)
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

	public class PertNetworkData
	{
		private Dictionary<uint, PertNetworkItem> m_Items;
		private HashSet<PertNetworkGroup> m_Groups;

		public PertNetworkData()
		{
			m_Items = new Dictionary<uint, PertNetworkItem>();
			m_Groups = new HashSet<PertNetworkGroup>();
		}

		public IEnumerable<PertNetworkItem> Items
		{
			get { return m_Items.Values; }
		}

		public IEnumerable<PertNetworkGroup> Groups
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
				item.Position = PertNetworkItem.NullPoint;
		}

		public PertNetworkItem GetItem(uint uniqueId)
		{
			PertNetworkItem item = null;
			m_Items.TryGetValue(uniqueId, out item);

			return item;
		}

		public bool DeleteItem(uint uniqueId)
		{
			if (!m_Items.ContainsKey(uniqueId))
				return false;

			m_Items.Remove(uniqueId);
			return true;
		}

		public bool AddItem(PertNetworkItem item)
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
				PertNetworkItem termItem = GetItem(termId);

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

		PertNetworkGroup NewGroup(PertNetworkItem termItem, ref Point maxPos)
		{
			var group = new PertNetworkGroup();

			AddTaskToGroup(termItem, group, ref maxPos);

			group.BalanceVerticalPositions();

			return group;
		}
		
		void AddTaskToGroup(PertNetworkItem item, PertNetworkGroup group, ref Point maxPos)
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
					PertNetworkItem dependItem = GetItem(dependId);

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

	public class PertNetworkGroup
	{
		private Dictionary<uint, PertNetworkItem> m_Items;

		public PertNetworkGroup()
		{
			m_Items = new Dictionary<uint, PertNetworkItem>();
		}

		public IEnumerable<PertNetworkItem> Items
		{
			get { return m_Items.Values; }
		}

		public bool AddItem(PertNetworkItem item)
		{
			// Must be unique
			if (m_Items.ContainsKey(item.UniqueId))
				return false;

			m_Items.Add(item.UniqueId, item);
			return true;
		}

		Dictionary<int, List<PertNetworkItem>> BuildHorizontalSubGroups()
		{
			var subGroups = new Dictionary<int, List<PertNetworkItem>>();

			foreach (var item in Items)
			{
				List<PertNetworkItem> subGroup = null;

				if (!subGroups.TryGetValue(item.Position.X, out subGroup))
				{
					subGroup = new List<PertNetworkItem>();
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

		void GetVerticalRange(IEnumerable<PertNetworkItem> items, out int minY, out int maxY)
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
			List<PertNetworkItem> subGroup = null;

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
				var dependents = GetItemDependents(item.UniqueId);

				int minY, maxY;
				GetVerticalRange(dependents, out minY, out maxY);

				int newPos = ((maxY + minY) / 2);

				if (!IsPositionTaken(subGroup, 0, newPos))
					item.Position.Y = newPos;
			}
		}

		PertNetworkItem GetItem(uint uniqueId)
		{
			PertNetworkItem item = null;
			m_Items.TryGetValue(uniqueId, out item);

			return item;
		}

		List<PertNetworkItem> GetItemDependents(uint uniqueId)
		{
			var dependents = new List<PertNetworkItem>();

			foreach (var item in Items)
			{
				foreach (uint dependID in item.DependencyUniqueIds)
				{
					if (dependID == uniqueId)
						dependents.Add(item);
				}
			}

			return dependents;
		}

		public List<PertNetworkItem> GetItemDependencies(PertNetworkItem item)
		{
			var dependencies = new List<PertNetworkItem>();

			foreach (uint dependID in item.DependencyUniqueIds)
				dependencies.Add(GetItem(dependID));

			return dependencies;
		}

		bool IsPositionTaken(IEnumerable<PertNetworkItem> items, int x, int y)
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

