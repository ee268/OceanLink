#ifndef BASEPAGE_H
#define BASEPAGE_H

#include <QWidget>
#include <QLayout>
#include <QStackedWidget>

class BasePage : public QWidget
{
public:
    explicit BasePage(QWidget* parent = nullptr);
    ~BasePage();

    QWidget* getLeftWidget() const;
    QStackedWidget* getStackedWidget() const;

    void switchToPlaceHolderPage();

protected:
    void initDefaultLayout();

private:
    QWidget* _leftWid;
    QStackedWidget* _stackedWidget;
};

#endif // BASEPAGE_H
