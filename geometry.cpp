#include "geometry.h"

Geometry::Geometry() : indicesBuf(QOpenGLBuffer::Type::IndexBuffer)
{
    initializeOpenGLFunctions();

    attrsBuf.create();
    indicesBuf.create();
}

Geometry::~Geometry()
{
    attrsBuf.destroy();
    indicesBuf.destroy();
}

void Geometry::setPosition(QVector3D newPos)
{
    pos = newPos;
}

QVector3D Geometry::position() const
{
    return pos;
}

void Geometry::updateRotation()
{
    rotationAngle += 1.0f;
}
