#ifndef ICONBUTTON_H
#define ICONBUTTON_H

#include <QPushButton>

class IconButton : public QPushButton
{
    Q_OBJECT
public:
    explicit IconButton(QWidget* parent = nullptr);
    explicit IconButton(QIcon icon, QWidget* parent = nullptr);
    ~IconButton() = default;

    void setBorderRadius(int borderRadius);
    void setIconSize(int size);
    void setIcon(const QIcon& icon);
    void setBorderWidth(int width);
    void setTransparentBackground(bool enable);

protected:
    bool eventFilter(QObject* obj, QEvent* event) override;

    void paintEvent(QPaintEvent *) override;

private:
    enum ButtonState {
        Normal,
        Hover,
        Pressed
    };

    int _borderRadius;
    QIcon _icon;
    int _iconSize;
    ButtonState _state;
    int _borderWidth;
    bool _enableTransparent;
};

#endif // ICONBUTTON_H
