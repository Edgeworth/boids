#include "vec.hpp"

float randFloat(float lower, float upper) {
  return float(rand())/RAND_MAX * (upper-lower) + lower;
}

Vec randVec(const Vec& lower, const Vec& upper) {
  return Vec(randFloat(lower.x, upper.x), randFloat(lower.y, upper.y));
}

Vec randVec(float mag) {
  Vec v;
  v.x = randFloat(-mag, mag);
  float ymag = sqrt(mag*mag-v.x*v.x);
  v.y = randFloat(-ymag, ymag);
  return v;
}
