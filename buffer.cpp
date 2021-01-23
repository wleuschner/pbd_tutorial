#include "buffer.h"
#include <GL/glew.h>

Buffer::Buffer()
{

}

Buffer::Buffer(BufferType type)
    : type(type)
{
    glGenBuffers(1,&id);
}

void Buffer::uploadData(unsigned char *data, int size)
{
    bind();
    switch (type) {
    case VERTEX_BUFFER:
        glBufferData(GL_ARRAY_BUFFER,size,data,GL_DYNAMIC_DRAW);
        break;
    case INDEX_BUFFER:
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,size,data,GL_STATIC_DRAW);
        break;
    }
}

void Buffer::bind()
{
    switch (type) {
    case VERTEX_BUFFER:
        glBindBuffer(GL_ARRAY_BUFFER,id);
        break;
    case INDEX_BUFFER:
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,id);
        break;
    }
}
