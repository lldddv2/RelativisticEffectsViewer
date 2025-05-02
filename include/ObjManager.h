#ifndef OBJ_MANAGER_H
#define OBJ_MANAGER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

// #include <SOIL/SOIL.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


#include "Constants.h"
#include "Relativity.h"
#include "stb_image.h"
#include <stdio.h>

typedef struct {
    char name[256];
    float Ka[4];  // Color ambiental
    float Kd[4];  // Color difuso
    float Ks[4];  // Color especular
    float Ke[4];  // Color emisivo
    float Ns;     // Brillo
    float Ni;     // Índice de refracción
    float d;      // Transparencia
    char map_Kd[256];  // Archivo de textura difusa
    char map_Ka[256];  // Archivo de textura ambiental
    int hasKdTexture;  // Indica si hay textura difusa
    int hasKaTexture;  // Indica si hay textura ambiental
    GLint texture; // Máximo 5 texturas
} Material;



// Materials
void trimLeadingSpaces(char *str) {
    char *start = str;
    while (isspace((unsigned char)*start)) {
        start++;
    }
    if (start != str) {
        memmove(str, start, strlen(start) + 1);  // Mueve el contenido
    }
}

void applyMaterial(Material *material) {
    // Aplicar propiedades del material
    glMaterialfv(GL_FRONT, GL_AMBIENT, material->Ka);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, material->Kd);
    glMaterialfv(GL_FRONT, GL_SPECULAR, material->Ks);
    glMaterialfv(GL_FRONT, GL_EMISSION, material->Ke);
    glMaterialf(GL_FRONT, GL_SHININESS, material->Ns);
    
    if (material->hasKdTexture) {
        glEnable(GL_TEXTURE_2D);
        
        glBindTexture(GL_TEXTURE_2D, material->texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    } else {
        glDisable(GL_TEXTURE_2D);
    }
    
}




// void rotateObj(Obj *obj, float angle, int axis) {
//     normaliceObj(obj, 1.0);

//     float x = 0, y = 0, z = 0;
//     for (int i = 0; i < obj->numVertices; i++) {
//         x = obj->vertices[i][0];
//         y = obj->vertices[i][1];
//         z = obj->vertices[i][2];
        
//         if (axis == 0) {
//             obj->vertices[i][0] = x;
//             obj->vertices[i][1] = y * cos(angle) - z * sin(angle);
//             obj->vertices[i][2] = y * sin(angle) + z * cos(angle);
//         } else if (axis == 1) {
//             obj->vertices[i][0] = x * cos(angle) + z * sin(angle);
//             obj->vertices[i][1] = y;
//             obj->vertices[i][2] = -x * sin(angle) + z * cos(angle);
//         } else {
//             obj->vertices[i][0] = x * cos(angle) - y * sin(angle);
//             obj->vertices[i][1] = x * sin(angle) + y * cos(angle);
//             obj->vertices[i][2] = z;
//         }
//     }
// }

typedef struct {
    char path[256];
    char name[256];
    float (*vertices)[3];
    float (*verticesDeformed)[3];
    float (*normals)[3];
    float (*textures)[2];
    float *verticesDesindex;
    float *verticesDesindexDeformed;
    float *normalsDesindex;
    float *texturesDesindex;
    int (*connectionsVertex);
    int (*connectionsNormals);
    int (*connectionsTextures);
    int numVertices;
    int numNormals;
    int numTextures;
    int numConnections;
    int numMaterials;
    float x_mean, y_mean, z_mean;
    bool justCenter;
    char texturePath[256];
    GLuint textureID;
    GLuint VAO, VBO, EBO;
    GLuint NBO, TBO;
    vec3 position;
    bool isVisible;
} Obj;

void normaliceObj(Obj *obj, float scale);
void moveObj(Obj *obj, float x0, float y0, float z0);

float getDistance(Obj *obj){
    return sqrt(obj->x_mean*obj->x_mean + obj->y_mean*obj->y_mean + obj->z_mean*obj->z_mean);
}

void moveJustCenter(Obj *obj, float x0, float y0, float z0) {
    obj->x_mean += x0;
    obj->y_mean += y0;
    obj->z_mean += z0;

    obj->justCenter = true;
}

