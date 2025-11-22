#include "models/file.h"

File::File(int id, const QString& globalId, bool approvalStatus,
           const QString& description,
           const QString& fileUrl,
           const QString& fileLocalPath,
           QObject* parent)
    : QObject(parent), id(id), globalId(globalId), approvalStatus(approvalStatus),
    description(description),
    fileUrl(fileUrl),
    fileLocalPath(fileLocalPath)
{
}
