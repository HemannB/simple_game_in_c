#ifndef GRID_H
#define GRID_H

#include <SDL2/SDL.h>

#define GRID_COLS       10
#define GRID_ROWS       20
#define CELL_SIZE       20
#define GRID_X_OFFSET   20
#define GRID_Y_OFFSET   20

typedef struct {
    int cells[GRID_ROWS][GRID_COLS];
} Grid;

void grid_init(Grid *g);
void grid_render(const Grid *g, SDL_Renderer *renderer);

int grid_clear_lines(Grid *g);

#endif /* GRID_H */