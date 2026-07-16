#ifndef CONTACTLIST_H
#define CONTACTLIST_H

#include "ElaTreeView.h"

class ContactListModel;
class ContactListDelegate;

class ContactList : public ElaTreeView
{
    Q_OBJECT
public:
    explicit ContactList(QWidget* parent);
    ~ContactList() = default;

private:
    ContactListModel* _model;
    ContactListDelegate* _delegate;

signals:
    void sigContactClicked(const QModelIndex& index);

private slots:
    void slotContactClicked(const QModelIndex& index);
};

#endif // CONTACTLIST_H
