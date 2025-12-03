#ifndef GOOD_RECEIVED_NOTE_CONTROLLER_H
#define GOOD_RECEIVED_NOTE_CONTROLLER_H

#include <QObject>
#include "repositories/good_received_note_repository.h"
#include "repositories/purchase_order_line_repository.h"
#include "repositories/purchase_order_repository.h"
#include "repositories/draft_entity_repository.h"

class GoodReceivedNoteController: public QObject
{
    Q_OBJECT
public:
    explicit GoodReceivedNoteController(QObject *parent = nullptr);
    Q_INVOKABLE void create(const int &quantity, const int &purchaseOrderLineId) const;
    Q_INVOKABLE std::vector<GoodReceivedNote*> getGoodReceivedNoteList(bool isApproved = false) const;

    //Q_INVOKABLE std::vector<PurchaseOrderLine*> getPurchaseOrderLineList() const;
    //Q_INVOKABLE std::vector<PurchaseOrder*> getPurchaseOrderList() const;
private:
    GoodReceivedNoteRepository* m_goodReceivedNoteRepository;
    //PurchaseOrderLineRepository* m_purchaseOrderLineRepository;
    //PurchaseOrderRepository* m_purchaseOrderRepository;
    DraftEntityRepository* m_draftEntityRepository;
};

#endif // GOOD_RECEIVED_NOTE_CONTROLLER_H
