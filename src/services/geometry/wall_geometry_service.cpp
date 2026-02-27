#include "wall_geometry_service.h"

WallGeometryService::WallGeometryService(QObject *parent)
    : QObject{parent}
{}


void WallGeometryService::generateMesh2D(BIMElement* wallElement, Mesh* mesh)
{
    ENTITY_LIST ents;

    std::vector<std::vector<Point>> polygon;
    std::vector<ReferenceLineSegment> referenceLine = {};
    std::vector<Layer> layers = {};
    float width = 0;
    float height = 0;
    float distance = 0;

    m_openglHelper.extractBIMParameters(wallElement, referenceLine, layers, width, height, distance);

    qInfo() << "layer length: " << layers.size();

    // if reference line is only one point then we don't need to render
    if (referenceLine.size() < 2)
    {
        mesh->Initialize({}, {}, {}, {}, {}, {}, {}, {}, {});
        return;
    }

    // 1. Convert referenceLine to ACIS open wire-body
    BODY* wire_body = nullptr;
    std::vector<EDGE*> edges = {};
    for (ReferenceLineSegment& line_seg: referenceLine)
    {
        EDGE* edge = nullptr;
        ents.add(edge);

        if (line_seg.type == "line")
        {
            api_curve_line(
                SPAposition(line_seg.points[0][0], line_seg.points[0][1], 0.0),
                SPAposition(line_seg.points[1][0], line_seg.points[1][1], 0.0),
                edge
            );
        }
        else if (line_seg.type == "3pt-circle")
        {
            api_curve_arc_3pt(
                SPAposition(line_seg.points[0][0], line_seg.points[0][1], 0.0),
                SPAposition(line_seg.points[1][0], line_seg.points[1][1], 0.0),
                SPAposition(line_seg.points[2][0], line_seg.points[2][1], 0.0),
                false,
                edge
            );
        }
        else if (line_seg.type == "bezier")
        {
            api_curve_bezier(
                SPAposition(line_seg.points[0][0], line_seg.points[0][1], 0.0),
                SPAposition(line_seg.points[1][0], line_seg.points[1][1], 0.0),
                SPAposition(line_seg.points[2][0], line_seg.points[2][1], 0.0),
                SPAposition(line_seg.points[3][0], line_seg.points[3][1], 0.0),
                edge
            );
        }

        edges.push_back(edge);
    }

    api_make_ewire(edges.size(), edges.data(), wire_body);

    // try offsetting
    BODY* offset_wire = nullptr;
    ents.add(offset_wire);
    SPAunit_vector wire_normal(0.0, 0.0, -1.0);

    wire_offset_options* offset_options = ACIS_NEW wire_offset_options();
    offset_options->set_distance(1.0);
    offset_options->set_plane_normal(wire_normal);
    offset_options->set_gap_type(sg_gap_type::natural);
    api_offset_planar_wire(wire_body, offset_options, offset_wire);

    ACIS_DELETE offset_options;

    // add two wire edges
    BODY* edge_body1 = nullptr;
    ents.add(edge_body1);
    std::vector<SPAposition> array_pts = {
        SPAposition(0.0,0.0,0.0),
        SPAposition(-1.0, 0.0, 0.0),
        // SPAposition(4.0,5.0,0.0),
        // SPAposition(4.0, 4.0, 0.0),
    };
    std::vector<double> array_bulges = {0.0};
    api_make_kwire(offset_wire, wire_normal, 2, array_pts.data(), array_bulges.data(), edge_body1);

    api_unite(edge_body1, offset_wire);

    BODY* edge_body2 = nullptr;
    ents.add(edge_body2);
    std::vector<SPAposition> array_pts2 = {
        SPAposition(4.0,5.0,0.0),
        SPAposition(4.0, 4.0, 0.0),
    };
    std::vector<double> array_bulges2 = {0.0};
    api_make_kwire(offset_wire, wire_normal, 2, array_pts2.data(), array_bulges2.data(), edge_body2);

    api_unite(edge_body2, offset_wire);

    if (offset_wire != nullptr)
    {
        ENTITY_LIST coedge_list;
        api_wire_to_chain(offset_wire, coedge_list);

        for (int i = 0; i < coedge_list.iteration_count(); i++)
        {
            COEDGE* coedge = static_cast<COEDGE*>(coedge_list[i]);
            EDGE* edge = coedge->edge();
            SPAposition start_pos = edge->start_pos();
            SPAposition end_pos = edge->end_pos();

            qInfo() << "Edge: (" << start_pos.x() << ", " << start_pos.y() << ", " << start_pos.z() << ") - (" << end_pos.x() << ", " << end_pos.y() << ", " << end_pos.z() << ")";
        }
    }

    // 2. sweep the open-wire body to generate solid face
    if (edges.size() == 0)
    {
        mesh->Initialize({}, {}, {}, {}, {}, {}, {}, {}, {});
        return;
    }

    EDGE* first_edge = edges[0];
    SPAposition first_edge_point = first_edge->start_pos();

    SPAvector tangent_vector = first_edge->start_deriv();
    SPAvector perp_vector(-1 * tangent_vector.y(), tangent_vector.x(), tangent_vector.z());
    perp_vector = (width / perp_vector.len()) * perp_vector;

    SPAposition profile_point(
        perp_vector.x() + first_edge_point.x(),
        perp_vector.y() + first_edge_point.y(),
        perp_vector.z()
    );

    EDGE* edge_for_sweep = nullptr;
    ents.add(edge_for_sweep);
    api_curve_line(first_edge_point, profile_point, edge_for_sweep);

    // do sweep
    BODY* new_body = nullptr;
    ents.add(new_body);

    EXCEPTION_BEGIN
        sweep_options* sw_options = ACIS_NEW sweep_options();
    EXCEPTION_TRY
        outcome sw_result = api_sweep_with_options(edge_for_sweep, wire_body, sw_options, new_body);

    if (!sw_result.ok())
    {
        error_info* info = sw_result.get_error_info();
        qInfo() << info->error_message();
    }

    EXCEPTION_CATCH_TRUE
        ACIS_DELETE sw_options;
    EXCEPTION_END

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

    m_openglHelper.getMeshGeometry(
        new_body,
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

    // // 3. Generate a parallel line
    // std::vector<Point> parallelLine = m_openglHelper.generateParallelCurve(referenceLine, width);

    // referenceLine.insert(referenceLine.end(), parallelLine.begin(), parallelLine.end());

    // // for (Point point: referenceLine)
    // // {
    // //     qInfo() << "x: " << point[0] << " , y: " << point[1];
    // // }

    // // 4. Get triangulated mesh
    // polygon.push_back(referenceLine);
    // polygon.push_back({}); // for holes
    // std::vector<uint32_t> indices = mapbox::earcut<uint32_t>(polygon);

    // // for (uint32_t index: indices)
    // // {
    // //     qInfo() << index;
    // // }

    // // 5. Create and export mesh
    // std::vector<Position> vertices_position = {};
    // std::vector<Normal> vertices_normal = {};
    // std::vector<TextureUV> vertices_textureuv = {};
    // std::vector<int> vertices_materialIndex = {};
    // std::vector<int> vertices_textureIndex = {};
    // for (int i = 0; i < referenceLine.size(); i++)
    // {
    //     Point point = referenceLine[i];

    //     // Vertex v = {
    //     //     {point[0], point[1], 0.0f},
    //     //     {0.0f, 0.0f, 1.0f},
    //     //     {0.0f, 0.0f},
    //     //     OpenGLMaterial::IVORY,
    //     //     Texture::NONE
    //     // };

    //     vertices_position.push_back({point[0], point[1], 0.0f, 0.0f});
    //     vertices_normal.push_back({0.0f, 0.0f, 1.0f});
    //     vertices_textureuv.push_back({0.0f, 0.0f});
    //     vertices_materialIndex.push_back(OpenGLMaterial::IVORY);
    //     vertices_textureIndex.push_back(Texture::NONE);

    //     // verticesVector.push_back(point[0]); // x
    //     // verticesVector.push_back(point[1]); // y
    //     // verticesVector.push_back(0.0f); // z
    //     // verticesVector.push_back(0.0f); // n.x
    //     // verticesVector.push_back(0.0f); // n.y
    //     // verticesVector.push_back(1.0f); // n.z
    // }

    // std::vector<int> edge_indices = {};
    // std::vector<EdgeDataInt> edge_data_int = {};
    // std::vector<EdgeDataFloat> edge_data_float = {};
    // for (int i = 0; i < referenceLine.size(); i++)
    // {
    //     EdgeDataInt ei;
    //     EdgeDataFloat ef;

    //     ei.material_index = OpenGLMaterial::BLACK;
    //     ei.dash = 0;
    //     ef.width = 1.0f;
    //     ef.dash_length = 1.0f;
    //     ef.gap_length = 1.0f;
    //     ef.padding = 0.0f;

    //     if (i == referenceLine.size() - 1)
    //     {
    //         ei.start_vertex = i;
    //         ei.end_vertex = 0;
    //     }
    //     else
    //     {
    //         ei.start_vertex = i;
    //         ei.end_vertex = i + 1;
    //     }

    //     edge_data_int.push_back(ei);
    //     edge_data_float.push_back(ef);
    //     edge_indices.push_back(i);
    // }

    // // // Allocate memory for the new array using std::unique_ptr for safety.
    // // auto indices_raw = std::make_unique<unsigned int[]>(indices.size());

    // // // Copy elements from the vector to the new array.
    // // std::copy(indices.begin(), indices.end(), indices_raw.get());

    // Mesh* mesh = new Mesh(this);
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
    mesh->setBIMElementId(wallElement->getId());


    // // GLfloat* vertices1 = mesh->getVerticies();
    // // unsigned int* indices1 = mesh->getIndices();
    // // for (int i = 0; i < 6; i++)
    // // {
    // //     qInfo() << vertices1[6*i] << " , " << vertices1[6*i + 1] << " , " << vertices1[6*i + 2] << " , " << vertices1[6*i + 3] << " , " << vertices1[6*i + 4] << " , " << vertices1[6*i + 5];
    // // }

    // // qInfo() << "----------------------------------";

    // // for (int i = 0; i < 6; i++)
    // // {
    // //     qInfo() << indices1[i];
    // // }

    // // return mesh;

    // delete entity list
    api_del_entity_list(ents);
}

void WallGeometryService::generateMesh3D(BIMElement* wallElement, Mesh* mesh)
{
    // ACIS entity list to delete all entities at the end of the function
    ENTITY_LIST ents;

    std::vector<Point> referenceLine = {};
    float width = 0;
    float height = 0;
    float distance = 0;


    m_openglHelper.extractBIMParameters(wallElement, referenceLine, width, height, distance);

    // if reference line is only one point then we don't need to render
    if (referenceLine.size() < 2)
    {
        mesh->Initialize({}, {}, {}, {}, {}, {}, {}, {}, {});
        return;
    }

    // 3. Generate a parallel line
    std::vector<Point> parallelLine = m_openglHelper.generateParallelCurve(referenceLine, width);

    referenceLine.insert(referenceLine.end(), parallelLine.begin(), parallelLine.end());

    // Create a contour2D
    FacetModeler::Contour2D polygon;

    OdGePoint2dArray points;
    points.reserve(referenceLine.size());

    for (Point point: referenceLine)
    {
        points.push_back(OdGePoint2d(point[0], point[1]));
    }

    polygon.appendVertices(points);

    for (int i = 0; i < referenceLine.size(); i++)
    {
        polygon.setOrientationAt(i, FacetModeler::efoFront);
    }

    polygon.setClosed();
    polygon.makeCCW();

    FacetModeler::Profile2D profile(polygon);
    FacetModeler::Body body = FacetModeler::Body::extrusion(profile, OdGeVector3d(0.0, 0.0, 1.0) * height);

    // ACIS BODY

    // 1. Create wire body of the reference line
    BODY* wire_body = nullptr;
    ents.add(wire_body);

    std::vector<EDGE*> wire_edges = {};
    for (int i = 0; i < referenceLine.size(); i++)
    {
        SPAposition first_point;
        SPAposition second_point;

        first_point = SPAposition(referenceLine[i][0], referenceLine[i][1], 0);

        if (i == referenceLine.size() - 1)
        {
            second_point = SPAposition(referenceLine[0][0], referenceLine[0][1], 0);
        }
        else
        {
            second_point = SPAposition(referenceLine[i + 1][0], referenceLine[i + 1][1], 0);
        }

        EDGE* edge = nullptr;
        ents.add(edge);

        api_curve_line(first_point, second_point, edge);
        wire_edges.push_back(edge);
    }

    api_make_ewire(wire_edges.size(), wire_edges.data(), wire_body);

    // 2. Sweep along z-axis vector
    BODY* new_body = nullptr;
    ents.add(new_body);

    EXCEPTION_BEGIN
        sweep_options* sw_options = ACIS_NEW sweep_options();
    EXCEPTION_TRY
        outcome sw_result = api_sweep_with_options(wire_body, SPAvector(0,0,1 * height), sw_options, new_body);

        if (!sw_result.ok())
        {
            error_info* info = sw_result.get_error_info();
            qInfo() << info->error_message();
        }

    EXCEPTION_CATCH_TRUE
        ACIS_DELETE sw_options;
    EXCEPTION_END


    QList<BIMElement*> hostedElementList = wallElement->getHostedElementList();
    for (BIMElement* hostedElement: hostedElementList)
    {
        if (hostedElement->getType() == "Door")
        {
            DoorGeometryService service = DoorGeometryService();
            FacetModeler::Body voidBody = service.generateVoidBody(hostedElement, wallElement);

            body = FacetModeler::Body::boolOper(FacetModeler::eDifference, body, voidBody);
        }
        else if (hostedElement->getType() == "Window")
        {
            WindowGeometryService service = WindowGeometryService();
            FacetModeler::Body voidBody = service.generateVoidBody(hostedElement, wallElement);

            body = FacetModeler::Body::boolOper(FacetModeler::eDifference, body, voidBody);
        }
    }

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
    int textureIndex = Texture::BRICK; // if less than zero then we don't need to worry about textures
    int materialIndex = OpenGLMaterial::IVORY;
    float edgeWidth = 1.0f;
    float edgeDashLength = 1.0f;
    float edgeGapLength = 1.0f;
    int edgeDash = 0;
    int edgeMaterialIndex = OpenGLMaterial::BLACK;
    int scalingFactor = 5;

    m_openglHelper.getMeshGeometry(
        wire_body,
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
    mesh->setBIMElementId(wallElement->getId());

    // delete entity list
    api_del_entity_list(ents);
}

void WallGeometryService::updateGeometry(BIMElement *wallElement, const QVector3D &point)
{
    std::vector<Point> referenceLine = {};
    float width = 0;
    float height = 0;
    float distance = 0;


    m_openglHelper.extractBIMParameters(wallElement, referenceLine, width, height, distance);

    // update the new point in the reference line
    referenceLine.push_back({ point.x(), point.y() });

    // updating the BIMElement
    QJsonArray referenceLineJsonArray;

    for (const auto& pointArray : referenceLine) {
        QJsonArray jsonInnerArray;
        for (float value : pointArray) {
            jsonInnerArray.append(QJsonValue(value));
        }
        referenceLineJsonArray.append(jsonInnerArray);
    }

    QJsonDocument jsonDoc(referenceLineJsonArray);
    QByteArray byteArray = jsonDoc.toJson(QJsonDocument::Compact);
    QString referenceLineString = QString(byteArray);

    QList<BIMParameter*> parameterList = wallElement->getParameterList();

    for (BIMParameter* parameter: parameterList)
    {
        if (parameter->getKey() == "ReferenceLine")
        {
            parameter->setValue(referenceLineString);

            break;
        }
    }
}

void WallGeometryService::generateWIPMesh2D(BIMElement *wallElement, Mesh *mesh, const QVector3D &point, const Point& screen_point, View *view, QList<HelperPoint> &helperPoints)
{
    std::vector<std::vector<Point>> polygon;
    std::vector<Point> referenceLine = {};
    float width = 0;
    float height = 0;
    float distance = 0;

    m_openglHelper.extractBIMParameters(wallElement, referenceLine, width, height, distance);

    int referenceLineSize = referenceLine.size(); // we need this value for the angle drawing, check at the end

    // if reference line is only one point then we don't need to render
    if (referenceLine.size() < 1)
    {
        mesh->Initialize({}, {}, {}, {}, {}, {}, {}, {}, {});
        return;
    }

    // show length helper point
    helperPoints[0].visible = true;

    // last point of referenceLine
    Point lastPointReferenceLine = referenceLine.back();
    QVector3D lastPoint(lastPointReferenceLine[0], lastPointReferenceLine[1], 0.0f);
    Point lastPointScreenSpace = view->GetPointInScreenSpace(lastPoint);
    Point middlePointScreenSpace = m_openglHelper.getMiddlePoint(lastPointScreenSpace, screen_point);

    Point new_point1 = m_openglHelper.getPointAtPerpendicularDistance(lastPointScreenSpace, screen_point, 4);
    Point new_point2 = m_openglHelper.getPointAtPerpendicularDistance(lastPointScreenSpace, screen_point, 10);
    Point new_point3 = m_openglHelper.getPointAtPerpendicularDistance(screen_point, lastPointScreenSpace, -10);
    Point new_point4 = m_openglHelper.getPointAtPerpendicularDistance(screen_point, lastPointScreenSpace, -4);
    Point middle_point = m_openglHelper.getPointAtPerpendicularDistance(middlePointScreenSpace, screen_point, 10);

    helperPoints[0].x = middle_point[0];
    helperPoints[0].y = middle_point[1] + 30; // 30px added to offset

    QVector3D wcs_point1 = view->GetPointInViewSpace(new_point1[0], new_point1[1]);
    QVector3D wcs_point2 = view->GetPointInViewSpace(new_point2[0], new_point2[1]);
    QVector3D wcs_point3 = view->GetPointInViewSpace(new_point3[0], new_point3[1]);
    QVector3D wcs_point4 = view->GetPointInViewSpace(new_point4[0], new_point4[1]);

    helperPoints[0].value = m_openglHelper.getDistanceBetweenPoints(lastPointReferenceLine, {point[0], point[1]});

    // qInfo() << "Last Point: " << lastPointScreenSpace[0] << ", " << lastPointScreenSpace[1];
    // qInfo() << "Middle Point: " << middlePointScreenSpace[0] << ", " << middlePointScreenSpace[1];
    // qInfo() << "Screen Point: " << screen_point[0] << ", " << screen_point[1];

    // 2. Add point to referenceLine
    referenceLine.push_back({ point.x(), point.y() });

    // 3. Generate a parallel line
    std::vector<Point> parallelLine = m_openglHelper.generateParallelCurve(referenceLine, width);

    referenceLine.insert(referenceLine.end(), parallelLine.begin(), parallelLine.end());

    // for (Point point: referenceLine)
    // {
    //     qInfo() << "x: " << point[0] << " , y: " << point[1];
    // }

    // 4. Get triangulated mesh
    polygon.push_back(referenceLine);
    polygon.push_back({}); // for holes
    std::vector<uint32_t> indices = mapbox::earcut<uint32_t>(polygon);

    // for (uint32_t index: indices)
    // {
    //     qInfo() << index;
    // }

    // 5. Create and export mesh
    std::vector<Position> vertices_position = {};
    std::vector<Normal> vertices_normal = {};
    std::vector<TextureUV> vertices_textureuv = {};
    std::vector<int> vertices_materialIndex = {};
    std::vector<int> vertices_textureIndex = {};
    for (int i = 0; i < referenceLine.size(); i++)
    {
        Point point = referenceLine[i];

        // Vertex v = {
        //     {point[0], point[1], 0.0f},
        //     {0.0f, 0.0f, 1.0f},
        //     {0.0f, 0.0f},
        //     OpenGLMaterial::IVORY,
        //     Texture::NONE
        // };

        vertices_position.push_back({point[0], point[1], 0.0f, 0.0f});
        vertices_normal.push_back({0.0f, 0.0f, 1.0f});
        vertices_textureuv.push_back({0.0f, 0.0f});
        vertices_materialIndex.push_back(OpenGLMaterial::IVORY);
        vertices_textureIndex.push_back(Texture::NONE);

        // verticesVector.push_back(point[0]); // x
        // verticesVector.push_back(point[1]); // y
        // verticesVector.push_back(0.0f); // z
        // verticesVector.push_back(0.0f); // n.x
        // verticesVector.push_back(0.0f); // n.y
        // verticesVector.push_back(1.0f); // n.z
    }

    std::vector<int> edge_indices = {};
    std::vector<EdgeDataInt> edge_data_int = {};
    std::vector<EdgeDataFloat> edge_data_float = {};
    for (int i = 0; i < referenceLine.size(); i++)
    {   
        EdgeDataInt ei;
        EdgeDataFloat ef;

        ei.material_index = OpenGLMaterial::BLACK;
        ei.dash = 0;
        ef.width = 1.0f;
        ef.dash_length = 1.0f;
        ef.gap_length = 1.0f;
        ef.padding = 0.0f;

        if (i == referenceLine.size() - 1)
        {
            ei.start_vertex = i;
            ei.end_vertex = 0;
        }
        else
        {
            ei.start_vertex = i;
            ei.end_vertex = i + 1;
        }

        edge_data_int.push_back(ei);
        edge_data_float.push_back(ef);
        edge_indices.push_back(i);
    }


    // Add the vertex and edges of helper lines
    vertices_position.push_back({wcs_point1[0], wcs_point1[1], 0.0f, 0.0f});
    vertices_normal.push_back({0.0f, 0.0f, 1.0f});
    vertices_textureuv.push_back({0.0f, 0.0f});
    vertices_materialIndex.push_back(OpenGLMaterial::IVORY);
    vertices_textureIndex.push_back(Texture::NONE);

    vertices_position.push_back({wcs_point2[0], wcs_point2[1], 0.0f, 0.0f});
    vertices_normal.push_back({0.0f, 0.0f, 1.0f});
    vertices_textureuv.push_back({0.0f, 0.0f});
    vertices_materialIndex.push_back(OpenGLMaterial::IVORY);
    vertices_textureIndex.push_back(Texture::NONE);

    vertices_position.push_back({wcs_point3[0], wcs_point3[1], 0.0f, 0.0f});
    vertices_normal.push_back({0.0f, 0.0f, 1.0f});
    vertices_textureuv.push_back({0.0f, 0.0f});
    vertices_materialIndex.push_back(OpenGLMaterial::IVORY);
    vertices_textureIndex.push_back(Texture::NONE);

    vertices_position.push_back({wcs_point4[0], wcs_point4[1], 0.0f, 0.0f});
    vertices_normal.push_back({0.0f, 0.0f, 1.0f});
    vertices_textureuv.push_back({0.0f, 0.0f});
    vertices_materialIndex.push_back(OpenGLMaterial::IVORY);
    vertices_textureIndex.push_back(Texture::NONE);

    for (int i = 0; i < 3; i++)
    {
        int first_point = referenceLine.size() + i;
        int second_point = referenceLine.size() + i + 1;

        EdgeDataInt ei;
        EdgeDataFloat ef;

        ei.material_index = OpenGLMaterial::BLACK;
        ei.dash = 1;
        ef.width = 2.0f;
        ef.dash_length = 5.0f;
        ef.gap_length = 5.0f;
        ef.padding = 0.0f;

        ei.start_vertex = first_point;
        ei.end_vertex = second_point;

        edge_data_int.push_back(ei);
        edge_data_float.push_back(ef);
        edge_indices.push_back(edge_indices.size());
    }


    ///////////////////////////////////////////
    // Add angle and its helper points

    // second last point of referenceLine
    if (referenceLineSize >= 2)
    {
        helperPoints[1].visible = true;

        Point secondLastPointReferenceLine = referenceLine[referenceLineSize - 2];
        QVector3D secondLastPoint(secondLastPointReferenceLine[0], secondLastPointReferenceLine[1], 0.0f);
        Point secondLastPointScreenSpace = view->GetPointInScreenSpace(secondLastPoint);

        float angle_degrees = m_openglHelper.getAngleBetweenPoints(secondLastPointReferenceLine, lastPointReferenceLine, {point[0], point[1]});

        // angles are negative because the screen-coordinates are following the left-handle rule
        // y axis points downwards
        Point angle_point0 = m_openglHelper.getPointAtDistanceAngle(secondLastPointScreenSpace, lastPointScreenSpace, 0.0f, 20);
        Point angle_point1 = m_openglHelper.getPointAtDistanceAngle(secondLastPointScreenSpace, lastPointScreenSpace, -1*angle_degrees/4, 20);
        Point angle_point2 = m_openglHelper.getPointAtDistanceAngle(secondLastPointScreenSpace, lastPointScreenSpace, -1*angle_degrees/2, 20);
        Point angle_point3 = m_openglHelper.getPointAtDistanceAngle(secondLastPointScreenSpace, lastPointScreenSpace, -3*angle_degrees/4, 20);
        Point angle_point4 = m_openglHelper.getPointAtDistanceAngle(secondLastPointScreenSpace, lastPointScreenSpace, -1*angle_degrees, 20);

        QVector3D wcs_angle_point0 = view->GetPointInViewSpace(angle_point0[0], angle_point0[1]);
        QVector3D wcs_angle_point1 = view->GetPointInViewSpace(angle_point1[0], angle_point1[1]);
        QVector3D wcs_angle_point2 = view->GetPointInViewSpace(angle_point2[0], angle_point2[1]);
        QVector3D wcs_angle_point3 = view->GetPointInViewSpace(angle_point3[0], angle_point3[1]);
        QVector3D wcs_angle_point4 = view->GetPointInViewSpace(angle_point4[0], angle_point4[1]);


        helperPoints[1].x = angle_point2[0];
        helperPoints[1].y = angle_point2[1] + 30; // go 30 px down
        helperPoints[1].value = angle_degrees;

        // Add the vertex and edges of helper lines
        vertices_position.push_back({wcs_angle_point0[0], wcs_angle_point0[1], 0.0f, 0.0f});
        vertices_normal.push_back({0.0f, 0.0f, 1.0f});
        vertices_textureuv.push_back({0.0f, 0.0f});
        vertices_materialIndex.push_back(OpenGLMaterial::IVORY);
        vertices_textureIndex.push_back(Texture::NONE);

        vertices_position.push_back({wcs_angle_point1[0], wcs_angle_point1[1], 0.0f, 0.0f});
        vertices_normal.push_back({0.0f, 0.0f, 1.0f});
        vertices_textureuv.push_back({0.0f, 0.0f});
        vertices_materialIndex.push_back(OpenGLMaterial::IVORY);
        vertices_textureIndex.push_back(Texture::NONE);

        vertices_position.push_back({wcs_angle_point2[0], wcs_angle_point2[1], 0.0f, 0.0f});
        vertices_normal.push_back({0.0f, 0.0f, 1.0f});
        vertices_textureuv.push_back({0.0f, 0.0f});
        vertices_materialIndex.push_back(OpenGLMaterial::IVORY);
        vertices_textureIndex.push_back(Texture::NONE);

        vertices_position.push_back({wcs_angle_point3[0], wcs_angle_point3[1], 0.0f, 0.0f});
        vertices_normal.push_back({0.0f, 0.0f, 1.0f});
        vertices_textureuv.push_back({0.0f, 0.0f});
        vertices_materialIndex.push_back(OpenGLMaterial::IVORY);
        vertices_textureIndex.push_back(Texture::NONE);

        vertices_position.push_back({wcs_angle_point4[0], wcs_angle_point4[1], 0.0f, 0.0f});
        vertices_normal.push_back({0.0f, 0.0f, 1.0f});
        vertices_textureuv.push_back({0.0f, 0.0f});
        vertices_materialIndex.push_back(OpenGLMaterial::IVORY);
        vertices_textureIndex.push_back(Texture::NONE);


        for (int i = 0; i < 4; i++)
        {
            // 4 is added to account for length helper point
            int first_point = referenceLine.size() + 4 + i;
            int second_point = referenceLine.size() + 4 + i + 1;

            EdgeDataInt ei;
            EdgeDataFloat ef;

            ei.material_index = OpenGLMaterial::BLACK;
            ei.dash = 1;
            ef.width = 2.0f;
            ef.dash_length = 5.0f;
            ef.gap_length = 5.0f;
            ef.padding = 0.0f;

            ei.start_vertex = first_point;
            ei.end_vertex = second_point;

            edge_data_int.push_back(ei);
            edge_data_float.push_back(ef);
            edge_indices.push_back(edge_indices.size());
        }
    }

    // // Allocate memory for the new array using std::unique_ptr for safety.
    // auto indices_raw = std::make_unique<unsigned int[]>(indices.size());

    // // Copy elements from the vector to the new array.
    // std::copy(indices.begin(), indices.end(), indices_raw.get());

    // Mesh* mesh = new Mesh(this);
    mesh->Initialize(
        vertices_position,
        vertices_normal,
        vertices_textureuv,
        vertices_materialIndex,
        vertices_textureIndex,
        edge_data_int,
        edge_data_float,
        indices,
        edge_indices
    );
    mesh->setBIMElementId(wallElement->getId());


    // GLfloat* vertices1 = mesh->getVerticies();
    // unsigned int* indices1 = mesh->getIndices();
    // for (int i = 0; i < 6; i++)
    // {
    //     qInfo() << vertices1[6*i] << " , " << vertices1[6*i + 1] << " , " << vertices1[6*i + 2] << " , " << vertices1[6*i + 3] << " , " << vertices1[6*i + 4] << " , " << vertices1[6*i + 5];
    // }

    // qInfo() << "----------------------------------";

    // for (int i = 0; i < 6; i++)
    // {
    //     qInfo() << indices1[i];
    // }

    // return mesh;
}

Point WallGeometryService::updatePoint2D(BIMElement *wallElement, const QList<HelperPoint> &helperPoints, const Point &screen_point, View *view)
{
    std::vector<std::vector<Point>> polygon;
    std::vector<Point> referenceLine = {};
    float width = 0;
    float height = 0;
    float distance = 0;

    m_openglHelper.extractBIMParameters(wallElement, referenceLine, width, height, distance);

    // last point of referenceLine
    Point lastPointReferenceLine = referenceLine.back();

    QVector3D screenPointInViewSpace = view->GetPointInViewSpace(screen_point[0], screen_point[1]);
    Point screenPointInViewSpace2D = {screenPointInViewSpace[0], screenPointInViewSpace[1]};

    Point newPointViewSpace = {0.0f, 0.0f};

    if (referenceLine.size() < 2)
    {
        newPointViewSpace = m_openglHelper.getPointAtDistance(lastPointReferenceLine, screenPointInViewSpace2D, helperPoints[0].value);
    }
    else
    {
        Point secondLastPointReferenceLine = referenceLine[referenceLine.size() - 2];
        newPointViewSpace = m_openglHelper.getPointAtDistanceAngle(secondLastPointReferenceLine, lastPointReferenceLine, helperPoints[1].value, helperPoints[0].value);
    }

    QVector3D newPoint(newPointViewSpace[0], newPointViewSpace[1], 0.0f);

    Point newPointScreenSpace = view->GetPointInScreenSpace(newPoint);

    return newPointScreenSpace;
}

void WallGeometryService::generateHelperPoints(BIMElement *bimElement, QList<HelperPoint> &helperPoints)
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
