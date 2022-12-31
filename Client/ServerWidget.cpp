#include "ServerWidget.h"
#include "ServerConnection.h"
#include <QDebug>

namespace Fhenix
{

    ServerWidget::ServerWidget(QWidget *parent, ServerConnection* serverConnection) :
        QWidget(parent)
    {
        mServerConnection = serverConnection;
        mLayout = new QVBoxLayout;
        mServerViewer = new ServerViewerWidget(0, serverConnection);
        mTabTextChat = new QTabWidget;
        mSplitter = new QSplitter;
        mInputTextChat = new QLineEdit;
        mServerTextChat = new TextChatWidget;

        mLayout->addWidget(mSplitter);
        mLayout->addWidget(mInputTextChat);

        mSplitter->addWidget(mServerViewer);
        mSplitter->addWidget(mTabTextChat);
        mSplitter->setOrientation(Qt::Vertical);

        mTabTextChat->addTab(mServerTextChat, "No server");

        setLayout(mLayout);

        // Connect
        connect(mInputTextChat, &QLineEdit::returnPressed,[&](){
            if(mInputTextChat->text() != "")
            {
                QString text = mInputTextChat->text();
                Shared::TargetType targetType;
                int targetID = -1;
                QWidget* currentTextChat = mTabTextChat->widget(mTabTextChat->currentIndex());

                if(currentTextChat == mServerTextChat)
                {
                    targetType = Shared::TargetType::Server;

                }
                else if(currentTextChat == mChannelTextChat)
                {
                    targetType = Shared::TargetType::Channel;
                }
                else
                {
                    targetID = findClientID(qobject_cast<TextChatWidget*>(currentTextChat));
                    targetType = Shared::TargetType::Client;
                }

                emit(clientMessageReady(text, targetType, targetID));


                mInputTextChat->clear();
            }

        });

        if(mServerConnection)
        {
            connect(mServerConnection, SIGNAL(serverInfoReceived(Shared::ServerInfo)), this, SLOT(updateServerInfo(Shared::ServerInfo)));
            connect(mServerConnection, SIGNAL(newClientConnected(Shared::ClientInfo)), this, SLOT(newClient(Shared::ClientInfo)));
            connect(mServerConnection, SIGNAL(clientDisconnected(int)), this, SLOT(removeClient(int)));
            connect(mServerConnection, SIGNAL(channelListReceived(QList<Shared::ChannelInfo>)), this, SLOT(updateChannelList(QList<Shared::ChannelInfo>)));
            connect(mServerConnection, SIGNAL(clientListReceived(QList<Shared::ClientInfo>)), this, SLOT(updateClientList(QList<Shared::ClientInfo>)));
            connect(mServerConnection, SIGNAL(clientIDUpdated(int)), this, SLOT(updateLocalClientID(int)));
            connect(this, SIGNAL(clientMessageReady(QString,Shared::TargetType,int)), mServerConnection, SLOT(sendMessage(QString,Shared::TargetType,int)));
            connect(mServerConnection, SIGNAL(messageReceived(QString,Shared::TargetType,int,int)), this, SLOT(updateMessage(QString,Shared::TargetType,int,int)));
            connect(mServerViewer, SIGNAL(switchChannelRequested(int,QString)), mServerConnection, SLOT(sendSwitchChannelRequest(int,QString)));
            connect(mServerConnection, SIGNAL(clientChannelSwitched(int,int)), this, SLOT(updateClientChannel(int,int)));
            connect(mServerConnection, SIGNAL(clientSpeaking(int)), this, SLOT(updateClientSpeaking(int)));
        }
    }


    int ServerWidget::findClientID(TextChatWidget *clientTextChat)
    {
        if(!clientTextChat)
            return -1;

        for(auto it = mClientsTextChat.constBegin(); it != mClientsTextChat.constEnd(); it++)
        {
            if(it.value() == clientTextChat)
                return it.key();
        }
    }

