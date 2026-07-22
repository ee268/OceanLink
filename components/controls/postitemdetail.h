#ifndef POSTITEMDETAIL_H
#define POSTITEMDETAIL_H

#include "postitem.h"

#include <QVBoxLayout>
#include <vector>

#include "ElaLineEdit.h"

class IconText;

struct ReplyCommentData {
    QString name;
    QPixmap avatar;
    QString date;
    QString content;
    ReplyCommentData* parent;
    std::vector<std::unique_ptr<ReplyCommentData>> replys;

    ReplyCommentData() : parent(nullptr) {}

    ReplyCommentData(const QString& name, const QPixmap& avatar,
                     const QString& date, const QString& content)
        : name(name), avatar(avatar), date(date), content(content), parent(nullptr)
    {}
};

class CommentWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CommentWidget(const ReplyCommentData& comment, int indent = 0, QWidget* parent = nullptr);

protected:
    bool eventFilter(QObject* obj, QEvent* event) override;

private:
    IconText* _replyText;
};

class PostItemDetail : public PostItem
{
    Q_OBJECT
public:
    explicit PostItemDetail(const PostData& data, QWidget *parent = nullptr);

    void updateData(const PostData& data);
    void setCommentList(std::vector<std::unique_ptr<ReplyCommentData>> comments);
    void addComment(std::unique_ptr<ReplyCommentData> comment);

private:
    void initCommentArea();
    void updateCommentList();

    CommentWidget* createCommentWidget(const ReplyCommentData& comment, int indent = 0);

private:
    std::vector<std::unique_ptr<ReplyCommentData>> _comments;
    ElaLineEdit* _commentEdit;
    QWidget* _commentListWid;
    QVBoxLayout* _commentListLayout;
    QList<CommentWidget*> _commentWidgets;

signals:
    void sigSendCommentSuccess();

private slots:
    void slotSendBtnClicked();
};

#endif // POSTITEMDETAIL_H
