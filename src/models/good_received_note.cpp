#include "models/good_received_note.h"

GoodReceivedNote::GoodReceivedNote(int id, const QString& globalId, const QString& approvalStatus, int nextApprovingUser,int createdByUser,
                   int quantity,int purchaseOrderLineId, QObject* parent)
    : QObject(parent), id(id), globalId(globalId), approvalStatus(approvalStatus),nextApprovingUser(nextApprovingUser),createdByUser(createdByUser),
    quantity(quantity),
    purchaseOrderLineId(purchaseOrderLineId)
{
}

