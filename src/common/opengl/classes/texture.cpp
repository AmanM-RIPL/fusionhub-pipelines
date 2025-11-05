#include "texture.h"

Texture::Texture(QObject *parent)
    : QObject{parent}
{}

void Texture::LoadTexture(const QString& path)
{
    // initialize opengl
    this->initializeOpenGLFunctions();

    this->glGenTextures(1, &m_textureId);
    this->glBindTexture(GL_TEXTURE_2D, m_textureId);

    // Load the image
    QImage image;
    if (!image.load(path)) {
        qDebug() << "Failed to load texture:" << path;
        // return 0;
    }

    // Convert to format OpenGL likes (RGBA, origin bottom-left flipped)
    QImage glImage = QImage(image.width(), image.height(),
                            QImage::Format_RGBA8888);

    glImage.fill(Qt::transparent);
    QPainter painter(&glImage);
    painter.drawImage(0, 0, image);
    painter.end();

    glImage = glImage.mirrored(); // Flip vertical

    // Upload to OpenGL
    this->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                 glImage.width(), glImage.height(),
                 0, GL_RGBA, GL_UNSIGNED_BYTE, glImage.bits());

    this->glGenerateMipmap(GL_TEXTURE_2D);

    // Texture parameters (wrap + filter)
    this->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    this->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    this->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    this->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

GLuint Texture::getTextureId()
{
    return m_textureId;
}
