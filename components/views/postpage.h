#ifndef POSTPAGE_H
#define POSTPAGE_H

#include "basepage.h"

#include <QPushButton>
#include <QPropertyAnimation>

#include "../controls/spacelist.h"
#include "../controls/postlist.h"
#include "../controls/postitemdetail.h"
#include "../controls/themecolorbutton.h"

#include "ElaLineEdit.h"

class EveryoneButton: public QPushButton
{
public:
    explicit EveryoneButton(const QString &text, const QIcon& icon, QWidget *parent = nullptr);

    void setFixedSize(int w, int h);

protected:
    void paintEvent(QPaintEvent* event) override;
    bool eventFilter(QObject* obj, QEvent* event) override;

private:
    enum State { Normal, Hover, Pressed };
    State _state = Normal;

    QString _text;
    QIcon _icon;
};

class ReplyEditArea: public QWidget
{
    Q_OBJECT
public:
    explicit ReplyEditArea(QWidget* parent = nullptr);

    void showAnimation();
    void hideAnimation();

    void setReplyCommentData(std::shared_ptr<ReplyCommentData> data);

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

class PostPage : public BasePage
{
    Q_OBJECT
public:
    explicit PostPage(QWidget* parent = nullptr);
    ~PostPage() = default;

    enum {
        PostListPage = 1,
        PostDetailPage
    };

private:
    void initLeftWidget();
    void initRightWidget();

private:
    ElaPushButton* _backBtn;
    SpaceList* _spaceList;
    PostList* _postList;
    PostItemDetail* _postDetail;
    ReplyEditArea* _replyEditArea;

private slots:
    void slotPostItemClicked(const PostData& data);

    void slotBackBtnClicked();

    void slotPublishBtnClicked();
};

#endif // POSTPAGE_H
