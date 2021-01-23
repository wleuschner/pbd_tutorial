#include "shader.h"
#include "utils.h"
#include<GL/glew.h>

#include <stdio.h>
#include <stdlib.h>

char* loadShaderSource(const char* path)
{
    FILE* f;
    f = fopen(path,"r");
    fseek(f,0,SEEK_END);
    int size = ftell(f);
    fseek(f,0,SEEK_SET);
    char* source = (char*)malloc((size + 1)*sizeof(char));
    fread(source,1,size,f);
    source[size] = '\0';
    fclose(f);
    return source;
}

Shader::Shader(ShaderType type)
{
    switch (type)
    {
    case VERTEX_SHADER:
        id = glCreateShader(GL_VERTEX_SHADER);
        break;
    case FRAGMENT_SHADER:
        id = glCreateShader(GL_FRAGMENT_SHADER);
        break;
    }
}

Shader::Shader(ShaderType type, const char* path) : Shader(type)
{
    addShaderSource(path);
}

void Shader::addShaderSource(const char *path)
{
    char* source = loadShaderSource(path);
    glShaderSource(id,1,&source,NULL);
    free(source);
}

bool Shader::compile()
{
    int compile_status = 0;
    glCompileShader(id);
    glGetShaderiv(id,GL_COMPILE_STATUS,&compile_status);
    if(!compile_status)
        return false;
    return true;
}

char* Shader::getCompileLog()
{
    int llen = 0;
    int slen = 0;
    glGetShaderiv(id,GL_INFO_LOG_LENGTH,&llen);
    char* log = (char*)malloc(llen * sizeof(char));
    glGetShaderInfoLog(id,llen,&slen,log);
    return log;
}
