#include "graphics_math.h"

// === Vectors ===
void vec4_translate(Vec4_t* v, float x, float y, float z) {
    v->x += x;
    v->y += y;
    v->z += z;
}

void vec4_scale(Vec4_t* v, float x, float y, float z) {
    v->x *= x;
    v->y *= y;
    v->z *= z;
}

Vec4_t vec4_add(Vec4_t a, Vec4_t b) {
    Vec4_t result;
    result.x = a.x + b.x;
    result.y = a.y + b.y;
    result.z = a.z + b.z;
    result.w = a.w + b.w;
    return result;
}

Vec4_t vec4_subtract(Vec4_t a, Vec4_t b) {
    Vec4_t result;
    result.x = a.x - b.x;
    result.y = a.y - b.y;
    result.z = a.z - b.z;
    result.w = a.w - b.w;
    return result;
}

Vec4_t vec4_multiply(Vec4_t a, Vec4_t b) {
    Vec4_t result;
    result.x = a.x * b.x;
    result.y = a.y * b.y;
    result.z = a.z * b.z;
    result.w = a.w * b.w;
    return result;
} 

Vec4_t vec4_dot_product(Vec4_t a, Vec4_t b) {
    Vec4_t result;
    result.x = a.x * b.x;
    result.y = a.y * b.y;
    result.z = a.z * b.z;
    result.w = a.w * b.w;
    return result;
}

Vec4_t vec4_cross_product(Vec4_t a, Vec4_t b) {
    Vec4_t result;
    result.x = a.y * b.z - a.z * b.y;
    result.y = a.z * b.x - a.x * b.z;
    result.z = a.x * b.y - a.y * b.x;
    result.w = 0.0f; // Cross product is a 3D vector, so w is set to 0
    return result;
}

// === Matrix ===
Mat4_t mat4_multiply(Mat4_t a, Mat4_t b) {
    Mat4_t result;
    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            result.m[row][col] 
                = a.m[row][0] * b.m[0][col] +
                  a.m[row][1] * b.m[1][col] +
                  a.m[row][2] * b.m[2][col] +
                  a.m[row][3] * b.m[3][col];
        }
    }
    return result;
}