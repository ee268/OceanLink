#ifndef PUBLISHPOSTEDITAREA_H
#define PUBLISHPOSTEDITAREA_H

#include "ElaDialog.h"
#include "ElaPlainTextEdit.h"
#include "themecolorbutton.h"

class PublishPostEditArea : public ElaDialog
{
    Q_OBJECT
public:
    explicit PublishPostEditArea(QWidget* parent = nullptr);
    ~PublishPostEditArea() = default;

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
    void sigPublishClicked();

private slots:
    void slotUpdateBasicBaseStyle();

};

#endif // PUBLISHPOSTEDITAREA_H
