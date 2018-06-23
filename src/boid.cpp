#include "boid.hpp"

std::vector<Boid> bs;
Vec low(0, 0), high(200, 200);

void initBoids(int num) {
  bs.resize(num);
  for (int i = 0; i < num; ++i)
    bs[i] = Boid(randVec(low, high), randVec(50));
}

