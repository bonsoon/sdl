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
static const int world_scale = 100;

// My types and definitions

#define PI 3.141592654

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

typedef struct Locations
{
    int x;
    int y;
} Location;

typedef struct Planets
{
    int x;
    int y;
    int w;
    int h;
    float angle;
    float angular_speed;
    SDL_Texture* planet_texture;
    SDL_Rect planet_dstrect;
} Planet;

Planet draw_planet();


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
    int heading = 30;
    int speed = 15;
    Dir dir;
    dir.x = speed * cos(heading * 180/PI);
    dir.y = speed * sin(heading * 180/PI);

    int fly = 1;
    int subpixels = 10;

    Star* stars = calloc(num_stars, sizeof(Star));
    Location loc;
    loc.x = 0;
    loc.y = 0;

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


    Planet arrakis;
        arrakis.x = 105000;
        arrakis.y = 0;
        arrakis.w = 100000;
        arrakis.h =  75000;
        arrakis.angle = 180;
        arrakis.angular_speed = -0.001;
        arrakis.planet_texture = IMG_LoadTexture(renderer, "arrakis.png");
        arrakis.planet_dstrect.x = width/2 - 15;
        arrakis.planet_dstrect.y = height/2 -15;
        arrakis.planet_dstrect.w = 30;
        arrakis.planet_dstrect.h = 30;

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
                    // dir.y += 1;
                    speed += 5;
                }
                if (strcmp(key, "Down") == 0)
                {
                    fly = 1;
                    // dir.y -= 1;
                    speed -= 5;
                }
                if (strcmp(key, "Left") == 0)
                {
                    // fly = 1;
                    // dir.x += 1;
                    heading-=15;
                }
                if (strcmp(key, "Right") == 0)
                {
                    // fly = 1;
                    // dir.x -=1;
                    heading+=15;
                }
                if (strcmp(key, "Space") == 0)
                {
                    fly = 1 - fly;
                }
                if (strcmp(key, "Backspace") == 0)
                {
                    speed /= 2;
                }
                printf("%s",key);
            }
        }

        dir.x = speed * cos(heading * PI / 180);
        dir.y = speed * sin(heading * PI / 180);
        loc.x += dir.x * fly;
        loc.y += dir.y * fly;
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

            stars[i].x = (subpixels*width + (stars[i].x + fly * dir.x * 1/stars[i].z)) % (subpixels*width);
            stars[i].y = (subpixels*height + (stars[i].y + fly * dir.y * 1/stars[i].z)) % (subpixels*height);
        }

        // Draw orbit
        arrakis = draw_planet(renderer, arrakis, loc);        

        // Draw spaceship
        double angle;
        angle = atan2(-cos(heading * PI / 180),sin(heading * PI / 180)) * 180 / 3.141592654;
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
};

ellipse(int x, int y, int w, int h, SDL_Renderer * renderer)
{
    SDL_SetRenderDrawColor(renderer, 150,200,150,255);
    for(float t = 0 ; t < 360 ; t+=1){
        SDL_RenderDrawLine(renderer, 
            x + w * cos(t / 180 * PI), 
            y + h * sin(t / 180 * PI), 
            x + w * cos((t+0.1) / 180 * PI),
            y + h * sin((t+0.1) / 180 * PI)
            );
    }
};

Planet draw_planet(SDL_Renderer * renderer, Planet planet, Location loc)
{
    ellipse((planet.x + loc.x)/world_scale, 
            (planet.y + loc.y)/world_scale, 
            planet.w/world_scale, 
            planet.h/world_scale,
            renderer);
    planet.angle += planet.angular_speed;
    planet.planet_dstrect.x = (loc.x + planet.x + planet.w * cos(planet.angle * PI / 180))/world_scale - 15;
    planet.planet_dstrect.y = (loc.y + planet.y + planet.h * sin(planet.angle * PI / 180))/world_scale - 15;

    SDL_RenderCopy(renderer, planet.planet_texture, NULL, &planet.planet_dstrect);
    return planet;
}

/*
Some solar system data
Object   Distance to sun in AU
Sun      0 AU
Mercury  0.39
Venus    0.72
Earth    1.00
Mars     1.52
Jupiter  5.20
Saturn   9.54
Uranus   19.2
Neptune  30.1
Pluto    39.4 

*/