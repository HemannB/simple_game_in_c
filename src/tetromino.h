#ifndef TETROMINO_H
#define TETROMINO_H

#include <SDL2/SDL.h>
#include "grid.h"

#define NUM_PIECES      7
#define NUM_ROTATIONS   4
#define PIECE_SIZE      4

extern const int PIECES[NUM_PIECES][NUM_ROTATIONS][PIECE_SIZE][PIECE_SIZE];
extern const SDL_Color PIECE_COLORS[NUM_PIECES];

typedef struct {
    int piece_idx;
    int rotation;
    int col;
    int row;
} Tetromino;

void tetromino_spawn(Tetromino *t);
void tetromino_render(const Tetromino *t, SDL_Renderer *renderer);

int tetromino_is_valid(const Tetromino *t, const Grid *g,
    int row, int col, int rotation);
void tetromino_move_left  (Tetromino *t, const Grid *g);
void tetromino_move_right (Tetromino *t, const Grid *g);
int tetromino_move_down  (Tetromino *t, const Grid *g);
void tetromino_rotate     (Tetromino *t, const Grid *g);
void tetromino_lock       (const Tetromino *t, Grid *g);
#endif //TETROMINO_H
