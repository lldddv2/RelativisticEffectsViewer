#ifndef BODY_H
#define BODY_H

#include "SphericalVectors.h"

float velocity = 0.0f;
float vel[3];


vector3 direction;
vector3 lastDirection;
vector3 velocityVector;
vector3 lastVelocityVector;
vector3 normal;

float lastTime = 0.0f;
float t = 0.0f;
float deltaT = 0.0f;
bool isTimeActive = true;

#include "Stars.h"
#include "ObjManager.h"

Obj objs[3];
int numObjs = 0;

int loadNewObj(char *path, char *name, char *texturePath, int index)
{
    if (numObjs >= 15)
    {
        printf("No se pueden cargar más objetos\n");
        return -1;
    }
    loadDotObj(&objs[index], path, name, texturePath);
    numObjs++;
    return numObjs - 1;
}

void vecToRADEC(float x, float y, float z, float *ra, float *dec)
{
    *ra = atan2(y, x) * 180.0f / M_PI;
    *dec = atan2(z, sqrt(x * x + y * y)) * 180.0f / M_PI;
}

// Moving the camera
void rotate(float *vector, float *vectorRotate, float angle, int axis)
{
    float x = vector[0];
    float y = vector[1];
    float z = vector[2];

    float c = cos(angle * M_PI / 180.0f);
    float s = sin(angle * M_PI / 180.0f);

    if (axis == 0)
    {
        vectorRotate[0] = x;
        vectorRotate[1] = y * c - z * s;
        vectorRotate[2] = y * s + z * c;
    }
    else if (axis == 1)
    {
        vectorRotate[0] = x * c + z * s;
        vectorRotate[1] = y;
        vectorRotate[2] = -x * s + z * c;
    }
    else
    {
        vectorRotate[0] = x * c - y * s;
        vectorRotate[1] = x * s + y * c;
        vectorRotate[2] = z;
    }
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void showHideObj(int index, float x0, float y0, float z0)
{
    moveObject(&objs[index], x0, y0, z0);
    objs[index].isVisible = !objs[index].isVisible;
}

void processInput(GLFWwindow *window)
{
    float theta = 0;
    float phi = 0;
    float step = 0.5f;

    vector3InitCartesian(&lastDirection, direction.x, direction.y, direction.z, false);
    lastVelocityVector = multiplyVector3(&lastDirection, -velocity);

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, 1);

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        {
            float velAddition = (velocity > 0.98f) ? 5e-4 : 5e-3;
            if (velocity < 1.0f - 2.0f * velAddition)
            {
                velocity += velAddition;
            }
        }
        else
        {
            theta = step;
        }
    }

    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        {
            float velAddition = (velocity > 0.98f) ? 5e-4 : 5e-3;
            if (velocity > 0.0001f)
            {
                velocity -= velAddition;
            }
        }
        else
        {
            theta = -step;
        }
    }

    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
    {
        if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        {
            deltaT -= 0.05;
            t += deltaT;
        }
        else
        {
            phi = step;
        }
    }

    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
    {
        if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        {
            deltaT += 0.05;
            t += deltaT;
        }
        else
        {
            phi = -step;
        }
    }

    static bool spaceWasPressed = false; // Guarda el estado previo de la tecla

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        if (!spaceWasPressed)
        {                                 // Solo entra si antes no estaba presionada
            isTimeActive = !isTimeActive; // Alterna estado
            spaceWasPressed = true;       // Marca como presionada
        }
    }
    else
    {
        spaceWasPressed = false; // Se resetea cuando la tecla se suelta
    }

    // alt + E para hacer la tierra visible
    static bool eWasPressed = false;

    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS)
    {
        if (!eWasPressed)
        {
            showHideObj(1, 1, 0, -0.30);
            eWasPressed = true;
        }
    }
    else
    {
        eWasPressed = false;
    }

    static bool jWasPressed = false;

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS)
    {
        if (!jWasPressed)
        {
            showHideObj(2, 1, -0.1, 0.10);
            jWasPressed = true;
        }
    }
    else
    {
        jWasPressed = false;
    }

    static bool dWasPressed = false;

    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS)
    {
        if (!dWasPressed)
        {
            showHideObj(0, 5, 0, 1.f);
            dWasPressed = true;
        }
    }
    else
    {
        dWasPressed = false;
    }

    // movemos la camara
    vector3InitSpherical(&direction, direction.RA + theta, direction.DEC - phi, true);

    velocityVector = multiplyVector3(&direction, -velocity);
    vel[0] = velocityVector.x;
    vel[1] = velocityVector.y;
    vel[2] = velocityVector.z;


    if (!isVectorEqual(&lastVelocityVector, &velocityVector))
    {
        modifyStarPositions();
    }
}

