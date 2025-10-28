#ifndef IFC_WALL_H
#define IFC_WALL_H

#include <QObject>

class IFCWall : public QObject
{
    Q_OBJECT
public:
    explicit IFCWall(QObject *parent = nullptr);

signals:
};

#endif // IFC_WALL_H
