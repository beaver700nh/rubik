#ifndef RUBIK_H
#define RUBIK_H

#include <curses.h>

#include "calc.h"

#define RUBIK_DIMS 3
#define CUBIE_SIZE_X0 4
#define CUBIE_SIZE_X1 6
#define CUBIE_SIZE_Y0 2
#define CUBIE_SIZE_Y1 3

typedef struct {
  unsigned short size;
  vec3_t *cubies; // 3D array [x][y][z]
} rubik_t;

void rubik_init(rubik_t *rubik, unsigned short size);
void rubik_init_cubie(vec3_t *cubie, unsigned short size, vec3_t pos);
void rubik_copy(rubik_t *rubik, rubik_t *other);
vec3_t *rubik_get(rubik_t *rubik, vec3_t position);
void rubik_draw(rubik_t *rubik, WINDOW *window);
void rubik_draw_face_xy0(rubik_t *rubik, WINDOW *window, unsigned short row, unsigned short col);
void rubik_dump(rubik_t *rubik, WINDOW *window, unsigned short position);
rubik_t rubik_rotated(rubik_t *rubik, rubik_rotator_t rotator);
void rubik_rotate_ip(rubik_t *rubik, rubik_rotator_t rotator);
void rubik_free(rubik_t *rubik);

#endif
