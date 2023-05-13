#ifndef GRAPHICSWIDGET_H
#define GRAPHICSWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QBasicTimer>
#include <QMatrix4x4>
#include <QOpenGLShaderProgram>
#include <math.h>

#include "cubegeometry.h"
#include "planegeometry.h"
#include "shadowmaptarget.h"

class GraphicsWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
public:
    GraphicsWidget(QWidget *parent);

    // QOpenGLWidget interface
protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    // QObject interface
protected:
    void timerEvent(QTimerEvent *event) override;

private:
    const int UPDATE_INTERVAL = 30;

    QBasicTimer timer;

    float lightRoationAngle;

    QVector3D eyePos;
    QVector3D lightInitialPos;

    QVector3D lightPos;
    QMatrix4x4 projection;

    QOpenGLShaderProgram objProgram;
    QOpenGLShaderProgram planeProgram;

    Geometry *cube;
    Geometry *plane;

    ShadowMapTarget *shadowMap;

    void initShaders();
    void initProgram(QOpenGLShaderProgram &program, QString vertName, QString fragName);
    void constructScene();

    void renderShadowMap();
    void renderScene();

    void renderSceneObject(QMatrix4x4 &viewProjMatrix);
    void renderPlaneWithShadow(QMatrix4x4 &viewProjMatrix);
};

#endif // GRAPHICSWIDGET_H
