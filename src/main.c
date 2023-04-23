#include <curses.h>

#include <stdio.h>
#include <stdlib.h>

#include "rubik.h"

unsigned short CUBIE_SIZE_X0 = 4;
unsigned short CUBIE_SIZE_X1 = 6;
unsigned short CUBIE_SIZE_Y0 = 2;
unsigned short CUBIE_SIZE_Y1 = 3;

unsigned short DIM_ENABLED = 1;

int main(int argc, char *argv[]) {
  if (argc < 3) {
    puts("Usage: rubik <dim_enabled> <size> [cubie_size_x0 cubie_size_x1 cubie_size_y0 cubie_size_y1]");
    return EXIT_FAILURE;
  }

  DIM_ENABLED = atoi(argv[1]);

  if (argc >= 7) {
    CUBIE_SIZE_X0 = atoi(argv[3]);
    CUBIE_SIZE_X1 = atoi(argv[4]);
    CUBIE_SIZE_Y0 = atoi(argv[5]);
    CUBIE_SIZE_Y1 = atoi(argv[6]);
  }

  void main_loop(unsigned short size);

  initscr();
  noecho();
  keypad(stdscr, 1);
  curs_set(0);

  if (has_colors()) {
    start_color();
    init_color(COLOR_RED,      900,    0,    0);
    init_color(COLOR_MAGENTA,  900,  500,    0); // magenta is orange
    init_color(COLOR_YELLOW,   800,  800,    0);
    init_color(COLOR_GREEN,      0,  750,    0);
    init_color(COLOR_BLUE,     250,  500, 1000);
    init_color(COLOR_WHITE,    900,  900,  900);
    init_color(COLOR_BLACK,      0,    0,    0);

    for (unsigned short i = 0; i < 8; ++i) {
      init_pair(i + 0, i, COLOR_BLACK);
      init_pair(i + 8, COLOR_BLACK, i);
    }
  }

  main_loop(atoi(argv[2]));

  nocbreak();
  noecho();
  curs_set(1);
  endwin();

  return EXIT_SUCCESS;
}

