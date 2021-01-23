#include "shaderprogram.h"
#include <GL/glew.h>
#include <stdlib.h>

ShaderProgram::ShaderProgram()
{

}

ShaderProgram::ShaderProgram(Shader& vertex_shader, Shader& fragment_shader)
{
    id = glCreateProgram();
    addShader(vertex_shader);
    addShader(fragment_shader);
}

void ShaderProgram::addShader(Shader& shader)
{
    glAttachShader(id,shader.id);
}

bool ShaderProgram::link()
{
    int link_status = 0;
    glLinkProgram(id);
    glGetProgramiv(id,GL_LINK_STATUS,&link_status);
    if(!link_status)
        return false;
    return true;
}

char* ShaderProgram::getLinkLog()
{
    int llen = 0;
    int slen = 0;
    glGetProgramiv(id,GL_INFO_LOG_LENGTH,&llen);
    char* log = (char*)malloc(llen * sizeof(char));
    glGetProgramInfoLog(id,llen,&slen,log);
    return log;
}

void ShaderProgram::bind()
{
    glUseProgram(id);
}

void ShaderProgram::uploadMat4(char* name, glm::mat4 &mat)
{
    unsigned int location = glGetUniformLocation(id, name);
    glUniformMatrix4fv(location,1,GL_FALSE,(float*)&mat);
}
