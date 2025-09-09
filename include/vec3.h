#ifndef VEC3_H
#define VEC3_H

typedef struct {
 float x,y,z;
} Vec3;



Vec3 operator +(Vec3 a, Vec3 b);
double operator *(Vec3 a, Vec3 b);
Vec3 mulElements(Vec3 a, Vec3 b);
Vec3 operator *(Vec3 a, double scalar);
Vec3 operator *(double scalar, Vec3 a);

Vec3 operator -(Vec3 a, Vec3 b);

double lengthVec2(Vec3 a);

double dotProduct(Vec3 a, Vec3 b);
Vec3 crossProduct(Vec3 a, Vec3 b);

Vec3 normalize(Vec3 a);
void printVec3(char* text, Vec3 vector);

Vec3 maxElements(Vec3 a, Vec3 b);
Vec3 operator /(Vec3 a, Vec3 b);
Vec3 rotateAroundAxis(Vec3 axis, Vec3 vector, float rotation);
#endif
