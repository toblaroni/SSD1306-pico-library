#ifndef GRAPHICS_MATH_H
#define GRAPHICS_MATH_H

// This will contain all math functions

typedef struct {
    int x;
    int y;
} Vec2i_t;

typedef struct {
    float x;
    float y;
    float z;
} Vec3f_t;

typedef struct {
    int x;
    int y;
    int z;
} Vec3i_t;

// Matrices are column major, so m[0][2] is the third element of the first column
typedef struct {
    float m[4][4];
} Mat4_t;


void vec3f_translate(Vec3f_t* v, float x, float y, float z);
void vec3f_scale(Vec3f_t* v, float x, float y, float z);
Vec3f_t vec3f_add(Vec3f_t a, Vec3f_t b);
Vec3f_t vec3f_subtract(Vec3f_t a, Vec3f_t b);
Vec3f_t vec3f_multiply(Vec3f_t a, Vec3f_t b);
Vec3f_t vec3f_dot_product(Vec3f_t a, Vec3f_t b);
Vec3f_t vec3f_cross_product(Vec3f_t a, Vec3f_t b);

Mat4_t mat4_multiply(Mat4_t a, Mat4_t b);

#endif

