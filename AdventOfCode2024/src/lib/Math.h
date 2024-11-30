#pragma once

#include "Array2D.h"
#include "Array3D.h"

struct Math
{
	Math() = delete;
	static int Dot(const Index2D& i1, const Index2D& i2)
	{
		return i1.x * i2.x + i1.y * i2.y;
	}

	static int Dot(const Index3D& i1, const Index3D& i2)
	{
		return i1.x * i2.x + i1.y * i2.y + i1.z * i2.z;
	}
};
