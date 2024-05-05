#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>

#include <SDL2/SDL.h>

#ifdef __TINYC__
#undef main
#endif

int randInt(int rmin, int rmax)
{
    return rand() % (rmax - rmin) + rmin;
}

// Window dimensions
static const int width = 200;
static const int height = 200;

int main(int argc, char **argv)
{
    // Initialize the random number generator
    srand(time(NULL));

    SDL_Window *window = SDL_CreateWindow("Lines", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_OPENGL);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    // Initial renderer color
    SDL_SetRenderDrawColor(renderer, randInt(0, 255), randInt(0, 255), randInt(0, 255), 255);

    bool running = true;
    struct Point
    {
        int x;
        int y;
        int vx;
        int vy;
    };

    struct Point p;
    p.x = randInt(0, 200);
    p.y = randInt(0, 200);
    p.vx = 1;
    p.vy = 1;

    SDL_Event event;
    while (running)
    {
        // Process events
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                running = false;
            }
            else if (event.type == SDL_KEYDOWN)
            {
                const char *key = SDL_GetKeyName(event.key.keysym.sym);
                if (strcmp(key, "Q") == 0)
                {
                    running = false;
                }
            }
        }

        // Clear screen
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Draw some lines

        if (p.x + p.vx + 1 > width || p.x + p.vx < 0)
        {
            p.vx = -p.vx;
        }
        if (p.y + p.vy + 1> height || p.y + p.vy < 0)
        {
            p.vy = -p.vy;
        }
        p.x = (p.x + p.vx) % width;
        p.y = (p.y + p.vy) % height;

        for (int i = 0; i < 10; i++)
        {
            SDL_SetRenderDrawColor(renderer, randInt(0, 255), randInt(0, 255), randInt(0, 255), 255);
            SDL_RenderDrawLine(renderer, p.x, p.y, randInt(0, 200), randInt(0, 200));
        }

        // SDL_Delay(10);

        // Show what was drawn
        SDL_RenderPresent(renderer);
    }

    // Release resources
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
