#include "shadowmaptarget.h"

ShadowMapTarget::ShadowMapTarget()
{
    viewMatrix.setToIdentity();

    initializeOpenGLFunctions();
    initializeObjects();
}

void ShadowMapTarget::addObject(Geometry *obj)
{
    objects << obj;
}

void ShadowMapTarget::setEyePos(QVector3D pos)
{
    eyePos = pos;
}

void ShadowMapTarget::initializeObjects()
{
    initializeTexture();
    initializeFbo();
    initializeProgram();
}

void ShadowMapTarget::initializeTexture()
{
    texture = new QOpenGLTexture(QOpenGLTexture::Target::Target2D);
    texture->create();

    texture->setMinificationFilter(QOpenGLTexture::Filter::Linear);
    texture->setMagnificationFilter(QOpenGLTexture::Filter::Nearest);
    texture->setComparisonFunction(QOpenGLTexture::ComparisonFunction::CompareLessEqual);
    texture->setComparisonMode(QOpenGLTexture::ComparisonMode::CompareRefToTexture);
    texture->setWrapMode(QOpenGLTexture::WrapMode::ClampToEdge);

    texture->setSize(TEXTURE_SIZE, TEXTURE_SIZE);
    texture->setFormat(QOpenGLTexture::TextureFormat::D16);

    texture->allocateStorage(QOpenGLTexture::PixelFormat::Depth, QOpenGLTexture::PixelType::UInt16);
}

void ShadowMapTarget::initializeFbo()
{
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, (GLint*)&defaultFboId);
    glGenFramebuffers(1, &shadowFboId);

    glBindFramebuffer(GL_FRAMEBUFFER, shadowFboId);

    GLenum none = GL_NONE;
    glDrawBuffers(1, &none);

    GLuint tid = texture->textureId();
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, tid, 0);

    texture->bind();

    GLenum fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if(fboStatus != GL_FRAMEBUFFER_COMPLETE) {
        throw "Error initializing framebuffer!";
    }

    glBindFramebuffer(GL_FRAMEBUFFER, defaultFboId);
}

void ShadowMapTarget::initializeProgram()
{
    program.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shadow_vertex.vsh");
    program.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shadow_fragment.fsh");

    if(!program.link()) {
        throw "Shadow program link failed!";
    }
}

void ShadowMapTarget::render()
{
    glBindFramebuffer(GL_FRAMEBUFFER, shadowFboId);

    glClear(GL_DEPTH_BUFFER_BIT);
    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

    glEnable(GL_POLYGON_OFFSET_FILL);
    glPolygonOffset(4.0f, 100.0f);

    program.bind();

    orthoMatrix.setToIdentity();
    orthoMatrix.ortho(-10.0f, 10.0f, -10.0f, 10.0f, -20.0f, 20.0f);

    for(auto *obj: objects) {
        QMatrix4x4 modelMatrix;
        modelMatrix.rotate(obj->rotation(), 1.0f, 0.75f, 0.5f);
        modelMatrix.translate(obj->position());

        viewMatrix.setToIdentity();
        viewMatrix.lookAt(eyePos, obj->position(), QVector3D(0.0f, -1.0f, 0.0f));

        QMatrix4x4 mvp = orthoMatrix * viewMatrix * modelMatrix;

        program.setUniformValue("u_mvp", mvp);
        obj->render(&program);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, defaultFboId);
}

QOpenGLTexture *ShadowMapTarget::textureRef() const
{
    return texture;
}
