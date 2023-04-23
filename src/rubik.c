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

static char const RUBIK_FACES[3][2] = {
  {'L', 'R'},
  {'B', 'F'},
  {'U', 'D'},
};

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

    cubie->facies.buf[d] = sign * color;
  }
}

void rubik_init(rubik_t *rubik, unsigned short size) {
  rubik->size = size;
  rubik->cubies = malloc(pow(size, RUBIK_DIMS) * sizeof (cubie_t));

  for (unsigned short i = 0; i < size; ++i) {
    for (unsigned short j = 0; j < size; ++j) {
      for (unsigned short k = 0; k < size; ++k) {
        vec3_t pos_vec = {.x = i, .y = j, .z = k};
        cubie_init(rubik_get(rubik, pos_vec), size, pos_vec);
      }
    }
  }
}

void rubik_copy(rubik_t *rubik, rubik_t *other) {
  if (rubik->size != other->size) {
    return;
  }
  if (rubik->cubies == other->cubies) {
    return;
  }

  memcpy(rubik->cubies, other->cubies, pow(rubik->size, 3) * sizeof (cubie_t));
}

cubie_t *rubik_get(rubik_t *rubik, vec3_t position) {
  return &rubik->cubies[vec3_to_index(&position, rubik->size)];
}

void rubik_draw(rubik_t *rubik, WINDOW *window) {
  rubik_dump(rubik, window, CUBIE_SIZE_Y1 + 6, (4 + 3*rubik->size) * CUBIE_SIZE_X1 + 3);

  rubik_draw_face_xy0(rubik, window, 2 + rubik->size, 2 + rubik->size);

  rubik_t rubik_x0z = rubik_rotated(rubik, ROTATORS[CDL_DIR_X][1]);
  rubik_draw_face_xy0(&rubik_x0z, window, 1, 2 + rubik->size);
  rubik_free(&rubik_x0z);

  rubik_t rubik_0yz = rubik_rotated(rubik, ROTATORS[CDL_DIR_Y][1]);
  rubik_draw_face_xy0(&rubik_0yz, window, 2 + rubik->size, 1);
  rubik_free(&rubik_0yz);

  rubik_t rubik_2yz = rubik_rotated(rubik, ROTATORS[CDL_DIR_Y][0]);
  rubik_draw_face_xy0(&rubik_2yz, window, 2 + rubik->size, 3 + 2*rubik->size);
  rubik_free(&rubik_2yz);

  rubik_t rubik_x2z = rubik_rotated(rubik, ROTATORS[CDL_DIR_X][0]);
  rubik_draw_face_xy0(&rubik_x2z, window, 3 + 2*rubik->size, 2 + rubik->size);

  rubik_t rubik_xy2 = rubik_rotated(&rubik_x2z, ROTATORS[CDL_DIR_X][0]);
  rubik_draw_face_xy0(&rubik_xy2, window, 4 + 3*rubik->size, 2 + rubik->size);
  rubik_free(&rubik_xy2);

  rubik_free(&rubik_x2z);
}

void draw_box(WINDOW *window, unsigned short row, unsigned short col, unsigned short width, unsigned short height, char ch) {
  wattron(window, (COLOR_WHITE + 8) | A_DIM_CHECKED);

  mvwhline(window, row,              col,             ch, width);
  mvwhline(window, row + height - 1, col,             ch, width);
  mvwvline(window, row,              col,             ch, height);
  mvwvline(window, row,              col + 1,         ch, height);
  mvwvline(window, row,              col + width - 2, ch, height);
  mvwvline(window, row,              col + width - 1, ch, height);

  wattroff(window, (COLOR_WHITE + 8) | A_DIM_CHECKED);
}

