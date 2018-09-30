#ifndef FORMAT_H
#define FORMAT_H

typedef struct Motor4Wheels {
  float velocity[4];
  float current[4];
}wheels_t;

typedef struct Fan {
  bool is_rotating;
  float velocity;
}fan_t;

extern void calc_current(float vel_dest);

#endif
