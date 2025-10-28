#ifndef IFC_FILLER_EXTENDER_H
#define IFC_FILLER_EXTENDER_H
///
#include "ExPrintConsole.h"
#include "IfcCore.h"
#include "IfcEntityTypes.h"
#include "IfcEntity.h"
#include "Ifc4x3/Ifc4x3Entities.h"
#include "Ifc4x3/Ifc4x3SelectTypes.h"
#include "Ifc4x3/Ifc4x3Enum.h"


#include<Ifc4x3/Ifc4x3BuildOption.h>
#include<Ifc4x3/IfcSiteAutoImpl.h>
#include<Ifc4x3/Ifc4x3EntityTypes.h>
#include<Ifc4x3/Ifc4x3SchemaSelectTypes.h>
#include<Ifc4x3/IfcOwnerHistoryAutoImpl.h>


#include "daiConsts.h"
#include "daiSettings.h"
#include "daiUtils/daiTextEncodeDecode.h"
#include "daiUtils/daiEmbededTypeUtils.h"
#include "daiObjectId.h"


namespace OdIfc4x3_add2
{
namespace Utils
{

template<class EntityType>
inline OdSmartPtr<EntityType> CreateEntity(OdIfcModelPtr model)
{
    OdSmartPtr<EntityType> inst =  EntityType::createObject(model);
    if(!inst.isNull()){
        SdaiAppInstance root = reinterpret_cast<SdaiAppInstance>(inst.get());
        if (sdaiIsKindOfBN(root, "IfcRoot") == sdaiTRUE)
        {
            OdDAI::CompressedGUID guid = OdDAI::CompressedGUID::generate();
            SdaiString globalId = guid;
            sdaiPutAttrBN(root, "GlobalId", sdaiSTRING, globalId);
        }
    }
    return inst;
}

inline OdDAIObjectId  createOwnerHistory(OdIfcModel *model)
{
    // Use static method createObject from entity instance classes
    OdIfc4x3_add2::IfcPersonPtr personData = CreateEntity<OdIfc4x3_add2::IfcPerson>(model);
    OdIfc4x3_add2::IfcOrganizationPtr organizationData = CreateEntity<OdIfc4x3_add2::IfcOrganization>(model);

    const char applicationName[] = "ODA ExIfcModelFiller";

    organizationData->setName(applicationName);

    OdIfc4x3_add2::IfcPersonAndOrganizationPtr personAndOrganizationData = CreateEntity<OdIfc4x3_add2::IfcPersonAndOrganization>(model);
    personAndOrganizationData->setTheOrganization(organizationData->id());
    personAndOrganizationData->setThePerson(personData->id());

    OdIfc4x3_add2::IfcApplicationPtr applicationData = CreateEntity<OdIfc4x3_add2::IfcApplication>(model);
    applicationData->setApplicationDeveloper(organizationData->id());

    const char applicationVersion[] = TD_SHORT_STRING_VER_S;
    applicationData->setVersion(applicationVersion); // provide version current SDK
    applicationData->setApplicationFullName(applicationName);
    applicationData->setApplicationIdentifier(applicationName);

    OdIfc4x3_add2::IfcOwnerHistoryPtr ownerHistoryData = CreateEntity<OdIfc4x3_add2::IfcOwnerHistory>(model);
    ownerHistoryData->setOwningUser(personAndOrganizationData->id());
    ownerHistoryData->setOwningApplication(applicationData->id());
    ownerHistoryData->setChangeAction(OdIfc4x3_add2::kIfcChangeActionEnum_ADDED);
    ownerHistoryData->setLastModifyingUser(personAndOrganizationData->id());
    ownerHistoryData->setLastModifyingApplication(applicationData->id());
    ownerHistoryData->setCreationDate(0);

    return ownerHistoryData->id();
}

inline OdDAIObjectId  createDirection(OdIfcModel *model, double x, double y, double z = OdDAI::Consts::OdNan)
{
    OdIfc4x3_add2::IfcDirectionPtr direction = CreateEntity<OdIfc4x3_add2::IfcDirection>(model);

    OdArray<double> arrRatios;
    arrRatios.reserve(OdDAI::Utils::isUnset(z) ? 2 : 3);
    arrRatios.append(x);
    arrRatios.append(y);
    if (!OdDAI::Utils::isUnset(z))
        arrRatios.append(z);
    direction->directionRatios().setArray(arrRatios);

    return direction->id();
}

inline OdDAIObjectId  createCartesianPoint(OdIfcModelPtr model, double x, double y, double z = OdDAI::Consts::OdNan)
{
    OdIfc4x3_add2::IfcCartesianPointPtr cartesianPoint = CreateEntity<OdIfc4x3_add2::IfcCartesianPoint>(model);

    OdArray<double> arrCoords;
    arrCoords.reserve(OdDAI::Utils::isUnset(z) ? 2 : 3);
    arrCoords.append(x);
    arrCoords.append(y);
    if (!OdDAI::Utils::isUnset(z))
        arrCoords.append(z);
    cartesianPoint->coordinates().setArray(arrCoords);

    return cartesianPoint->id();
}

inline OdDAIObjectId  createAxis2Placement3D(OdIfcModelPtr model,
                                            const OdDAIObjectId& location,
                                            const OdDAIObjectId& axis,
                                            const OdDAIObjectId& direction)
{
    OdIfc4x3_add2::IfcAxis2Placement3DPtr entityData = CreateEntity<OdIfc4x3_add2::IfcAxis2Placement3D>(model);

    entityData->setAxis(axis);
    entityData->setRefDirection(direction);
    entityData->setLocation(location);

    return entityData->id();
}

inline OdDAIObjectId  createGeometricRepresentationContext(OdIfcModelPtr model,
                                                          const char *contextIdentifier,
                                                          const char *contextType,
                                                          OdInt16 dimensionsCount,
                                                          double precision,
                                                          const OdDAIObjectId& coordinateSystem,
                                                          const OdDAIObjectId& trueNorth)
{
    OdIfc4x3_add2::IfcGeometricRepresentationContextPtr repContextData = CreateEntity<OdIfc4x3_add2::IfcGeometricRepresentationContext>(model);

    OdIfc4x3_add2::IfcAxis2Placement   axis2Placement = repContextData->worldCoordinateSystem();
    axis2Placement.select().setHandle(coordinateSystem);

    repContextData->setContextIdentifier(contextIdentifier);
    repContextData->setContextType(contextType);

    repContextData->setTrueNorth(trueNorth);
    repContextData->setPrecision(precision);
    repContextData->setCoordinateSpaceDimension(dimensionsCount);

    return repContextData->id();
}

inline OdDAIObjectId  createDimensionalExponents(OdIfcModelPtr model,
                                                OdInt32 lengthExponent,
                                                OdInt32 massExponent,
                                                OdInt32 timeExponent,
                                                OdInt32 electricCurrentExponent,
                                                OdInt32 thermodynamicTemperatureExponent,
                                                OdInt32 amountOfSubstanceExponent,
                                                OdInt32 luminousIntensityExponent)
{
    OdIfc4x3_add2::IfcDimensionalExponentsPtr entityData = CreateEntity<OdIfc4x3_add2::IfcDimensionalExponents>(model);

    entityData->setLengthExponent(lengthExponent);
    entityData->setMassExponent(massExponent);
    entityData->setTimeExponent(timeExponent);
    entityData->setElectricCurrentExponent(electricCurrentExponent);
    entityData->setThermodynamicTemperatureExponent(thermodynamicTemperatureExponent);
    entityData->setAmountOfSubstanceExponent(amountOfSubstanceExponent);
    entityData->setLuminousIntensityExponent(luminousIntensityExponent);    

    return entityData->id();
}

inline OdDAIObjectId  createSIUnit(OdIfcModelPtr model,
                                  OdIfc4x3_add2::IfcUnitEnum  unitType,
                                  OdIfc4x3_add2::IfcSIPrefix  prefix,
                                  OdIfc4x3_add2::IfcSIUnitName name,
                                  const OdDAIObjectId &dimensions)
{
    OdIfc4x3_add2::IfcSIUnitPtr entityData = CreateEntity<OdIfc4x3_add2::IfcSIUnit>(model);

    entityData->setUnitType(unitType);
    entityData->setPrefix(prefix);
    entityData->setName(name);
    if (dimensions)
    {
        entityData->setDimensions(dimensions);
    }

    return entityData->id();
}

inline OdDAIObjectId  createMeasureWithUnit(OdIfcModelPtr model,
                                           double angleMeasure,
                                           const OdDAIObjectId& unit)
{
    OdIfc4x3_add2::IfcMeasureWithUnitPtr entityData = CreateEntity<OdIfc4x3_add2::IfcMeasureWithUnit>(model);

    OdIfc4x3_add2::IfcUnit unitWrapper = entityData->unitComponent();
    unitWrapper.select().setHandle(unit);

    OdIfc4x3_add2::IfcValue valueComponent = entityData->valueComponent();
    valueComponent.setIfcPlaneAngleMeasure(angleMeasure);

    return entityData->id();
}


inline OdDAIObjectId  createConversionBasedUnit(OdIfcModelPtr model,
                                               const OdString& name,
                                               const OdDAIObjectId& conversionFactor,
                                               const OdDAIObjectId& dimensions,
                                               const OdIfc4x3_add2::IfcUnitEnum& unitType)
{
    OdIfc4x3_add2::IfcConversionBasedUnitPtr entityData = CreateEntity<OdIfc4x3_add2::IfcConversionBasedUnit>(model);

    OdAnsiString  encodedName;
    if (!OdDAI::Utils::encodeText(name, encodedName, OdDAI::Utils::CodecType::utf8))
    {
        throw OdError(eSyntaxError);
    }

    entityData->setName(encodedName);
    entityData->setConversionFactor(conversionFactor);
    entityData->setDimensions(dimensions);
    entityData->setUnitType(unitType);

    return entityData->id();
}


inline OdDAIObjectId  createUnitAssignment(OdIfcModelPtr model,
                                          const OdArray<OdDAIObjectId>& unitsCollection)
{
    OdIfc4x3_add2::IfcUnitAssignmentPtr entityData = CreateEntity<OdIfc4x3_add2::IfcUnitAssignment>(model);

    OdDAIObjectIds::const_iterator nextUnitHandle = unitsCollection.begin();

    OdDAI::Set<OdDAI::Select>&  unitsWrapperCollection = entityData->units();
    if (unitsWrapperCollection.empty())
    {
        unitsWrapperCollection.createEmpty();
    }

    auto schema = model->underlyingSchema();
    OdDAI::NamedTypePtr simpleValue = schema->types()->getAt("ifcunit");
    auto selectTypeToInit = OdDAI::Utils::extractEmbeddedType<OdDAI::SelectType>(simpleValue);

    for (; nextUnitHandle != unitsCollection.end(); ++nextUnitHandle)
    {
        OdDAI::Select unitWrapper(selectTypeToInit);

        if (unitWrapper.setHandle(*nextUnitHandle))
            unitsWrapperCollection.Add(unitWrapper);
    }

    return entityData->id();
}

inline OdDAIObjectId  createProject(OdIfcModelPtr model,
                                   const OdString& longName,
                                   const OdDAIObjectIds& representationContexts,
                                   const OdDAIObjectId& unitsInContext,
                                   const OdDAIObjectId& ownerHistory)
{
    OdIfc4x3_add2::IfcProjectPtr entityData = CreateEntity<OdIfc4x3_add2::IfcProject>(model);

    OdAnsiString  encodedLongName;

    if (!OdDAI::Utils::encodeText(longName, encodedLongName, OdDAI::Utils::CodecType::utf8))
    {
        throw OdError(eSyntaxError);
    }

    entityData->setLongName(encodedLongName);
    entityData->setUnitsInContext(unitsInContext);
    entityData->representationContexts().setArray(representationContexts);
    entityData->setOwnerHistory(ownerHistory);

    return entityData->id();
}

inline OdDAIObjectId  createLocalPlacement(OdIfcModelPtr model,
                                          const OdDAIObjectId& axis2PlacementRelative,
                                          const OdDAIObjectId* pPlacementRelatesTo = NULL)
{
    OdIfc4x3_add2::IfcLocalPlacementPtr entityData = CreateEntity<OdIfc4x3_add2::IfcLocalPlacement>(model);

    OdIfc4x3_add2::IfcAxis2Placement axisPlacement = entityData->relativePlacement();
    axisPlacement.select().setHandle(axis2PlacementRelative);

    if (pPlacementRelatesTo)
    {
        entityData->setPlacementRelTo(*pPlacementRelatesTo);
    }

    return entityData->id();
}

inline OdDAIObjectId  createSite(OdIfcModelPtr model,
                                const OdDAIObjectId& objectPlacement,
                                const OdDAIObjectId& ownerHistory,
                                OdIfc4x3_add2::IfcElementCompositionEnum compositionType)
{
    OdIfc4x3_add2::IfcSitePtr entityData = CreateEntity<OdIfc4x3_add2::IfcSite>(model);

    entityData->setObjectPlacement(objectPlacement);
    entityData->setCompositionType(compositionType);
    entityData->setOwnerHistory(ownerHistory);
    entityData->setName("Site");

    return entityData->id();
}

inline OdDAIObjectId  createRelAggregates(OdIfcModelPtr model,
                                         const OdDAIObjectId& ownerHistory,
                                         const OdDAIObjectId& relatingObject,
                                         const OdDAIObjectIds& relatedObjectCollection)
{
    OdIfc4x3_add2::IfcRelAggregatesPtr entityData = CreateEntity<OdIfc4x3_add2::IfcRelAggregates>(model);

    entityData->setRelatingObject(relatingObject);
    entityData->relatedObjects().setArray(relatedObjectCollection);
    entityData->setOwnerHistory(ownerHistory);

    return entityData->id();
}

inline OdDAIObjectId  createBuilding(OdIfcModelPtr model,
                                    const OdDAIObjectId& ownerHistory,
                                    const OdDAIObjectId& objectPlacement,
                                    OdIfc4x3_add2::IfcElementCompositionEnum compositionType)
{
    OdIfc4x3_add2::IfcBuildingPtr entityData = CreateEntity<OdIfc4x3_add2::IfcBuilding>(model);

    entityData->setObjectPlacement(objectPlacement);
    entityData->setCompositionType(compositionType);
    entityData->setOwnerHistory(ownerHistory);

    return entityData->id();
}

inline OdDAIObjectId  createBuildingStorey(OdIfcModelPtr model,
                                          const OdDAIObjectId& ownerHistory,
                                          const OdDAIObjectId& objectPlacement,
                                          OdIfc4x3_add2::IfcElementCompositionEnum compositionType)
{
    OdIfc4x3_add2::IfcBuildingStoreyPtr entityData = CreateEntity<OdIfc4x3_add2::IfcBuildingStorey>(model);

    entityData->setObjectPlacement(objectPlacement);
    entityData->setCompositionType(compositionType);
    entityData->setOwnerHistory(ownerHistory);

    return entityData->id();
}

inline OdDAIObjectId  createBuildingElementProxy(OdIfcModelPtr model,
                                                const OdDAIObjectId& ownerHistory,
                                                const OdDAIObjectId* pLocalPlacement = NULL,
                                                const OdDAIObjectId* pProductDefinitionShape = NULL)
{
    OdIfc4x3_add2::IfcBuildingElementProxyPtr entityData = CreateEntity<OdIfc4x3_add2::IfcBuildingElementProxy>(model);

    entityData->setOwnerHistory(ownerHistory);

    if (pLocalPlacement)
    {
        entityData->setObjectPlacement(*pLocalPlacement);
    }

    if (pProductDefinitionShape)
    {
        entityData->setRepresentation(*pProductDefinitionShape);
    }

    return entityData->id();
}

inline OdDAIObjectId  createRelContainedInspatialStructure(OdIfcModelPtr model,
                                                          const OdDAIObjectId& ownerHistory,
                                                          const OdDAIObjectIds& relatedElements,
                                                          const OdDAIObjectId& relatingStructure)
{
    OdIfc4x3_add2::IfcRelContainedInSpatialStructurePtr entityData = CreateEntity<OdIfc4x3_add2::IfcRelContainedInSpatialStructure>(model);

    entityData->setOwnerHistory(ownerHistory);
    entityData->relatedElements().setArray(relatedElements);
    entityData->setRelatingStructure(relatingStructure);

    return entityData->id();
}

inline OdDAIObjectId  createAxis2Placement2D(OdIfcModelPtr model,
                                            const OdDAIObjectId& location,
                                            const OdDAIObjectId& direction)
{
    OdIfc4x3_add2::IfcAxis2Placement2DPtr entityData = CreateEntity<OdIfc4x3_add2::IfcAxis2Placement2D>(model);

    entityData->setRefDirection(direction);
    entityData->setLocation(location);

    return entityData->id();
}

inline OdDAIObjectId  createZShapeProfileDef(OdIfcModelPtr model,
                                            OdIfc4x3_add2::IfcProfileTypeEnum profileType,
                                            const OdDAIObjectId& positionPlacement,
                                            double depth,
                                            double flangeWidth,
                                            double webThickness,
                                            double flangeThickness,
                                            const double* filletRadius = NULL,
                                            const double* edgeRadius = NULL)
{
    OdIfc4x3_add2::IfcZShapeProfileDefPtr entityData = CreateEntity<OdIfc4x3_add2::IfcZShapeProfileDef>(model);

    entityData->setPosition(positionPlacement);

    entityData->setProfileType(profileType);

    entityData->setDepth(depth);
    entityData->setFlangeWidth(flangeWidth);
    entityData->setWebThickness(webThickness);
    entityData->setFlangeThickness(flangeThickness);

    if (filletRadius)
    {
        entityData->setFilletRadius(*filletRadius);
    }

    if (edgeRadius)
    {
        entityData->setEdgeRadius(*edgeRadius);
    }

    return entityData->id();
}

inline OdDAIObjectId  createIShapeProfileDef(OdIfcModelPtr model,
                                            OdIfc4x3_add2::IfcProfileTypeEnum profileType,
                                            const OdDAIObjectId& positionPlacement,
                                            double depth,
                                            double flangeWidth,
                                            double webThickness,
                                            double flangeThickness,
                                            const double* filletRadius = NULL)
{
    OdIfc4x3_add2::IfcIShapeProfileDefPtr entityData = CreateEntity<OdIfc4x3_add2::IfcIShapeProfileDef>(model);

    entityData->setPosition(positionPlacement);

    entityData->setProfileType(profileType);

    entityData->setOverallDepth(depth);
    entityData->setOverallWidth(flangeWidth);
    entityData->setWebThickness(webThickness);
    entityData->setFlangeThickness(flangeThickness);

    if (filletRadius)
    {
        entityData->setFilletRadius(*filletRadius);
    }


    return entityData->id();
}



inline OdDAIObjectId  createExtrudedAreaSolid(OdIfcModelPtr model,
                                             const OdDAIObjectId& shape,
                                             const OdDAIObjectId& placementPoint,
                                             const OdDAIObjectId& direction,
                                             double depth)
{
    OdIfc4x3_add2::IfcExtrudedAreaSolidPtr entityData = CreateEntity<OdIfc4x3_add2::IfcExtrudedAreaSolid>(model);

    entityData->setDepth(depth);
    entityData->setExtrudedDirection(direction);
    entityData->setSweptArea(shape);
    entityData->setPosition(placementPoint);

    return entityData->id();
}

inline OdDAIObjectId  createShapeRepresentation(OdIfcModelPtr model,
                                               const OdDAIObjectId& context,
                                               const char *representationIdentifier,
                                               const char *representationType,
                                               const OdArray<OdDAIObjectId>& representationItemsCollection)
{
    OdIfc4x3_add2::IfcShapeRepresentationPtr entityData = CreateEntity<OdIfc4x3_add2::IfcShapeRepresentation>(model);

    entityData->setContextOfItems(context);
    entityData->setRepresentationIdentifier(representationIdentifier);
    entityData->setRepresentationType(representationType);
    entityData->items().setArray(representationItemsCollection);

    return entityData->id();
}

inline OdDAIObjectId  createProductDefinitionShape(OdIfcModelPtr model,
                                                  const OdArray<OdDAIObjectId>& represntationCollection)
{
    OdIfc4x3_add2::IfcProductDefinitionShapePtr entityData = CreateEntity<OdIfc4x3_add2::IfcProductDefinitionShape>(model);

    entityData->representations().setArray(represntationCollection);

    return entityData->id();
}
}

namespace Utils
{
template<class TTarget, class TParam>
class ParamPostSetter
{
public:
    ParamPostSetter(OdIfcModelPtr model, const OdDAIObjectId& targetHandle)
        : m_targetHandle(targetHandle)
        , m_wasSettled(false)
        , m_model(model)
    {
    }

