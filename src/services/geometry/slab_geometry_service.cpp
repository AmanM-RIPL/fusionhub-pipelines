#include "slab_geometry_service.h"

SlabGeometryService::SlabGeometryService(QObject *parent)
    : QObject{parent}
{}

void SlabGeometryService::generateMesh2D(BIMElement* slabElement, Mesh* mesh)
{
    ENTITY_LIST ents;

    std::vector<ReferenceLineSegment> referenceLine = {};
    std::vector<Layer> layers = {};
    float width = 0;
    float height = 0;
    float distance = 0;
    float slantAngle = 0;
    float taperAngle = 0;
    QString referenceLinePosition = "inner";

    m_openglHelper.extractBIMParameters(slabElement, referenceLine, layers, width, height, distance, slantAngle, taperAngle, referenceLinePosition);

    if (referenceLine.size() < 2)
    {
        mesh->Initialize({}, {}, {}, {}, {}, {}, {}, {}, {});
        return;
    }

    BODY* final_body = nullptr;
    ents.add(final_body);
    generateSlab2D(final_body, ents, referenceLine);

    // Mesh geometry generation
    std::vector<uint32_t> meshIndices = {};
    std::vector<Position> vertices_position = {};
    std::vector<Normal> vertices_normal = {};
    std::vector<TextureUV> vertices_textureuv = {};
    std::vector<int> vertices_materialIndex = {};
    std::vector<int> vertices_textureIndex = {};
    std::vector<int> edge_indices = {};
    std::vector<EdgeDataInt> edge_data_int = {};
    std::vector<EdgeDataFloat> edge_data_float = {};
    int textureIndex = Texture::NONE; // if less than zero then we don't need to worry about textures
    int materialIndex = OpenGLMaterial::IVORY;
    float edgeWidth = 1.0f;
    float edgeDashLength = 1.0f;
    float edgeGapLength = 1.0f;
    int edgeDash = 0;
    int edgeMaterialIndex = OpenGLMaterial::BLACK;
    int scalingFactor = 5;


    if (final_body != nullptr)
    {
        m_openglHelper.getMeshGeometry(
            final_body,
            vertices_position,
            vertices_normal,
            vertices_textureuv,
            vertices_materialIndex,
            vertices_textureIndex,
            meshIndices,
            edge_indices,
            edge_data_int,
            edge_data_float,
            textureIndex,
            materialIndex,
            scalingFactor,
            edgeWidth,
            edgeDashLength,
            edgeGapLength,
            edgeDash,
            edgeMaterialIndex
        );
    }

    mesh->Initialize(
        vertices_position,
        vertices_normal,
        vertices_textureuv,
        vertices_materialIndex,
        vertices_textureIndex,
        edge_data_int,
        edge_data_float,
        meshIndices,
        edge_indices
        );
    mesh->setBIMElementId(slabElement->getId());

    // delete entity list
    api_del_entity_list(ents);
}

