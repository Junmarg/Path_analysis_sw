#include "BoxObject.h"
#include <QVector3D>
#include <QOpenGLShaderProgram>

BoxObject::BoxObject() :
    m_vbo(QOpenGLBuffer::VertexBuffer), // actually the default, so default constructor would have been enough
    m_ebo(QOpenGLBuffer::IndexBuffer) // make this an Index Buffer
{
    // create building & path
    building();
    path();
    // triangleTest();
    unsigned int NBoxes = m_boxes.size();

    // resize storage arrays
    m_vertexBufferData.resize(NBoxes*BoxMesh::VertexCount);
    m_elementBufferData.resize(NBoxes*BoxMesh::IndexCount);

    // update the buffers
    Vertex * vertexBuffer = m_vertexBufferData.data();
    unsigned int vertexCount = 0;
    GLuint * elementBuffer = m_elementBufferData.data();
    for (const BoxMesh & b : m_boxes)
        b.copy2Buffer(vertexBuffer, elementBuffer, vertexCount);
}

void BoxObject::create(QOpenGLShaderProgram * shaderProgramm) {
    // create and bind Vertex Array Object

    m_vao.create();
    m_vao.bind();

    // create and bind vertex buffer
    m_vbo.create();
    m_vbo.bind();
    m_vbo.setUsagePattern(QOpenGLBuffer::StaticDraw);
    int vertexMemSize = m_vertexBufferData.size()*sizeof(Vertex);
    qDebug() << "BoxObject - VertexBuffer size =" << vertexMemSize/1024.0 << "kByte";
    m_vbo.allocate(m_vertexBufferData.data(), vertexMemSize);

    // create and bind element buffer
    m_ebo.create();
    m_ebo.bind();
    m_ebo.setUsagePattern(QOpenGLBuffer::StaticDraw);
    int elementMemSize = m_elementBufferData.size()*sizeof(GLuint);
    qDebug() << "BoxObject - ElementBuffer size =" << elementMemSize/1024.0 << "kByte";
    m_ebo.allocate(m_elementBufferData.data(), elementMemSize);

    // set shader attributes
    // tell shader program we have two data arrays to be used as input to the shaders

    // index 0 = position
    shaderProgramm->enableAttributeArray(0); // array with index/id 0
    shaderProgramm->setAttributeBuffer(0, GL_FLOAT, 0, 3, sizeof(Vertex));
    // index 1 = color
    shaderProgramm->enableAttributeArray(1); // array with index/id 1
    shaderProgramm->setAttributeBuffer(1, GL_FLOAT, offsetof(Vertex, r), 3, sizeof(Vertex));

    // Release (unbind) all
    m_vao.release();
    m_vbo.release();
    m_ebo.release();
}

void BoxObject::destroy() {
    m_vao.destroy();
    m_vbo.destroy();
    m_ebo.destroy();
}

void BoxObject::render() {
    // set the geometry ("position" and "color" arrays)
    m_vao.bind();

    // now draw the cube by drawing individual triangles
    // - GL_TRIANGLES - draw individual triangles via elements
    glDrawElements(GL_TRIANGLES, m_elementBufferData.size(), GL_UNSIGNED_INT, nullptr);
    // release vertices again
    m_vao.release();
}

void BoxObject::building()
{
    etcObject();

    // 외벽, 기둥
    wallObject(0);
//    pillar(0, Qt::black, true);
//    pillar(40, Qt::black, false);
//    pillar(76, Qt::black, false);
//    pillar(112, Qt::black, false);

    // 계단
    stairObject(3.5, Qt::darkGray);
    stairObject(39.5, Qt::darkGray);
    stairObject(75.5, Qt::darkGray);
    stairObject(111.5, Qt::darkGray);

    dancolor = Qt::green;
    // 2층
    floorObject(40, Qt::darkGray);
    roomObject1(40, dancolor);
    roomObject2(40, dancolor);
    roomObject3(40, dancolor);
    roomObject4(40, dancolor);
    roomObject5(40, dancolor);
    roomObject6(40, dancolor);
    roomObject7(40, dancolor);
    roomObject8(40, dancolor);

    // 3층
    floorObject(76, Qt::darkGray);
    roomObject1(76, dancolor);
    roomObject2(76, dancolor);
    roomObject3(76, dancolor);
    roomObject4(76, dancolor);
    roomObject5(76, dancolor);
    roomObject6(76, dancolor);
    roomObject7(76, dancolor);
    roomObject8(76, dancolor);

    // 4층
    floorObject(112, Qt::darkGray);
    roomObject1(112, dancolor);
    roomObject2(112, dancolor);
    roomObject3(112, dancolor);
    roomObject4(112, dancolor);
    roomObject5(112, dancolor);
    roomObject6(112, dancolor);
    roomObject7(112, dancolor);
    roomObject8(112, dancolor);

    // 5층
    floorObject(148, Qt::darkGray);
    roomObject1(148, dancolor);
    roomObject2(148, dancolor);
    roomObject3(148, dancolor);
    roomObject4(148, dancolor);
    roomObject5(148, dancolor);
    roomObject6(148, dancolor);
    roomObject7(148, dancolor);
    roomObject8(148, dancolor);



}

