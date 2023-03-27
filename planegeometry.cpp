#include "planegeometry.h"

PlaneGeometry::PlaneGeometry() : Geometry()
{
    construct();
}

void PlaneGeometry::construct()
{
    QVector3D pn(0.0f, 1.0f, 0.0f);

    VertexData vertices[] = {
        {QVector3D(-SIZE, 0.0f, SIZE), pn, QVector3D(1.0f, 0.0f, 0.0f)},
        {QVector3D(SIZE, 0.0f, -SIZE), pn, QVector3D(0.0f, 1.0f, 0.0f)},
        {QVector3D(-SIZE, 0.0f, -SIZE), pn, QVector3D(0.0f, 0.0f, 1.0f)},
        {QVector3D(SIZE, 0.0f, SIZE), pn, QVector3D(0.0f, 1.0f, 0.0f)}
    };

    GLushort indices[] = {
        0, 1, 2,
        0, 3, 1
    };

    // Buffers
    attrsBuf.bind();
    attrsBuf.allocate(vertices, sizeof(vertices));

    indicesBuf.bind();
    indicesBuf.allocate(indices, sizeof(indices));
}

void PlaneGeometry::render(QOpenGLShaderProgram *program)
{
    attrsBuf.bind();
    indicesBuf.bind();

    // Offset for position
    quintptr offset = 0;

    // Vertex positions
    int vertexLocation = program->attributeLocation("a_pos"); // !
    program->enableAttributeArray(vertexLocation);
    program->setAttributeBuffer(vertexLocation, GL_FLOAT, offset, 3, sizeof(VertexData));

    offset += sizeof(QVector3D);

    // Texture coordinates
    int texcoordLocation = program->attributeLocation("a_normal"); // !
    program->enableAttributeArray(texcoordLocation);
    program->setAttributeBuffer(texcoordLocation, GL_FLOAT, offset, 3, sizeof(VertexData));

    offset += sizeof(QVector3D);

    // Vertex colors
    int colorLocation = program->attributeLocation("a_color"); // !
    program->enableAttributeArray(colorLocation);
    program->setAttributeBuffer(colorLocation, GL_FLOAT, offset, 3, sizeof(VertexData));

    // Draw cube geometry using indices from VBO 1
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);
}
