#include "contactlist.h"

#include <QDebug>
#include <QTimer>

ContactList::ContactList(QWidget *parent)
    : ElaTreeView(parent)
    , _model(new ContactListModel(this))
    , _delegate(new ContactListDelegate(this))
{
    this->setModel(_model);
    this->setItemDelegate(_delegate);
    this->setHeaderHidden(true);

    connect(this, &ContactList::clicked, this, &ContactList::slotContactClicked);
}

QStringList ContactList::getGroupNames() const
{
    return _model->getGroupNames();
}

void ContactList::updateContact(const QModelIndex &index)
{
    _model->updateContact(index);
}

void ContactList::changeContactGroup(const QModelIndex &index, const QString &newGroup)
{
    _model->changeContactGroup(index, newGroup);
    this->expand(_model->getGroupIndex(newGroup));
    this->setCurrentIndex(index);
}

void ContactList::slotContactClicked(const QModelIndex &index)
{
    bool isGroupItem = index.data(ContactListModel::IsGroup).toBool();
    if (!isGroupItem) {
        emit sigContactClicked(index);
    }
}
