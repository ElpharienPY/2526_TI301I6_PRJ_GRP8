//
// Created by hdacc on 11/21/2025.
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// IMPORTANT: stop SDL from redefining main()
#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <SDL_ttf.h>

#include "adj_list.h"
#include "matrix.h"

typedef struct {
    SDL_Window   *window;
    SDL_Renderer *renderer;
    TTF_Font     *font;

    t_matrix M;        // transition matrix (weather Markov chain)
    int nbStates;      // number of states (M.size)
    int currentState;  // today's state (0..nbStates-1)
    int selectedDay;   // day offset from today (0 = today)
} App;

static const char *STATE_NAMES[] = {
    "Sunny", "Cloudy", "Rain", "Storm", "Sunny spells"
};

static int initSDL(App *app)
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "SDL_Init Error: %s\n", SDL_GetError());
        return 0;
    }

    if (TTF_Init() != 0) {
        fprintf(stderr, "TTF_Init Error: %s\n", TTF_GetError());
        SDL_Quit();
        return 0;
    }

    app->window = SDL_CreateWindow(
        "Markov Weather Calendar",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        800, 600,
        SDL_WINDOW_SHOWN
    );

    if (!app->window) {
        fprintf(stderr, "SDL_CreateWindow Error: %s\n", SDL_GetError());
        TTF_Quit();
        SDL_Quit();
        return 0;
    }

    app->renderer = SDL_CreateRenderer(app->window, -1, SDL_RENDERER_ACCELERATED);
    if (!app->renderer) {
        fprintf(stderr, "SDL_CreateRenderer Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(app->window);
        TTF_Quit();
        SDL_Quit();
        return 0;
    }

    // Put arial.ttf inside cmake-build-debug/
    app->font = TTF_OpenFont("arial.ttf", 18);
    if (!app->font) {
        fprintf(stderr, "TTF_OpenFont Error: %s\n", TTF_GetError());
        SDL_DestroyRenderer(app->renderer);
        SDL_DestroyWindow(app->window);
        TTF_Quit();
        SDL_Quit();
        return 0;
    }

    return 1;
}

static void cleanupSDL(App *app)
{
    if (app->font)     TTF_CloseFont(app->font);
    if (app->renderer) SDL_DestroyRenderer(app->renderer);
    if (app->window)   SDL_DestroyWindow(app->window);
    TTF_Quit();
    SDL_Quit();
}

static void renderText(App *app, const char *text, int x, int y)
{
    SDL_Color color = {0, 0, 0, 255};
    SDL_Surface *surface = TTF_RenderText_Blended(app->font, text, color);
    if (!surface) return;

    SDL_Texture *texture = SDL_CreateTextureFromSurface(app->renderer, surface);
    SDL_Rect dst = { x, y, surface->w, surface->h };

    SDL_FreeSurface(surface);
    SDL_RenderCopy(app->renderer, texture, NULL, &dst);
    SDL_DestroyTexture(texture);
}

static int hitTestDay(int mx, int my)
{
    int startX = 50;
    int startY = 100;
    int cellW  = 100;
    int cellH  = 60;
    int cols   = 7;

    for (int d = 0; d < 30; d++) {
        int row = d / cols;
        int col = d % cols;

        int x = startX + col * cellW;
        int y = startY + row * cellH;

        if (mx >= x && mx < x + cellW - 5 &&
            my >= y && my < y + cellH - 5) {
            return d;
        }
    }
    return -1;
}

static void drawCalendar(App *app)
{
    int startX = 50;
    int startY = 100;
    int cellW  = 100;
    int cellH  = 60;
    int cols   = 7;

    SDL_SetRenderDrawColor(app->renderer, 0, 0, 0, 255);

    for (int d = 0; d < 30; d++) {
        int row = d / cols;
        int col = d % cols;

        SDL_Rect r = {
            startX + col * cellW,
            startY + row * cellH,
            cellW - 5,
            cellH - 5
        };

        if (d == app->selectedDay) {
            SDL_SetRenderDrawColor(app->renderer, 180, 220, 255, 255);
            SDL_RenderFillRect(app->renderer, &r);
            SDL_SetRenderDrawColor(app->renderer, 0, 0, 0, 255);
        } else {
            SDL_RenderDrawRect(app->renderer, &r);
        }

        char buf[16];
        snprintf(buf, sizeof(buf), "D+%d", d);
        renderText(app, buf, r.x + 5, r.y + 5);
    }
}

static void computeDistributionForDay(const t_matrix M, int startState,
                                      int steps, double *outProbs, int nbStates)
{
    if (steps == 0) {
        for (int i = 0; i < nbStates; i++) outProbs[i] = 0.0;
        outProbs[startState] = 1.0;
        return;
    }

    t_matrix powM = matrixCreate(M.size);
    t_matrix tmp  = matrixCreate(M.size);

    matrixCopy(powM, M);

    for (int k = 1; k < steps; k++) {
        matrixMultiply(powM, M, tmp);
        matrixCopy(powM, tmp);
    }

    for (int j = 0; j < nbStates; j++) {
        outProbs[j] = powM.data[startState][j];
    }

    matrixFree(&powM);
    matrixFree(&tmp);
}

static void drawInfoPanel(App *app)
{
    char buf[128];

    snprintf(buf, sizeof(buf), "Today's weather: %s",
             STATE_NAMES[app->currentState]);
    renderText(app, buf, 50, 20);

    snprintf(buf, sizeof(buf), "Selected day: D+%d", app->selectedDay);
    renderText(app, buf, 50, 50);

    // allow up to 10 states safely
    double probs[10];

    computeDistributionForDay(app->M, app->currentState,
                              app->selectedDay, probs, app->nbStates);

    int y0 = 420;
    for (int i = 0; i < app->nbStates; i++) {
        snprintf(buf, sizeof(buf), "%s: %.1f%%",
                 STATE_NAMES[i], probs[i] * 100.0);
        renderText(app, buf, 50, y0 + i * 25);
    }
}

int main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;

    App app;
    app.window = NULL;
    app.renderer = NULL;
    app.font = NULL;
    app.selectedDay = 0;

    const char *filename = "exemple_meteo.txt";
    AdjList *adj = adjReadFile(filename);
    if (!adj) {
        fprintf(stderr, "Error: cannot read graph file %s\n", filename);
        return EXIT_FAILURE;
    }

    // Build transition matrix and set number of states from its size
    app.M = adjToMatrix(adj);
    app.nbStates = app.M.size;

    if (app.nbStates <= 0) {
        fprintf(stderr, "Error: matrix has no states.\n");
        matrixFree(&app.M);
        adjFree(adj);
        return EXIT_FAILURE;
    }

    srand((unsigned)time(NULL));
    app.currentState = rand() % app.nbStates;

    if (!initSDL(&app)) {
        fprintf(stderr, "SDL initialization failed\n");
        matrixFree(&app.M);
        adjFree(adj);
        return EXIT_FAILURE;
    }

    int running = 1;
    SDL_Event e;

    while (running) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                running = 0;
            } else if (e.type == SDL_MOUSEBUTTONDOWN) {
                int day = hitTestDay(e.button.x, e.button.y);
                if (day >= 0 && day < 30) {
                    app.selectedDay = day;
                }
            }
        }

        SDL_SetRenderDrawColor(app.renderer, 255, 255, 255, 255);
        SDL_RenderClear(app.renderer);

        drawCalendar(&app);
        drawInfoPanel(&app);

        SDL_RenderPresent(app.renderer);
    }

    cleanupSDL(&app);
    matrixFree(&app.M);
    adjFree(adj);

    return EXIT_SUCCESS;
}
