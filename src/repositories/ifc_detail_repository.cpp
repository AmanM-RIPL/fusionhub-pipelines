#include "ifc_detail_repository.h"

#include <sdai.h>
#include <IfcModel.h>
#include <Ifc4x3/IfcCartesianPointAutoImpl.h>
#include <GePoint3d.h>
#include <IfcProduct.h>
#include <vector>

#include <IfcRepresentation.h>
#include <IfcShapeRepresentation.h>
#include <IfcRepresentationItem.h>

#include <FMDataSerialize.h>
#include <Modeler/FMMdlFace.h>
#include <Modeler/FMMdlBody.h>
#include <Modeler/FMMdlVertex.h>
#include <Modeler/FMMdlVertexSurfData.h>
#include <Modeler/FMMdlIterators.h>
#include <FMGeometry.h>
#include <IfcModelerGeometry/IfcModelerGeometry.h>
#include <IfcProductRepresentation.h>
#include <IfcGeometricRepresentationItem.h>
#include "common/myglitem.h"

#include <QTemporaryFile>


extern QString gEnvironmentPath;
extern OdStaticRxObject<MyServices> svcs;


IFCDetailRepository::IFCDetailRepository(QObject* parent) : QObject(parent) {
    m_treeModel = nullptr;
}

/*IFCDetailRepository::IFCDetailRepository(QList<IFCDetail*> ifcDetailList, QObject *parent)
    : QObject{parent}
{
    m_ifcDetailList = ifcDetailList;
}*/


bool IFCDetailRepository::save(const IFCDetail& entity)
{
    return false;
}

bool IFCDetailRepository::create(const QString& projectName, const IFCDetail& entity)
{
    QString projectPath = gEnvironmentPath + "\\" + projectName;

    QDir dir;
    if (!dir.mkpath(projectPath)) {
        qDebug() << "Failed to create project folder:" << projectPath;
        return false;
    }

    QString ifcFileName = entity.getName();
    QString ifcFileFullName = ifcFileName + ".ifc";

    OdIfcFilePtr pDb = svcs.createDatabase(kScmIfc4x3);

    if (!pDb.isNull())
    {
        initHeader(pDb);
        fillModel(pDb);

        QString ifcFilePath =  projectPath + "\\" + ifcFileFullName;
        QByteArray utf8ByteArray = ifcFilePath.toUtf8();
        pDb->writeFile(OdString(utf8ByteArray.constData()), true);
        pDb.release();
        pDb = NULL;
        return true;
    }
    return false;
}


QList<IFCDetail*> IFCDetailRepository::getIFCDetails()
{
    return m_ifcDetailList;
}

QString IFCDetailRepository::getIfcFilePath(const QString& projectName, const QString& ifcFileName)
{
    if(projectName.isEmpty() || ifcFileName.isEmpty()){
        return "";
    }

    QString projectPath = gEnvironmentPath + "\\" + projectName;

    QDir dir;
    if (!dir.mkpath(projectPath)) {
        qDebug() << "Failed to create project folder:" << projectPath;
        return "";
    }

    QString ifcFileFullName = ifcFileName + ".ifc";
    QString ifcFilePath =  projectPath + "\\" + ifcFileFullName;

    return ifcFilePath;
}

