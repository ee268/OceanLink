#ifndef POSTPAGE_H
#define POSTPAGE_H

#include "basepage.h"

#include <QPushButton>
#include <QPropertyAnimation>

#include "../controls/spacelist.h"
#include "../controls/postlist.h"
#include "../controls/postitemdetail.h"
#include "../controls/themecolorbutton.h"
#include "../controls/publishposteditdialog.h"
#include "../controls/replyeditarea.h"

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

class PostPage : public BasePage
{
    Q_OBJECT
public:
    explicit PostPage(QWidget* parent = nullptr);
    ~PostPage();

    enum {
        PostListPage = 1,
        PostDetailPage
    };

protected:
    void resizeEvent(QResizeEvent* event) override;

private:
    void initLeftWidget();
    void initRightWidget();

private:
    ElaPushButton* _backBtn;
    SpaceList* _spaceList;
    PostList* _postList;

    PostItemDetail* _postDetail;

    ReplyEditArea* _replyEditArea;
    PublishPostEditDialog* _publishArea;

private slots:
    void slotPostItemClicked(const PostData& data);

    void slotBackBtnClicked();

    void slotWritePostBtnClicked();

    void slotPublishClicked(const QString& text);
};

#endif // POSTPAGE_H
