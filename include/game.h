#ifndef GAME_H
#define GAME_H

class Game {
public:
  Game(int width, int height, const char *title);
  ~Game();

  void Run();

private:
  int screenWidth;
  int screenHeight;
  const char *windowTitle;

  void Update(float dt);
  void Draw(float dt);
};

#endif // GAME_H
