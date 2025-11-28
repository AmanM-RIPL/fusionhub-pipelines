#include "repositories/unit_of_measurement_repository.h"
#include <QDebug>

UnitOfMeasurementRepository::UnitOfMeasurementRepository(QObject* parent) : QObject(parent) {}

std::unique_ptr<UnitOfMeasurement> UnitOfMeasurementRepository::findById(int id)
{
    QSqlQuery query(dbManager->getDatabase());
    query.prepare("SELECT * FROM UnitOfMeasurement WHERE id = ?");
    query.addBindValue(id);
    
    if (query.exec() && query.next()) {
        return mapFromQuery(query);
    }
    
    return nullptr;
}

std::vector<std::unique_ptr<UnitOfMeasurement>> UnitOfMeasurementRepository::findAll()
{
    std::vector<std::unique_ptr<UnitOfMeasurement>> units;
    QSqlQuery query(dbManager->getDatabase());
    
    if (query.exec("SELECT * FROM UnitOfMeasurement")) {
        while (query.next()) {
            units.push_back(mapFromQuery(query));
        }
    }
    
    return units;
}

std::vector<UnitOfMeasurement*> UnitOfMeasurementRepository::findAllQML() {
    std::vector<UnitOfMeasurement*> uom;


    QSqlQuery query(dbManager->getDatabase());


    if (query.exec("SELECT * FROM UnitOfMeasurement")) {
        while (query.next()) {
            uom.push_back(mapFromQueryQML(query, this));
        }
    }

    //uom.push_back(mapFromQueryQML(query, this));

    return uom;
}

bool UnitOfMeasurementRepository::save(const UnitOfMeasurement& entity)
{
    QSqlQuery query(dbManager->getDatabase());
    query.prepare(getInsertQuery());
    bindEntityToQuery(query, entity);
    
    return query.exec();
}

bool UnitOfMeasurementRepository::saveQML(UnitOfMeasurement* entity) {

    QSqlQuery query(dbManager->getDatabase());
    query.prepare(getInsertQuery());
    bindEntityToQuery(query, *entity);

    return query.exec();
}

bool UnitOfMeasurementRepository::update(const UnitOfMeasurement& entity)
{
    QSqlQuery query(dbManager->getDatabase());
    query.prepare(getUpdateQuery());
    bindEntityToQuery(query, entity);
    query.addBindValue(entity.getId());
    
    return query.exec();
}

bool UnitOfMeasurementRepository::deleteById(int id)
{
    QSqlQuery query(dbManager->getDatabase());
    query.prepare("DELETE FROM UnitOfMeasurement WHERE id = ?");
    query.addBindValue(id);
    
    return query.exec();
}

std::vector<std::unique_ptr<UnitOfMeasurement>> UnitOfMeasurementRepository::findByApprovalStatus(bool status)
{
    std::vector<std::unique_ptr<UnitOfMeasurement>> units;
    QSqlQuery query(dbManager->getDatabase());
    query.prepare("SELECT * FROM UnitOfMeasurement WHERE approval_status = ?");
    query.addBindValue(status);
    
    if (query.exec()) {
        while (query.next()) {
            units.push_back(mapFromQuery(query));
        }
    }
    
    return units;
}

std::vector<std::unique_ptr<UnitOfMeasurement>> UnitOfMeasurementRepository::findByUnitType(const QString& unitType)
{
    std::vector<std::unique_ptr<UnitOfMeasurement>> units;
    QSqlQuery query(dbManager->getDatabase());
    query.prepare("SELECT * FROM UnitOfMeasurement WHERE unit_type = ?");
    query.addBindValue(unitType);
    
    if (query.exec()) {
        while (query.next()) {
            units.push_back(mapFromQuery(query));
        }
    }
    
    return units;
}

std::unique_ptr<UnitOfMeasurement> UnitOfMeasurementRepository::findByUomName(const QString& uomName)
{
    QSqlQuery query(dbManager->getDatabase());
    query.prepare("SELECT * FROM UnitOfMeasurement WHERE uom_name = ?");
    query.addBindValue(uomName);
    
    if (query.exec() && query.next()) {
        return mapFromQuery(query);
    }
    
    return nullptr;
}

