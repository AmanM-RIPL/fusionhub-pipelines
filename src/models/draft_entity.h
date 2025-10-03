#ifndef DRAFT_ENTITY_H
#define DRAFT_ENTITY_H

#include <QString>
#include <QMetaType>
#include <QObject>
#include <QDate>

class DraftEntity : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int id READ getId CONSTANT)
    Q_PROPERTY(int tenant READ getTenant  WRITE setTenant NOTIFY TenantChanged)
    Q_PROPERTY(QDate createdOn READ getCreatedOn WRITE setCreatedOn NOTIFY CreatedOnChanged)
    Q_PROPERTY(int project READ getProject WRITE setProject NOTIFY ProjectChanged)
    Q_PROPERTY(QString entity READ getEntity WRITE setEntity NOTIFY EntityChanged)
    Q_PROPERTY(QString createdByUser READ getCreatedByUser WRITE setCreatedByUser NOTIFY CreatedByUserChanged)

    Q_PROPERTY(int nextApprovingUser READ getNextApprovingUser WRITE setNextApprovingUser NOTIFY NextApprovingUserChanged)
    Q_PROPERTY(QString entitySchema READ getEntitySchema WRITE setEntitySchema NOTIFY EntitySchemaChanged)
    Q_PROPERTY(int associatedApprovedEntity READ getAssociatedApprovedEntity WRITE setAssociatedApprovedEntity NOTIFY AssociatedApprovedEntityChanged)
    Q_PROPERTY(QString changeHistory READ getChangeHistory WRITE setChangeHistory NOTIFY ChangeHistoryChanged)

public:
    explicit DraftEntity(QObject *parent = nullptr): QObject(parent) {}
    DraftEntity(int id, int tenant, const QDate& createdOn, int project,
              const QString& entity, const QString& createdByUser, int nextApprovingUser,
              const QString& entitySchema, int associatedApprovedEntity, const QString& changeHistory,
              QObject* parent = nullptr);


    int getId() const { return id; }
    int getTenant() const { return tenant; }
    QDate getCreatedOn() const { return createdOn; }
    int getProject() const { return project; }
    QString getEntity() const { return entity; }
    QString getCreatedByUser() const { return createdByUser; }
    int getNextApprovingUser() const { return nextApprovingUser; }
    QString getEntitySchema() const { return entitySchema; }
    int getAssociatedApprovedEntity() const { return associatedApprovedEntity; }
    QString getChangeHistory() const { return changeHistory; }

    void setId(int id) { this->id = id; }
    void setTenant(int tenant) { this->tenant = tenant; }
    void setCreatedOn(const QDate& createdOn){ this->createdOn = createdOn; }
    void setProject(int project){ this->project = project; }
    void setEntity(const QString& entity){this->entity = entity; }
    void setCreatedByUser(const QString& createdByUser){ this->createdByUser = createdByUser; }
    void setNextApprovingUser(int nextApprovingUser){ this->nextApprovingUser = nextApprovingUser; }
    void setEntitySchema(const QString& entitySchema){ this->entitySchema = entitySchema; }
    void setAssociatedApprovedEntity(int associatedApprovedEntity){ this->associatedApprovedEntity = associatedApprovedEntity; }
    void setChangeHistory(const QString& changeHistory){ this->changeHistory = changeHistory; }


    /*
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    tenant INTEGER,
    createdOn DATE,
    project INTEGER NOT NULL,
    entity TEXT NOT NULL,
    createdByUser TEXT NOT NULL,
    nextApprovingUser INTEGER,
    entitySchema TEXT NOT NULL,
    associatedApprovedEntity INTEGER,
    changeHistory TEXT NOT NULL
    */

signals:
    void TenantChanged();
    void CreatedOnChanged();
    void ProjectChanged();
    void EntityChanged();
    void CreatedByUserChanged();
    void NextApprovingUserChanged();
    void EntitySchemaChanged();
    void AssociatedApprovedEntityChanged();
    void ChangeHistoryChanged();

private:
    int id;
    int tenant;
    QDate createdOn;
    int project;
    QString entity;
    QString createdByUser;
    int nextApprovingUser;
    QString entitySchema;
    int associatedApprovedEntity;
    QString changeHistory;

};

#endif // DRAFT_ENTITY_H
