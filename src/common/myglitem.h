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

struct MouseCoordinate
{
    double pressedX; //only different from currentX if the mouse is clicked and moved
    double pressedY;
    double currentX;
    double currentY;
};

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
    static void setDatabaseId(OdTvDatabaseId dbId);
    static void setModelId(OdTvModelId modelId);

    OdTvDatabaseId dbId();
    OdTvModelId modelId();

    static OdTvDatabaseId m_dbId;
    static OdTvModelId m_modelId;

    Renderer* createRenderer() const override;

    double getZoom();
    MouseCoordinate getMouseCoordinates();
    bool getMousePressed();

public slots:
    void zoomIn();
    void zoomOut();
    void mousePressed(double x, double y);
    void mouseReleased();
    void mousePositionChanged(double x, double y);

private:
    double m_zoom = 1;
    bool m_mouseIsPressed = false;
    double m_pressedMouseX = 0;
    double m_pressedMouseY = 0;
    double m_mouseX = 0; // these are for real-time value
    double m_mouseY = 0;
signals:
};

class MyGLRenderer : public QQuickFramebufferObject::Renderer {
public:
    MyGLRenderer(OdTvDatabaseId databaseId, OdTvModelId mId);
    ~MyGLRenderer();

    void synchronize(QQuickFramebufferObject *item) override;
    void render() override;
    void update();
    QOpenGLFramebufferObject* createFramebufferObject(const QSize &size) override;

private:
    OdGsDevicePtr pDevice;
    OdTvGsDeviceId deviceId;
    OdTvDatabaseId dbId;
    OdTvModelId modelId;
    OdGsModelPtr pGsModule;
    QSize m_lastSize;
    bool m_initialized;
    GLuint m_fboId = 0;
    QQuickFramebufferObject *m_item;
    double m_zoom = 1;
    double m_xOrbit = 0;
    double m_yOrbit = 0;
    bool m_orbit = false;
};


class TreeItem {
public:
    struct Property {
        QString key;
        QVariant value; // can hold string, int, list, etc.
    };

    TreeItem(const QString &type, const QString &globalId, const QString &name, TreeItem *parentItem = nullptr)
        : m_type(type), m_globalId(globalId), m_name(name), m_parentItem(parentItem) {}

    ~TreeItem() { qDeleteAll(m_childItems); }

    void appendChild(TreeItem *child) {
        m_childItems.append(child);
    }

    TreeItem *child(int row) { return m_childItems.value(row); }
    int childCount() const { return m_childItems.count(); }

    int row() const {
        if (m_parentItem)
            return m_parentItem->m_childItems.indexOf(const_cast<TreeItem*>(this));
        return 0;
    }

    TreeItem *parentItem() { return m_parentItem; }

    QString type() const { return m_type; }
    QString globalId() const { return m_globalId; }
    QString name() const { return m_name; }

    void addProperty(const QString &key, const QVariant &value) {
        m_properties.insert(key, value);
    }

    const QMap<QString, QVariant> &properties() const { return m_properties; }

private:
    QList<TreeItem*> m_childItems;
    TreeItem *m_parentItem;
    QString m_type;
    QString m_globalId;
    QString m_name;
    QMap<QString, QVariant> m_properties;
};

class TreeModel : public QAbstractItemModel {
    Q_OBJECT
public:
    TreeModel(TreeItem* root, QObject *parent = nullptr)
        : QAbstractItemModel(parent) {
        rootItem = root;
    }

    explicit TreeModel(QObject *parent = nullptr)
        : QAbstractItemModel(parent) {
        rootItem = new TreeItem("", "", "");
    }

    // ~TreeModel() override { delete rootItem; }

    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override {
        TreeItem *parentItem = getItem(parent);
        TreeItem *childItem = parentItem->child(row);
        if (childItem)
            return createIndex(row, column, childItem);
        return QModelIndex();
    }

