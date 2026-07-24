#ifndef PUBLISHPOSTEDITDIALOG_H
#define PUBLISHPOSTEDITDIALOG_H

#include "ElaDialog.h"
#include "ElaPlainTextEdit.h"
#include "themecolorbutton.h"

class PublishPostEditDialog : public ElaDialog
{
    Q_OBJECT
public:
    explicit PublishPostEditDialog(QWidget* parent = nullptr);
    ~PublishPostEditDialog() = default;

    QString getText() const;
    QStringList getImgList() const;

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

private:
    void initContent();

    void selectImages();

private:
    ElaPlainTextEdit* _textEdit;
    ThemeColorButton* _publishBtn;
    QWidget* _firstRow;
    QWidget* _uploadWid;
    QStringList _imgList;

signals:
    void sigPublishClicked(const QString& text);

private slots:
    void slotUpdateBasicBaseStyle();

    void slotPublishClicked();
};

#endif // PUBLISHPOSTEDITDIALOG_H
