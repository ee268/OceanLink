#ifndef THEMECOLORBUTTON_H
#define THEMECOLORBUTTON_H

#include "ElaPushButton.h"

class ThemeColorButton : public ElaPushButton
{
    Q_OBJECT
public:
    explicit ThemeColorButton(QWidget* parent = nullptr);
    explicit ThemeColorButton(const QString& text, QWidget* parent = nullptr);
    ~ThemeColorButton() = default;

    void recoverDefault();

    void changeToBasicColor();

private:
    void initButton();
};

#endif // THEMECOLORBUTTON_H
