#include "bim_element_controller.h"

BIMElementController::BIMElementController(QObject *parent)
    : QObject(parent),
    m_bimElementRepository(RepositoryLocator::instance().bimElementRepository()),
    m_bimParameterRepository(RepositoryLocator::instance().bimParameterRepository())
{}

void BIMElementController::create(const QString &type, const QString &name, int level)
{
    BIMElement* bimElement = new BIMElement(0, "1", false, type, name, level, this);
    m_bimElementRepository->saveQML(bimElement);
}

void BIMElementController::addParameter(int bim_element_id, const QString &key, const QString &value)
{
    BIMParameter* bimParameter = new BIMParameter(0, "1", false, key, value, bim_element_id, this);
    m_bimParameterRepository->saveQML(bimParameter);
}

BIMElement* BIMElementController::getElement() const
{
    std::vector<BIMElement*> elementList = m_bimElementRepository->findAllQML();

    if (elementList.size() > 0)
    {
        std::vector<BIMParameter*> parameterList = m_bimParameterRepository->findAllQML();

        BIMElement* firstElement = elementList[0];

        for (BIMParameter* parameter: parameterList)
        {
            firstElement->addParameter(parameter);
        }

        return firstElement;
    }

    return nullptr;
}