void BoxObject::etcObject()
{
    // bottom
    // BoxMesh b(600, 1, 400, Qt::darkGray);
    // trans.setTranslation(0,0.5,0);
    // b.transform(trans.toMatrix());
    // m_boxes.push_back(b);

    // f_1_a : 1층 외관
    BoxMesh f_1(QVector3D(-81,   0, 90),
                QVector3D( 64,   0, 15),
                QVector3D( 64, 1.5, 15),
                QVector3D(-81, 1.5, 90),
                QVector3D(-81,   0,  0),
                QVector3D( 64,   0,  0),
                QVector3D( 64, 1.5,  0),
                QVector3D(-81, 1.5,  0),
                Qt::darkGray);
    m_boxes.push_back(f_1);

    BoxMesh f_2(QVector3D(-69,   0,   0),
                QVector3D( 64,   0,   0),
                QVector3D( 64, 1.5,   0),
                QVector3D(-69, 1.5,   0),
                QVector3D(-69,   0, -15),
                QVector3D( 64,   0, -15),
                QVector3D( 64, 1.5, -15),
                QVector3D(-69, 1.5, -15),
                Qt::darkGray);
    m_boxes.push_back(f_2);

    BoxMesh f_3(QVector3D(-13,   0, -15),
                QVector3D( 13,   0, -15),
                QVector3D( 13, 1.5, -15),
                QVector3D(-13, 1.5, -15),
                QVector3D(-13,   0, -68.5),
                QVector3D( 13,   0, -68.5),
                QVector3D( 13, 1.5, -68.5),
                QVector3D(-13, 1.5, -68.5),
                Qt::darkGray);
    m_boxes.push_back(f_3);

    BoxMesh f_4(QVector3D(-69,   0, -15),
                QVector3D(-13,   0, -15),
                QVector3D(-13, 1.5, -15),
                QVector3D(-69, 1.5, -15),
                QVector3D(-69,   0, -68.5),
                QVector3D(-13,   0, -68.5),
                QVector3D(-13, 1.5, -68.5),
                QVector3D(-69, 1.5, -68.5),
                Qt::darkGray);
    m_boxes.push_back(f_4);

}

