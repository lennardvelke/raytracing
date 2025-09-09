#include "ray.h"
#include "circle.h"
#include "light.h"
#include "renderer.h"
#include "vec3.h"
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <functional>
#include <iomanip>
#include <iostream>
#include <limits>
#include <ostream>

bool lessEqualZero(double a){
  return a <= 0 || std::abs(a) <= std::numeric_limits<double>::epsilon();
}
bool lessZero(double a){
  return a < 0 || std::abs(a) < std::numeric_limits<double>::epsilon();
}

bool equalZero(double a){
  return -std::numeric_limits<double>::epsilon() <= a && a <= std::numeric_limits<double>::epsilon();
}
 #include <vector>

void printVec3(char * text, Vec3 vector);

void blendColors(Vec3* color_a, Vec3* color_b, double ration, Vec3* return_color){
  *return_color = ration * 2.0f/(lengthVec2(*color_a)+1) * *color_a + (1.0f-ration) *2.0f/(lengthVec2(*color_b)+1) * *color_b; 
  // *return_color = mulElements(*color_a, *color_b);

}

void calculateReflextionRay(Ray* ray, Hit_Information* info, Ray* return_ray){
  Vec3 normal = normalize(info->hit_point - info->hit_objects->position);
  Vec3 new_direction = normalize(ray->direction - 2 * (dotProduct(ray->direction, normal)) * normal);

  return_ray->direction = new_direction;
  return_ray->position = info->hit_point;

}

void calucalteNextRandomRayInHemisphere(Ray* ray, Hit_Information* info, Ray* return_ray){
  double u = ((double) rand() / (RAND_MAX));
  double v = ((double) rand() / (RAND_MAX));
  

  double theta = std::acos(u);      // Winkel von 0 bis pi/2
  double phi   = 2.0 * M_PI * v;  

  Vec3 normal = normalize(info->hit_point - info->hit_objects->position);

  Vec3 tangent = {0};
  if (std::fabs(normal.x) < 0.9){
    tangent = crossProduct(normal, Vec3{1,0,0});
  }else{
    tangent = crossProduct(normal, Vec3{0,1,0});

  } 
  tangent = normalize(tangent);
  Vec3 bitangent = crossProduct(normal, tangent);

Vec3 result = {
        tangent.x * std::sin(theta) * std::cos(phi) +
        bitangent.x * std::sin(theta) * std::sin(phi) +
        normal.x * std::cos(theta),

        tangent.y * std::sin(theta) * std::cos(phi) +
        bitangent.y * std::sin(theta) * std::sin(phi) +
        normal.y * std::cos(theta),

        tangent.z * std::sin(theta) * std::cos(phi) +
        bitangent.z * std::sin(theta) * std::sin(phi) +
        normal.z * std::cos(theta)
    };
  
  return_ray->position = info->hit_point;
  return_ray->direction = normalize(result);
}


bool cast_ray(Ray* ray, std::vector<Circle* >* objects, Circle* to_be_ignored, Option* option, int depth, Material* material){
  if(option->depth < depth){
    material->color = {0} ;
    return true;
  }

  Hit_Information information;

  if (!trace(ray, objects, to_be_ignored, &information)) {
    material->color = {0};
    return true;
  }

  material->color = information.hit_objects->material.color + material->color; 

  if (lengthVec2(information.hit_objects->material.emission) > 0) {
    material->emission = information.hit_objects->material.emission;
    return true;
  }


  Ray nextRay;
  calucalteNextRandomRayInHemisphere(ray, &information, &nextRay);

  cast_ray(&nextRay, objects, information.hit_objects, option, depth + 1, material);
 
  return true;
} 


bool trace(Ray* ray, std::vector<Circle* >* objects, Circle* to_be_ignored, Hit_Information* hit_point){

  Hit_Information closest_hit_information;
  Hit_Information current_hit_information;

  for (int current_object_id = 0;  current_object_id < objects->size(); current_object_id++) {
    bool hit = collision(ray, objects->at(current_object_id), &current_hit_information);
    if (
      to_be_ignored != objects->at(current_object_id) 
      && hit==true 
      && (lengthVec2(current_hit_information.hit_point-ray->position) < lengthVec2(closest_hit_information.hit_point - ray->position) 
          || closest_hit_information.hit_objects == nullptr)) {
      
      closest_hit_information.hit_point = current_hit_information.hit_point;
      closest_hit_information.hit_objects = current_hit_information.hit_objects;
    }
  }

  hit_point->hit_point = closest_hit_information.hit_point;
  hit_point->hit_objects = closest_hit_information.hit_objects;

  return closest_hit_information.hit_objects != nullptr;
}



bool collision(Ray *ray, Circle * circle, Hit_Information * hit_point){

  double a = std::pow(lengthVec2(ray->direction), 2); 
  double b = 2*((ray->position.x-circle->position.x) * ray->direction.x + (ray->position.y - circle->position.y)* ray->direction.y + (ray->position.z - circle->position.z) * ray->direction.z);
  double c = (std::pow(circle->position.x, 2) - 2 * circle->position.x * ray->position.x + std::pow(ray->position.x, 2) 
    +(std::pow(circle->position.y, 2) - 2 * circle->position.y * ray->position.y + std::pow(ray->position.y, 2))
    +(std::pow(circle->position.z, 2) - 2 * circle->position.z * ray->position.z + std::pow(ray->position.z, 2))  
  ) - circle->radius*circle->radius;

  double delta = pow(b,2) - 4*a*c;
  
  if (lessZero(delta)) {
    return false;
  }

  if (equalZero(delta)) {
      
    double n = -b/(2*a); 

    if ( lessEqualZero(n)) {
      return false;
    }

    hit_point->hit_objects = circle;
    hit_point->hit_point = ray->position + n * ray->direction;
    return true;
  }

  double n_1 = (-b - sqrt(delta))/(2*a);
  double n_2 = (-b + sqrt(delta))/(2*a);
   
  double minimum_greater_than_zero = std::min(n_1, n_2);
  if (lessEqualZero(std::max(n_1, n_2))) {
    return false; 
  }

  if (lessEqualZero(minimum_greater_than_zero)) {
    minimum_greater_than_zero = std::max(n_1, n_2);
  }

  
  hit_point->hit_point = ray->position + minimum_greater_than_zero * ray->direction;
  hit_point->hit_objects = circle;
  
  return true;
}