void moveObjRelative(Obj *obj, float dx, float dy, float dz) {
    if (obj->justCenter){
        obj->justCenter = false;
        float x0 = obj->x_mean;
        float y0 = obj->y_mean;
        float z0 = obj->z_mean;
        printf("x0: %f, y0: %f, z0: %f\n", x0, y0, z0);
        normaliceObj(obj, 1.0);
        moveObj(obj, x0, y0, z0);
    }

    for (int i = 0; i < obj->numVertices; i++) {
        obj->vertices[i][0] = obj->vertices[i][0] + dx;
        obj->vertices[i][1] = obj->vertices[i][1] + dy;
        obj->vertices[i][2] = obj->vertices[i][2] + dz;
    }
    obj->x_mean += dx;
    obj->y_mean += dy;
    obj->z_mean += dz;
}

void moveObj(Obj *obj, float x0, float y0, float z0) {
    if (obj->justCenter){
        obj->justCenter = false;
        float x0 = obj->x_mean;
        float y0 = obj->y_mean;
        float z0 = obj->z_mean;

        normaliceObj(obj, 1.0);
        moveObj(obj, x0, y0, z0);
    }

    float dx = x0 - obj->x_mean;
    float dy = y0 - obj->y_mean;
    float dz = z0 - obj->z_mean;

    for (int i = 0; i < obj->numVertices; i++) {
        obj->vertices[i][0] = obj->vertices[i][0] + dx;
        obj->vertices[i][1] = obj->vertices[i][1] + dy;
        obj->vertices[i][2] = obj->vertices[i][2] + dz;
    }

    obj->x_mean = x0;
    obj->y_mean = y0;
    obj->z_mean = z0;
}

void meanObj(Obj *obj){
    for (int i = 0; i < obj->numVertices; i++) {
        obj->x_mean += obj->vertices[i][0];
        obj->y_mean += obj->vertices[i][1];
        obj->z_mean += obj->vertices[i][2];
    }
    obj->x_mean /= obj->numVertices;
    obj->y_mean /= obj->numVertices;
    obj->z_mean /= obj->numVertices;
}

void normaliceObj(Obj *obj, float scale){
    meanObj(obj);
    moveObj(obj, 0.0, 0.0, 0.0);

    float Delta_x = 0, Delta_y = 0, Delta_z = 0;
    float max_x = 0, max_y = 0, max_z = 0;
    float min_x = 0, min_y = 0, min_z = 0;

    for (int i = 0; i < obj->numVertices; i++) {
        if (obj->vertices[i][0] > max_x) max_x = obj->vertices[i][0];
        if (obj->vertices[i][1] > max_y) max_y = obj->vertices[i][1];
        if (obj->vertices[i][2] > max_z) max_z = obj->vertices[i][2];
        if (obj->vertices[i][0] < min_x) min_x = obj->vertices[i][0];
        if (obj->vertices[i][1] < min_y) min_y = obj->vertices[i][1];
        if (obj->vertices[i][2] < min_z) min_z = obj->vertices[i][2];
    }

    Delta_x = max_x - min_x;
    Delta_y = max_y - min_y;
    Delta_z = max_z - min_z;

    float max_Delta = Delta_x;
    if (Delta_y > max_Delta) max_Delta = Delta_y;
    if (Delta_z > max_Delta) max_Delta = Delta_z;

    for (int i = 0; i < obj->numVertices; i++) {
        obj->vertices[i][0] = obj->vertices[i][0] * scale / (4*max_Delta);
        obj->vertices[i][1] = obj->vertices[i][1] * scale / (4*max_Delta);
        obj->vertices[i][2] = obj->vertices[i][2] * scale / (4*max_Delta);
    }

}

void setupObjBuffers(Obj *obj);

void desindexArray(float *indexedArray, int *connections, float *desindexArray, int numIndices, int elemSize) {
    for (int i = 0; i < numIndices; i++) {
        int index = connections[i];  // Índice en el array indexado
        int destIndex = i * elemSize; // Índice en el array desindexado

        // Copiar los elementos correspondientes
        for (int j = 0; j < elemSize; j++) {
            desindexArray[destIndex + j] = indexedArray[index * elemSize + j];
        }
    }
}

void desindexObj(Obj *obj) {

    // Desindexar vértices (cada vértice tiene 3 componentes: x, y, z)
    desindexArray((float*)obj->vertices, obj->connectionsVertex, (float*)obj->verticesDesindex, obj->numConnections * 3, 3);
    
    desindexArray((float*)obj->verticesDeformed, obj->connectionsVertex, (float*)obj->verticesDesindexDeformed, obj->numConnections * 3, 3);

    // Desindexar normales (cada normal tiene 3 componentes: x, y, z)
    desindexArray((float*)obj->normals, obj->connectionsNormals, (float*)obj->normalsDesindex, obj->numConnections * 3, 3);
    
    // Si necesitas desindexar coordenadas de textura (cada `vt` tiene 2 componentes: u, v)
    desindexArray((float*)obj->textures, obj->connectionsTextures, (float*)obj->texturesDesindex, obj->numConnections * 3, 2);
}


