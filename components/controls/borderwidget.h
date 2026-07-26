#ifndef BORDERWIDGET_H
#define BORDERWIDGET_H

#include <QWidget>

class QHBoxLayout;

class BorderWidget : public QWidget
{
public:
    explicit BorderWidget(QWidget *parent = nullptr);

    void setBorderRadius(int radius);
    QHBoxLayout* layout() const;

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    int _borderRadius;
};

#endif // BORDERWIDGET_H
