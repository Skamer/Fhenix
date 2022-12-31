#include "SettingWidget.h"

namespace Fhenix
{

SettingWidget::SettingWidget(QWidget *parent) :
    QDialog(parent)
{
    mContentWidgets = new QStackedWidget;
    mCategoryItems =  new QListWidget;
    mLayout = new QVBoxLayout;
    mLayoutOption = new QHBoxLayout;

    mCategoryItems->setIconSize(QSize(48, 48));
    mCategoryItems->setFixedWidth(150);

    createApplicationOption();
    createVoiceOption();
    createSoundOption();

     mLayoutOption->addWidget(mCategoryItems);
     mLayoutOption->addWidget(mContentWidgets);

    mLayoutButton = new QHBoxLayout;
    mButtonOk = new QPushButton("Ok");
    mButtonOk->setFixedSize(75, 25);

    mCancelButton = new QPushButton("Cancel");
    mCancelButton->setFixedSize(75, 25);

    mApplyButton = new QPushButton("Apply");
    mApplyButton->setFixedSize(75, 25);

    mLayoutButton->addWidget(mButtonOk);
    mLayoutButton->addWidget(mCancelButton);
    mLayoutButton->addWidget(mApplyButton);

    mLayout->addLayout(mLayoutOption);
    mLayout->addLayout(mLayoutButton);



    setLayout(mLayout);

    connect(mCategoryItems, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(changeCategory(QListWidgetItem*)));

}


void SettingWidget::createApplicationOption()
{
    QListWidgetItem* applicationCategory = new QListWidgetItem(mCategoryItems, 0);
    mApplication = new SettingApplicationWidget;

    applicationCategory->setText("Application");
    applicationCategory->setIcon(QIcon("fhenix.png"));

    mContentWidgets->addWidget(mApplication);


}

void SettingWidget::createVoiceOption()
{
    QListWidgetItem* voiceCategory = new QListWidgetItem(mCategoryItems, 1);
    mVoice = new SettingVoiceWidget;

    voiceCategory->setText("Voice");

    mContentWidgets->addWidget(mVoice);


}

void SettingWidget::createSoundOption()
{
    QListWidgetItem* soundCategory = new QListWidgetItem(mCategoryItems, 2);
    mSound = new SettingSoundWidget;

    soundCategory->setText("Sound");

    mContentWidgets->addWidget(mSound);
}

void SettingWidget::changeCategory(QListWidgetItem *item)
{
    if(item->type() == mContentWidgets->currentIndex())
        return;

    int categoryID = item->type();

    mContentWidgets->setCurrentIndex(categoryID);
}

}