    QModelIndex parent(const QModelIndex &index) const override {
        if (!index.isValid())
            return QModelIndex();

        TreeItem *childItem = getItem(index);
        TreeItem *parentItem = childItem->parentItem();

        if (parentItem == rootItem)
            return QModelIndex();

        return createIndex(parentItem->row(), 0, parentItem);
    }

    int rowCount(const QModelIndex &parent = QModelIndex()) const override {
        TreeItem *parentItem = getItem(parent);
        return parentItem->childCount();
    }

    int columnCount(const QModelIndex & = QModelIndex()) const override {
        // For example: Type, Name, GlobalId
        return 3;
    }

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override {
        if (!index.isValid())
            return QVariant();

        TreeItem *item = getItem(index);

        if (role == Qt::DisplayRole) {
            switch (index.column()) {
            case 0: return item->type();
            case 1: return item->name();
            case 2: return item->globalId();
            }
        }
        return QVariant();
    }

    void addNode(TreeItem *parent, TreeItem *child) {
        beginInsertRows(index(parent->row(), 0, QModelIndex()), parent->childCount(), parent->childCount());
        parent->appendChild(child);
        endInsertRows();
    }

    TreeItem *root() const { return rootItem; }

private:
    TreeItem *rootItem;

    TreeItem *getItem(const QModelIndex &index) const {
        if (index.isValid()) {
            TreeItem *item = static_cast<TreeItem*>(index.internalPointer());
            if (item)
                return item;
        }
        return rootItem;
    }
};



class AbstractNotifier
{
public:

    virtual ~AbstractNotifier() {};

    virtual void onLevelBegin(const OdRxObject *inst) = 0;
    virtual void onLevelEnd(const OdRxObject *inst) = 0;
    virtual void onHierarchyAttributeFound(const OdRxObject *inst, OdRxMember *member) = 0;
    virtual void onHasRepresentation(const OdRxObject *inst, const OdRxObject *reprInst) = 0;
    virtual void onAttribute(const OdRxMember *member, const OdRxValue &rxVal, const OdRxObject *inst, TreeItem* item) = 0;
};

class LevelSaver
{
    int& m_level;
public:

    LevelSaver(int& level)
        : m_level(level)
    {
        ++m_level;
    }

    ~LevelSaver()
    {
        --m_level;
    }
};

class CDATreePrinter : public AbstractNotifier
{
    int m_level;

public:

    CDATreePrinter()
        : m_level(0)
    {}

    void onLevelBegin(const OdRxObject *inst) override
    {
        ++m_level;

        //odPrintConsoleString(OD_T("%sHierarchy level: %d\n"), tabulation(m_level).c_str(), m_level);

        const OdString &className = inst->isA()->name();
        //odPrintConsoleString(OD_T("%sClass: %s\n"), tabulation(m_level).c_str(), className.c_str());

        OdString entityName;

        OdRxPropertyPtr propName = OdRxMemberQueryEngine::theEngine()->find(inst, OD_T("Name"));
        if (!propName.isNull())
        {
            OdRxValue valName;
            (eOk == propName->getValue(inst, valName)) &&
                (valName >> entityName);
        }

        //odPrintConsoleString(OD_T("%sName: `%s`\n"), tabulation(m_level).c_str(), entityName.c_str());
    }

    void onLevelEnd(const OdRxObject *inst) override
    {
        --m_level;
    }

    void onHierarchyAttributeFound(const OdRxObject *inst, OdRxMember *member) override
    {
        const OdString &propName = member->name();
        //odPrintConsoleString(OD_T("%sHierarchy property name: %s\n"), tabulation(m_level).c_str(), propName.c_str());
    }

    void onHasRepresentation(const OdRxObject *inst, const OdRxObject *reprInst) override
    {
        const OdString &reprClassName = reprInst->isA()->name();
        //odPrintConsoleString(OD_T("%sCan be vectorized here (Representation: %s)\n"), tabulation(m_level).c_str(), reprClassName.c_str());
        // vectorize(inst); // not reprInst, inst should be casted appropriately to OdGiDrawable/OdIfcInstance
    }

