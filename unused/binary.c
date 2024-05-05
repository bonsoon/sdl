#include <stdlib.h>

int main(){
    struct bitmap{
        int width;
        int height;
        int* data;
    };
    struct bitmap ship;
        ship.width = 16;
        ship.height = 16;
        ship.data = (int*) calloc(ship.height,sizeof(int));
        int temp[] = {1331,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};
        for(int k = 0; k<ship.height;k++){
            ship.data[k] = temp[k];
            char buffer[16];
            printf("%d ",atoi( itoa(ship.data[k], buffer, 2) ));
        }
    
    free(ship.data);
}