#include "moreoptpopup.h"

#include <QEvent>
#include <QLabel>
#include <QMouseEvent>
#include <QPropertyAnimation>
#include <QApplication>
#include <QScreen>

#include "ElaTheme.h"

#include "icontext.h"

MoreOptPopup::MoreOptPopup(QWidget *parent)
    : QDialog(parent)
    , _shadowColorLight(QColor(0, 0, 0, 50))
    , _shadowColorDark(QColor(105, 105, 105, 80))
    , _lightColor(QColor(236, 236, 236))
    , _darkColor(QColor(32, 32, 32))
    , _widget(new QWidget(this))
    , _shadowInit(false)
    , _shadow(new QGraphicsDropShadowEffect(this))
{
    initPopup();

    initSetShadow(_shadowColorLight, _lightColor);

    initContent();

    installEventFilter(this);

    connect(eTheme, &ElaTheme::themeModeChanged, [this](ElaThemeType::ThemeMode mode){
        if (mode == ElaThemeType::Light) {
            initSetShadow(_shadowColorLight, _lightColor);
        }
        else {
            initSetShadow(_shadowColorDark, _darkColor);
        }
    });
}

MoreOptPopup::~MoreOptPopup() {
}

void MoreOptPopup::addOption(const QString &name, const QIcon &icon,
                             const QString &key, QWidget *value)
{
    OptionWidget* optionWid = this->addOptWid(name, icon, key);

    connect(optionWid, &OptionWidget::sigOptionClicked, this, &MoreOptPopup::slotOptionClicked);

    _optList.insert(key, value);
}

void MoreOptPopup::addOption(const QString &name, const QIcon &icon, const QString &key, const indexFunc func)
{
    OptionWidget* optionWid = this->addOptWid(name, icon, key);

    connect(optionWid, &OptionWidget::sigOptionClicked, this, &MoreOptPopup::slotOptionFuncClicked);

    _optFuncList.insert(key, func);
}

OptionWidget *MoreOptPopup::getOptionWidget(const QString &key)
{
    if (_optList.contains(key)) {
        OptionWidget* wid = _optWidList.value(key);
        return wid;
    }

    return nullptr;
}

indexFunc MoreOptPopup::getIndexFunc(const QString &key)
{
    if (_optFuncList.contains(key)) {
        return _optFuncList[key];
    }

    return indexFunc();
}

void MoreOptPopup::setCurrentIndex(const QModelIndex &index)
{
    _currentIndex = index;
}

QModelIndex MoreOptPopup::getCurrentIndex() const
{
    return _currentIndex;
}

bool MoreOptPopup::eventFilter(QObject * obj, QEvent * event)
{
    if (obj == this) {
        if (event->type() == QEvent::WindowDeactivate) {
            this->close();
        }
        if (event->type() == QEvent::Show) {
            QPoint windowPos = QCursor::pos(); //获取屏幕全局坐标
            QWidget* parentWid = this->parentWidget();
            if (parentWid) {
                //将父窗口底部的坐标映射到屏幕的全局坐标
                QPoint parentBottomLeft = parentWid->mapToGlobal(QPoint(0, parentWid->height()));
                int spaceBelow = parentBottomLeft.y() - windowPos.y();
                if (spaceBelow < this->height()) {
                    windowPos.setY(windowPos.y() - this->height());
                }
            } else {
                windowPos.setY(windowPos.y());
            }

            QPropertyAnimation* anim = new QPropertyAnimation(this, "pos");
            anim->setDuration(300);
            anim->setStartValue(QPoint(windowPos.x(), windowPos.y() - 20));
            anim->setEndValue(windowPos);
            anim->setEasingCurve(QEasingCurve::OutCubic);
            anim->start(QAbstractAnimation::DeleteWhenStopped);
        }
    }

    return QDialog::eventFilter(obj, event);
}

void MoreOptPopup::initPopup()
{
    setModal(false);
    setSizeGripEnabled(false);
    setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground, true);
    _w = 150;
    _h = 10;
    resize(_w, _h);
}

