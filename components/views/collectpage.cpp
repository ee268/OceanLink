#include "collectpage.h"

#include <QVBoxLayout>

#include "ElaNavigationBar.h"

CollectPage::CollectPage(QWidget *parent/* = nullptr*/)
    : BasePage(parent)
    , _navigationBar(nullptr)
    , _allPage(nullptr)
    , _chatHistoryPage(nullptr)
    , _spaceArticlePage(nullptr)
    , _allList(nullptr)
    , _chatHistoryList(nullptr)
    , _spaceArticleList(nullptr)
{
    initDefaultLayout();
    initLeftWidget();
    initRightWidget();
}

CollectPage::~CollectPage() {}

void CollectPage::initLeftWidget()
{
    QWidget* leftWid = this->getLeftWidget();
    leftWid->setFixedWidth(280);
    QVBoxLayout* leftLayout = new QVBoxLayout(leftWid);
    leftLayout->setContentsMargins(0, 5, 0, 0);
    leftLayout->setSpacing(0);

    _navigationBar = new ElaNavigationBar(leftWid);

    _allPage = new QWidget(this);
    _chatHistoryPage = new QWidget(this);
    _spaceArticlePage = new QWidget(this);

    _navigationBar->addPageNode("所有", _allPage, ElaIconType::Grid2);
    _navigationBar->addPageNode("聊天记录", _chatHistoryPage, ElaIconType::CommentDots);
    _navigationBar->addPageNode("空间文章", _spaceArticlePage, ElaIconType::Blog);

    _navigationBar->setUserInfoCardVisible(false);

    leftLayout->addWidget(_navigationBar);
    leftWid->setLayout(leftLayout);

    connect(_navigationBar, &ElaNavigationBar::navigationNodeClicked,
            this, [this](ElaNavigationType::NavigationNodeType nodeType, QString nodeKey, bool isRouteBack) {
        QStackedWidget* stackedWid = this->getStackedWidget();
        if (nodeType == ElaNavigationType::PageNode) {
            for (int i = 0; i < stackedWid->count(); i++) {
                QWidget* page = stackedWid->widget(i);
                if (page && page->property("ElaPageKey").toString() == nodeKey) {
                    stackedWid->setCurrentIndex(i);
                    break;
                }
            }
        }
    });
}

void CollectPage::initRightWidget()
{
    QStackedWidget* stackedWid = this->getStackedWidget();
    stackedWid->addWidget(_allPage);
    stackedWid->addWidget(_chatHistoryPage);
    stackedWid->addWidget(_spaceArticlePage);

    _allList = new CollectList(this);
    _chatHistoryList = new CollectList(this);
    _spaceArticleList = new CollectList(this);

    QMargins margins(0, 2, 0, 2);

    QVBoxLayout* allLayout = new QVBoxLayout(_allPage);
    allLayout->addWidget(_allList);
    allLayout->setContentsMargins(margins);
    _allPage->setLayout(allLayout);

    QVBoxLayout* chatHistoryLayout = new QVBoxLayout(_chatHistoryPage);
    chatHistoryLayout->addWidget(_chatHistoryList);
    chatHistoryLayout->setContentsMargins(margins);
    _chatHistoryPage->setLayout(chatHistoryLayout);

    QVBoxLayout* spaceArticleLayout = new QVBoxLayout(_spaceArticlePage);
    spaceArticleLayout->addWidget(_spaceArticleList);
    spaceArticleLayout->setContentsMargins(margins);
    _spaceArticlePage->setLayout(spaceArticleLayout);

    stackedWid->setCurrentIndex(AllPage);
}
