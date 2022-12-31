#include "MainWindow.h"
#include "NativeWindowsInput.h"

namespace Fhenix
{
    MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent)
    {

        mServerConnectionDialog = new ServerConnectionDialog(this);
        mServerConnection = new ServerConnection;
        mServerWidget = new ServerWidget(0, mServerConnection);
        mSettingWidget = new SettingWidget(this);
        mInputDevice = nullptr;
        mOutputDevice = nullptr;
        mAudioInput = nullptr;
        mOutputDevice = nullptr;
        mOpusCodec = new OpusCodec;
        mOpusCodec->setChannelCount(2);
        mOpusCodec->setSampleRate(48000);
        mOpusCodec->initialize();

        mMicrophone = new AudioMicrophone(QAudioDeviceInfo::defaultInputDevice(), mOpusCodec, this);
        mSpeakers = new AudioSpeakers(QAudioDeviceInfo::defaultOutputDevice(), mOpusCodec, this);

        mMicrophone->initialize();
        mSpeakers->initialize();

        mSettingWidget->show();

        createActions();


        #ifdef Q_OS_WIN
        connect(NativeWindowsInput::get(), SIGNAL(KeyPressed(int,int)), this, SLOT(nativeKeyPressed(int,int)));
        connect(NativeWindowsInput::get(), SIGNAL(KeyReleased(int,int)), this, SLOT(nativeKeyReleased(int,int)));
        #endif
        connect(mServerConnectionDialog, SIGNAL(ConnectionInfoChanged(QHostAddress,int,QString,QString)), mServerConnection, SLOT(connectToHost(QHostAddress,int,QString,QString)));
        connect(mServerConnection, &ServerConnection::clientIDUpdated, [&]()
        {
            qWarning() << "Audio initialized";
            connect(mMicrophone, SIGNAL(newVoiceData(QByteArray)), mServerConnection, SLOT(sendVoiceData(QByteArray)));
            connect(mServerConnection, SIGNAL(voiceDataReceived(QByteArray)), mSpeakers, SLOT(play(QByteArray)));
        });

        resize(650, 700);

        setCentralWidget(mServerWidget);

        setWindowIcon(QIcon("fhenix_icon.png"));
    }

    MainWindow::~MainWindow()
    {

    }

    void MainWindow::createActions()
    {
        // Create the menu
        QMenu* connectionMenu = menuBar()->addMenu("&Connections");
        QMenu* selfMenu = menuBar()->addMenu("&Self");
        QMenu* settingsMenu = menuBar()->addMenu("Settings");
        QMenu* helpMenu = menuBar()->addMenu("&Help");


        // Connection Action
        QAction* connectAction = new QAction("Connect", this);
        // Quit Action
        QAction* quitAction = new QAction("Quit", this);

        connectionMenu->addAction(connectAction);
        connectionMenu->addSeparator();
        connectionMenu->addAction(quitAction);

        // Self Menu Action
        QAction* muteMic = new QAction("Mute Microphone", this);
        QAction* muteMicAndSound = new QAction("Mute Microphone/Speakers", this);
        muteMic->setCheckable(true);
        muteMicAndSound->setCheckable(true);

        selfMenu->addAction(muteMic);
        selfMenu->addAction(muteMicAndSound);


        connect(connectAction, SIGNAL(triggered()), mServerConnectionDialog, SLOT(show()));
        connect(quitAction, SIGNAL(triggered()), this, SLOT(close()));
    }

    void MainWindow::nativeKeyPressed(int keyCode, int previousKeyCode)
    {
        //qDebug() << "Native Key Pressed";
        if(keyCode == Qt::Key_Z && keyCode != previousKeyCode)
        {
            mMicrophone->setSpeaking(true);
            qDebug() << "Speaking TRUE";
        }
    }

    void MainWindow::nativeKeyReleased(int keyCode, int previousKeyCode)
    {
        //qDebug() << "Native Key Released";
        if(keyCode == Qt::Key_Z)
        {
            mMicrophone->setSpeaking(false);
            qDebug() << "Speaking False";
        }
    }



}


