#include "texture.h"

Texture::Texture(QObject *parent)
    : QObject{parent}
{}

void Texture::LoadTextures()
{
    // initialize opengl
    this->initializeOpenGLFunctions();

    this->glGenTextures(1, &m_textureId);
    this->glBindTexture(GL_TEXTURE_2D_ARRAY, m_textureId);


    /*
    For bricks.jpg:
    Height:  640 px
    Width:  960 px

    All textures should have this dimensions only for sampler2DArray to work
    */

    // Allocate storage for the layers of 2D array
    this->glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RGBA8, 960, 640, texturePathList.size(), 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

    int texturePathIndex = -1;
    for (QString texturePath: texturePathList)
    {
        texturePathIndex++;

        // Load the image
        QImage image;
        if (!image.load(texturePath)) {
            qDebug() << "Failed to load texture:" << texturePath;
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
        this->glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0,
                     texturePathIndex, glImage.width(), glImage.height(),
                     1, GL_RGBA, GL_UNSIGNED_BYTE, glImage.bits());
    }

    this->glGenerateMipmap(GL_TEXTURE_2D_ARRAY);

    // Texture parameters (wrap + filter)
    this->glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT);
    this->glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT);
    this->glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    this->glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

GLuint Texture::getTextureId()
{
    return m_textureId;
}
