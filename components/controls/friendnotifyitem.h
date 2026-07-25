#ifndef FRIENDNOTIFYITEM_H
#define FRIENDNOTIFYITEM_H

#include <QWidget>

#include "../../global/global.h"

#include "ElaPushButton.h"

#include "../controls/icontext.h"

struct FriendNotifyData {
    QPixmap avatar;
    QString name;
    QString date;
    QString msg;                  //留言
    bool isSelf;                  //true为我向他人发送，false为他人向我发送
    FriendNotifyStatus status;    //通知状态

    FriendNotifyData& operator=(FriendNotifyData& other){
        avatar = other.avatar;
        name = other.name;
        date = other.date;
        msg = other.msg;
        isSelf = other.isSelf;
        status = other.status;

        return *this;
    }

    FriendNotifyData(const QPixmap& avatar, const QString& name,
                     const QString& date, const QString& msg,
                    const bool& isSelf, const FriendNotifyStatus& status)
        : avatar(avatar)
        , name(name)
        , date(date)
        , msg(msg)
        , isSelf(isSelf)
        , status(status)
    {};
};

class FriendNotifyItem : public QWidget
{
    Q_OBJECT
public:
    explicit FriendNotifyItem(const FriendNotifyData& data, QWidget *parent = nullptr);

    FriendNotifyData getNotifyData() const;

    void setStatusText(FriendNotifyStatus status);

protected:
    void paintEvent(QPaintEvent *event) override;
    QSize sizeHint() const override;

private:
    void initContent();

    void setButtonHidden();

private:
    FriendNotifyData _data;

    ElaPushButton* _agreeButton;
    ElaPushButton* _refuseButton;

    IconText* _statusText;

signals:
    void sigAgreeButtonClicked();
    void sigRefuseButtonClicked();

private slots:
    void slotAgreeButtonClicked();
    void slotRefuseButtonClicked();
};

#endif // FRIENDNOTIFYITEM_H
