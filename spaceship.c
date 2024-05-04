#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include <math.h>

#include <SDL2/SDL.h>

#ifdef __TINYC__
#undef main
#endif

int randInt(int rmin, int rmax)
{
    return rand() % (rmax - rmin) + rmin;
}

static inline int min(const int a, const int b) {
    return a < b ? a : b;
}
static inline int max(const int a, const int b) {
    return a > b ? a : b;
}

// Window dimensions
static const int width = 200;
static const int height = 200;

int main(int argc, char **argv)
{
    // Initialize the random number generator
    srand(time(NULL));

    SDL_Window *window = SDL_CreateWindow("Spaceship", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_OPENGL);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    // Initial renderer color
    SDL_SetRenderDrawColor(renderer, randInt(0, 255), randInt(0, 255), randInt(0, 255), 255);

    bool running = true;
    struct Color
    {
        int r;
        int g;
        int b;
    };
    struct Star
    {
        int x;
        int y;
        int z;
        struct Color color;
    };

    struct Dir
    {
        int x;
        int y;
    };

    int num_stars = 150;
    struct Dir dir;
    dir.x = 1;
    dir.y = 0;

    int fly = 1;
    

    struct Star* stars = calloc(num_stars, sizeof(struct Star));

    for(int i = 0; i < num_stars; i++){
        stars[i].x = randInt(0,width);
        stars[i].y = randInt(0,height);
        stars[i].z = randInt(1,10);
        stars[i].color.r = randInt(0,255);
        stars[i].color.g = randInt(0,255);
        stars[i].color.b = randInt(0,255);
    }

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
                // keyboard controls
                const char *key = SDL_GetKeyName(event.key.keysym.sym);
                if (strcmp(key, "Q") == 0)
                {
                    running = false;
                }
                if (strcmp(key, "Up") == 0)
                {
                    fly = 1;
                    dir.y += 1;
                }
                if (strcmp(key, "Down") == 0)
                {
                    fly = 1;
                    dir.y -= 1;
                }
                if (strcmp(key, "Left") == 0)
                {
                    fly = 1;
                    dir.x += 1;
                }
                if (strcmp(key, "Right") == 0)
                {
                    fly = 1;
                    dir.x -=1;
                }
                if (strcmp(key, "Space") == 0)
                {
                    fly = 1 - fly;
                }
                if (strcmp(key, "Backspace") == 0)
                {
                    dir.x /= 2;
                    dir.y /= 2;
                }
                else{
                    printf("%s",key);
                }
            }
        }

        // Clear screen
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Draw stars
        for(int i = 0; i < num_stars; i++){
            SDL_SetRenderDrawColor(renderer, stars[i].color.r, stars[i].color.g, stars[i].color.b, 255);
            SDL_RenderDrawPoint(renderer, stars[i].x,stars[i].y);
            // SDL_SetRenderDrawColor(renderer, stars[i].color.r/3, stars[i].color.g/3, stars[i].color.b/3, 100);
            // SDL_RenderDrawPoint(renderer, stars[i].x - 1,stars[i].y);
            // SDL_SetRenderDrawColor(renderer, stars[i].color.r/6, stars[i].color.g/6, stars[i].color.b/6, 10);
            // SDL_RenderDrawPoint(renderer, stars[i].x - 2,stars[i].y);
            stars[i].x = (width + stars[i].x + fly * dir.x * 10/stars[i].z) % width;
            stars[i].y = (height + stars[i].y + fly * dir.y * 10/stars[i].z) % height;
        }

        // Draw spaceship
        SDL_SetRenderDrawColor(renderer,255,255,255,255);
        for(float t = 3.14; t < 3.14 * 2; t+=0.1){
            SDL_RenderDrawLine(renderer, 
            width/2 + 10 * cos(t), 
            height/2 + 5 * sin(t), 
            width/2 + 10 * cos(t+0.1), 
            height/2 + 5 * sin(t+0.1));

        }
        SDL_SetRenderDrawColor(renderer,0,0,0,255);
        for(float t = 3.14; t < 3.14 * 2; t+=0.1){
            SDL_RenderDrawLine(renderer, 
            width/2 + 9 * cos(t), 
            height/2 + 4 * sin(t), 
            width/2 + 9 * cos(t+0.1), 
            height/2 + 4 * sin(t+0.1));
        }
        
        SDL_SetRenderDrawColor(renderer,255,255,255,255);
        SDL_RenderDrawLine(renderer, width/2 - 9, height/2,width/2 +9, height/2 );

        SDL_Rect r;
            r.x = width/2 - 12;
            r.y = height/2;
            r.w = 2;
            r.h = 2;

        for(int j = 0 ; j < 6; j++){
            r.x += 3;
            SDL_RenderFillRect(renderer, &r );
        }


        // Show what was drawn
        SDL_RenderPresent(renderer);
    }

    // Release resources
    free(stars);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