void BoxObject::pillar(float height, QColor color, bool loop)
{
    float gap = 0;
    if(height != 0) gap = 4;

    // 좌측부터
    // 1번 기둥
    BoxMesh f_1(QVector3D(-258,  1, 167),
                QVector3D(-253,  1, 167),
                QVector3D(-253, 40-gap, 167),
                QVector3D(-258, 40-gap, 167),
                QVector3D(-258,  1, 162),
                QVector3D(-253,  1, 162),
                QVector3D(-253, 40-gap, 162),
                QVector3D(-258, 40-gap, 162),
                color);
    trans.setTranslation(0, 0 + height, 0);
    f_1.transform(trans.toMatrix());
    m_boxes.push_back(f_1);

    BoxMesh f_2(QVector3D(-199,  1, 167),
                QVector3D(-194,  1, 167),
                QVector3D(-194, 40-gap, 167),
                QVector3D(-199, 40-gap, 167),
                QVector3D(-199,  1, 162),
                QVector3D(-194,  1, 162),
                QVector3D(-194, 40-gap, 162),
                QVector3D(-199, 40-gap, 162),
                color);
    trans.setTranslation(0, 0 + height, 0);
    f_2.transform(trans.toMatrix());
    m_boxes.push_back(f_2);

    BoxMesh f_3(QVector3D(-258,  1, 65),
                QVector3D(-253,  1, 65),
                QVector3D(-253, 40-gap, 65),
                QVector3D(-258, 40-gap, 65),
                QVector3D(-258,  1, 60),
                QVector3D(-253,  1, 60),
                QVector3D(-253, 40-gap, 60),
                QVector3D(-258, 40-gap, 60),
                color);
    trans.setTranslation(0, 0 + height, 0);
    f_3.transform(trans.toMatrix());
    m_boxes.push_back(f_3);

    // not loop
    if(loop==true)
    {
        BoxMesh f_4(QVector3D(-199,  1, 65),
                    QVector3D(-194,  1, 65),
                    QVector3D(-194, 40-gap, 65),
                    QVector3D(-199, 40-gap, 65),
                    QVector3D(-199,  1, 60),
                    QVector3D(-194,  1, 60),
                    QVector3D(-194, 40-gap, 60),
                    QVector3D(-199, 40-gap, 60),
                    color);
        trans.setTranslation(0, 0 + height, 0);
        f_4.transform(trans.toMatrix());
        m_boxes.push_back(f_4);
    }

    // 2번 기둥
    BoxMesh f_5(QVector3D(-135,  1, 137),
                QVector3D(-130,  1, 137),
                QVector3D(-130, 40-gap, 137),
                QVector3D(-135, 40-gap, 137),
                QVector3D(-135,  1, 132),
                QVector3D(-130,  1, 132),
                QVector3D(-130, 40-gap, 132),
                QVector3D(-135, 40-gap, 132),
                color);
    trans.setTranslation(0, 0 + height, 0);
    f_5.transform(trans.toMatrix());
    m_boxes.push_back(f_5);

    if(loop==true)
    {
        // not loop
        BoxMesh f_6(QVector3D(-135,  1, 65),
                    QVector3D(-130,  1, 65),
                    QVector3D(-130, 40-gap, 65),
                    QVector3D(-135, 40-gap, 65),
                    QVector3D(-135,  1, 60),
                    QVector3D(-130,  1, 60),
                    QVector3D(-130, 40-gap, 60),
                    QVector3D(-135, 40-gap, 60),
                    color);
        trans.setTranslation(0, 0 + height, 0);
        f_6.transform(trans.toMatrix());
        m_boxes.push_back(f_6);

    }

    // 3번 기둥
    BoxMesh f_7(QVector3D(-69,  1, 105),
                QVector3D(-64,  1, 105),
                QVector3D(-64, 40-gap, 105),
                QVector3D(-69, 40-gap, 105),
                QVector3D(-69,  1, 100),
                QVector3D(-64,  1, 100),
                QVector3D(-64, 40-gap, 100),
                QVector3D(-69, 40-gap, 100),
                color);
    trans.setTranslation(0, 0 + height, 0);
    f_7.transform(trans.toMatrix());
    m_boxes.push_back(f_7);

    if(loop==false)
    {
        // not loop
        BoxMesh f_8(QVector3D(-69,  1, 65),
                    QVector3D(-64,  1, 65),
                    QVector3D(-64, 40-gap, 65),
                    QVector3D(-69, 40-gap, 65),
                    QVector3D(-69,  1, 60),
                    QVector3D(-64,  1, 60),
                    QVector3D(-64, 40-gap, 60),
                    QVector3D(-69, 40-gap, 60),
                    color);
        trans.setTranslation(0, 0 + height, 0);
        f_8.transform(trans.toMatrix());
        m_boxes.push_back(f_8);
    }

    if(loop==false)
    {
        // 4번 기둥
        BoxMesh f_9(QVector3D(-5,  1, 77),
                    QVector3D( 0,  1, 77),
                    QVector3D( 0, 40-gap, 77),
                    QVector3D(-5, 40-gap, 77),
                    QVector3D(-5,  1, 72),
                    QVector3D( 0,  1, 72),
                    QVector3D( 0, 40-gap, 72),
                    QVector3D(-5, 40-gap, 72),
                    color);
        trans.setTranslation(0, 0 + height, 0);
        f_9.transform(trans.toMatrix());
        m_boxes.push_back(f_9);
    }
    // 5번 기둥
    BoxMesh f_10(QVector3D(59,  1, 47),
                 QVector3D(64,  1, 47),
                 QVector3D(64, 40-gap, 47),
                 QVector3D(59, 40-gap, 47),
                 QVector3D(59,  1, 42),
                 QVector3D(64,  1, 42),
                 QVector3D(64, 40-gap, 42),
                 QVector3D(59, 40-gap, 42),
                 color);
    trans.setTranslation(0, 0 + height, 0);
    f_10.transform(trans.toMatrix());
    m_boxes.push_back(f_10);

    // 6번 기둥
    BoxMesh f_11(QVector3D(123,  1, 17),
                 QVector3D(128,  1, 17),
                 QVector3D(128, 40-gap, 17),
                 QVector3D(123, 40-gap, 17),
                 QVector3D(123,  1, 12),
                 QVector3D(128,  1, 12),
                 QVector3D(128, 40-gap, 12),
                 QVector3D(123, 40-gap, 12),
                 color);
    trans.setTranslation(0, 0 + height, 0);
    f_11.transform(trans.toMatrix());
    m_boxes.push_back(f_11);

    if(loop==true)
    {
        // not loop
        BoxMesh f_12(QVector3D(123,  1, -55),
                     QVector3D(128,  1, -55),
                     QVector3D(128, 40-gap, -55),
                     QVector3D(123, 40-gap, -55),
                     QVector3D(123,  1, -60),
                     QVector3D(128,  1, -60),
                     QVector3D(128, 40-gap, -60),
                     QVector3D(123, 40-gap, -60),
                     color);
        trans.setTranslation(0, 0 + height, 0);
        f_12.transform(trans.toMatrix());
        m_boxes.push_back(f_12);
    }

    // 7번 기둥
    BoxMesh f_13(QVector3D(187,  1, -13),
                 QVector3D(192,  1, -13),
                 QVector3D(192, 40-gap, -13),
                 QVector3D(187, 40-gap, -13),
                 QVector3D(187,  1, -18),
                 QVector3D(192,  1, -18),
                 QVector3D(192, 40-gap, -18),
                 QVector3D(187, 40-gap, -18),
                 color);
    trans.setTranslation(0, 0 + height, 0);
    f_13.transform(trans.toMatrix());
    m_boxes.push_back(f_13);

    if(loop==true)
    {
        // not loop
        BoxMesh f_14(QVector3D(187,  1,  -95),
                     QVector3D(192,  1,  -95),
                     QVector3D(192, 40-gap,  -95),
                     QVector3D(187, 40-gap,  -95),
                     QVector3D(187,  1, -100),
                     QVector3D(192,  1, -100),
                     QVector3D(192, 40-gap, -100),
                     QVector3D(187, 40-gap, -100),
                     color);
        trans.setTranslation(0, 0 + height, 0);
        f_14.transform(trans.toMatrix());
        m_boxes.push_back(f_14);
    }

    // 8번 기둥
    BoxMesh f_15(QVector3D(251,  1, -43),
                 QVector3D(256,  1, -43),
                 QVector3D(256, 40-gap, -43),
                 QVector3D(251, 40-gap, -43),
                 QVector3D(251,  1, -48),
                 QVector3D(256,  1, -48),
                 QVector3D(256, 40-gap, -48),
                 QVector3D(251, 40-gap, -48),
                 color);
    trans.setTranslation(0, 0 + height, 0);
    f_15.transform(trans.toMatrix());
    m_boxes.push_back(f_15);

    // not loop
    BoxMesh f_16(QVector3D(251,  1, -127),
                 QVector3D(256,  1, -127),
                 QVector3D(256, 40-gap, -127),
                 QVector3D(251, 40-gap, -127),
                 QVector3D(251,  1, -132),
                 QVector3D(256,  1, -132),
                 QVector3D(256, 40-gap, -132),
                 QVector3D(251, 40-gap, -132),
                 color);
    trans.setTranslation(0, 0 + height, 0);
    f_16.transform(trans.toMatrix());
    m_boxes.push_back(f_16);
}

