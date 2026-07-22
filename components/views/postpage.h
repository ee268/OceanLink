#ifndef POSTPAGE_H
#define POSTPAGE_H

#include "basepage.h"

#include <QPushButton>

#include "../controls/spacelist.h"
#include "../controls/postlist.h"
#include "../controls/postitemdetail.h"

#include "ElaPushButton.h"

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

private slots:
    void slotPostItemClicked(const PostData& data);

    void slotBackBtnClicked();
};

#endif // POSTPAGE_H
