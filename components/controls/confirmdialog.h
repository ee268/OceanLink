#ifndef CONFIRMDIALOG_H
#define CONFIRMDIALOG_H

#include "ElaContentDialog.h"
#include "ElaCheckBox.h"
#include "ElaText.h"

class ConfirmDialog : public ElaContentDialog
{
    Q_OBJECT
public:
    explicit ConfirmDialog(QWidget* parent = nullptr);

    Qt::CheckState getAskCheckState() const;

    void setTitleText(const QString& text);

    void setSubTitleText(const QString& text);

    void setCheckBoxHidden(bool isHidden);

    void setMiddleButtonHidden(bool isHidden);

protected:
    void setVisible(bool visible) override;

private:
    void initDialog();
    void initContent();

private:
    QWidget* _centralWid;
    ElaCheckBox* _isAlwaysAsk;
    ElaText* _title;
    ElaText* _subTitle;
};

#endif // CONFIRMDIALOG_H
