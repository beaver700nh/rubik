#include <curses.h>

#include <stdio.h>
#include <stdlib.h>

#include "rubik.h"

int main(int argc, char *argv[]) {
  if (argc != 2) {
    puts("Usage: rubik <size>");
    return EXIT_FAILURE;
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
      init_pair(i, i, COLOR_BLACK);
      init_pair(i + 8, i, i);
    }
  }

  main_loop(atoi(argv[1]));

  nocbreak();
  noecho();
  curs_set(1);
  endwin();

  return EXIT_SUCCESS;
}

void main_loop(unsigned short size) {
  rubik_t rubik;
  rubik_init(&rubik, size);

  for (;;) {
    rubik_draw(&rubik, stdscr);

    int ch = getch();

    if (ch == 'q') {
      break;
    }

    if (ch == 'x') {
      rubik_in_place(&rubik, &rubik_rotated, ROTATORS[CDL_DIR_X][0], 0);
    }
    if (ch == 'X') {
      rubik_in_place(&rubik, &rubik_rotated, ROTATORS[CDL_DIR_X][1], 0);
    }
    if (ch == 'y') {
      rubik_in_place(&rubik, &rubik_rotated, ROTATORS[CDL_DIR_Z][1], 0);
    }
    if (ch == 'Y') {
      rubik_in_place(&rubik, &rubik_rotated, ROTATORS[CDL_DIR_Z][0], 0);
    }
    if (ch == 'z') {
      rubik_in_place(&rubik, &rubik_rotated, ROTATORS[CDL_DIR_Y][1], 0);
    }
    if (ch == 'Z') {
      rubik_in_place(&rubik, &rubik_rotated, ROTATORS[CDL_DIR_Y][0], 0);
    }

    if (ch == 'u') {
      rubik_in_place(&rubik, &rubik_rotated_slice_z, ROTATORS[CDL_DIR_Z][1], 0);
    }
    if (ch == 'U') {
      rubik_in_place(&rubik, &rubik_rotated_slice_z, ROTATORS[CDL_DIR_Z][0], 0);
    }
    if (ch == 'e') {
      rubik_in_place(&rubik, &rubik_rotated_slice_z, ROTATORS[CDL_DIR_Z][0], 1);
    }
    if (ch == 'E') {
      rubik_in_place(&rubik, &rubik_rotated_slice_z, ROTATORS[CDL_DIR_Z][1], 1);
    }
    if (ch == 'd') {
      rubik_in_place(&rubik, &rubik_rotated_slice_z, ROTATORS[CDL_DIR_Z][0], 2);
    }
    if (ch == 'D') {
      rubik_in_place(&rubik, &rubik_rotated_slice_z, ROTATORS[CDL_DIR_Z][1], 2);
    }

    if (ch == 'l') {
      rubik_in_place(&rubik, &rubik_rotated_slice_x, ROTATORS[CDL_DIR_X][1], 0);
    }
    if (ch == 'L') {
      rubik_in_place(&rubik, &rubik_rotated_slice_x, ROTATORS[CDL_DIR_X][0], 0);
    }
    if (ch == 'm') {
      rubik_in_place(&rubik, &rubik_rotated_slice_x, ROTATORS[CDL_DIR_X][1], 1);
    }
    if (ch == 'M') {
      rubik_in_place(&rubik, &rubik_rotated_slice_x, ROTATORS[CDL_DIR_X][0], 1);
    }
    if (ch == 'r') {
      rubik_in_place(&rubik, &rubik_rotated_slice_x, ROTATORS[CDL_DIR_X][0], 2);
    }
    if (ch == 'R') {
      rubik_in_place(&rubik, &rubik_rotated_slice_x, ROTATORS[CDL_DIR_X][1], 2);
    }

    if (ch == 'b') {
      rubik_in_place(&rubik, &rubik_rotated_slice_y, ROTATORS[CDL_DIR_Y][0], 0);
    }
    if (ch == 'B') {
      rubik_in_place(&rubik, &rubik_rotated_slice_y, ROTATORS[CDL_DIR_Y][1], 0);
    }
    if (ch == 's') {
      rubik_in_place(&rubik, &rubik_rotated_slice_y, ROTATORS[CDL_DIR_Y][1], 1);
    }
    if (ch == 'S') {
      rubik_in_place(&rubik, &rubik_rotated_slice_y, ROTATORS[CDL_DIR_Y][0], 1);
    }
    if (ch == 'f') {
      rubik_in_place(&rubik, &rubik_rotated_slice_y, ROTATORS[CDL_DIR_Y][1], 2);
    }
    if (ch == 'F') {
      rubik_in_place(&rubik, &rubik_rotated_slice_y, ROTATORS[CDL_DIR_Y][0], 2);
    }
  }

  rubik_free(&rubik);
}
