#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

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

// My types

typedef struct Colors
{
    int r;
    int g;
    int b;
} Color;

typedef struct Stars
{
    int x;
    int y;
    int z;
    Color color;
} Star;

typedef struct Dirs
{
    int x;
    int y;
} Dir;

int main(int argc, char **argv)
{
    // Initialize the random number generator
    srand(time(NULL));
    int scale = 2;
    SDL_Window *window = SDL_CreateWindow("Spaceship", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width*scale, height*scale, SDL_WINDOW_OPENGL);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    SDL_RenderSetScale(renderer, scale,scale);

    // Initial renderer color
    SDL_SetRenderDrawColor(renderer, randInt(0, 255), randInt(0, 255), randInt(0, 255), 255);

    bool running = true;

    int num_stars = 150;
    Dir dir;
    dir.x = 2;
    dir.y = 1;

    int fly = 1;
    int subpixels = 10;

    Star* stars = calloc(num_stars, sizeof(Star));

    for(int i = 0; i < num_stars; i++){
        stars[i].x = randInt(0,width) * subpixels;
        stars[i].y = randInt(0,height) * subpixels;
        stars[i].z = randInt(1,10) * 10;
        stars[i].color.r = randInt(0,255);
        stars[i].color.g = randInt(0,255);
        stars[i].color.b = randInt(0,255);
    }
    IMG_Init(IMG_INIT_PNG);
    
    // SDL_Surface * background;
    // background = IMG_Load("bg.png"); 
    // Note to self: What is difference between SDL_Surface and SDL_Texture? 

    SDL_Texture* texture = NULL;
        texture = IMG_LoadTexture(renderer, "bg.png");

    // spaceship texture
    SDL_Texture* ship = NULL;
        ship = IMG_LoadTexture(renderer,"ship.png");
    // where to put this texture
    SDL_Rect ship_dstrect;
        ship_dstrect.x = width/2 - 15;
        ship_dstrect.y = height/2 -15;
        ship_dstrect.w = 30;
        ship_dstrect.h = 30;

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
                printf("%s",key);
            }
        }

        // Clear screen
        // SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        
        // Draw background
        SDL_RenderCopy(renderer, texture, NULL, NULL);

        // Draw stars
        for(int i = 0; i < num_stars; i++){
            int f = stars[i].z /30+ 1;
            // f = 1;
            SDL_SetRenderDrawColor(renderer, stars[i].color.r/f, stars[i].color.g/f, stars[i].color.b/f, 255);
            SDL_RenderDrawPoint(renderer, stars[i].x/subpixels,stars[i].y/subpixels);

            stars[i].x = (subpixels*width + (stars[i].x + fly * dir.x * 10/stars[i].z)) % (subpixels*width);
            stars[i].y = (subpixels*height + (stars[i].y + fly * dir.y * 10/stars[i].z)) % (subpixels*height);
        }

        // Draw spaceship
        double angle;
        angle = atan2(-dir.x,dir.y) * 180 / 3.141592654;
        SDL_RenderCopyEx(renderer, ship, NULL, &ship_dstrect, angle, NULL, SDL_FLIP_NONE);

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
