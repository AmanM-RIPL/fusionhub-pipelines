#include "myglitem.h"


void calcAverageNormals(unsigned int* indices, unsigned int indiceCount, GLfloat* vertices, unsigned int verticeCount, unsigned int vLength, unsigned int normalOffset)
{
    for (size_t i = 0; i < indiceCount; i += 3)
    {
        unsigned int in0 = indices[i] * vLength;
        unsigned int in1 = indices[i + 1] * vLength;
        unsigned int in2 = indices[i + 2] * vLength;

        QVector3D v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
        QVector3D v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);

        QVector3D normal = QVector3D::crossProduct(v1, v2);
        normal.normalize();

        in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
        vertices[in0] += normal.x(); vertices[in0 + 1] += normal.y(); vertices[in0 + 2] += normal.z();
        vertices[in1] += normal.x(); vertices[in1 + 1] += normal.y(); vertices[in1 + 2] += normal.z();
        vertices[in2] += normal.x(); vertices[in2 + 1] += normal.y(); vertices[in2 + 2] += normal.z();
    }

    for (size_t i = 0; i < verticeCount / vLength; i++)
    {
        unsigned int nOffset = i * vLength + normalOffset;
        QVector3D vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
        vec.normalize();
        vertices[nOffset] = vec.x(); vertices[nOffset + 1] = vec.y(); vertices[nOffset + 2] = vec.z();
    }
}

MyGLRenderer::MyGLRenderer()
{
    initializeOpenGLFunctions();
    // initGL();
    // initShaders();

    // initialize Mesh
    m_vertices[0] = 0.0f;  m_vertices[1] =  0.8f; m_vertices[2] = 0.0f; // top
    m_vertices[3] = -0.8f; m_vertices[4] = -0.8f; m_vertices[5] = 0.0f; // bottom
    m_vertices[6] = 0.8f;  m_vertices[7] = -0.8f; m_vertices[8] = 0.0f; // left

    // GLfloat verticies[] = {
    //     //    x,     y,    z,  n.x, n.y, n.z
    //     -1.0f, -1.0f, -0.6f, 0.0f, 0.0f, 0.0f,
    //     0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f,
    //     1.0f, -1.0f, -0.6f, 0.0f, 0.0f, 0.0f,
    //     0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f
    // };

    // unsigned int indices[] = {
    //     0, 3, 1,
    //     1, 3, 2,
    //     2, 3, 0,
    //     0, 1, 2
    // };

    // std::vector<GLfloat> verticies = {
    //     //    x,     y,    z,  n.x, n.y, n.z
    //     -1.0f, -1.0f, -0.6f, 0.0f, 0.0f, 0.0f,
    //     0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f,
    //     1.0f, -1.0f, -0.6f, 0.0f, 0.0f, 0.0f,
    //     0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f
    // };

    // std::vector<unsigned int> indices = {
    //     0, 3, 1,
    //     1, 3, 2,
    //     2, 3, 0,
    //     0, 1, 2
    // };

    // calcAverageNormals(indices.data(), 12, verticies.data(), 24, 6, 3);

    // m_mesh = new Mesh();
    // m_mesh->Initialize(verticies, indices, 24, 12);
    // mesh->Copy(m_mesh);

    // initialize Camera
    m_camera = new Camera();
    m_camera->Initialize(QVector3D(0.0f, 0.0f, -1.0f), QVector3D(0.0f, 1.0f, 0.0f), QVector3D(0.0f, 0.0f, 20.0f), 5.0f, 0.5f);

    // initialize Shader
    m_shader = new Shader();
    m_shader->CreateFromFiles("://resources/shaders/default.vert", "://resources/shaders/default.frag");

    // initialize Picking Shader
    m_picking_shader = new Shader();
    m_picking_shader->SetPickColor(true);
    m_picking_shader->CreateFromFiles("://resources/shaders/color-picking.vert", "://resources/shaders/color-picking.frag");

    // initialize Materials
    OpenGLMaterial* whiteMaterial = new OpenGLMaterial();
    whiteMaterial->setAmbient({1.0f, 1.0f, 1.0f}); // 0.96, 0.47f, 0.02f
    whiteMaterial->setDiffuse({1.0f, 1.0f, 1.0f}); // 0.0f, 0.5f, 0.31f
    whiteMaterial->setSpecular({1.0f, 1.0f, 1.0f}); // 0.5f, 0.5f, 0.5f
    whiteMaterial->setShininess(32.0f);
    m_materialList.append(whiteMaterial);

    OpenGLMaterial* blueMaterial = new OpenGLMaterial();
    blueMaterial->setAmbient({0.68f, 0.85f, 0.90f}); // 0.96, 0.47f, 0.02f
    blueMaterial->setDiffuse({1.0f, 1.0f, 1.0f}); // 0.0f, 0.5f, 0.31f
    blueMaterial->setSpecular({1.0f, 1.0f, 1.0f}); // 0.5f, 0.5f, 0.5f
    blueMaterial->setShininess(32.0f);
    m_materialList.append(blueMaterial);

    // initialize Textures
    Texture* texture = new Texture();
    texture->LoadTexture("://resources/images/brick.jpg");
    m_textureList.append(texture);

    // initialize View
    m_view = new View();
    // m_view->AddMesh(m_mesh);
    m_view->AddMaterial(whiteMaterial);
    m_view->AddMaterial(blueMaterial);
    m_view->AddTexture(texture);
    m_view->AddCamera(m_camera);
    m_view->AddShader(m_shader);
    m_view->AddPickingShader(m_picking_shader);
    // m_view->Initialize();
}

