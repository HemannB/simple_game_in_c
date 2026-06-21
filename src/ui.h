#ifndef UI_H
#define UI_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#define FONT_PATH "/usr/share/fonts/truetype/ubuntu/UbuntuMono-R.ttf"
#define FONT_SIZE 20

typedef struct {
    TTF_Font* font;
} UI;

int ui_init(UI *ui);
void ui_destroy(UI *ui);
void ui_draw_text(const UI *ui, SDL_Renderer *renderer,
                  const char *text, int x, int y, SDL_Color color);

#endif //TETRIS_UI_H
