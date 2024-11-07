#pragma once
#include "vec.h"
#include "ray.h"

Vec3 random_in_unit_disk(){
	Vec3 p;
	while(1){
		p = 2.0*Vec3(drand48(),drand48(),0.0) - Vec3(1,1,0);
		if(dot(p,p)<1.0) break;
	}
	return p;
}


class Camera{
public:
	Camera(Vec3 lookfrom,Vec3 lookat, Vec3 up, double vfov, 
		double aspect,double aperture, double focus_dist){
		u,v,w;
		lens_radius = aperture/2;
		double t = vfov*M_PI/180;
		double half_height = tan(t/2);
		double half_width  = aspect*half_height;
		origin = lookfrom;
		w = unit_vector(lookfrom-lookat);
		u = unit_vector(cross(up,w));
		v = cross(w,u);
		
		//lower_left_corner = Vec3(-half_width,-half_height,-1.0);
		//horizontal = Vec3(half_width*2,0.0,0.0);
		//vertical = Vec3(0.0,half_height*2,0.0);
		lower_left_corner = origin-half_width*focus_dist*u-half_height*focus_dist*v-focus_dist*w;
		horizontal=2*half_width*u*focus_dist;
		vertical=2*half_height*v*focus_dist;
		
	}
	
	Ray get_ray(double s, double t){
		Vec3 rd = lens_radius*random_in_unit_disk();
		Vec3 offset = u*rd.x +v*rd.y;
		return Ray(origin+offset, lower_left_corner + s*horizontal+t*vertical-origin-offset);
	}
	
public:	
	Vec3 lower_left_corner;
	Vec3 horizontal;
	Vec3 vertical;
	Vec3 origin;
	double lens_radius;
	Vec3 u,v,w;
};
