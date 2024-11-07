#include <iostream>
#include <cstdlib>
#include <cmath>
#include "ray.h"
#include "material.h"
#include "camera.h"
#include "hitable.h"
#include "hitable_list.h"
#include "sphere.h"
#include "sdl_utils.h"

Vec3 random_color(){
	return Vec3(drand48()*drand48(),drand48()*drand48(),drand48()*drand48());
}

hitable *random_scene(){
	int n = 500;
	hitable** list = new hitable*[n+1];
	list[0] = new sphere(Vec3(0,-1000,0),1000, new lambertian(Vec3(0.5,0.5,0.5)));
	int i =1;
	for(int a=-11;a<11;a++){
		for(int b=-11;b<11;b++){
			double rmat = drand48();
			Vec3 rcenter(a+0.9*drand48(),0.2,b+0.9*drand48());
			if(rmat <=0.4) list[i++] = new sphere(rcenter,0.2, new lambertian(random_color()));
			else if(rmat < 0.7) list[i++] = new sphere(rcenter,0.2, new metal(random_color(),
																	(drand48()+1)/2));
			else list[i++] = new sphere(rcenter,0.2, new dielectric(1.5));
			
		}
	}
	list[i++] = new sphere(Vec3(0,1,0),1, new dielectric(1.5));
	list[i++] = new sphere(Vec3(-4,1,0),1,new lambertian(Vec3(0.4,0.2,0.1)));
	list[i++] = new sphere(Vec3(4,1,0),1, new metal(Vec3(0.7,0.6,0.5),0));
	return new hitable_list(list,i);
}



Vec3 color(const Ray& r, hitable *objects, int depth){
	hit_record rec;
	if(objects->hit(r,0.001, MAXFLOAT, rec)){
		//Vec3 target = rec.p + rec.normal + random_in_unit_sphere();
		//return 0.5*color(Ray(rec.p,target-rec.p),objects);
		Ray scattered;
		Vec3 attenuation;
		if(depth < 50 && rec.mat_ptr->scatter(r,rec,attenuation,scattered)){
			return attenuation*color(scattered,objects,depth+1);
		}
		return Vec3(0,0,0);
	}
	Vec3 unit_dir = r.direction().normalised();
	double t = 0.5*(unit_dir.y + 1.0);
	return (1.0-t)*Vec3(1.0,1.0,1.0)+t*Vec3(0.5,0.7,1.0);
	
}

int main(){
	
	int width = 800;
	int height= 400;
	int ns = 100;
	std::cout << "P3\n" <<width << " "<<height<<"\n255\n";
	sdl_utils::sdl("Raybobier",width, height);
	sdl_utils::loop();
	
	double R = cos(PI/4);
	hitable *list[4];
	//list[0] = new sphere(Vec3(-R,0,-1),R, new lambertian(Vec3(0,0,1)));
	//list[1] = new sphere(Vec3(R,0,-1),R, new lambertian(Vec3(1,0,0)));
	
	/*
	list[0] = new sphere(Vec3(0,0,-1),0.5, new lambertian(Vec3(0.1,0.2,0.5)));
	list[1] = new sphere(Vec3(0,-100.5,-1),100,new lambertian(Vec3(0.8,0.8,0.0)));
	list[2] = new sphere(Vec3(1,0,-1),0.5, new metal(Vec3(0.8,0.6,0.2),0.2));
	list[3] = new sphere(Vec3(-1,0,-1),0.5, new dielectric(1.5));
	list[4] = new sphere(Vec3(-1,0,-1),-0.45, new dielectric(1.5));
	*/
	hitable *objects = random_scene();
	
	Vec3 lookfrom(13,2,3);
	Vec3 lookat(0,0,0);
	double dist_to_focus = 10; //(lookfrom-lookat).length();
	double apeture = 0.1;
	Camera cam(lookfrom,lookat,Vec3(0,1,0),20,double(width)/double(height),apeture,dist_to_focus);
	
	for(int y=height-1;y>=0;y--){
		for(int x=0; x<width;x++){
			//std::cout <<"\r" <<height-y <<" / " <<height;
			Vec3 col(0,0,0);
			for(int s=0; s<ns;s++){
				double u = double(x+drand48())/double(width);
				double v = double(y+drand48())/double(height);
				Ray r = cam.get_ray(u,v);
				Vec3 p = r.at(2.0);
				col += color(r,objects,0);	
			}
			col /= double(ns);
			//col = Vec3(sqrt(col.r()),sqrt(col.g()),sqrt(col.b())); 
		 	int ir = int(255.99*col.x);
			int ig = int(255.99*col.y);
			int ib = int(255.99*col.z);
			std::cout << ir << " "<<ig << " " <<ib << "\n";
			sdl_utils::setDrawColor(sdl_utils::createColor(ir,ig,ib,255));
			sdl_utils::drawPoint(x,height-y);
		}
	}
	
	while(sdl_utils::running){
		sdl_utils::loop();
	}
	
	return 0;
}
