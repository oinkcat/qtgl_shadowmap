#include "graphicswidget.h"

GraphicsWidget::GraphicsWidget(QWidget *parent)
    : QOpenGLWidget(parent),
      eyePos(0.0f, 12.0f, 18.0f),
      lightInitialPos(0.0f, 5.0f, 3.0f)
{
}

void GraphicsWidget::initializeGL()
{
    initializeOpenGLFunctions();

    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    shadowMap = new ShadowMapTarget();

    // Resources
    initShaders();
    constructScene();

    // Update timer
    timer.start(UPDATE_INTERVAL, this);
}

void GraphicsWidget::initShaders()
{
    initProgram(objProgram, ":/obj_vertex.vsh", ":/obj_fragment.fsh");
    initProgram(planeProgram, ":/plane_vertex.vsh", ":/plane_fragment.fsh");
}

void GraphicsWidget::initProgram(QOpenGLShaderProgram &program, QString vertName, QString fragName)
{
    if(!program.addShaderFromSourceFile(QOpenGLShader::Vertex, vertName))
    {
        throw "Vertex shader compilation error!";
    }

    if(!program.addShaderFromSourceFile(QOpenGLShader::Fragment, fragName))
    {
        throw "Fragment shader compilation error!";
    }

    if(!program.link())
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

    // Add objects casting shadow for shadow map render
    shadowMap->addObject(cube);
}

void GraphicsWidget::resizeGL(int w, int h)
{
    const qreal zNear = 2.0, zFar = 30.0, fov = 45.0;
    qreal ratio = (qreal)w / (qreal)h;

    projection.setToIdentity();
    projection.perspective(fov, ratio, zNear, zFar);
}

void GraphicsWidget::timerEvent(QTimerEvent *event)
{
    const float AMP = 2.0f;

    Q_UNUSED(event);

    float newCubeY = sin(lightRoationAngle / 10.0f) * AMP + AMP;
    QVector3D oldPos = cube->position();
    cube->setPosition(QVector3D(oldPos.x(), newCubeY, oldPos.z()));

    cube->updateRotation();

    // Light rotation
    lightRoationAngle += 1.0f;

    QMatrix4x4 lightMatrix;
    lightMatrix.rotate(lightRoationAngle, 0.0f, 1.0f, 0.0f);
    lightPos = lightMatrix * lightInitialPos;

    update();
}

void GraphicsWidget::paintGL()
{
    renderShadowMap();
    renderScene();
}

void GraphicsWidget::renderShadowMap()
{
    shadowMap->setEyePos(lightPos);
    shadowMap->render();
}

void GraphicsWidget::renderScene()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    glDisable(GL_POLYGON_OFFSET_FILL);

    objProgram.bind();

    // Camera view
    QMatrix4x4 viewMatrix;
    viewMatrix.lookAt(eyePos, QVector3D(0.0f, 0.0f, 0.0f), QVector3D(0.0f, 1.0f, 0.0f));
    QMatrix4x4 viewProjMatrix = projection * viewMatrix;

    renderSceneObject(viewProjMatrix);
    renderPlaneWithShadow(viewProjMatrix);
}

void GraphicsWidget::renderSceneObject(QMatrix4x4 &viewProjMatrix)
{
    // Render rotating cube
    QMatrix4x4 cubeRotationMatrix;
    cubeRotationMatrix.rotate(cube->rotation(), 1.0f, 0.75f, 0.5f);

    QMatrix4x4 cubeMatrix;
    cubeMatrix.translate(cube->position());
    cubeMatrix = cubeMatrix * cubeRotationMatrix;
    objProgram.setUniformValue("u_mvp", viewProjMatrix * cubeMatrix);

    // Light for cube
    QMatrix4x4 invCubeRotation = cubeRotationMatrix.inverted();
    objProgram.setUniformValue("u_light", invCubeRotation * lightPos);

    cube->render(&objProgram);
}

void GraphicsWidget::renderPlaneWithShadow(QMatrix4x4 &viewProjMatrix)
{
    planeProgram.bind();
    shadowMap->textureRef()->bind();

    // Render plane
    QMatrix4x4 planeMatrix;
    planeMatrix.translate(plane->position());
    planeProgram.setUniformValue("u_mvp", viewProjMatrix * planeMatrix);

    // Light projection matrix
    QMatrix4x4 lightView;
    lightView.lookAt(lightPos, QVector3D(0.0f, 0.0f, 0.0f), QVector3D(0.0f, 1.0f, 0.0f));

    QMatrix4x4 orthoMatrix;
    orthoMatrix.ortho(-10.0f, 10.0f, -10.f, 10.f, -20.0f, 20.0f);
    QMatrix4x4 lightProjMatrix = orthoMatrix * lightView;
    planeProgram.setUniformValue("u_mvpLight", lightProjMatrix * planeMatrix);

    planeProgram.setUniformValue("s_shadow", 0);

    // Light for plane
    QMatrix4x4 invPlaneTransform = planeMatrix.inverted();
    planeProgram.setUniformValue("u_light", invPlaneTransform * lightPos);

    plane->render(&planeProgram);
}
