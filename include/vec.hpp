#ifndef VEC_HPP_
#define VEC_HPP_
#include "common.hpp"

float wrap(float x, float a, float b);
float randFloat(float lower, float upper);

struct Vec {
  float x, y;

  Vec(float _x = 0, float _y = 0) : x(_x), y(_y) {}

  float mag() {return sqrt(x*x+y*y);}
  Vec resize(float m) {return *this * (m/mag());}
  Vec& clamp(float m) {if (mag() > m) *this *= m/mag(); return *this;}
  Vec& wrap(const Vec& a, const Vec& b) {x = ::wrap(x, a.x, b.x); y = ::wrap(y, a.y, b.y); return *this;}

  Vec operator+(const Vec& v) const {return Vec(x+v.x, y+v.y);}
  Vec operator-(const Vec& v) const {return Vec(x-v.x, y-v.y);}

  Vec& operator+=(const Vec& v) {x += v.x; y += v.y; return *this;}
  Vec& operator-=(const Vec& v) {x -= v.x; y -= v.y; return *this;}

  Vec operator*(const float& f) const {return Vec(x*f, y*f);}
  Vec operator/(const float& f) const {assert(f!=0.0); return Vec(x/f, y/f);}

  Vec& operator*=(const float& f) {x *= f; y *= f; return *this;}
  Vec& operator/=(const float& f) {assert(f!=0.0); x /= f; y /= f; return *this;}
} CL_ALIGNED(8);

inline Vec operator*(const float& f, const Vec& v) {return Vec(f*v.x, f*v.y);}

Vec randVec(const Vec& lower, const Vec& upper);
Vec randVec(float mag);

#endif
