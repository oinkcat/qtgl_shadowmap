#ifndef PLANEGEOMETRY_H
#define PLANEGEOMETRY_H

#include "geometry.h"

class PlaneGeometry : public Geometry
{
public:
    PlaneGeometry();

    // Geometry interface
public:
    void construct() override;
    void render(QOpenGLShaderProgram *program) override;

private:
    const float SIZE = 10.0f;

    float rotationAngle;
};

#endif // PLANEGEOMETRY_H
