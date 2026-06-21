#include "tetromino.h"
#include <stdlib.h>
#include <time.h>

const int PIECES[NUM_PIECES][NUM_ROTATIONS][PIECE_SIZE][PIECE_SIZE] = {
  //I
  {
    {{0,0,0,0},{1,1,1,1},{0,0,0,0},{0,0,0,0} },
    {{0,0,1,0},{0,0,1,0},{0,0,1,0},{0,0,1,0} },
    {{0,0,0,0},{0,0,0,0},{1,1,1,1},{0,0,0,0} },
    {{0,1,0,0},{0,1,0,0},{0,1,0,0},{0,1,0,0} },
  },
  //O
  {
    {{0,1,1,0},{0,1,1,0},{0,0,0,0},{0,0,0,0} },
    {{0,1,1,0},{0,1,1,0},{0,0,0,0},{0,0,0,0} },
    {{0,1,1,0},{0,1,1,0},{0,0,0,0},{0,0,0,0} },
    {{0,1,1,0},{0,1,1,0},{0,0,0,0},{0,0,0,0} },
  },
  //T
  {
    { {0,1,0,0}, {1,1,1,0}, {0,0,0,0}, {0,0,0,0} },
    { {0,1,0,0}, {0,1,1,0}, {0,1,0,0}, {0,0,0,0} },
    { {0,0,0,0}, {1,1,1,0}, {0,1,0,0}, {0,0,0,0} },
    { {0,1,0,0}, {1,1,0,0}, {0,1,0,0}, {0,0,0,0} },
  },
  //S
  {
    { {0,1,1,0}, {1,1,0,0}, {0,0,0,0}, {0,0,0,0} },
    { {0,1,0,0}, {0,1,1,0}, {0,0,1,0}, {0,0,0,0} },
    { {0,0,0,0}, {0,1,1,0}, {1,1,0,0}, {0,0,0,0} },
    { {1,0,0,0}, {1,1,0,0}, {0,1,0,0}, {0,0,0,0} },
  },
  //Z
  {
    { {1,1,0,0}, {0,1,1,0}, {0,0,0,0}, {0,0,0,0} },
    { {0,0,1,0}, {0,1,1,0}, {0,1,0,0}, {0,0,0,0} },
    { {0,0,0,0}, {1,1,0,0}, {0,1,1,0}, {0,0,0,0} },
    { {0,1,0,0}, {1,1,0,0}, {1,0,0,0}, {0,0,0,0} },
  },
  //J
  {
    { {1,0,0,0}, {1,1,1,0}, {0,0,0,0}, {0,0,0,0} },
    { {0,1,1,0}, {0,1,0,0}, {0,1,0,0}, {0,0,0,0} },
    { {0,0,0,0}, {1,1,1,0}, {0,0,1,0}, {0,0,0,0} },
    { {0,1,0,0}, {0,1,0,0}, {1,1,0,0}, {0,0,0,0} },
  },
  //L
  {
    { {0,0,1,0}, {1,1,1,0}, {0,0,0,0}, {0,0,0,0} },
    { {0,1,0,0}, {0,1,0,0}, {0,1,1,0}, {0,0,0,0} },
    { {0,0,0,0}, {1,1,1,0}, {1,0,0,0}, {0,0,0,0} },
    { {1,1,0,0}, {0,1,0,0}, {0,1,0,0}, {0,0,0,0} },
  },
};

const SDL_Color PIECE_COLORS[NUM_PIECES] = {
  {0,240,240,255},  //Cian
  {240,240,0,255},  //Yellow
  {160,0,240,255},  //Purple
  {0,240,0,255},    //Green
  {240,0,0,255},    //Red
  {0,0,240,255},    //Blue
  {240,160,0,255},  //Orange
};

void tetromino_spawn(Tetromino *t) {
  t->piece_idx  = rand() % NUM_PIECES;
  t->rotation   = 0;
  t->col        = GRID_COLS / 2 - PIECE_SIZE / 2;
  t->row        = 0;
}

void tetromino_render(const Tetromino *t, SDL_Renderer *renderer) {
  const SDL_Color *c = &PIECE_COLORS[t->piece_idx];
  for (int r = 0; r < PIECE_SIZE; r++) {
    for (int col = 0; col < PIECE_SIZE; col++) {
      if (!PIECES[t->piece_idx][t->rotation][r][col])
        continue;
      int x = GRID_X_OFFSET + (t->col + col) * CELL_SIZE;
      int y = GRID_Y_OFFSET + (t->row + r) * CELL_SIZE;

      SDL_Rect cell = {x , y , CELL_SIZE, CELL_SIZE};

      SDL_SetRenderDrawColor(renderer, c->r, c->g, c->b, c->a);
      SDL_RenderFillRect(renderer, &cell);

      SDL_SetRenderDrawColor(renderer,
        (Uint8)(c->r * 0.6f),
        (Uint8)(c->g * 0.6f),
        (Uint8)(c->b * 0.6f),
        255);
      SDL_RenderDrawRect(renderer, &cell);
    }
  }
}

int tetromino_is_valid(const Tetromino *t, const Grid *g, int row, int col, int rotation){
  for (int r = 0; r < PIECE_SIZE; r++) {
    for (int c = 0; c < PIECE_SIZE; c++) {
      if (!PIECES[t->piece_idx][rotation][r][c])
        continue;
      int gr = row + r;
      int gc = col + c;
      if (gc < 0 || gc >= GRID_COLS || gr >= GRID_ROWS)
        return 0;
      if (gr < 0)
        continue;
      if (g->cells[gr][gc] != 0)
        return 0;
    }
  }
  return 1;
}

void tetromino_move_left(Tetromino *t, const Grid *g) {
  if (tetromino_is_valid(t, g, t->row, t->col - 1, t->rotation))
    t->col--;
}
void tetromino_move_right(Tetromino *t, const Grid *g) {
  if (tetromino_is_valid(t, g, t->row, t->col + 1, t->rotation))
    t->col++;
}
int tetromino_move_down(Tetromino *t, const Grid *g) {
  if (tetromino_is_valid(t, g, t->row + 1, t->col, t->rotation)) {
    t->row++;
    return 1;
  }
  return 0;
}
void tetromino_rotate(Tetromino *t, const Grid *g) {
  int next_rot = (t->rotation + 1) % NUM_ROTATIONS;
  if (tetromino_is_valid(t,g,t->row, t->col, next_rot)) {
    t->rotation = next_rot;
    return;
  }
  int kicks[] = { 1, -1, 2, -2};
  for (int i = 0; i < NUM_ROTATIONS; i++) {
    if (tetromino_is_valid(t,g,t->row, t->col + kicks[i], next_rot)) {
      t->col      += kicks[i];
      t->rotation  = next_rot;
      return;
    }
  }
}

void tetromino_lock(const Tetromino *t, Grid *g) {
  for (int r = 0; r < PIECE_SIZE; r++) {
    for (int c = 0; c < PIECE_SIZE; c++) {
      if (!PIECES[t->piece_idx][t->rotation][r][c])
        continue;
      int gr = t->row + r;
      int gc = t->col + c;

      if (gr >= 0 && gr < GRID_ROWS && gc >= 0 && gc < GRID_COLS)
        g->cells[gr][gc] = t->piece_idx + 1;
    }
  }
}
