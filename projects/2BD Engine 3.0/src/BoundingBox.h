#pragma once

#ifndef _Simple_BoundingBox_
#define _Simple_BoundingBox_
#include <iostream>

typedef struct {

	int top, bottom, left, right;

}BoundingBox;

BoundingBox makeBBox(int top, int bottom, int left, int right);

#endif 