#ifndef BIRD_H  
#define BIRD_H  

#include "Vec2.h"
#include "Body.h"
#include <vector>  
#include <iostream>  


struct Bird {  
  VerletBody* body;  
  std::vector<Vec2*> trail;  
  std::vector<Vec2*> preview;  

  bool isDragging = false;
  bool isFlying = false;
  float stretchingMultiplier = 500.f;
  float birdSize = 30.f;

  Bird() = default;
  Bird(float size, Vec2 pos, const char* textureFileName);
  ~Bird();

  void RenderPreview();
  void RenderTrail();
};  

#endif