    void onAttribute(const OdRxMember* member, const OdRxValue& rxVal, const OdRxObject *inst, TreeItem* treeItem) override
    {
        OdString memberName = member->name();
        QString qtMemberName = QString::fromUtf16(reinterpret_cast<const ushort*>(memberName.c_str()));
        //odPrintConsoleString(OD_T("\n%s%s: "), tabulation(m_level).c_str(), memberName.c_str());

        const OdRxValueType &vt = rxVal.type();
        if (vt == OdRxValueType::Desc<OdDAIObjectId>::value())
        {
            OdDAIObjectId idVal = *rxvalue_cast<OdDAIObjectId>(&rxVal);
            //odPrintConsoleString(OD_T("#%llu"), (OdUInt64)idVal.getHandle());
            treeItem->addProperty(qtMemberName, (OdUInt64)idVal.getHandle());
        }
        else
            if (vt == OdRxValueType::Desc<OdAnsiString>::value())
            {
                OdAnsiString strVal = *rxvalue_cast<OdAnsiString>(&rxVal);
                //odPrintConsoleString(OD_T("'%hs'"), strVal.c_str());
                treeItem->addProperty(qtMemberName, strVal.c_str());
            }
            else
                if (vt == OdRxValueType::Desc<OdString>::value())
                {
                    OdString strVal = *rxvalue_cast<OdString>(&rxVal);
                    QString qtStrVal = QString::fromUtf16(reinterpret_cast<const ushort*>(strVal.c_str()));
                    //odPrintConsoleString(OD_T("'%s'"), strVal.c_str());
                    treeItem->addProperty(qtMemberName, qtStrVal);
                }
                else
                    if (vt == OdRxValueType::Desc<double>::value())
                    {
                        double dblVal = *rxvalue_cast<double>(&rxVal);
                        //odPrintConsoleString(OD_T("%.4f"), dblVal);
                        treeItem->addProperty(qtMemberName, dblVal);
                    }
        if (vt.isEnum())
        {
            OdAnsiString strVal;
            rxVal >> strVal;
            //odPrintConsoleString(OD_T("'%hs'"), strVal.c_str());
            treeItem->addProperty(qtMemberName, strVal.c_str());
        }
        if (vt.isSelect())
        {
            OdString typeValue = rxVal.typePath();
            typeValue.makeUpper();

            OdTCKind kind;
            rxVal >> kind;
            OdAnsiString sVal;
            switch (kind)
            {
            case tkDouble:
            {
                double dVal;
                if (rxVal >> dVal)
                    sVal.format("(%f)", dVal);
                break;
            }
            default:
                ;
            }
        }
        // ...

        // Children attributes processing
        const OdArray<OdRxMemberPtr> *children = member->children();
        if (member->children() && children->size())
        {
            //odPrintConsoleString(OD_T(", child attributes:"));
            LevelSaver saveLevel(m_level);

            if (rxVal.type() == OdRxValueType::Desc<OdDAIObjectId>::value())
            {
                OdDAIObjectId idVal = *rxvalue_cast<OdDAIObjectId>(&rxVal);
                OdDAI::ApplicationInstancePtr instParentSet = idVal.openObject();
                ODA_ASSERT_ONCE(instParentSet);

                for (const OdRxMemberPtr& childAttribute : *children)
                {
                    if (childAttribute->isKindOf(OdRxProperty::desc()))
                    {
                        const OdRxPropertyPtr rxProperty = OdRxProperty::cast(childAttribute);

                        OdRxValue childValue;
                        if (eOk == rxProperty->getValue(instParentSet, childValue))
                        {
                            onAttribute(childAttribute, childValue, instParentSet, treeItem);
                        }
                        else
                        {
                            //odPrintConsoleString(OD_T("\n%s%s: Can not get value."), tabulation(m_level).c_str(), childAttribute->name().c_str());
                        }
                    }
                    else
                        if (childAttribute->isKindOf(OdRxCollectionProperty::desc()))
                        {
                            //
                            // Process collection
                            //
                            const OdRxCollectionProperty* collection = dynamic_cast<const OdRxCollectionProperty*>(childAttribute.get());

                            // Haven't seen before
                        }
                }
            }
            else
            {
                for (const OdRxMemberPtr& childAttribute : *children)
                {
                    if (childAttribute->isKindOf(OdRxProperty::desc()))
                    {
                        OdRxProperty* childProp = OdRxProperty::cast(childAttribute);

                        OdRxValue childRxVal;
                        if (eOk == childProp->getValue(inst, childRxVal))
                        {
                            onAttribute(childProp, childRxVal, inst, treeItem);
                        }
                    }
                    else
                    {
                        // Haven't seen before
                    }
                }
            }
        }
    }

protected:

