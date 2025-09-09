

#ifndef RAY_H 

#define RAY_H


#include "light.h"
#include "circle.h"
#include "vec3.h"
#include <vector>
#include <cmath>
#include <cstdio>
#include "option.h"



typedef struct {
  Vec3 position;
  Vec3 direction;
  Vec3 color;
} Ray;

typedef struct {
  Vec3 hit_point = {0};
  Circle* hit_objects = nullptr;

} Hit_Information;

void blendColors(Vec3* color_a, Vec3* color_b, double ration, Vec3* return_color);

bool cast_ray(Ray* ray, std::vector<Circle* >* objects, Circle* to_be_ignored, Option* option, int depth, Material* material);

bool trace(Ray* ray, std::vector<Circle* >* objects, Circle* to_be_ignored, Hit_Information* hit_point);

bool collision(Ray *ray, Circle * circle, Hit_Information * hit_point);

#endif // !DEBUG

