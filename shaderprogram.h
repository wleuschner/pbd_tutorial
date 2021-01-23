#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H
#include "shader.h"
#include "glm/glm.hpp"

class ShaderProgram
{
public:
    ShaderProgram();
    ShaderProgram(Shader& vertex_shader, Shader& fragment_shader);

    void addShader(Shader& shader);
    bool link();
    char* getLinkLog();

    void uploadMat4(char* name, glm::mat4& mat);

    void bind();
private:
    unsigned int id = 0;
};

#endif // SHADERPROGRAM_H
