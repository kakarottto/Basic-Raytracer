#pragma once
#include "hitable.h"

class hitable_list: public hitable{
public:
	hitable_list(){}
	//leveled up to 2 stars!!!
	hitable_list(hitable **l, int n){
		list=l;size=n;
	}
	
	bool hit(const Ray& r, double t_min, double t_max, hit_record& rec) const{
		hit_record outp;
		bool hit_any=false;
		double closest = t_max;
		for(int i=0; i <size;i++){
			if(list[i]->hit(r,t_min,closest,outp)){
				rec = outp;
				hit_any = true;
				closest = rec.t;
			}
		}
		return hit_any;
	}
	
	
	
	hitable **list;
	int size;
};
