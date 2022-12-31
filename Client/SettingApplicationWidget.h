#ifndef SETTINGAPPLICATIONWIDGET_H
#define SETTINGAPPLICATIONWIDGET_H

#include <QWidget>
#include <QFormLayout>
#include <QVBoxLayout>
#include <QComboBox>

class SettingApplicationWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SettingApplicationWidget(QWidget *parent = 0);

signals:

public slots:

private:
    QVBoxLayout* mLayout;
    QFormLayout* mFormLayout;
    QComboBox* mLanguages;

};

#endif // SETTINGAPPLICATIONWIDGET_H
