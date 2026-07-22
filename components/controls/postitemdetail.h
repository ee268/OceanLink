#ifndef POSTITEMDETAIL_H
#define POSTITEMDETAIL_H

#include "postitem.h"

#include <QVBoxLayout>
#include <vector>
#include <memory>

#include "ElaLineEdit.h"
#include "ElaDrawerArea.h"

class IconText;

struct ReplyCommentData {
    QString name;
    QPixmap avatar;
    QString date;
    QString content;
    std::shared_ptr<ReplyCommentData> parent;
    std::vector<std::shared_ptr<ReplyCommentData>> replys;

    ReplyCommentData() = default;

    ReplyCommentData(const QString& name, const QPixmap& avatar,
                     const QString& date, const QString& content)
        : name(name), avatar(avatar), date(date), content(content)
    {}
};

class CommentWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CommentWidget(std::shared_ptr<ReplyCommentData> data, int indent = 0, QWidget* parent = nullptr);

protected:
    bool eventFilter(QObject* obj, QEvent* event) override;

private:
    IconText* _replyText;

    std::shared_ptr<ReplyCommentData> _data;

signals:
    void sigReplyButtonClicked(std::shared_ptr<ReplyCommentData> data);
};

class PostItemDetail : public PostItem
{
    Q_OBJECT
public:
    explicit PostItemDetail(const PostData& data, QWidget *parent = nullptr);

    void updateData(const PostData& data);
    void setCommentList(std::vector<std::shared_ptr<ReplyCommentData>> comments);
    void addComment(std::shared_ptr<ReplyCommentData> comment);

private:
    void initCommentArea();
    void updateCommentList();

    CommentWidget* createCommentWidget(std::shared_ptr<ReplyCommentData>, int indent = 0);

private:
    std::vector<std::shared_ptr<ReplyCommentData>> _comments;
    ElaLineEdit* _commentEdit;
    QWidget* _commentListWid;
    QVBoxLayout* _commentListLayout;
    QList<CommentWidget*> _commentWidgets;

    ElaDrawerArea* _replyDrawer;

signals:
    void sigSendCommentSuccess();

private slots:
    void slotSendBtnClicked();

    void slotReplyButtonClicked(std::shared_ptr<ReplyCommentData> data);
};

#endif // POSTITEMDETAIL_H
