#include "vec3.h"
#include <algorithm>
#include <cmath>
#include <cstdio>


Vec3 operator +(Vec3 a, Vec3 b){
  Vec3 changedVec3 = {0};

  changedVec3.x = a.x + b.x;
  changedVec3.y = a.y + b.y;
  changedVec3.z = a.z + b.z;
  
  return changedVec3;
}

Vec3 operator *(double scalar, Vec3 a){
  Vec3 changedVec3 = {0};

  changedVec3.x = a.x*scalar;
  changedVec3.y = a.y*scalar;
  changedVec3.z = a.z*scalar;
  
  return changedVec3;
}

Vec3 operator *(Vec3 a, double scalar){
  Vec3 changedVec3 = {0};

  changedVec3.x = a.x*scalar;
  changedVec3.y = a.y*scalar;
  changedVec3.z = a.z*scalar;
  
  return changedVec3;
}


double operator *(Vec3 a, Vec3 b){
  Vec3 changedVec3 = {0};

  changedVec3.x = a.x*b.x;
  changedVec3.y = a.y*b.y;
  changedVec3.z = a.z*b.z;
  
  return changedVec3.x+changedVec3.y + changedVec3.z;
}
Vec3 mulElements(Vec3 a, Vec3 b){
  Vec3 changedVec3 = {0};

  changedVec3.x = a.x*b.x;
  changedVec3.y = a.y*b.y;
  changedVec3.z = a.z*b.z;
  
  return changedVec3;
}

Vec3 operator /(Vec3 a, Vec3 b){
  Vec3 changedVec3 = {0};

  changedVec3.x = a.x/b.x;
  changedVec3.y = a.y/b.y;
  changedVec3.z = a.z/b.z;
  
  return changedVec3;
}


Vec3 operator -(Vec3 a, Vec3 b){
  Vec3 changedVec3 = {0};

  changedVec3.x = a.x - b.x;
  changedVec3.y = a.y - b.y;
  changedVec3.z = a.z - b.z;
  
  return changedVec3;
}

double lengthVec2(Vec3 a){

  double distance = std::sqrt( std::pow(a.x, 2) +  std::pow(a.y, 2) + std::pow(a.z, 2));  
  return distance;
}

double dotProduct(Vec3 a, Vec3 b){
  double upper_part = a*b;
  double lower_part = lengthVec2(a) * lengthVec2(b);

  return a.x*b.x + a.y * b.y + a.z * b.z;
}
float sum(Vec3 a){

  return a.x + a.y + a.z;
}


Vec3 crossProduct(Vec3 a, Vec3 b){
  Vec3 result = {0};

  result.x = a.y*b.z - a.z*b.y;
  result.y = a.z*b.x - a.x*b.z;
  result.z = a.x*b.y - a.y*b.x;

  return result;
}

Vec3 normalize(Vec3 a){
  float distance = sqrt(a.x * a.x + a.y * a.y + a.z*a.z);
  
  a.x = a.x / distance;
  a.y = a.y / distance;
  a.z = a.z / distance;

  return a;
}
void printVec3(char* text, Vec3 vector){
  printf("%s: (%lf, %lf, %lf). \n",text, vector.x, vector.y, vector.z);
}

Vec3 maxElements(Vec3 a, Vec3 b){
  Vec3 max;

  max.x = std::max(a.x, b.x);
  max.y = std::max(a.y, b.y);
  max.z = std::max(a.z, b.z);

  return max;
}

Vec3 rotateAroundAxis(Vec3 axis, Vec3 vector, float rotation){
  
  Vec3 a = normalize(axis);
  float cosT = std::cos(rotation);
  float sinT = std::sin(rotation); 

  Vec3 term1 = { vector.x*cosT, vector.y*cosT, vector.z*cosT };
    Vec3 axp = crossProduct(a, vector);
    Vec3 term2 = { axp.x*sinT, axp.y*sinT, axp.z*sinT };
    float adotp = dotProduct(a, vector);
    Vec3 term3 = { a.x*adotp*(1-cosT), a.y*adotp*(1-cosT), a.z *adotp * (1-cosT)};

  

  return term1 + term2 + term3;  
}

