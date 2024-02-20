namespace RadialTree
{
    public class Point
    {
		public Point(float x, float y, float radius = 1)
		{
			X = x;
			Y = y;
			Radius = radius;
		}

        public float X { get; set; }
        public float Y { get; set; }
        public float Radius { get; set; }
	}
}