void SlabGeometryService::generateMesh3D(BIMElement* slabElement, Mesh* mesh)
{
    ENTITY_LIST ents;

    std::vector<ReferenceLineSegment> referenceLine = {};
    std::vector<Layer> layers = {};
    float width = 0;
    float height = 0;
    float distance = 0;
    float slantAngle = 0;
    float taperAngle = 0;
    QString referenceLinePosition = "inner";

    m_openglHelper.extractBIMParameters(slabElement, referenceLine, layers, width, height, distance, slantAngle, taperAngle, referenceLinePosition);

    if (referenceLine.size() < 2)
    {
        mesh->Initialize({}, {}, {}, {}, {}, {}, {}, {}, {});
        return;
    }

    BODY* final_body = nullptr;
    ents.add(final_body);
    generateSlab3D(final_body, ents, referenceLine, height, distance);

    // Mesh geometry generation
    std::vector<uint32_t> meshIndices = {};
    std::vector<Position> vertices_position = {};
    std::vector<Normal> vertices_normal = {};
    std::vector<TextureUV> vertices_textureuv = {};
    std::vector<int> vertices_materialIndex = {};
    std::vector<int> vertices_textureIndex = {};
    std::vector<int> edge_indices = {};
    std::vector<EdgeDataInt> edge_data_int = {};
    std::vector<EdgeDataFloat> edge_data_float = {};
    int textureIndex = Texture::NONE; // if less than zero then we don't need to worry about textures
    int materialIndex = OpenGLMaterial::IVORY;
    float edgeWidth = 1.0f;
    float edgeDashLength = 1.0f;
    float edgeGapLength = 1.0f;
    int edgeDash = 0;
    int edgeMaterialIndex = OpenGLMaterial::BLACK;
    int scalingFactor = 5;


    if (final_body != nullptr)
    {
        m_openglHelper.getMeshGeometry(
            final_body,
            vertices_position,
            vertices_normal,
            vertices_textureuv,
            vertices_materialIndex,
            vertices_textureIndex,
            meshIndices,
            edge_indices,
            edge_data_int,
            edge_data_float,
            textureIndex,
            materialIndex,
            scalingFactor,
            edgeWidth,
            edgeDashLength,
            edgeGapLength,
            edgeDash,
            edgeMaterialIndex
        );
    }

    mesh->Initialize(
        vertices_position,
        vertices_normal,
        vertices_textureuv,
        vertices_materialIndex,
        vertices_textureIndex,
        edge_data_int,
        edge_data_float,
        meshIndices,
        edge_indices
    );
    mesh->setBIMElementId(slabElement->getId());

    // delete entity list
    api_del_entity_list(ents);
}

void SlabGeometryService::updateGeometry(BIMElement *slabElement, const QVector3D &point, const QString &curveType)
{
    std::vector<ReferenceLineSegment> referenceLine = {};
    std::vector<Layer> layers = {};
    float width = 0;
    float height = 0;
    float distance = 0;
    float slantAngle = 0;
    float taperAngle = 0;
    QString referenceLinePosition = "inner";

    m_openglHelper.extractBIMParameters(slabElement, referenceLine, layers, width, height, distance, slantAngle, taperAngle, referenceLinePosition);

    // update the new point in the reference line
    m_openglHelper.addPointToReferenceLine(referenceLine, point, curveType);

    // updating the BIMElement
    QJsonArray referenceLineJsonArray;

    for (const ReferenceLineSegment& rlsArray : referenceLine)
    {
        QJsonObject rlsObject;

        rlsObject.insert("type", rlsArray.type);

        QJsonArray pointsArray;

        for (const Point &point: rlsArray.points)
        {
            QJsonArray jsonInnerArray;
            for (float value : point) {
                jsonInnerArray.append(QJsonValue(value));
            }

            pointsArray.append(jsonInnerArray);
        }

        rlsObject.insert("points", pointsArray);

        referenceLineJsonArray.append(rlsObject);
    }

    QJsonDocument jsonDoc(referenceLineJsonArray);
    QByteArray byteArray = jsonDoc.toJson(QJsonDocument::Compact);
    QString referenceLineString = QString(byteArray);

    QList<BIMParameter*> parameterList = slabElement->getParameterList();

    for (BIMParameter* parameter: parameterList)
    {
        if (parameter->getKey() == "ReferenceLine")
        {
            parameter->setValue(referenceLineString);

            break;
        }
    }
}

