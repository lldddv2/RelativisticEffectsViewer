#ifndef RELATIVITY_H
#define RELATIVITY_H

#include <math.h>
#include "Constants.h"


void optical_deformation_vector(float *r0, float *rOptical, float *v0, float t){    
    float myr0[3] = {r0[X], r0[Y], r0[Z]};

    float vdotr0 = v0[X] * myr0[X] + v0[Y] * myr0[Y] + v0[Z] * myr0[Z];
    float vdotv = v0[X] * v0[X] + v0[Y] * v0[Y] + v0[Z] * v0[Z];
    float r0dotr0 = myr0[X] * myr0[X] + myr0[Y] * myr0[Y] + myr0[Z] * myr0[Z];

    float a = vdotv - 1;
    float b = 2 * vdotr0;
    float c = r0dotr0;

    float tPrime = (-b + sqrt(b*b - 4*a*c))/(2*a);
    
    rOptical[X] = myr0[X] + v0[X] * tPrime;
    rOptical[Y] = myr0[Y] + v0[Y] * tPrime;
    rOptical[Z] = myr0[Z] + v0[Z] * tPrime;
}

void calulate_aberration_vector(
    float *newX, float *newY, float *newZ, 
    float x, float y, float z, float velocity,
    float dirX, float dirY, float dirZ) 
    {
    float radius = 2;
    float cos_theta = (x * dirX + y * dirY + z * dirZ);
    float sin_theta = sqrt(1 - cos_theta * cos_theta);

    float cos_theta_prime = (cos_theta + velocity) / (1 + velocity * cos_theta);
    float sin_theta_prime = sqrt(1 - cos_theta_prime * cos_theta_prime);

    float beta = sin_theta_prime / sin_theta;
    float alpha = -beta * cos_theta + cos_theta_prime;

    *newX = (alpha * dirX + beta * x)*radius;
    *newY = (alpha * dirY + beta * y)*radius;
    *newZ = (alpha * dirZ + beta * z)*radius;
}

#endif // RELATIVITY_H