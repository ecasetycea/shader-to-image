#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>
//#include <math.h>


#define WIDTH 400
#define HEIGHT 300
#define MAX_FRAMES 300

int main(int argc, char* argv[]) {
    const int maxColorValue = 255; // 8-bit color

    const uint8_t red[]   = { 0xff, 0x00, 0x00 };
    const uint8_t blue[]  = { 0x00, 0x00, 0xff };
    const uint8_t green[] = { 0x00, 0xff, 0x00 };

    int folderStatus = mkdir("testFolder", 0755);
    if(folderStatus == -1) {
        if(errno == EEXIST) {
            printf("Directory already exists, images may be overwritten!\n");
        } else {
            perror("Error creating folder");
        }
    }

    //for(int frame = 0; frame < MAX_FRAMES; frame++) {


        FILE* file = fopen("testFolder/test.ppm", "wb");

        if(!file) {
            perror("Error opening file");
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
    //}

    return 0;
}
