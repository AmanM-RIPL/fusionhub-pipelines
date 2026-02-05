#ifndef MATERIAL_INDENT_CONTROLLER_H
#define MATERIAL_INDENT_CONTROLLER_H

#include <QObject>
#include "repositories/material_indent_repository.h"
#include "repositories/material_repository.h"
#include "repositories/task_repository.h"
#include "repositories/draft_entity_repository.h"

class MaterialIndentController: public QObject
{
    Q_OBJECT
public:
    explicit MaterialIndentController(QObject *parent = nullptr);
    Q_INVOKABLE void create(const double &quantity, const int &materialId, const int &taskId) const;
    Q_INVOKABLE void approvedCreate(const double &quantity, const int &materialId, const int &taskId) const;

    Q_INVOKABLE void update(int id, const double &quantity, const int &materialId, const int &taskId) const;

    Q_INVOKABLE std::vector<MaterialIndent*> getMaterialIndentList(bool isApproved ) const;

private:
    MaterialIndentRepository* m_materialIndentRepository;
    DraftEntityRepository* m_draftEntityRepository;
};

#endif // MATERIAL_INDENT_CONTROLLER_H
