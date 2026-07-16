#ifndef SPLITLINE_H
#define SPLITLINE_H

#include <QPushButton>

class SplitLine : public QPushButton
{
    Q_OBJECT
public:
    explicit SplitLine(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent* event) override;
};

#endif // SPLITLINE_H
