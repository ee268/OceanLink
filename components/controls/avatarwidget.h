#ifndef AVATARWIDGET_H
#define AVATARWIDGET_H

#include <QWidget>

class AvatarWidget : public QWidget
{
public:
    explicit AvatarWidget(QWidget* parent = nullptr);

    void setAvatar(const QPixmap& pixmap);
    void setName(const QString& name);
    void setPixeSize(int size);

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    QPixmap _avatar;
    QString _name;
    int _namePixelSize;
};

#endif // AVATARWIDGET_H
