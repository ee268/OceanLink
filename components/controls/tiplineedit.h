#ifndef TIPLINEEDIT_H
#define TIPLINEEDIT_H

#include <QWidget>

#include "icontext.h"
#include "ElaLineEdit.h"

class TipLineEdit : public QWidget
{
    Q_OBJECT
public:
    explicit TipLineEdit(QWidget *parent = nullptr);
    ~TipLineEdit();

    void setTipText(const QString& text);
    void setPixelSize(int size);

    ElaLineEdit* getLineEdit() const;

    void setEchoModePassword();

private:
    void initContent();

private:
    IconText* _tipText;
    ElaLineEdit* _lineEdit;
};

#endif // TIPLINEEDIT_H
