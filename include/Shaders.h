#ifndef SHADERS_H
#define SHADERS_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>  
#include <cglm/cglm.h> 

GLuint shaderProgram, starShaderProgram;

void createShaderProgram() {
    const char* vertexShaderSource = 
        "#version 300 es\n"
        "precision mediump float;\n"
        "layout (location = 0) in vec3 aPos;\n"
        "layout (location = 1) in vec3 aNormal;\n"
        "layout (location = 2) in vec2 aTexCoord;\n"  // Nuevo atributo
        "out vec3 FragPos;\n"
        "out vec3 Normal;\n"
        "out vec2 TexCoord;\n"  // Salida para textura
        "uniform mat4 model;\n"
        "uniform mat4 view;\n"
        "uniform mat4 projection;\n"
        "void main()\n"
        "{\n"
        "    FragPos = vec3(model * vec4(aPos, 1.0));\n"
        "    Normal = mat3(model) * aNormal;\n"  // Simplificado para WebGL
        "    TexCoord = aTexCoord;\n"  // Pasar coordenadas de textura
        "    gl_Position = projection * view * vec4(FragPos, 1.0);\n"
        "    gl_PointSize = 1.0;\n"  // Se reemplaza glPointSize()
        "}\0";

    const char* fragmentShaderSource = 
        "#version 300 es\n"
        "precision mediump float;\n"
        "out vec4 FragColor;\n"
        "in vec3 Normal;\n"
        "in vec3 FragPos;\n"
        "in vec2 TexCoord;\n"  // Entrada de textura
        "uniform sampler2D textureSampler;\n"  // Muestreador de textura
        "uniform vec3 lightPos;\n"
        "uniform vec3 lightColor;\n"
        "uniform vec3 objectColor;\n"
        "void main()\n"
        "{\n"
        "    vec4 texColor = texture(textureSampler, TexCoord);\n"  // Muestrear textura
        "    vec3 norm = normalize(Normal);\n"
        "    vec3 lightDir = normalize(lightPos - FragPos);\n"
        "    float diff = max(dot(norm, lightDir), 0.0);\n"
        "    vec3 ambient = 0.1 * lightColor;\n"
        "    vec3 result = (ambient + diff * lightColor) * texColor.rgb;\n"  // Usar color de textura
        "    FragColor = vec4(result, texColor.a);\n"  // Incluir canal alpha
        "}\0";


    
    // Crear shader de vértices
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    
    // Verificar errores de compilación
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        fprintf(stderr, "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n%s\n", infoLog);
    }
    
    // Crear shader de fragmentos
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    
    // Verificar errores de compilación
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        fprintf(stderr, "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n%s\n", infoLog);
    }
    
    // Crear programa de shader
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    
    // Verificar errores de enlace
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        fprintf(stderr, "ERROR::SHADER::PROGRAM::LINKING_FAILED\n%s\n", infoLog);
    }
    
    // Limpiar recursos
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}


