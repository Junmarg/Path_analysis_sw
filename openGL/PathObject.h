#ifndef PATHOBJECT_H
#define PATHOBJECT_H

#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>

QT_BEGIN_NAMESPACE
class QOpenGLShaderProgram;
QT_END_NAMESPACE

#include "BoxMesh.h"

/*! A container for all the boxes.
    Basically creates the geometry of the individual boxes and populates the buffers.
*/
class PathObject {
public:
    PathObject();

    /*! The function is called during OpenGL initialization, where the OpenGL context is current. */
    void create(QOpenGLShaderProgram * shaderProgramm);
    void destroy();

    void render(QVector<int> path2render);

    void object_source(float width, float height, float depth, int source, QColor color, QVector<float> get_position, QVector<float> get_rotate);
    QVector<float> position;
    QVector<float> rotate;

    void floor_path_01(QColor color);
    std::vector<BoxMesh>		f_path_01;
    std::vector<Vertex>			f_vbd_01;
    std::vector<GLuint>			f_ebd_01;

    QOpenGLVertexArrayObject	m_vao;
    QOpenGLBuffer				m_vbo;
    QOpenGLBuffer				m_ebo;

    Transform3D                 trans;

    unsigned int object_num;
};

#endif // PATHOBJECT_H

