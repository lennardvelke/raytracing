#ifndef CAMERA_H
#define CAMERA_H

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




#endif
