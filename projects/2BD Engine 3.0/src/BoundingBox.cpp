#include "BoundingBox.h"

BoundingBox makeBBox(int top, int bottom, int left, int right)
{
	BoundingBox boundingB;
	boundingB.top = top;
	boundingB.bottom = bottom;
	boundingB.left = left;
	boundingB.right = right;


	//return BoundingBox();
	return boundingB;
}
