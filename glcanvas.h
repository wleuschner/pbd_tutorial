#ifndef GLCANVAS_H
#define GLCANVAS_H

#include <GL/glew.h>
#include <QOpenGLWidget>
#include <QTimer>
#include <glm/glm.hpp>

#include "camera.h"
#include "shaderprogram.h"
#include "buffer.h"
#include "cloth.h"

class GLCanvas : public QOpenGLWidget
{
    Q_OBJECT
public:
    explicit GLCanvas(QWidget *parent = nullptr);

    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    void keyPressEvent(QKeyEvent* event) override;

signals:

private:
    ShaderProgram shader_program;
    Cloth cloth;

    unsigned int num_sphere_idx;
    Buffer sphere_verts;
    Buffer sphere_indices;

    glm::vec3 pos = glm::vec3(0.0f);

    glm::mat4 mat_projection = glm::mat4(1.0f);
    Camera camera;

    std::vector<SphereCollisionConstraint> spheres;

    QTimer updateTimer;
};

#endif // GLCANVAS_H
