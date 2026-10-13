#include "obj_parser.h"
#include "graphics_math.h"

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <memory.h>

#define TESTING 1 
#define VERBOSE 1

#define MAX_VERTICES 150        // Assume there won't be more than this for a small renderer
#define MAX_FACES    50        

// No w is parsed
// Could be refactored but it works for now
static int parse_vertex(FILE* fp, Vec3f_t* vert, int line_num) {
     int result = fscanf(
        fp, 
        "%f %f %f",
        &vert->x,
        &vert->y,
        &vert->z
    );

    if (result != 3) {
        fprintf(stderr, "PARSER ERROR: Invalid vertex on line %d\n", line_num);
        return PARSER_ERROR_INVALID_VERTEX;
    }

    return PARSER_SUCCESS;
}


// Ignores normals
static int parse_face(FILE* fp, Vec3i_t* face, int line_num) {
    int v1, v2, v3, n1, n2, n3;

    int res = fscanf(
        fp, 
        "%i//%i %i//%i %i//%i",
        &v1, &n1,
        &v2, &n2,
        &v3, &n3
    );

    if (res != 6) {
        fprintf(stderr, "PARSER ERROR: Invalid face on line %d\n", line_num);
        return PARSER_ERROR_INVALID_FACE;
    }

    face->x = v1;
    face->y = v2;
    face->z = v3;

    return PARSER_SUCCESS;
}


// Very simple parser for wavefront objects
// Only get vertices, normals and faces
// Assume there's no errors
// This is not robust
int parse_obj(char *file_path, Mesh_t* mesh) {
    // Open and read the contents of the file
    FILE* fp = fopen(file_path, "r");

    if (fp == NULL) {
        fprintf(stderr, "PARSER ERROR: Unable to read '%s\n", file_path);
        return PARSER_ERROR_INVALID_FILE;
    }

    // Keep track of vertices, normals and faces
    int nVerts = 0;
    Vec3f_t vertices[MAX_VERTICES];
    int nFaces = 0;
    Vec3i_t faces[MAX_FACES];
    int res;

    // Read through the file line by line
    int c, line_num = 1;
    while ((c = fgetc(fp)) != EOF) {
        if (c == '\n') {
            line_num++;
        } else if (c == '#') {
            // Consume the comment until newline
            while (fgetc(fp) != '\n') {}
            line_num++;
            continue;
        } else if (isspace(c)) 
            continue;
        else if (c == 'v') {
            if ((c = fgetc(fp)) == ' ') {
                res = parse_vertex(fp, &vertices[nVerts], line_num);

                if (res != PARSER_SUCCESS)
                    return res;

                nVerts++;
                line_num++;
            } else if ((c = fgetc(fp)) == 'n') {
                // Ignore normals 
                while (fgetc(fp) != '\n') {}
                line_num++;
                continue;
            }
        } else if (c == 'f') {
            // Faces
            res = parse_face(fp, &faces[nFaces], line_num);

            if (res != PARSER_SUCCESS)
                return res;

            nFaces++;
            continue;
        }

    }

    // Allocate
    mesh->vertices = (Vec3f_t*) malloc(sizeof(Vec3f_t) * nVerts);
    memcpy(mesh->vertices, vertices, nVerts * sizeof(Vec3f_t));
    mesh->nVertices = nVerts;

    mesh->faces = (Vec3i_t*) malloc(sizeof(Vec3i_t) * nFaces);
    memcpy(mesh->faces, faces, nFaces * sizeof(Vec3i_t));
    mesh->nFaces = nFaces;

    mesh->normals = NULL;
    mesh->nNormals = 0;
    
    fclose(fp);
    return PARSER_SUCCESS;
}


#if TESTING

int main(void) {
    Mesh_t mesh;

    if (parse_obj("../static/cube.obj", &mesh) != PARSER_SUCCESS) {
        return -1;
    }

    printf("Vertices: %d\n", mesh.nVertices);
    for (int i = 0; i < mesh.nVertices; i++) {
        printf("  [%d] (%f, %f, %f)\n", i,
            mesh.vertices[i].x,
            mesh.vertices[i].y,
            mesh.vertices[i].z
        );
    }

    printf("Normals: %d\n", mesh.nNormals);
    for (int i = 0; i < mesh.nNormals; i++) {
        printf("  [%d] (%f, %f, %f)\n", i,
            mesh.normals[i].x,
            mesh.normals[i].y,
            mesh.normals[i].z
        );
    }

    printf("Faces: %d\n", mesh.nFaces);

    return 0;
}

#endif