void main_loop(unsigned short size) {
  rubik_t rubik;
  rubik_init(&rubik, size);
  rubik_t visual;
  rubik_init(&visual, size);

  unsigned short move_axis = 0;
  unsigned short move_slice = 0;
  unsigned short move_show = 1;

  unsigned short text_column = (4 + 3*size) * CUBIE_SIZE_X1 + 3;

  mvaddstr(CUBIE_SIZE_Y1 + 0, text_column, "[1] Select axis");
  mvaddstr(CUBIE_SIZE_Y1 + 1, text_column, "[2] Select slice");
  mvaddstr(CUBIE_SIZE_Y1 + 2, text_column, "[8] Rotate forward");
  mvaddstr(CUBIE_SIZE_Y1 + 3, text_column, "[9] Rotate backward");
  mvaddstr(CUBIE_SIZE_Y1 + 4, text_column, "[Q] Quit");

  for (;;) {
    if (move_show) {
      rubik_copy(&visual, &rubik);
      rubik_selection(&visual, move_axis, move_slice, 1);
      rubik_draw(&visual, stdscr);
    }
    else {
      rubik_draw(&rubik, stdscr);
    }

    int ch = getch();

    if (ch == 'q') {
      break;
    }

    else if (ch == '0') {
      move_show = !move_show;
    }

    else if (ch == '1') {
      move_axis = (move_axis + 1) % 3;
    }
    else if (ch == '2') {
      move_slice = (move_slice + 1) % size;
    }
    else if (ch == '8') {
      RUBIK_IN_PLACE(&rubik, rubik_rotated_slice, ROTATORS[move_axis][1], move_axis, move_slice);
    }
    else if (ch == '9') {
      RUBIK_IN_PLACE(&rubik, rubik_rotated_slice, ROTATORS[move_axis][0], move_axis, move_slice);
    }

    else if (ch == 'x') {
      RUBIK_IN_PLACE(&rubik, rubik_rotated, ROTATORS[CDL_DIR_X][0]);
    }
    else if (ch == 'X') {
      RUBIK_IN_PLACE(&rubik, rubik_rotated, ROTATORS[CDL_DIR_X][1]);
    }
    else if (ch == 'y') {
      RUBIK_IN_PLACE(&rubik, rubik_rotated, ROTATORS[CDL_DIR_Z][1]);
    }
    else if (ch == 'Y') {
      RUBIK_IN_PLACE(&rubik, rubik_rotated, ROTATORS[CDL_DIR_Z][0]);
    }
    else if (ch == 'z') {
      RUBIK_IN_PLACE(&rubik, rubik_rotated, ROTATORS[CDL_DIR_Y][1]);
    }
    else if (ch == 'Z') {
      RUBIK_IN_PLACE(&rubik, rubik_rotated, ROTATORS[CDL_DIR_Y][0]);
    }

    else if (ch == 'u') {
      RUBIK_IN_PLACE(&rubik, rubik_rotated_slice, ROTATORS[CDL_DIR_Z][1], CDL_DIR_Z, 0);
    }
    else if (ch == 'U') {
      RUBIK_IN_PLACE(&rubik, rubik_rotated_slice, ROTATORS[CDL_DIR_Z][0], CDL_DIR_Z, 0);
    }
    else if (ch == 'e') {
      RUBIK_IN_PLACE(&rubik, rubik_rotated_slice, ROTATORS[CDL_DIR_Z][0], CDL_DIR_Z, 1);
    }
    else if (ch == 'E') {
      RUBIK_IN_PLACE(&rubik, rubik_rotated_slice, ROTATORS[CDL_DIR_Z][1], CDL_DIR_Z, 1);
    }
    else if (ch == 'd') {
      RUBIK_IN_PLACE(&rubik, rubik_rotated_slice, ROTATORS[CDL_DIR_Z][0], CDL_DIR_Z, size - 1);
    }
    else if (ch == 'D') {
      RUBIK_IN_PLACE(&rubik, rubik_rotated_slice, ROTATORS[CDL_DIR_Z][1], CDL_DIR_Z, size - 1);
    }

    else if (ch == 'l') {
      RUBIK_IN_PLACE(&rubik, rubik_rotated_slice, ROTATORS[CDL_DIR_X][1], CDL_DIR_X, 0);
    }
    else if (ch == 'L') {
      RUBIK_IN_PLACE(&rubik, rubik_rotated_slice, ROTATORS[CDL_DIR_X][0], CDL_DIR_X, 0);
    }
    else if (ch == 'm') {
      RUBIK_IN_PLACE(&rubik, rubik_rotated_slice, ROTATORS[CDL_DIR_X][1], CDL_DIR_X, 1);
    }
    else if (ch == 'M') {
      RUBIK_IN_PLACE(&rubik, rubik_rotated_slice, ROTATORS[CDL_DIR_X][0], CDL_DIR_X, 1);
    }
    else if (ch == 'r') {
      RUBIK_IN_PLACE(&rubik, rubik_rotated_slice, ROTATORS[CDL_DIR_X][0], CDL_DIR_X, size - 1);
    }
    else if (ch == 'R') {
      RUBIK_IN_PLACE(&rubik, rubik_rotated_slice, ROTATORS[CDL_DIR_X][1], CDL_DIR_X, size - 1);
    }

    else if (ch == 'b') {
      RUBIK_IN_PLACE(&rubik, rubik_rotated_slice, ROTATORS[CDL_DIR_Y][0], CDL_DIR_Y, 0);
    }
    else if (ch == 'B') {
      RUBIK_IN_PLACE(&rubik, rubik_rotated_slice, ROTATORS[CDL_DIR_Y][1], CDL_DIR_Y, 0);
    }
    else if (ch == 's') {
      RUBIK_IN_PLACE(&rubik, rubik_rotated_slice, ROTATORS[CDL_DIR_Y][1], CDL_DIR_Y, 1);
    }
    else if (ch == 'S') {
      RUBIK_IN_PLACE(&rubik, rubik_rotated_slice, ROTATORS[CDL_DIR_Y][0], CDL_DIR_Y, 1);
    }
    else if (ch == 'f') {
      RUBIK_IN_PLACE(&rubik, rubik_rotated_slice, ROTATORS[CDL_DIR_Y][1], CDL_DIR_Y, size - 1);
    }
    else if (ch == 'F') {
      RUBIK_IN_PLACE(&rubik, rubik_rotated_slice, ROTATORS[CDL_DIR_Y][0], CDL_DIR_Y, size - 1);
    }
  }

  rubik_free(&visual);
  rubik_free(&rubik);
}
