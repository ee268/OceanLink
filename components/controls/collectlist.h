#ifndef COLLECTLIST_H
#define COLLECTLIST_H

#include "ElaListView.h"


#include "collectlistmodel.h"
#include "collectlistdelegate.h"
#include "moreoptpopup.h"
#include "collectdetaildialog.h"

class CollectList : public ElaListView
{
    Q_OBJECT
public:
    explicit CollectList(QWidget* parent = nullptr);
    ~CollectList();

protected:
    void mousePressEvent(QMouseEvent *event);

private:
    void initTestData();

private:
    CollectListModel* _model;
    CollectListDelegate* _delegate;

    MoreOptPopup* _rightOptions;
    QString _multiSelectKey;
    QString _delKey;

    CollectDetailDialog* _detailDialog;
};

#endif // COLLECTLIST_H
