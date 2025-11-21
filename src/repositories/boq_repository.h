#ifndef BOQ_REPOSITORY_H
#define BOQ_REPOSITORY_H

#include <QObject>

class boq_repository : public QObject
{
    Q_OBJECT
public:
    explicit boq_repository(QObject *parent = nullptr);

signals:
};

#endif // BOQ_REPOSITORY_H