void copyDeformed(Obj *obj){
    for (int i = 0; i < obj->numConnections * 3 * 3; i++) {
        obj->verticesDeformed[i][0] = obj->vertices[i][0];
        obj->verticesDeformed[i][1] = obj->vertices[i][1];
        obj->verticesDeformed[i][2] = obj->vertices[i][2];
    }
}




GLuint loadTexture(const char *filename) {
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    // Configurar parámetros de textura
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Cargar la imagen
    int width, height, nrChannels;
    unsigned char *data = stbi_load(filename, &width, &height, &nrChannels, 0);
    if (data) {
        GLenum format = (nrChannels == 3) ? GL_RGB : GL_RGBA;
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        printf("Error: No se pudo cargar la textura %s\n", filename);
    }
    stbi_image_free(data);
    
    return texture;
}

void loadDotObj(Obj *obj, char *path, char *name, char *textureName) {
    strcpy(obj->path, path);
    strcpy(obj->name, name);

    char filename[256];
    sprintf(filename, "%s%s", path, name);

    char filenameDotObj[256];
    char filenameMTL[256];

    // add .obj and .mtl to the filename
    strcpy(filenameDotObj, filename);
    strcat(filenameDotObj, ".obj");

    strcpy(filenameMTL, name);
    strcat(filenameMTL, ".mtl");


    FILE* file = fopen(filenameDotObj, "r");
    if (!file) {
        printf("Error al abrir el archivo %s\n", filenameDotObj);
        exit(1);
    }

    int maxVertices = 1000000;
    int maxNormals = 1000000;
    int maxTextures = 1000000;
    int maxConnections = 2000000;

    obj->vertices = malloc(maxVertices * sizeof(*obj->vertices));
    obj->verticesDeformed  = malloc(maxVertices * sizeof(*obj->vertices));
    obj->normals = malloc(maxNormals * sizeof(*obj->normals));
    obj->textures = malloc(maxTextures * sizeof(*obj->textures));
    obj->connectionsVertex = malloc(maxConnections * sizeof(*obj->connectionsVertex));
    obj->connectionsNormals = malloc(maxConnections * sizeof(*obj->connectionsNormals));
    obj->connectionsTextures = malloc(maxConnections * sizeof(*obj->connectionsTextures));

    if (!obj->vertices || !obj->normals || !obj->textures ||
        !obj->connectionsVertex || !obj->connectionsNormals || !obj->connectionsTextures) {
        printf("Error al asignar memoria\n");
        exit(1);
    }

    int vIndex = 0, tIndex = 0, nIndex = 0, cIndexQuad = 0, cIndexTri = 0;
    char lineHeader[128];
    int valuesRead;
    
    int v[4], vt[4], vn[4];

    while (fscanf(file, "%s", lineHeader) != EOF) {
        if (strcmp(lineHeader, "v") == 0) {
            fscanf(file, "%f %f %f", &obj->vertices[vIndex][0], &obj->vertices[vIndex][1], &obj->vertices[vIndex][2]);
            vIndex++;
        } else if (strcmp(lineHeader, "vt") == 0){
            fscanf(file, "%f %f %f", &obj->textures[tIndex][0], &obj->textures[tIndex][1], &obj->textures[tIndex][2]);
            tIndex++;
        } else if (strcmp(lineHeader, "vn") == 0) {
            fscanf(file, "%f %f %f", &obj->normals[nIndex][0], &obj->normals[nIndex][1], &obj->normals[nIndex][2]);
            nIndex++;
        } else if (strcmp(lineHeader, "f") == 0) {
            fgets(lineHeader, sizeof(lineHeader), file);
            valuesRead = sscanf(lineHeader, "%d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d", 
                &v[0], &vt[0], &vn[0],
                &v[1], &vt[1], &vn[1],
                &v[2], &vt[2], &vn[2],
                &v[3], &vt[3], &vn[3]);

            if (valuesRead < 12) {
                valuesRead = sscanf(lineHeader, "%d//%d %d//%d %d//%d %d//%d", 
                    &v[0], &vn[0],
                    &v[1], &vn[1],
                    &v[2], &vn[2],
                    &v[3], &vn[3]);
                for (int i = 0; i < 4; i++) {
                    vt[i] = -1; // Si no hay texturas, ponemos -1
                }
            }

            if (valuesRead == 12 || valuesRead == 8) {  
                // Si es cuadrado, convertirlo en dos triángulos

                // Primer triángulo (v0, v1, v2)
                for (int k = 0; k < 3; k++) {
                    obj->connectionsVertex[3*cIndexTri + k] = v[k] - 1;
                    obj->connectionsTextures[3*cIndexTri + k] = (vt[k] != -1) ? vt[k] - 1 : 0;
                    obj->connectionsNormals[3*cIndexTri + k] = vn[k] - 1;
                }
                cIndexTri++;

                // Segundo triángulo (v0, v2, v3)
                obj->connectionsVertex[3*cIndexTri + 0] = v[0] - 1;
                obj->connectionsVertex[3*cIndexTri + 1] = v[2] - 1;
                obj->connectionsVertex[3*cIndexTri + 2] = v[3] - 1;

                obj->connectionsTextures[3*cIndexTri + 0] = (vt[0] != -1) ? vt[0] - 1 : 0;
                obj->connectionsTextures[3*cIndexTri + 1] = (vt[2] != -1) ? vt[2] - 1 : 0;
                obj->connectionsTextures[3*cIndexTri + 2] = (vt[3] != -1) ? vt[3] - 1 : 0;

                obj->connectionsNormals[3*cIndexTri + 0] = vn[0] - 1;
                obj->connectionsNormals[3*cIndexTri + 1] = vn[2] - 1;
                obj->connectionsNormals[3*cIndexTri + 2] = vn[3] - 1;

                cIndexTri++;
            } else if (valuesRead == 9 || valuesRead == 6) {  
                // Si es triángulo, guardarlo directamente
                for (int k = 0; k < 3; k++) {
                    obj->connectionsVertex[3*cIndexTri + k] = v[k] - 1;
                    obj->connectionsTextures[3*cIndexTri + k] = (vt[k] != -1) ? vt[k] - 1 : 0;
                    obj->connectionsNormals[3*cIndexTri + k] = vn[k] - 1;
                }
                cIndexTri++;
            }

        } else if (strcmp(lineHeader, "usemtl") == 0) {
            // fscanf(file, "%s", obj->materialNames[obj->numMaterials]);
            // obj->materials[obj->numMaterials] = loadMTL(path, filenameMTL, obj->materialNames[obj->numMaterials]);
            // obj->materialIndex[obj->numMaterials] = cIndexQuad;
            obj->numMaterials += 1;
        }
    }

    fclose(file);

    obj->numVertices = vIndex;
    obj->numNormals = nIndex;
    obj->numConnections = cIndexTri;

    normaliceObj(obj, 1.0);
    moveObj(obj, 1.0, 0, 0);

    // Asignación de memoria para vértices y normales desindexados
    obj->verticesDesindex = malloc(obj->numConnections * 3 * 3 * sizeof(float));
    obj->verticesDesindexDeformed = malloc(obj->numConnections * 3 * 3 * sizeof(float));
    obj->normalsDesindex = malloc(obj->numConnections * 3 * 3 * sizeof(float)); 
    obj->texturesDesindex = malloc(obj->numConnections * 3 * 2 * sizeof(float));
    
    copyDeformed(obj);
    desindexObj(obj);


    // if (textureName != NULL){
    // } else{
    //     obj->textureID = -1;
    // }
    strcpy(obj->texturePath, path);
    strcat(obj->texturePath, textureName);
    obj->textureID = loadTexture(obj->texturePath);

    obj->isVisible = true;
    setupObjBuffers(obj);

}