MyGLRenderer::~MyGLRenderer()
{
    // if (m_shaderProgram) this->glDeleteProgram(m_shaderProgram);
    // if (m_vbo) this->glDeleteBuffers(1, &m_vbo);
    // if (m_vao) this->glDeleteVertexArrays(1, &m_vao);

    // if (m_pickFBO) {
    //     this->glDeleteFramebuffers(1, &m_pickFBO);
    //     this->glDeleteTextures(1, &m_pickColorTex);
    //     this->glDeleteRenderbuffers(1, &m_pickDepthBuf);
    // }

    // delete m_mesh;
    delete m_camera;
    delete m_shader;
    delete m_picking_shader;
    delete m_view;

    for (OpenGLMaterial* material : m_materialList)
    {
        delete material;
    }
    m_materialList.clear();

    for (Texture* texture : m_textureList)
    {
        delete texture;
    }
    m_textureList.clear();

    for (Mesh* mesh: m_meshList)
    {
        delete mesh;
    }
    m_meshList.clear();
}

void MyGLRenderer::synchronize(QQuickFramebufferObject *item)
{
    // qInfo() << "Syncronize Function";
    MyGLItem* glItem = static_cast<MyGLItem*>(item);


    // Orbit only works in 3D mode and not in 2D
    if (glItem->m_moveUp && glItem->m_viewType == "ModelView") {
        m_camera->OrbitVertical(true);
    }
    glItem->m_moveUp = false;  // reset

    if (glItem->m_moveDown && glItem->m_viewType == "ModelView") {
        m_camera->OrbitVertical(false);
    }
    glItem->m_moveDown = false;

    if (glItem->m_moveLeft && glItem->m_viewType == "ModelView") {
        m_camera->OrbitHorizontal(false);
    }
    glItem->m_moveLeft = false;

    if (glItem->m_moveRight && glItem->m_viewType == "ModelView") {
        m_camera->OrbitHorizontal(true);
    }
    glItem->m_moveRight = false;


    float panLength = 0.1f;
    if (glItem->m_panUp) {
        m_camera->Pan(0.0f,panLength);
        glItem->m_panUp = false;  // reset
    }

    if (glItem->m_panDown) {
        m_camera->Pan(0.0f, -panLength);
        glItem->m_panDown = false;
    }

    if (glItem->m_panLeft) {
        m_camera->Pan(-panLength, 0.0f);
        glItem->m_panLeft = false;
    }

    if (glItem->m_panRight) {
        m_camera->Pan(panLength, 0.0f);
        glItem->m_panRight = false;
    }


    if (glItem->m_zoomIn)
    {
        m_camera->Zoom(true);
        glItem->m_zoomIn = false;
    }
    else if (glItem->m_zoomOut)
    {
        m_camera->Zoom(false);
        glItem->m_zoomOut = false;
    }

    // transfer click request safely
    if (meshInitialized && projectionMatrixInitialized && glItem->m_lastClickX >= 0) {
        m_pickX = glItem->m_lastClickX;
        m_pickY = glItem->m_lastClickY;
        m_pickRequested = true;
        // reset the stored GUI-side coords so we don't re-process
        glItem->m_lastClickX = -1;
        glItem->m_lastClickY = -1;

        m_view->SetSelectionCoordinates(m_pickX, m_pickY);
        QVector3D clickedPoint = m_view->GetPointInViewSpace();
        m_view->Selection();

        // update glItem BIM Element
        if (glItem->editableBimElement != nullptr)
        {
            GeometryServiceFactory::updateGeometry(glItem->editableBimElement, clickedPoint);

            // removing old geometry
            for (Mesh* mesh: m_meshList)
            {
                delete mesh;
            }
            m_meshList.clear();
            m_view->DeleteAllMesh();

            for (BIMElement* bimElement: glItem->bimElementList)
            {
                // adding new geometry
                Mesh* mesh = new Mesh();
                m_meshList.append(mesh);
                m_view->AddMesh(mesh);

                if (glItem->m_viewType == "ModelView")
                {
                    GeometryServiceFactory::generateMesh3D(bimElement, mesh, glItem->pIfcDetailController, glItem->pIfcGeometryService);
                }
                else if (glItem->m_viewType == "PlanView")
                {
                    GeometryServiceFactory::generateMesh2D(bimElement, mesh);
                }
            }

            if (glItem->m_viewType == "ModelView")
            {
                Mesh* mesh = new Mesh();
                m_meshList.append(mesh);
                m_view->AddMesh(mesh);

                Mesh::GenerateBaseSurface(mesh);
            }

            // generating mesh for Editable BIMElement
            Mesh* mesh = new Mesh();
            m_meshList.append(mesh);
            m_view->AddMesh(mesh);

            if (glItem->m_viewType == "ModelView")
            {
                GeometryServiceFactory::generateMesh3D(glItem->editableBimElement, mesh, glItem->pIfcDetailController, glItem->pIfcGeometryService);
            }
            else if (glItem->m_viewType == "PlanView")
            {
                GeometryServiceFactory::generateMesh2D(glItem->editableBimElement, mesh);
            }


            m_view->BindMeshWithOpenGL();
        }
    }

    if (!meshInitialized)
    {
        for (BIMElement* bimElement: glItem->bimElementList)
        {
            Mesh* mesh = new Mesh();
            m_meshList.append(mesh);
            m_view->AddMesh(mesh);

            if (glItem->m_viewType == "ModelView")
            {
                GeometryServiceFactory::generateMesh3D(bimElement, mesh, glItem->pIfcDetailController, glItem->pIfcGeometryService);
            }
            else if (glItem->m_viewType == "PlanView")
            {
                GeometryServiceFactory::generateMesh2D(bimElement, mesh);
            }
        }

        if (glItem->m_viewType == "ModelView")
        {
            Mesh* mesh = new Mesh();
            m_meshList.append(mesh);
            m_view->AddMesh(mesh);

            Mesh::GenerateBaseSurface(mesh);
        }

        m_view->Initialize();

        meshInitialized = true;
        m_viewType = glItem->m_viewType;
    }

    if (m_viewType != glItem->m_viewType)
    {
        // removing old geometry
        for (Mesh* mesh: m_meshList)
        {
            delete mesh;
        }
        m_meshList.clear();
        m_view->DeleteAllMesh();

        for (BIMElement* bimElement: glItem->bimElementList)
        {
            // adding new geometry
            Mesh* mesh = new Mesh();
            m_meshList.append(mesh);
            m_view->AddMesh(mesh);

            if (glItem->m_viewType == "ModelView")
            {
                GeometryServiceFactory::generateMesh3D(bimElement, mesh, glItem->pIfcDetailController, glItem->pIfcGeometryService);
            }
            else if (glItem->m_viewType == "PlanView")
            {
                GeometryServiceFactory::generateMesh2D(bimElement, mesh);
            }
        }

        if (glItem->m_viewType == "ModelView")
        {
            Mesh* mesh = new Mesh();
            m_meshList.append(mesh);
            m_view->AddMesh(mesh);

            Mesh::GenerateBaseSurface(mesh);
        }

        m_view->BindMeshWithOpenGL();

        m_viewType = glItem->m_viewType;

        m_camera->SetCameraParameters(QVector3D(0.0f, 0.0f, -1.0f), QVector3D(0.0f, 1.0f, 0.0f), QVector3D(0.0f, 0.0f, 20.0f), 5.0f, 0.5f);
    }
}

