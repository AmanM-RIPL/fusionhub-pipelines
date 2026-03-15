#include "morph.h"

Morph::Morph(QObject *parent)
    : QObject{parent}, BaseBimModel{-1, "Morph"}
{}

Morph::Morph(int id, QObject *parent)
    : QObject(parent), BaseBimModel{id, "Morph"}
{}

Morph::~Morph()
{
    // delete 2d entities
    api_del_entity_list(m_morph_bodies_2D);

    // delete 3d entities
    api_del_entity_list(m_morph_bodies_3D);
}

ENTITY_LIST &Morph::morph_bodies_3D()
{
    return m_morph_bodies_3D;
}

ENTITY_LIST &Morph::morph_bodies_2D()
{
    return m_morph_bodies_2D;
}

QString Morph::getSATFileName2D() const
{
    return SATFileName2D;
}

void Morph::setSATFileName2D(const QString &newSATFileName2D)
{
    SATFileName2D = newSATFileName2D;
}

QString Morph::getSATFileName3D() const
{
    return SATFileName3D;
}

void Morph::setSATFileName3D(const QString &newSATFileName3D)
{
    SATFileName3D = newSATFileName3D;
}

std::vector<SPAposition> Morph::morph_points_3D() const
{
    return m_morph_points_3D;
}

void Morph::setMorph_points_3D(const std::vector<SPAposition> &newMorph_points_3D)
{
    m_morph_points_3D = newMorph_points_3D;
}

std::vector<SPAposition> Morph::morph_points_2D() const
{
    return m_morph_points_2D;
}

void Morph::setMorph_points_2D(const std::vector<SPAposition> &newMorph_points_2D)
{
    m_morph_points_2D = newMorph_points_2D;
}

std::vector<int> Morph::morph_selected_bodies_3D() const
{
    return m_morph_selected_bodies_3D;
}

void Morph::setMorph_selected_bodies_3D(const std::vector<int> &newMorph_selected_bodies_3D)
{
    m_morph_selected_bodies_3D = newMorph_selected_bodies_3D;
}

std::vector<int> Morph::morph_selected_bodies_2D() const
{
    return m_morph_selected_bodies_2D;
}

void Morph::setMorph_selected_bodies_2D(const std::vector<int> &newMorph_selected_bodies_2D)
{
    m_morph_selected_bodies_2D = newMorph_selected_bodies_2D;
}
