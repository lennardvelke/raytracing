#include <stdio.h>
#include "circle.h"
#include "ray.h"

int main(){
  Circle test = {0};
  Ray ray = {0};

  test.position.x = 3.5;
  test.position.y = 1;
  test.radius = 1;

  ray.direction.x = 1;
 

  printf("Hello World %b", fire_ray(ray, test));


  return 0;
}


