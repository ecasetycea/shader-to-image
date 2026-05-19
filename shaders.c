/*=======================================================*\
| Shaders are implemented here. Each shader take as input |
| the pixel to shade, and the current frame and returns a |
| float vec4 with rgba values from 0.0 to 1.0.            |
|                                                         |
| Each shader can use the resolution (width x height) to  |
| map each pixel to the -1.0 to 1.0 range if needed.      |
|                                                         |
| All internal functions should be limited to this        |
| translation unit (static) with the only two exceptions  |
| being the init function and the function pointer        |
\*=======================================================*/

#include "shaders.h"
#include "vectors.h"
#include "assert.h"
#include "stdio.h"

#define PI 3.141596

static int screenWidth;
static int screenHeight;
static int maxColor;
static int speed;

// function pointer to use specific shader
vec4_t (*shadePixel)(int x, int y, int frame);

// static function declarations, they are hidden from main
// and thus not in the header file
static void printAvailableShaders(void);
static vec4_t shaderSquares(int x, int y, int frame);
static vec4_t shaderSplit(int x, int y, int frame);

// sets local variables that might be needed by different shaders
// and maps the function pointer to the right shader function
int initShaderSettings(
    int width,
    int height,
    int maxColorValue,
    shader_t shader,
    int shaderSpeed
) {
    screenWidth = width;
    screenHeight = height;
    maxColor = maxColorValue;
    speed = shaderSpeed;

    switch(shader) {
    case SQUARES:
        shadePixel = shaderSquares;
        break;
    case SPLIT:
        shadePixel = shaderSplit;
        break;
    default:
        printf("Error: This shader doesn't exist.\n");
        printAvailableShaders();
    }

    return 0;
}

static void printAvailableShaders() {
    printf("Currently available shaders: squares\n");
    return;
}

static vec4_t shaderSquares(int x, int y, int frame) {
    assert(x < screenWidth);
    assert(y < screenHeight);

    int u,v;
    u = ( (x + frame * 2 * speed) / 30 ) % 3;
    v = ( (y + frame * speed) / 30 ) % 3;

    vec4_t color;
    color.data[3] = 1.0;
    if(u == v) {
        color.data[0] = 1.0;
    }
    else if(u == v+1 || u+2 == v) {
        color.data[1] = 1.0;
    }
    else {
        color.data[2] = 1.0;
    }

    return color;
}

static vec4_t shaderSplit(int x, int y, int frame) {
    assert(x < screenWidth);
    assert(y < screenHeight);

    vec4_t color;
    float multiplier = (frame * PI / 300) - (int)(frame * PI);
    color.data[3] = 1.0;
    color.data[1] = 1.0 * multiplier;
    
    if(x < screenWidth/2) {
        color.data[0] = 1.0;
        color.data[2] = 0.0;
    } else {
        color.data[0] = 0.0;
        color.data[2] = 1.0;
    }

    return color;
}
