#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include "grid.h"
#include "tetromino.h"

// DEFINIÇÕES DO GAME
#define WINDOW_TITLE     "TETRIS"
#define WINDOW_WIDTH     (GRID_X_OFFSET * 2 + GRID_COLS * CELL_SIZE)
#define WINDOW_HEIGHT    (GRID_Y_OFFSET * 2 + GRID_ROWS * CELL_SIZE)
#define TARGET_FPS       60
#define MS_PER_FRAME     (1000 / TARGET_FPS)
#define GRAVITY_INTERVAL 1.0f

// GAME STATE
typedef struct {
    int         running;
    Grid        grid;
    Tetromino   current;
    float       gravity_acc;
}GameState;

// MANIPULADOR DE EVENTOS
static void handle_events(GameState *gs) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT)
            gs->running = 0;

        if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym) {
                case SDLK_ESCAPE:
                    gs->running = 0;
                break;
                case SDLK_LEFT:
                    tetromino_move_left(&gs->current, &gs->grid);
                break;
                case SDLK_RIGHT:
                    tetromino_move_right(&gs->current, &gs->grid);
                break;
                case SDLK_DOWN:
                    tetromino_move_down(&gs->current, &gs->grid);
                    gs->gravity_acc = 0.0f;
                break;
                case SDLK_UP:
                case SDLK_z:
                    tetromino_rotate (&gs->current, &gs->grid);
                break;
                default:
                break;
            }
        }
    }
}

// UPDATE
static void update(GameState *gs, float dt) {
    gs->gravity_acc += dt;
    if (gs->gravity_acc >= GRAVITY_INTERVAL) {
        tetromino_move_down(&gs->current, &gs->grid);
        gs->gravity_acc = 0.0f;
    }
}

// RENDER
static void render(SDL_Renderer *renderer, const GameState *gs) {
    SDL_SetRenderDrawColor(renderer, 15, 15, 20, 255);
    SDL_RenderClear(renderer);

    grid_render(&gs->grid, renderer);
    tetromino_render(&gs->current, renderer);

    SDL_RenderPresent(renderer);
}

// MAIN GAME
int main(void) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "SDL_Init falhou %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    SDL_Window *window = SDL_CreateWindow(
        WINDOW_TITLE,
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH, WINDOW_HEIGHT,
        SDL_WINDOW_SHOWN
    );
    if (!window) {
        fprintf(stderr, "SDL_CreateWindow falhou %s\n", SDL_GetError());
        SDL_Quit();
        return EXIT_FAILURE;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer (
        window, -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
        );
    if (!renderer) {
        fprintf(stderr, "SDL_CreateRenderer falhou %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return EXIT_FAILURE;
    }
    GameState gs = { .running = 1};
    grid_init(&gs.grid);
    tetromino_spawn(&gs.current);

    Uint64 last = SDL_GetTicks64();

    while (gs.running) {
        Uint64 now          = SDL_GetTicks64();
        Uint64 elapsed      = now - last;
        float dt            = (float)elapsed / 1000.0f;
        last                = now;

        handle_events (&gs);
        update(&gs, dt);
        render(renderer, &gs);

        Uint64 frame_time = SDL_GetTicks64() - now;
        if (frame_time < MS_PER_FRAME)
            SDL_Delay((Uint32)(MS_PER_FRAME - frame_time));
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return EXIT_SUCCESS;
}