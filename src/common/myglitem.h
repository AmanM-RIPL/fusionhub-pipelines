#ifndef MYGLITEM_H
#define MYGLITEM_H

#include <QObject>
#include <QDebug>
#include <QQuickFramebufferObject>
#include <QOpenGLFunctions>
#include <QtGui/QOpenGLContext>
#include <QOpenGLFramebufferObjectFormat>
#include <QtGui/QGuiApplication>
#include <QtGui/QWindow>
#include <QtQuick/QQuickWindow>
#include <windows.h>
#include <QAbstractItemModel>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLFramebufferObject>

#include <QMatrix4x4>
#include <QVector3D>
#include <QVector4D>

#include "OdaCommon.h"
#include "RxObject.h"
#include "RxObjectImpl.h"
#include "ExSystemServices.h"
#include "StaticRxObject.h"
#include "IfcCore.h"
#include "IfcFile.h"
#include "IfcModel.h"
#include "Entities/IfcProduct.h"
#include "Entities/IfcGeometricRepresentationItem.h"
#include "IfcExamplesCommon.h"
#include "IfcGiContext.h"
#include "IfcGsManager.h"
#include "TvFactory.h"
#include "FMMdlBody.h"
#include "FMMdlFace.h"
#include "FMMdlSurface.h"
#include "FMMdlEdge.h"
#include "FMMdlVertex.h"
#include "FMMdlIterators.h"
#include "Ge/GeTrMeshSimplification.h"

#include "common/opengl/classes/mesh.h"
#include "common/opengl/classes/camera.h"
#include "common/opengl/classes/shader.h"
#include "common/opengl/classes/view.h"

class MyApp : public ExSystemServices
{
protected:
    ODRX_USING_HEAP_OPERATORS(ExSystemServices);

public:
    MyApp() {}
};

class MyGLItem : public QQuickFramebufferObject
{
    Q_OBJECT
public:
    Renderer* createRenderer() const override;

    bool m_moveUp = false;
    bool m_moveDown = false;
    bool m_moveLeft = false;
    bool m_moveRight = false;
    int m_lastClickX = -1;
    int m_lastClickY = -1;

public slots:
    void cameraMoveUp();
    void cameraMoveDown();
    void cameraMoveLeft();
    void cameraMoveRight();

    void requestPick(int x, int y);

    void handlePick(int id);

private:

signals:
    void selectionChanged(int id);
};

class MyGLRenderer : public QQuickFramebufferObject::Renderer, protected QOpenGLFunctions_3_3_Core
{
public:
    MyGLRenderer();
    ~MyGLRenderer();

    void synchronize(QQuickFramebufferObject *item) override;
    void render() override;
    QOpenGLFramebufferObject* createFramebufferObject(const QSize &size) override;
    void update();

private:
    GLuint m_vbo = 0;
    GLuint m_vao = 0;
    GLuint m_shaderProgram = 0;
    GLuint m_modelLoc = -1;
    GLuint m_viewLoc = -1;
    GLuint m_projLoc = -1;
    GLfloat m_vertices[9];
    QVector3D m_cameraPos = QVector3D(0.0f, 0.0f, 2.0f);

    Mesh* m_mesh = nullptr;
    Camera* m_camera = nullptr;
    Shader* m_shader = nullptr;
    Shader* m_picking_shader = nullptr;
    View* m_view = nullptr;

    bool m_pickRequested = false;
    int m_pickX = -1;
    int m_pickY = -1;

    GLuint m_pickProgram = 0;
    GLuint m_pickColorLoc = -1;
    GLuint m_pickModelLoc = -1;
    GLuint m_pickViewLoc = -1;
    GLuint m_pickProjLoc = -1;
    GLuint m_pickFBO = 0;
    GLuint m_pickColorTex = 0;
    GLuint m_pickDepthBuf = 0;

    void initGL();
    void initShaders();
    GLuint compileShader(GLenum type, const char* src);
    void ensurePickFBO(int w, int h);
    void encodeIdToColor(unsigned int id, unsigned char &r, unsigned char &g, unsigned char &b);
    void moveTopVertexToClick(int mouseX, int mouseY, const QMatrix4x4 &proj, const QMatrix4x4 &view, const QMatrix4x4 &model);
};


#endif // MYGLITEM_H
