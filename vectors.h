/*======================================================================*\
| VECTORS                                                                |
|                                                                        |
| The custom vector classes use anonymous unions inside structs in order |
| to easily return data from shaders, reinterpret it as either values or |
| colors and easily use it in loops by iterating through the array in    |
| the union. Future implementations will use SIMD to do basic operations |
| even faster.                                                           |
\*======================================================================*/

#ifndef VEC4_INT
#define VEC4_INT
typedef struct { 
    union {
        struct { int x, y, z, w; };
        int data[4];
    };
} vec4int_t;
#endif

#ifndef VEC4_FLOAT
#define VEC4_FLOAT
typedef struct {
    union {
        struct { float x, y, z, w; };
        struct { float r, g, b, a; };
        float data[4];
    };
} vec4_t;
#endif