void MyGLRenderer::update() {
    // Continuous rendering
    QQuickFramebufferObject::Renderer::update();
}

void MyGLRenderer::initGL() {
    m_vertices[0] = 0.0f;  m_vertices[1] =  0.8f; m_vertices[2] = 0.0f; // top
    m_vertices[3] = -0.8f; m_vertices[4] = -0.8f; m_vertices[5] = 0.0f; // bottom
    m_vertices[6] = 0.8f;  m_vertices[7] = -0.8f; m_vertices[8] = 0.0f; // left

    this->glEnable(GL_DEPTH_TEST);

    this->glGenVertexArrays(1, &m_vao);
    this->glGenBuffers(1, &m_vbo);

    this->glBindVertexArray(m_vao);

        this->glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
            this->glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertices), m_vertices, GL_DYNAMIC_DRAW);

            this->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
            this->glEnableVertexAttribArray(0);

        this->glBindBuffer(GL_ARRAY_BUFFER, 0);
    this->glBindVertexArray(0);
}

void MyGLRenderer::initShaders() {
    const char *vshaderSrc =
        "#version 330 core\n"
        "layout (location = 0) in vec3 position;\n"
        "uniform mat4 u_model;\n"
        "uniform mat4 u_view;\n"
        "uniform mat4 u_proj;\n"
        "void main() {\n"
        "   gl_Position = u_proj * u_view * u_model * vec4(position, 1.0);\n"
        "}\n";

    const char *fshaderSrc =
        "#version 330 core\n"
        "out vec4 FragColor;\n"
        "void main() {\n"
        "   FragColor = vec4(0.9, 0.3, 0.2, 1.0);\n"
        "}\n";

    const char *vshaderPickSrc =
        "#version 330 core\n"
        "layout (location = 0) in vec3 position;\n"
        "uniform mat4 u_model;\n"
        "uniform mat4 u_view;\n"
        "uniform mat4 u_proj;\n"
        "void main() {\n"
        "   gl_Position = u_proj * u_view * u_model * vec4(position, 1.0);\n"
        "}\n";


    const char *fshaderPickSrc =
        "#version 330 core\n"
        "uniform vec4 u_pickColor;\n"
        "out vec4 FragColor;\n"
        "void main() {\n"
        "   FragColor = u_pickColor;\n"
        "}\n";

    GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vshaderSrc);
    GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fshaderSrc);
    GLuint vertexShaderPick = compileShader(GL_VERTEX_SHADER, vshaderPickSrc);
    GLuint fragmentShaderPick = compileShader(GL_FRAGMENT_SHADER, fshaderPickSrc);

    // First Pass for picking -------------------------------------------
    m_pickProgram = this->glCreateProgram();
    this->glAttachShader(m_pickProgram, vertexShaderPick);
    this->glAttachShader(m_pickProgram, fragmentShaderPick);
    this->glLinkProgram(m_pickProgram);

    GLint pickSuccess;
    this->glGetProgramiv(m_pickProgram, GL_LINK_STATUS, &pickSuccess);
    if (!pickSuccess) {
        char log[512];
        this->glGetProgramInfoLog(m_pickProgram, 512, nullptr, log);
        qWarning() << "Pick link error:" << log;
    }

    // cleanup shaders
    this->glDeleteShader(vertexShaderPick);
    this->glDeleteShader(fragmentShaderPick);

    // get uniform locations
    m_pickModelLoc = this->glGetUniformLocation(m_pickProgram, "u_model");
    m_pickViewLoc  = this->glGetUniformLocation(m_pickProgram, "u_view");
    m_pickProjLoc  = this->glGetUniformLocation(m_pickProgram, "u_proj");
    m_pickColorLoc = glGetUniformLocation(m_pickProgram, "u_pickColor");


    // Main program for rendering ----------------------------------------
    m_shaderProgram = this->glCreateProgram();
    this->glAttachShader(m_shaderProgram, vertexShader);
    this->glAttachShader(m_shaderProgram, fragmentShader);
    this->glLinkProgram(m_shaderProgram);

    GLint success;
    this->glGetProgramiv(m_shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        char log[512];
        this->glGetProgramInfoLog(m_shaderProgram, 512, nullptr, log);
        qWarning() << "Shader link error:" << log;
    }

    // add uniform variable u_matrix to vertex shader
    m_modelLoc = this->glGetUniformLocation(m_shaderProgram, "u_model");
    m_viewLoc  = this->glGetUniformLocation(m_shaderProgram, "u_view");
    m_projLoc  = this->glGetUniformLocation(m_shaderProgram, "u_proj");

    this->glDeleteShader(vertexShader);
    this->glDeleteShader(fragmentShader);
}

