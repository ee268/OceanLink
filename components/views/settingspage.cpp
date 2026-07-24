#include "settingspage.h"

#include "basepage.h"

#include <QVBoxLayout>

#include "ElaText.h"

SettingsPage::SettingsPage(QWidget *parent)
    : ElaDialog(parent)
{
    this->setWindowTitle("设置");
    this->setIsStayTop(false);

    initContent();
}

void SettingsPage::initContent()
{
    _centralWid = new BasePage(this);
    auto centralWid = dynamic_cast<BasePage*>(_centralWid);
    centralWid->initDefaultLayout();

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);
    mainLayout->addWidget(_centralWid);
    this->setLayout(mainLayout);

    auto leftWid = centralWid->getLeftWidget();
    leftWid->setFixedWidth(280);
    QVBoxLayout* leftLayout = new QVBoxLayout(leftWid);

    _navigation = new ElaNavigationBar(leftWid);
    _navigation->setUserInfoCardVisible(false);

    for (int i = 0; i < 5; i++) {
        QWidget* wid = new QWidget(this);
        QVBoxLayout* layout = new QVBoxLayout(wid);
        ElaText* text = new ElaText(QString("测试页面%1").arg(i), wid);
        layout->addWidget(text, 0, Qt::AlignCenter);
        wid->setLayout(layout);
        wid->hide();

        _navigation->addPageNode(QString("测试页面%1").arg(i), wid, ElaIconType::AddressBook);
    }

    leftLayout->setContentsMargins(0, 0, 0, 0);
    leftLayout->addWidget(_navigation);
    leftWid->setLayout(leftLayout);
}
