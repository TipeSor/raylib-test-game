#ifndef GAME_H
#define GAME_H

#include "inspector.h"
#include "object.h"
#include "rmanager.h"
#include <memory>
class Game {
public:
  Game(int width, int height, const char *title);
  ~Game();

  void Run();

private:
  int screenWidth;
  int screenHeight;
  std::string windowTitle;
  ResourceManager r_manager;
  std::unique_ptr<Object> object;
  Inspector inspector;

  void Setup();
  void Update(float dt);
  void Draw(float dt);
};

#endif // GAME_H