GLuint MyGLRenderer::compileShader(GLenum type, const char* src) {
    GLuint shader = glCreateShader(type);
    this->glShaderSource(shader, 1, &src, nullptr);
    this->glCompileShader(shader);

    GLint success;
    this->glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char log[512];
        this->glGetShaderInfoLog(shader, 512, nullptr, log);
        qWarning() << "Shader compile error:" << log;
    }
    return shader;
}

void MyGLRenderer::ensurePickFBO(int w, int h) {
    if (m_pickFBO != 0) {
        this->glDeleteFramebuffers(1, &m_pickFBO);
        this->glDeleteTextures(1, &m_pickColorTex);
        this->glDeleteRenderbuffers(1, &m_pickDepthBuf);
        m_pickFBO = m_pickColorTex = m_pickDepthBuf = 0;
    }

    // Generate framebuffer
    this->glGenFramebuffers(1, &m_pickFBO);
    this->glBindFramebuffer(GL_FRAMEBUFFER, m_pickFBO);

        // --- Color attachment (texture) ---
        this->glGenTextures(1, &m_pickColorTex);
        this->glBindTexture(GL_TEXTURE_2D, m_pickColorTex);
        this->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0,
                     GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        this->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        this->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        this->glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                               GL_TEXTURE_2D, m_pickColorTex, 0);

        // --- Depth attachment (renderbuffer) ---
        this->glGenRenderbuffers(1, &m_pickDepthBuf);
        this->glBindRenderbuffer(GL_RENDERBUFFER, m_pickDepthBuf);
        this->glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, w, h);
        this->glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                                  GL_RENDERBUFFER, m_pickDepthBuf);

        // Check FBO status
        if (this->glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            qWarning() << "Picking FBO is not complete!";
        }

    // Unbind
    GLuint defaultFbo = framebufferObject()->handle();
    this->glBindFramebuffer(GL_FRAMEBUFFER, defaultFbo);
}

void MyGLRenderer::encodeIdToColor(unsigned int id, unsigned char &r, unsigned char &g, unsigned char &b) {
    r = (id & 0x000000FF);
    g = (id & 0x0000FF00) >> 8;
    b = (id & 0x00FF0000) >> 16;
}


