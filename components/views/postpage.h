#ifndef POSTPAGE_H
#define POSTPAGE_H

#include "basepage.h"

#include <QPushButton>

#include "../controls/spacelist.h"

class EveryoneButton: public QPushButton
{
public:
    explicit EveryoneButton(const QString &text, const QIcon& icon, QWidget *parent = nullptr);

    void setFixedSize(int w, int h);

protected:
    void paintEvent(QPaintEvent* event) override;
    bool eventFilter(QObject* obj, QEvent* event) override;

private:
    enum State { Normal, Hover, Pressed };
    State _state = Normal;

    QString _text;
    QIcon _icon;
};

class PostPage : public BasePage
{
    Q_OBJECT
public:
    explicit PostPage(QWidget* parent = nullptr);
    ~PostPage() = default;

private:
    void initLeftWidget();

private:
    SpaceList* _spaceList;
};

#endif // POSTPAGE_H
