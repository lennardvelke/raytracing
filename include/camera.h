#ifndef CAMERA_H
#define CAMERA_H

#include "ray.h"
#include "vec3.h"


typedef struct  {
  double fov;
  Vec3 position;
  float ratio;

  int width;
  int height;
  Vec3 up;
  Vec3 right;
  Vec3 forward;
  int number_of_samples_per_ray;
} Camera; 


void createRayForPixel(Camera* camera, int pixelX, int pixelY,double scale, Ray* returnRay);


#endif
