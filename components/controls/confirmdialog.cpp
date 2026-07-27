#include "confirmdialog.h"

#include <QVBoxLayout>
#include <QShowEvent>
#include <QPushButton>

ConfirmDialog::ConfirmDialog(QWidget *parent)
    : ElaContentDialog(parent)
{
    initDialog();

    initContent();
}

Qt::CheckState ConfirmDialog::getAskCheckState() const
{
    return _isAlwaysAsk->checkState();
}

void ConfirmDialog::setTitleText(const QString &text)
{
    _title->setText(text);
}

void ConfirmDialog::setSubTitleText(const QString &text)
{
    _subTitle->setText(text);
}

void ConfirmDialog::setCheckBoxHidden(bool isHidden)
{
    _isAlwaysAsk->setHidden(isHidden);
}

void ConfirmDialog::setMiddleButtonHidden(bool isHidden)
{
    QList<QPushButton*> buttons = findChildren<QPushButton*>();
    for (QPushButton* button : buttons) {
        if (button->text() == "最小化") {
            button->setHidden(isHidden);
            break;
        }
    }
}

void ConfirmDialog::setVisible(bool visible)
{
    if (visible && getAskCheckState() == Qt::Checked) {
        emit rightButtonClicked();
        return;
    }

    ElaContentDialog::setVisible(visible);
}

void ConfirmDialog::initDialog()
{
    setLeftButtonText("取消");
    setMiddleButtonText("最小化");
    setRightButtonText("确定");

    connect(this, &ConfirmDialog::leftButtonClicked, this, &ConfirmDialog::close);
    connect(this, &ConfirmDialog::middleButtonClicked, this->parentWidget(), [this](){
        this->parentWidget()->showMinimized();
        this->close();
    });
}

void ConfirmDialog::initContent()
{
    _centralWid = new QWidget(this);

    QVBoxLayout* centralVLayout = new QVBoxLayout(_centralWid);
    centralVLayout->setContentsMargins(15, 25, 15, 10);

    _title = new ElaText("退出", _centralWid);
    _title->setTextStyle(ElaTextType::Title);
    _title->setIsWrapAnywhere(false);

    _subTitle = new ElaText("确定要退出程序吗？", _centralWid);
    _subTitle->setTextStyle(ElaTextType::Body);
    _subTitle->setIsWrapAnywhere(false);

    _isAlwaysAsk = new ElaCheckBox("不再询问", _centralWid);
    _isAlwaysAsk->setFont(_subTitle->font());

    QWidget* bottomWid = new QWidget(_centralWid);
    QHBoxLayout* centralHLayout = new QHBoxLayout(bottomWid);

    centralHLayout->addWidget(_subTitle);
    centralHLayout->addStretch();
    centralHLayout->addWidget(_isAlwaysAsk);
    centralHLayout->setContentsMargins(0, 0, 0, 0);
    bottomWid->setLayout(centralHLayout);

    centralVLayout->addWidget(_title);
    centralVLayout->addSpacing(2);
    centralVLayout->addWidget(bottomWid);
    centralVLayout->addStretch();

    _centralWid->setLayout(centralVLayout);

    setCentralWidget(_centralWid);
}
