
#ifndef RENDERER_H
#define RENDERER_H

#include "camera.h"
#include "option.h"
#include "light.h"
#include "circle.h"
#include <vector>

void renderRayTracingScene(Camera* camera, std::vector<Circle*> * objects, Option * option, unsigned char* framebuffer);


#endif // !RENDERER_H

