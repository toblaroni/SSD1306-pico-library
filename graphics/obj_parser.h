#ifndef OBJ_PARSER_H
#define OBJ_PARSER_H

#include "graphics_math.h"

#define PARSER_SUCCESS               0
#define PARSER_ERROR_INVALID_FILE   -1
#define PARSER_ERROR_INVALID_VERTEX -2
#define PARSER_ERROR_INVALID_FACE   -3

typedef struct {
    Vec3f_t* vertices;
    Vec3f_t* normals;
    Vec3i_t* faces;     

    int nVertices;
    int nNormals;
    int nFaces;
} Mesh_t;

int parse_obj(char* file_path, Mesh_t* m);
int free_obj(Mesh_t* m);

#endif