void BoxObject::wallObject(float height)
{
    Q_UNUSED(height);
}

void BoxObject::stairObject(float height, QColor color)
{

    for(int i=0; i<7; i++)
    {
        BoxMesh f_1(13, 3, 3, color);
        trans.setTranslation(6.5, i*3 + height, -70 - (i*3));

        f_1.transform(trans.toMatrix());
        m_boxes.push_back(f_1);
    }

    for(int i=0; i<7; i++)
    {
        BoxMesh f_2(13, 3, 3, color);
        trans.setTranslation(-6.5, 18 + i*3 + height, -88 + (i*3));

        f_2.transform(trans.toMatrix());
        m_boxes.push_back(f_2);
    }

    // 계단 중간 바닥
    BoxMesh f_3(QVector3D(-13, 0, -89.5),
                QVector3D( 13, 0, -89.5),
                QVector3D( 13, 1, -89.5),
                QVector3D(-13, 1, -89.5),
                QVector3D(-13, 0, -120),
                QVector3D( 13, 0, -120),
                QVector3D( 13, 1, -120),
                QVector3D(-13, 1, -120),
                color);
    trans.setTranslation(0, 0 + height + 18.5, 0);
    f_3.transform(trans.toMatrix());
    m_boxes.push_back(f_3);

    // 계단 - 복도 바닥
    BoxMesh f_4(QVector3D(-13, 0, -34.3),
                QVector3D( 13, 0, -46.8),
                QVector3D( 13, 1, -46.8),
                QVector3D(-13, 1, -34.3),
                QVector3D(-13, 0, -68.5),
                QVector3D( 13, 0, -68.5),
                QVector3D( 13, 1, -68.5),
                QVector3D(-13, 1, -68.5),
                color);
    trans.setTranslation(0, 0 + height + 36.5, 0);
    f_4.transform(trans.toMatrix());
    m_boxes.push_back(f_4);
}

