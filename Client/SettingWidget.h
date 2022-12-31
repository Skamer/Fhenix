#ifndef SETTINGWIDGET_H
#define SETTINGWIDGET_H

#include <QDialog>
#include <QListWidget>
#include <QListWidgetItem>
#include <QStackedWidget>
#include <QHBoxLayout>

#include "SettingApplicationWidget.h"
#include "SettingSoundWidget.h"
#include "SettingVoiceWidget.h"

namespace Fhenix
{

 enum OptionCategoryType
 {
     ApplicationCategory,
     VoiceCategory,
     SoundCategory
 };

class SettingWidget : public QDialog
{
    Q_OBJECT
public:
    explicit SettingWidget(QWidget *parent = 0);

    void createApplicationOption();
    void createVoiceOption();
    void createSoundOption();

signals:

public slots:
    void changeCategory(QListWidgetItem* item);

private:
    QListWidget* mCategoryItems;
    QStackedWidget* mContentWidgets;
    QHBoxLayout* mLayoutOption;

    QVBoxLayout* mLayout;

    SettingApplicationWidget* mApplication;
    SettingSoundWidget* mSound;
    SettingVoiceWidget* mVoice;

    QPushButton* mButtonOk;
    QPushButton* mCancelButton;
    QPushButton* mApplyButton;

    QHBoxLayout* mLayoutButton;
};

}
#endif // SETTINGWIDGET_H
