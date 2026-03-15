#ifndef MORPH_H
#define MORPH_H

#include <QObject>

#include "base_bim_model.h"

// ACIS header files
#include "acis.hxx"
#include "license.hxx"
#include "spa_unlock_result.hxx"
#include "spatial_license.h"
#include "kernapi.hxx"

#include <boolapi.hxx>
#include "api.hxx"
#include "lists.hxx"
#include "fileinfo.hxx"
#include <cstrapi.hxx>
#include "curdef.hxx"
#include "straight.hxx"
#include "sweepapi.hxx"
#include "swp_opts.hxx"

// for faceter
#include "af_api.hxx"
#include "fct_utl.hxx"
#include "af_serializable_mesh.hxx"

class Morph : public QObject, public BaseBimModel
{
    Q_OBJECT
public:
    explicit Morph(QObject *parent = nullptr);
    Morph(int id, QObject *parent = nullptr);
    ~Morph();

    ENTITY_LIST& morph_bodies_3D();
    ENTITY_LIST& morph_bodies_2D();

    QString getSATFileName2D() const;
    void setSATFileName2D(const QString &newSATFileName2D);

    QString getSATFileName3D() const;
    void setSATFileName3D(const QString &newSATFileName3D);

    std::vector<SPAposition> morph_points_3D() const;
    void setMorph_points_3D(const std::vector<SPAposition> &newMorph_points_3D);

    std::vector<SPAposition> morph_points_2D() const;
    void setMorph_points_2D(const std::vector<SPAposition> &newMorph_points_2D);

    std::vector<int> morph_selected_bodies_3D() const;
    void setMorph_selected_bodies_3D(const std::vector<int> &newMorph_selected_bodies_3D);

    std::vector<int> morph_selected_bodies_2D() const;
    void setMorph_selected_bodies_2D(const std::vector<int> &newMorph_selected_bodies_2D);

private:
    QString SATFileName2D;
    QString SATFileName3D;

    ENTITY_LIST m_morph_bodies_3D;
    ENTITY_LIST m_morph_bodies_2D;

    std::vector<SPAposition> m_morph_points_3D;
    std::vector<SPAposition> m_morph_points_2D;

    std::vector<int> m_morph_selected_bodies_3D;
    std::vector<int> m_morph_selected_bodies_2D;

signals:
};

#endif // MORPH_H
