#ifndef MESSAGELIST_H
#define MESSAGELIST_H

#include "ElaListView.h"

#include "messagelistdelegate.h"
#include "messagelistmodel.h"

#include <QPropertyAnimation>

class MessageList : public ElaListView
{
    Q_OBJECT
public:
    explicit MessageList(QWidget* parent = nullptr);
    ~MessageList() = default;

    MessageListModel* getModel() const;

protected:
    void wheelEvent(QWheelEvent *e) override;

    void mousePressEvent(QMouseEvent* e) override;

private:
    MessageListModel* _model;
    MessageListDelegate* _delegate;
    QPropertyAnimation* _scrollAnim;

signals:
    void sigItemRightClicked(const QModelIndex& index);

private slots:
    void slotRowInsert(const QModelIndex &parent, int first, int last);
};

#endif // MESSAGELIST_H
