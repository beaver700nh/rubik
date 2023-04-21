#include <math.h>
#include <stdlib.h>

#include <curses.h>

#include "calc.h"

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

cubie_t *rubik_get(rubik_t *rubik, vec3_t pos) {
  return &rubik->cubies[vec3_to_index(&pos, rubik->size)];
}

void rubik_draw(rubik_t *rubik, WINDOW *window, cdl_dir_t top, cdl_dir_t back) {
  rubik_dump(rubik, window, 160);
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

void rubik_free(rubik_t *rubik) {
  free(rubik->cubies);
  rubik->cubies = NULL;
}
