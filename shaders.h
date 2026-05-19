#include "vectors.h"

#ifndef SHADERS_H
#define SHADERS_H
typedef enum {
    SQUARES
} shader_t;


int initShaderSettings(
    uint16_t width,
    uint16_t height,
    //uint32_t maxColorValue,
    shader_t shader,
    float framerate,
    float shaderSpeed
);
#endif