#ifndef SETTINGVOICEWIDGET_H
#define SETTINGVOICEWIDGET_H

#include <QWidget>
#include <QRadioButton>
#include <QPushButton>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QButtonGroup>
#include "KeyInputButton.h"

class SettingVoiceWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SettingVoiceWidget(QWidget *parent = 0);

signals:

public slots:

private:
    QVBoxLayout* mLayout;
    QRadioButton* mPushToTalkButton;
    QRadioButton* mContinuousTransmissionButton;
    KeyInputButton* mPushToTalkBindButton;

    QButtonGroup* mButtonGroup;

    QFormLayout* mFormLayout;

};

#endif // SETTINGVOICEWIDGET_H
