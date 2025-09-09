#include "renderer.h"
#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <stdio.h>
#include "circle.h"
#include "light.h"
#include "option.h"
#include "ray.h"
#include <GLFW/glfw3.h>
#include <vector>
#include "camera.h"
#include "vec3.h"
#include <omp.h>

inline double clamp(double number){
  return number<0 ? 0 : number>1 ? 1 : number;  
}

void renderRayTracingScene(Camera* camera ,std::vector<Circle*> * objects, Option * option, unsigned char* framebuffer){ 
  
  // Ray ray;
  // createRayForPixel(camera, 600, 300, 1, &ray);

  // bool hit = cast_ray(&ray, objects, lights, option, 0);
  // 
  // printf("Hit: %b", hit);
  double procent = 0;
   #pragma omp parallel for 
  // return;
  for (int y = 0; y < camera->height; y++) {
   #pragma omp parallel for 
    for (int x = 0; x < camera->width; x++) {
      int idx = (y * camera->width + x) * 3;
      Ray ray;
      Material material;
      int number_of_zero_pixel = 0;


      createRayForPixel(camera, x, y, 1.0f, &ray);
      Circle* lastObject = nullptr; 
      bool found_light = false;
      material.color = {1,1,1};
      Vec3 color = {0}; 
      Vec3 max_colors = {0};



      for (int ray_count_pixel = 0; ray_count_pixel < option->rays_per_pixel; ray_count_pixel++) {

        cast_ray(&ray, objects, lastObject, option, 0, &material);
        double maxElement = std::max(std::max(material.color.x, material.color.y), material.color.z);
        if (maxElement != 0) {
          material.color = 1.0f/maxElement * material.color;
        
        }

        color = color + material.color;
      }
      
      color = 1.0f/(double) option->rays_per_pixel * color;

      framebuffer[idx + 0] = clamp(color.x) * 255;   // Rot
      framebuffer[idx + 1] = clamp(color.y) * 255; // Grün
      framebuffer[idx + 2] = clamp(color.z) * 255;
    }
    procent += 1.0f/(double)camera->height;
    std::cout << procent * 100 << "% \n";  
  }
}

