#ifndef CIRCLE_H
#define CIRCLE_H


#include "vec3.h"

enum Material_Type {
  DEFUSE,
  REFLECTIVE
};

typedef struct {
  Vec3 color = {0};
  Vec3 emission = {0};
  Material_Type type = DEFUSE;
} Material;


typedef struct {
  Vec3 position;
  double radius;
  Material material;
} Circle;

#endif // !CIRCLE_H

