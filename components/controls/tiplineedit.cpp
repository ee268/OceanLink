#include "tiplineedit.h"

#include <QVBoxLayout>
#include <QAction>

#include "ElaIcon.h"

TipLineEdit::TipLineEdit(QWidget *parent)
    : QWidget{parent}
    , _tipText(new IconText(this))
    , _lineEdit(new ElaLineEdit(this))
{
    initContent();
}

TipLineEdit::~TipLineEdit()
{

}

void TipLineEdit::setTipText(const QString &text)
{
    _tipText->setText(text);
}

void TipLineEdit::setPixelSize(int size)
{
    _tipText->setPixelSize(size);
}

ElaLineEdit *TipLineEdit::getLineEdit() const
{
    return _lineEdit;
}

void TipLineEdit::setEchoModePassword()
{
    QAction* eyeAction = _lineEdit->addAction(
        ElaIcon::getInstance()->getElaIcon(ElaIconType::Eye),
        QLineEdit::TrailingPosition
        );
    eyeAction->setVisible(false);

    connect(_lineEdit, &QLineEdit::textChanged, this, [=](const QString& text) {
        eyeAction->setVisible(text.length() > 0);
    });

    connect(eyeAction, &QAction::triggered, this, [=]() {
        if (_lineEdit->echoMode() == QLineEdit::Password) {
            _lineEdit->setEchoMode(QLineEdit::Normal);
            eyeAction->setIcon(ElaIcon::getInstance()->getElaIcon(ElaIconType::EyeSlash));
        } else {
            _lineEdit->setEchoMode(QLineEdit::Password);
            eyeAction->setIcon(ElaIcon::getInstance()->getElaIcon(ElaIconType::Eye));
        }
    });
}

void TipLineEdit::initContent()
{
    _lineEdit->setIsClearButtonEnable(true);
    _lineEdit->setBorderRadius(8);
    _tipText->setPixelSize(11);
    this->setContentsMargins(0, 0, 0, 0);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);
    mainLayout->addWidget(_lineEdit);
    mainLayout->addWidget(_tipText);

    this->setLayout(mainLayout);
}
