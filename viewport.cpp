#include "viewport.h"
#include <QOpenGLShaderProgram>

#include "stb_image.h"

#include <QMessageBox>
#include <qopenglvertexarrayobject.h>

ViewPort::ViewPort(QWidget* pParent)
    : QOpenGLWidget(pParent)
    , m_Indices { 1, 2, 0, 3 }
    , m_Vertices
      { -1.0f, -1.0f, 0.f, 0.f, 1.f,
        -1.0f,  1.0f, 0.f, 0.f, 0.f,
         1.0f,  1.0f, 0.f, 1.f, 0.f,
         1.0f, -1.0f, 0.f, 1.f, 1.f }

{
}

ViewPort::~ViewPort()
{
    glDeleteBuffers(1, &m_VerticesId);
    delete m_pShaderProgram;
}

void ViewPort::initializeGL()
{
    initializeOpenGLFunctions();
    m_pShaderProgram = new QOpenGLShaderProgram();
    m_pShaderProgram->addShaderFromSourceFile(QOpenGLShader::Vertex, "Shaders/2D.vert");
    m_pShaderProgram->addShaderFromSourceFile(QOpenGLShader::Fragment, "Shaders/2D.frag");
    m_pShaderProgram->link();

    glClearColor(0.f, 0.f, 0.f, 1.f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_COLOR_MATERIAL);

    glGenBuffers(1, &m_VerticesId);
    glBindBuffer(GL_ARRAY_BUFFER, m_VerticesId);
    glBufferData(GL_ARRAY_BUFFER, sizeof(m_Vertices), &m_Vertices, GL_STATIC_DRAW);

    glGenBuffers(1, &m_IndicesId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndicesId);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(m_Indices), &m_Indices, GL_STATIC_DRAW);

    m_TexLocation = m_pShaderProgram->uniformLocation("tex0");
    glUniform1i(m_TexLocation, 0);
}

void ViewPort::paintGL()
{
    int imgW, imgH, numColCh;
    unsigned char* bytes = stbi_load("Tomato.png", &imgW, &imgH, &numColCh, 0);
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imgW, imgH, 0, GL_RGBA, GL_UNSIGNED_BYTE, bytes);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(bytes);

    m_pShaderProgram->bind();
    
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    
    glBindTexture(GL_TEXTURE_2D, texture);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(sizeof(float) * 3));
    glEnableVertexAttribArray(1);

    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, 0);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);

    m_pShaderProgram->release();

    glDeleteTextures(1, &texture);

}

void ViewPort::resizeGL(int w, int h)
{
    const qreal retinaScale = devicePixelRatio();
    glViewport(0, 0, w * retinaScale, h * retinaScale);
}