#ifndef ABOUTPAGE_H
#define ABOUTPAGE_H

#include "ElaDialog.h"

#include <QIcon>

class AboutInfoWidget : public QWidget
{
public:
    explicit AboutInfoWidget(const QIcon& icon, const QString& text, QWidget* parent = nullptr);

    int getImplicitIconSize() const;

    void setTextColor(const QColor& color);

    void setTextBold(bool isBold);

protected:
    void paintEvent(QPaintEvent * event) override;
    QSize sizeHint() const override;

private:
    QIcon _icon;
    QString _text;
    QFont _textFont;
    QColor _iconBgColor;
    QColor _iconColor;
    int _iconSize;
    QColor _textColor;

    int _leftMargin;
    int _spacing;
    int _borderRadius;
};

class AboutPage : public ElaDialog
{
    Q_OBJECT
public:
    explicit AboutPage(QWidget* parent = nullptr);
    ~AboutPage();

private:
    void initDialog();
};

#endif // ABOUTPAGE_H