void createStarShaderProgram() {
    // const char* starVertexShaderSource = 
    //     "#version 300 es\n"
    //     "precision mediump float;\n"
    //     "layout (location = 0) in vec3 aPos;\n"
    //     "layout (location = 1) in vec3 aIntensity;\n" // Usamos el canal de normal para la intensidad
    //     "out vec3 Intensity;\n"
    //     "uniform mat4 view;\n"
    //     "uniform mat4 projection;\n"
    //     "void main()\n"
    //     "{\n"
    //     "    Intensity = aIntensity;\n"
    //     "    // Normalizar la posición para asegurar que siempre esté en el fondo\n"
    //     "    vec3 pos = normalize(aPos) * 99.0;\n" // Usar valor cercano al far plane
    //     "    gl_Position = projection * view * vec4(pos, 1.0);\n"
    //     "    // Tamaño del punto basado en la intensidad (brillo)\n"
    //     "    float brightness = (aIntensity.x + aIntensity.y + aIntensity.z) / 3.0;\n"
    //     "    gl_PointSize = max(1.0, 5.0 * brightness);\n"
    //     "}\0";

    // const char* starFragmentShaderSource = 
    //     "#version 300 es\n"
    //     "precision mediump float;\n"
    //     "in vec3 Intensity;\n"
    //     "out vec4 FragColor;\n"
    //     "void main()\n"
    //     "{\n"
    //     "    // Crea un efecto de punto circular con degradado para las estrellas\n"
    //     "    vec2 circCoord = 2.0 * gl_PointCoord - 1.0;\n"
    //     "    float distance = length(circCoord);\n"
    //     "    if (distance > 1.0) discard;\n" // Descarta fragmentos fuera del círculo
    //     "    \n"
    //     "    // Calcular intensidad basada en la distancia al centro del punto\n"
    //     "    float alpha = max(0.0, 1.0 - distance * distance);\n"
    //     "    \n"
    //     "    // Color basado en la intensidad (brillo más blanco para estrellas brillantes)\n"
    //     "    float avgIntensity = (Intensity.x + Intensity.y + Intensity.z) / 3.0;\n"
    //     "    vec3 starColor = mix(vec3(0.5, 0.7, 1.0), vec3(1.0), avgIntensity);\n"
    //     "    \n"
    //     "    FragColor = vec4(starColor, alpha);\n"
    //     "}\0";

    const char* starVertexShaderSource = 
        "#version 300 es\n"
        "precision mediump float;\n"
        "layout (location = 0) in vec3 aPos;\n"
        "layout (location = 1) in float aMagnitude;\n" // Magnitud aparente
        "out float Brightness;\n"
        "uniform mat4 view;\n"
        "uniform mat4 projection;\n"    
        "void main()\n"
        "{\n"
        "    // Convierte magnitud a brillo relativo (más magnitud = menos brillo)\n"
        "    Brightness = pow(10.0, -0.4 * aMagnitude);\n"
        "\n"
        "    vec3 pos = normalize(aPos) * 99.0;\n"
        "    gl_Position = projection * view * vec4(pos, 1.0);\n"
        "\n"
        "    // Escala punto: las estrellas más brillantes se ven más grandes\n"
        "    gl_PointSize = clamp(4.5 * Brightness, 1.3, 5.0);\n"
        "}\0";

    const char* starFragmentShaderSource = 
        "#version 300 es\n"
        "precision mediump float;\n"
        "in float Brightness;\n"
        "out vec4 FragColor;\n"
        "void main()\n"
        "{\n"
        "    vec2 circCoord = 2.0 * gl_PointCoord - 1.0;\n"
        "    float distance = length(circCoord);\n"
        "    if (distance > 1.0) discard;\n"
        "\n"
        "    float alpha = max(0.0, 1.0 - distance * distance);\n"
        "\n"
        "    // Mezcla un azul tenue con blanco, según el brillo\n"
        "    vec3 starColor = mix(vec3(0.5, 0.7, 1.0), vec3(1.0), Brightness);\n"
        "\n"
        "    FragColor = vec4(starColor, alpha);\n"
        "}\0";


    // Crear shader de vértices para estrellas
    unsigned int starVertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(starVertexShader, 1, &starVertexShaderSource, NULL);
    glCompileShader(starVertexShader);
    
    // Verificar errores de compilación
    int success;
    char infoLog[512];
    glGetShaderiv(starVertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(starVertexShader, 512, NULL, infoLog);
        fprintf(stderr, "ERROR::SHADER::STAR_VERTEX::COMPILATION_FAILED\n%s\n", infoLog);
    }
    
    // Crear shader de fragmentos para estrellas
    unsigned int starFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(starFragmentShader, 1, &starFragmentShaderSource, NULL);
    glCompileShader(starFragmentShader);
    
    // Verificar errores de compilación
    glGetShaderiv(starFragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(starFragmentShader, 512, NULL, infoLog);
        fprintf(stderr, "ERROR::SHADER::STAR_FRAGMENT::COMPILATION_FAILED\n%s\n", infoLog);
    }
    
    // Crear programa de shader para estrellas
    starShaderProgram = glCreateProgram();
    glAttachShader(starShaderProgram, starVertexShader);
    glAttachShader(starShaderProgram, starFragmentShader);
    glLinkProgram(starShaderProgram);
    
    // Verificar errores de enlace
    glGetProgramiv(starShaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(starShaderProgram, 512, NULL, infoLog);
        fprintf(stderr, "ERROR::SHADER::STAR_PROGRAM::LINKING_FAILED\n%s\n", infoLog);
    }
    
    // Limpiar recursos
    glDeleteShader(starVertexShader);
    glDeleteShader(starFragmentShader);
}



#endif // SHADERS_H