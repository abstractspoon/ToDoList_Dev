#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////

using namespace System;

////////////////////////////////////////////////////////////////////////////////////////////////

namespace Abstractspoon
{
	namespace Tdl
	{
		namespace PluginHelpers
		{
			public ref class RectUtil
			{
			public:
				static int CentreX(Drawing::Rectangle^ rect);
				static int CentreY(Drawing::Rectangle^ rect);

				static Drawing::Point Centre(Drawing::Rectangle^ rect);

				static Drawing::Point TopLeft(Drawing::Rectangle^ rect);
				static Drawing::Point TopCentre(Drawing::Rectangle^ rect);
				static Drawing::Point TopRight(Drawing::Rectangle^ rect);

				static Drawing::Point MiddleLeft(Drawing::Rectangle^ rect);
				static Drawing::Point MiddleRight(Drawing::Rectangle^ rect);

				static Drawing::Point BottomLeft(Drawing::Rectangle^ rect);
				static Drawing::Point BottomCentre(Drawing::Rectangle^ rect);
				static Drawing::Point BottomRight(Drawing::Rectangle^ rect);
			};

			public ref class PointUtil
			{
			public:
				static int Distance(Drawing::Point^ pt1, Drawing::Point^ pt2);
			
			};

		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////

