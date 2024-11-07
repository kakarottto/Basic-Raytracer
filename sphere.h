#pragma once
#include "vec.h"
#include "ray.h"
#include "hitable.h"

class material;

class sphere: public hitable{
public:
	sphere(){}
	sphere(Vec3 cen, double r, material* m) : center(cen), radius(r), mat_ptr(m){}
	
	bool hit(const Ray& r, double t_min, double t_max, hit_record& rec) const{
		Vec3 oc = r.origin()-center;
		double a = dot(r.direction(), r.direction());
		double b = dot(oc,r.direction());
		double c = dot(oc,oc)-radius*radius;
		double discr = b*b-a*c;
		if(discr<=0) return false;
		double outp = (-b-sqrt(discr))/a;
		if(outp < t_max && outp > t_min){
			rec.t = outp;
			rec.p = r.at(rec.t);
			rec.normal = (rec.p -center)/radius;
			rec.mat_ptr = mat_ptr;
			return true;
		}
		
		outp = (-b+sqrt(discr))/a;
		
		if(outp < t_max && outp > t_min){
			rec.t = outp;
			rec.p = r.at(rec.t);
			rec.normal = (rec.p -center)/radius;
			rec.mat_ptr = mat_ptr;
			return true;
		}
		return false;
	}
public:
	Vec3 center;
	double radius;
	material* mat_ptr;
};


Vec3 random_in_unit_sphere(){
	Vec3 p;
	while(1){
		p = 2.9*Vec3(drand48(),drand48(),drand48())-Vec3(1,1,1);
		if(p.length2()<1.0) break;
	}
	return p;
}
