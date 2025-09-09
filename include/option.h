#ifndef OPTION_H
#define OPTION_H

typedef struct {
double height = 50;
  double witdh = 50;
  int depth = 5;
  int bounces = 3;
  int rays_per_pixel = 1;
  int samples_per_ray_bounce = 1;
} Option;

#endif // !DEBUG
