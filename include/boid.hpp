#ifndef BOID_HPP_
#define BOID_HPP_
#include "common.hpp"

struct Boid {
  Boid(const Vec& _p = Vec(), const Vec& _v = Vec(),
    const Vec& _a = Vec(), int _next = -1)
    : p(_p), v(_v), a(_a), next(_next) {}
  Vec p, v, a;
  int next;
} CL_ALIGNED(32);


extern std::vector<Boid> bs;
extern Vec low, high;

void initBoids(int num);
void oclInit();
void moveBoids(float time);

#endif
