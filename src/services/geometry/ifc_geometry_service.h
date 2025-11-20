#ifndef IFC_GEOMETRY_SERVICE_H
#define IFC_GEOMETRY_SERVICE_H

#include <QObject>
#include "common/opengl/classes/opengl_helper.h"

class OdDbStub {
public:
    OdUInt64 getHandle()
    {
        return 0;
    }
};

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

#include "IfcMappedItem.h"
#include "IfcRepresentationMap.h"
#include "Ifc2x3/IfcRepresentationMapAutoImpl.h"
#include "Ifc2x3/IfcShapeRepresentationAutoImpl.h"


class IfcGeometryService : public QObject
{
    Q_OBJECT
public:
    explicit IfcGeometryService(QObject *parent = nullptr);

    void generateMesh2D(BIMElement* ifcElement, Mesh* mesh);    
    void generateMesh3D(OdIfcFilePtr ifcFilePtr, Mesh* mesh);
    void generateFinalMesh3D(OdDAI::OdBodyVariant bodyContainer, Mesh* mesh);

signals:

private:
    OpenglHelper m_openglHelper;
};

#endif // IFC_GEOMETRY_SERVICE_H
