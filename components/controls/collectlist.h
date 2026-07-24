#ifndef COLLECTLIST_H
#define COLLECTLIST_H

#include "ElaListView.h"

class CollectListModel;
class CollectListDelegate;

class CollectList : public ElaListView
{
    Q_OBJECT
public:
    explicit CollectList(QWidget* parent = nullptr);
    ~CollectList();

private:
    void initTestData();

private:
    CollectListModel* _model;
    CollectListDelegate* _delegate;
};

#endif // COLLECTLIST_H
