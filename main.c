#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>
//#include <math.h>


#define WIDTH 400
#define HEIGHT 300
#define MAX_FRAMES 300
#define MAX_FILENAME_SIZE 56
#define MAX_FOLDERNAME_SIZE 63
#define SPEED 2

int main(int argc, char* argv[]) {
    const int maxColorValue = 255; // 8-bit color

    const uint8_t red[]   = { 0xff, 0x00, 0x00 };
    const uint8_t green[] = { 0x00, 0xff, 0x00 };
    const uint8_t blue[]  = { 0x00, 0x00, 0xff };

    int folderStatus = mkdir("testFolder", 0755);
    if(folderStatus == -1) {
        if(errno == EEXIST) {
            printf("Directory already exists, images may be overwritten!\n");
        } else {
            perror("Error creating folder");
        }
    }

    // foldername + 1 for "/" + filename + 4 for padding + 4 for ".ppm"
    char currentFileName[MAX_FOLDERNAME_SIZE + MAX_FILENAME_SIZE + 9];

    // setup common header for all ppm files
    char ppmHeader[32];
    sprintf(ppmHeader,"P6\n" "%d %d\n" "%d\n", WIDTH, HEIGHT, maxColorValue);

    for(int frame = 1; frame <= MAX_FRAMES; frame++) {
        printf("Currently working on frame %d/%d [%.2f%%]\n",
               frame,
               MAX_FRAMES,
               ( (float)frame / MAX_FRAMES ) * 100
            );
        sprintf(currentFileName, "testFolder/test-%04d.ppm", frame);

        FILE* file = fopen(currentFileName, "wb");
        if(!file) {
            perror("Error opening file");
            return -1;
        }

        // write header
        fwrite(ppmHeader, sizeof(char), strlen(ppmHeader), file);

        // logic for squares
        int xWarped;
        int yWarped;
        for(int j=0; j<HEIGHT; j++) {
            yWarped = ( (j + frame * SPEED) / 30 ) % 3;

            for(int i=0; i<WIDTH; i++) {
                xWarped = ( (i + frame * 2 * SPEED) / 30 ) % 3;

                if(xWarped == yWarped)
                    fwrite(red, sizeof(uint8_t), 3, file);
                else if(xWarped == yWarped + 1 || xWarped + 2 == yWarped)
                    fwrite(green, sizeof(uint8_t), 3, file);
                else
                    fwrite(blue, sizeof(uint8_t), 3, file);

                /*
                //if( (j + frame*SPEED) % 90 < 30) {
                //    fwrite( ((i + frame*SPEED) % 100 < 50) ? red : blue, sizeof(uint8_t), 3, file);
                //}
                //else if( (j)) {
                //    fwrite( ((i + frame*SPEED) % 100 < 50) ? blue : red, sizeof(uint8_t), 3, file);
                //}
                */
            }
        }
        fclose(file);
    }

    return 0;
}
