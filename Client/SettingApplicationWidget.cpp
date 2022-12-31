#include "SettingApplicationWidget.h"

SettingApplicationWidget::SettingApplicationWidget(QWidget *parent) :
    QWidget(parent)
{

    mLayout = new QVBoxLayout;
    mFormLayout = new QFormLayout;

    mLanguages = new QComboBox;

    mFormLayout->addRow("Language", mLanguages);

    mLanguages->addItem("English");
    mLanguages->addItem("French");
    mLanguages->addItem("System Default");

    mLayout->addLayout(mFormLayout);

    setLayout(mLayout);
}


