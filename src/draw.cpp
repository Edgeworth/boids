#include "draw.hpp"
#include "boid.hpp"

const int DIV = 16;
const float pi = 3.14159265;

void drawCircle(Vec p, float r) {
  glBegin(GL_POLYGON);
  for (int i = 0; i < DIV; ++i) {
    float angle = (float(i)/DIV)*2*pi;
    glVertex2f(cos(angle)*r+p.x, sin(angle)*r+p.y);
  }
  glEnd();
}


void draw(sf::RenderWindow& win) {
  glLoadIdentity();
  glTranslatef(0.0, 0.0, -5.0);
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
  glColor3f(0.0, 0.0, 0.0);

  for (int i = 0; i < bs.size(); ++i) {
    drawCircle(bs[i].p, 0.5);
    if (bs[i].v.mag() > 0) {
      Vec dir = bs[i].p + bs[i].v.resize(2);
      glBegin(GL_LINES);
        glVertex2f(bs[i].p.x, bs[i].p.y);
        glVertex2f(dir.x, dir.y);
      glEnd();
    }
  }

  win.display();
}
