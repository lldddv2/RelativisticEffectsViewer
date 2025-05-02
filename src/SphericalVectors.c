#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#include <stdbool.h>
#include "SphericalVectors.h"


#define TOLERANCE 1e-6


void vector3InitCartesian(vector3 *vec, float x, float y, float z, bool normalize) {
    vec->x = x;
    vec->y = y;
    vec->z = z;
    vec->norm = sqrt(x * x + y * y + z * z);
    if (normalize) {
        normalizeVector3(vec);
    }

    vec->RA = atan2(y, x) * 180.0f / M_PI;
    vec->DEC = atan2(z, sqrt(x * x + y * y)) * 180.0f / M_PI;

    glm_vec3_copy((vec3){x, y, z}, vec->vec3);
}

void vector3InitSpherical(vector3 *vec, float RA, float DEC, bool normalize) {
    vec->RA = RA;
    vec->DEC = DEC;

    float raRad = RA * M_PI / 180.0f;
    float decRad = DEC * M_PI / 180.0f;

    vec->x = cos(decRad) * cos(raRad);
    vec->y = cos(decRad) * sin(raRad);
    vec->z = sin(decRad);
    vec->norm = sqrt(vec->x * vec->x + vec->y * vec->y + vec->z * vec->z);

    if (normalize) {
        normalizeVector3(vec);
    }
    glm_vec3_copy((vec3){vec->x, vec->y, vec->z}, vec->vec3);
}

void normalizeVector3(vector3 *vec) {
    vec->norm = sqrt(vec->x * vec->x + vec->y * vec->y + vec->z * vec->z);

    if (vec->norm > 0) {
        vec->x /= vec->norm;
        vec->y /= vec->norm;
        vec->z /= vec->norm;
        vec->norm = 1.0f;
    }
}

bool isVectorEqual(vector3 *vec1, vector3 *vec2) {
    return (fabs(vec1->x - vec2->x) < TOLERANCE &&
            fabs(vec1->y - vec2->y) < TOLERANCE &&
            fabs(vec1->z - vec2->z) < TOLERANCE &&
            fabs(vec1->RA - vec2->RA) < TOLERANCE &&
            fabs(vec1->DEC - vec2->DEC) < TOLERANCE);
}

vector3 multiplyVector3(vector3 *vec, float scalar) {
    float x = vec->x * scalar;
    float y = vec->y * scalar;
    float z = vec->z * scalar;

    vector3 result;
    vector3InitCartesian(&result, x, y, z, false);

    return result;
}