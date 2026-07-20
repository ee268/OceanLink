#ifndef POSTLIST_H
#define POSTLIST_H

#include "ElaScrollArea.h"

#include "postitem.h"

class PostList : public ElaScrollArea
{
    Q_OBJECT
public:
    explicit PostList(QWidget* parent = nullptr);
    ~PostList() = default;

    void addPost(const PostData& data);
    void removePost(int index);

private:
    void initContent();

signals:
    void sigPostItemClicked(const PostData& data);

private:
    QList<PostItem*> _postList;
};

#endif // POSTLIST_H
