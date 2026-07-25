#ifndef COLLECTDETAILDIALOG_H
#define COLLECTDETAILDIALOG_H

#include <QPixmap>

#include "avatarwidget.h"
#include "icontext.h"
#include "../../global/global.h"
#include "replyeditarea.h"

#include "ElaDialog.h"
#include "ElaText.h"
#include "ElaPushButton.h"
#include "ElaScrollArea.h"

struct CollectDetailData {
    QString name;
    QString date;
    QPixmap avatar;
    CollectType type;
    QString content;
    QList<QPixmap> imgList;

    CollectDetailData() {}

    CollectDetailData(const QString& name, const QString& date,
                      const QPixmap& avatar, const CollectType& type,
                      const QList<QPixmap>& imgList, QString content)
        : name(name), date(date)
        , avatar(avatar), type(type)
        , content(content), imgList(imgList) {}
};

class CollectDetailDialog : public ElaDialog
{
    Q_OBJECT
public:
    explicit CollectDetailDialog(const CollectDetailData& data, QWidget* parent = nullptr);
    ~CollectDetailDialog();

    void updateData(const CollectDetailData& data);

protected:
    void showEvent(QShowEvent *) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    void initContent();
    void updateContent();

    void initChatMsgDetail();
    void initPostDetail();

private:
    CollectDetailData _data;
    AvatarWidget* _avatar;
    ElaText* _name;
    IconText* _date;
    ElaPushButton* _deleteBtn;
    ElaScrollArea* _scrollArea;
    ReplyEditArea* _replyEditArea;
};

#endif // COLLECTDETAILDIALOG_H
