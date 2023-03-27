#include "graphicswidget.h"

GraphicsWidget::GraphicsWidget(QWidget *parent)
    : QOpenGLWidget(parent),
      light(1.0f, 1.0f, 1.0f)
{
}

void GraphicsWidget::initializeGL()
{
    initializeOpenGLFunctions();

    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    // Resources
    initShaders();
    constructScene();

    // Update timer
    timer.start(UPDATE_INTERVAL, this);
}

void GraphicsWidget::initShaders()
{
    if(!shaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/vertex.vsh"))
    {
        throw "Vertex shader compilation error!";
    }

    if(!shaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/fragment.fsh"))
    {
        throw "Fragment shader compilation error!";
    }

    if(!shaderProgram.link())
    {
        throw "Shader link error!";
    }
}

void GraphicsWidget::constructScene()
{
    cube = new CubeGeometry();
    cube->setPosition(QVector3D(0.0f, 1.0f, 0.0f));

    plane = new PlaneGeometry();
    plane->setPosition(QVector3D(0.0f, -1.0f, 0.0f));
}

void GraphicsWidget::resizeGL(int w, int h)
{
    const qreal zNear = 2.0, zFar = 25.0, fov = 45.0;
    qreal ratio = (qreal)w / (qreal)h;

    projection.setToIdentity();
    projection.perspective(fov, ratio, zNear, zFar);
}

void GraphicsWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shaderProgram.bind();

    // Camera view
    QMatrix4x4 viewMatrix;
    QVector3D eyePos(0.0f, 12.0f, 18.0f);
    viewMatrix.lookAt(eyePos, QVector3D(0.0f, 0.0f, 0.0f), QVector3D(0.0f, 1.0f, 0.0f));

    QMatrix4x4 viewProjMatrix = projection * viewMatrix;

    // Render rotating cube
    QMatrix4x4 cubeMatrix;
    cubeMatrix.translate(cube->position());
    cubeMatrix.rotate(rotationAngle, 1.0f, 1.0f, 1.0f);
    shaderProgram.setUniformValue("u_mvp", viewProjMatrix * cubeMatrix);

    // Light for cube
    QMatrix4x4 invCubeTransform = cubeMatrix.inverted();
    shaderProgram.setUniformValue("u_light", invCubeTransform * light);

    cube->render(&shaderProgram);

    // Render plane
    QMatrix4x4 planeMatrix;
    planeMatrix.translate(plane->position());

    shaderProgram.setUniformValue("u_mvp", viewProjMatrix * planeMatrix);

    // Light for plane
    QMatrix4x4 invPlaneTransform = planeMatrix.inverted();
    shaderProgram.setUniformValue("u_light", invPlaneTransform * light);

    plane->render(&shaderProgram);
}

void GraphicsWidget::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event);

    rotationAngle += 1.0f;
    update();
}
