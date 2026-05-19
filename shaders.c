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

#include <stdint.h>
#include "shaders.h"
#include "vectors.h"
#include "assert.h"
#include "stdio.h"

#define PI 3.141596

static struct {
    float time;
    float timeDelta;
    float framerate;
    float speed;
    uint32_t frame;
    //uint32_t maxColor;
    uint16_t width;
    uint16_t height;
} frameInfo;


// function pointer to use specific shader
vec4_t (*shadePixel)(uint16_t x, uint16_t y);

// static function declarations, they are hidden from main
// and thus not in the header file
static void printAvailableShaders(void);
static vec4_t shaderSquares(uint16_t x, uint16_t y);

// sets local variables that might be needed by different shaders
// and maps the function pointer to the right shader function
int initShaderSettings(
    uint16_t width,
    uint16_t height,
    //uint32_t maxColorValue,
    shader_t shader,
    float framerate,
    float shaderSpeed
) {
    frameInfo.width     = width;
    frameInfo.height    = height;
    //frameInfo.maxColor  = maxColorValue;
    frameInfo.framerate = framerate;
    frameInfo.speed     = shaderSpeed; 
    frameInfo.frame     = 0;
    frameInfo.time      = 0.0;
    frameInfo.timeDelta = 1.0 / framerate;


    switch(shader) {
    case SQUARES:
        shadePixel = shaderSquares;
        break;
    default:
        printf("Error: This shader doesn't exist.\n");
        printAvailableShaders();
    }

    return 0;
}

void advanceFrameTime() {
    frameInfo.frame += 1;
    frameInfo.time = frameInfo.timeDelta * frameInfo.frame;

    return;
}


static void printAvailableShaders() {
    printf("Currently available shaders: squares\n");
    return;
}

static vec4_t shaderSquares(uint16_t x, uint16_t y) {
    assert(x < frameInfo.width);
    assert(y < frameInfo.height);

    uint16_t u,v;
    u = ( (uint16_t)(x + frameInfo.frame * 2 * frameInfo.speed) / 30 ) % 3;
    v = ( (uint16_t)(y + frameInfo.frame * frameInfo.speed) / 30 ) % 3;

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
