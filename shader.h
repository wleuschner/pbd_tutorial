#ifndef SHADER_H
#define SHADER_H


class Shader
{
public:
    friend class ShaderProgram;
    enum ShaderType
    {
        VERTEX_SHADER,
        FRAGMENT_SHADER
    };

    Shader(ShaderType type);
    Shader(ShaderType type, const char* path);

    void addShaderSource(const char* path);
    bool compile();
    char* getCompileLog();
private:
    unsigned int id = 0;
};

#endif // SHADER_H
