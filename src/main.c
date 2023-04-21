#include <curses.h>

#include "calc.h"

int main(void) {
  void main_loop(void);

  initscr();
  noecho();
  keypad(stdscr, 1);
  curs_set(0);

  if (has_colors()) {
    start_color();
    use_default_colors();

    for (unsigned short i = 0; i < 8; ++i) {
      init_pair(i, i, -1);
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

  rubik_draw(&rubik, stdscr, CDL_DIR_Z, CDL_DIR_Y);

  getch();

  rubik_free(&rubik);
}
