#ifndef SCHEDULE_SETUP_H
#define SCHEDULE_SETUP_H
#include <QString>
#include <QMetaType>
#include <QObject>
//#include <vector>
#include <string>
#include <QVector>
//using namespace std;

class costParamDataDetails: public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString cost_param_name READ getCostParam WRITE setCostParam NOTIFY costParamChanged)
    Q_PROPERTY(QString purchase_material READ getMaterialParam WRITE setMaterialParam NOTIFY materailParamChanged)
    Q_PROPERTY(QString type_of_bim_dimension READ getCostBimParam WRITE setCostBimParam NOTIFY costBimParamChanged)

    QString cost_param_name;
    QString purchase_material;
    QString type_of_bim_dimension;

public:
    explicit costParamDataDetails(QObject* parent = nullptr): QObject(parent) {}
    QString getCostParam() const { return cost_param_name; }
    QString getMaterialParam() const { return purchase_material; }
    QString getCostBimParam() const { return type_of_bim_dimension; }

    void setCostParam(const QString& cost_param_name) { this->cost_param_name = cost_param_name; }
    void setMaterialParam(const QString& purchase_material){ this->purchase_material = purchase_material; }
    void setCostBimParam(const QString& type_of_bim_dimension) { this->type_of_bim_dimension = type_of_bim_dimension; }

    signals:
    void costParamChanged();
    void materailParamChanged();
    void costBimParamChanged();
};

class resourceParamDataDetails: public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString resource_param_name READ getResourceParam WRITE setResourceParam NOTIFY resourceParamChanged)
    Q_PROPERTY(QString type_of_bim_dimension READ getResourceBimParam WRITE setResourceBimParam NOTIFY resourceBimParamChanged)

    QString resource_param_name;
    QString type_of_bim_dimension;

public:
    explicit resourceParamDataDetails(QObject* parent = nullptr): QObject(parent) {}
    QString getResourceParam() const { return resource_param_name; }
    QString getResourceBimParam() const { return type_of_bim_dimension; }

    void setResourceParam(const QString& resource_param_name) { this->resource_param_name = resource_param_name; }
    void setResourceBimParam(const QString& type_of_bim_dimension) { this->type_of_bim_dimension = type_of_bim_dimension; }

signals:
   void resourceParamChanged();
   void resourceBimParamChanged();
};



class ScheduleSetup: public QObject
{
    Q_OBJECT

    Q_PROPERTY(int id READ getId CONSTANT)
    Q_PROPERTY(bool approvalStatus READ getApprovalStatus WRITE setApprovalStatus NOTIFY approvalStatusChanged)
    Q_PROPERTY(QString globalId READ getGlobalId WRITE setGlobalId NOTIFY globalIdChanged)
    Q_PROPERTY(QString scheduleSetupName READ getScheduleSetupName WRITE setScheduleSetupName NOTIFY scheduleSetupNameChanged)
    Q_PROPERTY(QString description READ getDescription WRITE setDescription NOTIFY descriptionChanged)
    Q_PROPERTY(QString costParameter READ getCostParameter WRITE setCostParameter NOTIFY costParameterChanged)
    Q_PROPERTY(QString resourceParameter READ getResourceParameter WRITE setResourceParameter NOTIFY resourceParameterChanged)

    Q_PROPERTY(QVector<costParamDataDetails*> vecCostParamDataDetails READ getCostParameterDataDetails WRITE setCostParameterDataDetails NOTIFY costParameterDataDetailsChanged)
    Q_PROPERTY(QVector<resourceParamDataDetails*> vecResourceParamDataDetails READ getResourceParameterDataDetails WRITE setResourceParameterDataDetails NOTIFY resourceParameterDataDetailsChanged)


public:
    explicit ScheduleSetup(QObject* parent = nullptr): QObject(parent) {}
    int getId() const { return id; }
    QString getGlobalId() const { return globalId; }
    bool getApprovalStatus() const { return approvalStatus; }
    QString getScheduleSetupName() const { return scheduleSetupName; }
    QString getDescription() const { return description; }
    QString getCostParameter() const { return costParameter; }
    QString getResourceParameter() const { return resourceParameter; }

    QVector<costParamDataDetails*> getCostParameterDataDetails()const { return vecCostParamDataDetails; }
    QVector<resourceParamDataDetails*> getResourceParameterDataDetails()const { return vecResourceParamDataDetails; }
    
    void setId(int id) { this->id = id; }
    void setGlobalId(const QString& globalId) { this->globalId = globalId; }
    void setApprovalStatus(bool status) { this->approvalStatus = status; }
    void setScheduleSetupName(const QString& scheduleSetupName) { this->scheduleSetupName = scheduleSetupName; }
    void setDescription(const QString& description) { this->description = description; }
    void setCostParameter(const QString& costParameter) { this->costParameter = costParameter; }
    void setResourceParameter(const QString& resourceParameter) { this->resourceParameter = resourceParameter; }

    void setCostParameterDataDetails(const QVector<costParamDataDetails*>& vecCostParamDataDetails)
    {
        //this->vecCostParamDataDetails = vecCostParamDataDetails;
        qDeleteAll(this->vecCostParamDataDetails);
        this->vecCostParamDataDetails.clear();

        for (costParamDataDetails* detailPtr : vecCostParamDataDetails) {
            if (detailPtr != nullptr) {
                this->vecCostParamDataDetails.append(detailPtr);
            }
        }
    }

    void setResourceParameterDataDetails(const QVector<resourceParamDataDetails*>& vecResourceParamDataDetails)
    {
       //this->vecResourceParamDataDetails = vecResourceParamDataDetails;

        qDeleteAll(this->vecResourceParamDataDetails);
        this->vecResourceParamDataDetails.clear();

        for (resourceParamDataDetails* detailPtr : vecResourceParamDataDetails) {
            if (detailPtr != nullptr) {
                this->vecResourceParamDataDetails.append(detailPtr);
            }
        }
    }


signals:
    void globalIdChanged();
    void approvalStatusChanged();
    void scheduleSetupNameChanged();
    void descriptionChanged();
    void costParameterChanged();
    void resourceParameterChanged();

    void costParameterDataDetailsChanged();
    void resourceParameterDataDetailsChanged();

private:
    int id = 0;
    QString globalId;
    bool approvalStatus = true;
    QString scheduleSetupName;
    QString description;
    QString costParameter;
    QString resourceParameter;

    QVector<costParamDataDetails*> vecCostParamDataDetails;
    QVector<resourceParamDataDetails*> vecResourceParamDataDetails;
};

Q_DECLARE_METATYPE(ScheduleSetup)
#endif
