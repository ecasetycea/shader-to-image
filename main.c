#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <math.h>


#define WIDTH 800
#define HEIGHT 600

int main(int argc, char* argv[]) {
    const int maxColorValue = 255; // 8-bit color

    FILE* file = fopen("test.ppm", "wb");

    if(!file) {
        fprintf(stderr, "Error: couldn't open file\n");
        return -1;
    }

    // write header
    char ppmHeader[64];
    sprintf(
        ppmHeader,
        "P6\n"
        "%d %d\n"
        "%d\n",
        WIDTH, HEIGHT, maxColorValue
    );
    fwrite(ppmHeader, sizeof(char), strlen(ppmHeader), file);
    //printf("%s\n", ppmHeader);


    uint8_t red[]  = { 0xff, 0x00, 0x00 };
    uint8_t blue[] = { 0x00, 0x00, 0xff };

    for(int j=0; j<HEIGHT; j++) {
        for(int i=0; i<WIDTH; i++) {
            if(j%100 < 50) {
                fwrite((i%100 < 50) ? red : blue, sizeof(uint8_t), 3, file);
            } else {
                fwrite((i%100 < 50) ? blue : red, sizeof(uint8_t), 3, file);
            }
        }
    }


    fclose(file);

    return 0;
}