void MyGLRenderer::moveTopVertexToClick(int mouseX, int mouseY, const QMatrix4x4 &proj, const QMatrix4x4 &view, const QMatrix4x4 &model)
{
    int w = framebufferObject()->width();
    int h = framebufferObject()->height();

    // --- Step 1: Screen -> NDC
    float x = (2.0f * mouseX) / float(w) - 1.0f;
    float y = (2.0f * mouseY) / float(h) - 1.0f; // no inversion as Y-axis flip is already in proj matrix
    QVector4D rayClip(x, y, -1.0f, 1.0f);

    // --- Step 2: NDC -> Eye space
    QVector4D rayEye = proj.inverted() * rayClip;
    rayEye = QVector4D(rayEye.x(), rayEye.y(), -1.0f, 0.0f);

    // --- Step 3: Eye -> World space
    QVector3D rayDirWorld = (view.inverted() * rayEye).toVector3D().normalized();
    QVector3D rayOriginWorld = m_cameraPos;

    // --- Step 4: Transform ray into *model space*
    QMatrix4x4 invModel = model.inverted();
    QVector3D rayOriginModel = (invModel * QVector4D(rayOriginWorld, 1.0f)).toVector3D();
    QVector3D rayDirModel    = (invModel * QVector4D(rayDirWorld, 0.0f)).toVector3D().normalized();

    // --- Step 5: Ray-plane intersection in model space (Z=0 plane)
    QVector3D planeNormal(0, 0, 1);
    QVector3D planePoint(0, 0, 0);
    float denom = QVector3D::dotProduct(planeNormal, rayDirModel);
    if (fabs(denom) < 1e-6f) {
        qWarning() << "Ray parallel to model plane, no intersection";
        return;
    }
    float t = QVector3D::dotProduct(planePoint - rayOriginModel, planeNormal) / denom;
    if (t < 0) {
        qWarning() << "Intersection is behind camera";
        return;
    }

    QVector3D hitPoint = rayOriginModel + t * rayDirModel;

    // --- Step 5: Update vertex (index 0 = top vertex)
    m_vertices[0] = hitPoint.x();
    m_vertices[1] = hitPoint.y();
    m_vertices[2] = hitPoint.z();

    // --- Step 6: Push updated vertices to GPU
    this->glBindVertexArray(m_vao);
        this->glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
            this->glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(m_vertices), m_vertices);
        this->glBindBuffer(GL_ARRAY_BUFFER, 0);
    this->glBindVertexArray(0);
}


