// SphericalVectors.h
#ifndef SPHERICAL_VECTORS_H
#define SPHERICAL_VECTORS_H

#include <cglm/vec3.h>

typedef struct {
    float RA, DEC;
    float x, y, z;
    float norm;
    vec3 vec3;
} vector3;

void vector3InitCartesian(vector3 *vec, float x, float y, float z, bool normalize);
void vector3InitSpherical(vector3 *vec, float RA, float DEC, bool normalize);

void normalizeVector3(vector3 *vec);
bool isVectorEqual(vector3 *vec1, vector3 *vec2);

vector3 multiplyVector3(vector3 *vec, float scalar);

#endif
