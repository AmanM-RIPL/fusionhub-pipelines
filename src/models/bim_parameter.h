#ifndef BIM_PARAMETER_H
#define BIM_PARAMETER_H

#include <QObject>

class BIMParameter : public QObject
{
    Q_OBJECT
public:
    explicit BIMParameter(QObject *parent = nullptr);

    BIMParameter(int id, const QString& globalId, bool approvalStatus, const QString& key, const QString& value, int bim_element_id, QObject* parent = nullptr);

    int getId() const { return id; }
    QString getGlobalId() const { return globalId; }
    bool getApprovalStatus() const { return approvalStatus; }
    QString getKey() const { return key; }
    QString getValue() const { return value; }
    int getBimElementId() const { return bim_element_id; }

    void setId(int id) { this->id = id; }
    void setGlobalId(const QString& globalId) { this->globalId = globalId; }
    void setApprovalStatus(bool status) { this->approvalStatus = status; }
    void setKey(const QString& key) { this->key = key; }
    void setValue(const QString& value) { this->value = value; }
    void setBimElementId(int bim_element_id) { this->bim_element_id = bim_element_id; }

signals:

private:
    int id = 0;
    QString globalId;
    bool approvalStatus = true;
    int bim_element_id;
    QString key;
    QString value;
};

#endif // BIM_PARAMETER_H
