#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>
#include <math.h>


#define WIDTH 1920
#define HEIGHT 1080
#define MAX_FRAMES 300
#define MAX_FILENAME_SIZE 56
#define MAX_FOLDERNAME_SIZE 63
#define MAX_COLOR_VALUE 255
#define SPEED 2
#define PI 3.1415926535

int main(int argc, char* argv[]) {
    //const int maxColorValue = 255; // 8-bit color

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
    sprintf(ppmHeader,"P6\n" "%d %d\n" "%d\n", WIDTH, HEIGHT, MAX_COLOR_VALUE);

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

        /*
        // moving squares shader
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

                / *
                //if( (j + frame*SPEED) % 90 < 30) {
                //    fwrite( ((i + frame*SPEED) % 100 < 50) ? red : blue, sizeof(uint8_t), 3, file);
                //}
                //else if( (j)) {
                //    fwrite( ((i + frame*SPEED) % 100 < 50) ? blue : red, sizeof(uint8_t), 3, file);
                //}
                * /
            }
        }
        */

        // diy hyperbolic shader (trippy)
        double u;
        double v;
        double color[3];
        double modifiedColor[3];
        uint8_t colorInt[3];
        color[0] = 0.5 + 0.5*cos( 2*PI*((double)frame / 120 + 0.263) );
        color[1] = 0.5 + 0.5*cos( 2*PI*((double)frame / 120 + 0.416) );
        color[2] = 0.5 + 0.5*cos( 2*PI*((double)frame / 120 + 0.557) );
        for(int j=0; j<HEIGHT; j++) {
            for(int i=0; i<WIDTH; i++) {
                // map to hyperbolic coordinates
                //u = log(sqrt( (double)i / j ));
                //v = sqrt( (double) i * j );

                // try polar
                double xC= (double)(i - WIDTH/2);
                double yC= (double)(j - HEIGHT/2);
                u = sqrt(xC*xC + yC*yC + (double)frame * WIDTH);
                v = atan( yC * frame / xC);

                double uScaled = ( (u*1.39) - floor(u*1.39) ) - 0.5;
                double vScaled = ( (v*1.42) - floor(v*1.42) ) - 0.5;
                double multiplier = sqrt(uScaled*uScaled + vScaled*vScaled) * exp(- sqrt(u*u + v*v) / MAX_FRAMES);
                multiplier = sin(multiplier*12 + (double)frame / MAX_FRAMES) / 9;
                multiplier = fabs(multiplier);
                multiplier = pow( 0.086/multiplier, 1.37);

                modifiedColor[0] = multiplier * sin(color[0] + (0.4*frame + 0.4*MAX_FRAMES) / 302 );
                modifiedColor[1] = multiplier * sin(color[1] + (0.41*frame + 0.4*MAX_FRAMES) / 296 );
                modifiedColor[2] = multiplier * sin(color[2] + (0.39*frame + 0.4*MAX_FRAMES) / 320 );

                colorInt[0] = (uint8_t)( (modifiedColor[0]-floor(modifiedColor[0])) * MAX_COLOR_VALUE );
                colorInt[1] = (uint8_t)( (modifiedColor[1]-floor(modifiedColor[1])) * MAX_COLOR_VALUE );
                colorInt[2] = (uint8_t)( (modifiedColor[2]-floor(modifiedColor[2])) * MAX_COLOR_VALUE );

                fwrite(colorInt, sizeof(uint8_t), 3, file);
            }
        }


        fclose(file);
    }

    return 0;
}
