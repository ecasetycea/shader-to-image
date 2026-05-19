#include "vectors.h"

#ifndef SHADERS_H
#define SHADERS_H
typedef enum {
    SQUARES,
    SPLIT
} shader_t;


int initShaderSettings(
    int width,
    int height,
    int maxColorValue,
    shader_t shader,
    int shaderSpeed
);
#endif