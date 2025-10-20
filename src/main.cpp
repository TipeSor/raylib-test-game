#include "game.h"

int main() {
  int monitor = GetCurrentMonitor();
  int screenWidth = GetMonitorWidth(monitor);
  int screenHeight = GetMonitorHeight(monitor);

  Game game(screenWidth, screenHeight, "Game");
  game.Run();
  return 0;
}
