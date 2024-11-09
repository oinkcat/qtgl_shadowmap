#include "planegeometry.h"

PlaneGeometry::PlaneGeometry() : Geometry()
{
    construct();
}

void PlaneGeometry::construct()
{
    QVector3D pn(0.0f, 1.0f, 0.0f);

    QVector<VertexData> vertices = {
        {QVector3D(-SIZE, 0.0f, SIZE), pn, QVector3D(1.0f, 0.0f, 0.0f)},
        {QVector3D(SIZE, 0.0f, -SIZE), pn, QVector3D(0.0f, 1.0f, 0.0f)},
        {QVector3D(-SIZE, 0.0f, -SIZE), pn, QVector3D(0.0f, 0.0f, 1.0f)},
        {QVector3D(SIZE, 0.0f, SIZE), pn, QVector3D(0.0f, 1.0f, 0.0f)}
    };

    QVector<GLushort> indices = {
        0, 1, 2,
        0, 3, 1
    };

    createBuffers(vertices, indices);
}
