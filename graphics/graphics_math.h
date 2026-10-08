#ifndef GRAPHICS_MATH_H
#define GRAPHICS_MATH_H

// This will contain all math functions
typedef struct {
    float x;
    float y;
    float z;
    float w;
} Vec4_t;

// Matrices are column major, so m[0][2] is the third element of the first column
typedef struct {
    float m[4][4];
} Mat4_t;

void vec4_translate(Vec4_t* v, float x, float y, float z);
void vec4_scale(Vec4_t* v, float x, float y, float z);
Vec4_t vec4_add(Vec4_t a, Vec4_t b);
Vec4_t vec4_subtract(Vec4_t a, Vec4_t b);
Vec4_t vec4_multiply(Vec4_t a, Vec4_t b);
Vec4_t vec4_dot_product(Vec4_t a, Vec4_t b);
Vec4_t vec4_cross_product(Vec4_t a, Vec4_t b);



#endif

