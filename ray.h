#pragma once
#include "vec.h"

//f(t) = A +t*B
struct Ray{
	Ray(){}
	~Ray(){}
	Ray(const Vec3& a, const Vec3 b){A = a; B = b;}

	Vec3 origin() const {return A;}
	Vec3 direction() const {return B;}

	Vec3 point_at_parameter(double t) const {return (A + t*B);}
	Vec3 at(double t) const {return (A +t*B);}
	
	Vec3 A, B;
};
