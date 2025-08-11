#pragma once

#include <cmath>
typedef struct {
 float x,y,z;
} Vec3;


Vec3 operator +(Vec3 a, Vec3 b){
  Vec3 changedVec3 = {0};

  changedVec3.x = a.x + b.x;
  changedVec3.y = a.y + b.y;
  changedVec3.z = a.z + b.z;
  
  return changedVec3;
}

Vec3 operator *(float scalar, Vec3 a){
  Vec3 changedVec3 = {0};

  changedVec3.x = a.x*scalar;
  changedVec3.y = a.y*scalar;
  changedVec3.z = a.z*scalar;
  
  return changedVec3;
}

Vec3 operator *(Vec3 a, float scalar){
  Vec3 changedVec3 = {0};

  changedVec3.x = a.x*scalar;
  changedVec3.y = a.y*scalar;
  changedVec3.z = a.z*scalar;
  
  return changedVec3;
}


float operator *(Vec3 a, Vec3 b){
  Vec3 changedVec3 = {0};

  changedVec3.x = a.x*b.x;
  changedVec3.y = a.y*b.y;
  changedVec3.z = a.z*b.z;
  
  return changedVec3.x+changedVec3.y + changedVec3.z;
}

Vec3 operator -(Vec3 a, Vec3 b){
  Vec3 changedVec3 = {0};

  changedVec3.x = a.x - b.x;
  changedVec3.y = a.y - b.y;
  changedVec3.z = a.z - b.z;
  
  return changedVec3;
}

float distanceVec3(Vec3 a){

  float distance = std::sqrt( std::pow(a.x, 2) +  std::pow(a.y, 2) + std::pow(a.z, 2));  
  return distance;
}

float dotProduct(Vec3 a, Vec3 b){
  float upper_part = a*b;
  float lower_part = distanceVec3(a) * distanceVec3(b);

  return upper_part/lower_part;
}

Vec3 crossProduct(Vec3 a, Vec3 b){
  Vec3 result = {0};

  result.x = a.y*b.z - a.z*b.y;
  result.y = a.z*b.x - a.x*b.z;
  result.z = a.x*b.y - a.y*b.x;

  return result;
}

