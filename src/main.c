#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>  
#include <cglm/cglm.h> 
#include <emscripten.h>

#include "Constants.h"
#include "Relativity.h"
#include "ObjManager.h"
#include "Shaders.h"


#include "Body.h"


GLFWwindow* window;

void main_loop() {
    float currentTime = glfwGetTime()/1000;
    float deltaTime = currentTime - lastTime;
    lastTime = currentTime;

    processInput(window);
    display();
    glfwSwapBuffers(window);
    glfwPollEvents();

    if (glfwWindowShouldClose(window)) {
        emscripten_cancel_main_loop();
        cleanup();
        glfwTerminate();
    }

    if (deltaTime > (1.0 / 10)) {
        printf("⚠️ Warning: Frame took %.3f ms\n", deltaTime);
    }
}

int main(int argc, char** argv) {
    if (!glfwInit()) {
        fprintf(stderr, "❌ Failed to initialize GLFW\n");
        return -1;
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);  // Usar OpenGL ES
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);         // WebGL 2.0 / OpenGL ES 3.0
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    window = glfwCreateWindow(1000, 700, "Star Viewer", NULL, NULL);
    if (!window) {
        fprintf(stderr, "❌ Failed to create GLFW window\n");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "❌ Failed to initialize GLFW\n");
        return -1;
    }

    init();

    // Reemplazar el bucle por emscripten_set_main_loop
    emscripten_set_main_loop_timing(EM_TIMING_RAF, 1000 / 20);
    emscripten_set_main_loop(main_loop, 0, 1);

    return 0;
}

