#ifndef POSTITEM_H
#define POSTITEM_H

#include <QWidget>

#include "ElaText.h"
#include "ElaPromotionView.h"

#include "avatarwidget.h"
#include "icontext.h"

struct PostData {
    QString name;
    QString account;
    QString date;
    QPixmap avatar;
    QList<QPixmap> imgList;
    QString content;
    int likeCnt;
    int commentCnt;
    int collectCnt;
    QString postID;

    PostData()
        : likeCnt(0)
        , commentCnt(0)
        , collectCnt(0)
    {}

    PostData(const QString& name, const QString& account, const QString& date,
             const QPixmap& avatar, const QList<QPixmap>& imgList, const QString& content,
             int likeCnt, int commentCnt, int collectCnt, const QString& postID)
        : name(name)
        , account(account)
        , date(date)
        , avatar(avatar)
        , imgList(imgList)
        , content(content)
        , likeCnt(likeCnt)
        , commentCnt(commentCnt)
        , collectCnt(collectCnt)
        , postID(postID)
    {}

    PostData& operator=(const PostData& other) {
        if (this != &other) {
            name = other.name;
            account = other.account;
            date = other.date;
            avatar = other.avatar;
            imgList = other.imgList;
            content = other.content;
            likeCnt = other.likeCnt;
            commentCnt = other.commentCnt;
            collectCnt = other.collectCnt;
            postID = other.postID;
        }
        return *this;
    }
};

class PostItem : public QWidget
{
    Q_OBJECT
public:
    explicit PostItem(const PostData& data, QWidget *parent = nullptr);

    void updateData(const PostData& data);

protected:
    void paintEvent(QPaintEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;

private:
    void initContent();

signals:
    void sigPostItemClicked(const PostData& data);

private:
    PostData _data;

    AvatarWidget* _avatar;
    ElaText* _nameText;
    IconText* _dateText;
    ElaText* _contentText;
    ElaPromotionView* _promotionView;
    IconText* _likeText;
    IconText* _commentText;
    IconText* _collectText;
};

#endif // POSTITEM_H
