#pragma once
#include "sphere.h"
#include "hitable.h"
#include "ray.h"
#include "vec.h"

class material{
public:
	//imagine senbon sakura kageyoshi but with different reflections
	virtual bool scatter(const Ray& r_in, const hit_record& rec, 
						 Vec3& attenuation, Ray& scattered) const = 0;
};

class lambertian : public material{
public:
	lambertian(const Vec3& a) : albedo(a){}
	virtual bool scatter(const Ray& r_in, const hit_record& rec, 
						 Vec3& attenuation, Ray& scattered) const {
		Vec3 target = rec.p + rec.normal + random_in_unit_sphere();
		scattered = Ray(rec.p, target-rec.p);
		attenuation = albedo;
		return true;
	}
	Vec3 albedo;
};

class metal : public material{
public:
	metal(const Vec3& a, int f) : albedo(a){if(f<1) fuzz = f; else fuzz =1;}
	virtual bool scatter(const Ray& r_in, const hit_record& rec, 
						 Vec3& attenuation, Ray& scattered) const {
		
		Vec3 reflected = reflect(r_in.direction().normalised(),rec.normal);
		scattered = Ray(rec.p, reflected+fuzz*random_in_unit_sphere());
		attenuation = albedo;
		return (dot(scattered.direction(),rec.normal)>0);
	}
	Vec3 albedo;
	int fuzz;
};


class dielectric : public material{
public:
	dielectric(double ri) : frac_i(ri) {}
	virtual bool scatter(const Ray& r_in, const hit_record& rec, 
						 Vec3& attenuation, Ray& scattered) const {
		double ni_over_nt;
		double reflect_prob;
		double cosine;
		Vec3 outward_normal;
		Vec3 refracted;
		
		Vec3 reflected = reflect(r_in.direction(),rec.normal);
		attenuation = Vec3(1.0,1.0,1.0);
		if(dot(r_in.direction(), rec.normal)>0){
			outward_normal = -rec.normal;
			ni_over_nt = frac_i;
			//cosine = dot(r_in.direction(),rec)/r_in.direction().length();
			cosine = sqrt(1-frac_i*frac_i*(1-cosine*cosine));
		}
		else{
			outward_normal = rec.normal;
			ni_over_nt = 1.0/frac_i;
			cosine = -dot(r_in.direction(),rec.normal)/r_in.direction().length();
		}
		if(refract(r_in.direction(),outward_normal, ni_over_nt, refracted)){
			reflect_prob = schlick(cosine,frac_i);
		}
		else{
			reflect_prob = 1.0;
		}
		if(drand48()<reflect_prob){
			scattered = Ray(rec.p,reflected);
		}
		else{
			scattered = Ray(rec.p,refracted);
		}
		return true;
	}
	double frac_i; //fracted index
};
