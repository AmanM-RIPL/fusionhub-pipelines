#include "view.h"

View::View(QObject *parent)
    : QObject{parent}
{
}

void View::Initialize()
{
    this->initializeOpenGLFunctions();

    // FOR TESTING:::: ADD MESH BEFORE HAND
    // getting the data of the first mesh
    Vertex* vertices = meshList[0]->getVerticies();
    unsigned int* indices = meshList[0]->getIndices();
    unsigned int* borderIndices = meshList[0]->getBorderIndices();
    unsigned int numOfVertices = meshList[0]->getNumOfVertices();
    unsigned int numOfIndices = meshList[0]->getNumOfIndices();
    unsigned int numOfBorderIndices = meshList[0]->getNumOfBorderIndices();

    // Initializing the vao, vbo, and ibo
    m_indexCount = numOfIndices;
    m_borderIndexCount = numOfBorderIndices;
    this->glEnable(GL_DEPTH_TEST);

    this->glGenVertexArrays(1, &m_vao);
    this->glGenBuffers(1, &m_static_ibo);
    this->glGenBuffers(1, &m_static_vbo);
    this->glGenBuffers(1, &m_static_border_ibo);

    // VAO
    this->glBindVertexArray(m_vao);
        //VBO
        this->glBindBuffer(GL_ARRAY_BUFFER, m_static_vbo);
            this->glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * numOfVertices, vertices, GL_STATIC_DRAW);

            // postition in verticies
            this->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
            this->glEnableVertexAttribArray(0);

            // normal in verticies
            this->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
            this->glEnableVertexAttribArray(1);

            //materialIndex in verticies
            this->glVertexAttribIPointer(2, 1, GL_INT, sizeof(Vertex), (void*)offsetof(Vertex, materialIndex));
            this->glEnableVertexAttribArray(2);

            // this->glBindBuffer(GL_ARRAY_BUFFER, 0);
        // this->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


        //TRIANGLE IBO
        this->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_static_ibo);
            this->glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * numOfIndices, indices, GL_STATIC_DRAW);

        //LINE IBO
        this->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_static_border_ibo);
            this->glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(borderIndices[0]) * numOfBorderIndices, borderIndices, GL_STATIC_DRAW);
    this->glBindVertexArray(0);
}

