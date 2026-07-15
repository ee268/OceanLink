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
};

#endif // CONTACTLIST_H
