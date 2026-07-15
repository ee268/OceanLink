#ifndef SESSIONLIST_H
#define SESSIONLIST_H

#include "ElaListView.h"

#include "../controls/sessionlistmodel.h"
#include "../controls/sessionlistdelegate.h"

class SessionList : public ElaListView
{
    Q_OBJECT
public:
    explicit SessionList(QWidget* parent = nullptr);
    ~SessionList();

protected:
    void wheelEvent(QWheelEvent *e) override;

    void mousePressEvent(QMouseEvent *event) override;

signals:
    void sigItemRightClicked(const QModelIndex& index);

private:
    SessionListModel* _model;
    SessionListDelegate* _delegate;
};

#endif // SESSIONLIST_H
