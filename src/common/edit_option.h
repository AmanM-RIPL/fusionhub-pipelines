#ifndef EDIT_OPTION_H
#define EDIT_OPTION_H

#include <QObject>
#include <QString>
#include <QMetaType>

class EditOption : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString viewType READ viewType WRITE setViewType NOTIFY viewTypeChanged)
    Q_PROPERTY(QString editType READ editType WRITE setEditType NOTIFY editTypeChanged)
    Q_PROPERTY(bool curveTypeVisible READ curveTypeVisible WRITE setCurveTypeVisible NOTIFY curveTypeVisibleChanged)
    Q_PROPERTY(bool solidTypeVisible READ solidTypeVisible WRITE setSolidTypeVisible NOTIFY solidTypeVisibleChanged)
    Q_PROPERTY(bool modifyTypeVisible READ modifyTypeVisible WRITE setModifyTypeVisible NOTIFY modifyTypeVisibleChanged)

public:
    explicit EditOption(QObject *parent = nullptr);

    static EditOption* instance();

    QString viewType() { return m_viewType; }
    QString editType() { return m_editType; }
    bool curveTypeVisible() { return m_curveTypeVisible; }
    bool solidTypeVisible() { return m_solidTypeVisible; }
    bool modifyTypeVisible() { return m_modifyTypeVisible; }

    void setViewType(QString value);
    void setEditType(QString value);
    void setCurveTypeVisible(bool value);
    void setSolidTypeVisible(bool value);
    void setModifyTypeVisible(bool value);

signals:
    void viewTypeChanged();
    void editTypeChanged();
    void curveTypeVisibleChanged();
    void solidTypeVisibleChanged();
    void modifyTypeVisibleChanged();

private:
    static EditOption* m_instance;

    QString m_viewType;
    QString m_editType;
    bool m_curveTypeVisible;
    bool m_solidTypeVisible;
    bool m_modifyTypeVisible;
};

Q_DECLARE_METATYPE(EditOption)

#endif // EDIT_OPTION_H
