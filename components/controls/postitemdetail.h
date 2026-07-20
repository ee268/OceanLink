#ifndef POSTITEMDETAIL_H
#define POSTITEMDETAIL_H

#include "postitem.h"

#include <QVBoxLayout>
#include <vector>

#include "ElaLineEdit.h"

struct ReplyCommentData {
    QString name;
    QString avatar;
    QString date;
    QString content;
    std::vector<ReplyCommentData> replys;

    ReplyCommentData() = default;

    ReplyCommentData(const QString& name, const QString& avatar,
                     const QString& date, const QString& content)
        : name(name), avatar(avatar), date(date), content(content)
    {}
};

class PostItemDetail : public PostItem
{
    Q_OBJECT
public:
    explicit PostItemDetail(const PostData& data, QWidget *parent = nullptr);

    void updateData(const PostData& data);
    void setCommentList(const QList<ReplyCommentData>& comments);

private:
    void initCommentArea();
    void updateCommentList();

    QWidget* createCommentWidget(const ReplyCommentData& comment, int indent = 0);

private:
    QList<ReplyCommentData> _comments;
    ElaLineEdit* _commentEdit;
    QWidget* _commentListWid;
    QVBoxLayout* _commentListLayout;
    QList<QWidget*> _commentWidgets;
};

#endif // POSTITEMDETAIL_H
