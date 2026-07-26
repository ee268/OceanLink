#ifndef BASECREATEOPTION_H
#define BASECREATEOPTION_H

#include <QWidget>

class QVBoxLayout;

class BaseCreateOption : public QWidget
{
public:
    explicit BaseCreateOption(QWidget *parent = nullptr);

    QVBoxLayout* layout() const;

protected:
    QWidget* createOption(const QString& text);

    QWidget* createButtonOption(const QString& text, const QString& btnText, bool isThemeBtn = false);

    QWidget* createComboBoxOption(const QString& text, const QStringList& options);
};

#endif // BASECREATEOPTION_H
