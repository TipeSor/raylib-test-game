#ifndef GAME_H
#define GAME_H

#include "rmanager.h"
class Game {
public:
  Game(int width, int height, const char *title);
  ~Game();

  void Run();

private:
  int screenWidth;
  int screenHeight;
  const char *windowTitle;
  ResourceManager r_manager;

  void Update(float dt);
  void Draw(float dt);
};

#endif // GAME_H
