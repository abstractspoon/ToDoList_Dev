using System;
using System.Linq;
using System.Collections.Generic;

namespace RadialTree
{
    public class RadialTree<T>
    {
		List<float> m_Radii = null;
		List<float> m_Circumferences = null;
		List<float> m_Increments = null;
		List<int> m_Counts = null;

		float m_RadialIncrement = 50f, m_InitialRadius = 50f;
		TreeNode<T> m_RootNode = null;

		public RadialTree(TreeNode<T> rootNode, float initialRadius = 50f, float radialIncrement = 50f)
		{
			if (rootNode.IsRoot)
			{
				m_RootNode = rootNode;
				SetRadius(initialRadius, radialIncrement);
			}
		}

		public float RadialIncrement { get { return m_RadialIncrement; } }

		public void CalculatePositions()
		{
			CalculatePositions(m_RootNode, 0, (float)(2 * Math.PI), m_InitialRadius, m_RadialIncrement);
		}

		public void CalculatePositions(float initialRadius, float radialIncrement)
		{
			SetRadius(initialRadius, radialIncrement);
			CalculatePositions();
		}

		protected void SetRadius(float initialRadius, float radialIncrement)
		{
			if (m_RootNode == null)
				return;

			m_InitialRadius = initialRadius;

			if (radialIncrement < 0)
			{
				float spacing = -radialIncrement;

				// Count the nodes at every level
				m_Counts = new List<int>();
				int numLevels = CountNodes(m_RootNode, m_Counts);

				// Convert the counts into equivalent minimum circumferences
				m_Circumferences = new List<float>() { 0f };

				for (int level = 1; level < numLevels; level++)
				{
					m_Circumferences.Add(m_Counts[level] * spacing);
				}

				// Convert the circumferences into equivalent normalised minimum radii
				m_Radii = new List<float>() { 0f };

				for (int level = 1; level < numLevels; level++)
				{
					m_Radii.Add(m_Circumferences[level] / (float)(2 * Math.PI));
				}

				// Convert the radii into equivalent radial increments
				m_Increments = new List<float>() { 0f };

				for (int level = 2; level < numLevels; level++)
				{
					m_Increments.Add((m_Radii[level] - m_InitialRadius) / (level - 1));
				}

				// Get the maximum
				m_RadialIncrement = Math.Max(spacing, m_Increments.Max());
			}
			else
			{
				m_RadialIncrement = radialIncrement;
			}
		}

		protected static void CalculatePositions(TreeNode<T> node, float startRadians, float endRadians, float circleRadius, float radialIncrement)
        {
            float theta = startRadians;
			int leavesNumber = BreadthFirstSearch(node);

            foreach (var child in node.Children)
            {
                float lambda = BreadthFirstSearch(child);
                float mi = theta + ((lambda / leavesNumber) * (endRadians - startRadians));

                float x = (float)(circleRadius * Math.Cos((theta + mi) / 2.0));
                float y = (float)(circleRadius * Math.Sin((theta + mi) / 2.0));

                child.Point.X = x;
                child.Point.Y = y;

                if (child.Children.Count > 0)
                {
					// RECURSIVE CALL
                    CalculatePositions(child, theta, mi, circleRadius + radialIncrement, radialIncrement);
                }

                theta = mi;
            }
        }

		protected static int CountNodes(TreeNode<T> node, IList<int> counts)
		{
			if (node.Level >= counts.Count)
				counts.Add(0);

			counts[node.Level]++;

            foreach (var child in node.Children)
            {
				CountNodes(child, counts);
            }

			return counts.Count;
		}

/*
        /// <summary>
        /// Calculate the positions of each node in a radial layout.
        /// </summary>
        /// <param name="node">The node you want to create the layout from.</param>
        /// <param name="alfa">The start angle for the radial layout (Radians).</param>
        /// <param name="beta">The amount of the circle to use for the radial layout (Radians).</param>
        /// <param name="circleRadius">Distance between the root node and first children.</param>
        /// <param name="deltaDistance">Distance between other child nodes.</param>
        /// <param name="outputGraph">Calculated positions for the nodes.</param>

		public static void RadialPositions<T>(TreeNode<T> node, float startAngleRad, float endAngleRad, float circleRadius, float deltaDistance, List<RadialPoint<T>> outputGraph)
		{
			//check if node is root of the tree
			if (node.IsRoot)
			{
				node.Point.X = 0;
				node.Point.Y = 0;
				outputGraph.Add(new RadialPoint<T>
				{
					Node = node,
					Point = new Point(0f, 0f),
					ParentPoint = null
				});

			}
			//Depth of node starting from 0
			int depthOfVertex = node.Level;
			float theta = startAngleRad;
			float radius = circleRadius + (deltaDistance * depthOfVertex);

			int leavesNumber = BreadthFirstSearch(node);
			float minRadius = (float)((leavesNumber * deltaDistance) / (2 * Math.PI));

			foreach (var child in node.Children)
			{
				float lambda = BreadthFirstSearch(child);
				float mi = theta + (lambda / leavesNumber * (endAngleRad - startAngleRad));

				float x = (float)(radius * Math.Cos((theta + mi) / 2.0));
				float y = (float)(radius * Math.Sin((theta + mi) / 2.0));

				child.Point.X = x;
				child.Point.Y = y;

				outputGraph.Add(new RadialPoint<T>
				{
					Node = child,
					Point = new Point(x, y, radius),
					ParentPoint = node.Point
				});

				if (child.Children.Count > 0)
				{
					child.Point.Y = y;
					child.Point.X = x;
					RadialPositions(child, theta, mi, circleRadius, deltaDistance, outputGraph);
				}
				theta = mi;
			}
		}
*/

		private static int BreadthFirstSearch(TreeNode<T> root)
        {
            var visited = new List<TreeNode<T>>();
            var queue = new Queue<TreeNode<T>>();
            int leaves = 0;

            visited.Add(root);
            queue.Enqueue(root);

            while (queue.Count != 0)
            {
                var current = queue.Dequeue();
                if (current.Children.Count == 0)
                    leaves++;
                foreach (var node in current.Children)
                {
                    if (!visited.Contains(node))
                    {
                        visited.Add(node);
                        queue.Enqueue(node);
                    }
                }
            }

            return leaves;
        }
    }
}
