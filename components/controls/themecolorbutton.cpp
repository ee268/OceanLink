#include "themecolorbutton.h"

ThemeColorButton::ThemeColorButton(QWidget *parent)
    : ElaPushButton(parent)
{
    initButton();
}

ThemeColorButton::ThemeColorButton(const QString &text, QWidget *parent)
    : ElaPushButton(text, parent)
{
    initButton();
}

void ThemeColorButton::initButton()
{
    this->setLightDefaultColor(QColor("#1a6bf8"));
    this->setLightHoverColor(QColor("#4d8efb"));
    this->setLightPressColor(QColor("#0d5ce0"));

    this->setDarkDefaultColor(QColor("#1a6bf8"));
    this->setDarkHoverColor(QColor("#4d8efb"));
    this->setDarkPressColor(QColor("#0d5ce0"));

    this->setLightTextColor(Qt::white);
    this->setDarkTextColor(Qt::white);
}
