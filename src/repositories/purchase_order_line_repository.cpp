#include "repositories/purchase_order_line_repository.h"

PurchaseOrderLineRepository::PurchaseOrderLineRepository(QObject* parent) : QObject(parent) {}

std::unique_ptr<PurchaseOrderLine> PurchaseOrderLineRepository::findById(int id) { return nullptr; }

std::vector<std::unique_ptr<PurchaseOrderLine>> PurchaseOrderLineRepository::findAll() { return {}; }

std::vector<PurchaseOrderLine*> PurchaseOrderLineRepository::findAllQML() {
    std::vector<PurchaseOrderLine*> list;
    QSqlQuery query(dbManager->getDatabase());

    if (query.exec("SELECT * FROM PurchaseOrderLine")) {
        while (query.next()) {
            list.push_back(mapFromQueryQML(query, this));
        }
    }

    return list;
}

bool PurchaseOrderLineRepository::save(const PurchaseOrderLine& entity) { return false; }

bool PurchaseOrderLineRepository::saveQML(PurchaseOrderLine* entity) {

    QSqlQuery query(dbManager->getDatabase());
    query.prepare(getInsertQuery());
    bindEntityToQuery(query, *entity);

    return query.exec();
}

bool PurchaseOrderLineRepository::update(const PurchaseOrderLine& entity) { return false; }

bool PurchaseOrderLineRepository::deleteById(int id) { return false; }

std::vector<std::unique_ptr<PurchaseOrderLine>> PurchaseOrderLineRepository::findByApprovalStatus(bool status) { return {}; }

QString PurchaseOrderLineRepository::getTableName() const { return "PurchaseOrderLine"; }

std::unique_ptr<PurchaseOrderLine> PurchaseOrderLineRepository::mapFromQuery(const QSqlQuery& query) const {
    return nullptr;
}

std::vector<PurchaseOrderLine*> PurchaseOrderLineRepository::findDashboardQML(int materialId)
{
    std::vector<PurchaseOrderLine*> results;

    QSqlQuery query;
    query.prepare(R"(SELECT
        COALESCE((
            SELECT SUM(pol.dollar_value)
            FROM PurchaseOrderLine pol
            WHERE pol.material_id = :materialId
            AND pol.approval_status = 1
        ), 0) AS total_po,

        COALESCE((
            SELECT SUM(grn.amount_of_material_received)
            FROM GoodReceivedNote grn
            INNER JOIN PurchaseOrderLine pol ON pol.id = grn.purchase_order_line_id
            WHERE pol.material_id = :materialId
            AND grn.approval_status = 1
        ), 0) AS total_grn,

        COALESCE((
            SELECT SUM(wol.dollar_value)
            FROM WorkOrderLine wol
            INNER JOIN MaterialIndent mi ON mi.task_id = wol.task_id
            WHERE mi.material_id = :materialId
            AND wol.approval_status = 1
        ), 0) AS total_store_amount,

        COALESCE((
            SELECT SUM(wbl.dollar_value)
            FROM WorkBillingLine wbl
            INNER JOIN WorkOrderLine wol ON wol.id = wbl.work_order_line_id
            INNER JOIN MaterialIndent mi ON mi.task_id = wol.task_id
            WHERE mi.material_id = :materialId
            AND wbl.approval_status = 1
        ), 0) AS total_purchase_expense,

        COALESCE((
            SELECT SUM(mi.total_quantity)
            FROM MaterialIndent mi
            WHERE mi.material_id = :materialId
            AND mi.approval_status = 1
        ), 0) AS total_indent_qty

       )");

    query.bindValue(":materialId", materialId);

    if (query.exec() && query.next()) {
        PurchaseOrderLine* poData = new PurchaseOrderLine();

        poData->setTotalIndent(query.value("total_indent_qty").toDouble());
        poData->setTotalPO(query.value("total_po").toDouble());
        poData->setTotalGRN(query.value("total_grn").toDouble());
        poData->setTotalStoreAmount(query.value("total_store_amount").toDouble());
        poData->setTotalPurchaseExpense(query.value("total_purchase_expense").toDouble());

        results.push_back(poData);
    } else {
        qDebug() << "No Data Found:" << query.lastError().text();
    }

    return results;
}

std::vector<PurchaseOrderLine*> PurchaseOrderLineRepository::findDashboardTableQML(int materialId)
{
    std::vector<PurchaseOrderLine*> results;

    QSqlQuery query;
    query.prepare(R"(
                    SELECT
                    v.id AS vendor_id,
                    v.vendor_name AS vendor_name,
                    SUM(pol.dollar_value) AS total_purchase_volume,
                    AVG(pol.dollar_value) AS average_price
                FROM
                    Vendor v
                JOIN
                    PurchaseOrder po ON v.id = po.vendor_id
                JOIN
                    PurchaseOrderLine pol ON po.id = pol.purchase_order_id
                WHERE
                    pol.material_id = :materialId
                GROUP BY  v.id, v.vendor_name;
               )");

    query.bindValue(":materialId", materialId);

    if (query.exec() && query.next()) {
        PurchaseOrderLine* poVData = new PurchaseOrderLine();

        poVData->setVendorId(query.value("vendor_id").toInt());
        poVData->setVendorName(query.value("vendor_name").toString());
        poVData->setTotalPurchaseExpense(query.value("total_purchase_volume").toDouble());
        poVData->setTotalStoreAmount(query.value("average_price").toDouble());

        results.push_back(poVData);
    } else {
        qDebug() << "No Data Found:" << query.lastError().text();
    }

    return results;
}

PurchaseOrderLine* PurchaseOrderLineRepository::mapFromQueryQML(const QSqlQuery& query, QObject* parent) const {
    auto po = new PurchaseOrderLine(parent);

    po->setId(query.value("id").toInt());
    po->setGlobalId(query.value("global_id").toString());
    po->setApprovalStatus(query.value("approval_status").toBool());
    po->setPurchaseOrderId(query.value("purchase_order_id").toInt());
    po->setMaterialId(query.value("material_id").toInt());
    po->setUnitOfMeasurementId(query.value("unit_of_measurement_id").toInt());
    po->setQuantity(query.value("quantity").toDouble());
    po->setAmount(query.value("dollar_value").toDouble());


    return po;
}

void PurchaseOrderLineRepository::bindEntityToQuery(QSqlQuery& query, const PurchaseOrderLine& entity) const {
    query.addBindValue(entity.getGlobalId());
    query.addBindValue(entity.getApprovalStatus());
    query.addBindValue(entity.getPurchaseOrderId());
    query.addBindValue(entity.getMaterialId());
    query.addBindValue(entity.getUnitOfMeasurementId());

}

QString PurchaseOrderLineRepository::getInsertQuery() const {
    return "INSERT INTO PurchaseOrderLine (global_id, approval_status, vendor_id) "
           "VALUES (?, ?, ?)";
}

QString PurchaseOrderLineRepository::getUpdateQuery() const {
    return "UPDATE PurchaseOrderLine SET global_id = ?, approval_status = ?, vendor_id = ?, "
           " WHERE id = ?";
}