    OdString tabulation(int tab)
    {
        if (tab < 0)
            tab = 0;
        OdString res;
        while (tab > 0)
        {
            res += OD_T("  ");
            --tab;
        }
        return res;
    }

};

class CDAWalker
{
    AbstractNotifier *m_pNotifier;

public:

    CDAWalker(AbstractNotifier *notifier)
        : m_pNotifier(notifier)
    {}

    ~CDAWalker()
    {
        delete m_pNotifier;
    }

    void run(const OdRxObject *inst, TreeItem* parentItem)
    {
        return walkCDA(inst, parentItem);
    }

protected:

    void walkAttributes(const OdRxObject *inst, TreeItem* treeItem)
    {
        OdRxMemberIteratorPtr memberIterator = OdRxMemberQueryEngine::theEngine()->newMemberIterator(inst);
        if (memberIterator)
        {
            OdUInt64 memberCount = memberIterator->size();
            for (; !memberIterator->done(); memberIterator->next())
            {
                OdRxMember *member = memberIterator->current();

                if (member->isKindOf(OdRxProperty::desc()))
                {
                    //
                    // Process single property
                    //
                    OdRxProperty *prop = OdRxProperty::cast(member); // dynamic_cast<OdRxProperty*>(member);

                    OdRxValue rxVal;
                    if (eOk == prop->getValue(inst, rxVal))
                    {
                        m_pNotifier->onAttribute(member, rxVal, inst, treeItem);
                    }
                }
                else
                    if (member->isKindOf(OdRxCollectionProperty::desc()))
                    {
                        //
                        // Process collection
                        //
                        OdRxCollectionProperty *collection = dynamic_cast<OdRxCollectionProperty*>(member);
                        OdRxValueIteratorPtr itValues = collection->newValueIterator(inst);

                        if (!itValues.isNull())
                        {
                            for (; !itValues->done(); itValues->next())
                            {
                                OdRxValue rxVal = itValues->current();
                                m_pNotifier->onAttribute(member, rxVal, inst, treeItem);
                                // Typed values extraction
                            }
                        }
                    }
            }
        }
    }

