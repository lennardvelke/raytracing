
#version 430 core
out vec4 FragColor;
in vec2 TexCoord;

#define PI 3.1415926
#define EPSILON 1.0e-10 // Small value to avoid division by zero
#define GAMMA 1.7
#define INF 1.0 / 0.0;
#define wantPixelInfo true
struct Material {
  vec3 color;
  vec3 emission_color;
  float emission_strength;
};


//Object Types
#define SPHERE 1
#define PLANE 2

struct Object {
  int id;
  uint type;
  //General
  vec3 position;
  //Object  
  float radius;
  //Plane
  vec2 dimensions; 
  Material material;
};

struct Triangle {
  vec3 pointA;
  vec3 pointB;
  vec3 pointC;
};


struct Ray {
  int last_hit_id;
  vec3 pos;
  vec3 direction;
};

struct Camera {
  float fov;
  vec3 position;
  float ratio;
  vec3 up;
  vec3 right;
  vec3 forward;
};

struct Hit{
  bool collide ;
  vec3 hit_point;
  vec3 normal;
  Object object;
  int last_hit_id;
};


uniform float cam_fov;
uniform vec3 cam_pos;
uniform float cam_ratio;
uniform vec3 cam_up;
uniform vec3 cam_rigth;
uniform vec3 cam_forward;
uniform float u_time;
uniform uint u_numAccumulatedFrames;
uniform uint u_num_rays_per_pixel;

Ray createRayForPixel(Camera camera) {
  vec3 camRight = normalize(cross(camera.forward, camera.up));
  vec3 camUp    = normalize(cross(camRight, camera.forward));

  // Normalisierte Pixelkoordinaten [-1, 1]
  vec2 uv = TexCoord * 2.0 - 1.0;

  // Field of view (in radians!)
  float fovScale = tan(radians(camera.fov) * 0.5);

  // Seitenverhältnis
  float aspect = camera.ratio; // = width / height

  // Strahlrichtung mit FOV berücksichtigen
  vec3 rayDir = normalize(
    camera.forward +
    uv.x * camRight * aspect * fovScale +
    uv.y * camUp * fovScale
  );

  return Ray(-1, camera.position, rayDir);
}
/** The RandomValue function generates a random value between 0 and 1 using a simple linear congruential generator (LCG).
 * The function uses the LCG algorithm to generate a sequence of pseudo-random numbers based on a seed value.
 * Thanks to https://www.pcg-random.org, https://www.shadertoy.com/view/XlGcRh
 */
float RandomValue(inout uint state)
{

  state = state * 747796405u + 2891336453u;
  uint result = ((state >> ((state >> 28u) + 4u)) ^ state) * 277803737u;
  result = (result >> 22u) ^ result;
  return float(result) / 4294967295.0;
}

/** The RandomValueNormalDistribution function generates a random value from a normal distribution using the Box-Muller transform.
 * The function generates two random values from a uniform distribution and transforms them into a random value from a normal distribution.
 * Thanks to https://stackoverflow.com/a/6178290
 */
float RandomValueNormalDistribution(inout uint state)
{
  float theta = 2 * PI * RandomValue(state);
  float rho = sqrt(-2 * log(RandomValue(state)));
  return rho * cos(theta);
}

/** The RandomDirection function generates a random direction vector by sampling from a normal distribution in three dimensions.
 * The function generates three random values from a normal distribution and normalizes them to create a random direction vector.
 * Thanks to https://math.stackexchange.com/questions/1585975
 */
vec3 RandomDirection(inout uint state)
{
  float x = RandomValueNormalDistribution(state);
  float y = RandomValueNormalDistribution(state);
  float z = RandomValueNormalDistribution(state);
  return normalize(vec3(x, y, z));
}

/** The RandomDirectionInHemisphere function generates a random direction vector in the hemisphere defined by the normal vector.
 * The distribution is cosine-weighted. (meaning more rays are sent in the direction of the normal)
 */
vec3 RandomDirectionInHemisphere(vec3 normalVector, inout uint state)
{
  vec3 randomDirectionVector = RandomDirection(state);
  if (dot(normalVector, randomDirectionVector) < 0)
  {
    randomDirectionVector = -randomDirectionVector;
  }
  return randomDirectionVector;
}


Hit collisionTriangle( Ray ray, Triangle triangle, Object object)
{
  Hit information;
  information.collide = false;
    vec3 edge1 = triangle.pointB - triangle.pointA;
    vec3 edge2 = triangle.pointB - triangle.pointA;
    vec3 ray_cross_e2 = cross(ray.direction, edge2);
    float det = dot(edge1, ray_cross_e2);

    if (det == 0)
        return information;    // This ray is parallel to this triangle.

    float inv_det = 1.0 / det;
    vec3 s = ray.pos - triangle.pointA;
    float u = inv_det * dot(s, ray_cross_e2);

    if (u < 0 || u > 1)
        return information;

    vec3 s_cross_e1 = cross(s, edge1);
    float v = inv_det * dot(ray.direction, s_cross_e1);

    if (v < 0 || u + v > 1 )
        return information;

    // At this stage we can compute t to find out where the intersection point is on the line.
    float t = inv_det * dot(edge2, s_cross_e1);

    if (t > 0) // ray intersection
    {
  information.last_hit_id = object.id;
  information.collide = true;
  information.object = object;
  


    information.hit_point = ray.pos + t * ray.direction;
  

    information.normal = information.hit_point - object.position;
    
    return  information;
    }
    else // This means that there is a line intersection but not a ray intersection.
        return information;
}


