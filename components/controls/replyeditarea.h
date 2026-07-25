#ifndef REPLYEDITAREA_H
#define REPLYEDITAREA_H

#include <QWidget>
#include <QPropertyAnimation>
#include <memory>

#include "postitemdetail.h"
#include "themecolorbutton.h"

#include "ElaLineEdit.h"

class ReplyEditArea: public QWidget
{
    Q_OBJECT
public:
    explicit ReplyEditArea(QWidget* parent = nullptr);

    void showAnimation();
    void hideAnimation();

    void setReplyCommentData(std::shared_ptr<ReplyCommentData> data);

    bool getIsShow() const;
    void setIsShow(bool isShow);

protected:
    void paintEvent(QPaintEvent* event) override;
    bool eventFilter(QObject* obj, QEvent* event) override;

private:
    ElaLineEdit* _replyEdit;
    ThemeColorButton* _sendBtn;
    QPropertyAnimation* _slideAnimation;
    bool _isShow;

    std::shared_ptr<ReplyCommentData> _data;

signals:
    void sigSendReplyBtnClicked(std::shared_ptr<ReplyCommentData> data, const QString& text);

private slots:
    void slotSendBtnClicked();
};

#endif // REPLYEDITAREA_H
