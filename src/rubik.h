#ifndef RUBIK_H
#define RUBIK_H

#include <curses.h>

#include "calc.h"

#define RUBIK_DIMS 3

#define RUBIK_IN_PLACE(ptr_rubik, operation, ...)          \
  {                                                        \
    rubik_t temp = (operation)((ptr_rubik), __VA_ARGS__);  \
    rubik_copy((ptr_rubik), &temp);                        \
    rubik_free(&temp);                                     \
  }

#define A_DIM_CHECKED (DIM_ENABLED ? A_DIM : 0)

extern unsigned short CUBIE_SIZE_X0, CUBIE_SIZE_X1, CUBIE_SIZE_Y0, CUBIE_SIZE_Y1;
extern unsigned short DIM_ENABLED;

typedef struct {
  vec3_t facies;
  unsigned short selected;
} cubie_t;

typedef struct {
  unsigned short size;
  cubie_t *cubies; // 3D array [x][y][z]
} rubik_t;

void cubie_init(cubie_t *cubie, unsigned short size, vec3_t pos);
void rubik_init(rubik_t *rubik, unsigned short size);
void rubik_copy(rubik_t *rubik, rubik_t *other);
cubie_t *rubik_get(rubik_t *rubik, vec3_t position);
void rubik_draw(rubik_t *rubik, WINDOW *window);
void rubik_draw_face_xy0(rubik_t *rubik, WINDOW *window, unsigned short row, unsigned short col);
void rubik_dump(rubik_t *rubik, WINDOW *window, unsigned short row, unsigned short col);
rubik_t rubik_rotated(rubik_t *rubik, rubik_rotator_t rotator);
rubik_t rubik_rotated_slice(rubik_t *rubik, rubik_rotator_t rotator, unsigned short axis, unsigned short slice);
void rubik_selection(rubik_t *rubik, unsigned short axis, unsigned short slice, unsigned short value);
void rubik_free(rubik_t *rubik);

#endif
