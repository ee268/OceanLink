#ifndef CONTACTLIST_H
#define CONTACTLIST_H

#include "ElaTreeView.h"

#include "contactlistmodel.h"
#include "contactlistdelegate.h"

class ContactListModel;
class ContactListDelegate;

class ContactList : public ElaTreeView
{
    Q_OBJECT
public:
    explicit ContactList(QWidget* parent);
    ~ContactList() = default;

    QStringList getGroupNames() const;

    void updateContact(const QModelIndex& index);

    void changeContactGroup(const QModelIndex& index, const QString& newGroup);

    void addGroup(const QString& groupName);

private:
    ContactListModel* _model;
    ContactListDelegate* _delegate;

signals:
    void sigContactClicked(const QModelIndex& index);

private slots:
    void slotContactClicked(const QModelIndex& index);
};

#endif // CONTACTLIST_H
