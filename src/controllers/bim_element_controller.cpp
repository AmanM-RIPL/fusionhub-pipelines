#include "bim_element_controller.h"

BIMElementController::BIMElementController(QObject *parent)
    : QObject(parent),
    m_bimElementRepository(RepositoryLocator::instance().bimElementRepository()),
    m_bimParameterRepository(RepositoryLocator::instance().bimParameterRepository())
{}

BIMElement* BIMElementController::create(const QString &type, const QString &name, int level, int host_id)
{
    // VVIMP: CHANGE THIS BEFORE COMMITTING
    // 4 has been added as BIMELement Id just to check if the door works
    BIMElement* bimElement = new BIMElement(4, "1", false, type, name, level, host_id, this);
    bool result = m_bimElementRepository->saveQML(bimElement);

    if (result)
    {
        return bimElement;
    }

    return nullptr;

    //return bimElement;
}

void BIMElementController::addParameter(BIMElement* bim_element, const QString &key, const QString &value)
{
    BIMParameter* bimParameter = new BIMParameter(0, "1", false, key, value, bim_element->getId(), this);
    bool result = m_bimParameterRepository->saveQML(bimParameter);

    if (result)
    {
        bim_element->addParameter(bimParameter);
    }

    //bim_element->addParameter(bimParameter);
}

std::vector<BIMElement*> BIMElementController::getAllElements() const
{
    std::vector<BIMElement*> elementList = m_bimElementRepository->findAllQML();

    for (BIMElement* element: elementList)
    {
        std::vector<BIMParameter*> parameterList = m_bimParameterRepository->findAllForElement(element->getId());

        for (BIMParameter* parameter: parameterList)
        {
            element->addParameter(parameter);
        }
    }

    return elementList;
}


int BIMElementController::getAreaValue(const QString& costParam) const
{
    QByteArray rawData = costParam.toUtf8();
    QJsonDocument doc = QJsonDocument::fromJson(rawData);
    double totalMeterValue = 0;

    if (!doc.isNull() && doc.isObject()) {
        QJsonObject rootObj = doc.object();
        qDebug() << "Root object loaded. Row count:" << rootObj["rows"].toInt();
        QJsonArray dataArray = rootObj.value("data").toArray();

        for (const QJsonValue &value : dataArray) {
            QJsonObject obj = value.toObject();
            if (obj.value("uom").toString() == "square meter") {
                QString value = obj.value("value").toString();
                totalMeterValue += value.toDouble();
            }
        }

    } else {
        qDebug() << "Invalid JSON data!";
    }

    return totalMeterValue;
}
int BIMElementController::getVolumeValue(const QString& costParam) const
{
    QByteArray rawData = costParam.toUtf8();
    QJsonDocument doc = QJsonDocument::fromJson(rawData);
    double totalMeterValue = 0;

    if (!doc.isNull() && doc.isObject()) {
        QJsonObject rootObj = doc.object();
        qDebug() << "Root object loaded. Row count:" << rootObj["rows"].toInt();
        QJsonArray dataArray = rootObj.value("data").toArray();

        for (const QJsonValue &value : dataArray) {
            QJsonObject obj = value.toObject();
            if (obj.value("uom").toString() == "cubic meter" || obj.value("uom").toString() == "cubic mtr") {
                QString value = obj.value("value").toString();
                totalMeterValue += value.toDouble();
            }
        }

    } else {
        qDebug() << "Invalid JSON data!";
    }

    return totalMeterValue;
}
int BIMElementController::getLengthValue(const QString& costParam) const
{
    QByteArray rawData = costParam.toUtf8();
    QJsonDocument doc = QJsonDocument::fromJson(rawData);
    double totalMeterValue = 0.0;

    if (!doc.isNull() && doc.isObject()) {
        QJsonObject rootObj = doc.object();
        qDebug() << "Root object loaded. Row count:" << rootObj["rows"].toInt();
        QJsonArray dataArray = rootObj.value("data").toArray();

        for (const QJsonValue &value : dataArray) {
            QJsonObject obj = value.toObject();
            if (obj.value("uom").toString() == "meter") {
                QString value = obj.value("value").toString();
                totalMeterValue += value.toDouble();
            }
        }

    } else {
        qDebug() << "Invalid JSON data!";
    }

    return totalMeterValue;
}

int BIMElementController::getCountValue(const QString& costParam) const
{    
    QByteArray rawData = costParam.toUtf8();
    QJsonDocument doc = QJsonDocument::fromJson(rawData);
    double totalMeterValue = 0;

    if (!doc.isNull() && doc.isObject()) {
        QJsonObject rootObj = doc.object();
        qDebug() << "Root object loaded. Row count:" << rootObj["rows"].toInt();
        QJsonArray dataArray = rootObj.value("data").toArray();

        for (const QJsonValue &value : dataArray) {
            QJsonObject obj = value.toObject();
            if (obj.value("uom").toString() == "Nos") {
                QString value = obj.value("value").toString();
                totalMeterValue += value.toDouble();
            }
        }

    } else {
        qDebug() << "Invalid JSON data!";
    }

    return totalMeterValue;
}

int BIMElementController::getWeightValue(const QString& costParam) const
{
    double totalMeterValue = 0;
    QByteArray rawData = costParam.toUtf8();
    QJsonDocument doc = QJsonDocument::fromJson(rawData);

    if (!doc.isNull() && doc.isObject()) {
        QJsonObject rootObj = doc.object();
        qDebug() << "Root object loaded. Row count:" << rootObj["rows"].toInt();
        QJsonArray dataArray = rootObj.value("data").toArray();

        for (const QJsonValue &value : dataArray) {
            QJsonObject obj = value.toObject();
            if (obj.value("uom").toString() == "kg") {
                QString value = obj.value("value").toString();
                totalMeterValue += value.toDouble();
            }
        }

    } else {
        qDebug() << "Invalid JSON data!";
    }

    return totalMeterValue;
}

int BIMElementController::getTimeValue(const QString& costParam) const
{
    QByteArray rawData = costParam.toUtf8();
    QJsonDocument doc = QJsonDocument::fromJson(rawData);
    double totalMeterValue = 0;

    if (!doc.isNull() && doc.isObject()) {
        QJsonObject rootObj = doc.object();
        qDebug() << "Root object loaded. Row count:" << rootObj["rows"].toInt();
        QJsonArray dataArray = rootObj.value("data").toArray();

        for (const QJsonValue &value : dataArray) {
            QJsonObject obj = value.toObject();
            if (obj.value("uom").toString() == "hour") {
                QString value = obj.value("value").toString();
                totalMeterValue += value.toDouble();
            }
        }

    } else {
        qDebug() << "Invalid JSON data!";
    }

    return totalMeterValue;
}

int BIMElementController::getTemperatureValue(const QString& costParam) const
{
    QByteArray rawData = costParam.toUtf8();
    QJsonDocument doc = QJsonDocument::fromJson(rawData);
    double totalMeterValue = 0;

    if (!doc.isNull() && doc.isObject()) {
        QJsonObject rootObj = doc.object();
        qDebug() << "Root object loaded. Row count:" << rootObj["rows"].toInt();
        QJsonArray dataArray = rootObj.value("data").toArray();

        for (const QJsonValue &value : dataArray) {
            QJsonObject obj = value.toObject();
            if (obj.value("uom").toString() == "degree") {
                QString value = obj.value("value").toString();
                totalMeterValue += value.toDouble();
            }
        }

    } else {
        qDebug() << "Invalid JSON data!";
    }

    return totalMeterValue;
}
