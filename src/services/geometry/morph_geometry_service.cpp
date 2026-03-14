#include "morph_geometry_service.h"

MorphGeometryService::MorphGeometryService(QObject *parent)
    : QObject{parent}
{}

void MorphGeometryService::generateMesh2D(Morph *morphModel, Mesh *mesh)
{
    ENTITY_LIST& entity_list_2d = morphModel->morph_bodies_2D();

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


    for (BODY* &new_body: entity_list_2d)
    {
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
    mesh->setBIMElementId(morphModel->id());
}

void MorphGeometryService::generateMesh3D(Morph *morphModel, Mesh *mesh)
{
    ENTITY_LIST& entity_list_3d = morphModel->morph_bodies_3D();

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


    for (BODY* &new_body: entity_list_3d)
    {
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
    mesh->setBIMElementId(morphModel->id());
}

void MorphGeometryService::updateGeometry(Morph *morphModel, const QVector3D &point, EditOption *editOption)
{
    QString editType = editOption->editType();

    if (editType == "line")
    {
        generateLineEdge(morphModel, editOption, point);
    }
}

Morph *MorphGeometryService::generateBimModel(BIMElement *bimElement)
{
    QList<BIMParameter*> parameterList = bimElement->getParameterList();

    // 2. Find SATFileName
    QString SATFileName3DString = "";
    QString SATFileName2DString = "";

    for (BIMParameter* parameter: parameterList)
    {
        if (parameter->getKey() == "SATFileName3D")
        {
            SATFileName3DString = parameter->getValue();
        }
        else if (parameter->getKey() == "SATFileName2D")
        {
            SATFileName2DString = parameter->getValue();
        }
    }

    if (SATFileName3DString == "")
    {
        QUuid uuid = QUuid::createUuid();
        SATFileName3DString = uuid.toString();
    }

    if (SATFileName2DString == "")
    {
        QUuid uuid = QUuid::createUuid();
        SATFileName2DString = uuid.toString();
    }

    QString SATFilePath3DString = gEnvironmentPath + "\\" + gProjectName + "\\" + SATFileName3DString + ".sat";
    QString SATFilePath2DString = gEnvironmentPath + "\\" + gProjectName + "\\" + SATFileName2DString + ".sat";

    // 3. Read the SAT file to generate entity_list
    Morph* morphModel = new Morph();
    morphModel->setId(bimElement->getId());
    morphModel->setSATFileName2D(SATFileName2DString);
    morphModel->setSATFileName3D(SATFileName3DString);

    ENTITY_LIST& entity_list_3d = morphModel->morph_bodies_3D();
    ENTITY_LIST& entity_list_2d = morphModel->morph_bodies_2D();

    m_openglHelper.readSATFile(SATFilePath3DString, entity_list_3d);
    m_openglHelper.readSATFile(SATFilePath2DString, entity_list_2d);

    return morphModel;
}

void MorphGeometryService::updateBimElement(BIMElement *wallElement, Morph *morphModel)
{
    ENTITY_LIST& entity_list_3d = morphModel->morph_bodies_3D();
    ENTITY_LIST& entity_list_2d = morphModel->morph_bodies_2D();

    QString SATFilePath3DString = gEnvironmentPath + "\\" + gProjectName + "\\" + morphModel->getSATFileName3D() + ".sat";
    QString SATFilePath2DString = gEnvironmentPath + "\\" + gProjectName + "\\" + morphModel->getSATFileName3D() + ".sat";

    m_openglHelper.saveSATFile(SATFilePath3DString, entity_list_3d);
    m_openglHelper.saveSATFile(SATFilePath2DString, entity_list_2d);

    QList<BIMParameter*> parameterList = wallElement->getParameterList();

    for (BIMParameter* parameter: parameterList)
    {
        if (parameter->getKey() == "SATFileName3D")
        {
            parameter->setValue(morphModel->getSATFileName3D());
        }
        else if (parameter->getKey() == "SATFileName2D")
        {
            parameter->setValue(morphModel->getSATFileName2D());
        }
    }
}

void MorphGeometryService::generateLineEdge(Morph *morphModel, EditOption *editOption, const QVector3D &point)
{
    std::vector<SPAposition> points;
    ENTITY_LIST& entity_list;

    // 1. get points
    if (editOption->viewType() == "ModelView")
    {
        points = morphModel->morph_points_3D();
        entity_list = morphModel->morph_bodies_3D();
    }
    else
    {
        points = morphModel->morph_points_2D();
        entity_list = morphModel->morph_bodies_2D();
    }

    // 2. make updates
    if (points.size() == 0)
    {
        points.push_back(SPAposition(point.x(), point.y(), point.z()));
    }
    else
    {
        BODY* wire_body;
        std::vector<EDGE*> edges;

        EDGE* edge = nullptr;
        api_curve_line(
            points[0],
            SPAposition(point.x(), point.y(), point.z()),
            edge
        );
        edges.push_back(edge);

        api_make_ewire(1, edges.data(), wire_body);

        if (wire_body != nullptr) entity_list.add(wire_body);

        points = {};
    }

    // 3. save update to points
    if (editOption->viewType() == "ModelView")
    {
        morphModel->setMorph_points_3D(points);
    }
    else
    {
        morphModel->setMorph_points_2D(points);
    }
}
