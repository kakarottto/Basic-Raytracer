#pragma once
#include "vec.h"
#include "ray.h"

class material;

struct hit_record{
	double t;
	Vec3 p, normal;
	material* mat_ptr;
};

class hitable{
public:
	virtual bool hit(const Ray& r, double t_min, double t_max, hit_record& rec) const = 0;
};
