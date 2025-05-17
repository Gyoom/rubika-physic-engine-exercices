#ifndef BIRD_H  
#define BIRD_H  

#include "Entity.h"

class World;

struct Bird : Entity {   
  std::vector<Vec2*> trail;  
  std::vector<Vec2*> preview;  

  bool isDragging = false;
  bool isFlying = false;
  
  float maxStretching = 100.f;
  float stretchingMultiplier = 500.f;
  
  float birdSize = 30.f;

  Vec2 catapultPos;
  Vec2 catapultBackPos;
  Vec2 catapultFrontPos;

  Bird() = default;
  Bird(float size, Vec2 pos, const char* textureFileName, Vec2 catapultPos);
  ~Bird();

  void Update(float dt, World* world);

  void RenderPreview();
  void RenderTrail();
  void RenderStretching(bool back);
};  

#endif
