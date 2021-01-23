#ifndef BUFFER_H
#define BUFFER_H


class Buffer
{
public:
    enum BufferType
    {
        VERTEX_BUFFER,
        INDEX_BUFFER
    };

    Buffer();
    Buffer(BufferType type);

    void uploadData(unsigned char* data,int size);
    void bind();
private:
    BufferType type;
    unsigned int id;
};

#endif // BUFFER_H
