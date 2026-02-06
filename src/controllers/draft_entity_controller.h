#ifndef DRAFT_ENTITY_CONTROLLER_H
#define DRAFT_ENTITY_CONTROLLER_H

#include <QObject>
#include "repositories/draft_entity_repository.h"

class DraftEntityController: public QObject
{
    Q_OBJECT
public:
    explicit DraftEntityController(QObject *parent = nullptr);

    Q_INVOKABLE void approve(int draftEntityId) const;

    Q_INVOKABLE void cancel(int draftEntityId, const QString &cancellationReason ) const;

private:
    DraftEntityRepository* m_draftEntityRepository;
};

#endif // DRAFT_ENTITY_CONTROLLER_H
