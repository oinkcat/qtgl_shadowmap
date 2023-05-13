#ifndef SHADOWMAPTARGET_H
#define SHADOWMAPTARGET_H

#include <QOpenGLExtraFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QOpenGLFramebufferObject>
#include <QList>
#include <QVector3D>
#include <QMatrix4x4>

#include "geometry.h"

class ShadowMapTarget : protected QOpenGLExtraFunctions
{
public:
    ShadowMapTarget();

    void addObject(Geometry *obj);
    void setProjectionMatrix(QMatrix4x4 proj);
    void setEyePos(QVector3D pos);

    void render();

    QOpenGLTexture *textureRef() const;

private:
    const int TEXTURE_SIZE = 512;

    GLuint defaultFboId;
    GLuint shadowFboId;

    QOpenGLTexture *texture;
    QOpenGLShaderProgram program;

    QList<Geometry*> objects;
    QVector3D lookAtPos;

    QMatrix4x4 orthoMatrix;
    QMatrix4x4 viewMatrix;

    void initializeObjects();

    void initializeTexture();
    void initializeFbo();
    void initializeProgram();
};

#endif // SHADOWMAPTARGET_H
