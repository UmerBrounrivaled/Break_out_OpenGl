#include "Shader.h"

#include <glm/gtc/type_ptr.hpp>
#include <fstream>
#include <iostream>
#include <sstream>

void Shader::Load(const char* vertexPath, const char* fragmentPath) {
    std::ifstream vertexFile(vertexPath);
    std::ifstream fragmentFile(fragmentPath);

    if (!vertexFile.is_open()) {
        std::cerr << "ERROR::SHADER::VERTEX_FILE_NOT_FOUND: " << vertexPath << '\n';
        return;
    }

    if (!fragmentFile.is_open()) {
        std::cerr << "ERROR::SHADER::FRAGMENT_FILE_NOT_FOUND: " << fragmentPath << '\n';
        return;
    }

    std::stringstream vertexStream;
    std::stringstream fragmentStream;
    vertexStream << vertexFile.rdbuf();
    fragmentStream << fragmentFile.rdbuf();

    Compile(vertexStream.str().c_str(), fragmentStream.str().c_str());
}

void Shader::Compile(const char* vertexSource, const char* fragmentSource) {
    unsigned int vertexShader = CompileShader(vertexSource, GL_VERTEX_SHADER, "VERTEX");
    unsigned int fragmentShader = CompileShader(fragmentSource, GL_FRAGMENT_SHADER, "FRAGMENT");

    ID = glCreateProgram();
    glAttachShader(ID, vertexShader);
    glAttachShader(ID, fragmentShader);
    glLinkProgram(ID);
    CheckCompileErrors(ID, "PROGRAM");

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

unsigned int Shader::CompileShader(const char* source, unsigned int type, const char* name) {  //reads and compiles shader files
    unsigned int shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);
    CheckCompileErrors(shader, name);
    return shader;
}

void Shader::Use() {  //activates the linked shader program
    glUseProgram(ID);
}

void Shader::SetInteger(const char* name, int value) {
    glUniform1i(glGetUniformLocation(ID, name), value);
}

void Shader::SetVector3(const char* name, float x, float y, float z) {
    glUniform3f(glGetUniformLocation(ID, name), x, y, z);
}

void Shader::SetMatrix4(const char* name, const glm::mat4& value) {
    glUniformMatrix4fv(
        glGetUniformLocation(ID, name),
        1,
        GL_FALSE,
        glm::value_ptr(value)
    );
}

void Shader::CheckCompileErrors(unsigned int object, const char* type) {
    int success;
    char infoLog[1024];

    if (std::string(type) == "PROGRAM") {
        glGetProgramiv(object, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(object, 1024, nullptr, infoLog);
            std::cerr << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << '\n'
                      << infoLog << '\n';
        }
    } else {
        glGetShaderiv(object, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(object, 1024, nullptr, infoLog);
            std::cerr << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << '\n'
                      << infoLog << '\n';
        }
    }
}
