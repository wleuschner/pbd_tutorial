#include "glcanvas.h"
#include "shader.h"
#include "vertex.h"

#include <QKeyEvent>
#include <glm/gtc/matrix_transform.hpp>

void createSphere(int stacks, int slices, std::vector<Vertex>& vertices, std::vector<unsigned int>& indices)
{
    const float PI = 3.14159265359;
    float stack_inc = (PI)/stacks;
    float slice_inc = (2*PI)/slices;
    vertices.clear();
    indices.clear();
    for(int st=0;st<stacks;st++)
    {
        float u = st*stack_inc;

        for(int sl=0;sl<slices;sl++)
        {
            float v = sl*slice_inc;

            Vertex v1;

            v1.pos.x = sin(u)*cos(v);
            v1.pos.y = sin(u)*sin(v);
            v1.pos.z = cos(u);
            v1.normal = glm::normalize(v1.pos);

            vertices.push_back(v1);
        }
    }

    for(int st=0;st<=stacks;st++)
    {
        unsigned int stack = st<stacks ? st+1 : 0;
        for(int sl=0;sl<=slices;sl++)
        {
            unsigned int slice = sl<slices ? sl+1 : 0;
            unsigned int id1 = st*slices+sl;
            unsigned int id2 = stack*slices+sl;
            unsigned int id3 = stack*slices+slice;
            unsigned int id4 = st*slices+slice;

            indices.push_back(id1);
            indices.push_back(id2);
            indices.push_back(id4);

            indices.push_back(id4);
            indices.push_back(id2);
            indices.push_back(id3);
        }
    }
}

GLCanvas::GLCanvas(QWidget *parent) : QOpenGLWidget(parent)
{
    connect(&updateTimer,SIGNAL(timeout()),this,SLOT(update()));
    updateTimer.setInterval(1000.0f/60.0f);
    updateTimer.setSingleShot(false);
}

void GLCanvas::initializeGL()
{
    glewInit();
    unsigned int pseudo_vao;
    glGenVertexArrays(1,&pseudo_vao);
    glBindVertexArray(pseudo_vao);

    std::vector<Vertex> sphere_vtx;
    std::vector<unsigned int> sphere_idx;
    createSphere(32,32,sphere_vtx,sphere_idx);
    sphere_verts = Buffer(Buffer::VERTEX_BUFFER);
    sphere_indices = Buffer(Buffer::INDEX_BUFFER);
    sphere_verts.uploadData((unsigned char*)sphere_vtx.data(),sphere_vtx.size()*sizeof(Vertex));
    sphere_indices.uploadData((unsigned char*)sphere_idx.data(),sphere_idx.size()*sizeof(unsigned int));
    num_sphere_idx = sphere_idx.size();

    cloth = Cloth(32,32);

    Shader vertex_shader(Shader::VERTEX_SHADER,"effect.vs");
    Shader fragment_shader(Shader::FRAGMENT_SHADER,"effect.fs");
    if(!vertex_shader.compile())
    {
        printf("%s",vertex_shader.getCompileLog());
    }
    if(!fragment_shader.compile())
    {
        printf("%s",fragment_shader.getCompileLog());
    }

    shader_program = ShaderProgram(vertex_shader,fragment_shader);
    if(!shader_program.link())
    {
        printf("%s",shader_program.getLinkLog());
    }
    glClearColor(0.0f,0.0f,0.0f,1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    updateTimer.start();
    camera.move(glm::vec3(0.0f,0.0f,-50.0f));
}

void GLCanvas::resizeGL(int w, int h)
{
    mat_projection = glm::perspective(45.0f,float(w)/float(h),0.1f,100.0f);
    glViewport(0,0,w,h);
}

void GLCanvas::paintGL()
{
    for(int i=0;i<spheres.size();i++)
    {
        spheres[i].center += (1.0f/60.0f)*glm::vec3(0.0,0.0,-1.0f);
    }
    cloth.solve(1.0/60.0,10,spheres);

    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    shader_program.bind();

    //Draw spheres
    sphere_verts.bind();
    sphere_indices.bind();
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(Vertex),0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,sizeof(Vertex),0);
    for(int i=0;i<spheres.size();i++)
    {
        glm::mat4 m = glm::translate(glm::mat4(1.0f),spheres[i].center);
        shader_program.uploadMat4("world",m);
        glDrawElements(GL_TRIANGLES,num_sphere_idx,GL_UNSIGNED_INT,0);
    }

    glm::mat4 I(1.0f);
    shader_program.uploadMat4("world",I);
    shader_program.uploadMat4("projection",mat_projection);
    shader_program.uploadMat4("view",camera.get_view_mat());
    cloth.getIndexBuffer().bind();
    cloth.getVertexBuffer().bind();
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(Vertex),0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,sizeof(Vertex),0);

    glDrawElements(GL_TRIANGLES,31*31*6,GL_UNSIGNED_INT,0);
}

void GLCanvas::keyPressEvent(QKeyEvent *event)
{
    switch(event->key())
    {
    case Qt::Key_W:
        camera.move(1.0f/60.0f*glm::vec3(0.0f,0.0f,5.0f));
        break;
    case Qt::Key_A:
        camera.move(1.0f/60.0f*glm::vec3(-5.0f,0.0f,0.0f));
        break;
    case Qt::Key_S:
        camera.move(1.0f/60.0f*glm::vec3(0.0f,0.0f,-5.0f));
        break;
    case Qt::Key_D:
        camera.move(1.0f/60.0f*glm::vec3(5.0f,0.0f,0.0f));
        break;
    case Qt::Key_E:
        {
            SphereCollisionConstraint sphere;
            sphere.center = camera.getPosition();
            sphere.radius = 2.0f;
            spheres.push_back(sphere);
            break;
        }
    }
}
