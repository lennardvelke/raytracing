#ifndef RAYTREE_H
#define RAYTREE_H


#include "ray.h"


typedef struct {
  Ray* ray_parent;
  Ray* child_ray;
} Ray_Tree;


#endif






