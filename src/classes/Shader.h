#ifndef BREAKOUT_SHADER_H
#define BREAKOUT_SHADER_H

#include <glad/gl.h>
#include <string>

#include <glm/glm.hpp> //matrix support for shader class 

class Shader {
public:
    unsigned int ID;

    Shader() = default;
    void Load(const char* vertexPath, const char* fragmentPath);
    void Use();

    void SetMatrix4(const char* name, const glm::mat4& value);
    void SetInteger(const char* name, int value);
    void SetVector3(const char* name, float x, float y, float z);

private:
    void Compile(const char* vertexSource, const char* fragmentSource);
    void CheckCompileErrors(unsigned int object, const char* type);
    unsigned int CompileShader(const char* source, unsigned int type, const char* name);
};

#endif
