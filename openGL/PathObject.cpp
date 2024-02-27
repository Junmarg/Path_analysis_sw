#include "PathObject.h"
#include <QVector3D>
#include <QOpenGLShaderProgram>

PathObject::PathObject() :
    m_vbo(QOpenGLBuffer::VertexBuffer), // actually the default, so default constructor would have been enough
    m_ebo(QOpenGLBuffer::IndexBuffer) // make this an Index Buffer
{
    // create building & path
    floor_path_01(Qt::red);
    // triangleTest();
    unsigned int NBoxes = f_path_01.size();

    // resize storage arrays
    f_vbd_01.resize(NBoxes*BoxMesh::VertexCount);
    f_ebd_01.resize(NBoxes*BoxMesh::IndexCount);
    object_num = BoxMesh::VertexCount/12;

    // update the buffers
    Vertex * vertexBuffer = f_vbd_01.data();
    unsigned int vertexCount = 0;
    GLuint * elementBuffer = f_ebd_01.data();
    for (const BoxMesh & b : f_path_01)
        b.copy2Buffer(vertexBuffer, elementBuffer, vertexCount);
}

void PathObject::create(QOpenGLShaderProgram * shaderProgramm) {
    // create and bind Vertex Array Object

    m_vao.create();
    m_vao.bind();

    // create and bind vertex buffer
    m_vbo.create();
    m_vbo.bind();
    m_vbo.setUsagePattern(QOpenGLBuffer::StaticDraw);
    int vertexMemSize = f_vbd_01.size()*sizeof(Vertex);
    qDebug() << "BoxObject - VertexBuffer size =" << vertexMemSize/1024.0 << "kByte";
    m_vbo.allocate(f_vbd_01.data(), vertexMemSize);


    // create and bind element buffer
    m_ebo.create();
    m_ebo.bind();
    m_ebo.setUsagePattern(QOpenGLBuffer::StaticDraw);
    int elementMemSize = f_ebd_01.size()*sizeof(GLuint);
    qDebug() << "BoxObject - ElementBuffer size =" << elementMemSize/1024.0 << "kByte";
    m_ebo.allocate(f_ebd_01.data(), elementMemSize);


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

void PathObject::destroy() {
    m_vao.destroy();
    m_vbo.destroy();
    m_ebo.destroy();
}

void PathObject::render(QVector<int> path2render) {

    // memory -> 1.125kByte

    // ALL
    //m_vao.bind();
    //glDrawElements(GL_TRIANGLES, f_ebd_01.size(), GL_UNSIGNED_INT, nullptr);
    //m_vao.release();

    object_num = 228;

    // 노드 간 Object가 2개인 메모리 번호 등록
    QVector<int> double_object_number = {9, 16, 31, 33, 35, 37, 39, 41, 43, 45, 47, 49, 51, 53, 55, \
                                         66, 73, 88, 90, 92, 94, 96, 98, 100, 102, 104, 106, 108, 110, 112, \
                                         123, 130, 145, 147, 149, 151, 153, 155, 157, 159, 161, 163, 165, 167, 169, \
                                         180, 187, 202, 204, 206, 208, 210, 212, 214, 216, 218, 220, 222, 224, 226, 228};

    for(int i=0; i<path2render.size(); i++)
    {
        if(path2render[i]==0)
        {
            // room01 -> floor01
            m_vao.bind();
            glDrawElements(GL_TRIANGLES, f_ebd_01.size()/object_num, GL_UNSIGNED_INT, nullptr);
            m_vao.release();
        }
        else if(double_object_number.indexOf(path2render[i]) > 0)
        {
            m_vao.bind();
            glDrawElements(GL_TRIANGLES, f_ebd_01.size()/object_num*2, GL_UNSIGNED_INT, (const void*)(f_ebd_01.size()/object_num*path2render[i]*sizeof(unsigned int)));
            m_vao.release();
        }
        else
        {
            m_vao.bind();
            glDrawElements(GL_TRIANGLES, f_ebd_01.size()/object_num, GL_UNSIGNED_INT, (const void*)(f_ebd_01.size()/object_num*path2render[i]*sizeof(unsigned int)));
            m_vao.release();
        }
    }
}

void PathObject::floor_path_01(QColor color)
{
    // 화살표 개당 memory -> 1.125kByte
    // height 간격 => append 36
    // 메모리에 노드 간 화살표 등록

    for(int i=0; i<4; i++)
    {
        float height = i*36;

        object_source(0, 0,   0, 1, color, position={-210,   height,   60}, rotate={  0, 0.0f, 0.0f, 0.0f});        // room01 -> floor01
        object_source(0, 0,   0, 1, color, position={-162,   height,   60}, rotate={  0, 0.0f, 0.0f, 0.0f});        // room02 -> floor02
        object_source(0, 0,   0, 1, color, position={ -32,   height,   -5}, rotate={  0, 0.0f, 0.0f, 0.0f});        // room03 -> floor04
        object_source(0, 0,   0, 1, color, position={  32,   height,  -35}, rotate={  0, 0.0f, 0.0f, 0.0f});        // room04 -> floor05
        object_source(0, 0,   0, 1, color, position={  96,   height,  -65}, rotate={  0, 0.0f, 0.0f, 0.0f});        // room05 -> floor06
        object_source(0, 0,   0, 1, color, position={ 160,   height,  -95}, rotate={  0, 0.0f, 0.0f, 0.0f});        // room06 -> floor07
        object_source(0, 0, -10, 1, color, position={ 192,   height, -120}, rotate={ 90, 0.0f, 1.0f, 0.0f});        // room07 -> floor07

        object_source(0, 0,   0, 1, color, position={-170,   height,   52}, rotate={-90, 0.0f, 1.0f, 0.0f});        // floor01 -> floor02
        object_source(0, 0,  40, 1, color, position={ -80,   height,   52}, rotate={-90, 0.0f, 1.0f, 0.0f});        // floor02 -> floor03
        object_source(0, 0,   0, 1, color, position={ -76,   height,   12}, rotate={  0, 0.0f, 1.0f, 0.0f});        // floor03 -> floor04 part_01 // 9
        object_source(0, 0,   0, 1, color, position={ -42,   height,  -10}, rotate={-65, 0.0f, 1.0f, 0.0f});        // floor03 -> floor04 part_02
        object_source(0, 0,  20, 1, color, position={  22,   height,  -40}, rotate={-65, 0.0f, 1.0f, 0.0f});        // floor04 -> floor05
        object_source(0, 0,  20, 1, color, position={  86,   height,  -70}, rotate={-65, 0.0f, 1.0f, 0.0f});        // floor05 -> floor06
        object_source(0, 0,  20, 1, color, position={ 150,   height, -100}, rotate={-65, 0.0f, 1.0f, 0.0f});        // floor06 -> floor07

        object_source(0, 0,   0, 1, color, position={-202,   height,   52}, rotate={  90, 0.0f, 1.0f, 0.0f});       // floor02 -> floor01
        object_source(0, 0,  40, 1, color, position={-156,   height,   52}, rotate={  90, 0.0f, 1.0f, 0.0f});       // floor03 -> floor02
        object_source(0, 0,   0, 1, color, position={ -74,   height,   42}, rotate={ 180, 0.0f, 1.0f, 0.0f});       // floor04 -> floor03 part_01 // 16
        object_source(0, 0,   0, 1, color, position={ -67,   height,    0}, rotate={ 115, 0.0f, 1.0f, 0.0f});       // floor04 -> floor03 part_02
        object_source(0, 0,  20, 1, color, position={ -24,   height,  -20}, rotate={ 115, 0.0f, 1.0f, 0.0f});       // floor05 -> floor04
        object_source(0, 0,  20, 1, color, position={  38,   height,  -50}, rotate={ 115, 0.0f, 1.0f, 0.0f});       // floor06 -> floor05
        object_source(0, 0,  20, 1, color, position={ 100,   height,  -80}, rotate={ 115, 0.0f, 1.0f, 0.0f});       // floor07 -> floor06

        object_source(0, 0,   0, 1, color, position={-210,   height,  100}, rotate={ 180, 0.0f, 1.0f, 0.0f});       // floor01 -> room01
        object_source(0, 0,   0, 1, color, position={-162,   height,  100}, rotate={ 180, 0.0f, 1.0f, 0.0f});       // floor02 -> room02
        object_source(0, 0,   0, 1, color, position={ -32,   height,   35}, rotate={ 180, 0.0f, 1.0f, 0.0f});       // floor04 -> room03
        object_source(0, 0,   0, 1, color, position={  32,   height,   -5}, rotate={ 180, 0.0f, 1.0f, 0.0f});       // floor05 -> room04
        object_source(0, 0,   0, 1, color, position={  96,   height,  -35}, rotate={ 180, 0.0f, 1.0f, 0.0f});       // floor06 -> room05
        object_source(0, 0,   0, 1, color, position={ 160,   height,  -65}, rotate={ 180, 0.0f, 1.0f, 0.0f});       // floor07 -> room06
        object_source(0, 0,   0, 1, color, position={ 195,   height, -123}, rotate={ -65, 0.0f, 1.0f, 0.0f});       // floor07 -> room07

        object_source(0, 0,  10, 1, color, position={ -76,   height,  100}, rotate={ 180, 0.0f, 1.0f, 0.0f});       // floor03 -> terrace01
        object_source(0, 0, -10, 1, color, position={ -76,   height,   60}, rotate={   0, 0.0f, 0.0f, 0.0f});       // terrace01 -> floor03
        object_source(0, 0, -10, 1, color, position={ -76,  65 +height,  150}, rotate={ 210, 1.0f, 0.0f, 0.0f});    // terrace01 -> exit08

        object_source(0, 0, -10, 1, color, position={ -10,   height,  -25}, rotate={ -65, 0.0f, 1.0f, 0.0f});       // floor04 -> stair02 part_01 // 31
        object_source(0, 0, -10, 1, color, position={   0,   height,  -60}, rotate={   0, 0.0f, 1.0f, 0.0f});       // floor04 -> stair02 part_02

        object_source(0, 0, -10, 1, color, position={  10,   height,  -35}, rotate={ 115, 0.0f, 1.0f, 0.0f});       // floor05 -> stair02 part_01 // 33
        object_source(0, 0, -10, 1, color, position={   0,   height,  -60}, rotate={   0, 0.0f, 1.0f, 0.0f});       // floor05 -> stair02 part_02

        object_source(0, 0, -10, 1, color, position={ -24,   height,  -20}, rotate={ 115, 0.0f, 1.0f, 0.0f});       // stair02 -> floor03 part_01 // 35
        object_source(0, 0, -10, 1, color, position={   0,   height,  -40}, rotate={ 180, 0.0f, 1.0f, 0.0f});       // stair02 -> floor03 part_02

        object_source(0, 0, -15, 1, color, position={  22,   height,  -40}, rotate={ -65, 0.0f, 1.0f, 0.0f});       // stair02 -> floor04 part_01 // 37
        object_source(0, 0, -10, 1, color, position={   0,   height,  -40}, rotate={ 180, 0.0f, 1.0f, 0.0f});       // stair02 -> floor04 part_02

        object_source(0, 0, -10, 1, color, position={  -6,  -3 + height,  -60}, rotate={ -45, 1.0f, 0.0f, 0.0f});    // stair02 -> stair01 part_01 // 39
        object_source(0, 0, -10, 1, color, position={   6,  45 + height,  -40}, rotate={-140, 1.0f, 0.0f, 0.0f});    // stair02 -> stair01 part_02

        object_source(0, 0, -10, 1, color, position={   6,  30 + height,  -115}, rotate={ 45, 1.0f, 0.0f, 0.0f});    // stair02 -> stair03 part_01 // 41
        object_source(0, 0, -10, 1, color, position={  -6, 115 + height,  -100}, rotate={140, 1.0f, 0.0f, 0.0f});    // stair02 -> stair03 part_02

        object_source(0, 0,  20, 1, color, position={-210,   height,  170}, rotate={ 180, 0.0f, 1.0f, 0.0f});        // room01 -> exit01 part_01 // 43
        object_source(0, 0,  -5, 1, color, position={-210,  65 + height,  230}, rotate={ 210, 1.0f, 0.0f, 0.0f});    // room01 -> exit01 part_02

        object_source(0, 0,   0, 1, color, position={-162,   height,  150}, rotate={ 180, 0.0f, 1.0f, 0.0f});        // room02 -> exit02 part_01 // 45
        object_source(0, 0,  -5, 1, color, position={-162,  65 + height,  210}, rotate={ 210, 1.0f, 0.0f, 0.0f});    // room02 -> exit02 part_02

        object_source(0, 0,   0, 1, color, position={ -32,   height,   85}, rotate={ 180, 0.0f, 1.0f, 0.0f});        // room03 -> exit03 part_01 // 47
        object_source(0, 0,  -5, 1, color, position={ -32,  65 + height,  144}, rotate={ 210, 1.0f, 0.0f, 0.0f});    // room03 -> exit03 part_02

        object_source(0, 0,  10, 1, color, position={  32,   height,   65}, rotate={ 180, 0.0f, 1.0f, 0.0f});        // room04 -> exit04 part_01 // 49
        object_source(0, 0,  -5, 1, color, position={  32,  65 + height,  125}, rotate={ 210, 1.0f, 0.0f, 0.0f});    // room04 -> exit04 part_02

        object_source(0, 0,  10, 1, color, position={  96,   height,   25}, rotate={ 180, 0.0f, 1.0f, 0.0f});        // room05 -> exit05 part_01 // 51
        object_source(0, 0,  -5, 1, color, position={  96,  65 + height,   85}, rotate={ 210, 1.0f, 0.0f, 0.0f});    // room05 -> exit05 part_02

        object_source(0, 0,  10, 1, color, position={ 160,   height,   -5}, rotate={ 180, 0.0f, 1.0f, 0.0f});        // room06 -> exit06 part_01 // 53
        object_source(0, 0,  -5, 1, color, position={ 160,  65 + height,   55}, rotate={ 210, 1.0f, 0.0f, 0.0f});    // room06 -> exit06 part_02

        object_source(0, 0,  40, 1, color, position={ 210,   height,  -30}, rotate={ 180, 0.0f, 1.0f, 0.0f});        // room07 -> exit07 part_01 // 55
        object_source(0, 0,  -5, 1, color, position={ 210,  65 + height,   30}, rotate={ 210, 1.0f, 0.0f, 0.0f});    // room07 -> exit07 part_02
    }

    object_source(0, 0,  70, 1, color, position={ -40, -36,   30}, rotate={ 155, 0.0f, 1.0f, 0.0f});    // stair01 -> exit00 part_01 // 228
    object_source(0, 0,  30, 1, color, position={ -10, -36,   95}, rotate={ 210, 0.0f, 1.0f, 0.0f});    // stair01 -> exit00 part_02

}

void PathObject::object_source(float width=0, float height=0, float depth=0, int source=0, QColor color=Qt::red, QVector<float> get_position={0,0,0}, QVector<float> get_rotate={0, 0.0f, 0.0f, 0.0f})
{
    // source 1 : arrow

    if(source==1)
    {
        BoxMesh f1(QVector3D(-2+width, 42+height,  35+depth),
                   QVector3D( 2+width, 42+height,  35+depth),
                   QVector3D( 2+width, 46+height,  35+depth),
                   QVector3D(-2+width, 46+height,  35+depth),
                   QVector3D(-2+width, 42+height,   0),
                   QVector3D( 2+width, 42+height,   0),
                   QVector3D( 2+width, 46+height,   0),
                   QVector3D(-2+width, 46+height,   0),
                   color);

        BoxMesh f2(QVector3D(-5+width, 42+height,  0),
                   QVector3D( 5+width, 42+height,  0),
                   QVector3D( 5+width, 46+height,  0),
                   QVector3D(-5+width, 46+height,  0),
                   QVector3D( 0+width, 42+height, -5),
                   QVector3D( 0+width, 42+height, -5),
                   QVector3D( 0+width, 46+height, -5),
                   QVector3D( 0+width, 46+height, -5),
                   color);

        trans.setTranslation(get_position[0], get_position[1], get_position[2]);
        trans.rotate(get_rotate[0], QVector3D(get_rotate[1], get_rotate[2], get_rotate[3]));
        f1.transform(trans.toMatrix());
        f2.transform(trans.toMatrix());
        f_path_01.push_back(f1);
        f_path_01.push_back(f2);

        //reset
        position.clear();
        rotate.clear();
        trans.setTranslation(0, 0, 0);
        trans.rotate(get_rotate[0] * (-1), QVector3D(get_rotate[1], get_rotate[2], get_rotate[3]));
    }
}
