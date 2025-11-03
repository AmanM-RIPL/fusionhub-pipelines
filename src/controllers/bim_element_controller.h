#ifndef BIM_ELEMENT_CONTROLLER_H
#define BIM_ELEMENT_CONTROLLER_H

#include <QObject>

#include "repositories/bim_element_repository.h"
#include "repositories/bim_parameter_repository.h"
#include "common/repository_locator.h"

class BIMElementController : public QObject
{
    Q_OBJECT
public:
    explicit BIMElementController(QObject *parent = nullptr);

   Q_INVOKABLE BIMElement* create(const QString &type, const QString &name, int level);
   Q_INVOKABLE void addParameter(BIMElement* bim_element, const QString &key, const QString &value);
   Q_INVOKABLE std::vector<BIMElement*> getAllElements() const;

signals:

private:
    BIMElementRepository* m_bimElementRepository;
    BIMParameterRepository* m_bimParameterRepository;
};

#endif // BIM_ELEMENT_CONTROLLER_H
