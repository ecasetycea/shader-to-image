#include <stdio.h>
#include <stdint.h>
#include <string.h>


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


    uint8_t red[] = { 0xff, 0x00, 0x00 };
    for(int j=0; j<HEIGHT; j++) {
        for(int i=0; i<WIDTH; i++) {
            fwrite(red, sizeof(uint8_t), 3, file);
        }
    }


    fclose(file);

    return 0;
}
