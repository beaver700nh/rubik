#include <math.h>

#include "calc.h"

unsigned short vec3_to_index(vec3_t *vec3, unsigned short size) {
  unsigned short index = 0;

  for (unsigned short d = 0; d < 3; ++d) {
    index += vec3->buf[d] * pow(size, 2 - d);
  }

  return index;
}

vec3_t rotator_xp(vec3_t input, unsigned short size) {
  vec3_t output = {{input.x, size - 1 - input.z, input.y}};
  return output;
}

vec3_t rotator_xm(vec3_t input, unsigned short size) {
  vec3_t output = {{input.x, input.z, size - 1 - input.y}};
  return output;
}

vec3_t rotator_yp(vec3_t input, unsigned short size) {
  vec3_t output = {{size - 1 - input.z, input.y, input.x}};
  return output;
}

vec3_t rotator_ym(vec3_t input, unsigned short size) {
  vec3_t output = {{input.z, input.y, size - 1 - input.x}};
  return output;
}

vec3_t rotator_zp(vec3_t input, unsigned short size) {
  vec3_t output = {{size - 1 - input.y, input.x, input.z}};
  return output;
}

vec3_t rotator_zm(vec3_t input, unsigned short size) {
  vec3_t output = {{input.y, size - 1 - input.x, input.z}};
  return output;
}

rubik_rotator_t const ROTATORS[3][2] = {
  {&rotator_xm, &rotator_xp},
  {&rotator_ym, &rotator_yp},
  {&rotator_zm, &rotator_zp},
};