void BoxObject::floorObject(float height, QColor color)
{
    // 복도 1
    BoxMesh f_1(QVector3D(-64,0,10), QVector3D(192,0,-113), QVector3D(192,1,-113), QVector3D(-64,1,10),
                QVector3D(-85,0,0), QVector3D(192,0,-132), QVector3D(192,1,-132), QVector3D(-85,1,0), color);
    trans.setTranslation(0, 0 + height, 0);
    f_1.transform(trans.toMatrix());
    m_boxes.push_back(f_1);

    // 복도 2
    BoxMesh f_2(QVector3D(-85,0,0), QVector3D(-85,0,60), QVector3D(-85,1,60), QVector3D(-85,1,0),
                QVector3D(-64,0,10), QVector3D(-64,0,60), QVector3D(-64,1,60), QVector3D(-64,1,10), color);
    trans.setTranslation(0, 0 + height, 0);
    f_2.transform(trans.toMatrix());
    m_boxes.push_back(f_2);

    // 복도 3_1
    BoxMesh f_3(QVector3D(-226,0,60), QVector3D(-85,0,60), QVector3D(-85,1,60), QVector3D(-226,1,60),
                QVector3D(-226,0,43), QVector3D(-85,0,43), QVector3D(-85,1,43), QVector3D(-226,1,43), color);
    trans.setTranslation(0, 0 + height, 0);
    f_3.transform(trans.toMatrix());
    m_boxes.push_back(f_3);

    // 복도 3_2
    BoxMesh f_4(QVector3D(-226,0,75), QVector3D(-194,0,75), QVector3D(-194,1,75), QVector3D(-226,1,75),
                QVector3D(-226,0,60), QVector3D(-194,0,60), QVector3D(-194,1,60), QVector3D(-226,1,60), color);
    trans.setTranslation(0, 0 + height, 0);
    f_4.transform(trans.toMatrix());
    m_boxes.push_back(f_4);


}

