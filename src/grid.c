#include "grid.h"
#include "tetromino.h"
#include "string.h"

void grid_init(Grid *g)
{
    memset(g->cells, 0, sizeof(g->cells));
}

void grid_render(const Grid *g, SDL_Renderer *renderer)
{
    for (int row = 0; row < GRID_ROWS; row++) {
        for (int col = 0; col < GRID_COLS; col++) {
            int x = GRID_X_OFFSET + col * CELL_SIZE;
            int y = GRID_Y_OFFSET + row * CELL_SIZE;

            SDL_Rect cell = { x, y, CELL_SIZE, CELL_SIZE };

            if (g->cells[row][col] == 0) {
                SDL_SetRenderDrawColor(renderer, 25, 25, 35, 255);
                SDL_RenderFillRect(renderer, &cell);

                SDL_SetRenderDrawColor(renderer, 40, 40, 55, 255);
                SDL_RenderDrawRect(renderer, &cell);
            } else {
                int idx = g->cells[row][col] - 1;

                const SDL_Color *c = &PIECE_COLORS[idx];

                SDL_SetRenderDrawColor(renderer, c->r, c->g, c->b, c->a);
                SDL_RenderFillRect(renderer, &cell);

                SDL_SetRenderDrawColor(renderer,
                    (Uint8)(c->r * 0.6f),
                    (Uint8)(c->g * 0.06f),
                    (Uint8)(c->b * 0.06f),
                    255);
                SDL_RenderDrawRect(renderer, &cell);
            }
        }
    }

    SDL_Rect border = {
        GRID_X_OFFSET - 1,
        GRID_Y_OFFSET - 1,
        GRID_COLS * CELL_SIZE + 2,
        GRID_ROWS * CELL_SIZE + 2
    };
    SDL_SetRenderDrawColor(renderer, 100, 100, 120, 255);
    SDL_RenderDrawRect(renderer, &border);
}