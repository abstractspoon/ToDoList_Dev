// PluginHelpers.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "GeometryUtil.h"

////////////////////////////////////////////////////////////////////////////////////////////////

using namespace System::Drawing;

using namespace Abstractspoon::Tdl::PluginHelpers;

////////////////////////////////////////////////////////////////////////////////////////////////

int RectUtil::CentreX(Drawing::Rectangle^ rect)
{
	return ((rect->Left + rect->Right) / 2);
}

int RectUtil::CentreY(Drawing::Rectangle^ rect)
{
	return ((rect->Top + rect->Bottom) / 2);
}

Drawing::Point RectUtil::Centre(Drawing::Rectangle^ rect)
{
	return Point(CentreX(rect), CentreY(rect));
}

Drawing::Point RectUtil::TopLeft(Drawing::Rectangle^ rect)
{
	return Point(rect->Left, rect->Top);
}

Drawing::Point RectUtil::TopCentre(Drawing::Rectangle^ rect)
{
	return Point(CentreX(rect), rect->Top);
}

Drawing::Point RectUtil::TopRight(Drawing::Rectangle^ rect)
{
	return Point(rect->Right, rect->Top);
}

Drawing::Point RectUtil::MiddleLeft(Drawing::Rectangle^ rect)
{
	return Point(rect->Left, CentreY(rect));
}

Drawing::Point RectUtil::MiddleRight(Drawing::Rectangle^ rect)
{
	return Point(rect->Right, CentreY(rect));
}

Drawing::Point RectUtil::BottomLeft(Drawing::Rectangle^ rect)
{
	return Point(rect->Left, rect->Bottom);
}

Drawing::Point RectUtil::BottomCentre(Drawing::Rectangle^ rect)
{
	return Point(CentreX(rect), rect->Bottom);
}

Drawing::Point RectUtil::BottomRight(Drawing::Rectangle^ rect)
{
	return Point(rect->Right, rect->Bottom);
}

////////////////////////////////////////////////////////////////////////////////////////////////

int PointUtil::Distance(Drawing::Point^ pt1, Drawing::Point^ pt2)
{
	return (int)sqrt(((pt1->X - pt2->X) * (pt1->X - pt2->X)) + 
					 ((pt1->Y - pt2->Y) * (pt1->Y - pt2->Y)));
}
