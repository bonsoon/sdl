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

    SDL_Window *window = SDL_CreateWindow("Snow", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_OPENGL);
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

    int num_particles = 50;
    int max_num_particles = 1000;
    int pause = 0;
    int turbo = 0;

    int* xs = calloc(max_num_particles, sizeof(int));
    int* ys = calloc(max_num_particles, sizeof(int));
    int speed = 1;
    int* floor = calloc(width, sizeof(int));
    for(int i = 0; i < max_num_particles; i++){
        xs[i] = randInt(2,width-2);
        ys[i] = randInt(0,height);
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
                const char *key = SDL_GetKeyName(event.key.keysym.sym);
                if (strcmp(key, "Q") == 0)
                {
                    running = false;
                }
                if (strcmp(key, "R") == 0)
                {
                    for(int i = 0 ; i < width; i++){
                        floor[i] = 0;
                    }
                    for(int i = 0; i < num_particles; i++){
                        xs[i] = randInt(2,width-2);
                        ys[i] = randInt(0,height);
                    }
                    pause = 1;
                    SDL_Delay(20);
                }
                if (strcmp(key, "=") == 0)
                {
                    num_particles = min(max_num_particles, num_particles+50);
                    SDL_Delay(20);
                }
                if (strcmp(key, "-") == 0)
                {
                    num_particles = max(10, num_particles-50);
                    SDL_Delay(20);
                }
                if (strcmp(key, "P") == 0)
                {
                    pause = 1 - pause;
                }
                if (strcmp(key, "T") == 0)
                {
                    turbo = 1 - turbo;
                }
            }
        }

        // Clear screen
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Draw a pixel

        for(int i = 0 ; i < num_particles; i++){
        SDL_SetRenderDrawColor(renderer, randInt(0, 255), randInt(0, 255), randInt(0, 255), 255);
        if (xs[i] >= width-1 || xs[i]  <= 1)
        {
            xs[i] = width - xs[i];
        }
        if (ys[i] + speed >= height - floor[xs[i]]-1)
        {
            // ys[i] = randInt(0,height - floor[xs[i]]-2);
            ys[i] = 0;
            xs[i] = randInt(2,width-2);
            floor[xs[i]]++;
            if(floor[xs[i]] > height*0.95){
                SDL_Delay(100);
                pause = 1;
                // for(int i = 0 ; i < width; i++){
                //     floor[i] = 0;
                // }
                // SDL_Delay(100); 
            }
        }
        if(!pause){
            // xs[i] = (xs[i] + speed * randInt(-3,8)/5) % width;
            ys[i] = (ys[i] + speed + randInt(0,10)/5) % height;
        }

        SDL_RenderDrawPoint(renderer, xs[i],ys[i]);
        }
        
        // draw floor
        // average the floor
        for(int i = 1; i < width;i++){
            if(abs(floor[i-1] - floor[i])> 2){
            int a = (floor[i-1] + floor[i])/2; 
            floor[i-1] = a;
            floor[i] = a;}
        }
        if(abs(floor[1]-floor[width-1]>2)){
            int a = (floor[1] + floor[width-1])/2; 
            floor[width-1] = a;
            floor[0] = a;
        }
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        // for(int i = 0; i < width; i++){
        // SDL_RenderDrawLine(renderer,i,height,i,height-floor[i]);        
        // }
        for(int i = 1; i < width; i++){
        SDL_RenderDrawLine(renderer,i-1,height-floor[i-1],i,height-floor[i]);        
        }

        if(!turbo){
            SDL_Delay(50);
        }
        // Show what was drawn
        SDL_RenderPresent(renderer);
    }

    // Release resources
    free(floor);
    free(xs);
    free(ys);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
