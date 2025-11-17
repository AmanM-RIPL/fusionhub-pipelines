#ifndef IFC_GEOMETRY_SERVICE_H
#define IFC_GEOMETRY_SERVICE_H

#include <QObject>
#include "common/opengl/classes/opengl_helper.h"

//#include "IfcCore.h"
//#include "IfcModel.h"
//#include "IfcProduct.h"
//#include "OdaCommon.h"
#include "OdString.h"
#include "daiObjectId.h"
#include "OdArray.h"
#include "Ge/GeVector3dArray.h"

#include "IfcFile.h"
#include "IfcModel.h"
#include "Entities/IfcProduct.h"
#include "Entities/IfcGeometricRepresentationItem.h"


class IfcGeometryService : public QObject
{
    Q_OBJECT
public:
    explicit IfcGeometryService(QObject *parent = nullptr);

    void generateMesh2D(BIMElement* ifcElement, Mesh* mesh);
    void generateMesh3D(OdIfcModelPtr pIfcModel, Mesh* mesh);   

signals:

private:
    OpenglHelper m_openglHelper;
};

#endif // IFC_GEOMETRY_SERVICE_H
