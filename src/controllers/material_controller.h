#ifndef MATERIAL_CONTROLLER_H
#define MATERIAL_CONTROLLER_H

#include <QObject>
#include "repositories/material_repository.h"
#include "repositories/draft_entity_repository.h"

class MaterialController: public QObject
{
    Q_OBJECT
public:
    explicit MaterialController(QObject *parent = nullptr);

    Q_INVOKABLE void create(const QString &name, const QString &catergory, const int &UnitOfMeasurementId) const;

    Q_INVOKABLE void approvedCreate(const QString &name, const QString &catergory, const int &UnitOfMeasurementId) const;

    Q_INVOKABLE void update(int id, const QString &name, const QString &catergory, const int &UnitOfMeasurementId) const;

    Q_INVOKABLE void approve(int draftEntityId) const;

    Q_INVOKABLE void cancel(int draftEntityId, const QString &cancellationReason ) const;

    Q_INVOKABLE std::vector<Material*> getMaterialList(bool isApproved = false) const;

private:
    MaterialRepository* m_materialRepository;
    DraftEntityRepository* m_draftEntityRepository;
};

#endif // MATERIAL_CONTROLLER_H
