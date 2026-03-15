#ifndef BASE_BIM_MODEL_H
#define BASE_BIM_MODEL_H

#include <QString>

class BaseBimModel
{
public:
    virtual ~BaseBimModel() = default;

    BaseBimModel(int id = -1, QString type = "");

    int id() const;
    void setId(int newId);

    QString type() const;

protected:
    int m_id = -1;
    QString m_type = "";
};

#endif // BASE_BIM_MODEL_H