void setupObjBuffersVAO(Obj *obj){
    glGenVertexArrays(1, &obj->VAO);

    glBindVertexArray(obj->VAO);

    // Buffer de Vértices
    glBindBuffer(GL_ARRAY_BUFFER, obj->VBO);
    glBufferData(GL_ARRAY_BUFFER, obj->numConnections * 3 * 3 * sizeof(float), obj->verticesDesindexDeformed, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

}

void setupObjBuffers(Obj *obj) {
    glGenVertexArrays(1, &obj->VAO);
    glGenBuffers(1, &obj->VBO);
    glGenBuffers(1, &obj->NBO);
    glGenBuffers(1, &obj->TBO);
    
    glBindVertexArray(obj->VAO);

    // Buffer de Vértices
    glBindBuffer(GL_ARRAY_BUFFER, obj->VBO);
    glBufferData(GL_ARRAY_BUFFER, obj->numConnections * 3 * 3 * sizeof(float), obj->verticesDesindexDeformed, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Buffer de Normales
    glBindBuffer(GL_ARRAY_BUFFER, obj->NBO);
    glBufferData(GL_ARRAY_BUFFER, obj->numConnections * 3 * 3 * sizeof(float), obj->normalsDesindex, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);


    // Buffer de Coordenadas de Textura
    glBindBuffer(GL_ARRAY_BUFFER, obj->TBO);
    glBufferData(GL_ARRAY_BUFFER, obj->numConnections * 3 * 2 * sizeof(float), obj->texturesDesindex, GL_STATIC_DRAW);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(2);

    // Desvincular VAO
    glBindVertexArray(0);
}


void drawObj(Obj *obj, GLuint shaderProgram, mat4 modelMatrix) {
    if (obj->isVisible){
        glUseProgram(shaderProgram);
        
        // Activar y vincular la textura
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, obj->textureID);
        glUniform1i(glGetUniformLocation(shaderProgram, "textureSampler"), 0);
        
        // Cargar la matriz de transformación en la GPU
        GLuint modelLoc = glGetUniformLocation(shaderProgram, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, (const GLfloat*)modelMatrix);
        
        glBindVertexArray(obj->VAO);
        glDrawArrays(GL_TRIANGLES, 0, obj->numConnections * 3);
        glBindVertexArray(0);
    }
}



void firstVertex(Obj *obj) {
    glBindBuffer(GL_ARRAY_BUFFER, obj->VBO);
    float firstVertices[30]; // 10 vértices * 3 componentes (x, y, z)
    glGetBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(firstVertices), firstVertices);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    printf("Primeros 10 vértices:\n");
    for (int i = 0; i < 10 && i < obj->numVertices; i++) {
        printf("Vértice %d: (%f, %f, %f)\n", i, firstVertices[i * 3], firstVertices[i * 3 + 1], firstVertices[i * 3 + 2]);
    }
}