void BoxObject::roomObject1(float height, QColor color)
{
    // 1-1
    BoxMesh f_1(QVector3D(-258,0,167), QVector3D(-226,0,167), QVector3D(-226,1,167), QVector3D(-258,1,167),
                QVector3D(-258,0,60), QVector3D(-226,0,60), QVector3D(-226,1,60), QVector3D(-258,1,60), color);
    trans.setTranslation(0, 0 + height, 0);
    f_1.transform(trans.toMatrix());
    m_boxes.push_back(f_1);

    // 1-2
    BoxMesh f_2(QVector3D(-226,0,167), QVector3D(-194,0,167), QVector3D(-194,1,167), QVector3D(-226,1,167),
                QVector3D(-226,0,75), QVector3D(-194,0,75), QVector3D(-194,1,75), QVector3D(-226,1,75), color);
    trans.setTranslation(0, 0 + height, 0);
    f_2.transform(trans.toMatrix());
    m_boxes.push_back(f_2);
}
void BoxObject::roomObject2(float height, QColor color)
{
    // 2
    BoxMesh f_3(QVector3D(-194,0,137), QVector3D(-130,0,137), QVector3D(-130,1,137), QVector3D(-194,1,137),
                QVector3D(-194,0,60), QVector3D(-130,0,60), QVector3D(-130,1,60), QVector3D(-194,1,60), color);
    trans.setTranslation(0, 0 + height, 0);
    f_3.transform(trans.toMatrix());
    m_boxes.push_back(f_3);
}
void BoxObject::roomObject3(float height, QColor color)
{
    // 3
    BoxMesh f_4(QVector3D(-130,0,105), QVector3D(-64,0,105), QVector3D(-64,1,105), QVector3D(-130,1,105),
                QVector3D(-130,0,60), QVector3D(-64,0,60), QVector3D(-64,1,60), QVector3D(-130,1,60), color);
    trans.setTranslation(0, 0 + height, 0);
    f_4.transform(trans.toMatrix());
    m_boxes.push_back(f_4);
}
void BoxObject::roomObject4(float height, QColor color)
{
    // 4
    BoxMesh f_5(QVector3D(-64,0,77), QVector3D(0,0,77), QVector3D(0,1,77), QVector3D(-64,1,77),
                QVector3D(-64,0,10), QVector3D(0,0,-20.7), QVector3D(0,1,-20.7), QVector3D(-64,1,10), color);
    trans.setTranslation(0, 0 + height, 0);
    f_5.transform(trans.toMatrix());
    m_boxes.push_back(f_5);
}
void BoxObject::roomObject5(float height, QColor color)
{
    // 5
    BoxMesh f_6(QVector3D(0,0,47), QVector3D(64,0,47), QVector3D(64,1,47), QVector3D(0,1,47),
                QVector3D(0,0,-20.7), QVector3D(64,0,-51.5), QVector3D(64,1,-51.5), QVector3D(0,1,-20.7), color);
    trans.setTranslation(0, 0 + height, 0);
    f_6.transform(trans.toMatrix());
    m_boxes.push_back(f_6);
}
void BoxObject::roomObject6(float height, QColor color)
{
    // 6
    BoxMesh f_7(QVector3D(64,0,17), QVector3D(128,0,17), QVector3D(128,1,17), QVector3D(64,1,17),
                QVector3D(64,0,-51.5), QVector3D(128,0,-82.3), QVector3D(128,1,-82.3), QVector3D(64,1,-51.5), color);
    trans.setTranslation(0, 0 + height, 0);
    f_7.transform(trans.toMatrix());
    m_boxes.push_back(f_7);
}
void BoxObject::roomObject7(float height, QColor color)
{
    // 7
    BoxMesh f_8(QVector3D(128,0,-13), QVector3D(192,0,-13), QVector3D(192,1,-13), QVector3D(128,1,-13),
                QVector3D(128,0,-82.3), QVector3D(192,0,-113), QVector3D(192,1,-113), QVector3D(128,1,-82.3), color);
    trans.setTranslation(0, 0 + height, 0);
    f_8.transform(trans.toMatrix());
    m_boxes.push_back(f_8);
}
void BoxObject::roomObject8(float height, QColor color)
{
    // 8
    BoxMesh f_9(QVector3D(192,0,-43), QVector3D(256,0,-43), QVector3D(256,1,-43), QVector3D(192,1,-43),
                QVector3D(192,0,-132), QVector3D(256,0,-132), QVector3D(256,1,-132), QVector3D(192,1,-132), color);
    trans.setTranslation(0, 0 + height, 0);
    f_9.transform(trans.toMatrix());
    m_boxes.push_back(f_9);
}

void BoxObject::path()
{

}
