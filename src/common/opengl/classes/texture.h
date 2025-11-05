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

    void LoadTexture(const QString& path);
    GLuint getTextureId();

private:
    GLuint m_textureId;
    QString m_resourcePath;

signals:
};

#endif // TEXTURE_H
