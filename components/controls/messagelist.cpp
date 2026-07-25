#include "messagelist.h"

#include "ElaTheme.h"

#include <QPropertyAnimation>
#include <QWheelEvent>
#include <QScrollBar>
#include <QMouseEvent>

MessageList::MessageList(QWidget *parent)
    : ElaListView(parent)
    , _model(new MessageListModel(this))
    , _delegate(new MessageListDelegate(this))
    , _scrollAnim(new QPropertyAnimation(this->verticalScrollBar(), "value"))
{
    _scrollAnim->setDuration(150);
    _scrollAnim->setEasingCurve(QEasingCurve::OutCubic);
    QString lightStyle = "#MessageList { border-top: 2px solid #d1d1d1; "
                         "border-bottom: 2px solid #d1d1d1; "
                         "background-color: transparent; }";

    QString darkStyle = "#MessageList { border-top: 2px solid #454545; "
                        "border-bottom: 2px solid #454545; "
                        "background-color: transparent; }";

    setObjectName("MessageList");
    setStyleSheet(lightStyle);
    connect(eTheme, &ElaTheme::themeModeChanged, this, [=](ElaThemeType::ThemeMode mode){
        if (mode == ElaThemeType::Dark) {
            this->setStyleSheet(darkStyle);
            return;
        }
        this->setStyleSheet(lightStyle);
    });

    for (int i = 0; i < 10; i++) {
        MessageItemData item;
        item.isSelf = i % 2 == 0;
        if (item.isSelf) {
            QString msg = "982134981234098127349127349182734192387412093487120934982134981234098127349127349182734192387412093487120934982134981234098127349127349182734192387412093487120934982134981234098127349127349182734192387412093487120934982134981234098127349127349182734192387412093487120934982134981234098127349127349182734192387412093487120934982134981234098127349127349182734192387412093487120934";
            item.msg = msg + msg + msg + msg + msg + msg + msg;
        }
        else {
            item.msg = "hello world";
            item.avatar = QPixmap(":/resource/image/avatar.jpg");
        }
        item.date = "";
        item.name = "XunLink";
        _model->addItem(item);
    }

    this->setModel(_model);
    this->setUniformItemSizes(false);
    this->setItemDelegate(_delegate);
    this->scrollToBottom();

    connect(_model, &MessageListModel::rowsInserted, this, &MessageList::slotRowInsert);
}

MessageListModel *MessageList::getModel() const
{
    return _model;
}

void MessageList::wheelEvent(QWheelEvent *e)
{
    int delta = e->angleDelta().y();
    int step = delta / 2;

    _scrollAnim->stop();
    _scrollAnim->setStartValue(this->verticalScrollBar()->value());
    _scrollAnim->setEndValue(this->verticalScrollBar()->value() - step);
    _scrollAnim->start();

    e->accept();
}

void MessageList::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton) {
        QModelIndex index = this->indexAt(event->pos());

        if (index.isValid()) {
            emit sigItemRightClicked(index);
        }
    }

    ElaListView::mousePressEvent(event);
}

void MessageList::slotRowInsert(const QModelIndex &parent, int first, int last)
{
    this->scrollToBottom();
}
