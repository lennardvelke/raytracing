typedef struct {
 float x,y,z;
} Vec3;


Vec3 addVec3(Vec3 a, Vec3 b){
  Vec3 changedVec3 = {0};

  changedVec3.x = a.x + b.x;
  changedVec3.y = a.y + b.y;
  changedVec3.z = a.z + b.z;
  
  return changedVec3;
}
