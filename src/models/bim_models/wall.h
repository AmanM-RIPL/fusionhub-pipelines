#ifndef WALL_H
#define WALL_H

#include <QObject>

class IFCWall : public QObject
{
    Q_OBJECT
public:
    explicit IFCWall(QObject *parent = nullptr);

signals:
};

#endif // WALL_H