void SlabGeometryService::generateWIPMesh2D(BIMElement *slabElement, Mesh *mesh, const QVector3D &point, const Point& screen_point, View *view, QList<HelperPoint> &helperPoints, const QString &curveType)
{
    ENTITY_LIST ents;

    std::vector<ReferenceLineSegment> referenceLine = {};
    std::vector<Layer> layers = {};
    float width = 0;
    float height = 0;
    float distance = 0;
    float slantAngle = 0;
    float taperAngle = 0;
    QString referenceLinePosition = "inner";

    m_openglHelper.extractBIMParameters(slabElement, referenceLine, layers, width, height, distance, slantAngle, taperAngle, referenceLinePosition);

    // update the new point in the reference line
    m_openglHelper.addPointToReferenceLine(referenceLine, point, curveType);

    // if reference line is only one point then we don't need to render
    if (referenceLine.size() < 1)
    {
        mesh->Initialize({}, {}, {}, {}, {}, {}, {}, {}, {});
        return;
    }

    BODY* final_body = nullptr;
    ents.add(final_body);
    generateSlab2D(final_body, ents, referenceLine);

    // Mesh geometry generation
    std::vector<uint32_t> meshIndices = {};
    std::vector<Position> vertices_position = {};
    std::vector<Normal> vertices_normal = {};
    std::vector<TextureUV> vertices_textureuv = {};
    std::vector<int> vertices_materialIndex = {};
    std::vector<int> vertices_textureIndex = {};
    std::vector<int> edge_indices = {};
    std::vector<EdgeDataInt> edge_data_int = {};
    std::vector<EdgeDataFloat> edge_data_float = {};
    int textureIndex = Texture::NONE; // if less than zero then we don't need to worry about textures
    int materialIndex = OpenGLMaterial::IVORY;
    float edgeWidth = 1.0f;
    float edgeDashLength = 1.0f;
    float edgeGapLength = 1.0f;
    int edgeDash = 0;
    int edgeMaterialIndex = OpenGLMaterial::BLACK;
    int scalingFactor = 5;


    if (final_body != nullptr)
    {
        m_openglHelper.getMeshGeometry(
            final_body,
            vertices_position,
            vertices_normal,
            vertices_textureuv,
            vertices_materialIndex,
            vertices_textureIndex,
            meshIndices,
            edge_indices,
            edge_data_int,
            edge_data_float,
            textureIndex,
            materialIndex,
            scalingFactor,
            edgeWidth,
            edgeDashLength,
            edgeGapLength,
            edgeDash,
            edgeMaterialIndex
        );

        ReferenceLineSegment& last_rls = referenceLine.back();

        if (last_rls.type == "line")
        {
            m_openglHelper.addHelperPointsForLine(
                referenceLine,
                ents,
                point,
                screen_point,
                view,
                helperPoints,
                vertices_position,
                vertices_normal,
                vertices_textureuv,
                vertices_materialIndex,
                vertices_textureIndex,
                meshIndices,
                edge_indices,
                edge_data_int,
                edge_data_float
            );
        }
        else if (last_rls.type == "3pt-circle")
        {
            m_openglHelper.addHelperPointsFor3PtCircle(
                referenceLine,
                ents,
                point,
                screen_point,
                view,
                helperPoints,
                vertices_position,
                vertices_normal,
                vertices_textureuv,
                vertices_materialIndex,
                vertices_textureIndex,
                meshIndices,
                edge_indices,
                edge_data_int,
                edge_data_float
            );
        }
        else if (last_rls.type == "bezier")
        {
            m_openglHelper.addHelperPointsForBezier(
                referenceLine,
                ents,
                point,
                screen_point,
                view,
                helperPoints,
                vertices_position,
                vertices_normal,
                vertices_textureuv,
                vertices_materialIndex,
                vertices_textureIndex,
                meshIndices,
                edge_indices,
                edge_data_int,
                edge_data_float
            );
        }
    }

    // initialize the mesh
    mesh->Initialize(
        vertices_position,
        vertices_normal,
        vertices_textureuv,
        vertices_materialIndex,
        vertices_textureIndex,
        edge_data_int,
        edge_data_float,
        meshIndices,
        edge_indices
        );
    mesh->setBIMElementId(slabElement->getId());

    // delete entity list
    api_del_entity_list(ents);
}

Point SlabGeometryService::updatePoint2D(BIMElement *slabElement, const QList<HelperPoint> &helperPoints, const Point &screen_point, View *view)
{
    ENTITY_LIST ents;

    std::vector<ReferenceLineSegment> referenceLine = {};
    std::vector<Layer> layers = {};
    float width = 0;
    float height = 0;
    float distance = 0;
    float slantAngle = 0;
    float taperAngle = 0;
    QString referenceLinePosition = "inner";

    m_openglHelper.extractBIMParameters(slabElement, referenceLine, layers, width, height, distance, slantAngle, taperAngle, referenceLinePosition);

    Point newPointScreenSpace = { 0.0f, 0.0f };
    ReferenceLineSegment& last_rls = referenceLine.back();

    if (last_rls.type == "line")
    {
        newPointScreenSpace = m_openglHelper.updatePointForLine(referenceLine, ents, helperPoints, screen_point, view);
    }
    else if (last_rls.type == "3pt-circle")
    {
        newPointScreenSpace = m_openglHelper.updatePointFor3PtCircle(referenceLine, ents, helperPoints, screen_point, view);
    }
    else if (last_rls.type == "bezier")
    {
        newPointScreenSpace = m_openglHelper.updatePointForBezier(referenceLine, ents, helperPoints, screen_point, view);
    }

    // delete entity list
    api_del_entity_list(ents);

    return newPointScreenSpace;
}

