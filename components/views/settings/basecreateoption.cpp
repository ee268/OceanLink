#include "basecreateoption.h"

#include "../../controls/borderwidget.h"
#include "../../controls/themecolorbutton.h"

#include <QHBoxLayout>

#include "ElaToggleSwitch.h"
#include "ElaText.h"
#include "ElaPushButton.h"
#include "ElaComboBox.h"

BaseCreateOption::BaseCreateOption(QWidget *parent)
    : QWidget{parent}
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(30, 30, 30, 30);
    mainLayout->setSpacing(20);

    this->setLayout(mainLayout);
}

QVBoxLayout *BaseCreateOption::layout() const
{
    auto ly = dynamic_cast<QVBoxLayout*>(QWidget::layout());

    return ly;
}

QWidget *BaseCreateOption::createOption(const QString& text)
{
    BorderWidget* wid = new BorderWidget(this);
    ElaText* title = new ElaText(text, wid);
    ElaToggleSwitch* swichButton = new ElaToggleSwitch(wid);

    title->setTextPixelSize(15);

    wid->layout()->addWidget(title);
    wid->layout()->addStretch();
    wid->layout()->addWidget(swichButton);

    return wid;
}

QWidget *BaseCreateOption::createButtonOption(const QString &text, const QString& btnText, bool isThemeBtn)
{
    BorderWidget* wid = new BorderWidget(this);
    ElaText* title = new ElaText(text, wid);
    ElaPushButton* button;
    if (isThemeBtn) {
        button = new ThemeColorButton(btnText, wid);
    }
    else {
        button = new ElaPushButton(btnText, wid);
    }

    title->setTextPixelSize(15);

    wid->layout()->addWidget(title);
    wid->layout()->addStretch();
    wid->layout()->addWidget(button);

    return wid;
}

QWidget *BaseCreateOption::createComboBoxOption(const QString &text, const QStringList &options)
{
    BorderWidget* wid = new BorderWidget(this);
    ElaText* title = new ElaText(text, wid);
    ElaComboBox* comboBox = new ElaComboBox(this);

    title->setTextPixelSize(15);
    title->setIsWrapAnywhere(false);

    comboBox->addItems(options);

    wid->layout()->addWidget(title);
    wid->layout()->addStretch();
    wid->layout()->addWidget(comboBox);

    return wid;
}
