#include "publishposteditdialog.h"

#include <QVBoxLayout>
#include <QFileDialog>
#include <QFileInfo>
#include <QMouseEvent>

#include "icontext.h"

#include "ElaIcon.h"
#include "ElaTheme.h"
#include "ElaImageCard.h"
#include "ElaScrollBar.h"
#include "ElaMessageBar.h"

PublishPostEditDialog::PublishPostEditDialog(QWidget *parent)
    : ElaDialog(parent)
    , _textEdit(new ElaPlainTextEdit(this))
    , _publishBtn(new ThemeColorButton("发布", this))
{
    initContent();

    this->setWindowTitle("写一篇");

    this->setWindowButtonFlags(ElaAppBarType::CloseButtonHint |
                               ElaAppBarType::MinimizeButtonHint |
                               ElaAppBarType::StayTopButtonHint);

    this->setSizeGripEnabled(false);
    this->setIsFixedSize(true);
}

QString PublishPostEditDialog::getText() const
{
    return _textEdit->toPlainText();
}

QStringList PublishPostEditDialog::getImgList() const
{
    return _imgList;
}

bool PublishPostEditDialog::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == _uploadWid) {
        if (event->type() == QEvent::MouseButtonPress) {
            QMouseEvent* mouseEvent = dynamic_cast<QMouseEvent*>(event);
            if (mouseEvent->button() == Qt::LeftButton) {
                selectImages();
            }
        }
    }

    return QWidget::eventFilter(obj, event);
}

void PublishPostEditDialog::initContent()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(10);

    //编辑区、上传图片
    _firstRow = new QWidget(this);
    QVBoxLayout* firstRowLayout = new QVBoxLayout(_firstRow);
    firstRowLayout->setContentsMargins(20, 20, 20, 20);
    firstRowLayout->setSpacing(10);

    _textEdit->setPlaceholderText("写一篇帖子...");
    _textEdit->setObjectName("PublishPostTextEdit");
    _textEdit->setStyleSheet("#PublishPostTextEdit{ border: none; background-color: transparent; }");

    ElaScrollBar* scrollBar = new ElaScrollBar(this);
    _textEdit->setVerticalScrollBar(scrollBar);

    _uploadWid = new QWidget(_firstRow);
    _uploadWid->setFixedSize(100, 100);
    _uploadWid->installEventFilter(this);

    QVBoxLayout* uploadLayout = new QVBoxLayout(_uploadWid);
    uploadLayout->setContentsMargins(8, 8, 8, 8);
    uploadLayout->setSpacing(4);
    uploadLayout->setAlignment(Qt::AlignCenter);

    ElaImageCard* imageCard = new ElaImageCard(_uploadWid);
    imageCard->setFixedSize(25, 25);
    QIcon icon = ElaIcon::getInstance()->getElaIcon(ElaIconType::Images, QColor(121, 113, 123));
    QPixmap pixmap = icon.pixmap(40, 40);
    imageCard->setCardImage(pixmap.toImage());

    IconText* uploadText = new IconText("上传图片", _uploadWid);
    QFont textFont = uploadText->font();
    textFont.setPixelSize(11);
    uploadText->setFont(textFont);
    uploadText->setAlignment(Qt::AlignCenter);

    uploadLayout->addWidget(imageCard, 0, Qt::AlignCenter);
    uploadLayout->addWidget(uploadText, 0, Qt::AlignCenter);
    _uploadWid->setLayout(uploadLayout);

    firstRowLayout->addWidget(_textEdit, 1);
    firstRowLayout->addWidget(_uploadWid);
    _firstRow->setLayout(firstRowLayout);

    //发布按钮
    _publishBtn->setFixedWidth(80);

    connect(_publishBtn, &ThemeColorButton::clicked, this, &PublishPostEditDialog::slotPublishClicked);

    QWidget* secondRow = new QWidget(this);
    QHBoxLayout* secondRowLayout = new QHBoxLayout(secondRow);
    secondRowLayout->setContentsMargins(0, 0, 0, 0);
    secondRowLayout->addStretch();
    secondRowLayout->addWidget(_publishBtn, 0, Qt::AlignCenter);
    secondRow->setLayout(secondRowLayout);

    mainLayout->addWidget(_firstRow);
    mainLayout->addWidget(secondRow);
    setLayout(mainLayout);

    slotUpdateBasicBaseStyle();

    connect(eTheme, &ElaTheme::themeModeChanged, this, &PublishPostEditDialog::slotUpdateBasicBaseStyle);
}

void PublishPostEditDialog::slotUpdateBasicBaseStyle()
{
    _firstRow->setStyleSheet(
        QString("background-color: %1; border-radius: 8px;")
            .arg(ElaThemeColor(eTheme->getThemeMode(), BasicBase).name()));

    _uploadWid->setStyleSheet(
        QString("background-color: %1; border-radius: 8px;")
            .arg(ElaThemeColor(eTheme->getThemeMode(), BasicHover).name()));
}

void PublishPostEditDialog::slotPublishClicked()
{
    QString plainText = _textEdit->toPlainText();
    if (plainText.isEmpty()) {
        ElaMessageBar::warning(ElaMessageBarType::Top, "提示", "请输入内容", 2000, this);
        return;
    }

    _textEdit->clear();
    emit sigPublishClicked(plainText);
}

void PublishPostEditDialog::selectImages()
{
    QString filter = "图片文件 (*.png *.jpg *.jpeg *.gif)";
    QStringList files = QFileDialog::getOpenFileNames(this, "选择图片", QString(), filter);

    if (files.isEmpty()) {
        return;
    }

    if (files.count() > 8) {
        ElaMessageBar::warning(ElaMessageBarType::Top, "提示", "最多只能选择8张图片", 2000, this);
        return;
    }

    for (const QString& file : files) {

        QFileInfo info(file);
        if (info.size() > 20 * 1024 * 1024) {
            ElaMessageBar::warning(ElaMessageBarType::Top, "提示",
                QString("图片 %1 超过20MB，已跳过").arg(info.fileName()), 2000, this);
            continue;
        }

        _imgList.append(file);
    }

    if (!files.isEmpty() && _imgList.count() > 0) {
        ElaMessageBar::success(ElaMessageBarType::Top, "成功",
            QString("共 %1 张").arg(_imgList.count()), 2000, this);
    }
}