void View::Render()
{
    this->glEnable(GL_DEPTH_TEST);
    this->glDepthFunc(GL_LEQUAL);
    this->glDepthMask(GL_TRUE);
    this->glDisable(GL_BLEND);

    this->glEnable(GL_CULL_FACE);
    this->glCullFace(GL_BACK);
    this->glFrontFace(GL_CW); // GL_CW or GL_CCW ??? GL_CW seems to work because we flipped Y in the vertex shader

    this->glViewport(0, 0, viewportWidth, viewportHeight);
    this->glClearColor(0.97f, 0.99f, 0.98f, 1.0f);
    this->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // draw scene with the required colors
    this->glUseProgram(shader->getShaderId());

    QVector3D cameraPosition = camera->getCameraPosition();
    this->glUniform3f(shader->getViewPositionId(), cameraPosition.x(), cameraPosition.y(), cameraPosition.z());

    // For 3D
    for (int i = 0; i < materialList.size(); i++)
    {
        OpenGLMaterial* material = materialList[i];

        auto ambient = material->ambient();
        this->glUniform3f(shader->getMaterialAmbientId(i), ambient[0], ambient[1], ambient[2]); // 0.96, 0.47f, 0.02f

        auto diffuse = material->diffuse();
        this->glUniform3f(shader->getMaterialDiffuseId(i), diffuse[0], diffuse[1], diffuse[2]); // 0.0f, 0.5f, 0.31f

        auto specular = material->specular();
        this->glUniform3f(shader->getMaterialSpecularId(i), specular[0], specular[1], specular[2]); //0.5f, 0.5f, 0.5f

        this->glUniform1f(shader->getMaterialShininessId(i), material->shininess());
    }

    this->glUniform3f(shader->getLightPositionId(), 20.0f, 0.0f, 0.0f);
    this->glUniform3f(shader->getLightAmbientId(), 0.2f, 0.2f, 0.2f);
    this->glUniform3f(shader->getLightDiffuseId(), 0.5f, 0.5f, 0.5f);
    this->glUniform3f(shader->getLightSpecularId(), 1.0f, 1.0f, 1.0f);

    // For 2D
    // this->glUniform3f(shader->getMaterialAmbientId(), 0.96f, 0.47f, 0.02f);
    // this->glUniform3f(shader->getMaterialDiffuseId(), 0.0f, 0.0f, 0.0f);
    // this->glUniform3f(shader->getMaterialSpecularId(), 0.0f, 0.0f, 0.0f);
    // this->glUniform1f(shader->getMaterialShininessId(), 32.0f);

    // this->glUniform3f(shader->getLightPositionId(), 0.0f, 0.0f, 2.0f);
    // this->glUniform3f(shader->getLightAmbientId(), 1.0f, 1.0f, 1.0f);
    // this->glUniform3f(shader->getLightDiffuseId(), 0.5f, 0.5f, 0.5f);
    // this->glUniform3f(shader->getLightSpecularId(), 1.0f, 1.0f, 1.0f);

    // --- Upload to shader ---
    this->glUniformMatrix4fv(shader->getModelId(), 1, GL_FALSE, meshList[0]->getModelMatrix().constData());
    this->glUniformMatrix4fv(shader->getViewId(),  1, GL_FALSE, camera->calculateViewMatrix().constData());
    this->glUniformMatrix4fv(shader->getProjectionId(),  1, GL_FALSE, m_projectionMatrix.constData());

    // GLint depthTestEnabled = 1;
    // this->glGetIntegerv(GL_DEPTH_TEST, &depthTestEnabled);
    // qInfo() << "Depth test:" << (depthTestEnabled ? "ON" : "OFF");
    // this->glGetIntegerv(GL_DEPTH_BITS, &depthTestEnabled);
    // qInfo() << "Depth bits:" << depthTestEnabled;

    // GLint depthBits = 0;

    // // Use glGetFramebufferAttachmentParameteriv to query the specific attachment
    // // for the currently bound FBO.
    // // The attachment point for a renderbuffer (which Qt uses by default for depth) is GL_DEPTH_ATTACHMENT
    // this->glGetFramebufferAttachmentParameteriv(
    //     GL_FRAMEBUFFER,
    //     GL_DEPTH_ATTACHMENT,
    //     GL_FRAMEBUFFER_ATTACHMENT_DEPTH_SIZE,
    //     &depthBits
    //     );

    // qInfo() << "FBO Depth bits:" << depthBits;



    // Draw
    this->glBindVertexArray(m_vao);
        this->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_static_ibo);
            this->glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_INT, 0);
        // this->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        this->glUniform3f(shader->getMaterialAmbientId(0), 0.0f, 0.0f, 0.0f);

        this->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_static_border_ibo);
            this->glDrawElements(GL_LINES, m_borderIndexCount, GL_UNSIGNED_INT, 0);
    this->glBindVertexArray(0);
}

void View::Selection()
{
    // ensure pick FBO exists and matches size
    ensurePickFBO();

    if (m_pickFBO)
    {
        // Bind picking framebuffer
        this->glBindFramebuffer(GL_FRAMEBUFFER, m_pickFBO);
            this->glViewport(0, 0, viewportWidth, viewportHeight);
            this->glClearColor(0,0,0,0);
            this->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            // draw scene with flat pick colors
            this->glUseProgram(pickingShader->getShaderId());

            unsigned int objectId = 1;
            unsigned char r,g,b;
            encodeIdToColor(objectId, r,g,b);
            this->glUniform4f(pickingShader->getPickColorId(), r/255.0f, g/255.0f, b/255.0f, 1.0f);

            // --- Upload to shader ---
            this->glUniformMatrix4fv(pickingShader->getModelId(), 1, GL_FALSE, meshList[0]->getModelMatrix().constData());
            this->glUniformMatrix4fv(pickingShader->getViewId(),  1, GL_FALSE, camera->calculateViewMatrix().constData());
            this->glUniformMatrix4fv(pickingShader->getProjectionId(),  1, GL_FALSE, m_projectionMatrix.constData());

            this->glBindVertexArray(m_vao);
                // this->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_static_ibo);
                    this->glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_INT, 0);
                // this->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
            this->glBindVertexArray(0);

            // Read pixel
            int readX = m_pickX;
            int readY = m_pickY; //(viewportHeight - 1) - m_pickY;
            unsigned char pixel[4];
            this->glReadPixels(readX, readY, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, pixel);

            unsigned int pickedId = pixel[0] | (pixel[1] << 8) | (pixel[2] << 16);

        // Restore default framebuffer
        this->glBindFramebuffer(GL_FRAMEBUFFER, defaultFBO);

        qInfo() << "Picked Id: " << pickedId;
    }
}