    void ServerWidget::openClientTextChat(const QString& name, const int id)
    {
        TextChatWidget* clientTabText = nullptr;

        if(mClientsTextChat[id])
            return;

        clientTabText = new TextChatWidget;

        mTabTextChat->addTab(clientTabText, name);
        mClientsTextChat[id] = clientTabText;
    }

    // ********************************************************************
    // *                          SLOT                                    *
    // ********************************************************************


    void ServerWidget::newClient(const Shared::ClientInfo &client)
    {
        mServerViewer->addClient(client.id, client.name, client.channel);

        qDebug() << QString("New client (id : %1 / name : %2 / channel : %3").arg(client.id).arg(client.name).arg(client.channel);

        mServerTextChat->addMessage(
                    QString("%1 connected to channel \"%2\"")
                    .arg(client.name)
                    .arg(mServerViewer->channelName(client.channel)
                         ));
    }

    void ServerWidget::removeClient(const int id)
    {

        QString clientName = mServerViewer->clientName(id);

        qDebug() << QString("Remove client (id : %1 / name : %2").arg(id).arg(clientName);

        mServerTextChat->addMessage(
                    QString("%1 disconnected from the server")
                    .arg(clientName)
                    );

        mServerViewer->removeClient(id);
    }

    void ServerWidget::updateChannelList(const QList<Shared::ChannelInfo> &channelList)
    {
        qDebug() << QString("Update the channel list");

        foreach (Shared::ChannelInfo channel, channelList)
        {
            mServerViewer->addChannel(channel);
        }

    }

    void ServerWidget::updateClientList(const QList<Shared::ClientInfo> &clientList)
    {
       qDebug() << QString("Update the client list");

        foreach (Shared::ClientInfo client, clientList)
        {
            mServerViewer->addClient(client.id, client.name, client.channel);

            if(mServerViewer->clientID() == client.id)
            {
                if(mTabTextChat->count() >= 2)
                    mTabTextChat->setTabText(1, mServerViewer->channelName(client.channel));
                else
                {
                    mChannelTextChat = new TextChatWidget;
                    mTabTextChat->addTab(mChannelTextChat, mServerViewer->channelName(client.channel));
                }

            }
        }


    }

    void ServerWidget::updateLocalClientID(const int id)
    {
        qDebug() << QString("The local client id is %1").arg(id);

         mServerViewer->setClientID(id);
    }

    void ServerWidget::updateMessage(const QString& msg, const Shared::TargetType targetType, const int targetID, const int senderID)
    {
        qDebug() << QString("Message received (msg : \"%1\" / targetType : %2 / targetID : %3 / senderID : %4").arg(msg).arg(targetType).arg(targetID).arg(senderID);

        if(targetType == Shared::TargetType::Server)
        {
            mServerTextChat->addMessage(msg);
        }
        else if(targetType == Shared::TargetType::Channel)
        {
            mChannelTextChat->addMessage(msg);
        }
        else if(targetType == Shared::TargetType::Client)
        {
            TextChatWidget* clientTextChat = mClientsTextChat[senderID];

            if(!clientTextChat)
                return;

            clientTextChat->addMessage(msg);

        }
    }

    void ServerWidget::updateServerInfo(const Shared::ServerInfo &serverInfo)
    {
        qDebug() << QString("Update Server Info (name : %1 / defaultChannel : %2").arg(serverInfo.name).arg(serverInfo.defaultChannel);

        mServerViewer->setServerName(serverInfo.name);
        mDefaultChannel = serverInfo.defaultChannel;

        mTabTextChat->setTabText(0, serverInfo.name);
    }

    void ServerWidget::updateClientChannel(const int id, const int channelID)
    {

        qDebug() << QString("Move client to channel (client id : %1 / target channel id : %2").arg(id).arg(channelID);

        mServerViewer->moveClient(id, channelID);

        mServerTextChat->addMessage(
                    QString("%1 switched to the channel \"%2\"")
                    .arg(mServerViewer->clientName(id))
                    .arg(mServerViewer->channelName(channelID))
                    );

    }

    void ServerWidget::updateClientSpeaking(const int id)
    {
       mServerViewer->clientSpeaking(id);
    }


}
