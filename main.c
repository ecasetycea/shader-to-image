#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>
#include <math.h>
#include <assert.h>

#include "vectors.h"
#include "shaders.h"


#define WIDTH 400
#define HEIGHT 300
#define FRAMERATE 30.0
#define MAX_FRAMES 300
#define MAX_COLOR_VALUE 255
#define BUFFER_SIZE 256
#define SPEED 2
#define PI 3.1415926535


static char textBuffer[BUFFER_SIZE];
static char workingFolder[BUFFER_SIZE / 2];
static char workingFilePath[BUFFER_SIZE];
static char ppmHeader[32];
static int maxFrameDigits;

extern vec4_t (*shadePixel)(int x, int y, int frame);
extern int initShaderSettings(
    uint16_t width,
    uint16_t height,
    shader_t shader,
    float framerate,
    float shaderSpeed
);
extern void advanceFrameTime();

void init(void);
int createFolder(char* folderName);
void deleteAllInFolder(char* folderName);
void printProgress(int currentFrame);

int main(int argc, char* argv[]) {
    init();
    initShaderSettings(WIDTH, HEIGHT, SQUARES, FRAMERATE, SPEED);

    for(int frame = 1; frame <= MAX_FRAMES; frame++) {
        printProgress(frame);

        // setup image path for frame
        sprintf(
            workingFilePath,
            "%s/test-%0*d.ppm",
            workingFolder,
            maxFrameDigits,
            frame
        );
        
        // open file
        FILE* file = fopen(workingFilePath, "wb");
        if(!file) {
            perror("Error opening file");
            return -1;
        }

        // write header
        fwrite(ppmHeader, sizeof(char), strlen(ppmHeader), file);

        vec4_t colors;
        uint8_t colorsInt[3];
        for(int j=0; j<HEIGHT; j++) {
            for(int i=0; i<WIDTH; i++) {
                colors = shadePixel(i, j, frame);
                for(int k=0; k<3; k++) {
                    // translate colors to current ppm format
                    colorsInt[k] = (uint8_t)(colors.data[k] * MAX_COLOR_VALUE);
                }
                // write to file
                fwrite(colorsInt, sizeof(uint8_t), 3, file);
            }
        }
        
        fclose(file);
        advanceFrameTime(); // advance time
    }

    return 0;
}

void init() {
    maxFrameDigits = (int)log10(MAX_FRAMES) + 1;
    sprintf(ppmHeader,"P6\n" "%d %d\n" "%d\n", WIDTH, HEIGHT, MAX_COLOR_VALUE);
    createFolder("testFolder");
    sprintf(workingFolder, "testFolder");

    printf("\n"); // print new line that gets consumed by printing progress
}

int createFolder(char* folderName) {
    int folderStatus = mkdir(folderName, 0755); // drwxr-xr-x, read/execute for non owners
    strcpy(workingFolder, folderName);
    if(folderStatus == -1) {
        if(errno == EEXIST) {
            printf("Directory already exists, emptying!\n");
            deleteAllInFolder(folderName);
            return 0;
        } else {
            perror("Error creating folder");
            exit(folderStatus);
        }
    }
    printf("Created project folder \"%s\" successfully\n", folderName);
    return folderStatus;
}

void deleteAllInFolder(char* folderName) {
    DIR *folder = opendir(folderName);
    struct dirent* nextFile;
    char filePath[BUFFER_SIZE + BUFFER_SIZE/2];

    // iterate files
    while( (nextFile=readdir(folder)) != NULL ) {
        // skip "." and ".." entries
        if(strcmp(nextFile->d_name, ".") == 0 || strcmp(nextFile->d_name, "..") == 0) {
            continue;
        }
        
        // build path for each file
        sprintf(filePath, "%s/%s", folderName, nextFile->d_name);
        remove(filePath);
    }

    closedir(folder);

    printf("Emptied folder \"%s\" successfully\n", folderName);
    return;
}

void printProgress(int currentFrame) {
    float progress = ( (float)currentFrame / MAX_FRAMES ) * 100; // for %

    memset(textBuffer, '\0', sizeof(textBuffer)); // clear text buffer

    // progress to buffer
    sprintf(textBuffer,
        "Currently working on frame [%0*d/%d] - [%05.2f%%] - ",
        maxFrameDigits, currentFrame,
        MAX_FRAMES,
        progress
    );

    // status bar to buffer
    char tempChar;
    char* textBufferEnd = strchr(textBuffer, '\0');
    sprintf(textBufferEnd, "["); // continue printing from end of previous string
    textBufferEnd++;
    for(int i=0; i<50; i++) {
        if(i < (int)progress / 2) {
            tempChar = '=';
        }
        else if(i > (int)progress / 2) {
            tempChar = '.' ;
        }
        else {
            tempChar = ((int)progress % 2 == 1) ? '-' : '.';
        }
        sprintf(textBufferEnd, "%c", tempChar);
        textBufferEnd++;
    }
    sprintf(textBufferEnd, "]\n");

    // output to terminal        
    printf("\x1b[1F");   // move to beginning previous line
    //printf("\x1b[2K"); // clear entire line
    printf("\x1B[0J");   // erase from cursor to end of screen
    printf(textBuffer);
}