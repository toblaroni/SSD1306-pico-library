#include "graphics_math.h"

// === Vectors ===
void vec3f_translate(Vec3f_t* v, float x, float y, float z) {
    v->x += x;
    v->y += y;
    v->z += z;
}

void vec3f_scale(Vec3f_t* v, float x, float y, float z) {
    v->x *= x;
    v->y *= y;
    v->z *= z;
}

Vec3f_t vec3f_add(Vec3f_t a, Vec3f_t b) {
    Vec3f_t result;
    result.x = a.x + b.x;
    result.y = a.y + b.y;
    result.z = a.z + b.z;
    return result;
}

Vec3f_t vec3f_subtract(Vec3f_t a, Vec3f_t b) {
    Vec3f_t result;
    result.x = a.x - b.x;
    result.y = a.y - b.y;
    result.z = a.z - b.z;
    return result;
}

Vec3f_t vec3f_multiply(Vec3f_t a, Vec3f_t b) {
    Vec3f_t result;
    result.x = a.x * b.x;
    result.y = a.y * b.y;
    result.z = a.z * b.z;
    return result;
} 

Vec3f_t vec3f_dot_product(Vec3f_t a, Vec3f_t b) {
    Vec3f_t result;
    result.x = a.x * b.x;
    result.y = a.y * b.y;
    result.z = a.z * b.z;
    return result;
}

Vec3f_t vec3f_cross_product(Vec3f_t a, Vec3f_t b) {
    Vec3f_t result;
    result.x = a.y * b.z - a.z * b.y;
    result.y = a.z * b.x - a.x * b.z;
    result.z = a.x * b.y - a.y * b.x;
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
