#include "SettingVoiceWidget.h"

SettingVoiceWidget::SettingVoiceWidget(QWidget *parent) :
    QWidget(parent)
{
    mLayout = new QVBoxLayout;
    mFormLayout = new QFormLayout;
    mButtonGroup = new QButtonGroup;
    QHBoxLayout* buttonLayout = new QHBoxLayout;


    mPushToTalkButton = new QRadioButton("Push to Talk");
    mContinuousTransmissionButton = new QRadioButton("Continuous Transmission");
    mPushToTalkBindButton = new KeyInputButton;

    buttonLayout->addWidget(mPushToTalkButton);
    buttonLayout->addWidget(mPushToTalkBindButton);


    mButtonGroup->addButton(mPushToTalkButton);
    mButtonGroup->addButton(mContinuousTransmissionButton);


    mLayout->addLayout(buttonLayout);
    mLayout->addWidget(mContinuousTransmissionButton);

    setLayout(mLayout);
}
