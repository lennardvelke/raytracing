#include "camera.h"
#include "ray.h"
#include "vec3.h"
#include <cmath>


void createRayForPixel(Camera* camera, int pixelX, int pixelY,  double scale, Ray* returnRay){
 
  double ndc_x = ((double)pixelX + 0.5f) / camera->width;
  double ndc_y = ((double)pixelY + 0.5f) / camera->height;

  double screen_x = 2 * ndc_x - 1;
  double screen_y = 1 - 2.0f * ndc_y; 

  double aspect = (double) camera->width / (double) camera->height;

  double px = screen_x * tan(camera->fov/2.0f * M_PI/180.0f) * aspect;
  double py = screen_y * tan(camera->fov/2.0f * M_PI/180.0f) * aspect;

  double screenWidth = camera->width * scale; 
  
  double distanceToScreenWidthEdge = screenWidth/(2* std::sin(camera->fov * M_PI / 180.0f));

  double distanceToScreen = 1.0f/2.0f * sqrt(4 * distanceToScreenWidthEdge*distanceToScreenWidthEdge - screenWidth*screenWidth);

  returnRay->direction = normalize(Vec3{distanceToScreen, pixelX*scale - scale*camera->width/2.0, pixelY*scale - scale*camera->height/2});
  returnRay->position = camera->position;
  
}