void MyGLRenderer::render() {

    if (!projectionMatrixInitialized)
    {
        // famebufferObject() is null when syncronize()
        // runs for the first time


        int w = framebufferObject()->width();
        int h = framebufferObject()->height();
        GLuint defaultFbo = framebufferObject()->handle();

        QMatrix4x4 proj;
        float aspect = (h > 0) ? (float)w / (float)h : 1.0f;
        proj.perspective(45.0f, aspect, 0.1f, 100.0f);

        // Flip Y so it matches Qt Quick
        // proj.scale(1.0f, -1.0f, 1.0f);

        m_view->SetProjection(proj);
        m_view->SetWidth(w);
        m_view->SetHeight(h);
        m_view->SetDefaultFBO(defaultFbo);

        projectionMatrixInitialized = true;
    }


    if (m_pickRequested)
    {
        // m_view->SetSelectionCoordinates(m_pickX, m_pickY);

        m_view->Selection();
        // m_view->UpdateGeometry();
        m_pickRequested = false;
    }
    m_view->Render();

    // // all variables are added here
    // // --- Model matrix (triangle local transform) ---
    // QMatrix4x4 model;
    // model.setToIdentity();
    // // model.rotate(45.0f, 0.0f, 0.0f, 1.0f);   // rotate around Z
    // // model.scale(0.8f);                        // shrink slightly

    // // --- View matrix (camera transform) ---
    // QMatrix4x4 view;
    // view.setToIdentity();
    // view.lookAt(
    //     m_cameraPos,              // camera position
    //     QVector3D(0.0f, 0.0f, 0.0f), // target at origin
    //     QVector3D(0.0f, 1.0f, 0.0f)  // up vector
    //     );

    // // --- Projection matrix ---
    // QMatrix4x4 proj;
    // float aspect = (h > 0) ? (float)w / (float)h : 1.0f;
    // proj.perspective(45.0f, aspect, 0.1f, 100.0f);

    // // float orthoHeight = 2.0f;                     // world units in Y
    // // float orthoWidth  = orthoHeight * aspect;     // scale X by aspect

    // // proj.ortho(-orthoWidth / 2.0f, orthoWidth / 2.0f,   // left, right
    // //            -orthoHeight / 2.0f, orthoHeight / 2.0f, // bottom, top
    // //            -100.0f, 100.0f);                        // near, far

    // // Flip Y so it matches Qt Quick
    // proj.scale(1.0f, -1.0f, 1.0f);


    // // Ray-Casting for change in coordinates ------------------------
    // if (m_pickRequested) {
    //     moveTopVertexToClick(m_pickX, m_pickY, proj, view, model);
    // }



    // // First Pass for Picking ----------------------------------------
    // // ensure pick FBO exists and matches size
    // if (m_pickRequested) {
    //     ensurePickFBO(w, h);
    // }

    // if (m_pickRequested && m_pickFBO)
    // {
    //     // Bind picking framebuffer
    //     this->glBindFramebuffer(GL_FRAMEBUFFER, m_pickFBO);
    //         this->glViewport(0, 0, w, h);
    //         this->glClearColor(0,0,0,0);
    //         this->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //         // draw scene with flat pick colors
    //         this->glUseProgram(m_pickProgram);

    //         unsigned int objectId = 1;
    //         unsigned char r,g,b;
    //         encodeIdToColor(objectId, r,g,b);
    //         this->glUniform4f(m_pickColorLoc, r/255.0f, g/255.0f, b/255.0f, 1.0f);

    //         // --- Upload to shader ---
    //         this->glUniformMatrix4fv(m_pickModelLoc, 1, GL_FALSE, model.constData());
    //         this->glUniformMatrix4fv(m_pickViewLoc,  1, GL_FALSE, view.constData());
    //         this->glUniformMatrix4fv(m_pickProjLoc,  1, GL_FALSE, proj.constData());

    //         this->glBindVertexArray(m_vao);
    //             this->glDrawArrays(GL_TRIANGLES, 0, 3);
    //         this->glBindVertexArray(0);

    //         // Read pixel
    //         int readX = m_pickX;
    //         int readY = (h - 1) - m_pickY;
    //         unsigned char pixel[4];
    //         this->glReadPixels(readX, readY, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, pixel);

    //         unsigned int pickedId = pixel[0] | (pixel[1] << 8) | (pixel[2] << 16);

    //     // Restore default framebuffer
    //     GLuint defaultFbo = framebufferObject()->handle();
    //     this->glBindFramebuffer(GL_FRAMEBUFFER, defaultFbo);

    //     // reset flag
    //     m_pickRequested = false;
    //     qInfo() << "Picked Id: " << pickedId;
    // }





    // // Actual Rendering -------------------------------------------
    // this->glViewport(0, 0, w, h);
    // this->glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
    // this->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // this->glUseProgram(m_shaderProgram);

    // // --- Upload to shader ---
    // this->glUniformMatrix4fv(m_modelLoc, 1, GL_FALSE, model.constData());
    // this->glUniformMatrix4fv(m_viewLoc,  1, GL_FALSE, view.constData());
    // this->glUniformMatrix4fv(m_projLoc,  1, GL_FALSE, proj.constData());


    // // Draw
    // this->glBindVertexArray(m_vao);
    //     this->glDrawArrays(GL_TRIANGLES, 0, 3);
    // this->glBindVertexArray(0);

    // //update(); // keep continuous rendering
}

QOpenGLFramebufferObject* MyGLRenderer::createFramebufferObject(const QSize &size) {
    QOpenGLFramebufferObjectFormat format;
    format.setAttachment(QOpenGLFramebufferObject::Depth);  //Request depth buffer

    return new QOpenGLFramebufferObject(size, format);
}


