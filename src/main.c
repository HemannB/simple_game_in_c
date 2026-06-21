#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "grid.h"
#include "tetromino.h"
#include "ui.h"
#include "audio.h"

// DEFINIÇÕES DO GAME
#define WINDOW_TITLE  "TETRIS"
#define WINDOW_WIDTH  (GRID_X_OFFSET * 2 + GRID_COLS * CELL_SIZE + 160)
#define WINDOW_HEIGHT (GRID_Y_OFFSET * 2 + GRID_ROWS * CELL_SIZE)
#define TARGET_FPS    60
#define MS_PER_FRAME  (1000 / TARGET_FPS)

// Score table: pontos para 1, 2, 3, 4 linhas (Tetris original Nintendo)
static const int SCORE_TABLE[5] = { 0, 40, 100, 300, 1200 };

// Intervalo de gravidade por nível (segundos)
static float gravity_for_level(int level)
{
    if (level >= 10) return 0.05f;
    float intervals[] = {
        1.00f, 0.85f, 0.72f, 0.60f, 0.50f,
        0.42f, 0.35f, 0.28f, 0.22f, 0.17f
    };
    return intervals[level];
}

// GAME STATE
typedef struct {
    int       running;
    Grid      grid;
    Tetromino current;
    float     gravity_acc;
    int       score;
    int       level;
    int       lines_cleared;
} GameState;

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
                    tetromino_rotate(&gs->current, &gs->grid);
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

    if (gs->gravity_acc >= gravity_for_level(gs->level)) {
        gs->gravity_acc = 0.0f;

        if (!tetromino_move_down(&gs->current, &gs->grid)) {
            tetromino_lock(&gs->current, &gs->grid);

            int cleared = grid_clear_lines(&gs->grid);
            if (cleared > 0) {
                gs->score         += SCORE_TABLE[cleared] * (gs->level + 1);
                gs->lines_cleared += cleared;
                gs->level          = gs->lines_cleared / 10;
            }

            tetromino_spawn(&gs->current);
        }
    }
}

// RENDER
static void render(SDL_Renderer *renderer, const GameState *gs, const UI *ui) {
    SDL_SetRenderDrawColor(renderer, 15, 15, 20, 255);
    SDL_RenderClear(renderer);

    grid_render(&gs->grid, renderer);
    tetromino_render(&gs->current, renderer);

    // sidebar
    int sx = GRID_X_OFFSET * 2 + GRID_COLS * CELL_SIZE + 10;
    SDL_Color white  = { 255, 255, 255, 255 };
    SDL_Color yellow = { 240, 240,   0, 255 };

    char buf[64];

    ui_draw_text(ui, renderer, "SCORE",  sx, 30,  white);
    snprintf(buf, sizeof(buf), "%d", gs->score);
    ui_draw_text(ui, renderer, buf,      sx, 55,  yellow);

    ui_draw_text(ui, renderer, "LEVEL",  sx, 100, white);
    snprintf(buf, sizeof(buf), "%d", gs->level + 1);
    ui_draw_text(ui, renderer, buf,      sx, 125, yellow);

    ui_draw_text(ui, renderer, "LINES",  sx, 170, white);
    snprintf(buf, sizeof(buf), "%d", gs->lines_cleared);
    ui_draw_text(ui, renderer, buf,      sx, 195, yellow);

    SDL_RenderPresent(renderer);
}

// MAIN GAME
int main(void) {
    srand((unsigned int)time(NULL));

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
        fprintf(stderr, "SDL_Init falhou %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    UI ui;
    if (!ui_init(&ui)) {
        SDL_Quit();
        return EXIT_FAILURE;
    }

    Audio audio;
    if (!audio_init(&audio)) {
        fprintf(stderr, "Audio init falhou, continuando sem música\n");
    } else {
        audio_play_music(&audio);
    }

    SDL_Window *window = SDL_CreateWindow(
        WINDOW_TITLE,
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH, WINDOW_HEIGHT,
        SDL_WINDOW_SHOWN
    );
    if (!window) {
        fprintf(stderr, "SDL_CreateWindow falhou %s\n", SDL_GetError());
        audio_destroy(&audio);
        ui_destroy(&ui);
        SDL_Quit();
        return EXIT_FAILURE;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(
        window, -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );
    if (!renderer) {
        fprintf(stderr, "SDL_CreateRenderer falhou %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        audio_destroy(&audio);
        ui_destroy(&ui);
        SDL_Quit();
        return EXIT_FAILURE;
    }

    GameState gs = {
        .running       = 1,
        .gravity_acc   = 0.0f,
        .score         = 0,
        .level         = 0,
        .lines_cleared = 0
    };
    grid_init(&gs.grid);
    tetromino_spawn(&gs.current);

    Uint64 last = SDL_GetTicks64();

    while (gs.running) {
        Uint64 now        = SDL_GetTicks64();
        Uint64 elapsed    = now - last;
        float  dt         = (float)elapsed / 1000.0f;
        last              = now;

        handle_events(&gs);
        update(&gs, dt);
        render(renderer, &gs, &ui);

        Uint64 frame_time = SDL_GetTicks64() - now;
        if (frame_time < MS_PER_FRAME)
            SDL_Delay((Uint32)(MS_PER_FRAME - frame_time));
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    audio_destroy(&audio);
    ui_destroy(&ui);
    SDL_Quit();
    return EXIT_SUCCESS;
}