void freeObj(Obj *obj) {
    free(obj->vertices);
    free(obj->verticesDeformed);
    free(obj->normals);
    free(obj->textures);
    free(obj->connectionsVertex);
    free(obj->connectionsNormals);
    free(obj->connectionsTextures);
    free(obj->verticesDesindex);
    free(obj->verticesDesindexDeformed);
    free(obj->normalsDesindex);

    // free Buffers
    glDeleteVertexArrays(1, &obj->VAO);
    glDeleteBuffers(1, &obj->VBO);
    glDeleteBuffers(1, &obj->NBO);
    glDeleteBuffers(1, &obj->EBO);
    glDeleteBuffers(1, &obj->TBO);
}


void moveObjectRelative(Obj *obj, float dx, float dy, float dz) {
    moveObjRelative(obj, dx, dy, dz);
    copyDeformed(obj);
    desindexObj(obj);
    setupObjBuffers(obj);
}

void moveObject(Obj *obj, float x0, float y0, float z0) {
    moveObj(obj, x0, y0, z0);
    copyDeformed(obj);
    desindexObj(obj);
    setupObjBuffers(obj);
}

void scaleObject(Obj *obj, float scale) {
    float x_mean = obj->x_mean;
    float y_mean = obj->y_mean;
    float z_mean = obj->z_mean;

    normaliceObj(obj, scale);
    moveObj(obj, x_mean, y_mean, z_mean);

    copyDeformed(obj);
    desindexObj(obj);
    setupObjBuffers(obj);
}

// here is the problem
void opticalDeformation(Obj *obj, float myVelocity[3], float t, float deltaT) {
    if (deltaT != 0) {
        moveObjRelative(obj, myVelocity[X]*deltaT, myVelocity[Y]*deltaT, myVelocity[Z]*deltaT);
    }
    // copyDeformed(obj);


    for (int i = 0; i < obj->numVertices; i++) {
        optical_deformation_vector(obj->vertices[i], obj->verticesDeformed[i], myVelocity, t);
    }
    
    // desindexObj(obj);
    desindexArray((float*)obj->verticesDeformed, obj->connectionsVertex, (float*)obj->verticesDesindexDeformed, obj->numConnections * 3, 3);

    glBindBuffer(GL_ARRAY_BUFFER, obj->VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, obj->numConnections * 3 * 3 * sizeof(float), obj->verticesDesindexDeformed); // Más eficiente
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void plotObj(Obj *obj, float myVelocity[3], float t, float lastTime) {
    float deltaTime = t - lastTime;
    vec3 translation;
    glm_vec3_scale(myVelocity, deltaTime, translation);
    glm_vec3_add(obj->position, translation, obj->position);
}


#endif // OBJ_MANAGER_H