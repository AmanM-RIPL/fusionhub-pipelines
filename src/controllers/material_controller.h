#ifndef MATERIAL_CONTROLLER_H
#define MATERIAL_CONTROLLER_H

#include <QObject>
#include "repositories/material_repository.h"
#include "repositories/unit_of_measurement_repository.h"
#include "repositories/draft_entity_repository.h"

class MaterialController: public QObject
{
    Q_OBJECT
public:
    //MaterialController();
    explicit MaterialController(QObject *parent = nullptr);
    Q_INVOKABLE void create(const QString &name, const QString &catergory, const int &UnitOfMeasurementId) const;
    Q_INVOKABLE std::vector<Material*> getMaterialList(bool isApproved = false) const;

    Q_INVOKABLE std::vector<UnitOfMeasurement*> getUOMList() const;

private:
    MaterialRepository* m_materialRepository;
    UnitOfMeasurementRepository* m_unitOfMeasurementRepository;
    DraftEntityRepository* m_draftEntityRepository;
};

#endif // MATERIAL_CONTROLLER_H
