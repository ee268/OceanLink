#ifndef MOREOPTPOPUP_H
#define MOREOPTPOPUP_H

#include <QDialog>
#include <QGraphicsDropShadowEffect>
#include <QVBoxLayout>
#include <QIcon>
#include <QMap>
#include <QModelIndex>

#include <functional>

#include "icontext.h"

using indexFunc = std::function<void(const QModelIndex&)>;

class OptionWidget : public QWidget {
    Q_OBJECT
public:
    explicit OptionWidget(const QString& name, const QIcon& icon,
                          const QString& key, QWidget* parent = nullptr);
    ~OptionWidget();

    void setFixedColor(QColor color);

protected:
    bool eventFilter(QObject *, QEvent *) override;

private:
    void initWidget();

private:
    QString _name;
    QIcon _icon;
    QString _key;
    QColor _fixedColor;
    IconText* _optionText;

signals:
    void sigOptionClicked(const QString& key);
};

class MoreOptPopup : public QDialog
{
    Q_OBJECT
public:
    explicit MoreOptPopup(QWidget* parent = nullptr);
    ~MoreOptPopup();

    void addOption(const QString& name, const QIcon& icon,
                   const QString& key, QWidget* value);

    void addOption(const QString& name, const QIcon& icon,
                   const QString& key, const indexFunc func);

    OptionWidget* getOptionWidget(const QString& key);

    void setCurrentIndex(const QModelIndex& index);
    QModelIndex getCurrentIndex() const;

    indexFunc getIndexFunc(const QString& key);

protected:
    bool eventFilter(QObject *, QEvent *) override;

private:
    void initPopup();
    void initSetShadow(const QColor& shadowColor, const QColor& backgroundColor);
    void initContent();
    OptionWidget* addOptWid(const QString& name, const QIcon& icon,
                            const QString& key);

private:
    QWidget* _widget;
    QVBoxLayout* _widgetLayout;
    QGraphicsDropShadowEffect* _shadow;
    QColor _shadowColorLight;
    QColor _shadowColorDark;
    QColor _lightColor;
    QColor _darkColor;
    bool _shadowInit;
    int _w;
    int _h;

    //窗口
    QMap<QString, QWidget*> _optList;
    //函数
    QMap<QString, indexFunc> _optFuncList;

    //选项widget
    QMap<QString, OptionWidget*> _optWidList;

    QModelIndex _currentIndex;

signals:
    void sigOptionClicked(const QString& key);

private slots:
    void slotOptionClicked(const QString& key);
    void slotOptionFuncClicked(const QString& key);
};

#endif // MOREOPTPOPUP_H
