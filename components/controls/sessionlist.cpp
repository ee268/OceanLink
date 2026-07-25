#include "sessionlist.h"
#include <QStringListModel>

#include <QScrollBar>
#include <QWheelEvent>
#include <QPropertyAnimation>
#include <QMouseEvent>

SessionList::SessionList(QWidget *parent)
    : ElaListView(parent)
    , _model(new SessionListModel(this))
    , _delegate(new SessionListDelegate(this))
{
    setObjectName("SessionList");
    setStyleSheet("#SessionList { border: none; background-color: transparent; }");

    for (int i = 0; i < 20; i++) {
        UserItemData item;
        item.name = QString("一二三四五六齐吧就时是%1").arg(i + 1);
        item.latest_msg = QString("这是第 %1 条消息123123123123123123123123123123123123123").arg(i + 1);
        item.latest_date = "2026/12/23";
        item.avatar = QPixmap(":/resource/image/avatar.jpg");
        item.unread_cnt = 10;
        _model->addItem(item);
    }

    this->setModel(_model);
    this->setItemDelegate(_delegate);
}

SessionList::~SessionList() {}

void SessionList::wheelEvent(QWheelEvent *e)
{
    int delta = e->angleDelta().y();
    int step = delta / 2;

    QPropertyAnimation* anim = new QPropertyAnimation(this->verticalScrollBar(), "value");
    anim->setDuration(150);
    anim->setStartValue(this->verticalScrollBar()->value());
    anim->setEndValue(this->verticalScrollBar()->value() - step);
    anim->setEasingCurve(QEasingCurve::OutCubic);
    anim->start(QAbstractAnimation::DeleteWhenStopped);

    e->accept();
}

void SessionList::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton) {
        QModelIndex index = this->indexAt(event->pos());

        if (index.isValid()) {
            emit sigItemRightClicked(index);
        }
    }

    ElaListView::mousePressEvent(event);
}
