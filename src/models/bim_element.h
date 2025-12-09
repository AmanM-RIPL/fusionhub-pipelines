#ifndef BIM_ELEMENT_H
#define BIM_ELEMENT_H

#include <QObject>

#include "bim_parameter.h"

class BIMElement : public QObject
{
    Q_OBJECT
public:
    explicit BIMElement(QObject *parent = nullptr): QObject(parent) {}

    BIMElement(int id, const QString& globalId, bool approvalStatus, const QString& type, const QString& name, int level, int hostId, QObject* parent = nullptr);

    int getId() const { return id; }
    QString getGlobalId() const { return globalId; }
    bool getApprovalStatus() const { return approvalStatus; }
    QString getName() const { return name; }
    QString getType() const { return type; }
    int getLevel() const { return level; }
    int getHostId() const { return host_id; }
    QList<BIMParameter*> getParameterList() const { return this->parameterList; }
    QList<BIMElement*> getHostedElementList() const { return this->hostedElementList; }

    void setId(int id) { this->id = id; }
    void setGlobalId(const QString& globalId) { this->globalId = globalId; }
    void setApprovalStatus(bool status) { this->approvalStatus = status; }
    void setName(const QString& name) { this->name = name; }
    void setType(const QString& type) { this->type = type; }
    void setLevel(int level) { this->level = level; }
    void setHostId(int hostId) { this->host_id = hostId; }
    void addParameter(BIMParameter* parameter) { this->parameterList.append(parameter); }
    void addHostedElement(BIMElement* element) { this->hostedElementList.append(element); }

signals:

private:
    int id = 0;
    QString globalId;
    bool approvalStatus = true;
    QString type;
    QString name;
    int level;
    int host_id;
    QList<BIMParameter*> parameterList;
    QList<BIMElement*> hostedElementList; // elements hosted inside that BIM Element
};

Q_DECLARE_METATYPE(BIMElement)

#endif // BIM_ELEMENT_H
