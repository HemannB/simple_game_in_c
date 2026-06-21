#include "ui.h"
#include <stdio.h>

int ui_init(UI *ui) {
    if (TTF_Init() != 0) {
        fprintf(stderr, "TTF_Init falhou %s\n", TTF_GetError());
        return 0;
    }
    ui->font = TTF_OpenFont(FONT_PATH, FONT_SIZE);
    if (!ui->font) {
        fprintf(stderr, "TTF_OpenFont falhou %s\n", TTF_GetError());
        TTF_Quit();
        return 0;
    }
    return 1;
}

void ui_destroy(UI *ui)
{
    if (ui->font)
        TTF_CloseFont(ui->font);
    TTF_Quit();
}

void ui_draw_text(const UI *ui, SDL_Renderer *renderer,
    const char *text, int x, int y, SDL_Color color) {
    SDL_Surface *surface = TTF_RenderText_Blended(ui->font, text, color);
    if (!surface)
        return;

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    if (!texture)
        return;

    int w, h;
    SDL_QueryTexture(texture, NULL, NULL, &w, &h);
    SDL_Rect dst = { x, y, w, h };
    SDL_RenderCopy(renderer, texture, NULL, &dst);
    SDL_DestroyTexture(texture);
}
