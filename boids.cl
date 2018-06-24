#include "include/constants.hpp"

__constant int xdir[9] = {-1, 0, 1, -1, 0, 1, -1, 0, 1};
__constant int ydir[9] = {1, 1, 1, 0, 0, 0, -1, -1, -1};

typedef struct {
  float2 p, v, a;
  int next;
} Boid __attribute((aligned(32)));

int2 qtize(float2 v, float2 low) {
  return convert_int2((v-low)/SEE);
}

void kernel move(global Boid* in, global Boid* out,
  global int* bt, float time, float2 low, float2 high) {
  int xid = get_global_id(0);

  int nClose = 0;
  float2 avgp = 0, avgv = 0, sep = 0;
  float min = 1000000000;

  int2 qtized = qtize(in[xid].p, low);
  int2 sz = qtize(high, low)+(int2)(1);
  for (int i = 0; i < 9; ++i) {
    int2 np = qtized + (int2)(xdir[i], ydir[i]);
    if (np.x < 0 || np.x >= sz.x || np.y < 0 || np.y >= sz.y) continue;

    int cur = bt[np.x*sz.x+np.y];
    while (cur != -1) {
      if (cur != xid) {
        float dist = distance(in[xid].p, in[cur].p);
        if (dist <= SEE) {
          ++nClose;
          avgp += in[cur].p;
          avgv += in[cur].v;
          if (dist < min && dist <= SEP) {
            min = dist;
            sep = (in[xid].p - in[cur].p)*pown(SEP/dist, 2);
          }
        }
      }
      cur = in[cur].next;
    }
  }

  out[xid] = in[xid];
  out[xid].a = 0;

  if (nClose > 0) {
    avgp /= nClose;
    avgv /= nClose;
    out[xid].a = sep*SEP_F + 
      (avgv-in[xid].v)*STEER_F + 
      (avgp-in[xid].p)*ATTRACT_F + 
      in[xid].v*SPEED_F;
  }

  out[xid].p += 0.5f*time*time*out[xid].a + out[xid].v*time;

  float2 range = high-low;
  out[xid].p -= range*floor((out[xid].p-low)/range);
  if (out[xid].p.x < low.x) out[xid].p.x += range.x;
  if (out[xid].p.y < low.y) out[xid].p.y += range.y;

  out[xid].v += out[xid].a*time;
  float len = length(out[xid].v);
  if (len > MAX_SPEED) out[xid].v *= MAX_SPEED/len;
}
