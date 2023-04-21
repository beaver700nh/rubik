#include <curses.h>

#define RUBIK_DIMS 3

typedef enum {
  CDL_DIR_X,
  CDL_DIR_Y,
  CDL_DIR_Z,
} cdl_dir_t;

typedef union {
  struct {
    unsigned short x;
    unsigned short y;
    unsigned short z;
  };
  unsigned short buf[3];
} vec3_t;

unsigned short vec3_to_index(vec3_t *vec3, unsigned short size);

typedef struct {
  short color;
  short sign;
} facie_t;

void facie_init(facie_t *facie, short color, short sign);

typedef struct {
  facie_t facies[RUBIK_DIMS];
} cubie_t;

void cubie_init(cubie_t *cubie, unsigned short size, vec3_t pos);

typedef struct {
  unsigned short size;
  cubie_t *cubies; // 3D array [x][y][z]
} rubik_t;

void rubik_init(rubik_t *rubik, unsigned short size);
cubie_t *rubik_get(rubik_t *rubik, vec3_t pos);
void rubik_draw(rubik_t *rubik, WINDOW *window, cdl_dir_t top, cdl_dir_t back);
void rubik_dump(rubik_t *rubik, WINDOW *window, unsigned short position);
void rubik_free(rubik_t *rubik);
