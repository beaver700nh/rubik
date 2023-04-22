#include <math.h>
#include <stdlib.h>
#include <string.h>

#include <curses.h>

#include "rubik.h"

static short const RUBIK_COLORS[3][2] = {
  {COLOR_GREEN, COLOR_BLUE},
  {COLOR_MAGENTA, COLOR_RED},
  {COLOR_WHITE, COLOR_YELLOW},
};

unsigned short vec3_to_index(vec3_t *vec3, unsigned short size) {
  unsigned short index = 0;

  for (unsigned short d = 0; d < RUBIK_DIMS; ++d) {
    index += vec3->buf[d] * pow(size, RUBIK_DIMS - 1 - d);
  }

  return index;
}

void facie_init(facie_t *facie, short color, short sign) {
  facie->color = color;
  facie->sign = sign;
}

void cubie_init(cubie_t *cubie, unsigned short size, vec3_t pos) {
  for (unsigned short d = 0; d < RUBIK_DIMS; ++d) {
    short color, sign;

    if (pos.buf[d] == 0) {
      color = RUBIK_COLORS[d][0];
      sign = -1;
    }
    else if (pos.buf[d] == size - 1) {
      color = RUBIK_COLORS[d][1];
      sign = 1;
    }
    else {
      color = COLOR_BLACK;
      sign = 0;
    }

    facie_init(&cubie->facies[d], color, sign);
  }
}

void rubik_init(rubik_t *rubik, unsigned short size) {
  rubik->size = size;
  rubik->cubies = malloc(size*size*size * sizeof (cubie_t));

  for (unsigned short i = 0; i < size; ++i) {
    for (unsigned short j = 0; j < size; ++j) {
      for (unsigned short k = 0; k < size; ++k) {
        vec3_t pos_vec = {.x = i, .y = j, .z = k};
        cubie_init(rubik_get(rubik, pos_vec), size, pos_vec);
      }
    }
  }
}

cubie_t *rubik_get(rubik_t *rubik, vec3_t position) {
  return &rubik->cubies[vec3_to_index(&position, rubik->size)];
}

void rubik_draw(rubik_t *rubik, WINDOW *window) {
  rubik_dump(rubik, window, 160);

  rubik_draw_face_xy0(rubik, window, 5, 5);

  rubik_t rubik_x0z = rubik_rotated(rubik, ROTATORS[CDL_DIR_X][1]);
  rubik_draw_face_xy0(&rubik_x0z, window, 1, 5);
  rubik_dump(&rubik_x0z, window, 100);
  rubik_free(&rubik_x0z);

  rubik_t rubik_0yz = rubik_rotated(rubik, ROTATORS[CDL_DIR_Y][1]);
  rubik_draw_face_xy0(&rubik_0yz, window, 5, 1);
  rubik_free(&rubik_0yz);

  rubik_t rubik_2yz = rubik_rotated(rubik, ROTATORS[CDL_DIR_Y][0]);
  rubik_draw_face_xy0(&rubik_2yz, window, 5, 9);
  rubik_free(&rubik_2yz);

  rubik_t rubik_x2z = rubik_rotated(rubik, ROTATORS[CDL_DIR_X][0]);
  rubik_draw_face_xy0(&rubik_x2z, window, 9, 5);

  rubik_t rubik_xy2 = rubik_rotated(&rubik_x2z, ROTATORS[CDL_DIR_X][0]);
  rubik_draw_face_xy0(&rubik_xy2, window, 13, 5);
  rubik_free(&rubik_xy2);

  rubik_free(&rubik_x2z);
}

void rubik_draw_face_xy0(rubik_t *rubik, WINDOW *window, unsigned short row, unsigned short col) {
  for (unsigned short i = 0; i < rubik->size; ++i) {
    for (unsigned short j = 0; j < rubik->size; ++j) {
      vec3_t pos_vec = {.x = i, .y = j, .z = 0};
      cubie_t *cubie = rubik_get(rubik, pos_vec);
      int color = COLOR_PAIR(cubie->facies[CDL_DIR_Z].color + 8);

      wattron(window, color);

      for (unsigned short a = 0; a < CUBIE_SIZE_X0; ++a) {
        for (unsigned short b = 0; b < CUBIE_SIZE_Y0; ++b) {
          mvwaddch(window, (row + i) * CUBIE_SIZE_Y1 + b, (col + j) * CUBIE_SIZE_X1 + a, ' ');
        }
      }

      wattroff(window, color);
    }
  }
}

void rubik_dump(rubik_t *rubik, WINDOW *window, unsigned short position) {
  mvwprintw(window, 1, 2 + position, "Cube size: %hd", rubik->size);

  for (unsigned short i = 0; i < rubik->size; ++i) {
    for (unsigned short j = 0; j < rubik->size; ++j) {
      for (unsigned short k = 0; k < rubik->size; ++k) {
        vec3_t pos_vec = {.x = i, .y = j, .z = k};
        cubie_t *cubie = rubik_get(rubik, pos_vec);
        unsigned short flat = vec3_to_index(&pos_vec, rubik->size);

        mvwaddstr(window, 3 + flat, 9 + position, "--");

        for (unsigned short d = 0; d < RUBIK_DIMS; ++d) {
          facie_t *facie = &cubie->facies[d];

          mvwprintw(window, 3 + flat, 3 + 2*d + position, "%hd", pos_vec.buf[d]);

          wattron(window, COLOR_PAIR(facie->color) | A_DIM * (facie->sign == 0));
          mvwprintw(
            window, 3 + flat, 12 + 11*d + position,
            "%c {%hd, %2d};", 'X' + d, facie->color, facie->sign
          );
          wattroff(window, COLOR_PAIR(facie->color) | A_DIM);
        }
      }
    }
  }
}

rubik_t rubik_rotated(rubik_t *rubik, rubik_rotator_t rotator) {
  rubik_t result;
  rubik_init(&result, rubik->size);

  for (unsigned short i = 0; i < rubik->size; ++i) {
    for (unsigned short j = 0; j < rubik->size; ++j) {
      for (unsigned short k = 0; k < rubik->size; ++k) {
        vec3_t before_vec = {.x = i, .y = j, .z = k};
        unsigned short before = vec3_to_index(&before_vec, rubik->size);
        vec3_t after_vec = (*rotator)(before_vec, rubik->size);
        unsigned short after = vec3_to_index(&after_vec, rubik->size);
        memcpy(&result.cubies[after], &rubik->cubies[before], sizeof (cubie_t));
      }
    }
  }

  return result;
}

void rubik_free(rubik_t *rubik) {
  free(rubik->cubies);
  rubik->cubies = NULL;
}