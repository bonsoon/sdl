#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <SDL2/SDL.h>


#ifdef __TINYC__
#undef main
#endif

void left(char * a);
void right(char * a);
void insert(char x, char * a);
void delete();
void grow(char * a);
int l = 0;
int r = 10;
int n = 10;
char * a; 

int width = 200;
int height = 200;

void main(){
    char * a = malloc(n * sizeof(char));    
    
    int scale = 2;
    SDL_Window *window = SDL_CreateWindow("Spaceship", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width*scale, height*scale, SDL_WINDOW_OPENGL);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    SDL_RenderSetScale(renderer, scale,scale);

    // Initial renderer color
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    bool running = true;
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
                if (strcmp(key,"Left") == 0)
                {
                    left(a);
                }
                if (strcmp(key,"Right") == 0)
                {
                    right(a);
                }
                if (strcmp(key,"Delete") == 0)
                {
                    delete();
                }
                if (strcmp(key,"A") == 0)
                {
                    insert('a', a);
                }
                if (strcmp(key,"B") == 0)
                {
                    insert('b', a);
                }
                printf("%s :",key);
                printf("%d, %d \n", l, r);
                printf("%s|\n", a);
            }
            }
    }
    free(a);
}

void left(char * a){
    if(l != 0){
        l = l - 1;
        r = r - 1;
        a[r] = a[l];
    }
}

void right(char * a){
    if(r != n){
        l = l + 1;
        r = r + 1;
        a[l] = a[r];
    }
}

void insert(char x, char * a){
    if( l == r ){
        grow(a);
    }
    a[l] = x;
    l = l+1;
}

void delete(){
    if(l != 0){
        l = l-1;
    }
}

void grow(char * a){
    char * b = malloc((n+10)*sizeof(char));
    for(int i = 0; i < l-1; i++){
        b[i] = a[i];
    }
    for(int i = r; i < n - 1; i++){
        b[i + 10] = a[i];
    }
    r = r + 10;
    n = n + 10;
    a = b;
}