QList<IFCDetail*> IFCDetailRepository::loadIFC(const QString& qstrFilePath)
{
    //Here need to populate m_ifcDetailList
    QList<IFCDetail*> ifcDetailList;

    if(qstrFilePath.isEmpty()){
        return ifcDetailList;
    }    

    QByteArray utf8ByteArray = qstrFilePath.toUtf8();
    OdString ifcFileName = OdString(utf8ByteArray.constData());

    QFile file(qstrFilePath);
    if (file.exists()) {
         qDebug() << "Ifc file exists.";
    }
    else
    {
        qDebug() << "Error: No ifc file exist.";
        return ifcDetailList;
    }

    OdIfcFilePtr pDatabase;
    OdIfcModelPtr pIfcModel;
    try {
        pDatabase = svcs.createDatabase();

        if (!pDatabase) {
            qDebug() << "Error: createDatabase returned a null pointer.";            
        }

        OdResult result = pDatabase->readFile(ifcFileName);

        if (result != tvOk) {
            qDebug() << "IFCDatabase Loaded: Failed to read file with OdResult: ";
        }
        else{
            pIfcModel = pDatabase->getModel();

            if (pIfcModel.isNull()) {
                qDebug() << "IFCDatabase Loaded: Failed to retrieve IFC model after successful read.";
            }
            else {
                qDebug() << "IFCDatabase Loaded: Success!";
            }
        }

    } catch (const OdError& e) {
       // qDebug() << "IFCDatabase Loaded: Failed with exception: " << e.description();
   }

   /*
   OdDAI::InstanceIteratorPtr it = pIfcModel->newIterator();
   unsigned int entIdx;
   for (entIdx = 0; !it->done(); it->step(), ++entIdx)
   {
       OdIfc::OdIfcInstancePtr  pInst;
       //opens an entity
       pInst = it->id().openObject();
       if (!pInst.isNull())
       {
           odPrintConsoleString(OD_T("Entity %d: \n"), entIdx);
           odPrintConsoleString(OD_T("\tEntity handle (corresponds to the STEP-id) = %d\n"), it->id().getHandle());
           odPrintConsoleString(OD_T("\tEntity type code = %d\n"), pInst->type());
           odPrintConsoleString(OD_T("\tEntity type name = %hs\n"), pInst->typeName().c_str());
           if (pInst->isKindOf(OdIfc::kIfcRepresentationItem) || pInst->isKindOf(OdIfc::kIfcProfileDef))
           {
               odPrintConsoleString(OD_T("\tEntity is kind of kGeom\n"));
               OdArray<OdGePoint3d> p =  getCartesianPoints(pInst);
           }
           else
           {
               odPrintConsoleString(OD_T("\tEntity is kind of kBIM\n"));               
           }
       }
   }   
   //Finalize the process, OdIfcFile and underlying header section and Model will be released.
   */

   pDatabase.release();
   pDatabase = NULL;

   pIfcModel.release();
   pIfcModel = NULL;

   for(IFCDetail* obj : ifcDetailList) {
       m_ifcDetailList.append(obj);
   }

   return ifcDetailList;
}

OdIfcFilePtr IFCDetailRepository::getIfcFilePtrFromLoadedIFC(const QString& qstrFilePath)
{
    QByteArray utf8ByteArray = qstrFilePath.toUtf8();
    OdString ifcFileName = OdString(utf8ByteArray.constData());

    QFile file(qstrFilePath);
    if (file.exists()) {
        qDebug() << "Ifc file exists.";
    }
    else
    {
        qDebug() << "Error: No ifc file exist.";
        return nullptr;
    }

    OdIfcFilePtr ifcFilePtr;
    try {
        ifcFilePtr = svcs.createDatabase();

        if (!ifcFilePtr) {
            qDebug() << "Error: createDatabase returned a null pointer.";
        }

        OdResult result = ifcFilePtr->readFile(ifcFileName);
        if (result != tvOk) {
            qDebug() << "IFCDatabase Loaded: Failed to read file with OdResult: ";
            return nullptr;
        }

    }
    catch (const OdError& e){
        qDebug() << "IFCDatabase Loaded: Failed with exception: " << e.description();
        return nullptr;
    }

    return ifcFilePtr;
}


OdIfcModelPtr IFCDetailRepository::getModelptrFromLoadedIFC(const QString& qstrFilePath)
{
    QByteArray utf8ByteArray = qstrFilePath.toUtf8();
    OdString ifcFileName = OdString(utf8ByteArray.constData());

     QFile file(qstrFilePath);
     if (file.exists()) {
         qDebug() << "Ifc file exists.";
     }
     else
     {
         qDebug() << "Error: No ifc file exist.";
     }

     OdIfcFilePtr pDatabase;
     OdIfcModelPtr odIfcModelPtr;
     try {
         pDatabase = svcs.createDatabase();

         if (!pDatabase) {
             qDebug() << "Error: createDatabase returned a null pointer.";
         }

         OdResult result = pDatabase->readFile(ifcFileName);

         if (result != tvOk) {
             qDebug() << "IFCDatabase Loaded: Failed to read file with OdResult: ";
         }
         else{

            OdIfcModelContext& modelContext = pDatabase->getContext();

            //Choose what kinds of entities to compose (optional, but recommended)
            modelContext.getGeometryComposeTypes().append(OdIfc::kIfcProduct);
            //modelContext.getGeometryComposeTypes().push_back(OdIfc::kIfcProduct);

            //Set geometry options
            modelContext.setComposeOutOfSpatialStructure(true);
            modelContext.setDrawOpenings(true);
            modelContext.setDrawPoints(true);
            modelContext.setDrawSpaces(true);

            //Compose IFC geometry
            OdResult composeR =  pDatabase->composeEntities();

            odIfcModelPtr = pDatabase->getModel();
            if (!odIfcModelPtr.isNull()) {
                return odIfcModelPtr;
            }
            else{
                qDebug() << "IFCDatabase Loaded: Failed to retrieve IFC model after successful read.";
            }
        }
     }
     catch (const OdError& e){
         qDebug() << "IFCDatabase Loaded: Failed with exception: " << e.description();
     }
    return odIfcModelPtr;
}