void rubik_draw_face_xy0(rubik_t *rubik, WINDOW *window, unsigned short row, unsigned short col) {
  for (unsigned short i = 0; i < rubik->size; ++i) {
    for (unsigned short j = 0; j < rubik->size; ++j) {
      unsigned short y = (row + j) * CUBIE_SIZE_Y1;
      unsigned short x = (col + i) * CUBIE_SIZE_X1;

      draw_box(window, y - 1, x - 2, CUBIE_SIZE_X1 + 2, CUBIE_SIZE_Y1 + 1, ' ');
    }
  }

  for (unsigned short i = 0; i < rubik->size; ++i) {
    for (unsigned short j = 0; j < rubik->size; ++j) {
      vec3_t pos_vec = {.x = i, .y = j, .z = 0};
      cubie_t *cubie = rubik_get(rubik, pos_vec);
      int color = COLOR_PAIR(abs(cubie->facies.buf[CDL_DIR_Z]) + 8);

      unsigned short y = (row + j) * CUBIE_SIZE_Y1;
      unsigned short x = (col + i) * CUBIE_SIZE_X1;

      if (cubie->selected) {
        draw_box(window, y - 1, x - 2, CUBIE_SIZE_X1 + 2, CUBIE_SIZE_Y1 + 1, ':');
      }

      wattron(window, color);

      for (unsigned short a = 0; a < CUBIE_SIZE_X0; ++a) {
        for (unsigned short b = 0; b < CUBIE_SIZE_Y0; ++b) {
          mvwaddch(window, y + b, x + a, ' ');
        }
      }

      wattroff(window, color);
    }
  }
}

void rubik_dump(rubik_t *rubik, WINDOW *window, unsigned short row, unsigned short col) {
  for (unsigned short i = 0; i < rubik->size; ++i) {
    for (unsigned short j = 0; j < rubik->size; ++j) {
      for (unsigned short k = 0; k < rubik->size; ++k) {
        vec3_t pos_vec = {.x = i, .y = j, .z = k};
        cubie_t *cubie = rubik_get(rubik, pos_vec);
        unsigned short flat = vec3_to_index(&pos_vec, rubik->size);

        for (unsigned short d = 0; d < RUBIK_DIMS; ++d) {
          short facie = cubie->facies.buf[d];
          short color = abs(facie);

          unsigned short y = row + (flat % (int) pow(rubik->size, 2));
          unsigned short x = col + 3*d + 10*i;
          char direction = facie < 0 ? RUBIK_FACES[d][0] : facie > 0 ? RUBIK_FACES[d][1] : ' ';

          wattron(window, COLOR_PAIR(color) | A_DIM_CHECKED * (facie == 0));
          mvwprintw(window, y, x, "%c%hd", direction, pos_vec.buf[d]);
          wattroff(window, COLOR_PAIR(color) | A_DIM_CHECKED);
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

        result.cubies[after].facies = (*rotator)(rubik->cubies[before].facies, 1);
        result.cubies[after].selected = rubik->cubies[before].selected;
      }
    }
  }

  return result;
}

rubik_t rubik_rotated_slice(rubik_t *rubik, rubik_rotator_t rotator, unsigned short axis, unsigned short slice) {
  rubik_t result;
  rubik_init(&result, rubik->size);
  rubik_copy(&result, rubik);

  for (unsigned short a = 0; a < rubik->size; ++a) {
    for (unsigned short b = 0; b < rubik->size; ++b) {
      vec3_t before_vec = {
        .x = (axis == CDL_DIR_X ? slice : a),
        .y = (axis == CDL_DIR_Y ? slice : axis == CDL_DIR_X ? a : b),
        .z = (axis == CDL_DIR_Z ? slice : b),
      };
      unsigned short before = vec3_to_index(&before_vec, rubik->size);
      vec3_t after_vec = (*rotator)(before_vec, rubik->size);
      unsigned short after = vec3_to_index(&after_vec, rubik->size);

      result.cubies[after].facies = (*rotator)(rubik->cubies[before].facies, 1);
      result.cubies[after].selected = rubik->cubies[before].selected;
    }
  }

  return result;
}

void rubik_selection(rubik_t *rubik, unsigned short axis, unsigned short slice, unsigned short value) {
  for (unsigned short a = 0; a < rubik->size; ++a) {
    for (unsigned short b = 0; b < rubik->size; ++b) {
      vec3_t pos_vec = {
        .x = (axis == CDL_DIR_X ? slice : a),
        .y = (axis == CDL_DIR_Y ? slice : axis == CDL_DIR_X ? a : b),
        .z = (axis == CDL_DIR_Z ? slice : b),
      };
      unsigned short index = vec3_to_index(&pos_vec, rubik->size);
      rubik->cubies[index].selected = value;
    }
  }
}

void rubik_free(rubik_t *rubik) {
  free(rubik->cubies);
  rubik->cubies = NULL;
}