MyGLItem::MyGLItem(QQuickItem *parent)
    : QQuickFramebufferObject(parent)
{
    BIMElement* bimElement = new BIMElement(1,"1",false,"Wall", "Front Wall", 0, 0, this);
    BIMParameter* widthParameter = new BIMParameter(1,"1",false,"Width","1",1,this);
    BIMParameter* heightParameter = new BIMParameter(37, "1", false, "Height", "4", 1, this);
    BIMParameter* rlParameter = new BIMParameter(1,"1",false,"ReferenceLine","[[0,0], [0,4], [4,4]]",1,this);
    bimElement->addParameter(widthParameter);
    bimElement->addParameter(heightParameter);
    bimElement->addParameter(rlParameter);

    bimElementList.append(bimElement);

    BIMElement* bimElementNew = new BIMElement(2,"1",false,"Wall", "Front Wall", 0, 0, this);
    BIMParameter* widthParameterNew = new BIMParameter(1,"1",false,"Width","1",2,this);
    BIMParameter* heightParameterNew = new BIMParameter(37, "1", false, "Height", "4", 2, this);
    BIMParameter* rlParameterNew = new BIMParameter(1,"1",false,"ReferenceLine","[[0,0], [4,0], [4,4]]",2,this);
    bimElementNew->addParameter(widthParameterNew);
    bimElementNew->addParameter(heightParameterNew);
    bimElementNew->addParameter(rlParameterNew);

    bimElementList.append(bimElementNew);

    BIMElement* bimElementDoor = new BIMElement(3,"1",false,"Door", "Front Door", 0, 1, this);
    BIMParameter* distanceParameterDoor = new BIMParameter(1,"1",false,"Distance","1",3,this);
    BIMParameter* heightParameterDoor = new BIMParameter(37, "1", false, "Height", "4", 3, this);
    BIMParameter* widthParameterDoor = new BIMParameter(1,"1",false,"Width","2",3,this);
    BIMParameter* rlParameterDoor = new BIMParameter(1,"1",false,"ReferenceLine","[[0,0], [4,0]]",3,this);
    bimElementDoor->addParameter(distanceParameterDoor);
    bimElementDoor->addParameter(heightParameterDoor);
    bimElementDoor->addParameter(rlParameterDoor);
    bimElementDoor->addParameter(widthParameterDoor);

    bimElementList.append(bimElementDoor);

    bimElementNew->addHostedElement(bimElementDoor);

    pIfcDetailController = new IFCDetailController(this);
    pIfcGeometryService = new IfcGeometryService(this);
    // mesh = getMeshptr();

   /* if(m_currentItem == "Beam")
    {
        BIMElement* newElement = new BIMElement(1, "1", false, "Beam", "Front Beam", 0, this);
        BIMParameter* widthParameter = new BIMParameter(1, "1", false, "Width", "1.5", 22, this);
        BIMParameter* heightParameter = new BIMParameter(37, "1", false, "Height", "5", 15, this);
        BIMParameter* rlParameter = new BIMParameter(1, "1", false, "ReferenceLine","[[0,0], [0,2]]", 22, this);
        newElement->addParameter(widthParameter);
        newElement->addParameter(heightParameter);
        newElement->addParameter(rlParameter);

        mesh = new Mesh(this);
        BeamGeometryService* service = new BeamGeometryService(this);
        service->generateMesh3D(newElement, mesh);
    }

   else if(m_currentItem == "Column")
    {
        BIMElement* newElement = new BIMElement(23,"1",false,"Column", "Front Column", 0, this);
        BIMParameter* widthParameter = new BIMParameter(62,"1",false,"Width","3",23,this);
        BIMParameter* heightParameter = new BIMParameter(37, "1", false, "Height", "0.05", 15, this);
        BIMParameter* rlParameter = new BIMParameter(63,"1",false,"ReferenceLine","[[0,0], [0,4]]",23,this);
        newElement->addParameter(widthParameter);
        newElement->addParameter(heightParameter);
        newElement->addParameter(rlParameter);

        mesh = new Mesh(this);
        ColumnGeometryService* service = new ColumnGeometryService(this);
        service->generateMesh3D(newElement, mesh);
    }
    else if(m_currentItem == "Slab")
    {
    BIMElement* bimElement = new BIMElement(1,"1",false,"Wall", "Front Wall", 0, this);
    BIMParameter* widthParameter = new BIMParameter(1,"1",false,"Width","1",1,this);
    BIMParameter* rlParameter = new BIMParameter(1,"1",false,"ReferenceLine","[[0,0], [0,4], [4,4]]",1,this);
    bimElement->addParameter(widthParameter);
    bimElement->addParameter(rlParameter);

    bimElementList.append(bimElement);

    BIMElement* bimElementNew = new BIMElement(1,"1",false,"Wall", "Front Wall", 0, this);
    BIMParameter* widthParameterNew = new BIMParameter(1,"1",false,"Width","1",1,this);
    BIMParameter* rlParameterNew = new BIMParameter(1,"1",false,"ReferenceLine","[[0,0], [4,0], [4,4]]",1,this);
    bimElementNew->addParameter(widthParameterNew);
    bimElementNew->addParameter(rlParameterNew);

    bimElementList.append(bimElementNew);

        BIMElement* newElement = new BIMElement(15, "1", false, "Slab", "Front Slab", 0, this);
        BIMParameter* distFromLevelParameter = new BIMParameter(38, "1", false, "Distance", "5", 15, this);
        BIMParameter* heightParameter = new BIMParameter(37, "1", false, "Height", "1", 15, this);
        //Regular Octagonal Slab
        BIMParameter* rlParameter = new BIMParameter(39, "1", false, "ReferenceLine", "[[5.0, 0.0], [3.54, 3.54], [0.0, 5.0], [-3.54, 3.54], [-5.0, 0.0], [-3.54, -3.54], [0.0, -5.0], [3.54, -3.54] ]", 15, this);
        newElement->addParameter(distFromLevelParameter);
        newElement->addParameter(heightParameter);
        newElement->addParameter(rlParameter);

        mesh = new Mesh(this);
        SlabGeometryService* service = new SlabGeometryService(this);
        service->generateMesh3D(newElement, mesh);
    }


    else if(m_currentItem == "Wall")
    {
        // BIMElementController* bimElementController = new BIMElementController(this);

        BIMElement* newElement = new BIMElement(1,"1",false,"Wall", "Front Wall", 0, this);
        BIMParameter* widthParameter = new BIMParameter(1,"1",false,"Width","1",1,this);
        BIMParameter* heightParameter = new BIMParameter(37, "1", false, "Height", "4", 15, this);
        BIMParameter* rlParameter = new BIMParameter(1,"1",false,"ReferenceLine","[[0,0], [0,4], [4,4]]",1,this);
        newElement->addParameter(widthParameter);
        newElement->addParameter(heightParameter);
        newElement->addParameter(rlParameter);
    // mesh = new Mesh(this);
    // WallGeometryService service = WallGeometryService();
    // service.generateMesh3D(newElement, mesh);

        // bimElementController->create("Wall", "Front Wall", 0);

        // bimElementController->addParameter(newElement, "Height", "3000");
        // bimElementController->addParameter(newElement, "Width", "100");
        // bimElementController->addParameter(newElement, "ReferenceLine", "");

        // qInfo() << "BIM Element with Params is null: " << (newElement == nullptr);

        mesh = new Mesh(this);
        WallGeometryService* service = new WallGeometryService(this);
        service->generateMesh3D(newElement, mesh);

        // GLfloat* vertices = mesh->getVerticies();
        // unsigned int* indices = mesh->getIndices();
        // for (int i = 0; i < 6; i++)
        // {
        //     qInfo() << vertices[6*i] << " , " << vertices[6*i + 1] << " , " << vertices[6*i + 2] << " , " << vertices[6*i + 3] << " , " << vertices[6*i + 4] << " , " << vertices[6*i + 5];
        // }

        // qInfo() << "----------------------------------";

        // for (int i = 0; i < 6; i++)
        // {
        //     qInfo() << indices[i];
        // }
   }*/

    //else{

   //}

}


