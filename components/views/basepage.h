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
    QWidget* getRightWidget() const;
    QStackedWidget* getStackedWidget() const;

    void switchToPlaceHolderPage();

    void setCurrentIndex(int index);

protected:
    void initDefaultLayout();

    void resizeEvent(QResizeEvent *event) override;

private:
    QWidget* _leftWid;
    QWidget* _rightWid;
    QStackedWidget* _stackedWidget;
};

#endif // BASEPAGE_H