Hit collisionPlane(Ray ray, Object object){


  


  Hit hit_information = Hit(false, vec3(0,0,0), vec3(0,0,0), object, -1);

  // Triangle triangle = Triangle(vec3(object.position )); 
  // hit_information = collisionTriangle(ray, object)

  return hit_information; 

}


Hit collisionSphere(Ray ray, Object object){

  float a = pow(length(ray.direction), 2); 
  float b = 2*((ray.pos.x-object.position.x) * ray.direction.x + (ray.pos.y - object.position.y)* ray.direction.y + (ray.pos.z - object.position.z) * ray.direction.z);
  float c = (pow(object.position.x, 2) - 2 * object.position.x * ray.pos.x + pow(ray.pos.x, 2) 
    +(pow(object.position.y, 2) - 2 * object.position.y * ray.pos.y + pow(ray.pos.y, 2))
    +(pow(object.position.z, 2) - 2 * object.position.z * ray.pos.z + pow(ray.pos.z, 2))  
  ) - object.radius*object.radius;

  float delta = pow(b,2) - 4*a*c;


  Hit hit_information = Hit(false, vec3(0,0,0), vec3(0,0,0), object, -1);

  if (delta < 0) {
    return hit_information;
  }

  // if(delta >= b*b){
  //   return hit_information;
  // }


  float n = (-b - sqrt(delta))/(2*a); 
  if (n <=0) {
    n = (-b + sqrt(delta))/(2*a); 
    if(n<= 0){
      return hit_information;
    } 
  }



  hit_information.last_hit_id = object.id;
  hit_information.collide = true;
  hit_information.object = object;
  hit_information.hit_point = ray.pos + n * ray.direction;
  
  vec3 normal = hit_information.hit_point - object.position;  

  if(dot(ray.direction, normal)>=0){
    normal = -normal;
  }
  hit_information.normal = normal;
  return hit_information; 

}

Hit traceRay(Object[4] objects, Camera camera, Ray ray){
  Hit information;
  information.collide = false;
  float current_distance = 1.0/0.0;
  for(int cur_id = 0; cur_id < objects.length(); cur_id++){
    if(objects[cur_id].id == ray.last_hit_id){
      continue;
    }
    Hit current_info;
    current_info.collide = false;

    switch(objects[cur_id].type){
      case SPHERE: 
        current_info = collisionSphere(ray, objects[cur_id]);
        break;
      default:
        break;
    }

    if(!current_info.collide){
      continue;
    }
    if(length(current_info.hit_point - camera.position) < current_distance ){
      current_distance = length(current_info.hit_point - camera.position);
      information = current_info;
    }
  }

  return information;
}
uint getCurrentState(vec2 texelCoords, int screenWidth)
{
  uint pixelIndex = (uint(texelCoords.y) * uint(screenWidth)) + uint(texelCoords.x);
  return pixelIndex + u_numAccumulatedFrames * 745621; // new state every frame
}
vec3 rayCast(Ray ray, Camera camera, Object[4] objects, inout uint state){
  vec3 color = vec3(1);
  vec3 brightness = vec3(0,0,0);
  for(int number_of_bounces = 0; number_of_bounces < 5000; number_of_bounces++){

    Hit info = traceRay(objects, camera, ray);

    if(info.collide){
      vec3 dir = RandomDirectionInHemisphere(info.normal, state);

      ray.pos = info.hit_point;
      ray.direction = dir;
      ray.last_hit_id = info.last_hit_id;
      vec3 emitted_light = info.object.material.emission_color * info.object.material.emission_strength;
      brightness += emitted_light * color;
      color *= info.object.material.color;

    }
    else{
      break;
    }
  }
  return brightness;
}

void main(void){


  Camera camera;
  camera.position = cam_pos;
  camera.right = cam_rigth;
  camera.up = cam_up;
  camera.ratio = cam_ratio;
  camera.fov = cam_fov;
  camera.forward = cam_forward;

  uint state = getCurrentState(TexCoord*800, 800);

  Material material = Material(vec3(1,1,1),vec3(1),1);  
  Material material2 = Material(vec3(0,0,1),vec3(0,0,1),0.0);
  Material material3 = Material(vec3(1,0,0),vec3(1,0,0),0.01);
  Material material4 = Material(vec3(0,1,0),vec3(0,0,0),1);
  Object object = Object(0,SPHERE,vec3(12,5,0), 3, vec2(0), material);
  Ray ray = createRayForPixel(camera);
  Object object2 = Object(1,SPHERE,vec3(15,0,0), 1, vec2(0), material2);
  Object object3 = Object(2,SPHERE,vec3(10,0,-20), 20, vec2(0), material3);
  Object object4 = Object(3,SPHERE,vec3(10,0,0), 1, vec2(0), material4);

  Object[] objects = {object2, object,object4, object3};


    vec3 color = vec3(0);  

    uint number_of_samples = u_num_rays_per_pixel;

    for(uint sample_count = 0; sample_count < number_of_samples; sample_count++){
      color += rayCast(ray, camera, objects, state);
    }

    vec3 finalcolor = color/number_of_samples ;



    finalcolor = pow(finalcolor, vec3(1.0/GAMMA));  
    FragColor = vec4(finalcolor,1);

}



