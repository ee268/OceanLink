#ifndef COLLECTPAGE_H
#define COLLECTPAGE_H

#include "basepage.h"

#include "../controls/collectlist.h"

class ElaNavigationBar;

class CollectPage : public BasePage
{
public:
    explicit CollectPage(QWidget* parent = nullptr);
    ~CollectPage();

    enum {
        AllPage = 1,
        ChatHistoryPage,
        SpaceArticlePage
    };

private:
    void initLeftWidget();
    void initRightWidget();

private:
    ElaNavigationBar* _navigationBar;
    QWidget* _allPage;
    QWidget* _chatHistoryPage;
    QWidget* _spaceArticlePage;

    CollectList* _allList;
    CollectList* _chatHistoryList;
    CollectList* _spaceArticleList;
};

#endif // COLLECTPAGE_H