    void walkCDA(const OdRxObject *inst, TreeItem* parentItem)
    {
        if (m_pNotifier)
            m_pNotifier->onLevelBegin(inst);

        OdIfc::OdIfcInstancePtr pEntity = OdIfc::OdIfcInstance::cast(inst);

        OdString globalId = "";
        if (!pEntity.isNull())
        {
            OdRxValue result = pEntity->getAttrCaseInsensitive("GlobalId");
            result >> globalId;
            //qInfo() << "GlobalId: " << nameValue;
        }

        OdString nameValue = "";
        if (!pEntity.isNull())
        {
            OdRxValue result = pEntity->getAttrCaseInsensitive("Name");
            result >> nameValue;
            //qInfo() << "GlobalId: " << nameValue;
        }

        //
        // Uncomment to get OdRx class name.
        //
        OdString rxClassName = inst->isA()->name();
        QString qtClassName = QString::fromUtf16(reinterpret_cast<const ushort*>(rxClassName.c_str()));
        QString qtGlobalId = QString::fromUtf16(reinterpret_cast<const ushort*>(globalId.c_str()));
        QString qtNameValue = QString::fromUtf16(reinterpret_cast<const ushort*>(nameValue.c_str()));

        TreeItem* childItem = new TreeItem(qtClassName, qtGlobalId, qtNameValue, parentItem);
        parentItem->appendChild(childItem);


        // Uncomment this line for dump attributes/properties of an instance

        walkAttributes(inst, childItem);

        //
        // Try to find attribute called Representation, if it isn't null, the instance can be vectorized.
        //
        // OdRxMember *memRepr = OdRxMemberQueryEngine::theEngine()->find(inst, OD_T("Representation"));
        // if (memRepr)
        // {
        //     if (memRepr->isKindOf(OdRxProperty::desc()))
        //     {
        //         OdRxProperty *propRepr = OdRxProperty::cast(memRepr);

        //         OdRxValue idRepresentation;
        //         if (eOk == propRepr->getValue(inst, idRepresentation))
        //         {
        //             const IOdRxReferenceType *reference = idRepresentation.type().reference();
        //             if (reference)
        //             {
        //                 OdRxObjectPtr reprInst = reference->dereference(idRepresentation, IOdRxReferenceType::kForRead);
        //                 if (!reprInst.isNull())
        //                 {
        //                     if (m_pNotifier)
        //                         m_pNotifier->onHasRepresentation(inst, reprInst);
        //                 }
        //             }
        //         }
        //     }
        // }

        //
        // Walk along all Rx Properties of instance and try to find Hierarchy Attribute
        //
        OdRxMemberIteratorPtr it = OdRxMemberQueryEngine::theEngine()->newMemberIterator(inst);
        for (; !it->done(); it->next())
        {
            OdRxMember *member = it->current();

            OdRxAttributeCollection &attrs = member->attributes();

            int numAttrs = attrs.count();
            for (int i = 0; i < numAttrs; ++i)
            {
                OdRxAttribute *attr = attrs.getAt(i);

                OdRxClass* pRx = attr->isA();
                if (pRx->isDerivedFrom(OdRxHierarchyLevelAttribute::desc()))
                {
                    if (m_pNotifier)
                        m_pNotifier->onHierarchyAttributeFound(inst, member);

                    if (member->isKindOf(OdRxProperty::desc()))
                    {
                        OdRxProperty *prop = dynamic_cast<OdRxProperty*>(member);

                        OdRxValue idHierarchy;
                        if (eOk == prop->getValue(inst, idHierarchy))
                        {
                            const IOdRxReferenceType *reference = idHierarchy.type().reference();
                            if (reference)
                            {
                                OdGiDrawablePtr pInst = reference->dereference(idHierarchy, IOdRxReferenceType::kForRead);
                                walkCDA(pInst, childItem);
                            }
                            else
                            {
                                for (const auto& it : *prop->children())
                                {
                                    //
                                }
                            }
                        }
                    }
                    else
                        if (member->isKindOf(OdRxCollectionProperty::desc()))
                        {
                            OdRxCollectionProperty *collection = dynamic_cast<OdRxCollectionProperty*>(member);
                            OdRxValueIteratorPtr itValues = collection->newValueIterator(inst);

                            if (!itValues.isNull())
                                for (; !itValues->done(); itValues->next())
                                {
                                    OdRxValue treeNodeId = itValues->current();
                                    const IOdRxReferenceType *reference = treeNodeId.type().reference();
                                    if (reference)
                                    {
                                        OdGiDrawablePtr pInst = reference->dereference(treeNodeId, IOdRxReferenceType::kForRead);
                                        walkCDA(pInst, childItem);
                                    }
                                }
                        }
                }
            }
        }

        if (m_pNotifier)
            m_pNotifier->onLevelEnd(inst);
    }

};



#endif // MYGLITEM_H
