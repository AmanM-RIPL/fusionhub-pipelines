#ifndef IFCDETAIL_H
#define IFCDETAIL_H

#include <QObject>
#include <QString>

class IFCDetail : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString name READ getName WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString type READ getType WRITE setType NOTIFY typeChanged)
    Q_PROPERTY(QString globalId READ getGlobalId WRITE setGlobalId NOTIFY globalIdChanged)

public:
    explicit IFCDetail(QObject *parent = nullptr);

    QString getName() { return m_name; }
    QString getType() { return m_type; }
    QString getGlobalId() { return m_globalId; }

    void setName(const QString& name) { m_name = name; }
    void setType(const QString& type) { m_type = type; }
    void setGlobalId(const QString& globalId) { m_globalId = globalId; }

signals:
    void nameChanged();
    void typeChanged();
    void globalIdChanged();

private:
    QString m_name;
    QString m_type;
    QString m_globalId;
};

#endif // IFCDETAIL_H
