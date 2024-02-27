#ifndef BOXOBJECT_H
#define BOXOBJECT_H

#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>

QT_BEGIN_NAMESPACE
class QOpenGLShaderProgram;
QT_END_NAMESPACE

#include "BoxMesh.h"

/*! A container for all the boxes.
	Basically creates the geometry of the individual boxes and populates the buffers.
*/
class BoxObject {
public:
	BoxObject();

	/*! The function is called during OpenGL initialization, where the OpenGL context is current. */
	void create(QOpenGLShaderProgram * shaderProgramm);
	void destroy();

	void render();
    void building();
    void path();

    void etcObject();
    void pillar(float height, QColor color, bool loop);
    void wallObject(float height);
    void stairObject(float height, QColor color);
    void floorObject(float height, QColor color);
    void roomObject1(float height, QColor color);
    void roomObject2(float height, QColor color);
    void roomObject3(float height, QColor color);
    void roomObject4(float height, QColor color);
    void roomObject5(float height, QColor color);
    void roomObject6(float height, QColor color);
    void roomObject7(float height, QColor color);
    void roomObject8(float height, QColor color);

    QColor dancolor;
	std::vector<BoxMesh>		m_boxes;
    std::vector<BoxMesh>        m_paths;
	std::vector<Vertex>			m_vertexBufferData;
	std::vector<GLuint>			m_elementBufferData;

	/*! Wraps an OpenGL VertexArrayObject, that references the vertex coordinates and color buffers. */
	QOpenGLVertexArrayObject	m_vao;

	/*! Holds position and colors in a single buffer. */
	QOpenGLBuffer				m_vbo;
	/*! Holds elements. */
	QOpenGLBuffer				m_ebo;

    Transform3D                 trans;
};

#endif // BOXOBJECT_H
