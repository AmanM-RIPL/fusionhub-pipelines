#ifndef IFC_DATABASE_MANAGER_H
#define IFC_DATABASE_MANAGER_H

#include <QObject>

class IFCDatabaseManager : public QObject
{
    Q_OBJECT
public:
    explicit IFCDatabaseManager(QObject *parent = nullptr);

signals:
};

#endif // IFC_DATABASE_MANAGER_H
