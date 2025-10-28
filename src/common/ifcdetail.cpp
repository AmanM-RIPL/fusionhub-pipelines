#include "ifcdetail.h"

IFCDetail::IFCDetail(int id, const QString& globalId, const QString& name, const QString& type, QObject* parent)
    : id(id), globalId(globalId), name(name), type(type), QObject(parent)
{

}


