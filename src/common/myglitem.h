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
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLFramebufferObject>

#include <QMatrix4x4>
#include <QVector3D>
#include <QVector4D>
#include <QString>

#include <vector>

#include "OdaCommon.h"
#include "RxObject.h"
#include "RxObjectImpl.h"
#include "ExSystemServices.h"
#include "StaticRxObject.h"

#include "oda_static_definations.h"

#include "IfcCore.h"
#include "IfcFile.h"
#include "IfcModel.h"
#include "Entities/IfcProduct.h"
#include "Entities/IfcGeometricRepresentationItem.h"
#include "IfcExamplesCommon.h"
#include "IfcGiContext.h"
#include "IfcGsManager.h"
#include "IfcMappedItem.h"
#include "IfcRepresentationMap.h"
#include "Ifc2x3/IfcRepresentationMapAutoImpl.h"
#include "Ifc2x3/IfcShapeRepresentationAutoImpl.h"

#include "TvFactory.h"

#include "FMDataSerialize.h"
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
#include "common/opengl/classes/opengl_material.h"
#include "common/opengl/classes/texture.h"
// #include "controllers/bim_element_controller.h"
#include "models/bim_element.h"
#include "models/bim_parameter.h"
#include "services/geometry/wall_geometry_service.h"
#include "services/geometry/beam_geometry_service.h"
#include "services/geometry/column_geometry_service.h"
#include "services/geometry/slab_geometry_service.h"
// #include "services/geometry/ifc_geometry_service.h"

#include "controllers/ifc_detail_controller.h"
#include "services/geometry/geometry_service_factory.h"

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
    explicit MyGLItem(QQuickItem *parent = nullptr);
    Renderer* createRenderer() const override;

    QString m_currentItem = "-1";


    bool m_moveUp = false;
    bool m_moveDown = false;
    bool m_moveLeft = false;
    bool m_moveRight = false;

    bool m_panUp = false;
    bool m_panDown = false;
    bool m_panLeft = false;
    bool m_panRight = false;

    bool m_zoomIn = false;
    bool m_zoomOut = false;

    int m_lastClickX = -1;
    int m_lastClickY = -1;
    Mesh* mesh = nullptr;
    IFCDetailController* pIfcDetailController;
    // IfcGeometryService* pIfcGeometryService;

    QString m_viewType = "ModelView";

    QList<BIMElement*> bimElementList;
    BIMElement* editableBimElement = nullptr;

public slots:
    void cameraMoveUp();
    void cameraMoveDown();
    void cameraMoveLeft();
    void cameraMoveRight();

    void cameraPanUp();
    void cameraPanDown();
    void cameraPanLeft();
    void cameraPanRight();

    void zoomIn();
    void zoomOut();

    void updateView(QString viewType);

    void requestPick(int x, int y);

    void handlePick(int id);
    void viewIfc();

    void setCurrentItem(QString strCurrentItem);

    Mesh* getMeshptr();
    void updateEditableBimElement(QVariant bimElement);
    void saveEditableBimElement();

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
    QVector3D m_cameraPos = QVector3D(0.0f, 0.0f, 20.0f);

    bool meshInitialized = false;
    bool projectionMatrixInitialized = false;

    QList<Mesh*> m_meshList;
    Camera* m_camera = nullptr;
    Shader* m_shader = nullptr;
    Shader* m_picking_shader = nullptr;
    View* m_view = nullptr;
    QList<OpenGLMaterial*> m_materialList;
    QList<Texture*> m_textureList;

    bool m_pickRequested = false;
    int m_pickX = -1;
    int m_pickY = -1;
    QString m_viewType = "ModelView";

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
