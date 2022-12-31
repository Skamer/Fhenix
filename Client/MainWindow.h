#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenuBar>

#include "ServerConnectionDialog.h"
#include "ServerWidget.h"
#include "ServerConnection.h"

#include <QIODevice>
#include <QAudioOutput>
#include <QAudioInput>
#include "OpusCodec.h"

#include "AudioMicrophone.h"
#include "AudioSpeakers.h"
#include "SettingWidget.h"

#include <QKeyEvent>
#include <QAbstractNativeEventFilter>

#ifdef Q_OS_WIN
#include <windows.h>
#endif


namespace Fhenix
{

    class MainWindow : public QMainWindow
    {
        Q_OBJECT

    public:
        MainWindow(QWidget *parent = 0);
        ~MainWindow();
        void createActions();

    public slots:
        void nativeKeyPressed(int keyCode, int previousKeyCode);
        void nativeKeyReleased(int keyCode, int previousKeyCode);

    private:
        ServerConnectionDialog* mServerConnectionDialog;
        ServerWidget* mServerWidget;
        ServerConnection* mServerConnection;

        QAudioInput* mAudioInput;
        QAudioOutput* mAudioOutput;

        QIODevice* mInputDevice;
        QIODevice* mOutputDevice;
        OpusCodec* mOpusCodec;

        AudioMicrophone* mMicrophone;
        AudioSpeakers* mSpeakers;

        SettingWidget* mSettingWidget;

    };

}
#endif // MAINWINDOW_H
