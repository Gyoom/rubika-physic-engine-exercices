#ifndef PIG_H  
#define PIG_H  

#include "Entity.h" 


struct Pig : Entity {  
  
  Pig() = default;
  Pig(float size, Vec2 pos, const char* textureFileName);
  ~Pig();
};  

#endif
