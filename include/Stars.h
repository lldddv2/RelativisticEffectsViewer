#ifndef STARS_H
#define STARS_H

#include "Constants.h"
#include "Shaders.h"  // Incluir el archivo de shaders

int numStars = 0;
float *stars;
float *starsAberration;

// Convierte coordenadas ecuatoriales a cartesianas
void eqToCart(float ra, float dec, float radius, float* x, float* y, float* z) {
    float raRad = ra * M_PI / 180.0f;
    float decRad = dec * M_PI / 180.0f;
    
    *x = radius * cos(decRad) * cos(raRad);
    *y = radius * cos(decRad) * sin(raRad);
    *z = radius * sin(decRad);
}

// Variables globales para OpenGL
unsigned int starVAO = 0, starVBO = 0;

void initStarsBuffers() {
    glGenVertexArrays(1, &starVAO);
    glGenBuffers(1, &starVBO);

    glBindVertexArray(starVAO);
    glBindBuffer(GL_ARRAY_BUFFER, starVBO);
    glBufferData(GL_ARRAY_BUFFER, numStars * 4 * sizeof(float),
                 starsAberration, GL_STATIC_DRAW);

    // aPos (vec3)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                          4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // aMagnitude (float)
    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE,
                          4 * sizeof(float),
                          (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void loadStars(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Error al abrir el archivo %s\n", filename);
        exit(1);
    }

    stars = (float*)malloc(MAX_STARS * 4 * sizeof(float));
    if (!stars) {
        printf("Error al asignar memoria\n");
        exit(1);
    }

    float x, y, z;
    float mag;
    float ra, dec;

    while (numStars < MAX_STARS && fscanf(file, "%f %f %f\n", &ra, &dec, &mag)==3) {
        eqToCart(ra, dec, 1.0f, &x, &y, &z);
        float *s = &stars[4*numStars];
        s[0] = x;
        s[1] = y;
        s[2] = z;
        s[3] = mag;
        numStars++;
    }

    starsAberration = (float*)malloc(MAX_STARS * 4 * sizeof(float));
    if (!starsAberration) {
        printf("Error al asignar memoria\n");
        exit(1);
    }

    memcpy(starsAberration, stars, numStars * 4 * sizeof(float));

    fclose(file);
    printf("Se cargaron %d estrellas\n", numStars);
    
    initStarsBuffers();

}


// Libera los recursos
void cleanupStars() {
    free(stars);
    free(starsAberration);

    glDeleteVertexArrays(1, &starVAO);
    glDeleteBuffers(1, &starVBO);
} 

void modifyStarPositions() {
    for (int i = 0; i < numStars; i++) {
        float x = stars[4*i + 0];
        float y = stars[4*i + 1];
        float z = stars[4*i + 2];

        calulate_aberration_vector(
            &starsAberration[4*i + 0], &starsAberration[4*i + 1], &starsAberration[4*i + 2],
            x, y, z, 
            velocity, direction.x, direction.y, direction.z);
    }

    // update VBO con las nuevas posiciones
    glBindBuffer(GL_ARRAY_BUFFER, starVBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, numStars * 4 * sizeof(float), starsAberration); 
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void plotStars() {
    glUseProgram(starShaderProgram);
    
    // Configurar la cámara (similar a la función display)
    vec3 cameraPos = {0.0f, 0.0f, 0.0f};
    vec3 cameraUp = {0.0f, 1.0f, 0.0f};
    glm_normalize(direction.vec3);
    vec3 center;
    glm_vec3_add(cameraPos, direction.vec3, center);
    
    // Matrices para las estrellas
    mat4 viewMatrix, projectionMatrix;
    glm_lookat(cameraPos, center, cameraUp, viewMatrix);
    glm_perspective(glm_rad(45.0f), 1000.0f / 700.0f, 0.1f, 100.0f, projectionMatrix);
    
    // Enviar matrices al shader de estrellas
    glUniformMatrix4fv(glGetUniformLocation(starShaderProgram, "view"), 1, GL_FALSE, (float*)viewMatrix);
    glUniformMatrix4fv(glGetUniformLocation(starShaderProgram, "projection"), 1, GL_FALSE, (float*)projectionMatrix);
    
    // Configurar estado OpenGL para renderizar estrellas
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE); // Aditivo para efecto de brillo
    glDepthMask(GL_FALSE); // Desactivar escritura al buffer de profundidad
    
    // Dibujar las estrellas
    glBindVertexArray(starVAO);
    glDrawArrays(GL_POINTS, 0, numStars);
    glBindVertexArray(0);
    
    // Restaurar estado OpenGL
    glDepthMask(GL_TRUE);
    glDisable(GL_BLEND);
}


#endif // STARS_H