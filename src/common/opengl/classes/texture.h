#ifndef TEXTURE_H
#define TEXTURE_H

#include <QObject>
#include <QOpenGLFunctions_3_3_Core>
#include <QImage>
#include <QPainter>

class Texture : public QObject, protected QOpenGLFunctions_3_3_Core
{
    Q_OBJECT
public:
    explicit Texture(QObject *parent = nullptr);

    enum TextureName
    {
        NONE = -1,
        BRICK = 0,
        WOOD = 1
    };

    void LoadTextures();
    GLuint getTextureId();

private:
    GLuint m_textureId;
    QString m_resourcePath;

    QList<QString> texturePathList = {
        "://resources/images/brick.jpg",
        "://resources/images/wood.jpg"
    };

signals:
};

#endif // TEXTURE_H
