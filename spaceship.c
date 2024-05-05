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
#define AU 100000

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
    char* name;
    int x;
    int y;
    int w;
    int h;
    float angle;
    float angular_speed;
    SDL_Texture* planet_texture;
    SDL_Rect planet_dstrect;
    SDL_Rect planet_srcrect;
} Planet;

Planet draw_planet();

typedef struct Stringbufs 
{
  char buf[64];
} Stringbuf;

// font engine;
write();
Stringbuf ftoa(float f, int places){
    int p = pow(10,places);
    int temp = f * p;
    temp %= p;
    int ftemp = f;
    Stringbuf r;
    char res[16] ="";
    itoa(ftemp, r.buf, 10);
    strcat(res, r.buf);
    strcat(res, ".");
    itoa(temp, r.buf, 10);
    strcat(res, r.buf);
    strcpy(r.buf, res);
    // printf("%s", res);
    return r;
};

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
    loc.x = 100000;
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
        texture = IMG_LoadTexture(renderer, "assets/bg.png");

    // spaceship texture
    SDL_Texture* ship = NULL;
        ship = IMG_LoadTexture(renderer,"assets/ship.png");
    // where to put this texture
    SDL_Rect ship_dstrect;
        ship_dstrect.x = width/2 - 15;
        ship_dstrect.y = height/2 -15;
        ship_dstrect.w = 30;
        ship_dstrect.h = 30;

    Planet sun = {name: "Sun", x:0, y:0, w: 0, h: 0, 
        angle: 0, angular_speed:0};
    Planet mercury = {name: "Mercury", x:0, y:0, w:0.39*AU, h: 0.39*AU, 
        angle: 30, angular_speed:0.005};
    Planet venus = {name: "Venus", x:0, y:0, w:0.72*AU, h: 0.72*AU, 
        angle: randInt(0,360), angular_speed:0.005};
    Planet earth = {name: "Earth", x:0, y:0, w: AU, h: AU, 
        angle: 180, angular_speed:-0.001};
    Planet mars = {name:"Mars", x:0, y:0, w: 1.52*AU, h: 1.52*AU, 
        angle: randInt(0,360), angular_speed:0.0005};
    Planet jupiter = {name:"Jupiter", x:0, y:0, w:5.20*AU, h: 5.20*AU, 
        angle: randInt(0,360), angular_speed:0.0005};
    Planet saturn = {name:"Saturn", x:0, y:0, w:9.54*AU, h: 9.54*AU, 
        angle: randInt(0,360), angular_speed:0.0005};
    Planet uranus = {name:"Uranus", x:0, y:0, w:19.2*AU, h: 19.2*AU, 
        angle: randInt(0,360), angular_speed:0.0005};
    Planet neptune = {name:"Neptune", x:0, y:0, w:30.1*AU, h: 30.1*AU, 
        angle: randInt(0,360), angular_speed:0.0005};
    Planet pluto = {name:"Pluto", x:0, y:0, w:39.4*AU, h: 39.4*AU, 
        angle: randInt(0,360), angular_speed:0.000005};

    // Planets tile map
    SDL_Texture* planet_textures;
        planet_textures = IMG_LoadTexture(renderer, "assets/solar_system.png");

    Planet planets[10] ={sun, mercury, venus, earth, mars, jupiter, saturn, uranus, neptune, pluto};
    for(int i = 0 ; i < sizeof(planets)/sizeof(Planet); i++){
        planets[i].planet_texture = planet_textures;
        SDL_Rect dstrect = {x:width/2 - 15, y:height/2 -15, w:30, h:30};
        planets[i].planet_dstrect = dstrect; 
        SDL_Rect srcrect = {x:((i * 30) %300), y:((i*30)/300), w:30, h:30};
        planets[i].planet_srcrect = srcrect;
    }

    // Load font texture
    SDL_Texture* font;
        font = IMG_LoadTexture(renderer, "assets/font.png");

    // Auto pilot

    int auto_pilot = 0; 
    int auto_target = 3;
    int auto_journey = 1;
    int visit_duration = 0;
    int visit_duration_max = 1000;
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
                if (strcmp(key, "A") == 0)
                {
                    auto_pilot = 1 - auto_pilot;
                }
                if (strcmp(key, "J") == 0)
                {
                    auto_journey = 1 - auto_journey;
                    auto_pilot = auto_journey;
                }
                if (strcmp(key, "0")*strcmp(key, "1")*strcmp(key, "2")*strcmp(key, "3")*strcmp(key, "4")*strcmp(key, "5")*strcmp(key, "6")*strcmp(key, "7")*strcmp(key, "8")*strcmp(key, "9") == 0)
                {
                    auto_target = atoi(key);
                }
                printf("%s",key);
            }
        }

        // Autopilot and autojourney
        if(auto_pilot){
            float orientation;
            orientation = 
            cos(heading * PI / 180) * 
                ((planets[auto_target].y + planets[auto_target].h * sin(planets[auto_target].angle* PI / 180) + loc.y)/world_scale - height/2) - 
            sin(heading * PI / 180) * 
                ((planets[auto_target].x + planets[auto_target].w * cos(planets[auto_target].angle* PI / 180) + loc.x )/world_scale- width/2);
            if(orientation > 0){
                heading -= 1;
            }
            if(orientation < 0){
                heading += 1;
            }
            if(auto_journey){
                if(abs((planets[auto_target].x + planets[auto_target].w * cos(planets[auto_target].angle* PI / 180) + loc.x )/world_scale- width/2) + abs((planets[auto_target].y + planets[auto_target].h * sin(planets[auto_target].angle* PI / 180) + loc.y)/world_scale - height/2) < 30){
                    visit_duration += 1;
                    if(visit_duration > 1000){
                        auto_target = randInt(0,10);
                        visit_duration=0;
                    }

                }
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

        for(int i = 0 ; i < sizeof(planets)/sizeof(planets[0]); i++){
            planets[i] = draw_planet(renderer, planets[i],loc);
        }

        // Draw spaceship
        double angle;
        angle = atan2(-cos(heading * PI / 180),sin(heading * PI / 180)) * 180 / 3.141592654;
        SDL_RenderCopyEx(renderer, ship, NULL, &ship_dstrect, angle, NULL, SDL_FLIP_NONE);

        // Draw UI

        char temp[100] = "Destination: ";
        strcat(temp, planets[auto_target].name);
        write(temp, NULL, 0, NULL, renderer, font);
        strcpy(temp, "Visted: ");
        // printf("%s",temp);
        float visted_percent = (visit_duration/1.0) / (visit_duration_max/1.0) * 100;
        strcat(temp, ftoa(visted_percent,2).buf);
        strcat(temp, "%");
        write(temp, NULL, 1, NULL, renderer, font);
        
        // Show what was drawn
        SDL_RenderPresent(renderer);

        // Attempt at at most 50fps
        SDL_Delay(20);
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
    SDL_SetRenderDrawColor(renderer, 220,220,250,255);
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
    planet.planet_dstrect.x = (planet.x + planet.w * cos(planet.angle * PI / 180)+loc.x)/world_scale - 15;
    planet.planet_dstrect.y = (planet.y + planet.h * sin(planet.angle * PI / 180)+loc.y)/world_scale - 15;

    SDL_RenderCopy(renderer, planet.planet_texture, &planet.planet_srcrect, &planet.planet_dstrect);
    return planet;
}

write(char * string, int cx, int cy, int cw, SDL_Renderer * renderer, SDL_Texture* font){
    for(int i = 0 ; i < strlen(string); i++){
        // printf("%c ", string[i]);
        int p = string[i]-30 + 200;
        // printf("%d ", (p % 10) * 6);
        SDL_Rect srcrect = {x: (p % 10) * 6, y: (p / 10) * 8, w: 6, h: 8};
        // SDL_Rect dstrect = {x: 6 * p, y: 0, w: 6, h: 8};
        // SDL_Rect srcrect = {x: 0, y: 0, w: 6, h: 8};
        SDL_Rect dstrect = {x: 6 * i + cx * 8, y: cy*8, w: 6, h: 8};
        SDL_RenderCopy(renderer, font, &srcrect, &dstrect);
    }
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