void SlabGeometryService::generateHelperPoints(BIMElement *bimElement, QList<HelperPoint> &helperPoints)
{
    HelperPoint length;
    HelperPoint angle;

    length.x = 0.0f;
    length.y = 0.0f;
    length.text = "Length";
    length.value = 0.0f;
    length.visible = false;

    angle.x = 0.0f;
    angle.y = 0.0f;
    angle.text = "Angle";
    angle.value = 0.0f;
    angle.visible = false;

    helperPoints.clear();

    helperPoints.append(length);
    helperPoints.append(angle);
}

void SlabGeometryService::generateSlab2D(BODY *&final_body, ENTITY_LIST &ents, std::vector<ReferenceLineSegment> &referenceLine)
{
    // 1. Convert referenceLine to ACIS open wire-body
    BODY* wire_body = nullptr;
    ents.add(wire_body);
    std::vector<EDGE*> edges = {};
    m_openglHelper.getReferenceLineWireBody(wire_body, ents, referenceLine, edges);

    if (wire_body == nullptr) return;

    // 2. Find the first and last of the open wire and if not the same
    //    the create an edge to connect it

    ENTITY_LIST coedge_list;
    api_wire_to_chain(wire_body, coedge_list);

    COEDGE* first_coedge = static_cast<COEDGE*>(coedge_list[0]);
    SPAposition first_point = first_coedge->start_pos();

    COEDGE* last_coedge = static_cast<COEDGE*>(coedge_list[coedge_list.iteration_count() - 1]);
    SPAposition last_point = last_coedge->end_pos();

    if (first_point != last_point)
    {
        EDGE* end_edge = nullptr;
        ents.add(end_edge);
        api_curve_line(last_point, first_point, end_edge);

        std::vector<EDGE*> edges = { end_edge };
        BODY* end_edge_wire_body = nullptr;
        ents.add(end_edge_wire_body);
        api_make_ewire(1, edges.data(), end_edge_wire_body);

        api_unite(end_edge_wire_body, wire_body);
    }

    // 3. Convert the wire-body to a sheet body with one face
    ENTITY_LIST wire_body_list;
    wire_body_list.add(wire_body);
    api_cover_wire_loops(wire_body_list, final_body);
}

void SlabGeometryService::generateSlab3D(BODY *&final_body, ENTITY_LIST &ents, std::vector<ReferenceLineSegment> &referenceLine, float height, float distance)
{

    qInfo() << "Distance: " << distance;

    // 1. Generate 2D planar profile
    generateSlab2D(final_body, ents, referenceLine);

    if (final_body == nullptr) return;

    // 2. Sweep in z-direction as per height
    EXCEPTION_BEGIN
        sweep_options* sw_options = ACIS_NEW sweep_options();
        BODY* new_body = nullptr;
        ents.add(new_body);
    EXCEPTION_TRY

        outcome sw_result = api_sweep_with_options(final_body, SPAvector(0.0, 0.0, 1 * height), sw_options, new_body);

        if (!sw_result.ok())
        {
            error_info* info = sw_result.get_error_info();
            qInfo() << info->error_message();
        }

    EXCEPTION_CATCH_TRUE
        ACIS_DELETE sw_options;
    EXCEPTION_END

    // 3. Translate the body by +z distance
    SPAtransf translation_matrix = translate_transf(SPAvector(0.0, 0.0, 1 * distance));

    SPAvector trans_vector = translation_matrix.translation();
    qInfo() << "Translation: " << trans_vector.x() << ", " << trans_vector.y() << ", " << trans_vector.z();

    api_transform_entity(final_body, translation_matrix);
}