TreeModel* IFCDetailRepository::getTreeModel()
{
    return m_treeModel;
}

void IFCDetailRepository::fillModel(OdIfcFile *pIfcFile)
{
    if (pIfcFile == NULL)
    {
        throw OdError(eNullPtr);
    }    

    OdIfcModelPtr model = pIfcFile->getModel(sdaiRW);

    if (model.isNull() || model->underlyingSchemaName().find("IFC4X3") != 0)
    {
        ODA_FAIL_M("Model is not valid.");
        throw OdError(eNullEntityPointer);
    }

    OdDAIObjectId handleOwnerHistory = OdIfc4x3_add2::Utils::createOwnerHistory(model);


    OdDAIObjectId direction_6 = OdIfc4x3_add2::Utils::createDirection(model, 5., 1., 5.);
    OdDAIObjectId direction_7 = OdIfc4x3_add2::Utils::createDirection(model, 1., 0., 0.);
    OdDAIObjectId direction_8 = OdIfc4x3_add2::Utils::createDirection(model, 5., 5., 1.);


    OdDAIObjectId cartesian_9 = OdIfc4x3_add2::Utils::createCartesianPoint(model, 0, 0, 0);
    OdDAIObjectId axis2Placement3D_10 = OdIfc4x3_add2::Utils::createAxis2Placement3D(model, cartesian_9, direction_8, direction_7);


    OdDAIObjectId representationContext_11 = OdIfc4x3_add2::Utils::createGeometricRepresentationContext(model, "Plan", "Model", 3, 0.00001, axis2Placement3D_10, direction_6);
    OdDAIObjectId dimensionalExponents_12 = OdIfc4x3_add2::Utils::createDimensionalExponents(model, 0, 0, 0, 0, 0, 0, 0);

    OdDAIObjectId siunit_13 = OdIfc4x3_add2::Utils::createSIUnit(model, OdIfc4x3_add2::kIfcUnitEnum_LENGTHUNIT, OdIfc4x3_add2::kIfcSIPrefix_MILLI, OdIfc4x3_add2::kIfcSIUnitName_METRE, NULL);
    OdDAIObjectId siunit_14 = OdIfc4x3_add2::Utils::createSIUnit(model, OdIfc4x3_add2::kIfcUnitEnum_PLANEANGLEUNIT, OdIfc4x3_add2::kIfcSIPrefix_unset, OdIfc4x3_add2::kIfcSIUnitName_RADIAN, NULL);

    OdDAIObjectId measureWithUnit_15 = OdIfc4x3_add2::Utils::createMeasureWithUnit(model, 0.0174532925, siunit_14);
    OdDAIObjectId conversionBasedUnit_16 = OdIfc4x3_add2::Utils::createConversionBasedUnit(model, OD_T("Degrees"), measureWithUnit_15, dimensionalExponents_12, OdIfc4x3_add2::kIfcUnitEnum_PLANEANGLEUNIT);

    OdDAIObjectIds  unitsCollection_17;
    unitsCollection_17.push_back(siunit_13);
    unitsCollection_17.push_back(conversionBasedUnit_16);
    OdDAIObjectId unitAssignment_17 = OdIfc4x3_add2::Utils::createUnitAssignment(model, unitsCollection_17);


    OdDAIObjectIds  representationContext_18;
    representationContext_18.push_back(representationContext_11);
    OdDAIObjectId project_18 = OdIfc4x3_add2::Utils::createProject(model, OD_T("Sample project made with ODA ExIfcModelFiller app"), representationContext_18, unitAssignment_17, handleOwnerHistory);

    OdDAIObjectId direction_19 = OdIfc4x3_add2::Utils::createDirection(model, 1, 0, 0);
    OdDAIObjectId direction_20 = OdIfc4x3_add2::Utils::createDirection(model, 0, 0, 1);
    OdDAIObjectId cartesian_21 = OdIfc4x3_add2::Utils::createCartesianPoint(model, 0, 0, 0);

    OdDAIObjectId axis2Placement3D_22 = OdIfc4x3_add2::Utils::createAxis2Placement3D(model, cartesian_21, direction_20, direction_19);
    OdDAIObjectId localPlacement_23 = OdIfc4x3_add2::Utils::createLocalPlacement(model, axis2Placement3D_22);

    OdDAIObjectId site_24 = OdIfc4x3_add2::Utils::createSite(model, localPlacement_23, handleOwnerHistory, OdIfc4x3_add2::kIfcElementCompositionEnum_ELEMENT);

    OdDAIObjectIds  relatedObjectCollection_25;
    relatedObjectCollection_25.push_back(site_24);
    OdDAIObjectId relAggregates_25 = OdIfc4x3_add2::Utils::createRelAggregates(model, handleOwnerHistory, project_18, relatedObjectCollection_25);

    OdDAIObjectId direction_26 = OdIfc4x3_add2::Utils::createDirection(model, 1., 0., 0.);
    OdDAIObjectId direction_27 = OdIfc4x3_add2::Utils::createDirection(model, 0., 0., 1.);
    OdDAIObjectId cartesian_28 = OdIfc4x3_add2::Utils::createCartesianPoint(model, 0., 0., 0.);

    OdDAIObjectId axis2Placement3D_29 = OdIfc4x3_add2::Utils::createAxis2Placement3D(model, cartesian_28, direction_27, direction_26);
    OdDAIObjectId localPlacement_30 = OdIfc4x3_add2::Utils::createLocalPlacement(model, axis2Placement3D_29, &localPlacement_23);

    OdDAIObjectId building_31 = OdIfc4x3_add2::Utils::createBuilding(model, handleOwnerHistory, localPlacement_30, OdIfc4x3_add2::kIfcElementCompositionEnum_ELEMENT);

    OdDAIObjectIds  relatedObjectCollection_32;
    relatedObjectCollection_32.push_back(building_31);
    OdDAIObjectId relAggregates_32 = OdIfc4x3_add2::Utils::createRelAggregates(model, handleOwnerHistory, site_24, relatedObjectCollection_32);

    OdDAIObjectId direction_33 = OdIfc4x3_add2::Utils::createDirection(model, 1, 0, 0);
    OdDAIObjectId direction_34 = OdIfc4x3_add2::Utils::createDirection(model, 0, 0, 1);
    OdDAIObjectId cartesian_35 = OdIfc4x3_add2::Utils::createCartesianPoint(model, 0, 0, 0);

    OdDAIObjectId axis2Placement3D_36 = OdIfc4x3_add2::Utils::createAxis2Placement3D(model, cartesian_35, direction_34, direction_33);
    OdDAIObjectId localPlacement_37 = OdIfc4x3_add2::Utils::createLocalPlacement(model, axis2Placement3D_36, &localPlacement_30);

    OdDAIObjectId buildingStorey_38 = OdIfc4x3_add2::Utils::createBuildingStorey(model, handleOwnerHistory, localPlacement_37, OdIfc4x3_add2::kIfcElementCompositionEnum_ELEMENT);

    OdDAIObjectIds  relatedObjectCollection_39;
    relatedObjectCollection_39.push_back(buildingStorey_38);
    OdDAIObjectId relAggregates_39 = OdIfc4x3_add2::Utils::createRelAggregates(model, handleOwnerHistory, building_31, relatedObjectCollection_39);

    OdDAIObjectId buildingElementProxy_40 = OdIfc4x3_add2::Utils::createBuildingElementProxy(model, handleOwnerHistory);
    OdIfc4x3_add2::Utils::LocalPlacementPostSetter<OdIfc4x3_add2::IfcBuildingElementProxy> localPlacementSetter_40(model, buildingElementProxy_40);
    OdIfc4x3_add2::Utils::ProductDefinitionShapeSetter<OdIfc4x3_add2::IfcBuildingElementProxy> productDefinitionShapeSetter_40(model, buildingElementProxy_40);

    OdDAIObjectIds  relatedElements_41;
    relatedElements_41.push_back(buildingElementProxy_40);
    OdDAIObjectId ispetialStructure_41 = OdIfc4x3_add2::Utils::createRelContainedInspatialStructure(model, handleOwnerHistory, relatedElements_41, buildingStorey_38);

    OdDAIObjectId direction_42 = OdIfc4x3_add2::Utils::createDirection(model, 1, 0, 0);
    OdDAIObjectId direction_43 = OdIfc4x3_add2::Utils::createDirection(model, 0, 0, 1);
    OdDAIObjectId cartesian_44 = OdIfc4x3_add2::Utils::createCartesianPoint(model, 0, 0, 0);


    OdDAIObjectId axis2Placement3D_45 = OdIfc4x3_add2::Utils::createAxis2Placement3D(model, cartesian_44, direction_43, direction_42);
    OdDAIObjectId localPlacement_46 = OdIfc4x3_add2::Utils::createLocalPlacement(model, axis2Placement3D_45, NULL);

    localPlacementSetter_40.Set(localPlacement_46);

    OdDAIObjectId direction_47 = OdIfc4x3_add2::Utils::createDirection(model, 1, 0);
    OdDAIObjectId cartesian_48 = OdIfc4x3_add2::Utils::createCartesianPoint(model, 0, 0);

    OdDAIObjectId axis2Placement_49 = OdIfc4x3_add2::Utils::createAxis2Placement2D(model, cartesian_48, direction_47);

    OdDAIObjectId direction_50 = OdIfc4x3_add2::Utils::createDirection(model, 0, 0, 1);
    OdDAIObjectId direction_51 = OdIfc4x3_add2::Utils::createDirection(model, 1, 0, 0);
    OdDAIObjectId direction_52 = OdIfc4x3_add2::Utils::createDirection(model, 0, 0, 1);
    OdDAIObjectId cartesian_53 = OdIfc4x3_add2::Utils::createCartesianPoint(model, 0, 0, 0);
    OdDAIObjectId axis2Placement3D_54 = OdIfc4x3_add2::Utils::createAxis2Placement3D(model, cartesian_53, direction_52, direction_51);

    OdDAIObjectId iShapeProfileDef_55 = OdIfc4x3_add2::Utils::createIShapeProfileDef(model, OdIfc4x3_add2::kIfcProfileTypeEnum_AREA, axis2Placement_49, 50., 25., 5., 2.);

    OdDAIObjectId extrudedAreaSolid_56 = OdIfc4x3_add2::Utils::createExtrudedAreaSolid(model, iShapeProfileDef_55, axis2Placement3D_54, direction_50, 20);

    OdDAIObjectIds  representationItemsCollection_57;
    representationItemsCollection_57.push_back(extrudedAreaSolid_56);
    OdDAIObjectId shapeRepresentation_57 = OdIfc4x3_add2::Utils::createShapeRepresentation(model, representationContext_11, "Body", "SweptSolid", representationItemsCollection_57);

    OdDAIObjectIds  represntationCollection_58;
    represntationCollection_58.push_back(shapeRepresentation_57);
    OdDAIObjectId productDefinitionShape_58 = OdIfc4x3_add2::Utils::createProductDefinitionShape(model, represntationCollection_58);

    productDefinitionShapeSetter_40.Set(productDefinitionShape_58);

    OdDAIObjectId buildingElementProxy_59 = OdIfc4x3_add2::Utils::createBuildingElementProxy(model, handleOwnerHistory);
    OdIfc4x3_add2::Utils::LocalPlacementPostSetter<OdIfc4x3_add2::IfcBuildingElementProxy> localPlacementSetter_59(model, buildingElementProxy_59);
    OdIfc4x3_add2::Utils::ProductDefinitionShapeSetter<OdIfc4x3_add2::IfcBuildingElementProxy> productDefinitionShapeSetter_59(model, buildingElementProxy_59);


    OdDAIObjectIds  relatedElements_60;
    relatedElements_60.push_back(buildingElementProxy_59);
    OdDAIObjectId ispetialStructure_60 = OdIfc4x3_add2::Utils::createRelContainedInspatialStructure(model, handleOwnerHistory, relatedElements_60, buildingStorey_38);

    OdDAIObjectId direction_61 = OdIfc4x3_add2::Utils::createDirection(model, 1, 0, 0);
    OdDAIObjectId direction_62 = OdIfc4x3_add2::Utils::createDirection(model, 0, 0, 1);
    OdDAIObjectId cartesian_63 = OdIfc4x3_add2::Utils::createCartesianPoint(model, 100, 0, 0);

    OdDAIObjectId axis2Placement3D_64 = OdIfc4x3_add2::Utils::createAxis2Placement3D(model, cartesian_63, direction_62, direction_61);
    OdDAIObjectId localPlacement_65 = OdIfc4x3_add2::Utils::createLocalPlacement(model, axis2Placement3D_64, NULL);
    localPlacementSetter_59.Set(localPlacement_65);

    OdDAIObjectId direction_66 = OdIfc4x3_add2::Utils::createDirection(model, 1, 0);
    OdDAIObjectId cartesian_67 = OdIfc4x3_add2::Utils::createCartesianPoint(model, 0, 0);
    OdDAIObjectId axis2Placement_68 = OdIfc4x3_add2::Utils::createAxis2Placement2D(model, cartesian_67, direction_66);

    OdDAIObjectId direction_69 = OdIfc4x3_add2::Utils::createDirection(model, 0, 0, 1);
    OdDAIObjectId direction_70 = OdIfc4x3_add2::Utils::createDirection(model, 1, 0, 0);
    OdDAIObjectId direction_71 = OdIfc4x3_add2::Utils::createDirection(model, 0, 0, 1);
    OdDAIObjectId cartesian_72 = OdIfc4x3_add2::Utils::createCartesianPoint(model, 0, 0, 0);
    OdDAIObjectId axis2Placement3D_73 = OdIfc4x3_add2::Utils::createAxis2Placement3D(model, cartesian_72, direction_71, direction_70);


    const double radius = 2.;
    OdDAIObjectId iShapeProfileDef_74 = OdIfc4x3_add2::Utils::createIShapeProfileDef(model, OdIfc4x3_add2::kIfcProfileTypeEnum_AREA, axis2Placement_68, 50., 25., 5., 5., &radius);

    OdDAIObjectId extrudedAreaSolid_75 = OdIfc4x3_add2::Utils::createExtrudedAreaSolid(model, iShapeProfileDef_74, axis2Placement3D_73, direction_69, 20);

    OdDAIObjectIds  representationItemsCollection_76;
    representationItemsCollection_76.push_back(extrudedAreaSolid_75);
    OdDAIObjectId shapeRepresentation_76 = OdIfc4x3_add2::Utils::createShapeRepresentation(model, representationContext_11, "Body", "SweptSolid", representationItemsCollection_76);

    OdDAIObjectIds  represntationCollection_77;
    represntationCollection_77.push_back(shapeRepresentation_76);
    OdDAIObjectId productDefinitionShape_77 = OdIfc4x3_add2::Utils::createProductDefinitionShape(model, represntationCollection_77);
    productDefinitionShapeSetter_59.Set(productDefinitionShape_77);
}


void IFCDetailRepository::initHeader(OdIfcFile *pDb)
{
    if (pDb == NULL)
    {
        throw OdError(eNullPtr);
    }

    OdDAI::OdHeaderSectionPtr headerSection = pDb->getHeaderSection();

    if (headerSection.isNull())
    {
        ODA_ASSERT(0 && "Header is not valid.");
        headerSection->initialize();
    }

    OdDAI::FileDescription* fileDescription = getHeaderFromSection<OdDAI::FileDescription, OdDAI::kFileDescription>(headerSection);
    OdArray<OdAnsiString> descriptionCollection;
    descriptionCollection.push_back("ViewDefinition [CoordinationView]");
    fileDescription->setDescription(descriptionCollection);

    OdDAI::FileName* fileName = getHeaderFromSection<OdDAI::FileName, OdDAI::kFileName>(headerSection);
    OdAnsiStringArray arrAuthor;
    arrAuthor.append("ExIfcModelFiller example");
    fileName->setAuthor(arrAuthor);
    OdAnsiStringArray arrOrganization;
    arrOrganization.append("ODA");
    fileName->setOrganization(arrOrganization);
}


