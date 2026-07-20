#ifndef ICONTEXT_H
#define ICONTEXT_H

#include <QLabel>
#include <QIcon>
#include "ElaTheme.h"

class IconText : public QLabel
{
    Q_OBJECT
public:
    explicit IconText(QWidget* parent = nullptr);
    explicit IconText(const QString& text, QWidget* parent = nullptr);
    ~IconText();

    void setPixelSize(int size);
    void setTextColor(QColor color);
    void setTextBold(bool enable);

    void setIcon(const QIcon& icon);
    void setIconThemeColor(bool enable);

    void setSpacing(int spacing);

    void setTextColorDark(const QColor& color);
    void setTextColorLight(const QColor& color);    

    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;

protected:
    void paintEvent(QPaintEvent *) override;
    void mousePressEvent(QMouseEvent* event) override;

signals:
    void sigClicked();

private:
    void initText();

private slots:
    void slotChangedTheme(ElaThemeType::ThemeMode mode);

private:
    QIcon _icon;
    int _spacing;
    QColor _textColorLight;
    QColor _textColorDark;
    QColor _textColor;
    bool _b_iconThemeColor;
};

#endif // ICONTEXT_H
