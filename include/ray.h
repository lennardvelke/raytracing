#pragma once 

#include "circle.h"
#include "vec3.h"
#include <cmath>
#include <cstdio>


typedef struct {
  Vec3 position;
  Vec3 direction;
} Ray;

bool fire_ray(Ray ray, Circle circle){
  //ax^2 + bx + c

  float t_close = dotProduct(ray.direction, circle.position - ray.position);
printf("%f\n", t_close);
  
  Vec3 x_close = ray.position + t_close*ray.direction;
  float d = distanceVec3(x_close - circle.position);
  float a = std::sqrt(std::pow(circle.radius,2) - std::pow(d, 2));
  
printf("%f\n", d);
  

  if (d<=circle.radius) {
    Vec3 hit_point = ray.position + (t_close - a)*ray.direction;
    return true;
  }

  return false; 
}

