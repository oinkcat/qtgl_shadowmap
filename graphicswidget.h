#ifndef GRAPHICSWIDGET_H
#define GRAPHICSWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QBasicTimer>
#include <QMatrix4x4>
#include <QOpenGLShaderProgram>

#include "cubegeometry.h"
#include "planegeometry.h"

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

    float rotationAngle;
    QVector3D light;
    QMatrix4x4 projection;

    QOpenGLShaderProgram shaderProgram;
    Geometry *plane;
    Geometry *cube;

    void initShaders();
    void constructScene();
};

#endif // GRAPHICSWIDGET_H
