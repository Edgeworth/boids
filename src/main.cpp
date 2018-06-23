#include "common.hpp"
#include "draw.hpp"
#include "boid.hpp"

#define PROFILE(x) { \
  sf::Clock clock; \
  x; \
  printf(#x " time: %f\n", clock.GetElapsedTime()); \
}

int main(int argc, char *argv[]) {
  if (argc != 6) {
    printf("usage: winx winy viewx viewy nboids\n");
    return 0;
  }
  int winx = atoi(argv[1]), winy = atoi(argv[2]);
  high.x = atoi(argv[3]); high.y = atoi(argv[4]);

  sf::RenderWindow win(sf::VideoMode(winx, winy), "Boids",
    sf::Style::Resize|sf::Style::Close, sf::ContextSettings(24, 8, 16));
  win.setVerticalSyncEnabled(true);
  glViewport(0, 0, winx, winy);
  glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(low.x, high.x, low.y, high.y, -10, 10);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_LINE_SMOOTH);
  glEnable(GL_POINT_SMOOTH);
  glEnable(GL_POLYGON_SMOOTH);
  glClearColor(1, 1, 1, 1);

  srand(time(NULL));
  initBoids(atoi(argv[5]));
  oclInit();

  sf::Clock clock;
  while (win.isOpen()) {
    sf::Event event;
    while (win.pollEvent(event))
      if (event.type == sf::Event::Closed)
        win.close();
    moveBoids(clock.getElapsedTime().asSeconds());
    clock.restart();

    draw(win);
  }
}
