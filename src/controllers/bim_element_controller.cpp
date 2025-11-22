#include "bim_element_controller.h"

BIMElementController::BIMElementController(QObject *parent)
    : QObject(parent),
    m_bimElementRepository(RepositoryLocator::instance().bimElementRepository()),
    m_bimParameterRepository(RepositoryLocator::instance().bimParameterRepository())
{}

BIMElement* BIMElementController::create(const QString &type, const QString &name, int level)
{
    BIMElement* bimElement = new BIMElement(0, "1", false, type, name, level, this);
    // bool result = m_bimElementRepository->saveQML(bimElement);

    // if (result)
    // {
    //     return bimElement;
    // }

    // return nullptr;

    return bimElement;
}

void BIMElementController::addParameter(BIMElement* bim_element, const QString &key, const QString &value)
{
    BIMParameter* bimParameter = new BIMParameter(0, "1", false, key, value, bim_element->getId(), this);
    // bool result = m_bimParameterRepository->saveQML(bimParameter);

    // if (result)
    // {
    //     bim_element->addParameter(bimParameter);
    // }

    bim_element->addParameter(bimParameter);
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