void cleanup()
{
    for (int i = 0; i < numObjs; i++)
    {
        freeObj(&objs[i]);
    }
    cleanupStars();
    glDeleteProgram(shaderProgram);
}

void sendJsonInfo(float velocity)
{
    char jsonInfo[5024];
    snprintf(jsonInfo, sizeof(jsonInfo),
             "jsonInfo = {"
             "    'velocity': %.3f,"
             "    'time': %.3f,"
             "    'RA': %.3f,"
             "    'DEC': %.3f,"
             "    'dirX': %.3f,"
             "    'dirY': %.3f,"
             "    'dirZ': %.3f,"
             "    'normX': %.3f,"
             "    'normY': %.3f,"
             "    'normZ': %.3f,"
             "    'diceX': %.3f,"
             "    'diceY': %.3f,"
             "    'diceZ': %.3f,"
             "    'earthX': %.3f,"
             "    'earthY': %.3f,"
             "    'earthZ': %.3f,"
             "    'saturnX': %.3f,"
             "    'saturnY': %.3f,"
             "    'saturnZ': %.3f"
             "};"
             "execJsonInfo();",
             velocity, t, 
             direction.RA, direction.DEC,
             direction.x, direction.y, direction.z,
             0.0f, 1.0f, 0.0f,
             objs[0].x_mean, objs[0].y_mean, objs[0].z_mean,
             objs[1].x_mean, objs[1].y_mean, objs[1].z_mean,
             objs[2].x_mean, objs[2].y_mean, objs[2].z_mean);
    emscripten_run_script(jsonInfo);
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (isTimeActive)
    {
        deltaT += 5e-3;
        t += deltaT;
    }
    for (int i = 0; i < numObjs; i++)
    {
        if (getDistance(&objs[i]) > 10)
        {
            moveJustCenter(&objs[i], velocityVector.x * deltaT, velocityVector.y * deltaT, velocityVector.z * deltaT);
        }
        else
        {
            if (deltaT != 0 || !isVectorEqual(&lastVelocityVector, &velocityVector))
            {
                opticalDeformation(&objs[i], vel, t, deltaT);
            }
        }
    }

    sendJsonInfo(velocity);

    // plotStars(shaderProgram);
    plotStars();

    // Crear matrices de modelo, vista y proyección
    mat4 modelMatrix, viewMatrix, projectionMatrix;
    glm_mat4_identity(modelMatrix);

    // Configurar la cámara
    vec3 cameraPos = {0.0f, 0.0f, 0.0f};   // Posición de la cámara
    vec3 cameraUp = {0.0f, 1.0f, 0.0f}; // Vector hacia arriba de la cámara
    glm_normalize(direction.vec3);

    vec3 center;
    glm_vec3_add(cameraPos, direction.vec3, center);

    glm_lookat(cameraPos, center, cameraUp, viewMatrix);

    // Configurar proyección en perspectiva
    glm_perspective(glm_rad(45.0f), 1000.0f / 700.0f, 0.1f, 100.0f, projectionMatrix);

    glUseProgram(shaderProgram);

    glUniform3f(glGetUniformLocation(shaderProgram, "lightPos"), -1.0f, 1.0f, -1.0f);
    glUniform3f(glGetUniformLocation(shaderProgram, "lightColor"), 1.0f, 1.0f, 1.0f);
    glUniform3f(glGetUniformLocation(shaderProgram, "objectColor"), 1.0f, 1.0f, 1.0f);

    glUseProgram(shaderProgram);

    // Enviar matrices al shader
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, (float *)modelMatrix);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, (float *)viewMatrix);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, (float *)projectionMatrix);

    for (int i = 0; i < numObjs; i++)
    {
        mat4 modelMatrix;
        glm_mat4_identity(modelMatrix);

        // Aplicar transformaciones al modelo

        if (getDistance(&objs[i]) < 10)
        {
            // Dibujar con la matriz actualizada
            drawObj(&objs[i], shaderProgram, modelMatrix);
        }
    }

    deltaT = 0;
}

void init()
{
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    createShaderProgram();
    createStarShaderProgram();

    loadStars("assets/stars.txt");

    loadNewObj("assets/objects3d/Dice/", "dice", "Earth_diff.jpg", 0);
    loadNewObj("assets/objects3d/Earth/", "Earth", "Earth_diff.jpg", 1);
    loadNewObj("assets/objects3d/Saturn/", "Saturn", "Saturn (body).jpg", 2);

    objs[0].isVisible = true;
    objs[1].isVisible = false;
    objs[2].isVisible = false;

    moveObject(&objs[0], 5, 0, 1.f);

    lastTime = glfwGetTime(); // Initialize lastTime before the loop
}

#endif