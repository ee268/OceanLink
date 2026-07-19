#ifndef SPACELIST_H
#define SPACELIST_H

#include "ElaListView.h"

#include "spacelistmodel.h"
#include "spacelistdelegate.h"

class SpaceListModel;
class SpaceListDelegate;

class SpaceList : public ElaListView
{
    Q_OBJECT
public:
    explicit SpaceList(QWidget* parent);
    ~SpaceList() = default;

private:
    SpaceListModel* _model;
    SpaceListDelegate* _delegate;
};

#endif // SPACELIST_H
