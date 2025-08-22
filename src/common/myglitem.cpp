#include "myglitem.h"
#include <unordered_set>

MyGLRenderer::MyGLRenderer(OdTvDatabaseId databaseId, OdTvModelId mId)
{
    dbId = databaseId;
    modelId = mId;

    try
    {
        qInfo() << "Constructor";

        OdTvResult rc;
        OdTvDatabasePtr pTvDatabase = dbId.openObject(OdTv::kForWrite, &rc);
        OdTvModelPtr modelPtr = modelId.openObject(OdTv::kForWrite, &rc);

        deviceId = pTvDatabase->createDevice(OD_T("Device0"), &rc);
        OdTvGsDevicePtr pDevice0 = deviceId.openObject(OdTv::kForWrite, &rc);

        OdTvGsViewId viewId0 = pDevice0->createView(OD_T("View_0"), true, &rc);
        OdTvGsViewPtr pView0 = viewId0.openObject(OdTv::kForWrite, &rc);

        pView0->addModel(modelId, &rc);
        pView0->setActive(true);
        pView0->setView(OdGePoint3d(20000,-10000,0), OdGePoint3d(0,0,0), OdGeVector3d(0,0,1), 10000, 10000, OdTvGsView::kPerspective);
        rc = pView0->setMode(OdTvGsView::kGouraudShaded);
        rc = pView0->setLensLength(16.0);

        pDevice0->addView(viewId0);

        m_initialized = false;
    }
    catch (...)
    {
        qInfo() << "Crashed";
    }
}

MyGLRenderer::~MyGLRenderer()
{
    try
    {
        qInfo() << "Desctructor";
        // pDatabase.release();
    }
    catch (...)
    {
        qInfo() << "Destructor Crashed";
    }
}

void MyGLRenderer::synchronize(QQuickFramebufferObject *item)
{
    //qInfo() << "Syncronize Function";

    m_item = item;

    MyGLItem* glItem = static_cast<MyGLItem*>(item);
    m_zoom = glItem->getZoom();
    m_orbit = glItem->getMousePressed();

    MouseCoordinate m = glItem->getMouseCoordinates();

    // we find where the movement is more, if in x then xObit, otherwise yOrbit
    double diffX = (m.currentX - m.pressedX) > 0 ? (m.currentX - m.pressedX) : (m.pressedX - m.currentX);
    double diffY = (m.currentY - m.pressedY) > 0 ? (m.currentY - m.pressedY) : (m.pressedY - m.currentY);

    if (diffX < diffY)
    {
        m_xOrbit = (m.currentX - m.pressedX) > 0 ? 0.02 : -0.02;
        m_yOrbit = 0;
    }
    else
    {
        m_xOrbit = 0;
        m_yOrbit = (m.currentY - m.pressedY) > 0 ? 0.02 : -0.02;
    }


    // qInfo() << "Zoom Level: " << m_zoom;
    // qInfo() << "Diff: " << diffX << ", " << diffY;
}

void MyGLRenderer::render() {
    //qInfo() << "Render Function";

    try
    {
        if (!m_initialized) {
            QOpenGLFramebufferObject *fbo = framebufferObject();
            QOpenGLContext* context = QOpenGLContext::currentContext();

            if (fbo) {
                qInfo() << "FBO is there";
                OdTvResult rc;
                OdTvGsDevicePtr pDevice0 = deviceId.openObject(OdTv::kForWrite, &rc);
                rc = pDevice0->setupGsWithContext(context, OdTvDCRect(0, fbo->width(), 0, fbo->height()), OdTvGsDevice::kOpenGL);
                qInfo() << (rc == tvOk);

                m_initialized = true;
            }
        }

        //Resize device if needed
        QOpenGLFramebufferObject *fbo = framebufferObject();
        OdTvResult rc;
        OdTvGsDevicePtr pDevice0 = deviceId.openObject(OdTv::kForWrite, &rc);

        OdTvGsViewId viewId0 = pDevice0->getActiveView(); //->createView(OD_T("View_0"), true, &rc);
        OdTvGsViewPtr pView0 = viewId0.openObject(OdTv::kForWrite, &rc);
        pView0->zoom(m_zoom);

        if (m_orbit)
        {
            // qInfo() << "Render x, y: " << m_xOrbit << "," << m_yOrbit;
            pView0->orbit(m_xOrbit, m_yOrbit);
        }

        QSize currentSize(fbo->width(), fbo->height());
        if (m_lastSize != currentSize) {
            pDevice0->onSize(OdTvDCRect(0, currentSize.width(), 0, currentSize.height()));
            m_lastSize = currentSize;
        }

        pDevice0->update();

    } catch (...)
    {
        qInfo() << "Error in renderer";
    }
}

QOpenGLFramebufferObject *MyGLRenderer::createFramebufferObject(const QSize &size) {
    // qInfo() << "Create Frame Buffer Object";
    QOpenGLFramebufferObjectFormat format;
    format.setAttachment(QOpenGLFramebufferObject::Depth);
    return new QOpenGLFramebufferObject(size, format);
}


QQuickFramebufferObject::Renderer* MyGLItem::createRenderer() const {
    // qInfo() << "Create Renderer";
    return new MyGLRenderer(m_dbId, m_modelId);
}

void MyGLItem::zoomIn()
{
    // qInfo() << "Zoom In";

    // m_zoom += 0.1;
    m_zoom = 1.1;

    update();
}

void MyGLItem::zoomOut()
{
    // qInfo() << "Zoom Out";

    // m_zoom -= 0.1;
    m_zoom = 0.9;

    update();
}

void MyGLItem::mousePressed(double x, double y)
{
    m_mouseIsPressed = true;
    m_pressedMouseX = x;
    m_pressedMouseY = y;
    m_mouseX = x;
    m_mouseY = y;

    // needed to re-set any zoom
    m_zoom = 1;
}

void MyGLItem::mouseReleased()
{
    m_mouseIsPressed = false;
    m_pressedMouseX = 0;
    m_pressedMouseY = 0;
    m_mouseX = 0;
    m_mouseY = 0;

    m_zoom = 1;
}

void MyGLItem::mousePositionChanged(double x, double y)
{
    if (m_mouseIsPressed)
    {
        m_mouseX = x;
        m_mouseY = y;
        m_zoom = 1;

        update();
    }
    else
    {
        m_pressedMouseX = 0;
        m_pressedMouseY = 0;
        m_mouseX = 0;
        m_mouseY = 0;
        m_zoom = 1;
    }
}

void MyGLItem::setDatabaseId(OdTvDatabaseId dbId)
{
    m_dbId = dbId;
}

void MyGLItem::setModelId(OdTvModelId modelId)
{
    m_modelId = modelId;
}

OdTvDatabaseId MyGLItem::dbId()
{
    return m_dbId;
}

OdTvModelId MyGLItem::modelId()
{
    return m_modelId;
}

double MyGLItem::getZoom()
{
    //qInfo() << "Zoom got";
    return m_zoom;
}

MouseCoordinate MyGLItem::getMouseCoordinates()
{
    MouseCoordinate m;
    m.pressedX = m_pressedMouseX;
    m.pressedY = m_pressedMouseY;
    m.currentX = m_mouseX;
    m.currentY = m_mouseY;

    return m;
}

bool MyGLItem::getMousePressed()
{
    return m_mouseIsPressed;
}

OdTvDatabaseId MyGLItem::m_dbId = OdTvDatabaseId();
OdTvModelId MyGLItem::m_modelId = OdTvModelId();