void View::UpdateGeometry()
{
    // --- Step 1: Screen -> NDC
    float x = (2.0f * m_pickX) / float(viewportWidth) - 1.0f;
    float y = (2.0f * m_pickY) / float(viewportHeight) - 1.0f; // no inversion as Y-axis flip is already in proj matrix
    QVector4D rayClip(x, y, -1.0f, 1.0f);

    // --- Step 2: NDC -> Eye space
    QVector4D rayEye = m_projectionMatrix.inverted() * rayClip;
    rayEye = QVector4D(rayEye.x(), rayEye.y(), -1.0f, 0.0f);

    // --- Step 3: Eye -> World space
    QVector3D rayDirWorld = (camera->calculateViewMatrix().inverted() * rayEye).toVector3D().normalized();
    QVector3D rayOriginWorld = camera->getCameraPosition();

    // --- Step 4: Transform ray into *model space*
    QMatrix4x4 invModel = meshList[0]->getModelMatrix().inverted();
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
    meshList[0]->UpdateGeometry(hitPoint);

    // --- Step 6: Push updated vertices to GPU
    this->glBindVertexArray(m_vao);
        //IBO
        // this->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_static_ibo);
            this->glBindBuffer(GL_ARRAY_BUFFER, m_static_vbo);
                this->glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(meshList[0]->getVerticies()), meshList[0]->getVerticies());
            this->glBindBuffer(GL_ARRAY_BUFFER, 0);
        // this->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    this->glBindVertexArray(0);
}

void View::AddMesh(Mesh *mesh)
{
    meshList.append(mesh);
}

void View::AddMaterial(OpenGLMaterial *material)
{
    materialList.append(material);
}

void View::AddCamera(Camera *cam)
{
    camera = cam;
}

void View::AddShader(Shader *shad)
{
    shader = shad;
}

void View::AddPickingShader(Shader *shad)
{
    pickingShader = shad;
}

void View::SetProjection(QMatrix4x4 &projection)
{
    m_projectionMatrix = projection;
}

void View::SetWidth(int width)
{
    viewportWidth = width;
}

void View::SetHeight(int height)
{
    viewportHeight = height;
}

void View::SetDefaultFBO(GLuint fbo)
{
    defaultFBO = fbo;
}

void View::SetSelectionCoordinates(int x, int y)
{
    m_pickX = x;
    m_pickY = y;
}

void View::encodeIdToColor(unsigned int id, unsigned char &r, unsigned char &g, unsigned char &b)
{
    r = (id & 0x000000FF);
    g = (id & 0x0000FF00) >> 8;
    b = (id & 0x00FF0000) >> 16;
}

void View::ensurePickFBO()
{
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
    this->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, viewportWidth, viewportHeight, 0,
                       GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    this->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    this->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    this->glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                                 GL_TEXTURE_2D, m_pickColorTex, 0);

    // --- Depth attachment (renderbuffer) ---
    this->glGenRenderbuffers(1, &m_pickDepthBuf);
    this->glBindRenderbuffer(GL_RENDERBUFFER, m_pickDepthBuf);
    this->glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, viewportWidth, viewportHeight);
    this->glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                                    GL_RENDERBUFFER, m_pickDepthBuf);

    // Check FBO status
    if (this->glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        qWarning() << "Picking FBO is not complete!";
    }

    // Unbind
    this->glBindFramebuffer(GL_FRAMEBUFFER, defaultFBO);
}

View::~View()
{
    this->glDeleteBuffers(1, &m_static_vbo);
    this->glDeleteBuffers(1, &m_static_ibo);
    this->glDeleteVertexArrays(1, &m_vao);
}