    virtual ~ParamPostSetter()
    {
        ODA_ASSERT(m_wasSettled);
    }

    void Set(const TParam& paramToSet)
    {
        OdSmartPtr<TTarget> entity = m_targetHandle.openObject();

        ODA_ASSERT(!entity.isNull());
        if (!entity.isNull())
        {
            SetParam(*entity, paramToSet);
            m_wasSettled = true;
        }
    }

private:
    virtual void SetParam(TTarget& target, const TParam& paramToSet) = 0;

private:
    OdIfcModelPtr m_model;
    OdDAIObjectId m_targetHandle;
    bool  m_wasSettled;
};

template <class TTarget>
class LocalPlacementPostSetter
    : public ParamPostSetter<TTarget, OdDAIObjectId>
{
public:
    LocalPlacementPostSetter(OdIfcModelPtr model, const OdDAIObjectId& targetHandle) : ParamPostSetter<TTarget, OdDAIObjectId>(model, targetHandle) {}

private:
    virtual void SetParam(TTarget& target, const OdDAIObjectId& paramToSet)
    {
        target.setObjectPlacement(paramToSet);
    }
};

template <class TTarget>
class ProductDefinitionShapeSetter
    : public ParamPostSetter<TTarget, OdDAIObjectId>
{
public:
    ProductDefinitionShapeSetter(OdIfcModelPtr model, const OdDAIObjectId& targetHandle) : ParamPostSetter<TTarget, OdDAIObjectId>(model, targetHandle) {}

private:
    virtual void SetParam(TTarget& target, const OdDAIObjectId& paramToSet)
    {
        target.setRepresentation(paramToSet);
    }
};

}
}

#endif // IFC_FILLER_EXTENDER_H
