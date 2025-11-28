#include "models/good_received_note.h"

GoodReceivedNote::GoodReceivedNote(int id, const QString& globalId, bool approvalStatus,
                   int quantity,int purchaseOrderLineId, QObject* parent)
    : QObject(parent), id(id), globalId(globalId), approvalStatus(approvalStatus),
    quantity(quantity),
    purchaseOrderLineId(purchaseOrderLineId)
{
}

