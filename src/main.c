#include <curses.h>

#include <stdio.h>
#include <stdlib.h>

#include "rubik.h"

unsigned short CUBIE_SIZE_X0 = 4;
unsigned short CUBIE_SIZE_X1 = 6;
unsigned short CUBIE_SIZE_Y0 = 2;
unsigned short CUBIE_SIZE_Y1 = 3;

int main(int argc, char *argv[]) {
  if (argc < 2) {
    puts("Usage: rubik <size> [cubie_size_x0 cubie_size_x1 cubie_size_y0 cubie_size_y1]");
    return EXIT_FAILURE;
  }

  if (argc >= 6) {
    CUBIE_SIZE_X0 = atoi(argv[2]);
    CUBIE_SIZE_X1 = atoi(argv[3]);
    CUBIE_SIZE_Y0 = atoi(argv[4]);
    CUBIE_SIZE_Y1 = atoi(argv[5]);
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

  unsigned short move_axis = 0;
  unsigned short move_sign = 0;
  unsigned short move_slice = 0;

  unsigned short text_column = (1 + 3*size) * CUBIE_SIZE_X1 + 3;

  mvaddstr(CUBIE_SIZE_Y1 + 0, text_column, "Press [F4] to rotate:");
  mvaddstr(CUBIE_SIZE_Y1 + 1, text_column, "Slice                           axis");
  mvaddstr(CUBIE_SIZE_Y1 + 2, text_column, "   [F3] [F2]                [F1]");

  for (;;) {
    rubik_draw(&rubik, stdscr);

    attron(A_BOLD);
    mvprintw(
      CUBIE_SIZE_Y1 + 1, text_column + 6, "%d %s                  %c",
      move_slice, move_sign == 0 ? "nega" : "posi", "XZY"[move_axis]
    );
    attroff(A_BOLD);
    mvaddstr(CUBIE_SIZE_Y1 + 1, text_column + 12, "tively around the");

    int ch = getch();

    if (ch == 'q') {
      break;
    }

    else if (ch == KEY_F(1)) {
      move_axis = (move_axis + 1) % 3;
    }
    else if (ch == KEY_F(2)) {
      move_sign = (move_sign + 1) % 2;
    }
    else if (ch == KEY_F(3)) {
      move_slice = (move_slice + 1) % size;
    }
    else if (ch == KEY_F(4)) {
      rubik_t (*operator)(rubik_t *, rubik_rotator_t, unsigned short);
      unsigned short sign;

      if (move_axis == 0) {
        operator = &rubik_rotated_slice_x;
        sign = move_sign;
      }
      else if (move_axis == 1) {
        operator = &rubik_rotated_slice_y;
        sign = move_sign;
      }
      else if (move_axis == 2) {
        operator = &rubik_rotated_slice_z;
        sign = !move_sign;
      }

      rubik_in_place(&rubik, operator, ROTATORS[move_axis][sign], move_slice);
    }

    else if (ch == 'x') {
      rubik_in_place(&rubik, &rubik_rotated, ROTATORS[CDL_DIR_X][0], 0);
    }
    else if (ch == 'X') {
      rubik_in_place(&rubik, &rubik_rotated, ROTATORS[CDL_DIR_X][1], 0);
    }
    else if (ch == 'y') {
      rubik_in_place(&rubik, &rubik_rotated, ROTATORS[CDL_DIR_Z][1], 0);
    }
    else if (ch == 'Y') {
      rubik_in_place(&rubik, &rubik_rotated, ROTATORS[CDL_DIR_Z][0], 0);
    }
    else if (ch == 'z') {
      rubik_in_place(&rubik, &rubik_rotated, ROTATORS[CDL_DIR_Y][1], 0);
    }
    else if (ch == 'Z') {
      rubik_in_place(&rubik, &rubik_rotated, ROTATORS[CDL_DIR_Y][0], 0);
    }

    else if (ch == 'u') {
      rubik_in_place(&rubik, &rubik_rotated_slice_z, ROTATORS[CDL_DIR_Z][1], 0);
    }
    else if (ch == 'U') {
      rubik_in_place(&rubik, &rubik_rotated_slice_z, ROTATORS[CDL_DIR_Z][0], 0);
    }
    else if (ch == 'e') {
      rubik_in_place(&rubik, &rubik_rotated_slice_z, ROTATORS[CDL_DIR_Z][0], 1);
    }
    else if (ch == 'E') {
      rubik_in_place(&rubik, &rubik_rotated_slice_z, ROTATORS[CDL_DIR_Z][1], 1);
    }
    else if (ch == 'd') {
      rubik_in_place(&rubik, &rubik_rotated_slice_z, ROTATORS[CDL_DIR_Z][0], size - 1);
    }
    else if (ch == 'D') {
      rubik_in_place(&rubik, &rubik_rotated_slice_z, ROTATORS[CDL_DIR_Z][1], size - 1);
    }

    else if (ch == 'l') {
      rubik_in_place(&rubik, &rubik_rotated_slice_x, ROTATORS[CDL_DIR_X][1], 0);
    }
    else if (ch == 'L') {
      rubik_in_place(&rubik, &rubik_rotated_slice_x, ROTATORS[CDL_DIR_X][0], 0);
    }
    else if (ch == 'm') {
      rubik_in_place(&rubik, &rubik_rotated_slice_x, ROTATORS[CDL_DIR_X][1], 1);
    }
    else if (ch == 'M') {
      rubik_in_place(&rubik, &rubik_rotated_slice_x, ROTATORS[CDL_DIR_X][0], 1);
    }
    else if (ch == 'r') {
      rubik_in_place(&rubik, &rubik_rotated_slice_x, ROTATORS[CDL_DIR_X][0], size - 1);
    }
    else if (ch == 'R') {
      rubik_in_place(&rubik, &rubik_rotated_slice_x, ROTATORS[CDL_DIR_X][1], size - 1);
    }

    else if (ch == 'b') {
      rubik_in_place(&rubik, &rubik_rotated_slice_y, ROTATORS[CDL_DIR_Y][0], 0);
    }
    else if (ch == 'B') {
      rubik_in_place(&rubik, &rubik_rotated_slice_y, ROTATORS[CDL_DIR_Y][1], 0);
    }
    else if (ch == 's') {
      rubik_in_place(&rubik, &rubik_rotated_slice_y, ROTATORS[CDL_DIR_Y][1], 1);
    }
    else if (ch == 'S') {
      rubik_in_place(&rubik, &rubik_rotated_slice_y, ROTATORS[CDL_DIR_Y][0], 1);
    }
    else if (ch == 'f') {
      rubik_in_place(&rubik, &rubik_rotated_slice_y, ROTATORS[CDL_DIR_Y][1], size - 1);
    }
    else if (ch == 'F') {
      rubik_in_place(&rubik, &rubik_rotated_slice_y, ROTATORS[CDL_DIR_Y][0], size - 1);
    }
  }

  rubik_free(&rubik);
}
