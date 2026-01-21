#ifndef HELPER_POINT_H
#define HELPER_POINT_H

#include <QString>
#include <QMetaType>
#include <QObject>

struct HelperPoint
{
    Q_GADGET

    Q_PROPERTY(float x MEMBER x)
    Q_PROPERTY(float y MEMBER y)
    Q_PROPERTY(QString text MEMBER text)
    Q_PROPERTY(float value MEMBER value)

public:
    float x;
    float y;
    QString text;
    float value;
};

Q_DECLARE_METATYPE(HelperPoint)

#endif // HELPER_POINT_H