void MoreOptPopup::initSetShadow(const QColor& shadowColor, const QColor& backgroundColor)
{
    if (!_shadowInit) {
        _shadow->setOffset(0, 0);
        _shadow->setBlurRadius(20);

        _widget->setGraphicsEffect(_shadow);
        _widget->setObjectName("widgetShadow");

        QSize s = this->size();
        _widget->resize(s);
        this->resize(s.width() + 20, s.height() + 20);

        _widget->move(10,10);
    }

    _shadow->setColor(shadowColor);
    _widget->setStyleSheet(QString("#widgetShadow{background: %1; border-radius: 5px;}")
                               .arg(backgroundColor.name(QColor::HexArgb)));

    _shadowInit = true;
}

void MoreOptPopup::initContent()
{
    _widgetLayout = new QVBoxLayout(_widget);
    _widgetLayout->setContentsMargins(5, 0, 5, 0);
    _widgetLayout->setSpacing(0);

    _widget->setLayout(_widgetLayout);
}

OptionWidget *MoreOptPopup::addOptWid(const QString &name, const QIcon &icon, const QString &key)
{
    OptionWidget* optionWid = new OptionWidget(name, icon, key, _widget);

    _widgetLayout->addWidget(optionWid);

    _h += optionWid->height();
    this->resize(_w, _h);
    _shadowInit = false;

    if (eTheme->getThemeMode() == ElaThemeType::Light) {
        initSetShadow(_shadowColorLight, _lightColor);
    }
    else {
        initSetShadow(_shadowColorDark, _darkColor);
    }

    _optWidList.insert(key, optionWid);

    return optionWid;
}

void MoreOptPopup::slotOptionClicked(const QString &key)
{
    if (_optWidList.contains(key)) {
        QWidget* wid = _optList.value(key);
        if (wid) {
            wid->show();
        }
    }

    emit sigOptionClicked(key);
}

void MoreOptPopup::slotOptionFuncClicked(const QString &key)
{
    if (_optFuncList.contains(key)) {
        _optFuncList[key](_currentIndex);
        this->close();
    }
}

OptionWidget::OptionWidget(const QString &name, const QIcon &icon,
                           const QString& key, QWidget *parent)
    : QWidget(parent)
    , _name(name)
    , _icon(icon)
    , _key(key)
{
    initWidget();

    this->setObjectName("OptionWidget");
    installEventFilter(this);
}

OptionWidget::~OptionWidget()
{

}

void OptionWidget::setFixedColor(QColor color)
{
    _fixedColor = color;
    _optionText->setTextColor(color);
}

bool OptionWidget::eventFilter(QObject * obj, QEvent * event)
{
    if (obj == this) {
        QString enter = "#OptionWidget{background-color: #30808080; border-radius: 5px;}";
        QString lightPress = "#OptionWidget{background-color: #80808080; border-radius: 5px;}";
        QString darkPress = "#OptionWidget{background-color: #10808080; border-radius: 5px;}";

        if (event->type() == QEvent::Enter) {
            setStyleSheet(enter);
        }
        else if (event->type() == QEvent::Leave) {
            setStyleSheet("#OptionWidget{background-color: transparent;}");
        }
        else if (event->type() == QEvent::MouseButtonPress) {
            QMouseEvent* mouseEvent = dynamic_cast<QMouseEvent*>(event);
            if (mouseEvent->button() == Qt::LeftButton) {
                if (eTheme->getThemeMode() == ElaThemeType::Light) {
                    setStyleSheet(lightPress);
                }
                else {
                    setStyleSheet(darkPress);
                }
            }
        }
        else if (event->type() == QEvent::MouseButtonRelease) {
            QMouseEvent* mouseEvent = dynamic_cast<QMouseEvent*>(event);
            if (mouseEvent->button() == Qt::LeftButton) {
                setStyleSheet(enter);
                emit sigOptionClicked(_key);
            }
        }
    }

    return QWidget::eventFilter(obj, event);
}

void OptionWidget::initWidget()
{
    QVBoxLayout* optionLayout = new QVBoxLayout(this);
    _optionText = new IconText(_name, this);
    _optionText->setIcon(_icon);
    _optionText->setSpacing(8);
    _optionText->setPixelSize(13);
    _optionText->setTextColorLight(Qt::black);
    _optionText->setTextColorDark(Qt::white);

    optionLayout->setContentsMargins(5, 0, 0, 0);
    optionLayout->addWidget(_optionText);
    optionLayout->setSpacing(0);

    this->setFixedHeight(_optionText->height() + 10);
    this->setLayout(optionLayout);
    this->setAttribute(Qt::WA_StyledBackground, true);
}
