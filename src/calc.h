#ifndef CALC_H
#define CALC_H

typedef enum {
  CDL_DIR_X,
  CDL_DIR_Y,
  CDL_DIR_Z,
} cdl_dir_t;

typedef union {
  struct {
    unsigned short x;
    unsigned short y;
    unsigned short z;
  };
  unsigned short buf[3];
} vec3_t;

unsigned short vec3_to_index(vec3_t *vec3, unsigned short size);

typedef vec3_t (*rubik_rotator_t)(vec3_t, unsigned short);

extern rubik_rotator_t const ROTATORS[3][2];

#endif
