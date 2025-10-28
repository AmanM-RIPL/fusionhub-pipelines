#ifndef IFCDETAIL_H
#define IFCDETAIL_H

#include <QObject>
#include <QString>
#include <QMetaType>


class IFCDetail : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int id READ getId CONSTANT)

    Q_PROPERTY(QString name READ getName WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString type READ getType WRITE setType NOTIFY typeChanged)
    Q_PROPERTY(QString globalId READ getGlobalId WRITE setGlobalId NOTIFY globalIdChanged)

public:    
    explicit IFCDetail(QObject* parent = nullptr): QObject(parent) {}

    IFCDetail(int id, const QString& globalId,
            const QString& name, const QString& type,  QObject* parent = nullptr);


    int getId() const { return this->id; }
    QString getName()const { return this->name; }
    QString getType()const { return this->type; }
    QString getGlobalId()const { return this->globalId; }

    void setId(int id) { this->id = id; }
    void setName(const QString& name) { this->name = name; }
    void setType(const QString& type) { this->type = type; }
    void setGlobalId(const QString& globalId) { this->globalId = globalId; }

signals:
    void nameChanged();
    void typeChanged();
    void globalIdChanged();

private:
    int id = 0;
    QString globalId;
    QString name;
    QString type;
};
Q_DECLARE_METATYPE(IFCDetail)
#endif // IFCDETAIL_H