QString UnitOfMeasurementRepository::getTableName() const
{
    return "UnitOfMeasurement";
}

std::unique_ptr<UnitOfMeasurement> UnitOfMeasurementRepository::mapFromQuery(const QSqlQuery& query) const
{

    auto unit = std::make_unique<UnitOfMeasurement>();
    unit->setId(query.value("id").toInt());
    unit->setGlobalId(query.value("global_id").toString());
    unit->setApprovalStatus(query.value("approval_status").toBool());
    unit->setUomName(query.value("uom_name").toString());
    unit->setUnitType(query.value("unit_type").toString());
    unit->setConversionToSqm(query.value("conversion_to_sqm").toDouble());
    unit->setConversionToCubicMeter(query.value("conversion_to_cubic_meter").toDouble());
    unit->setConversionToMeter(query.value("conversion_to_meter").toDouble());
    unit->setConversionToKilogram(query.value("conversion_to_kilogram").toDouble());


    //  auto unit = std::make_unique<UnitOfMeasurement>();
    // unit->setId(1);
    // unit->setGlobalId("123");
    // unit->setApprovalStatus(true);
    // unit->setUomName("Meter");
    // unit->setUnitType("Length");
    // unit->setConversionToSqm(0.01);
    // unit->setConversionToCubicMeter(0.5);
    // unit->setConversionToMeter(0.1);
    // unit->setConversionToKilogram(0.3);
    
    return unit;
}

UnitOfMeasurement* UnitOfMeasurementRepository::mapFromQueryQML(const QSqlQuery& query, QObject* parent) const {

    auto unit = new UnitOfMeasurement(parent);
    unit->setId(query.value("id").toInt());
    unit->setGlobalId(query.value("global_id").toString());
    unit->setApprovalStatus(query.value("approval_status").toBool());
    unit->setUomName(query.value("uom_name").toString());
    unit->setUnitType(query.value("unit_type").toString());
    unit->setConversionToSqm(query.value("conversion_to_sqm").toDouble());
    unit->setConversionToCubicMeter(query.value("conversion_to_cubic_meter").toDouble());
    unit->setConversionToMeter(query.value("conversion_to_meter").toDouble());
    unit->setConversionToKilogram(query.value("conversion_to_kilogram").toDouble());

    // auto unit = new UnitOfMeasurement(parent);
    // unit->setId(1);
    // unit->setGlobalId("123");
    // unit->setApprovalStatus(true);
    // unit->setUomName("Meter");
    // unit->setUnitType("Length");
    // unit->setConversionToSqm(0.01);
    // unit->setConversionToCubicMeter(0.5);
    // unit->setConversionToMeter(0.1);
    // unit->setConversionToKilogram(0.3);

    return unit;
}

void UnitOfMeasurementRepository::bindEntityToQuery(QSqlQuery& query, const UnitOfMeasurement& entity) const
{
    query.addBindValue(entity.getGlobalId());
    query.addBindValue(entity.getApprovalStatus());
    query.addBindValue(entity.getUomName());
    query.addBindValue(entity.getUnitType());
    query.addBindValue(entity.getConversionToSqm());
    query.addBindValue(entity.getConversionToCubicMeter());
    query.addBindValue(entity.getConversionToMeter());
    query.addBindValue(entity.getConversionToKilogram());
}

QString UnitOfMeasurementRepository::getInsertQuery() const
{
    return "INSERT INTO UnitOfMeasurement (global_id, approval_status, uom_name, unit_type, "
           "conversion_to_sqm, conversion_to_cubic_meter, conversion_to_meter, conversion_to_kilogram) "
           "VALUES (?, ?, ?, ?, ?, ?, ?, ?)";
}

QString UnitOfMeasurementRepository::getUpdateQuery() const
{
    return "UPDATE UnitOfMeasurement SET global_id = ?, approval_status = ?, uom_name = ?, "
           "unit_type = ?, conversion_to_sqm = ?, conversion_to_cubic_meter = ?, "
           "conversion_to_meter = ?, conversion_to_kilogram = ? WHERE id = ?";
}
