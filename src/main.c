#include <curses.h>

#include "rubik.h"

int main(void) {
  void main_loop(void);

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

  main_loop();

  nocbreak();
  noecho();
  curs_set(1);
  endwin();
}

void main_loop(void) {
  rubik_t rubik;
  rubik_init(&rubik, 3);

  for (;;) {
    rubik_draw(&rubik, stdscr);

    int ch = getch();

    if (ch == 'q') {
      break;
    }

    if (ch == 'x') {
      rubik_rotate_ip(&rubik, ROTATORS[CDL_DIR_X][0]);
    }
    if (ch == 'X') {
      rubik_rotate_ip(&rubik, ROTATORS[CDL_DIR_X][1]);
    }
    if (ch == 'y') {
      rubik_rotate_ip(&rubik, ROTATORS[CDL_DIR_Z][1]);
    }
    if (ch == 'Y') {
      rubik_rotate_ip(&rubik, ROTATORS[CDL_DIR_Z][0]);
    }
    if (ch == 'z') {
      rubik_rotate_ip(&rubik, ROTATORS[CDL_DIR_Y][1]);
    }
    if (ch == 'Z') {
      rubik_rotate_ip(&rubik, ROTATORS[CDL_DIR_Y][0]);
    }
  }

  rubik_free(&rubik);
}