QQuickFramebufferObject::Renderer* MyGLItem::createRenderer() const {
    // qInfo() << "Create Renderer";
    return new MyGLRenderer();
}

void MyGLItem::setCurrentItem(QString currentSelctedItem){
    m_currentItem = currentSelctedItem;
}

Mesh* MyGLItem::getMeshptr()
{
    if(mesh == nullptr)
    {
         mesh = new Mesh(this);
    }
    return mesh;
}

void MyGLItem::cameraMoveUp() {
    m_moveUp = true;
    update();
}

void MyGLItem::cameraMoveDown() {
    m_moveDown = true;
    update();
}

void MyGLItem::cameraMoveLeft() {
    m_moveLeft = true;
    update();
}

void MyGLItem::cameraMoveRight() {
    m_moveRight = true;
    update();
}

void MyGLItem::cameraPanUp() {
    m_panUp = true;
    update();
}

void MyGLItem::cameraPanDown() {
    m_panDown = true;
    update();
}

void MyGLItem::cameraPanLeft() {
    m_panLeft = true;
    update();
}

void MyGLItem::cameraPanRight() {
    m_panRight = true;
    update();
}


void MyGLItem::zoomIn()
{
    m_zoomIn = true;
    update();
}

void MyGLItem::zoomOut()
{
    m_zoomOut = true;
    update();
}

void MyGLItem::updateView(QString viewType)
{
    m_viewType = viewType;
    update();
}

void MyGLItem::requestPick(int x, int y) {
    m_lastClickX = x;
    m_lastClickY = y;
    update();
}

void MyGLItem::handlePick(int id) {
    emit selectionChanged(id);
}

void MyGLItem::viewIfc()
{
    QString strFilePath = "C:\\Users\\RIPL\\Downloads\\DblDoor-1-Panel.ifc";
    //QString strFilePath = "C:\\Users\\RIPL\\Documents\\FusionHubData\\DblDoor-4-Panel.ifc";
    //QString strFilePath = "C:\\Users\\RIPL\\Documents\\FusionHubData\\BasicHouse.ifc";
    //QString strFilePath = "C:\\Users\\RIPL\\Documents\\FusionHubData\\DblDoor-2-Panel.ifc";
    //QString strFilePath = "C:\\Users\\RIPL\\Documents\\FusionHubData\\DblDoor-Flush.ifc";
    //QString strFilePath = "C:\\Users\\RIPL\\Documents\\FusionHubData\\Door-Entry_2-Panel-Glz-Arc-Top.ifc";
    //QString strFilePath = "C:\\Users\\RIPL\\Documents\\FusionHubData\\Taylor_Entrance_1_Panel_High_Def.ifc";
    //QString strFilePath = "C:\\Users\\RIPL\\Documents\\FusionHubData\\Skylight_GTVSF_AmericanSkylites.ifc";

    //QString strFilePath = pIfcDetailController->getIfcFilePath();
    OdIfcFilePtr pDatabase = pIfcDetailController->getIfcFilePtrFromLoadedIFC(strFilePath);
    if(pDatabase)
    {
        // delete mesh;
        // mesh = NULL;
        // mesh = new Mesh(this);
        pIfcGeometryService->generateMesh3D(pDatabase, nullptr);
        qInfo() << "File has been loaded";
        pDatabase.release();
        pDatabase = NULL;
    }
    // update();
}

void MyGLItem::updateEditableBimElement(QVariant bimElement)
{
    editableBimElement = bimElement.value<BIMElement*>();
}

void MyGLItem::saveEditableBimElement()
{
    bimElementList.append(editableBimElement);

    // bimElement is created in the controller and its lifecycle is handled by the controller
    editableBimElement = nullptr;
    update();
}
